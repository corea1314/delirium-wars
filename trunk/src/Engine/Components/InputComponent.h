#pragma once

class CEngine;

#include "Math/Vector2.h"

class InputComponent : public CEntity::Component
{
public:
	InputComponent();
	virtual ~InputComponent();

	// component interface
			
	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

	// engine callbacks
	// void OnUpdate( float in_fDeltaTime );
	void OnKeyboard( unsigned char in_cKey, bool in_bKeyDown );
	void OnMouseClick( unsigned int in_nButton, int in_nScreenSpacePositionX,  int in_nScreenSpacePositionY, const Vector2& in_vWorldPos );
	void OnMouseWheel( unsigned int in_nValue );

private:
		
	// callbacks
	LuaCallbackInfo		m_cbOnKeyboardKey;
	LuaCallbackInfo		m_cbOnMouseClick;
	LuaCallbackInfo		m_cbOnMouseWheel;
};


