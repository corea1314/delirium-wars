

#include "VisualEditor.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"
#include "Lair/Input/Input.h"

static const char* OPENFILE_BCF_EXTENSION = "bcf";
static const char* OPENFILE_BCF_FILTER = "Bezier Curve File\0*.bcf\0";

static const char* OPENFILE_TGA_EXTENSION = "tga";
static const char* OPENFILE_TGA_FILTER = "Targa Image\0*.tga\0";

static const int kMinDeltaTrackPosY	= 16;
static const int kMinDeltaFramePosX	= 16;

static const int kStartTrackPosX	= 64;
static const int kStartTrackPosY	= 34;

static const int kKeySize			= 12;
static const int kTrackLineSize		= 14;

static const int kMarkFrameCount	= 15;	// mark very 15 frames


void VisualEditor::OnInit()
{
	GetCamera()->GetPos().Set( 256.0f, 256.0f );

	mFirstFrame = 0;
	mFirstFrameDelta = 0.0f;
	mKeyValueScale = 1.0f;
	mCurrFrame = 0;

	mTracks[0].Set( "pos:x", kStartTrackPosY+kMinDeltaTrackPosY*0);	mTracks[0].mCurve.AddKey(0,0); mTracks[0].mCurve.AddKey(14,14);	mTracks[0].mCurve.AddKey(29,29); mTracks[0].mCurve.AddKey(59,59);
	mTracks[1].Set( "pos:y", kStartTrackPosY+kMinDeltaTrackPosY*1);	mTracks[1].mCurve.AddKey(0,0); mTracks[1].mCurve.AddKey( 1, 1);	mTracks[1].mCurve.AddKey( 2, 2); mTracks[1].mCurve.AddKey(39,39);
	mTracks[2].Set( "angle", kStartTrackPosY+kMinDeltaTrackPosY*2);	mTracks[2].mCurve.AddKey(0,0); mTracks[2].mCurve.AddKey( 2, 2);	mTracks[2].mCurve.AddKey( 4, 4); mTracks[2].mCurve.AddKey(29,29);
	mTracks[3].Set( "alpha", kStartTrackPosY+kMinDeltaTrackPosY*3);	mTracks[3].mCurve.AddKey(0,0); mTracks[3].mCurve.AddKey( 4, 4);	mTracks[3].mCurve.AddKey( 8, 8); mTracks[3].mCurve.AddKey(49,49);
}

void VisualEditor::OnExit()
{
	glPointSize( 8.0f );
	glLineWidth( 2.0f );
}

void VisualEditor::ClearSelection() 
{ 
	mSelectedKeys.clear(); 
	mTracks[0].bSelected = false;
	mTracks[1].bSelected = false;
	mTracks[2].bSelected = false;
	mTracks[3].bSelected = false;
	mTracks[4].bSelected = false;
	mTracks[5].bSelected = false;
}

int VisualEditor::KeyToScreen( int inPosition )
{
	return kStartTrackPosX + (inPosition - mFirstFrame) * kMinDeltaFramePosX;
}

int VisualEditor::ScreenToKey( int inPosition )
{
	return mFirstFrame + ((inPosition + kMinDeltaFramePosX/2 - kStartTrackPosX) / kMinDeltaFramePosX);
}

void VisualEditor::OnRender()
{
	GetGrid()->Render();
}

void VisualEditor::RenderTrack( Track& inTrack, int inPosY )
{
	if( inTrack.bSelected )
		gl_SetColor(COLORS::eORANGE);
	else
		gl_SetColor(COLORS::eDARKGREY);
	glBegin( GL_LINES );
		glVertex2f( (GLfloat)KeyToScreen(mFirstFrame), (GLfloat)inPosY );
		glVertex2f( 1280.0f, (GLfloat)inPosY );
	glEnd();
		
	if( inTrack.bSelected )
		gl_SetColor(COLORS::eRED);
	else
		gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, inPosY, "%s", inTrack.mName );	//fixme 8
}

void VisualEditor::RenderTrackKeys( Track& inTrack, int inPosY )
{
	gl_SetColor(COLORS::eGREY);
	glBegin( GL_POINTS );
	for( unsigned int i=0; i<inTrack.mCurve.GetKeyCount(); i++ )
	{
		glVertex2f( (GLfloat)KeyToScreen( inTrack.mCurve.GetKey(i).mPosition), (GLfloat)inPosY );
	}
	glEnd();
}

