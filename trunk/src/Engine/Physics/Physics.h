#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <Box2D/Box2D.h>

#include <vector>

// 8 pixels is 2 meters
#define PixelToPhysics( a ) ((a)/8)

class World
{
public:
	World() : m_fHz(60.0f), m_nVelocityIterations(8), m_nPositionIterations(3), m_bEnableWarmStarting(true), m_bEnableContinuous(true)
	{

	}

	void Init( const b2Vec2& in_v2Gravity, bool in_bAllowSleep )
	{
		m_pWorld = new b2World( in_v2Gravity, in_bAllowSleep );

		m_pContactListener = new ContactListener(m_pWorld);
		m_pContactFilter = new ContactFilter(m_pWorld);
		m_pDestructionListener = new DestructionListener(m_pWorld);
			
		m_pWorld->SetContactListener( m_pContactListener );
		m_pWorld->SetContactFilter( m_pContactFilter );
		m_pWorld->SetDestructionListener( m_pDestructionListener );
	}

	void Exit()
	{
		delete m_pWorld; m_pWorld = 0;
		delete m_pContactListener; m_pContactListener = 0;
		delete m_pContactFilter; m_pContactFilter = 0;
		delete m_pDestructionListener; m_pDestructionListener = 0;
	}

	void Update()
	{
		float32 timeStep = m_fHz > 0.0f ? 1.0f / m_fHz : float32(0.0f);
				
		m_pWorld->SetWarmStarting(m_bEnableWarmStarting);
		m_pWorld->SetContinuousPhysics(m_bEnableContinuous);

		m_pWorld->Step(timeStep, m_nVelocityIterations, m_nPositionIterations);
	}

	b2World*	GetWorld() { return m_pWorld; }

private:

	class ContactListener : public b2ContactListener
	{
	public:
		ContactListener( b2World* in_pWorld ) : m_pWorld(in_pWorld) {}

		void BeginContact(b2Contact* contact)
		{
			// This is called when two fixtures begin to overlap. This is called for sensors and non-sensors. 
			// This event can only occur inside the time step.
		}
		void EndContact(b2Contact* contact)
		{
			// This is called when two fixtures cease to overlap. This is called for sensors and non-sensors.
			// This may be called when a body is destroyed, so this event can occur outside the time step.
		}
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
		{
			// This is called after collision detection, but before collision resolution. 
			// This gives you a chance to disable the contact based on the current configuration.
		}
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
		{
			// The post solve event is where you can gather collision impulse results.
		}

	private:
		b2World*	m_pWorld;
	};

	class ContactFilter : public b2ContactFilter
	{
	public:
		ContactFilter( b2World* in_pWorld ) : m_pWorld(in_pWorld) {}

		bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
		{
			const b2Filter& filterA = fixtureA->GetFilterData();
			const b2Filter& filterB = fixtureB->GetFilterData();

			if (filterA.groupIndex == filterB.groupIndex && filterA.groupIndex != 0)
			{
				return filterA.groupIndex > 0;
			}

			bool collide = (filterA.maskBits & filterB.categoryBits) != 0 && (filterA.categoryBits & filterB.maskBits) != 0;
			return collide;
		}
	private:
		b2World* m_pWorld;
	};
	

	// This is called when a joint in the world is implicitly destroyed
	// because an attached body is destroyed. This gives us a chance to
	// nullify the mouse joint.
	class DestructionListener : public b2DestructionListener
	{
	public:
		DestructionListener( b2World* in_pWorld ) : m_pWorld(in_pWorld) {}
		void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
		void SayGoodbye(b2Joint* joint);

	private:
		b2World*	m_pWorld;
	};
		
	class QueryCallback : public b2QueryCallback
	{
	public:
		QueryCallback( b2World* in_pWorld, unsigned int in_nReserve ) : m_pWorld(in_pWorld), m_bQueryAll(false)
		{
			m_vecResult.reserve(in_nReserve);
		}

