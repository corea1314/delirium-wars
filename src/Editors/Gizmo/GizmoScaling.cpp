#include "GizmoScaling.h"

#include "Lair/Lair.h"
#include "../Editor.h"
#include "../Grid.h"
#include "gfx.h"

#include "Editors/Animatable/Animatable.h"

#include <stdlib.h>
#include <float.h>

static const int kWidgetSize	=	64;
static const float kScaleChangeFactor		= 0.005f;

GizmoScaling::GizmoScaling( Editor* inEditor ) : Gizmo(inEditor), mMode(Mode::NotDragging)
{
}

void GizmoScaling::Init( AnimatableElement* inAnimatable )
{
	mPos = inAnimatable->mPos;
	mScale = inAnimatable->mScale;
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
		gl_RenderText( x+16, y-16, "(%0.2f,%0.2f)", mScale.x, mScale.y );
	}
	else
	{
		// Display text entry
		if( mTextEntry.size() != 0 )
			gl_RenderText( x+16, y-16, "(%s)", mTextEntry.c_str() );
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
		}
	}
	else if( mMode == Mode::Dragging )
	{
		// Convert distance to scale ratio 
		// Negative distance equals reduction
		// Positive distance equals magnification
		// Apply on selected axis

		Vector2 vOldScale = mScale;

		if( mAxis == Axis::X )
			mScale.x += mm.dx * kScaleChangeFactor;
		else if( mAxis == Axis::Y )
			mScale.y += mm.dy * kScaleChangeFactor;
		else
		{
			if( ABS(mm.dx) > ABS(mm.dy) )
			{
				mScale.x += mm.dx * kScaleChangeFactor;
				mScale.y += mm.dx * kScaleChangeFactor;
			}
			else
			{
				mScale.x += mm.dy * kScaleChangeFactor;
				mScale.y += mm.dy * kScaleChangeFactor;
			}
		}

		mEditor->OnScale( mScale, mScale-vOldScale );
	}
}

void GizmoScaling::OnMouseClick( int button, int state, const MouseMotion& mm )
{	
	if( button == InputMan::MouseButton::Left && state )
	{
		const float dx = mm.pos.x-mPos.x;
		const float fdx = fabsf(dx);
		const float dy = mm.pos.y-mPos.y;
		const float fdy = fabsf(dy);

		const float kAxisThickness = 4.0f * mEditor->GetCamera()->GetZoom();
		const float kAxisLength = kWidgetSize * mEditor->GetCamera()->GetZoom();

		// Click on Y axis
		if( dy > 0 && fdx < kAxisThickness && 0 < fdy && fdy < kAxisLength )
			mAxis = Axis::Y;
		// Click on X axis
		else if( dx > 0 && fdy < kAxisThickness && 0 < fdx && fdx < kAxisLength )
			mAxis = Axis::X;
		// Click on triangle
		else if( dx > 0 && dy > 0 && fdx > 0 && fdy > 0 && fdx < kAxisLength/2 && fdy < kAxisLength/2 )
			mAxis = Axis::Both;
		else
			mAxis = Axis::None;

		if( mAxis != Axis::None )
		{
			mMode = Mode::Dragging;
		}		
	}
	else
	{
		mMode = Mode::DoneDragging;
	}	
}

void GizmoScaling::ProcessTextEntry( const char* inText )
{
	float x,y;
	if( sscanf_s( inText, "%f %f", &x, &y ) == 2 )
	{
		if( _isnan(x) == 0 && _isnan(y) == 0 )
		{
			Vector2 vOldScale = mScale;
			mScale.Set(x,y);
			mEditor->OnScale( mScale, mScale-vOldScale );
		}
	}
}