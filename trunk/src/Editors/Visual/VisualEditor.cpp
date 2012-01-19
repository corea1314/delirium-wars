
//FIXME
// - Implement insertion/deletion/manipulation of keys (maybe using curve editor)
// - Move rendering of track and keys to AnimatableElement (maybe even key manipulation too)
// - Determine if we want to support multiple selection of animatable (and keyframing of multiple animatables at the same time)
// - Add sprite support (to animatable also)

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

typedef struct
{
	TrackType::E	mTrackType;
	const char*		mTrackName;

} TrackInfoHelper;

const TrackInfoHelper	kTrackInfo[] = 
{
	{ TrackType::PosX,		"position.x" },
	{ TrackType::PosY, 		"position.y" },
	{ TrackType::Angle, 	"angle" },
	{ TrackType::Alpha, 	"alpha" },
	{ TrackType::ScaleX, 	"scale.x" }, 
	{ TrackType::ScaleY,	"scale.y" }
};

void VisualEditor::OnInit()
{
	GetCamera()->GetPos().Set( 0.0f, 0.0f );
	
	mTrackInfo[TrackType::PosX ].Set( "pos:x", TrackType::PosX, kStartTrackPosY+kMinDeltaTrackPosY*0);	
	mTrackInfo[TrackType::PosY ].Set( "pos:y", TrackType::PosY, kStartTrackPosY+kMinDeltaTrackPosY*1);	
	mTrackInfo[TrackType::Angle].Set( "angle", TrackType::Angle, kStartTrackPosY+kMinDeltaTrackPosY*2);	
	mTrackInfo[TrackType::Alpha].Set( "alpha", TrackType::Alpha, kStartTrackPosY+kMinDeltaTrackPosY*3);	
	mTrackInfo[TrackType::ScaleX].Set( "scale:x", TrackType::ScaleX, kStartTrackPosY+kMinDeltaTrackPosY*4);	
	mTrackInfo[TrackType::ScaleY].Set( "scale:y", TrackType::ScaleY, kStartTrackPosY+kMinDeltaTrackPosY*5);	

	// push in dummy tracks
	mAnimatables.push_back( new AnimatableElement(this) );
	mSelectedAnimatable = mAnimatables[0];
	
	mSelectedAnimatable->mCurve[TrackType::Alpha].AddKey( 0, 1.0f ); //fixme
	mSelectedAnimatable->mCurve[TrackType::ScaleX].AddKey( 0, 1.0f ); //fixme
	mSelectedAnimatable->mCurve[TrackType::ScaleY].AddKey( 0, 1.0f ); //fixme

	// init info
	mFirstFrame = 0;
	mLastFrame = 100;
	mFirstFrameDelta = 0.0f;
	mKeyValueScale = 1.0f;
	SetCurrFrame( 0 );

	mIsPlaying = false;
	mCurrTime = 0.0f;
	mFPS = 60;

	mShowCurve = false;
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
		mAnimatables[i]->SetFramePosition( (float)mCurrFrame );

	if( mSelectedAnimatable && GetActiveGizmo() )
		GetActiveGizmo()->Init( mSelectedAnimatable );		
}

void VisualEditor::OnUpdate( float inDeltaTime )
{
	if( mIsPlaying )
	{
		mCurrTime += inDeltaTime / 4.0f;

		mCurrFrame = (int)( mCurrTime * mFPS );

		if( mCurrFrame == mLastFrame )
		{
			mCurrFrame = 0;
			mCurrTime = 0.0f;
		}

		for( unsigned int i=0;i<mAnimatables.size();i++)
		{
			mAnimatables[i]->SetFramePosition( mCurrTime * mFPS );
			mAnimatables[i]->OnUpdate( inDeltaTime );
		}

		if( mSelectedAnimatable && GetActiveGizmo() )
			GetActiveGizmo()->Init( mSelectedAnimatable );		
	}

}

void VisualEditor::OnActivateGizmo()
{
	if( mSelectedAnimatable && GetActiveGizmo() )
		GetActiveGizmo()->Init( mSelectedAnimatable );
}

void VisualEditor::OnRender()
{
	GetGrid()->Render();

	Lair::GetAtlasMan()->Bind();
	GetSpriteMan()->Render();

	for( unsigned int i=0;i<mAnimatables.size(); i++)
		mAnimatables[i]->OnRender();
}

void VisualEditor::RenderTrack( TrackInfo& inTrackInfo, int inPosY )
{
	if( inTrackInfo.bSelected )
		gl_SetColor(COLORS::eORANGE,0.5f);
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

		gl_SetColor( COLORS::eWHITE );
		for(unsigned int i=0;i<inCurve.GetKeyCount();i++)
			gl_RenderText( inCurve.GetKey(i).mPosition, (GLint)(inCurve.GetKey(i).mValue+8), "%0.2f", inCurve.GetKey(i).mValue ); //fixme 8

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
		
	glBegin( GL_LINES );
/*
		// Render fix distance marker
		gl_SetColor(COLORS::eLIGHTGREY);
		int nStartPos = KeyToScreen( kMarkFrameCount-1 );
		int nNextPos =  KeyToScreen( 2*kMarkFrameCount-1 );
		int nDeltaPos = nNextPos-nStartPos;

		for( int i=nStartPos; i<1280; i+=nDeltaPos )	//fixme 1280
		{
			glVertex2f( (GLfloat)i, (GLfloat)(kStartTrackPosY - kKeySize/2) );
			glVertex2f( (GLfloat)i, (GLfloat)(kStartTrackPosY + 4 * kMinDeltaTrackPosY - kMinDeltaTrackPosY/2) ); //fixme 4
		}
*/
		int nCurrFramePosX = KeyToScreen( mCurrFrame );

		glLineWidth( 6.0f );
		gl_SetColor(COLORS::eGREEN);
		glVertex2f( (GLfloat)nCurrFramePosX, (GLfloat)(kStartTrackPosY - kKeySize/2) );
		glVertex2f( (GLfloat)nCurrFramePosX, (GLfloat)(kStartTrackPosY + 4 * kMinDeltaTrackPosY - kMinDeltaTrackPosY/2) ); //fixme 4

		nCurrFramePosX = KeyToScreen( mLastFrame );
		gl_SetColor(COLORS::eBLACK);
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
				if( Lair::GetInputMan()->IsMouseButtonDown( InputMan::MouseButton::Left ) && !(mm.mod & SK_MOD_SHIFT) )
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
		if( Lair::GetInputMan()->IsMouseButtonDown( InputMan::MouseButton::Left ) && (mm.mod & SK_MOD_SHIFT) )
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
	case SK_END:	
		if( mod == SK_MOD_SHIFT )
			mLastFrame = mCurrFrame;
		else
			SetCurrFrame(mLastFrame);	break;
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
	case 'c':
		mShowCurve = !mShowCurve;
		break;
	}
}

