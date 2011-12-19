
#include "Math/Vector2.h"
#include "Engine/Entity.h"
#include "Engine/Engine.h"
#include "Engine/Clock/Clock.h"

#include "Lair/Lair.h"

#include "CameraComponent.h"

// component interface ////////////////////////////////////////////////////////

CameraComponent::CameraComponent()
	: m_bZooming(false)
{
	m_bZooming = false;
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Zoom( float in_fNewZoom, float in_fDelayToDest )
{
	m_vOZoom = m_pCamera->GetZoom();
	m_vDZoom = in_fNewZoom;
	m_fOTime = GetEngine()->GetClock()->GetTotalTime();
	m_fDuration = in_fDelayToDest;
	m_fDTime = m_fOTime + m_fDuration;
	m_bZooming = true;
}

void CameraComponent::Abort()
{
	m_bZooming = false;
}

float CameraComponent::GetZoom() 
{ 
	return m_pCamera->GetZoom(); 
}

void CameraComponent::SetZoom( float in_fZoom ) 
{ 
	m_pCamera->GetZoom() = in_fZoom; 
}

void CameraComponent::Bind() 
{ 
	m_pCamera->SetEntity(GetEntity());
}

bool CameraComponent::BindTo( std::string in_EntityName ) 
{ 
	CEntity* pEntity = GetEngine()->GetEntity(in_EntityName);

	if( pEntity != nullptr )
	{
		m_pCamera->SetEntity( pEntity );
		return true;
	}
	// FIXME todo
	// else
	//	Lair::GetLogMan()->Log( "CameraComponent", "Trying to bind camera named %s to unknown entity %s in component associated to entity named %s",   )

	return false;
}

// engine connection //////////////////////////////////////////////////////////

void CameraComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	m_pCamera = Lair::GetCameraMan()->GetDefaultCamera();

	in_pEngine->Connect_OnUpdate( this, &CameraComponent::OnUpdate );

	GetEntity()->GetLuaContext().registerFunction( "zoom", &CameraComponent::Zoom );
	GetEntity()->GetLuaContext().registerFunction( "abort", &CameraComponent::Abort );
	GetEntity()->GetLuaContext().registerFunction( "get_zoom", &CameraComponent::GetZoom );
	GetEntity()->GetLuaContext().registerFunction( "set_zoom", &CameraComponent::SetZoom );
	GetEntity()->GetLuaContext().registerFunction( "bind", &CameraComponent::Bind );
	GetEntity()->GetLuaContext().registerFunction( "bind_to", &CameraComponent::BindTo );

	BindCallback( "OnCameraComponent_ZoomReached", m_cbOnDestinationReached );
	BindCallback( "OnCameraComponent_Zooming", m_cbOnZooming );
}

void CameraComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
	in_pEngine->Disconnect_OnUpdate( this );

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void CameraComponent::OnUpdate( float in_fDeltaTime )
{
	if( m_bZooming )
	{
		float tNow = GetEngine()->GetClock()->GetTotalTime();
		if( tNow > m_fDTime )
		{
			// reached destination
			m_pCamera->GetZoom() = m_vDZoom;
			m_bZooming = false;

			if( m_cbOnDestinationReached.IsEnabled() )
				GetEntity()->GetLuaContext().callLuaFunction<void>(m_cbOnDestinationReached.GetName());
		}
		else
		{
			float fRatio = SMOOTH_STEP( m_fOTime, m_fDTime, tNow );

			m_pCamera->GetZoom() = LERP( m_vOZoom, m_vDZoom, fRatio );

			if( m_cbOnZooming.IsEnabled() )
				GetEntity()->GetLuaContext().callLuaFunction<void>( m_cbOnZooming.GetName(), fRatio );
		}
	}
}
