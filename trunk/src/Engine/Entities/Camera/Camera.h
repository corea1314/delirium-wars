#ifndef _CAMERA_H
#define _CAMERA_H

#include "Engine/Entity.h"
#include "Math/Vector2.h"

class CEngine;

class CCamera : public CEntity
{
public:
	CCamera();

	void Goto( Vector2 in_vNewPos, float in_fDelayToDest );
	void ZoomTo( float in_fNewZoom, float in_fDelayToDest );

	void Update( float in_fDeltaTime );

	Vector2 GetPos() const { return m_vCurrPos; }
	float	GetZoom() const { return m_fCurrZoom; }

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );
	
private:
	Vector2 m_vCurrPos;
	float	m_fCurrZoom;
	
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

	// used in zoom animation
	struct ZoomData
	{
		float	fOZoom;			// origin zoom
		float	fDZoom;			// destination zoom
		float	fOTime;			// time when zoom request occured
		float	fDTime;			// time when zoom should reach destination zoom
		float	fDuration;		// delay in which animation to destination zoom is required
		bool	bZooming;		// does the camera need to zoom

	} m_ZoomData;

	CEngine* m_pEngine;	
};


#endif//_CAMERA_H
