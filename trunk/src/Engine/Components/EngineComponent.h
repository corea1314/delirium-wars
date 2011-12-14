#pragma once

class CEngine;

#include "Math/Vector2.h"

class EngineComponent : public CEntity::Component
{
public:
	EngineComponent();
	virtual ~EngineComponent();

	// component interface
	void CreateEntity( std::string in_szEntityName, std::string in_szLuaScript, float in_fPosX, float in_fPosY, float in_fAngle );

	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

	// engine callbacks
	void OnUpdate( float in_fDeltaTime );
	
private:
	// callbacks
	LuaCallbackInfo		m_cbOnUpdate;
};


