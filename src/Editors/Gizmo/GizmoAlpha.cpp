#include "GizmoAlpha.h"

#include "Lair/Lair.h"
#include "../Editor.h"
#include "../Grid.h"
#include "gfx.h"

#include "Editors/Animatable/Animatable.h"

#include <stdlib.h>
#include <float.h>

static const int kWidgetSize	=	64;

GizmoAlpha::GizmoAlpha( Editor* inEditor ) : Gizmo(inEditor), mMode(Mode::NotDragging)
{
}

void GizmoAlpha::Init( Animatable* inAnimatable )
{
	mPos = inAnimatable->mPos;
	mAlpha = inAnimatable->mAlpha;
}

void GizmoAlpha::OnRender()
{
}

void GizmoAlpha::OnRenderGUI()
{
	int x,y;
	mEditor->EditorToScreen(mPos,x,y);

	int ax,ay;
	mEditor->EditorToScreen(mAnchor,ax,ay);

	const int kBoxSizeX = kWidgetSize / 16; 
	const int kBoxSizeY = kWidgetSize; 

	// Squares
	glBegin(GL_QUADS);	
		if( mMode == Mode::Dragging )
			gl_SetColor( COLORS::eYELLOW, mAlpha );
		else
			gl_SetColor( COLORS::eGREY, mAlpha );
		glVertex2i( x+kBoxSizeX, y+kBoxSizeY );
		glVertex2i( x-kBoxSizeX, y+kBoxSizeY );
		glVertex2i( x-kBoxSizeX, y-kBoxSizeY );
		glVertex2i( x+kBoxSizeX, y-kBoxSizeY );
	glEnd();

	gl_SetColor( COLORS::eWHITE );
	
	// Drag vector
	glBegin(GL_LINE_LOOP);
		glVertex2i( x+kBoxSizeX, y+kBoxSizeY );
		glVertex2i( x-kBoxSizeX, y+kBoxSizeY );
		glVertex2i( x-kBoxSizeX, y-kBoxSizeY );
		glVertex2i( x+kBoxSizeX, y-kBoxSizeY );		
	glEnd();

	// Text
	if( mMode == Mode::Dragging )
	{		
		Vector2 vDelta = (mPos-mAnchor);
		gl_RenderText( x+16, y-16, "(%0.2f)", mAlpha );
	}
	else
	{
		// Display text entry
		if( mTextEntry.size() != 0 )
			gl_RenderText( x-16, y-48, "(%s)", mTextEntry.c_str() );
	}
}

void GizmoAlpha::OnKeyboard( unsigned char key, int mod )
{
	switch( key )
	{
	case 13:	// Enter key
	case 27: 	// Escape key
		mMode = Mode::NotDragging;	break;	// Clear widget
	}

	if( mMode != Mode::Dragging )
		Gizmo::OnKeyboard( key, mod );
}

void GizmoAlpha::OnMouseMotion( const MouseMotion& mm )
{
	if( mMode == Mode::NotDragging || mMode == Mode::DoneDragging )
	{
		if( Lair::GetInputMan()->GetMouseButtonState( InputMan::MouseButton::Left ).bState )
		{
			mMode = Mode::Dragging;
			mPos = mEditor->GetGrid()->Snap(mm.pos);
		}
	}
	else if( mMode == Mode::Dragging )
	{
		mAnchor = mEditor->GetGrid()->Snap(mm.pos);

		// Compute distance from center of widget
		Vector2 vDelta = mAnchor - mPos;

		// Convert distance to scale ratio 
		// Negative distance equals reduction
		// Positive distance equals magnification
		// Apply on selected axis
		/*
		if( mAxis == Axis::X )
		else if( mAxis == Axis::Y )
		else
		*/
	}
}

void GizmoAlpha::OnMouseClick( int button, int state, const MouseMotion& mm )
{	
	if( state )
	{
		mMode = Mode::Dragging;
		mAnchor = mEditor->GetGrid()->Snap(mm.pos);
	}
	else
	{
		mMode = Mode::DoneDragging;
	}	
}

void GizmoAlpha::ProcessTextEntry( const char* inText )
{
	float a;
	if( sscanf_s( inText, "%f", &a ) == 1 )
	{
		if( _isnan(a) == 0 )
		{
			a = CLAMP( a, 0.0f, 1.0f );
			float fOldAlpha = mAlpha;
			mAlpha = a;
			mEditor->OnAlpha( mAlpha, mAlpha-fOldAlpha );
		}
	}
}