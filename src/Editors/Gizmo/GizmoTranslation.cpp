#include "GizmoTranslation.h"

#include "Lair/Lair.h"
#include "../Editor.h"
#include "../Grid.h"
#include "gfx.h"

#include "Editors/Animatable/Animatable.h"

#include <stdlib.h>
#include <float.h>

static const int kWidgetSize	=	64;

GizmoTranslation::GizmoTranslation( Editor* inEditor ) : Gizmo(inEditor), mMode(Mode::NotDragging)
{
}


void GizmoTranslation::Init( AnimatableElement* inAnimatable )
{
	mOrigin = mPos = mClickOrigin = inAnimatable->mPos;
}


void GizmoTranslation::OnRender()
{
}

void GizmoTranslation::OnRenderGUI()
{
	int x,y;
	mEditor->EditorToScreen(mPos,x,y);
	
	int ox,oy;
	mEditor->EditorToScreen(mOrigin,ox,oy);

	const int kWidgetArrowSize = kWidgetSize/4;

	// Triangle
	if( mMode == Mode::Dragging && mAxis == Axis::Both )
		gl_SetColor( COLORS::eYELLOW );
	else
		gl_SetColor( COLORS::eLIGHTGREY );
	glBegin(GL_TRIANGLES);
		glVertex2i( x, y );
		glVertex2i( x+kWidgetSize/2, y );
		glVertex2i( x, y+kWidgetSize/2 );

		if( mMode == Mode::Dragging && (mAxis == Axis::X || mAxis == Axis::Both) )
			gl_SetColor( COLORS::eYELLOW );
		else
			gl_SetColor( COLORS::eRED );
		glVertex2i( x+kWidgetSize-kWidgetArrowSize, y-kWidgetArrowSize/3 );
		glVertex2i( x+kWidgetSize, y );
		glVertex2i( x+kWidgetSize-kWidgetArrowSize, y+kWidgetArrowSize/3 );
		
		if( mMode == Mode::Dragging && (mAxis == Axis::Y || mAxis == Axis::Both) )
			gl_SetColor( COLORS::eYELLOW );
		else
			gl_SetColor( COLORS::eGREEN );
		glVertex2i( x-kWidgetArrowSize/3, y+kWidgetSize-kWidgetArrowSize );
		glVertex2i( x, y+kWidgetSize );
		glVertex2i( x+kWidgetArrowSize/3, y+kWidgetSize-kWidgetArrowSize );
	glEnd();

	
	glBegin(GL_LINES);
	
		// Drag vector
		if( mMode == Mode::Dragging )
		{
			gl_SetColor( COLORS::eLIGHTGREY );
			glVertex2i( x, y );
			glVertex2i( ox, oy );
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
		Vector2 vDelta = (mPos-mOrigin);
		gl_RenderText( x+16, y-16, "(%0.2f,%0.2f)", vDelta.x, vDelta.y );
	}
	else
	{
		// Display text entry
		if( mTextEntry.size() != 0 )
			gl_RenderText( x+16, y-16, "(%s)", mTextEntry.c_str() );
	}
}

void GizmoTranslation::OnKeyboard( unsigned char key, int mod, bool down )
{	
	if( !down ) 
		return; // process key down only

	switch( key )
	{		
	case 27: 	// Escape key
		mMode = Mode::NotDragging;	break;	// Clear widget
	}

	if( mMode != Mode::Dragging )
		Gizmo::OnKeyboard( key, mod, down );
}

void GizmoTranslation::OnMouseMotion( const MouseMotion& mm )
{
	if( mMode == Mode::Dragging )
	{
		Vector2 vOldPos = mPos;
		Vector2 vPos = mm.pos;
		mDelta = vPos - mClickOrigin;

		vPos = mEditor->GetGrid()->Snap(mOrigin + mDelta);	// snap resulting point
		
		if( mAxis == Axis::X )
			mPos.x = vPos.x;
		else if( mAxis == Axis::Y )
			mPos.y = vPos.y;
		else
			mPos = vPos;

		mEditor->OnTranslate( mPos, mPos-vOldPos );
	}
}

void GizmoTranslation::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	//fixme: should handle the delta position to correct the glitch on click
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
			mOrigin = mPos;
			mClickOrigin = mm.pos;
		}		
	}
	else
	{		
		mMode = Mode::DoneDragging;
	}
}

void GizmoTranslation::ProcessTextEntry( const std::string& inText )
{
	float x,y;
	if( sscanf_s( inText.c_str(), "%f %f", &x, &y ) == 2 )
	{
		if( _isnan(x) == 0 && _isnan(y) == 0 )
		{
			Vector2 vOldPos = mPos;
			mPos.Set(x,y);
			mEditor->OnTranslate( mPos, mPos-vOldPos );
		}
	}
}
