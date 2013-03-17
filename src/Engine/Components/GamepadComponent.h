#pragma once

class CEngine;

class GamepadComponent : public CEntity::Component
{
public:
	GamepadComponent();
	virtual ~GamepadComponent();

	// component interface
	bool IsButtonDown( unsigned int inGamepadIndex, unsigned int inButton );
	bool IsButtonJustDown( unsigned int inGamepadIndex, unsigned int inButton );
	bool IsButtonJustUp( unsigned int inGamepadIndex, unsigned int inButton );

	bool IsPadDown( unsigned int inGamepadIndex, unsigned int inButton );
	bool IsPadJustDown( unsigned int inGamepadIndex, unsigned int inButton );
	bool IsPadJustUp( unsigned int inGamepadIndex, unsigned int inButton );
		
	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

	// engine callbacks
	void OnUpdate( float in_fDeltaTime );

private:
	// callbacks
//	LuaCallbackInfo		m_cbOnDestinationReached;
//	LuaCallbackInfo		m_cbOnMoving;
};


