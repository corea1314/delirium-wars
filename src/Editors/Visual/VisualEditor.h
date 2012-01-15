#pragma once

#include "../Editor.h"
#include "../Menu.h"

#include "Lair/Animation/Curve.h"

#include "Editors/Animatable/Animatable.h"

class VisualEditor : public Editor
{
private:		
	class TrackInfo
	{
	public:
		void Set( const char* inName, TrackType::E inType, int inPosY ) 
		{
			mType = inType;
			mName = inName;
			mPosY = inPosY;
			bSelected = false;
		}

		const char*		mName;
		Curve*			mCurve;
		bool			bSelected;
		int				mPosY;
		TrackType::E	mType;
	};
	
	class KeySelection
	{
	public:
		KeySelection( TrackInfo* inTrackInfo, int inKey ) : mTrackInfo(inTrackInfo), mKey(inKey) {}
		TrackInfo* mTrackInfo;
		int mKey;
	};

protected:
	// Editor callbacks
	virtual void OnInit();
	virtual void OnExit();
	virtual void OnRender();
	virtual void OnUpdate( float inDeltaTime );
	virtual void OnRenderGUI();
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual void OnCreateMenu();
	virtual void OnTranslate( const Vector2& inNewPos, const Vector2& inDelta );
	virtual void OnScale( const Vector2& inNewScale, const Vector2& inDelta );
	virtual void OnRotate( float inAngle, float inDelta );
	virtual void OnAlpha( float inAlpha, float inDelta );
	virtual void OnActivateGizmo();
	
	// Menu callbacks
	virtual void OnMenuShowCurve( int unused );

	// File management
	virtual const char*	GetFileExtension() { return "xvsl"; }
	virtual const char*	GetFileFilter() { return "XML Visual File\0*.xvsl\0"; }
	
private:
	// Local methods
	void RenderTrack( TrackInfo& inTrackInfo, int inPosY );
	void RenderTrackKeys( TrackType::E inType, int inPosY );
	void RenderTrackCurves( TrackType::E inType, int inPosY );
	void RenderCurve( Curve& inCurve, bool inSelected, int inPosY );
	void RenderSelectedTrackKeys();

	void OnKeyframeSelected();
	void OnKeyframeAllSelected();

	void SetCurrFrame( int inFrame );
	void ClearSelection();

	int KeyToScreen( int inPosition );
	int ScreenToKey( int inPosition );

	void OnMouseClickTrackArea( int button, int x, int y );

private:
	int			mCurrFrame;
	int			mFirstFrame;
	int			mLastFrame;
	float		mFirstFrameDelta;
	TrackInfo	mTrackInfo[6];
	float		mKeyValueScale;

	bool		mIsPlaying;
	float		mCurrTime;
	int			mFPS;

	bool		mShowCurve;

	AnimatableElement*	mSelectedAnimatable;
	
	std::vector<AnimatableElement>		mAnimatables;
	std::vector<KeySelection>	mSelectedKeys;
};