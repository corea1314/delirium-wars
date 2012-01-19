#pragma once

#include "Lair/Animation/Curve.h"
#include "Editors/EditorElement.h"

class Sprite;
class AtlasFrame;

class TrackType { public: enum E { PosX, PosY, Angle, Alpha, ScaleX, ScaleY }; };

class AnimatableElement : public EditorElement
{
public:
	AnimatableElement( Editor* inEditor );
	virtual ~AnimatableElement();
	
	Curve	mCurve[6];
	Vector2 mScale;
	std::string mName;
	Sprite*		mSprite;
	AtlasFrame*	mFrame;

	float mFramePosition;

	void SetFramePosition( float inFramePosition );

	void OnUpdate( float inDeltaTime );
	void OnRender();
	void OnRenderGUI() {}

	virtual void KeyFrame( TrackType::E inTrackType, int inPosition );

	virtual void OnTranslate( const Vector2& inNewPos, const Vector2& inDelta );
	virtual void OnScale( const Vector2& inNewScale, const Vector2& inDelta );
	virtual void OnRotate( float inNewAngle, float inDelta );
	virtual void OnAlpha( float inNewAlpha, float inDelta );

	void UpdateSprite();
};
