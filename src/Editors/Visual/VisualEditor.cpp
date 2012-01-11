
//FIXME
// - Implement keyframing of single selected track
// - Implement keyframing of multiple selected track
// - Implement insertion/deletion/manipulation of keys

#include "VisualEditor.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"
#include "Lair/Input/Input.h"
#include "Editors/Gizmo/Gizmo.h"

static const int	kMinDeltaTrackPosY	= 16;
static const int	kMinDeltaFramePosX	=  8;

static const int	kStartTrackPosX		= 64;
static const int	kStartTrackPosY		= 34;

static const int	kKeySize			= 12;
static const int	kTrackLineSize		= 14;

static const int	kMarkFrameCount		= 15;	// mark very 15 frames

static const float	kNormalLength		= 32.0f;

void VisualEditor::OnInit()
{
	GetCamera()->GetPos().Set( 256.0f, 256.0f );

	mFirstFrame = 0;
	mFirstFrameDelta = 0.0f;
	mKeyValueScale = 1.0f;
	SetCurrFrame( 0 );

	mIsPlaying = false;
	mCurrTime = 0.0f;
	mFPS = 60;

	mShowCurve = false;

	mTrackInfo[TrackType::PosX ].Set( "pos:x", TrackType::PosX, kStartTrackPosY+kMinDeltaTrackPosY*0);	
	mTrackInfo[TrackType::PosY ].Set( "pos:y", TrackType::PosY, kStartTrackPosY+kMinDeltaTrackPosY*1);	
	mTrackInfo[TrackType::Angle].Set( "angle", TrackType::Angle, kStartTrackPosY+kMinDeltaTrackPosY*2);	
	mTrackInfo[TrackType::Alpha].Set( "alpha", TrackType::Alpha, kStartTrackPosY+kMinDeltaTrackPosY*3);	

	// push in dummy tracks
	mAnimatables.push_back( Animatable() );
	mSelectedAnimatable = &mAnimatables[0];

	mSelectedAnimatable->mCurve[TrackType::PosX ].AddKey(   0,   0.0f ); 
//	mSelectedAnimatable->mCurve[TrackType::PosX ].AddKey(  15,  15.0f );	
//	mSelectedAnimatable->mCurve[TrackType::PosX ].AddKey(  60, 300.0f ); 
//	mSelectedAnimatable->mCurve[TrackType::PosX ].AddKey( 120, 600.0f );

	mSelectedAnimatable->mCurve[TrackType::PosY ].AddKey(   0,   0.0f ); 
//	mSelectedAnimatable->mCurve[TrackType::PosY ].AddKey(  10, 100.0f );	
//	mSelectedAnimatable->mCurve[TrackType::PosY ].AddKey(  60,   0.0f ); 
//	mSelectedAnimatable->mCurve[TrackType::PosY ].AddKey( 120, 200.0f );

	mSelectedAnimatable->mCurve[TrackType::Angle].AddKey(   0,   0.0f ); 
//	mSelectedAnimatable->mCurve[TrackType::Angle].AddKey(  60,  90.0f );	
//	mSelectedAnimatable->mCurve[TrackType::Angle].AddKey(  80,  45.0f );	
//	mSelectedAnimatable->mCurve[TrackType::Angle].AddKey( 120,   0.0f ); 

	mSelectedAnimatable->mCurve[TrackType::Alpha].AddKey(   0,   1.0f ); 
//	mSelectedAnimatable->mCurve[TrackType::Alpha].AddKey(  40,   0.5f );	
//	mSelectedAnimatable->mCurve[TrackType::Alpha].AddKey( 120,   1.0f ); 
}

void VisualEditor::OnExit()
{
	glPointSize( 8.0f );
	glLineWidth( 2.0f );
}

void VisualEditor::ClearSelection() 
{ 
	mSelectedKeys.clear(); 
	mTrackInfo[0].bSelected = false;
	mTrackInfo[1].bSelected = false;
	mTrackInfo[2].bSelected = false;
	mTrackInfo[3].bSelected = false;
	mTrackInfo[4].bSelected = false;
	mTrackInfo[5].bSelected = false;
}

int VisualEditor::KeyToScreen( int inPosition )
{
	return kStartTrackPosX + (inPosition - mFirstFrame) * kMinDeltaFramePosX;
}

