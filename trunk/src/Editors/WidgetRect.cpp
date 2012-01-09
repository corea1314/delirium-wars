#include "WidgetRect.h"

#include "Lair/Lair.h"
#include "Editor.h"
#include "Grid.h"
#include "gfx.h"

WidgetRect::WidgetRect( Editor* inEditor ) : mMode(Mode::NotDragging), mEditor(inEditor)
{
}

void WidgetRect::OnRender()
{
	if( mMode == Mode::Dragging || mMode == Mode::DoneDragging )
	{
		glBegin(GL_LINE_LOOP);
			glVertex2f( mRect[0].x, mRect[0].y );
			glVertex2f( mRect[1].x, mRect[0].y );
			glVertex2f( mRect[1].x, mRect[1].y );
			glVertex2f( mRect[0].x, mRect[1].y );
		glEnd();
	}
}

void WidgetRect::OnRenderGUI()
{
	if( mMode == Mode::Dragging || mMode == Mode::DoneDragging )
	{
		int x,y;
		mEditor->EditorToScreen(mRect[0],x,y);
		gl_RenderText( x,y, "(%0.2f,%0.2f)", mRect[0].x, mRect[0].y );
		
		mEditor->EditorToScreen(mRect[1],x,y);
		gl_RenderText( x,y, "(%0.2f, %0.2f)", mRect[1].x, mRect[1].y );

		Vector2 vCenter = (mRect[0]+mRect[1])/2.0f;
		mEditor->EditorToScreen(vCenter,x,y);
		gl_RenderText( x,y, "[%0.2f x %0.2f]", fabsf(mRect[0].x-mRect[1].x), fabsf(mRect[0].y-mRect[1].y) );
	}
}

void WidgetRect::OnKeyboard( unsigned char key, int mod )
{
	switch( key )
	{
		
	case 13:	// Enter key
	case 27: 	// Escape key
		mMode = Mode::NotDragging;	break;	// Clear widget
	}
}

void WidgetRect::OnMouseMotion( const MouseMotion& mm )
{
	if( mMode == Mode::NotDragging || mMode == Mode::DoneDragging )
	{
		if( Lair::GetInputMan()->GetMouseButtonState( InputMan::MouseButton::Left ).bState )
		{
			mMode = Mode::Dragging;
			mRect[0] = mEditor->GetGrid()->Snap(mm.pos);
		}
	}
	else if( mMode == Mode::Dragging )
	{
		mRect[1] = mEditor->GetGrid()->Snap(mm.pos);
	}
}

void WidgetRect::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	if( state )
	{
		mMode = Mode::Dragging;
		mRect[0] = mEditor->GetGrid()->Snap(mm.pos);
		mRect[1] = mEditor->GetGrid()->Snap(mm.pos);
	}
	else
	{
		mMode = Mode::DoneDragging;
	}
}