
#include "Math/Vector2.h"
#include "Engine/Entity.h"
#include "Engine/Engine.h"
#include "Engine/Entities/Clock/Clock.h"

#include "GotoComponent.h"

GotoComponent::GotoComponent()
{
	m_MovementData.bMoving = false;
	m_vCurrPos.Set(0,0);
}

void GotoComponent::Goto( float in_fNewPosX, float in_fNewPosY, float in_fDelayToDest )
{
	m_MovementData.vOPos = m_vCurrPos;
	m_MovementData.vDPos.x = in_fNewPosX;
	m_MovementData.vDPos.y = in_fNewPosY;
	m_MovementData.fOTime = GetEngine()->GetClock()->GetTotalTime();
	m_MovementData.fDuration = in_fDelayToDest;
	m_MovementData.fDTime = m_MovementData.fOTime + m_MovementData.fDuration;
	m_MovementData.bMoving = true;
}

void GotoComponent::Abort()
{
	m_MovementData.bMoving = false;
}

void GotoComponent::Update( float in_fDeltaTime )
{
	if( m_MovementData.bMoving )
	{
		float tNow = GetEngine()->GetClock()->GetTotalTime();
		if( tNow > m_MovementData.fDTime )
		{
			// reached destination
			m_vCurrPos = m_MovementData.vDPos;
			m_MovementData.bMoving = false;

			GetEntity()->GetLuaContext().callLuaFunction<void>("OnReachedDestinaion");
		}
		else
		{
			float fRatio = SMOOTH_STEP( m_MovementData.fOTime, m_MovementData.fDTime, tNow );

			m_vCurrPos.x = LERP( m_MovementData.vOPos.x, m_MovementData.vDPos.x, fRatio );
			m_vCurrPos.y = LERP( m_MovementData.vOPos.y, m_MovementData.vDPos.y, fRatio );

			GetEntity()->GetLuaContext().callLuaFunction<void>( "OnMoving", fRatio );
		}
	}
}

void GotoComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	GetEntity()->GetLuaContext().registerFunction("goto", &GotoComponent::Goto);
	GetEntity()->GetLuaContext().registerFunction("abort", &GotoComponent::Abort);
}
