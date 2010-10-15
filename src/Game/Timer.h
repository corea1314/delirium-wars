#ifndef _TIMER_H
#define _TIMER_H

class CTimer
{
private:
	bool	m_bPaused;
	bool	m_bEnabled;

	float	m_fDeltaTime;
	float	m_fTotalTime;

	float	m_fRatio;

public:
	CTimer() : m_bEnabled(false), m_bPaused(false), m_fDeltaTime(0.0), m_fTotalTime(0.0), m_fRatio(1.0f) {}
	
	void Enable( bool in_bEnabled ) { m_bEnabled = in_bEnabled; }
	void Pause( bool in_bPaused ) { m_bPaused = in_bPaused; }

	bool IsEnabled() const { return m_bEnabled; }
	bool IsPaused() const { return m_bPaused; }

	void SetRatio( float in_fRatio ) { m_fRatio = in_fRatio; }

	void Update( float in_fDeltaTime )
	{		
		if( m_bEnabled && m_bPaused == false )
		{
			m_fDeltaTime = in_fDeltaTime * m_fRatio;
			m_fTotalTime += m_fDeltaTime;
		}
	}
		
	inline float	GetDeltaTime() const { return m_fDeltaTime; }
	inline float	GetTotalTime() const { return m_fTotalTime; }
};



#endif//_TIMER_H
