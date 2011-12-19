#ifndef _CLOCK_H
#define _CLOCK_H

#include "Engine/SigSlot.h"

// the clock is holding the master time line.
// it cannot be paused, stopped, etc. only updated.
// you can acquire user timers from it.
// they get updated when the clock is updated.
// timers can be paused or stopped.

class CEngine;
class CTimer;

class CClock : public has_slots<>
{
public:
	CClock();
	virtual ~CClock();
	
	void Update( float in_fDeltaTime );
	void DebugRender();

	inline float GetDeltaTime() const { return m_fDeltaTime; }
	inline float GetTotalTime() const { return m_fTotalTime; }
	inline void Reset() { m_fTotalTime = 0.0f; }

	virtual void Connect( CEngine* in_pEngine );
	virtual void Disconnect( CEngine* in_pEngine );

	bool	CreateTimer( CTimer** );
	bool	FreeTimer( CTimer** );

private:
	float m_fDeltaTime;
	float m_fTotalTime;

	std::set<CTimer*>	m_setTimer;
};

#endif//_CLOCK_H
