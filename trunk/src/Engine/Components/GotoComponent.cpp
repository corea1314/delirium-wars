
#include "Math/Vector2.h"
#include "Engine/Entity.h"
#include "Engine/Engine.h"
#include "Engine/Entities/Clock/Clock.h"

#include "GotoComponent.h"

// component interface ////////////////////////////////////////////////////////

GotoComponent::GotoComponent()
	: m_bMoving(false)
{
	m_bMoving = false;
}

GotoComponent::~GotoComponent()
{

}

void GotoComponent::Goto( float in_fNewPosX, float in_fNewPosY, float in_fDelayToDest )
{
	m_vOPos = GetEntity()->GetPos();
	m_vDPos.x = in_fNewPosX;
	m_vDPos.y = in_fNewPosY;
	m_fOTime = GetEngine()->GetClock()->GetTotalTime();
	m_fDuration = in_fDelayToDest;
	m_fDTime = m_fOTime + m_fDuration;
	m_bMoving = true;
}

void GotoComponent::Abort()
{
	m_bMoving = false;
}

// engine connection //////////////////////////////////////////////////////////

void GotoComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	in_pEngine->Connect_OnUpdate( this, &GotoComponent::OnUpdate );

	GetEntity()->GetLuaContext().registerFunction( "goto", &GotoComponent::Goto );
	GetEntity()->GetLuaContext().registerFunction( "abort", &GotoComponent::Abort );

	BindCallback( "OnGotoComponent_DestReached", m_cbDestReached );
	BindCallback( "OnGotoComponent_Moving", m_cbMoving );
}

void GotoComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
	in_pEngine->Disconnect_OnUpdate( this );

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void GotoComponent::OnUpdate( float in_fDeltaTime )
{
	if( m_bMoving )
	{
		float tNow = GetEngine()->GetClock()->GetTotalTime();
		if( tNow > m_fDTime )
		{
			// reached destination
			GetEntity()->GetPos() = m_vDPos;
			m_bMoving = false;

			if( m_cbDestReached.IsEnabled() )
				GetEntity()->GetLuaContext().callLuaFunction<void>(m_cbDestReached.GetName());
		}
		else
		{
			float fRatio = SMOOTH_STEP( m_fOTime, m_fDTime, tNow );

			GetEntity()->GetPos().x = LERP( m_vOPos.x, m_vDPos.x, fRatio );
			GetEntity()->GetPos().y = LERP( m_vOPos.y, m_vDPos.y, fRatio );

			if( m_cbMoving.IsEnabled() )
				GetEntity()->GetLuaContext().callLuaFunction<void>( m_cbMoving.GetName(), fRatio );
		}
	}
}
