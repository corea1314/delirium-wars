
#include "Math/Vector2.h"
#include "Engine/Entity.h"
#include "Engine/Engine.h"

#include "EngineComponent.h"

#include "Lair/Lair.h"

// component interface ////////////////////////////////////////////////////////

EngineComponent::EngineComponent()
{
}

EngineComponent::~EngineComponent()
{

}

// component interface //////////////////////////////////////////////////////////////////

void EngineComponent::CreateEntity( std::string in_szEntityName, std::string in_szLuaScript, float in_fPosX, float in_fPosY, float in_fAngle )
{
	GetEngine()->GetEntity( in_szEntityName, in_szLuaScript, Vector2(in_fPosX,in_fPosY), in_fAngle );
}

// engine connection //////////////////////////////////////////////////////////

void EngineComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	in_pEngine->Connect_OnUpdate( this, &EngineComponent::OnUpdate );

	GetEntity()->GetLuaContext().registerFunction( "create_entity", &EngineComponent::CreateEntity );
	
	BindCallback( "OnEngineComponent_Update", m_cbOnUpdate );
}

void EngineComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
	in_pEngine->Disconnect_OnUpdate( this );

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void EngineComponent::OnUpdate( float in_fDeltaTime )
{
	if( m_cbOnUpdate.IsEnabled() )
		GetEntity()->GetLuaContext().callLuaFunction<void>(m_cbOnUpdate.GetName());
}