int VisualEditor::ScreenToKey( int inPosition )
{
	return mFirstFrame + ((inPosition + kMinDeltaFramePosX/2 - kStartTrackPosX) / kMinDeltaFramePosX);
}

void VisualEditor::SetCurrFrame( int inFrame )
{
	mCurrFrame = inFrame;

	for( unsigned int i=0;i<mAnimatables.size();i++)
		mAnimatables[i].Update( (float)mCurrFrame );

	if( mSelectedAnimatable && GetActiveGizmo() )
		GetActiveGizmo()->Init( mSelectedAnimatable->mPos, mSelectedAnimatable->mAngle );		
}

void VisualEditor::OnUpdate( float inDeltaTime )
{
	if( mIsPlaying )
	{
		mCurrTime += inDeltaTime / 4.0f;

		if( mCurrTime > 3.0f ) // fixme 5 secs
			mCurrTime = 0.0f;

		mCurrFrame = (int)( mCurrTime * mFPS );

		for( unsigned int i=0;i<mAnimatables.size();i++)
			mAnimatables[i].Update( mCurrTime * mFPS );

		if( mSelectedAnimatable && GetActiveGizmo() )
			GetActiveGizmo()->Init( mSelectedAnimatable->mPos, mSelectedAnimatable->mAngle );		
	}
}

void VisualEditor::OnActivateGizmo()
{
	if( mSelectedAnimatable && GetActiveGizmo() )
		GetActiveGizmo()->Init( mSelectedAnimatable->mPos, mSelectedAnimatable->mAngle );
}

void VisualEditor::OnRender()
{
	GetGrid()->Render();

	for( unsigned int i=0;i<mAnimatables.size(); i++)
		mAnimatables[i].Render();
}

void VisualEditor::RenderTrack( TrackInfo& inTrackInfo, int inPosY )
{
	if( inTrackInfo.bSelected )
		gl_SetColor(COLORS::eORANGE);
	else
		gl_SetColor(COLORS::eDARKGREY);
	glBegin( GL_LINES );
		glVertex2f( (GLfloat)KeyToScreen(mFirstFrame), (GLfloat)inPosY );
		glVertex2f( 1280.0f, (GLfloat)inPosY );
	glEnd();
		
	if( inTrackInfo.bSelected )
		gl_SetColor(COLORS::eRED);
	else
		gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, inPosY, "%s", inTrackInfo.mName );	//fixme 8
}

void VisualEditor::RenderTrackKeys( TrackType::E inType, int inPosY )
{
	if( mSelectedAnimatable )
	{
		gl_SetColor(COLORS::eGREY);
		glBegin( GL_POINTS );
		for( unsigned int i=0; i<mSelectedAnimatable->mCurve[inType].GetKeyCount(); i++ )
		{
			glVertex2f( (GLfloat)KeyToScreen( mSelectedAnimatable->mCurve[inType].GetKey(i).mPosition), (GLfloat)inPosY );
		}
		glEnd();
	}	
}

void VisualEditor::RenderSelectedTrackKeys()
{
	gl_SetColor(COLORS::eRED);
	glBegin( GL_POINTS );
	for( unsigned int i=0; i<mSelectedKeys.size(); i++ )
	{
//fixme		glVertex2f( (GLfloat)KeyToScreen(mSelectedKeys[i].mTrack->mCurve.GetKey(mSelectedKeys[i].mKey).mPosition), (GLfloat)mSelectedKeys[i].mTrack->mPosY );
	}
	glEnd();
}