void VisualEditor::RenderSelectedTrackKeys()
{
	gl_SetColor(COLORS::eRED);
	glBegin( GL_POINTS );
	for( unsigned int i=0; i<mSelectedKeys.size(); i++ )
	{
		glVertex2f( (GLfloat)KeyToScreen(mSelectedKeys[i].mTrack->mCurve.GetKey(mSelectedKeys[i].mKey).mPosition), (GLfloat)mSelectedKeys[i].mTrack->mPosY );
	}
	glEnd();
}

void VisualEditor::RenderCurve( Curve& inCurve, int inPosY )
{
	if( inCurve.GetKeyCount() < 2 )
		return;	// not rendering imcomplete curves

	int i = 0;

	float SPAN = (float)(inCurve.GetKey(inCurve.GetKeyCount()-1).mPosition - inCurve.GetKey(0).mPosition);
	float START_POS = 0.0f;
	float MID_START_POS = (float)(inCurve.GetKey(0).mPosition);
	float MID_END_POS = (float)(inCurve.GetKey(inCurve.GetKeyCount()-1).mPosition);
	float END_POS = (float)(inCurve.GetKey(inCurve.GetKeyCount()-1).mPosition + SPAN);

	glLineWidth( 1.0f );

	glPushMatrix();
		
		glTranslatef( (GLfloat)KeyToScreen(0), (GLfloat)(kStartTrackPosY + 5 * kMinDeltaTrackPosY), 0.0f );	// offset on y //fixme 5
		glScalef( (GLfloat)kMinDeltaFramePosX, mKeyValueScale, 1.0f );		// scale on y //fixme 8.0f
				
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
			glVertex2f( (GLfloat)inCurve.GetKey(i).mPosition, inCurve.GetKey(i).mValue );
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
	glLineWidth( (GLfloat)kTrackLineSize );

	int nPosY = kStartTrackPosY;

	for( int i=0; i<6; i++ )
	{
		if( mTracks[i].mCurve.GetKeyCount() )
		{
			RenderTrack( mTracks[i], nPosY );
			nPosY += kMinDeltaTrackPosY;
		}
	}

	// Render first frame marker
	glLineWidth( 6.0f );
	gl_SetColor(COLORS::eBLACK);
	glBegin( GL_LINES );
		glVertex2f( (GLfloat)KeyToScreen(0), (GLfloat)(kStartTrackPosY - kMinDeltaTrackPosY/2) );
		glVertex2f( (GLfloat)KeyToScreen(0), (GLfloat)(kStartTrackPosY + 4 * kMinDeltaTrackPosY - kMinDeltaTrackPosY/2) ); //fixme 4
	glEnd();

	// Render fix distance marker
	gl_SetColor(COLORS::eLIGHTGREY);
	glBegin( GL_LINES );
		int nStartPos = KeyToScreen( kMarkFrameCount-1 );
		int nNextPos =  KeyToScreen( 2*kMarkFrameCount-1 );
		int nDeltaPos = nNextPos-nStartPos;

		for( int i=nStartPos; i<1280; i+=nDeltaPos )	//fixme 1280
		{
			glVertex2f( (GLfloat)i, (GLfloat)(kStartTrackPosY - kKeySize/2) );
			glVertex2f( (GLfloat)i, (GLfloat)(kStartTrackPosY + 4 * kMinDeltaTrackPosY - kMinDeltaTrackPosY/2) ); //fixme 4
		}

		int nCurrFramePosX = KeyToScreen( mCurrFrame );

		glLineWidth( 6.0f );
		gl_SetColor(COLORS::eGREEN);
		glVertex2f( (GLfloat)nCurrFramePosX, (GLfloat)(kStartTrackPosY - kKeySize/2) );
		glVertex2f( (GLfloat)nCurrFramePosX, (GLfloat)(kStartTrackPosY + 4 * kMinDeltaTrackPosY - kMinDeltaTrackPosY/2) ); //fixme 4

	glEnd();

	// Render keys	
	glPointSize( (GLfloat)kKeySize );

	nPosY = kStartTrackPosY;

	for( int i=0; i<6; i++ )
	{
		if( mTracks[i].mCurve.GetKeyCount() )
		{
			RenderTrackKeys( mTracks[i], nPosY );
			nPosY += kMinDeltaTrackPosY;
		}
	}
	
	RenderSelectedTrackKeys();

	glPointSize( 8.0f );

	for( int i=0; i<6; i++ )
	{
		RenderCurve( mTracks[i].mCurve, kMinDeltaTrackPosY );
	}
	glLineWidth( 2.0f );
}

