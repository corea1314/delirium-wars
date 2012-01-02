#include "GizmoRotation.h"

#include "Lair/Lair.h"
#include "Editor.h"
#include "Grid.h"
#include "gfx.h"

static const int kWidgetSize	=	64;
static const int kWidgetSize2	=	(64*7)/10;

GizmoRotation::GizmoRotation( Editor* inEditor ) : mMode(Mode::NotDragging), mEditor(inEditor)
{
}

void GizmoRotation::OnRender()
{
}

void GizmoRotation::OnRenderGUI()
{
	int x,y;
	mEditor->EditorToScreen(mPos,x,y);

	int ax,ay;
	mEditor->EditorToScreen(mAnchor,ax,ay);

	// Diamond
	if( mMode == Mode::Dragging )
		gl_SetColor( COLORS::eWHITE );
	else
		gl_SetColor( COLORS::eLIGHTGREY );
	glBegin(GL_TRIANGLE_FAN);
	glVertex2i( x, y );
	glVertex2i( x, y+kWidgetSize/2 );				//U
	glVertex2i( x+kWidgetSize2/2, y+kWidgetSize2/2 );
	glVertex2i( x+kWidgetSize/2, y );				//L
	glVertex2i( x+kWidgetSize2/2, y-kWidgetSize2/2 );
	glVertex2i( x, y-kWidgetSize/2 );				//D
	glVertex2i( x-kWidgetSize2/2, y-kWidgetSize2/2 );
	glVertex2i( x-kWidgetSize/2, y );				//R
	glVertex2i( x-kWidgetSize2/2, y+kWidgetSize2/2 );
	glVertex2i( x, y+kWidgetSize/2 );				//U
	glEnd();

	// Drag vector
	glBegin(GL_LINES);
	if( mMode == Mode::Dragging )
	{
		gl_SetColor( COLORS::eLIGHTGREY );
		glVertex2i( x, y );
		glVertex2i( ax, ay );
	}
	glEnd();

	glBegin(GL_LINES);
	gl_SetColor( COLORS::eRED );
	glVertex2i( x, y );
	glVertex2i( x+(int)(mDir.x*kWidgetSize), (int)(y+mDir.y*kWidgetSize) );
	glEnd();


	// Text
	gl_SetColor( COLORS::eWHITE );
	if( mMode == Mode::Dragging )
	{		
		Vector2 vDelta = (mPos-mAnchor);
		gl_RenderText( x+16, y-16, "(%0.2f)", (float)RAD_TO_DEG(mAngle) );
	}

	glLineWidth(2.0f);
}

void GizmoRotation::OnKeyboard( unsigned char key, int mod )
{
	switch( key )
	{
		
	case 13:	// Enter key
	case 27: 	// Escape key
		mMode = Mode::NotDragging;	break;	// Clear widget
	}
}

void GizmoRotation::OnMouseMotion( const Vector2& pos, const Vector2& delta, int mod )
{
	if( mMode == Mode::Dragging )
	{
		mAnchor = mEditor->GetGrid()->Snap(pos);
		mDir = mAnchor - mPos;
		Vector2::Normalize(mDir);
		mAngle = atan2( mDir.y, mDir.x );
	}
}

void GizmoRotation::OnMouseClick( int button, int state, const Vector2& pos, int mod )
{
	if( state )
	{
		//fixme: should check if we clicked on gizmo
		mAnchor = mEditor->GetGrid()->Snap(pos);
		mDir = mAnchor - mPos;
		Vector2::Normalize(mDir);
		mAngle = atan2( mDir.y, mDir.x );

		mMode = Mode::Dragging;
	}
	else
	{
		mMode = Mode::DoneDragging;
	}	
}