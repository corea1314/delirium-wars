#include "GizmoScaling.h"

#include "Lair/Lair.h"
#include "../Editor.h"
#include "../Grid.h"
#include "gfx.h"

#include <stdlib.h>
#include <float.h>

static const int kWidgetSize	=	64;

GizmoScaling::GizmoScaling( Editor* inEditor ) : Gizmo(inEditor), mMode(Mode::NotDragging)
{
}

void GizmoScaling::OnRender()
{
}

void GizmoScaling::OnRenderGUI()
{
	int x,y;
	mEditor->EditorToScreen(mPos,x,y);

	int ax,ay;
	mEditor->EditorToScreen(mAnchor,ax,ay);

	const int kScaleHandleSize = kWidgetSize / 16; 

	// Squares
	glBegin(GL_QUADS);	
		if( mMode == Mode::Dragging && mAxis == Axis::Both)
			gl_SetColor( COLORS::eYELLOW );
		else
			gl_SetColor( COLORS::eLIGHTGREY );
		glVertex2i( x+kScaleHandleSize*2, y+kScaleHandleSize*2 );
		glVertex2i( x-kScaleHandleSize*2, y+kScaleHandleSize*2 );
		glVertex2i( x-kScaleHandleSize*2, y-kScaleHandleSize*2 );
		glVertex2i( x+kScaleHandleSize*2, y-kScaleHandleSize*2 );

		if( mMode == Mode::Dragging && (mAxis == Axis::X || mAxis == Axis::Both) )
			gl_SetColor( COLORS::eYELLOW );
		else
			gl_SetColor( COLORS::eRED );
		glVertex2i( x+kWidgetSize+kScaleHandleSize, y+kScaleHandleSize );
		glVertex2i( x+kWidgetSize-kScaleHandleSize, y+kScaleHandleSize );
		glVertex2i( x+kWidgetSize-kScaleHandleSize, y-kScaleHandleSize );
		glVertex2i( x+kWidgetSize+kScaleHandleSize, y-kScaleHandleSize );

		if( mMode == Mode::Dragging && (mAxis == Axis::Y || mAxis == Axis::Both) )
			gl_SetColor( COLORS::eYELLOW );
		else
			gl_SetColor( COLORS::eGREEN );
		glVertex2i( x+kScaleHandleSize, y+kWidgetSize+kScaleHandleSize );
		glVertex2i( x-kScaleHandleSize, y+kWidgetSize+kScaleHandleSize );
		glVertex2i( x-kScaleHandleSize, y+kWidgetSize-kScaleHandleSize );
		glVertex2i( x+kScaleHandleSize, y+kWidgetSize-kScaleHandleSize );
	glEnd();

	gl_SetColor( COLORS::eLIGHTGREY );
	
	// Drag vector
	glBegin(GL_LINES);
		if( mMode == Mode::Dragging )
		{		
			glVertex2i( x, y );
			glVertex2i( ax, ay );
		}

		// X axis
		if( mMode == Mode::Dragging && (mAxis == Axis::X || mAxis == Axis::Both) )
			gl_SetColor( COLORS::eYELLOW );
		else
			gl_SetColor( COLORS::eRED );
		glVertex2i( x, y );
		glVertex2i( x+kWidgetSize, y );

		// Y axis
		if( mMode == Mode::Dragging && (mAxis == Axis::Y || mAxis == Axis::Both) )
			gl_SetColor( COLORS::eYELLOW );
		else
			gl_SetColor( COLORS::eGREEN );
		glVertex2i( x, y );
		glVertex2i( x, y+kWidgetSize );
	glEnd();

	// Text
	gl_SetColor( COLORS::eWHITE );
	if( mMode == Mode::Dragging )
	{		
		Vector2 vDelta = (mPos-mAnchor);
		gl_RenderText( x+16, y-16, "(%0.2f,%0.2f)", vDelta.x, vDelta.y );
	}
}

void GizmoScaling::OnKeyboard( unsigned char key, int mod )
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

void GizmoScaling::OnMouseMotion( const MouseMotion& mm )
{
	if( mMode == Mode::NotDragging || mMode == Mode::DoneDragging )
	{
		if( Lair::GetInputMan()->IsMouseButtonDown( InputMan::MouseButton::Left ) )
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

void GizmoScaling::OnMouseClick( int button, int state, const MouseMotion& mm )
{	
	if( button == InputMan::MouseButton::Left && state )
	{
		//fixme: should check if we clicked on gizmo
		if( fabsf( mm.pos.x-mPos.x) < 2.0f && fabsf( mm.pos.y-mPos.y) > 8.0f )
			mAxis = Axis::Y;
		else if( fabsf( mm.pos.y-mPos.y) < 2.0f && fabsf( mm.pos.x-mPos.x) > 8.0f )
			mAxis = Axis::X;
		else
			mAxis = Axis::Both;

		mMode = Mode::Dragging;
		mAnchor = mEditor->GetGrid()->Snap(mm.pos);
	}
	else
	{
		mMode = Mode::DoneDragging;
	}	
}

void GizmoScaling::ProcessTextEntry( const char* inText )
{
	float x,y;
	if( sscanf_s( inText, "%f &f", &x, &y ) == 2 )
	{
		if( _isnan(x) == 0 && _isnan(y) == 0 )
		{
			Vector2 vOldScale = mScale;
			mScale.Set(x,y);
			mEditor->OnScale( mScale, mScale-vOldScale );
		}
	}
}