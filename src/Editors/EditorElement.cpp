
#include "gfx.h"

#include "Grid.h"
#include "Editor.h"
#include "EditorElement.h"

#include "Lair/Lair.h"

void EditorElement::OnKeyboard( unsigned char key, int mod )
{
	if( mSelected == false )
		return;

	switch( key )
	{
	case 13:	// Enter key
	case 27: 	// Escape key
		{	// Deselect
			mSelected = false;	
			break;
		}
	case 127:	// Delete key
		{	// Delete element
			mDeleteRequest = true;
		}
		break;
	}
}

void EditorElement::OnSpecialKey( int key, int mod ) {}
bool EditorElement::OnMouseMotion( const MouseMotion& mm ) { return false; }
bool EditorElement::OnMouseClick( int button, int state, const MouseMotion& mm ) { return false; }

void EditorElement::OnTranslate( const Vector2& inNewPos, const Vector2& inDelta ) {}
void EditorElement::OnScale( const Vector2& inNewScale, const Vector2& inDelta ) {}
void EditorElement::OnRotate( float inAngle, float inDelta ) {}
