#include "Animatable.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"
#include "Lair/Input/Input.h"
#include "Editors/Editor.h"
#include "Editors/Gizmo/Gizmo.h"

AnimatableElement::AnimatableElement( Editor* inEditor ) : EditorElement(inEditor), mScale(1.0f,1.0f), mFramePosition(0.0f)
{
	mFrame = Lair::GetAtlasMan()->Get("sprite.png");	// fixme, should have a frame track

	mSprite = mEditor->GetSpriteMan()->GetSprite();
	mSprite->SetFrame(mFrame);

	mScale.Set( 1.0f, 1.0f );

}

void AnimatableElement::SetFramePosition( float inFramePosition ) 
{ 
	mFramePosition = inFramePosition; 

	mPos.x	 = mCurve[TrackType::PosX].Evaluate(mFramePosition);
	mPos.y	 = mCurve[TrackType::PosY].Evaluate(mFramePosition);
	mAngle	 = mCurve[TrackType::Angle].Evaluate(mFramePosition);
	mAngle	 = (float)DEG_TO_RAD(mAngle);
	mAlpha	 = mCurve[TrackType::Alpha].Evaluate(mFramePosition);
	mScale.x = mCurve[TrackType::ScaleX].Evaluate(mFramePosition);
	mScale.y = mCurve[TrackType::ScaleY].Evaluate(mFramePosition);

	// mSprite->SetFrame(mFrame);	//todo, set frame based on animation curve

	UpdateSprite();
}

void AnimatableElement::OnUpdate( float inDeltaTime )
{

}

void AnimatableElement::UpdateSprite()
{
	mSprite->Set( mPos.x, mPos.y, mAngle, mScale.x, mScale.y );
	mSprite->SetAlpha( mAlpha );
}

void AnimatableElement::KeyFrame( TrackType::E inTrackType, int inPosition )
{
	switch( inTrackType )
	{
	case TrackType::PosX:	mCurve[TrackType::PosX  ].AddKey( inPosition, mPos.x );		break;
	case TrackType::PosY:	mCurve[TrackType::PosY  ].AddKey( inPosition, mPos.y );		break;
	case TrackType::Angle:	mCurve[TrackType::Angle ].AddKey( inPosition, (float)RAD_TO_DEG(mAngle) );	break;
	case TrackType::ScaleX:	mCurve[TrackType::ScaleX].AddKey( inPosition, mScale.x );	break;
	case TrackType::ScaleY:	mCurve[TrackType::ScaleY].AddKey( inPosition, mScale.y );	break;
	case TrackType::Alpha:	mCurve[TrackType::Alpha ].AddKey( inPosition, mAlpha );		break;
	}	
}

void AnimatableElement::OnRender()
{
	glPointSize( (GLfloat)4 );		//fixme 16
	gl_SetColor( COLORS::eGREEN, mAlpha );
	glBegin( GL_POINTS );
	glVertex2f( mPos.x, mPos.y );
	glEnd();

	glBegin( GL_LINES );
	glVertex2f( mPos.x, mPos.y );
	glVertex2f( mPos.x+cos(mAngle)*32, mPos.y+sin(mAngle)*32 );	//fixme 32
	glEnd();
}

void AnimatableElement::OnTranslate( const Vector2& inNewPos, const Vector2& inDelta ) 
{ 
	EditorElement::OnTranslate( inNewPos, inDelta );
	UpdateSprite();
}

void AnimatableElement::OnScale( const Vector2& inNewScale, const Vector2& inDelta ) 
{
	mScale = inNewScale;
	UpdateSprite();
}

void AnimatableElement::OnRotate( float inNewAngle, float inDelta ) 
{ 
	EditorElement::OnRotate( inNewAngle, inDelta );
	UpdateSprite();
}

void AnimatableElement::OnAlpha( float inNewAlpha, float inDelta ) 
{ 
	EditorElement::OnAlpha( inNewAlpha, inDelta );
	UpdateSprite();
}