void VisualEditor::OnMouseClick( int button, int x, int y, int mod )
{
	Vector2 v;	
	ScreenToEditor( x, y, v );

	if( (y < 4 * kMinDeltaTrackPosY + kStartTrackPosY) && !(mod & SK_MOD_SHIFT) )	//fixme 4
		OnMouseClickTrackArea( button, x, y );

	switch(button)
	{
	case 0:		
		{	
			if( y < 4 * kMinDeltaTrackPosY + kStartTrackPosY  )	//fixme 4
			{
				if( Lair::GetInputMan()->GetMouseButtonState(0).bState && !(mod & SK_MOD_SHIFT) )
				{
					// Select track
					int nTrackIndex = (y-kStartTrackPosY+kMinDeltaTrackPosY/2)/kMinDeltaTrackPosY;
					
					mTracks[nTrackIndex].bSelected = true;

					// Select frame

					mCurrFrame = ScreenToKey( x );
				}
			}
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

void VisualEditor::OnMouseClickTrackArea( int button, int x, int y )
{
	Vector2 vKeyPos;
	const Vector2 vClickPos((float)x,(float)y);
	Vector2 vDelta;

	const float fKeyRadiusSquared = (kKeySize/2)*(kKeySize/2);

	for( int i=0; i<6; i++ ) //fixme 6
	{
		for( unsigned int j=0; j<mTracks[i].mCurve.GetKeyCount(); j++ )
		{
			vKeyPos.Set( (float)KeyToScreen( mTracks[i].mCurve.GetKey(j).mPosition), (float)(kStartTrackPosY + i*kMinDeltaTrackPosY) );
			vDelta = vKeyPos - vClickPos;

			if( vDelta.GetLengthSquare() < fKeyRadiusSquared )
			{
				// found one
				mSelectedKeys.push_back( KeySelection(&mTracks[i],j) );
			}
		}
	}
}


void VisualEditor::OnMouseMotion( int x, int y, int dx, int dy, int mod )
{
	/*
	Vector2 last; 
	ScreenToEditor( x-dx, y-dy, last );

	Vector2 v;	
	ScreenToEditor( x, y, v );
	Vector2 d = v - last;
	*/

	if( y < 4 * kMinDeltaTrackPosY + kStartTrackPosY  )	//fixme 4
	{
		if( Lair::GetInputMan()->GetMouseButtonState(0).bState  && (mod & SK_MOD_SHIFT) )
		{
			// Scroll key tracks
			mFirstFrameDelta -= ( dx / (float)kMinDeltaFramePosX );
			mFirstFrame = (int)mFirstFrameDelta;
		}		
	}
	else
	{
		if( x < 32 ) // fixme 32
		{
			// Scale curve values
			mKeyValueScale += dy / 10.0f;

			if( mKeyValueScale < 0.5f ) //fixme 0.5f
				mKeyValueScale = 0.5f;
		}
		else
		{
			Editor::OnMouseMotion(x,y,dx,dy,mod);
		}		
	}
}

void VisualEditor::OnSpecialKey( int key, int mod )
{
	Editor::OnSpecialKey(key, mod);

	switch( key )
	{
	case SK_LEFT:	mCurrFrame--;	mCurrFrame = std::max( 0, mCurrFrame ); break;
	case SK_RIGHT:	mCurrFrame++;	break;
	}
}

void VisualEditor::OnKeyboard( unsigned char key, int mod )
{
	Editor::OnKeyboard(key, mod);

	switch( key )
	{
	case 'k':	// Keyframe selected tracks
	case 'K':	// Keyframe all tracks
		break;
	case 27:	// Escape key
		ClearSelection();	
		break;
	}
}

void VisualEditor::OnCreateMenu()
{	
	CREATE_MENU( pFile, "  File...  " );
		ADD_MENU_ITEM( pFile, "  Save  ", &VisualEditor::OnMenuFileSave, 0 );
		ADD_MENU_ITEM( pFile, "  Load  ", &VisualEditor::OnMenuFileSave, 0 );
		
//	ADD_MENU_ITEM( GetMenu(), "Animate",	&VisualEditor::OnMenuAnimate, 0 );
//	ADD_MENU_ITEM( GetMenu(), "Texture",	&VisualEditor::OnMenuTexture, 0 );	
}

void VisualEditor::OnMenuFileSave( int inUnused )
{

}

void VisualEditor::OnMenuFileLoad( int inUnused )
{

}

