#include "GizmoSelection.h"

#include "Lair/Lair.h"
#include "../Editor.h"
#include "../Grid.h"
#include "gfx.h"

#include "Editors/Animatable/Animatable.h"

#include <stdlib.h>
#include <float.h>

static const int kWidgetSize	=	64;

GizmoSelection::GizmoSelection( Editor* inEditor ) : Gizmo(inEditor), mMode(Mode::NotDragging)
{
}


void GizmoSelection::Init( AnimatableElement* inAnimatable )
{
}


void GizmoSelection::OnRender()
{
}

void GizmoSelection::OnRenderGUI()
{
	int tx,ty;
	mEditor->EditorToScreen(mPos,tx,ty);
	
	int ox,oy;
	mEditor->EditorToScreen(mOrigin,ox,oy);

	// Triangle
	if( mMode == Mode::Dragging )
	{
		gl_SetColor( COLORS::eGREY );
	
		glBegin(GL_LINE_LOOP);
		glVertex2i( ox, oy );
		glVertex2i( tx, oy );
		glVertex2i( tx, ty );
		glVertex2i( ox, ty );
		glEnd();
	}
}

void GizmoSelection::OnKeyboard( unsigned char key, int mod, bool down )
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

void GizmoSelection::OnMouseMotion( const MouseMotion& mm )
{
	if( mMode == Mode::Dragging )
	{
		Vector2 vOldPos = mPos;
		mPos = mEditor->GetGrid()->Snap(mm.pos);
		mDelta = mPos - mOrigin;

		Vector2 vMin, vMax;

		vMin.x = MIN( mPos.x, mOrigin.x );
		vMin.y = MIN( mPos.y, mOrigin.y );
		vMax.x = MAX( mPos.x, mOrigin.x );
		vMax.y = MAX( mPos.y, mOrigin.y );

		mEditor->OnSelectRect( vMin, vMax );
	}
}

void GizmoSelection::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	if( button == InputMan::MouseButton::Left && state )
	{
		mMode = Mode::Dragging;
		mOrigin = mPos = mEditor->GetGrid()->Snap(mm.pos);

		mEditor->OnSelect( mPos );
	}
	else
	{		
		mMode = Mode::DoneDragging;
	}
}

void GizmoSelection::ProcessTextEntry( const std::string& inText )
{
	mEditor->OnSelectTextEntry( inText );
}
