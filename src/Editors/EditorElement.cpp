
#include "gfx.h"

#include "Grid.h"
#include "Editor.h"
#include "EditorElement.h"

#include "Lair/Lair.h"

const int kClickSelectionDelta	= 8;

bool EditorElement::OnSelect( const Vector2& inPos )
{
	int mx,my;
	mEditor->EditorToScreen( mPos, mx, my );

	int ix,iy;
	mEditor->EditorToScreen( inPos, ix, iy );
	
	if( ABS(ix-mx) < kClickSelectionDelta && ABS(iy-my) < kClickSelectionDelta )
		return true;

	return false;
}

bool EditorElement::OnSelectRect( const Vector2& inMin, const Vector2& inMax )
{	
	return Vector2::IsPointInRect( mPos, inMin, inMax );
}

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
