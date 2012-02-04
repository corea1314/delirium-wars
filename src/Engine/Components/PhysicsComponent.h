#pragma once

class CEngine;

class b2Body;

#include "Math/Vector2.h"

class PhysicsComponent : public CEntity::Component
{
public:
	PhysicsComponent();
	virtual ~PhysicsComponent();

	// component interface
	void OnPostUpdate();
	void CreateBody( std::string inBodyDefFilename, bool inStartActive );
	void DestroyBody();
	void Warp( float inX, float inY, float inAngle );
			
	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

	// engine callbacks
	void OnCallback();

private:
	// 
	b2Body*	m_pBody;
		
	// callbacks
	LuaCallbackInfo		m_cbOnCallback;
};


