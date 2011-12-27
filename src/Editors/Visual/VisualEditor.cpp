

#include "VisualEditor.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"

static const char* OPENFILE_BCF_EXTENSION = "bcf";
static const char* OPENFILE_BCF_FILTER = "Bezier Curve File\0*.bcf\0";

static const char* OPENFILE_TGA_EXTENSION = "tga";
static const char* OPENFILE_TGA_FILTER = "Targa Image\0*.tga\0";

static const int kMinDeltaTrackPosY	= 16;
static const int kMinDeltaFramePosX	= 16;

static const int kStartTrackPosX	= 64;
static const int kStartTrackPosY	= 32;

static const int kKeySize			= 14;
static const int kTrackLineSize		= 14;

static const int kMarkFrameCount	= 15;	// mark very 15 frames


void VisualEditor::OnInit()
{
	GetCamera()->GetPos().Set( 256.0f, 256.0f );

	mFirstFrame = 0;
	mFirstFrameDelta = 0.0f;
	mKeyValueScale = 1.0f;
	mCurrFrame = 0;

	mTracks[0].mName = "pos:x";	mTracks[0].mCurve.AddKey(0,0); mTracks[0].mCurve.AddKey(14,14);	mTracks[0].mCurve.AddKey(29,29); mTracks[0].mCurve.AddKey(59,59);
	mTracks[1].mName = "pos:y";	mTracks[1].mCurve.AddKey(0,0); mTracks[1].mCurve.AddKey( 1, 1);	mTracks[1].mCurve.AddKey( 2, 2); mTracks[1].mCurve.AddKey(39,39);
	mTracks[2].mName = "angle";	mTracks[2].mCurve.AddKey(0,0); mTracks[2].mCurve.AddKey( 2, 2);	mTracks[2].mCurve.AddKey( 4, 4); mTracks[2].mCurve.AddKey(29,29);
	mTracks[3].mName = "alpha";	mTracks[3].mCurve.AddKey(0,0); mTracks[3].mCurve.AddKey( 4, 4);	mTracks[3].mCurve.AddKey( 8, 8); mTracks[3].mCurve.AddKey(49,49);

	class TrackType { enum E { PositionX, PositionY, Angle, Alpha, ScaleX, ScaleY } ; };
}

void VisualEditor::OnExit()
{
	glPointSize( 8.0f );
	glLineWidth( 2.0f );
}

int VisualEditor::KeyToScreen( int inPosition )
{
	return kStartTrackPosX + (inPosition - mFirstFrame) * kMinDeltaFramePosX;
}

void VisualEditor::OnRender()
{

}

void VisualEditor::RenderTrack( Track& inTrack, int inPosY )
{
	gl_SetColor(COLORS::eDARKGREY);
	glBegin( GL_LINES );
		glVertex2f( KeyToScreen(mFirstFrame), inPosY );
		glVertex2f( 1280.0f, inPosY );
	glEnd();
		
	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, inPosY, "%s", inTrack.mName );	//fixme 8
}

void VisualEditor::RenderTrackKeys( Track& inTrack, int inPosY )
{
	gl_SetColor(COLORS::eGREY);
	glBegin( GL_POINTS );
	for( unsigned int i=0; i<inTrack.mCurve.GetKeyCount(); i++ )
	{
		glVertex2f( KeyToScreen( inTrack.mCurve.GetKey(i).mPosition), inPosY );
	}
	glEnd();
}

void VisualEditor::RenderCurve( Curve& inCurve, int inPosY )
{
	if( inCurve.GetKeyCount() < 2 )
		return;	// not rendering imcomplete curves

	int i = 0;

	float SPAN = inCurve.GetKey(inCurve.GetKeyCount()-1).mPosition - inCurve.GetKey(0).mPosition;
	float START_POS = 0.0f;
	float MID_START_POS = inCurve.GetKey(0).mPosition;
	float MID_END_POS = inCurve.GetKey(inCurve.GetKeyCount()-1).mPosition;
	float END_POS = inCurve.GetKey(inCurve.GetKeyCount()-1).mPosition + SPAN;

	glLineWidth( 1.0f );

	glPushMatrix();
		
		glTranslatef( KeyToScreen(0), kStartTrackPosY + 5 * kMinDeltaTrackPosY, 0.0f );	// offset on y //fixme 5
		glScalef( kMinDeltaFramePosX, mKeyValueScale, 1.0f );		// scale on y //fixme 8.0f
				
		// -inf to first key
		glBegin( GL_LINE_STRIP );
		gl_SetColor( COLORS::eDARKGREY );
		for(float t=START_POS;t<MID_START_POS; t+= 0.01f )		
			glVertex2f( t, inCurve.Evaluate(t) );

		// first keys to last key
		gl_SetColor( COLORS::eWHITE );
		for(float t=MID_START_POS;t<MID_END_POS; t+= 0.01f )		
			glVertex2f( t, inCurve.Evaluate(t) );

		// last key to +inf
		gl_SetColor( COLORS::eDARKGREY );
		for(float t=MID_END_POS;t<END_POS; t+= 0.01f )		
			glVertex2f( t, inCurve.Evaluate(t) );
		glEnd();
		glVertex2f( END_POS, inCurve.Evaluate(END_POS) );


		// Tangents
		/*
		gl_SetColor( COLORS::eGREEN );
		glBegin( GL_LINES );
		for(unsigned int i=0;i<mCurve.GetKeyCount();i++)
		{
			Vector2 ta =mCurve.GetKey(i).mTangentInVector;
			glVertex2f( mCurve.GetKey(i).mPosition, mCurve.GetKey(i).mValue );
			glVertex2f( mCurve.GetKey(i).mPosition + ta.x/10.0f, mCurve.GetKey(i).mValue + ta.y/10.0f );
		}
		glEnd();
		gl_SetColor( COLORS::eRED );
		glBegin( GL_LINES );
		for(unsigned int i=0;i<mCurve.GetKeyCount();i++)
		{
			Vector2 ta =mCurve.GetKey(i).mTangentOutVector;
			glVertex2f( mCurve.GetKey(i).mPosition, mCurve.GetKey(i).mValue );
			glVertex2f( mCurve.GetKey(i).mPosition + ta.x/10.0f, mCurve.GetKey(i).mValue + ta.y/10.0f );
		}
		glEnd();
		*/

		// Keys
		gl_SetColor( COLORS::eGREY );
		glBegin( GL_POINTS );
		for(unsigned int i=0;i<inCurve.GetKeyCount();i++)
			glVertex2f( inCurve.GetKey(i).mPosition, inCurve.GetKey(i).mValue );
		glEnd();

	glPopMatrix();

	glLineWidth( 2.0f );
}