		void Query( const b2Vec2& lowerBound, const b2Vec2& upperBound, bool in_bQueryAll = false  )
		{
			m_bQueryAll = in_bQueryAll;
			m_vecResult.clear();
			m_aabb.lowerBound = lowerBound;
			m_aabb.upperBound = upperBound;
			m_pWorld->QueryAABB( this, m_aabb );
		}

		std::vector<b2Fixture*>& GetResult() { return m_vecResult; }

	protected:
		bool ReportFixture(b2Fixture* fixture)
		{
			m_vecResult.push_back(fixture);
			return m_bQueryAll; // Return true to continue the query.
		}

	private:
		b2AABB	m_aabb;
		bool	m_bQueryAll;
		std::vector<b2Fixture*>	m_vecResult;
		b2World*	m_pWorld;
	};
	
	// This class captures the closest hit shape.
	class RayCastCallback : public b2RayCastCallback
	{
	public:
		class Mode { public: enum E { eAny, eClosest, eAll, eMax }; };

		RayCastCallback( b2World* in_pWorld, unsigned int in_nReserve ) : m_pWorld(in_pWorld)
		{
//			m_fixture = NULL;
			m_pfnProcessModes[Mode::eAny] = &RayCastCallback::ProcessFixtureAny;
			m_pfnProcessModes[Mode::eClosest] = &RayCastCallback::ProcessFixtureClosest;
			m_pfnProcessModes[Mode::eAll] = &RayCastCallback::ProcessFixtureAll;
		}

		void Raycast( const b2Vec2& point1, const b2Vec2& point2, Mode::E mode )
		{
			m_pfnProcess = m_pfnProcessModes[mode];
			m_vecResult.clear();
			m_pWorld->RayCast( this, point1, point2);
		}

		void Raycast( const b2Vec2& origin, const b2Vec2& direction, float distance,  Mode::E mode )
		{
			m_pfnProcess = m_pfnProcessModes[mode];
			m_vecResult.clear();
			m_pWorld->RayCast( this, origin, origin + (distance * direction) );
		}
		
		class RaycastResult
		{
		public:
			b2Fixture* m_fixture;
			b2Vec2 m_point;
			b2Vec2 m_normal;
			float32 m_fraction;
		};
		
	protected:
		float32 ProcessFixtureAny(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
		{
			return 0.0f;
		}

		float32 ProcessFixtureClosest(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
		{
			// keep current result

			return fraction;
		}

		float32 ProcessFixtureAll(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
		{
			return 1.0f;
		}
		
		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) 
			//You cannot make any assumptions about the order of the callbacks.
			// You control the continuation of the ray cast by returning a fraction. Returning a fraction of zero indicates
			// the ray cast should be terminated. A fraction of one indicates the ray cast should continue as if no hit
			// occurred. If you return the fraction from the argument list, the ray will be clipped to the current
			// intersection point. So you can ray cast any shape, ray cast all shapes, or ray cast the closest shape by
			// returning the appropriate fraction.
			// You may also return of fraction of -1 to filter the fixture. Then the ray cast will proceed as if the fixture
			// does not exist.
		{
			/*
			m_vecResult

			m_fixture = fixture;
			m_point = point;
			m_normal = normal;
			m_fraction = fraction;
			*/
			return fraction;	// returning 0 stop the cast, returning 1 continues
		}
		
	private:
		typedef float32 (RayCastCallback::*ProcessMethod)(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction); 

		ProcessMethod m_pfnProcessModes[Mode::eMax];
		ProcessMethod m_pfnProcess;

		b2World*	m_pWorld;
		std::vector<RaycastResult>	m_vecResult;
	};
			

private:
	b2World*				m_pWorld;
	ContactListener*		m_pContactListener;
	ContactFilter*			m_pContactFilter;
	DestructionListener*	m_pDestructionListener;

	float m_fHz;
	int m_nVelocityIterations;
	int m_nPositionIterations;
	bool m_bEnableWarmStarting;
	bool m_bEnableContinuous;
};

#endif//_PHYSICS_H

