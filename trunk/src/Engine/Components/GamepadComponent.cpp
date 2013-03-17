
#include "Math/Vector2.h"
#include "Engine/Entity.h"
#include "Engine/Engine.h"
#include "Engine/Clock/Clock.h"

#include "GamepadComponent.h"

#include "Lair/Lair.h"

// component interface ////////////////////////////////////////////////////////

GamepadComponent::GamepadComponent()
{
}

GamepadComponent::~GamepadComponent()
{
}

bool GamepadComponent::IsButtonDown( unsigned int inGamepadIndex, unsigned int inButton )
{
	return Lair::GetInputMan()->GetGamepad(inGamepadIndex)->IsButtonDown((Xbox360Gamepad::Button::Enum)inButton);
}

bool GamepadComponent::IsButtonJustDown( unsigned int inGamepadIndex, unsigned int inButton )
{
	return Lair::GetInputMan()->GetGamepad(inGamepadIndex)->IsButtonJustDown((Xbox360Gamepad::Button::Enum)inButton);
}

bool GamepadComponent::IsButtonJustUp( unsigned int inGamepadIndex, unsigned int inButton )
{
	return Lair::GetInputMan()->GetGamepad(inGamepadIndex)->IsButtonJustUp((Xbox360Gamepad::Button::Enum)inButton);
}

bool GamepadComponent::IsPadDown( unsigned int inGamepadIndex, unsigned int inButton )
{
	return Lair::GetInputMan()->GetGamepad(inGamepadIndex)->IsPadDown((Xbox360Gamepad::Pad::Enum)inButton);
}

bool GamepadComponent::IsPadJustDown( unsigned int inGamepadIndex, unsigned int inButton )
{
	return Lair::GetInputMan()->GetGamepad(inGamepadIndex)->IsPadJustDown((Xbox360Gamepad::Pad::Enum)inButton);
}

bool GamepadComponent::IsPadJustUp( unsigned int inGamepadIndex, unsigned int inButton )
{
	return Lair::GetInputMan()->GetGamepad(inGamepadIndex)->IsPadJustUp((Xbox360Gamepad::Pad::Enum)inButton);
}


// engine connection //////////////////////////////////////////////////////////

void GamepadComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

//	in_pEngine->Connect_OnUpdate( this, &GamepadComponent::OnUpdate );

	GetEntity()->GetLuaContext().registerFunction( "is_button_down", &GamepadComponent::IsButtonDown );
	GetEntity()->GetLuaContext().registerFunction( "is_button_just_down", &GamepadComponent::IsButtonJustDown );
	GetEntity()->GetLuaContext().registerFunction( "is_button_just_up", &GamepadComponent::IsButtonJustUp );

	GetEntity()->GetLuaContext().registerFunction( "is_pad_down", &GamepadComponent::IsPadDown );
	GetEntity()->GetLuaContext().registerFunction( "is_pad_just_down", &GamepadComponent::IsPadJustDown );
	GetEntity()->GetLuaContext().registerFunction( "is_pad_just_up", &GamepadComponent::IsPadJustUp );

//	BindCallback( "OnGotoComponent_DestReached", m_cbOnDestinationReached );
//	BindCallback( "OnGotoComponent_Moving", m_cbOnMoving );
}

void GamepadComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
//	in_pEngine->Disconnect_OnUpdate( this );

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void GamepadComponent::OnUpdate( float in_fDeltaTime )
{
	/*
	if( m_bMoving )
	{
		float tNow = GetEngine()->GetClock()->GetTotalTime();
		if( tNow > m_fDTime )
		{
			// reached destination
			GetEntity()->GetPos() = m_vDPos;
			m_bMoving = false;

			if( m_cbOnDestinationReached.IsEnabled() )
				GetEntity()->GetLuaContext().callLuaFunction<void>(m_cbOnDestinationReached.GetName());
		}
		else
		{
			float fRatio = SMOOTH_STEP( m_fOTime, m_fDTime, tNow );

			GetEntity()->GetPos().x = LERP( m_vOPos.x, m_vDPos.x, fRatio );
			GetEntity()->GetPos().y = LERP( m_vOPos.y, m_vDPos.y, fRatio );

			if( m_cbOnMoving.IsEnabled() )
				GetEntity()->GetLuaContext().callLuaFunction<void>( m_cbOnMoving.GetName(), fRatio );
		}
	}
	*/
}
