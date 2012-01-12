#include "Animatable.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"
#include "Lair/Input/Input.h"
#include "Editors/Gizmo/Gizmo.h"

void Animatable::Update( float inPosition )
{
	mPos.x	 = mCurve[TrackType::PosX].Evaluate(inPosition);
	mPos.y	 = mCurve[TrackType::PosY].Evaluate(inPosition);
	mAngle	 = mCurve[TrackType::Angle].Evaluate(inPosition);
	mAngle	 = (float)DEG_TO_RAD(mAngle);
	mAlpha	 = mCurve[TrackType::Alpha].Evaluate(inPosition);
	mScale.x = mCurve[TrackType::ScaleX].Evaluate(inPosition);
	mScale.y = mCurve[TrackType::ScaleY].Evaluate(inPosition);
}

void Animatable::KeyFrame( TrackType::E inTrackType, int inPosition )
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

void Animatable::Render()
{
	glPointSize( (GLfloat)16 );		//fixme 16
	gl_SetColor( COLORS::eGREEN, mAlpha );
	glBegin( GL_POINTS );
	glVertex2f( mPos.x, mPos.y );
	glEnd();

	glBegin( GL_LINES );
	glVertex2f( mPos.x, mPos.y );
	glVertex2f( mPos.x+cos(mAngle)*32, mPos.y+sin(mAngle)*32 );	//fixme 32
	glEnd();
}
