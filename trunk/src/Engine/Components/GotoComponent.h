#pragma once

class CEngine;

#include "Math/Vector2.h"

class GotoComponent : public CEntity::Component
{
public:
	GotoComponent();
	virtual ~GotoComponent();

	// component interface
	void Goto( float in_fNewPosX, float in_fNewPosY, float in_fDelayToDest );
	void Abort();
		
	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

	// engine callbacks
	void OnUpdate( float in_fDeltaTime );

private:
	Vector2 m_vOPos;		// origin position
	Vector2 m_vDPos;		// destination position
	float	m_fOTime;		// time when movement request occured
	float	m_fDTime;		// time when movement should reach destination position
	float	m_fDuration;	// delay in which movement to destination position is required
	bool	m_bMoving;		// does the it needs to move
	
	// callbacks
	LuaCallbackInfo		m_cbDestReached;
	LuaCallbackInfo		m_cbMoving;
};