void VisualEditor::OnRenderGUI()
{
	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %f -- Grid: %d -- Snap: %s", GetCamera()->GetZoom(), GetGrid()->GetGridSize(), GetGrid()->GetSnap() ? "true" : "false" );
	gl_RenderText( 8, 720-16-8, "Visual Editor v%d.%d.%d (%s at %s)", CRV_VERSION_MAIN, CRV_VERSION_MAJOR, CRV_VERSION_MINOR, __DATE__, __TIME__ );

	// Render tracks
	glLineWidth( kTrackLineSize );

	int nPosY = kStartTrackPosY;

	for( int i=0; i<6; i++ )
	{
		if( mTracks[i].mCurve.GetKeyCount() )
		{
			RenderTrack( mTracks[i], nPosY );
			nPosY += kMinDeltaTrackPosY;
		}
	}

	glLineWidth( 2.0f );
	
	// Render first frame marker
	gl_SetColor(COLORS::eBLACK);
	glBegin( GL_LINES );
	glVertex2f( KeyToScreen(0), kStartTrackPosY - kKeySize/2 );
	glVertex2f( KeyToScreen(0), kStartTrackPosY + 4 * kMinDeltaTrackPosY - kKeySize/2 ); //fixme 4
	glEnd();

	// Render fix distance marker
	gl_SetColor(COLORS::eLIGHTGREY);
	glBegin( GL_LINES );
		int nStartPos = KeyToScreen( kMarkFrameCount-1 );
		int nNextPos =  KeyToScreen( 2*kMarkFrameCount-1 );
		int nDeltaPos = nNextPos-nStartPos;

		for( int i=nStartPos; i<1280; i+=nDeltaPos )	//fixme 1280
		{
			glVertex2f( i, kStartTrackPosY - kKeySize/2 );
			glVertex2f( i, kStartTrackPosY + 4 * kMinDeltaTrackPosY - kKeySize/2 ); //fixme 4
		}		
	glEnd();

	// Render keys	
	glPointSize( kKeySize );

	nPosY = kStartTrackPosY;

	for( int i=0; i<6; i++ )
	{
		if( mTracks[i].mCurve.GetKeyCount() )
		{
			RenderTrackKeys( mTracks[i], nPosY );
			nPosY += kMinDeltaTrackPosY;
		}
	}
	glPointSize( 8.0f );

	for( int i=0; i<6; i++ )
	{
		RenderCurve( mTracks[i].mCurve, kMinDeltaTrackPosY );
	}
}

void VisualEditor::OnMouseClick( int button, int x, int y )
{
	Vector2 v;	
	ScreenToEditor( x, y, v );

	switch(button)
	{
	case 0:		
		{		
		}
		break;
	case 1:
		{
		}
		break;
	case 2:		
		{
		}
		break;
	}
}

void VisualEditor::OnMouseMotion( int x, int y, int dx, int dy )
{
	if( y < 4 * kMinDeltaTrackPosY + kStartTrackPosY  )	//fixme 4
	{
		if( Lair::GetInputMan()->GetMouseButtonState(0).bState )
		{
			mFirstFrameDelta -= ( dx / (float)kMinDeltaFramePosX );
			mFirstFrame = (int)mFirstFrameDelta;


			mKeyValueScale += dy / 10.0f;

			if( mKeyValueScale < 0.5f ) //fixme 0.5f
				mKeyValueScale = 0.5f;
		}

		/*
		Vector2 last; 
		ScreenToEditor( x-dx, y-dy, last );

		Vector2 v;	
		ScreenToEditor( x, y, v );
		Vector2 d = v - last;
		*/
	}
	else
	{
		Editor::OnMouseMotion(x,y,dx,dy);
	}
}

void VisualEditor::OnCreateMenu()
{	
	CREATE_MENU( pFile, "File..." );
		ADD_MENU_ITEM( pFile, "Save", &VisualEditor::OnMenuFileSave, 0 );
		ADD_MENU_ITEM( pFile, "Load", &VisualEditor::OnMenuFileSave, 0 );
		
//	ADD_MENU_ITEM( GetMenu(), "Animate",	&VisualEditor::OnMenuAnimate, 0 );
//	ADD_MENU_ITEM( GetMenu(), "Texture",	&VisualEditor::OnMenuTexture, 0 );	
}

void VisualEditor::OnMenuFileSave( int inUnused )
{

}

void VisualEditor::OnMenuFileLoad( int inUnused )
{

}

