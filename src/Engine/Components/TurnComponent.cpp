
#include "Math/Vector2.h"
#include "Engine/Entity.h"
#include "Engine/Engine.h"
#include "Engine/Clock/Clock.h"

#include "TurnComponent.h"


// component interface ////////////////////////////////////////////////////////

TurnComponent::TurnComponent() 
	: m_bTurning(false)
{
	m_bTurning = false;
}

TurnComponent::~TurnComponent()
{

}

void TurnComponent::Turn( float in_fNewAngle, float in_fDelayToDest )
{
	m_fOAngle = GetEntity()->GetAngle();
	m_fDAngle = (float)DEG_TO_RAD(in_fNewAngle);
	m_fOTime = GetEngine()->GetClock()->GetTotalTime();
	m_fDuration = in_fDelayToDest;
	m_fDTime = m_fOTime + m_fDuration;
	m_bTurning = true;
}

void TurnComponent::Abort()
{
	m_bTurning = false;
}

// engine connection //////////////////////////////////////////////////////////

void TurnComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	in_pEngine->Connect_OnUpdate( this, &TurnComponent::OnUpdate );

	GetEntity()->GetLuaContext().registerFunction( "turn", &TurnComponent::Turn );
	GetEntity()->GetLuaContext().registerFunction( "abort", &TurnComponent::Abort );

	// callbacks
	BindCallback( "OnTurnComponent_AngleReached", m_cbOnAngleReached );
	BindCallback( "OnTurnComponent_Turning", m_cbOnTurning );
}

void TurnComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
	in_pEngine->Disconnect_OnUpdate( this );

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void TurnComponent::OnUpdate( float in_fDeltaTime )
{
	if( m_bTurning )
	{
		float tNow = GetEngine()->GetClock()->GetTotalTime();
		if( tNow > m_fDTime )
		{
			// reached destination
			GetEntity()->GetAngle() = m_fDAngle;
			m_bTurning = false;

			if( m_cbOnAngleReached.IsEnabled() )
				GetEntity()->GetLuaContext().callLuaFunction<void>( m_cbOnAngleReached.GetName() );
		}
		else
		{
			float fRatio = SMOOTH_STEP( m_fOTime, m_fDTime, tNow );

			GetEntity()->GetAngle() = LERP( m_fOAngle, m_fDAngle, fRatio );

			if( m_cbOnTurning.IsEnabled() )
				GetEntity()->GetLuaContext().callLuaFunction<void>( m_cbOnTurning.GetName(), fRatio );
		}
	}
}
