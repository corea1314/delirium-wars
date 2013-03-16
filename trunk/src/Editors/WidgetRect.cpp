#include "WidgetRect.h"

#include "Lair/Lair.h"
#include "Editor.h"
#include "Grid.h"
#include "gfx.h"

const int kHalfHandleSize =	(16/2);

WidgetRect::WidgetRect( Editor* inEditor ) : mMode(Mode::NotDragging), mEditor(inEditor)
{
}

void AddHandle( int x, int y )
{
	glBegin(GL_LINE_LOOP);
		glVertex2i( x-kHalfHandleSize, y-kHalfHandleSize );
		glVertex2i( x+kHalfHandleSize, y-kHalfHandleSize );
		glVertex2i( x+kHalfHandleSize, y+kHalfHandleSize );
		glVertex2i( x-kHalfHandleSize, y+kHalfHandleSize );
	glEnd();
}

void WidgetRect::OnRender()
{
	if( mMode == Mode::Dragging || mMode == Mode::DoneDragging )
	{
		gl_SetColor( COLORS::eGREY );
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
		gl_SetColor( COLORS::eGREY );

		int x0,y0,x1,y1;
		
		mEditor->EditorToScreen(mRect[0],x0,y0);
		mEditor->EditorToScreen(mRect[1],x1,y1);
		
		int sx = (x1-x0);
		int sy = (y1-y0);
		int cx = x0+sx/2;
		int cy = y0+sy/2;
		
		AddHandle(x0,y0);
		AddHandle(x1,y1);
		AddHandle(x0,y1);
		AddHandle(x1,y0);

		AddHandle(x0+sx/2,y0);
		AddHandle(x0+sx/2,y1);
		AddHandle(x0,y0+sy/2);
		AddHandle(x1,y0+sy/2);

		AddHandle(cx,cy);

		gl_SetColor( COLORS::eWHITE );
		gl_RenderText( x0+kHalfHandleSize,y0+kHalfHandleSize, "(%0.2f,%0.2f)", mRect[0].x, mRect[0].y );
		gl_RenderText( x1+kHalfHandleSize,y1+kHalfHandleSize, "(%0.2f, %0.2f)", mRect[1].x, mRect[1].y );
		gl_RenderText( cx+kHalfHandleSize,cy+kHalfHandleSize, "[%0.2f x %0.2f]", fabsf(mRect[0].x-mRect[1].x), fabsf(mRect[0].y-mRect[1].y) );
	}
}

void WidgetRect::OnKeyboard( unsigned char key, int mod, bool down )
{
	if( down ) 
	{
		switch( key )
		{		
		case 13:	// Enter key
		case 27: 	// Escape key
			mMode = Mode::NotDragging;	break;	// Clear widget
		}
	}	
}

void WidgetRect::OnMouseMotion( const MouseMotion& mm )
{
	if( mMode == Mode::NotDragging || mMode == Mode::DoneDragging )
	{
		if( Lair::GetInputMan()->IsMouseButtonDown( InputMan::MouseButton::Left ) )
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