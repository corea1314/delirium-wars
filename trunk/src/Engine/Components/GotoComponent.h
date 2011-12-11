#pragma once

class CEngine;

#include "Math/Vector2.h"

class GotoComponent : public CEntity::Component
{
public:
	GotoComponent();

	void Goto( float in_fNewPosX, float in_fNewPosY, float in_fDelayToDest );
	void Abort();
	void Update( float in_fDeltaTime );

	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity ) ;

private:
	Vector2 m_vCurrPos;

	// used in movement animation
	struct MovementData
	{
		Vector2 vOPos;			// origin position
		Vector2 vDPos;			// destination position
		float	fOTime;			// time when movement request occured
		float	fDTime;			// time when movement should reach destination position
		float	fDuration;		// delay in which movement to destination position is required
		bool	bMoving;		// does the camera need to move

	} m_MovementData;
};


