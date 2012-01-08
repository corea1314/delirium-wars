
#include "gfx.h"

#include "Editors/Grid.h"
#include "Editors/Editor.h"
#include "LayoutElement.h"

#include "Lair/Lair.h"

static const float	kElementSelectionRadius			= 8.0f;
static const float	kElementSelectionRadiusSquared	= kElementSelectionRadius * kElementSelectionRadius;


bool IsAcceptableCharForText( char inKey ) 
{ 
	return isalnum(inKey) || inKey == '.' || inKey == '-' || inKey == '_'; 
}

void LayoutElement::OnRender()
{
	glPointSize( kElementSelectionRadius*2 );

	if( mSelected )
		gl_SetColor( COLORS::eRED );
	else
		gl_SetColor( COLORS::eGREY );

	glBegin( GL_POINTS );
		glVertex2f( mPos.x, mPos.y );
	glEnd();

	glPointSize( 4.0f );
}

void LayoutElement::OnRenderGUI()
{
	int x,y;
	mEditor->EditorToScreen( mPos, x, y );

	gl_RenderText( x + 8, y - 16, "\"%s\" @ %0.2f, %0.2f", mName.c_str(), mPos.x, mPos.y );
}

void LayoutElement::OnKeyboard( unsigned char key, int mod )
{
	if( mSelected == false )
		return;
	
	switch( key )
	{
	case  8:	// Backspace key
		{	// Delete character from name
			if( mName.size() )
				 mName.pop_back();	
			break;
		}	
	default:
		{	// Adding character to name
			if( IsAcceptableCharForText(key) )
				mName += key;
			else
				EditorElement::OnKeyboard( key, mod );
		}
		break;
	}
}

void LayoutElement::OnSpecialKey( int key, int mod )
{
}

void LayoutElement::OnTranslate( const Vector2& inNewPos, const Vector2& inDelta ) 
{
	mPos += inDelta;
}

void LayoutElement::OnScale( const Vector2& inNewScale, const Vector2& inDelta ) 
{
	// Do nothing, cannot scale base element
}

void LayoutElement::OnRotate( float inAngle, float inDelta ) 
{
	mAngle += inDelta;
}
