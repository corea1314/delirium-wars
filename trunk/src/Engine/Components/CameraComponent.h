#pragma once

class Camera;
class CEngine;

#include "Math/Vector2.h"

class CameraComponent : public CEntity::Component
{
public:
	CameraComponent();
	virtual ~CameraComponent();

	// component interface
	void Zoom( float in_fNewZoom, float in_fDelayToDest );
	void Abort();
	float GetZoom();
	void SetZoom( float in_fZoom );
	void Bind();
	bool BindTo( std::string in_EntityName );
		
	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

	// engine callbacks
	void OnUpdate( float in_fDeltaTime );

private:
	Camera*	m_pCamera;		// target camera
	float	m_vOZoom;		// origin zoom
	float	m_vDZoom;		// destination zoom
	float	m_fOTime;		// time when zooming request occurred
	float	m_fDTime;		// time when zooming should reach destination value
	float	m_fDuration;	// delay in which zooming to destination value is required
	bool	m_bZooming;		// does the it needs to zoom
	
	// callbacks
	LuaCallbackInfo		m_cbOnDestinationReached;
	LuaCallbackInfo		m_cbOnZooming;
};


