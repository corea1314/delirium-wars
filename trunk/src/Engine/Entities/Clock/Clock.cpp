
#include "Clock.h"
#include "Timer.h"

#include "Engine/Engine.h"

#include <algorithm>

IMPLEMENT_CLASS_TYPE(CClock)

CClock::CClock() : m_fDeltaTime(0.0f), m_fTotalTime(0.0f)
{
}

CClock::~CClock()
{
}

void CClock::Connect( CEngine* in_pEngine )
{
	in_pEngine->Connect_OnUpdate( this, &CClock::Update );
	in_pEngine->Connect_OnRenderGUI( this, &CClock::Render );
}

void CClock::Disconnect( CEngine* in_pEngine )
{
	in_pEngine->Disconnect_OnUpdate( this );
	in_pEngine->Disconnect_OnRenderGUI( this );
}

void CClock::Update( float in_fDeltaTime )
{
	m_fDeltaTime = in_fDeltaTime;
	m_fTotalTime += m_fDeltaTime;

	std::for_each( m_setTimer.begin(), m_setTimer.end(), std::bind2nd( std::mem_fun( &CTimer::Update ), m_fDeltaTime ) );
}


void CClock::Render()
{
//todo:
}

bool CClock::CreateTimer( CTimer** out_ppTimer )
{
	if( *out_ppTimer != 0 )
		return false;

	*out_ppTimer = new CTimer;
	m_setTimer.insert( *out_ppTimer );

	return true;
}

bool CClock::FreeTimer( CTimer** out_ppTimer )
{
	std::set<CTimer*>::iterator it = m_setTimer.find( *out_ppTimer );
	if( it != m_setTimer.end() )
	{
		m_setTimer.erase( it );
		*out_ppTimer = 0;
		return true;
	}

	return false;
}