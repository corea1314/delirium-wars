#include "Camera.h"

#include "Engine.h"
#include "Clock.h"


CCamera::CCamera()
{
	m_MovementData.bMoving = false;
	m_ZoomData.bZooming = false;
	m_vCurrPos.Set(0,0);
	m_fCurrZoom = 1.0f;	// 1X
}

void CCamera::Goto( Vector2 in_vNewPos, float in_fDelayToDest )
{
	m_MovementData.vOPos = m_vCurrPos;
	m_MovementData.vDPos = in_vNewPos;
	m_MovementData.fOTime = m_pEngine->GetClock()->GetTotalTime();
	m_MovementData.fDuration = in_fDelayToDest;
	m_MovementData.fDTime = m_MovementData.fOTime + m_MovementData.fDuration;
	m_MovementData.bMoving = true;
}

void CCamera::ZoomTo( float in_fNewZoom, float in_fDelayToDest )
{
	m_ZoomData.fOZoom = m_fCurrZoom;
	m_ZoomData.fDZoom = in_fNewZoom;
	m_ZoomData.fOTime = m_pEngine->GetClock()->GetTotalTime();
	m_ZoomData.fDuration = in_fDelayToDest;
	m_ZoomData.fDTime = m_ZoomData.fOTime + m_ZoomData.fDuration;
	m_ZoomData.bZooming = true;
}

void CCamera::Update( float in_fDeltaTime )
{
	if( m_MovementData.bMoving )
	{
		float tNow = m_pEngine->GetClock()->GetTotalTime();
		if( tNow > m_MovementData.fDTime )
		{
			// reached destination
			m_vCurrPos = m_MovementData.vDPos;
			m_MovementData.bMoving = false;
		}
		else
		{
			float fRatio = SMOOTH_STEP( m_MovementData.fOTime, m_MovementData.fDTime, tNow );

			m_vCurrPos.x = LERP( m_MovementData.vOPos.x, m_MovementData.vDPos.x, fRatio );
			m_vCurrPos.y = LERP( m_MovementData.vOPos.y, m_MovementData.vDPos.y, fRatio );
		}
	}

	if( m_ZoomData.bZooming )
	{
		float tNow = m_pEngine->GetClock()->GetTotalTime();
		if( tNow > m_ZoomData.fDTime )
		{
			// reached zoom level
			m_fCurrZoom = m_ZoomData.fDZoom;
			m_ZoomData.bZooming = false;
		}
		else
		{
			float fRatio = SMOOTH_STEP( m_ZoomData.fOTime, m_ZoomData.fDTime, tNow );
			m_fCurrZoom = LERP( m_ZoomData.fOZoom, m_ZoomData.fDZoom, fRatio );
		}
	}
	
}

void CCamera::Connect( CEngine* in_pEngine )
{
	m_pEngine = in_pEngine;
	m_pEngine->Connect_Update( this, &CCamera::Update );
}

void CCamera::Disconnect( CEngine* in_pEngine )
{
	assert( m_pEngine == in_pEngine );
	in_pEngine->Disconnect_Update( this );
	m_pEngine = 0;
}
