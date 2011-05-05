
#include "World.h"

#include <Box2D/Box2D.h>
#include <Engine/Physics/BodyDef.h>

#include "Lair/Lair.h"
#include "Engine/Engine.h"

IMPLEMENT_CLASS_TYPE(CWorld)

CWorld::CWorld() : m_fHz(60.0f), m_nVelocityIterations(8), m_nPositionIterations(3), m_bEnableWarmStarting(true), m_bEnableContinuous(true), 
	m_pWorld(0), m_pContactListener(0), m_pContactFilter(0), m_pDestructionListener(0), m_pBodyDefMan(0)
{
}

void CWorld::Connect( CEngine* in_pEngine )
{
	Init( b2Vec2(0,-9.8f), true );
	in_pEngine->Connect_OnUpdate( this, &CWorld::Update );
}

void CWorld::Disconnect( CEngine* in_pEngine )
{
	in_pEngine->Disconnect_OnUpdate( this );
	Exit();
}

void CWorld::Init( const b2Vec2& in_v2Gravity, bool in_bAllowSleep )
{
	m_pWorld = new b2World( in_v2Gravity, in_bAllowSleep );

	m_pContactListener = new ContactListener(m_pWorld);
	m_pContactFilter = new ContactFilter(m_pWorld);
	m_pDestructionListener = new DestructionListener(m_pWorld);

	m_pWorld->SetContactListener( m_pContactListener );
	m_pWorld->SetContactFilter( m_pContactFilter );
	m_pWorld->SetDestructionListener( m_pDestructionListener );

	m_pBodyDefMan = new BodyDefinitionMan;
}

void  CWorld::Exit()
{
	delete m_pBodyDefMan; m_pBodyDefMan = 0;
	delete m_pWorld; m_pWorld = 0;
	delete m_pContactListener; m_pContactListener = 0;
	delete m_pContactFilter; m_pContactFilter = 0;
	delete m_pDestructionListener; m_pDestructionListener = 0;
}

void  CWorld::Update( float in_fDeltaTime )
{
	float32 timeStep = m_fHz > 0.0f ? 1.0f / m_fHz : float32(0.0f);

	m_pWorld->SetWarmStarting(m_bEnableWarmStarting);
	m_pWorld->SetContinuousPhysics(m_bEnableContinuous);

	m_pWorld->Step(timeStep, m_nVelocityIterations, m_nPositionIterations);
}

b2Body* CWorld::CreateBody( CEntityPhysics* in_pEntity, const std::string& in_szBodyDefinitionFilename, const b2Vec2& in_vPos, bool in_bActive )
{
	BodyDefinition* pBodyDef = m_pBodyDefMan->Get( in_szBodyDefinitionFilename );
	if( pBodyDef )
	{
		pBodyDef->GetBody().position = in_vPos;
		pBodyDef->GetBody().active = in_bActive;
		b2Body* pBody = m_pWorld->CreateBody( &pBodyDef->GetBody() );

		pBody->SetUserData( in_pEntity );
		
		std::vector<b2FixtureDef>::iterator itEnd = pBodyDef->GetFixtures().end();

		for( std::vector<b2FixtureDef>::iterator it = pBodyDef->GetFixtures().begin(); it != itEnd; it++ )
		{
			pBody->CreateFixture( &(*it) );
		}	
	}

	return 0;	
}

void CWorld::DestroyBody( b2Body** in_ppBody )
{
	m_pWorld->DestroyBody( *in_ppBody );
	*in_ppBody = 0;
}