void VisualEditor::OnTranslate( const Vector2& inNewPos, const Vector2& inDelta )
{
	if(mSelectedAnimatable)
		mSelectedAnimatable->OnTranslate(inNewPos,inDelta);
}

void VisualEditor::OnScale( const Vector2& inNewScale, const Vector2& inDelta )
{
	if(mSelectedAnimatable)
		mSelectedAnimatable->OnScale(inNewScale,inDelta);
}

void VisualEditor::OnRotate( float inAngle, float inDelta )
{
	if(mSelectedAnimatable)
		mSelectedAnimatable->OnRotate(inAngle,inDelta);
}

void VisualEditor::OnAlpha( float inAlpha, float inDelta )
{
	if(mSelectedAnimatable)
		mSelectedAnimatable->OnAlpha(inAlpha,inDelta);
}

void VisualEditor::OnKeyframeSelected()
{
	if( mSelectedAnimatable )
	{
		switch( GetActiveGizmoType() )
		{
		case GizmoType::Scaling:
			mSelectedAnimatable->KeyFrame( TrackType::ScaleX, mCurrFrame );	
			mSelectedAnimatable->KeyFrame( TrackType::ScaleY, mCurrFrame );
			break;
		case GizmoType::Rotation:		
			mSelectedAnimatable->KeyFrame( TrackType::Angle, mCurrFrame );		
			break;
		case GizmoType::Translation:	
			mSelectedAnimatable->KeyFrame( TrackType::PosX, mCurrFrame );
			mSelectedAnimatable->KeyFrame( TrackType::PosY, mCurrFrame );		
			break;
		case GizmoType::Alpha:
			mSelectedAnimatable->KeyFrame( TrackType::Alpha, mCurrFrame );
			break;
		}		
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

void VisualEditor::OnSerializeSave( TiXmlElement* inNode )
{
	TiXmlElement* pxmlVisual = new TiXmlElement("visual");

	TiXmlElement* pxmlAnimatable, *pxmlCurve;
	for( std::vector<AnimatableElement*>::iterator it = mAnimatables.begin(); it != mAnimatables.end(); it++ )
	{
		pxmlAnimatable = new TiXmlElement("animatable");
		pxmlAnimatable->SetAttribute( "name", (*it)->mName );
		
		for( int i=0; i<6; i++ )	//fixme 6
		{
			pxmlCurve = new TiXmlElement("curve");
			pxmlCurve->SetAttribute( "name", kTrackInfo[i].mTrackName );
			(*it)->mCurve[kTrackInfo[i].mTrackType].SerializeSave( pxmlCurve );
			pxmlAnimatable->LinkEndChild(pxmlCurve);
		}
		
		pxmlVisual->LinkEndChild(pxmlAnimatable);
	}

	inNode->LinkEndChild(pxmlVisual);
}

struct DeletePtr
{
	template <class T>
	void operator() ( T* ptr) const
	{
		delete ptr;
	}
};

void VisualEditor::OnSerializeLoad( TiXmlElement* inNode )
{
	std::for_each( mAnimatables.begin(), mAnimatables.end(), DeletePtr() );
	mAnimatables.clear();

	TiXmlElement* pxmlVisual = inNode->FirstChildElement("visual");

	if( pxmlVisual )
	{
		for( TiXmlElement* pxmlAnimatable = pxmlVisual->FirstChildElement("animatable"); pxmlAnimatable; pxmlAnimatable = pxmlAnimatable->NextSiblingElement("animatable") )
		{	
			mAnimatables.push_back( new AnimatableElement(this) );

			AnimatableElement* pAnimatable = mAnimatables.back();

			pAnimatable->mName = pxmlAnimatable->Attribute( "name" );

			for( TiXmlElement* pxmlCurve = pxmlAnimatable->FirstChildElement("curve"); pxmlCurve; pxmlCurve = pxmlCurve->NextSiblingElement("curve") )
			{
				for( int i=0; i<6; i++ )
				{
					if( strcmp( kTrackInfo[i].mTrackName, pxmlCurve->Attribute("name") ) == 0 )
						pAnimatable->mCurve[i].SerializeLoad( pxmlCurve );
				}				
			}
		}	
	}

	if( !mAnimatables.empty() )
		mSelectedAnimatable = mAnimatables[0];
	else
		mSelectedAnimatable = 0;
}

/*
<visual>
	<animatable>
		<curve name="position.x">
			<key position="" value=""/>
		</curve>
	</animatable>
</visual>
*/