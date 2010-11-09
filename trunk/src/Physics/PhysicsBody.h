#ifndef _PHYSICS_BODY_H__
#define _PHYSICS_BODY_H__

#include "../Math/Vector2.h"
#include <string>
#include "../Game/SigSlot.h"

typedef int CollisionMask;

class CPhysicsBody;

class CCollisionBodyOnBodyResult 
{
	CPhysicsBody* m_pListenerBody;
	CPhysicsBody* m_pForeignBody;

	public :

		CCollisionBodyOnBodyResult()
			:m_pListenerBody(NULL),
			m_pForeignBody(NULL)
		{}
		~CCollisionBodyOnBodyResult(){}

		void SetListenerPhysicsBody( CPhysicsBody* in_pListenerPhysicsBody) { m_pListenerBody = in_pListenerPhysicsBody; }
		CPhysicsBody* GetListenerPhysicsBody() const { return m_pListenerBody; }

		void SetForeignPhysicsBody( CPhysicsBody* in_pForeignPhysicsBody) { m_pForeignBody = in_pForeignPhysicsBody; }
		CPhysicsBody* GetForeignPhysicsBody() const{ return m_pForeignBody; }
};

class CCollisionBodyOnGridResult
{
	//Normal always goes away from the listening body
	CPhysicsBody* m_pListenerBody;

    //Sub grid portion that collides
    unsigned int m_nFromX;
    unsigned int m_nToX;
    unsigned int m_nFromY;
    unsigned int m_nToY;

public :

	CCollisionBodyOnGridResult()
    :m_pListenerBody(NULL),
    m_nFromX(0),
    m_nFromY(0),
    m_nToX(0),
    m_nToY(0)
    {}
    ~CCollisionBodyOnGridResult(){}
        
	void SetPhysicsBody( CPhysicsBody* in_pListenerPhysicsBody) { m_pListenerBody = in_pListenerPhysicsBody; }
	CPhysicsBody* GetPhysicsBody() const { return m_pListenerBody; }

    unsigned int GetFromX() const { return m_nFromX; }
    unsigned int& GetFromX() { return m_nFromX; }
    void SetFromX(unsigned int val) { m_nFromX = val; }

    unsigned int GetToX() const { return m_nToX; }
    unsigned int& GetToX() { return m_nToX; }
    void SetToX(unsigned int val) { m_nToX = val; }

    unsigned int GetFromY() const { return m_nFromY; }
    unsigned int& GetFromY() { return m_nFromY; }
    void SetFromY(unsigned int val) { m_nFromY = val; }

    unsigned int GetToY() const { return m_nToY; }
    unsigned int& GetToY() { return m_nToY; }
    void SetToY(unsigned int val) { m_nToY = val; }
};

//This is meant to implements all the physics and collision related stuff: applying forces, enable/disable collision response, mask, and all that good jazz.
//Simple system with no torque for now
class CPhysicsBody
{
private :
	std::string m_strName;
	CollisionMask m_nMyMask;
	CollisionMask m_nCollideWithMask;
	Vector2 m_v2Shape;
	//Internal optimization
	Vector2 m_v2HalfShape;

	Vector2 m_v2ForceAccumulator;
	Vector2 m_v2ImpulseAccumulator;

	Vector2 m_v2Gravity;
	Vector2 m_v2Velocity;
	Vector2 m_v2PhysicsPosition;
	//Use this to sync your visual. Will be much smoother.
	Vector2 m_v2InterpolatedPosition;

	float m_fMass;
	float m_fInverseMass;

	bool m_bIsSimulationEnable;
	bool m_bIsCollisionEnable;
	bool m_bIsActive;
    bool m_bCollideWithGrid;

public :
	CPhysicsBody( const char * in_szName );
	~CPhysicsBody();

	//////////////////////////////////////
	//Callbacks
	//////////////////////////////////////
	//Last parameter for all callbacks is the calling body
	//You can change any physics property while the callbacks are called. Be aware of what you are doing :-). body activation and removal will take effect after an update.
	//Do not destroy a body while in the update. You can de-activate it though.
	//These callbacks will be called in this order for each substep
	//Apply custom force every substep here. Every force beside gravity should be put in there. Also, put velocity caps and other stuff here.
	NEW_SIGNAL1( PhysicsBodyOnPreSubStep, CPhysicsBody*);

	NEW_SIGNAL1( PhysicsBodyOnPreSync, CPhysicsBody*);

	//Have OnFirstCollision and OnLastCollision at some point would be great. We need the physics manager to cache collision in order to do that
	//You can specify if you want to listen to a specific body via the collision listener
	//User can filter on his own if he wants to listen a specific pair of bodies
	NEW_SIGNAL2( PhysicsBodyOnBodyCollision, CCollisionBodyOnBodyResult*, CPhysicsBody* );
	NEW_SIGNAL2( PhysicsBodyOnGridCollision, CCollisionBodyOnGridResult*, CPhysicsBody* );

