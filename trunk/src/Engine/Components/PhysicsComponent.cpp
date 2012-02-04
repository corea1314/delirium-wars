
#include "Math/Vector2.h"
#include "Engine/Entity.h"
#include "Engine/Engine.h"

#include "PhysicsComponent.h"

#include "Lair/Lair.h"
#include "Engine/Physics/World.h"

// component interface ////////////////////////////////////////////////////////

PhysicsComponent::PhysicsComponent() : m_pBody(0)
{
}

PhysicsComponent::~PhysicsComponent()
{

}

// engine connection //////////////////////////////////////////////////////////

void PhysicsComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	in_pEngine->Connect_OnPostUpdate( this, &PhysicsComponent::OnPostUpdate );

	GetEntity()->GetLuaContext().registerFunction( "create_body", &PhysicsComponent::CreateBody );
	GetEntity()->GetLuaContext().registerFunction( "destroy_body", &PhysicsComponent::DestroyBody );
	GetEntity()->GetLuaContext().registerFunction( "warp", &PhysicsComponent::Warp );
	
	BindCallback( "OnPhysicsComponent_Callback", m_cbOnCallback );
}

void PhysicsComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
	if( m_pBody )
	{
		DestroyBody();
	}

	in_pEngine->Disconnect_OnPostUpdate( this );

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void PhysicsComponent::OnCallback()
{
	if( m_cbOnCallback.IsEnabled() )
		GetEntity()->GetLuaContext().callLuaFunction<void>(m_cbOnCallback.GetName() );
}

// //////////////////////////////////

void PhysicsComponent::OnPostUpdate()
{
	if( m_pBody )
	{
		GetEntity()->GetPos().x = m_pBody->GetPosition().x;
		GetEntity()->GetPos().y = m_pBody->GetPosition().y;
		GetEntity()->GetAngle() = m_pBody->GetAngle();
	}	
}

void PhysicsComponent::CreateBody( std::string inBodyDefFilename, bool inStartActive )
{
	if( m_pBody )
		DestroyBody();

	m_pBody = GetEngine()->GetWorld()->CreateBody( GetEntity(), inBodyDefFilename, GetEntity()->GetPos(), inStartActive );
}

void PhysicsComponent::DestroyBody()
{
	if( m_pBody )
	{
		GetEngine()->GetWorld()->DestroyBody( &m_pBody );
	}
}

void PhysicsComponent::Warp( float inX, float inY, float inAngle )
{
	m_pBody->SetTransform( b2Vec2(inX,inY), DEG_TO_RAD(inAngle) );
}