void VisualEditor::RenderCurve( Curve& inCurve, bool inSelected, int inPosY )
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
		for(float t=START_POS;t<MID_START_POS; t+= 1.0f )		
			glVertex2f( t, inCurve.Evaluate(t) );

		// first keys to last key
		if( inSelected )
			gl_SetColor( COLORS::eORANGE );
		else
			gl_SetColor( COLORS::eWHITE );
		for(float t=MID_START_POS;t<MID_END_POS; t+= 1.0f )		
			glVertex2f( t, inCurve.Evaluate(t) );

		// last key to +inf
		gl_SetColor( COLORS::eDARKGREY );
		for(float t=MID_END_POS;t<END_POS; t+= 1.0f )		
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

	for( int i=0; i<4; i++ )	//fixme 4
	{
		RenderTrack( mTrackInfo[i], nPosY );
		nPosY += kMinDeltaTrackPosY;
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

	for( int i=0; i<4; i++ )	//fixme 4
	{
		RenderTrackKeys( (TrackType::E)i, nPosY );
		nPosY += kMinDeltaTrackPosY;
	}
	
	RenderSelectedTrackKeys();

	glPointSize( 8.0f );

	if( mShowCurve )
	{
		if( mSelectedAnimatable )
		{
			for( int i=0; i<4; i++ )	//fixme 4
			{
				RenderCurve( mSelectedAnimatable->mCurve[i], mTrackInfo[i].bSelected, kMinDeltaTrackPosY );
			}
		}
	}

	glLineWidth( 2.0f );
}