	//Post substep stuff
	NEW_SIGNAL1( PhysicsBodyOnPostSync, CPhysicsBody*);
	NEW_SIGNAL1( PhysicsBodyOnPostSubStep, CPhysicsBody*);

	public :

	//////////////////////////////////////
	//Collision
	//////////////////////////////////////
	void SetMyMask( CollisionMask in_nMyMask ) { m_nMyMask = in_nMyMask; }
	CollisionMask GetMyMask() const { return m_nMyMask; }

	void SetCollideWitMask( CollisionMask in_nCollideWithMask ) { m_nCollideWithMask = in_nCollideWithMask; }
	CollisionMask GetCollideWithMask() const { return m_nCollideWithMask; }

	//Shape is centered around the position
	void SetShape( const Vector2 in_v2AABDefinition){ m_v2Shape = in_v2AABDefinition; m_v2HalfShape = in_v2AABDefinition * 0.5f; }
	Vector2 GetShape() const { return m_v2Shape; }
	Vector2 GetHalfShape() const { return m_v2HalfShape; }

	//////////////////////////////////////
	//Physics
	//////////////////////////////////////
	//Forces will be applied for a single physics substep. It will be resetted at the end of the substep so it does not add.
	//****You should use it in the OnSubStepApplyForce callback only****
	void SetForce( const Vector2 & in_v2Force ) { m_v2ForceAccumulator = in_v2Force; }
	void AddForce( const Vector2 & in_v2Force ) { m_v2ForceAccumulator += in_v2Force; }
	Vector2 GetForce() const{return m_v2ForceAccumulator; }
	void ResetAccumulator(){ SetForce(Vector2(0,0)); SetImpulse(Vector2(0,0));}

	//Good only for a single push
	//Putting negative mass means infinite mass
	void SetMass( float in_fMass);
	void SetInfiniteMass(){ m_fMass = m_fInverseMass = 0.0f;}
	float GetMass() const { return m_fMass; }
	float GetInverseMass() const{ return m_fInverseMass; }

	void SetImpulse( const Vector2 in_v2Impulse ) { m_v2ImpulseAccumulator = in_v2Impulse; }
	void AddImpulse( const Vector2 in_v2Impulse )  { m_v2ImpulseAccumulator += in_v2Impulse; }
	Vector2 GetImpulse() const { return m_v2ImpulseAccumulator; }

	//This allows to apply constant force wihtout custom force callback.
	void SetGravity( const Vector2 in_v2Gravity ) { m_v2Gravity = in_v2Gravity; }
	Vector2 GetGravity()const { return m_v2Gravity; }

	void SetVelocity( const Vector2 & in_v2Velocity ) { m_v2Velocity = in_v2Velocity; }
	Vector2 GetVelocity() const { return m_v2Velocity; }

	void SetPhysicsPosition( const Vector2 & in_v2WorldPhysicsPosition ) { m_v2PhysicsPosition = in_v2WorldPhysicsPosition; }
	void TranslatePhysicsPosition( const Vector2 & in_v2PhysicsTranslation ) { m_v2PhysicsPosition += in_v2PhysicsTranslation; }
	Vector2 GetPhysicsPosition() const { return m_v2PhysicsPosition; }

	void SetInterpolatedPosition( const Vector2 & in_v2InterpolatedPosition ) { m_v2InterpolatedPosition = in_v2InterpolatedPosition; }
	Vector2 GetInterpolatedPosition() const { return m_v2InterpolatedPosition; }

	//////////////////////////////////////
	//Setting
	//////////////////////////////////////
	//Should the object be simulated with forces, impulse, velocities and so on?
	//true by default
	void SetSimulationEnable( bool in_bIsSimulationEnabled ){ m_bIsSimulationEnable = in_bIsSimulationEnabled; }
	bool IsSimulationEnable() const{ return m_bIsSimulationEnable; }

	//Should the object try to collide with other bodies?
	void SetCollisionEnable( bool in_bCollisionEnable){ m_bIsCollisionEnable = in_bCollisionEnable; }
	bool IsCollisionEnable() const{ return m_bIsCollisionEnable; }

	void SetActive( bool in_bIsActive) { m_bIsActive = in_bIsActive; }
	bool IsActive() const { return m_bIsActive; }

    void SetCollideWithGrid(bool val) { m_bCollideWithGrid = val; }
    bool IsCollideWithGrid() const { return m_bCollideWithGrid; }
    
	std::string GetName() const {return m_strName; }
};

#endif //_PHYSICS_BODY_H__