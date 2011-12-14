
#include "Math/Vector2.h"
#include "Engine/Entity.h"
#include "Engine/Engine.h"

#include "InputComponent.h"

#include "Lair/Lair.h"

// component interface ////////////////////////////////////////////////////////

InputComponent::InputComponent()
{
}

InputComponent::~InputComponent()
{

}

// engine connection //////////////////////////////////////////////////////////

void InputComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	in_pEngine->Connect_OnKeyboard( this, &InputComponent::OnKeyboard );
	in_pEngine->Connect_OnMouseClick( this, &InputComponent::OnMouseClick );
	in_pEngine->Connect_OnMouseWheel( this, &InputComponent::OnMouseWheel );

//	GetEntity()->GetLuaContext().registerFunction( "goto", &InputComponent::Goto );
	
	BindCallback( "OnInputComponent_Keyboard", m_cbOnKeyboardKey );
	BindCallback( "OnInputComponent_MouseClick", m_cbOnMouseClick );
	BindCallback( "OnInputComponent_MouseWheel", m_cbOnMouseWheel );
}

void InputComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
	in_pEngine->Disconnect_OnKeyboard( this );
	in_pEngine->Disconnect_OnMouseClick( this );
	in_pEngine->Disconnect_OnMouseWheel( this );

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void InputComponent::OnKeyboard( unsigned char in_cKey )
{
	if( m_cbOnKeyboardKey.IsEnabled() )
		GetEntity()->GetLuaContext().callLuaFunction<int>(m_cbOnKeyboardKey.GetName(), (char)in_cKey );
}

void InputComponent::OnMouseClick( unsigned int in_nButton, int in_nScreenSpacePositionX, int in_nScreenSpacePositionY, const Vector2& in_vWorldPos )
{
	if( m_cbOnMouseClick.IsEnabled() )
		GetEntity()->GetLuaContext().callLuaFunction<int>(m_cbOnMouseClick.GetName(), (int)in_nButton, in_nScreenSpacePositionX, in_nScreenSpacePositionY, in_vWorldPos.x, in_vWorldPos.y );
}

void InputComponent::OnMouseWheel( unsigned int in_nValue )
{
	if( m_cbOnMouseWheel.IsEnabled() )
		GetEntity()->GetLuaContext().callLuaFunction<int>(m_cbOnMouseWheel.GetName(), in_nValue );
}
