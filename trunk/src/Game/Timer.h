#ifndef _TIMER_H
#define _TIMER_H

// the clock is holding the master time line.
// it cannot be paused, stopped, etc. only updated.
// you can acquire user timers from it.
// they get updated when the clock is updated.
// timers can be paused or stopped.

#include "../macros.h"

#include <windows.h>

class CTimer
{
private:
	bool	m_bPaused;
	bool	m_bEnabled;

	double	m_dDeltaTime;
	double	m_dTotalTime;

public:
	CTimer() : m_bEnabled(false), m_bPaused(false), m_dDeltaTime(0.0), m_dTotalTime(0.0) {}
	
	void Enable( bool in_bEnabled ) { m_bEnabled = in_bEnabled; }
	void Pause( bool in_bPaused ) { m_bPaused = in_bPaused; }

	bool IsEnabled() const { return m_bEnabled; }
	bool IsPaused() const { return m_bPaused; }

	void Update( double	in_dDeltaTime )
	{		
		if( m_bEnabled && m_bPaused == false )
		{
			m_dDeltaTime = in_dDeltaTime;
			m_dTotalTime += m_dDeltaTime;
		}
	}
		
	double	GetDeltaTime() const { return m_dDeltaTime; }
	double	GetTotalTime() const { return m_dTotalTime; }
};

class CClock
{
public:
	CClock()
	{
		// constructor
		QueryPerformanceFrequency(&liTimerFrequency);
		dFreq = (double) liTimerFrequency.QuadPart;

		// calibration
		Start();
		Stop();
		dCorrection = (double)(liEndCount.QuadPart - liStartCount.QuadPart);
	}

	virtual ~CClock() {};

	void Update();

private:

	inline void Start(void)
	{
		dTime = 0.0;
		QueryPerformanceCounter(&liStartCount);
		dStart = (double)liStartCount.QuadPart;
	}

	inline void Stop(void)
	{
		QueryPerformanceCounter(&liEndCount);
		dStop = (double)liEndCount.QuadPart;
	}

	inline double GetDeltaTime(void) const
	{
		return (dStop - dStart - dCorrection)/(dFreq);
	}	

	// VARIABLES
protected:
	LARGE_INTEGER liTimerFrequency;
	LARGE_INTEGER liStartCount;
	LARGE_INTEGER liEndCount;

	double	dStart;
	double	dStop;
	double	dFreq;
	double	dTime;
	double	dCorrection;
};

#endif//_TIMER_H
