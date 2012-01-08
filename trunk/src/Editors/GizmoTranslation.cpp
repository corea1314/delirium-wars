#include "GizmoTranslation.h"

#include "Lair/Lair.h"
#include "Editor.h"
#include "Grid.h"
#include "gfx.h"

static const int kWidgetSize	=	64;

GizmoTranslation::GizmoTranslation( Editor* inEditor ) : Gizmo(inEditor), mMode(Mode::NotDragging)
{
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

	// Triangle
	if( mMode == Mode::Dragging && mAxis == Axis::Both )
		gl_SetColor( COLORS::eWHITE );
	else
		gl_SetColor( COLORS::eLIGHTGREY );
	glBegin(GL_TRIANGLES);
		glVertex2i( x, y );
		glVertex2i( x+kWidgetSize/2, y );
		glVertex2i( x, y+kWidgetSize/2 );
	glEnd();

	// Drag vector
	glBegin(GL_LINES);
		if( mMode == Mode::Dragging )
		{
			gl_SetColor( COLORS::eLIGHTGREY );
			glVertex2i( x, y );
			glVertex2i( ox, oy );
		}
	glEnd();
	
	// X axis
	if( mMode == Mode::Dragging && mAxis == Axis::X )
		glLineWidth(6.0f);
	else
		glLineWidth(2.0f);
	glBegin(GL_LINES);
	gl_SetColor( COLORS::eRED );
	glVertex2i( x, y );
	glVertex2i( x+kWidgetSize, y );
	glEnd();

	// Y axis
	if( mMode == Mode::Dragging && mAxis == Axis::Y )
		glLineWidth(6.0f);
	else
		glLineWidth(2.0f);
	glBegin(GL_LINES);
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

	glLineWidth(2.0f);
}

void GizmoTranslation::OnKeyboard( unsigned char key, int mod )
{
	switch( key )
	{		
	case 13:	// Enter key
	case 27: 	// Escape key
		mMode = Mode::NotDragging;	break;	// Clear widget
	}
}

void GizmoTranslation::OnMouseMotion( const MouseMotion& mm )
{
	if( mMode == Mode::NotDragging || mMode == Mode::DoneDragging )
	{
		if( Lair::GetInputMan()->GetMouseButtonState(0).bState )
		{
			mMode = Mode::Dragging;
			mPos = mEditor->GetGrid()->Snap(mm.pos);
		}
	}
	else if( mMode == Mode::Dragging )
	{
		Vector2 vPos = mEditor->GetGrid()->Snap(mm.pos);
		
		if( mAxis == Axis::X )
			mPos.x = vPos.x;
		else if( mAxis == Axis::Y )
			mPos.y = vPos.y;
		else
			mPos = vPos;
	}
}

void GizmoTranslation::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	//fixme: should handle the delta position to correct the glitch on click
	if( state )
	{
		//fixme: should check if we clicked on gizmo
		if( fabsf( mm.pos.x-mPos.x) < 2.0f && fabsf( mm.pos.y-mPos.y) > 8.0f )
			mAxis = Axis::Y;
		else if( fabsf( mm.pos.y-mPos.y) < 2.0f && fabsf( mm.pos.x-mPos.x) > 8.0f )
			mAxis = Axis::X;
		else
			mAxis = Axis::Both;

		mMode = Mode::Dragging;
		mOrigin = mPos;
		mPos = mEditor->GetGrid()->Snap(mm.pos);
	}
	else
	{
		mMode = Mode::DoneDragging;
	}
}