void VisualEditor::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	Editor::OnMouseClick( button, state, mm );

	if( (mm.y < 4 * kMinDeltaTrackPosY + kStartTrackPosY) && !(mm.mod & SK_MOD_SHIFT) )	//fixme 4
		OnMouseClickTrackArea( button, mm.x, mm.y );

	switch(button)
	{
	case 0:		
		{	
			if( mm.y < 4 * kMinDeltaTrackPosY + kStartTrackPosY  )	//fixme 4
			{
				if( Lair::GetInputMan()->GetMouseButtonState( InputMan::MouseButton::Left ).bState && !(mm.mod & SK_MOD_SHIFT) )
				{
					// Select track
					int nTrackIndex = (mm.y-kStartTrackPosY+kMinDeltaTrackPosY/2)/kMinDeltaTrackPosY;
					
					mTrackInfo[nTrackIndex].bSelected = true;

					// Select frame

					SetCurrFrame( ScreenToKey( mm.x ) );
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
	if( mSelectedAnimatable )
	{
		Vector2 vKeyPos;
		const Vector2 vClickPos((float)x,(float)y);
		Vector2 vDelta;

		const float fKeyRadiusSquared = (kKeySize/2)*(kKeySize/2);

		for( int i=0; i<6; i++ ) //fixme 6
		{
			for( unsigned int j=0; j<mSelectedAnimatable->mCurve[i].GetKeyCount(); j++ )
			{
				vKeyPos.Set( (float)KeyToScreen(mSelectedAnimatable->mCurve[i].GetKey(j).mPosition), (float)(kStartTrackPosY + i*kMinDeltaTrackPosY) );
				vDelta = vKeyPos - vClickPos;

				if( vDelta.GetLengthSquare() < fKeyRadiusSquared )
				{
					// found one
					//fixme mSelectedKeys.push_back( KeySelection(&mTrackInfo[i],j) );
				}
			}
		}
	}
}


void VisualEditor::OnMouseMotion( const MouseMotion& mm )
{
	if( mm.y < 4 * kMinDeltaTrackPosY + kStartTrackPosY  )	//fixme 4
	{
		if( Lair::GetInputMan()->GetMouseButtonState( InputMan::MouseButton::Left ).bState && (mm.mod & SK_MOD_SHIFT) )
		{
			// Scroll key tracks
			mFirstFrameDelta -= ( mm.dx / (float)kMinDeltaFramePosX );
			mFirstFrame = (int)mFirstFrameDelta;
		}		
	}
	else
	{
		if( mm.x < 32 ) // fixme 32
		{
			// Scale curve values
			mKeyValueScale += mm.dy / 10.0f;

			if( mKeyValueScale < 0.5f ) //fixme 0.5f
				mKeyValueScale = 0.5f;
		}
		else
		{
			Editor::OnMouseMotion(mm);
		}		
	}
}

void VisualEditor::OnSpecialKey( int key, int mod )
{
	Editor::OnSpecialKey(key, mod);

	switch( key )
	{
	case SK_HOME:	SetCurrFrame(0);	break;
	case SK_LEFT:	mCurrFrame--;	mCurrFrame = std::max( 0, mCurrFrame ); SetCurrFrame(mCurrFrame); break;
	case SK_RIGHT:	mCurrFrame++;	SetCurrFrame(mCurrFrame); break;
	}
}

void VisualEditor::OnKeyboard( unsigned char key, int mod )
{
	Editor::OnKeyboard(key, mod);

	switch( key )
	{
	case 'k':	// Keyframe selected tracks
		OnKeyframeSelected();		break;
	case 'K':	// Keyframe all tracks
		OnKeyframeAllSelected();	break;
		break;
	case 27:	// Escape key
		ClearSelection();	
		break;
	case 'p':
		mIsPlaying = !mIsPlaying;
		break;
	}
}

void VisualEditor::OnTranslate( const Vector2& inNewPos, const Vector2& inDelta )
{
	if(mSelectedAnimatable)
		mSelectedAnimatable->OnTranslate(inNewPos);
}

void VisualEditor::OnScale( const Vector2& inNewScale, const Vector2& inDelta )
{
	if(mSelectedAnimatable)
		mSelectedAnimatable->OnScale(inNewScale);
}

void VisualEditor::OnRotate( float inAngle, float inDelta )
{
	if(mSelectedAnimatable)
		mSelectedAnimatable->OnRotate(inAngle);
}

void VisualEditor::OnKeyframeSelected()
{
	if( mSelectedAnimatable )
	{
		//fixme check gizmo and keyframe active gizmo
		mSelectedAnimatable->KeyFrame( TrackType::PosX, mCurrFrame );
		mSelectedAnimatable->KeyFrame( TrackType::PosY, mCurrFrame );
		mSelectedAnimatable->KeyFrame( TrackType::Angle, mCurrFrame );
	}
}

void VisualEditor::OnKeyframeAllSelected()
{
	if( mSelectedAnimatable )
	{
		mSelectedAnimatable->KeyFrame( TrackType::PosX, mCurrFrame );
		mSelectedAnimatable->KeyFrame( TrackType::PosY, mCurrFrame );
		mSelectedAnimatable->KeyFrame( TrackType::Angle, mCurrFrame );
	}
}

void VisualEditor::OnCreateMenu()
{	
	Editor::OnCreateMenu();
		
	CREATE_MENU( pShow, "  Show...  " );
		ADD_MENU_ITEM( pShow, "  Curve  ", &VisualEditor::OnMenuShowCurve, 0 );

//	ADD_MENU_ITEM( GetMenu(), "Animate",	&VisualEditor::OnMenuAnimate, 0 );
//	ADD_MENU_ITEM( GetMenu(), "Texture",	&VisualEditor::OnMenuTexture, 0 );	
}

void VisualEditor::OnMenuShowCurve( int unused )
{
	mShowCurve = !mShowCurve;
}

void VisualEditor::Animatable::Update( float inPosition )
{
	mPos.x = mCurve[TrackType::PosX ].Evaluate(inPosition);
	mPos.y = mCurve[TrackType::PosY ].Evaluate(inPosition);
	mAngle = (float)DEG_TO_RAD(mCurve[TrackType::Angle].Evaluate(inPosition));
}

void VisualEditor::Animatable::KeyFrame( TrackType::E inTrackType, int inPosition )
{
	switch( inTrackType )
	{
	case TrackType::PosX:	mCurve[TrackType::PosX ].AddKey( inPosition, mPos.x );	break;
	case TrackType::PosY:	mCurve[TrackType::PosY ].AddKey( inPosition, mPos.y );	break;
	case TrackType::Angle:	mCurve[TrackType::Angle].AddKey( inPosition, mAngle );	break;
//	case TrackType::ScaleX:	mCurve[TrackType::ScaleX].AddKey( inPosition, mPos.x );	break;
//	case TrackType::ScaleY:	mCurve[TrackType::ScaleX].AddKey( inPosition, mPos.x );	break;
	}	
}

void VisualEditor::Animatable::Render()
{
	glPointSize( (GLfloat)kKeySize );
	gl_SetColor( COLORS::eGREEN );
	glBegin( GL_POINTS );
		glVertex2f( mPos.x, mPos.y );
	glEnd();

	gl_SetColor( COLORS::eGREEN );
	glBegin( GL_LINES );
		glVertex2f( mPos.x, mPos.y );
		glVertex2f( mPos.x+cos(mAngle)*kNormalLength, mPos.y+sin(mAngle)*kNormalLength );
	glEnd();
}
