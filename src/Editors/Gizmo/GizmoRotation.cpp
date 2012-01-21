#include "GizmoRotation.h"

#include "Lair/Lair.h"
#include "../Editor.h"
#include "../Grid.h"
#include "gfx.h"

#include "Editors/Animatable/Animatable.h"

#include <stdlib.h>
#include <float.h>

static const int kWidgetSize	=	64;
static const int kWidgetSize2	=	(64*7)/10;
static const int kWidgetCirclePointCount = 16;

GizmoRotation::GizmoRotation( Editor* inEditor ) : Gizmo(inEditor), mMode(Mode::NotDragging)
{
	mAngle = 0.0f;

	int i;
	float fAngle;
	float fDeltaAngle = (float)(2*PI/kWidgetCirclePointCount);
	for( fAngle=0.0f, i=0; fAngle<2*PI; fAngle+=fDeltaAngle, i++ )
	{
		mVertexBuffer[i].x = cos(fAngle) * kWidgetSize;
		mVertexBuffer[i].y = sin(fAngle) * kWidgetSize;
		mVertexBuffer[i].z = 0.0f;
		mVertexBuffer[i].color = COLORS::eYELLOW;
	}
	for( fAngle=0.0f, i=GIZMO_CIRCLE_POINT_COUNT; fAngle<2*PI; fAngle+=fDeltaAngle, i++ )
	{
		mVertexBuffer[i].x = cos(fAngle) * kWidgetSize/2;
		mVertexBuffer[i].y = sin(fAngle) * kWidgetSize/2;
		mVertexBuffer[i].z = 0.0f;
		mVertexBuffer[i].color = COLORS::eORANGE;
	}
}

void GizmoRotation::Init( AnimatableElement* inAnimatable )
{
	mPos = inAnimatable->mPos;
	mAngle = inAnimatable->mAngle;
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

	// Circle
	if( mMode == Mode::Dragging )
		gl_SetColor( COLORS::eWHITE );
	else
		gl_SetColor( COLORS::eLIGHTGREY );

	glPushMatrix();
		glTranslatef( (float)x, (float)y, 0.0f );
		gl_RenderVB( GL_LINE_LOOP, mVertexBuffer, 0, GIZMO_CIRCLE_POINT_COUNT );
		gl_RenderVB( GL_LINE_LOOP, mVertexBuffer, GIZMO_CIRCLE_POINT_COUNT, GIZMO_CIRCLE_POINT_COUNT );
	glPopMatrix();

	// Drag vector
	glBegin(GL_LINES);
		// Display angle line
		gl_SetColor( COLORS::eYELLOW );
		glVertex2i( x, y );
		glVertex2i( x+(GLint)(cos(mAngle)*kWidgetSize), y+(GLint)(sin(mAngle)*kWidgetSize) );

		// Display drag line
		if( mMode == Mode::Dragging )
		{
			gl_SetColor( COLORS::eLIGHTGREY );
			glVertex2i( x, y );
			glVertex2i( ax, ay );		

			// Click angle line
			gl_SetColor( COLORS::eDARKGREY );
			glVertex2i( x, y );
			glVertex2i( x+(int)(mClickDir.x*kWidgetSize), (int)(y+mClickDir.y*kWidgetSize) );
		}
	glEnd();
	
	// Text
	gl_SetColor( COLORS::eWHITE );
	if( mMode == Mode::Dragging )
	{		
		Vector2 vDelta = (mPos-mAnchor);
		float fDelta = (float)(Vector2::AngleBetweenVectors( mClickDir, mDir ));
		gl_RenderText( x+16, y-16, "%0.2f -> (%0.2f)", (float)RAD_TO_DEG(fDelta), (float)RAD_TO_DEG(mAngle) );
	}
	else
	{
		// Display text entry
		if( mTextEntry.size() != 0 )
			gl_RenderText( x-16, y-48, "(%s)", mTextEntry.c_str() );
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
	
	if( mMode != Mode::Dragging )
		Gizmo::OnKeyboard( key, mod );
}

void GizmoRotation::OnMouseMotion( const MouseMotion& mm )
{
	if( mMode == Mode::Dragging && Lair::GetInputMan()->IsMouseButtonDown( InputMan::MouseButton::Left ) )
	{
		Vector2 vOldDir = mDir;
		mAnchor = mEditor->GetGrid()->Snap(mm.pos);
		mDir = mAnchor - mPos;
		Vector2::Normalize(mDir);

		float fDelta = (float)(Vector2::AngleBetweenVectors( vOldDir, mDir ));

		mAngle += fDelta;

		mEditor->OnRotate( mAngle, fDelta );
	}
}

void GizmoRotation::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	if( button == InputMan::MouseButton::Left && state )
	{
		const float kWidgetRadius = kWidgetSize * mEditor->GetCamera()->GetZoom();

		Vector2 vDist = mm.pos - mPos;

		if( vDist.GetLengthSquare() < kWidgetRadius*kWidgetRadius )
		{
			mAnchor = mEditor->GetGrid()->Snap(mm.pos);
			mClickDir = mAnchor - mPos;
			Vector2::Normalize(mClickDir);
			mDir = mClickDir;
			mMode = Mode::Dragging;
		}		
	}
	else
	{
		mMode = Mode::DoneDragging;
	}	
}

void GizmoRotation::ProcessTextEntry( const std::string& inText )
{
	float a;
	if( sscanf_s( inText.c_str(), "%f", &a ) == 1 )
	{
		if( _isnan(a) == 0 )
		{
			float fOldAngle = mAngle;
			mAngle = (float)DEG_TO_RAD(a);
			mEditor->OnRotate( mAngle, mAngle-fOldAngle );
		}
	}
}