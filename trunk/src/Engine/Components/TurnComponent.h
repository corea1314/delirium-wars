#pragma once

class CEngine;

#include "Math/Vector2.h"

class TurnComponent : public CEntity::Component
{
public:
	TurnComponent();
	virtual ~TurnComponent();

	// component interface
	void Turn( float in_fNewAngle, float in_fDelayToDest );
	void Abort();
		
	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

	// engine callbacks
	void OnUpdate( float in_fDeltaTime );

private:
	float	m_fOAngle;		// origin angle
	float	m_fDAngle;		// destination angle
	float	m_fOTime;		// time when rotation request occured
	float	m_fDTime;		// time when rotation should reach destination angle
	float	m_fDuration;	// delay in which rotation to destination angle is required
	bool	m_bTurning;		// does the it needs to turn

	// callbacks
	LuaCallbackInfo		m_cbOnDestinationReached;
	LuaCallbackInfo		m_cbOnTurning;
};


