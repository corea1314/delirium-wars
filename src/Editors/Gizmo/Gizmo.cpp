#include "Gizmo.h"
#include "Editors/Editor.h"

void Gizmo::Init( AnimatableElement* inAnimatable ) {}
void Gizmo::OnRender() {}
void Gizmo::OnRenderGUI() {}
void Gizmo::OnUpdate( float inDeltaTime ) {}
void Gizmo::OnMouseMotion( const MouseMotion& mm ) {}
void Gizmo::OnMouseClick( int button, int state, const MouseMotion& mm ) {}

void Gizmo::OnKeyboard( unsigned char key, int mod )
{
	switch( key )
	{
	case  8:	// Backspace key
		{	// Delete character from name
			if( mTextEntry.size() )
				mTextEntry.pop_back();	
		}	
		break;
	case 13:	// Enter key
		{
			if( mTextEntry.size() != 0 )
			{
				ProcessTextEntry( mTextEntry.c_str() );
				mTextEntry = "";
			}
		}
		break;
	default:
		{	// Adding character to name
			if( IsAcceptableCharForText(key) )
				mTextEntry += key;
		}
		break;
	}
}

bool Gizmo::IsAcceptableCharForText( char inKey ) 
{ 
	return isdigit(inKey) || inKey == '.' || inKey == '-' || inKey == ' '; 
}
