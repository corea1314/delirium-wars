#pragma once

#include "Lair/Animation/Curve.h"

class TrackType { public: enum E { PosX, PosY, Angle, Alpha, ScaleX, ScaleY }; };

class Animatable
{
public:
	Animatable() : mAngle(0.0f), mAlpha(1.0f) {}

	Vector2 mPos;
	Vector2 mScale;
	float	mAngle;
	float	mAlpha;

	Curve	mCurve[6];

	void Update( float inPosition );
	void Render();

	void KeyFrame( TrackType::E inTrackType, int inPosition );

	inline void OnTranslate( const Vector2& inPos ) { mPos = inPos;}
	inline void OnRotate( float inAngle ) { mAngle = inAngle; }
	inline void OnScale( const Vector2& inScale ) { mScale = inScale;}
	inline void OnAlpha( float inAlpha ) { mAlpha = inAlpha; }
};
