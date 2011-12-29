#pragma once

#include "../Editor.h"
#include "../Menu.h"

#include "Lair/Animation/Curve.h"

class VisualEditor : public Editor
{
private:
	class TrackType { public: enum E { PosX, PosY, Angle, Alpha, ScaleX, ScaleY }; };
	
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
	virtual void OnMouseClick( int button, int x, int y, int mod );
	virtual void OnMouseMotion( int x, int y, int dx, int dy, int mod );
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual void OnCreateMenu();
	
	// Menu callbacks
	void OnMenuFileSave( int inUnused );
	void OnMenuFileLoad( int inUnused );
	
private:
	// Local methods
	void RenderTrack( TrackInfo& inTrackInfo, int inPosY );
	void RenderTrackKeys( TrackType::E inType, int inPosY );
	void RenderTrackCurves( TrackType::E inType, int inPosY );
	void RenderCurve( Curve& inCurve, bool inSelected, int inPosY );
	void RenderSelectedTrackKeys();

	void SetCurrFrame( int inFrame );
	void ClearSelection();

	int KeyToScreen( int inPosition );
	int ScreenToKey( int inPosition );

	void OnMouseClickTrackArea( int button, int x, int y );

private:
	int			mCurrFrame;
	int			mFirstFrame;
	float		mFirstFrameDelta;
	TrackInfo	mTrackInfo[6];
	float		mKeyValueScale;

	bool		mIsPlaying;
	float		mCurrTime;
	int			mFPS;

	Animatable*	mSelectedAnimatable;
	
	std::vector<Animatable>		mAnimatables;
	std::vector<KeySelection>	mSelectedKeys;
};