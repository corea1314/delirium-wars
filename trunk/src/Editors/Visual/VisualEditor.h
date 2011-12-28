#pragma once

#include "../Editor.h"
#include "../Menu.h"

#include "Lair/Animation/Curve.h"

class VisualEditor : public Editor
{
private:
	class Track
	{
	public:
		void Set( const char* inName, int inPosY ) 
		{
			mName = inName;
			mPosY = inPosY;
			bSelected = false;
		}

		const char*	mName;
		Curve		mCurve;
		bool		bSelected;
		int			mPosY;
	};

	class KeySelection
	{
	public:
		KeySelection( Track* inTrack, int inKey ) : mTrack(inTrack), mKey(inKey) {}
		Track* mTrack;
		int mKey;
	};

protected:
	// Editor callbacks
	virtual void OnInit();
	virtual void OnExit();
	virtual void OnRender();
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
	void RenderTrack( Track& inTrack, int inPosY );
	void RenderTrackKeys( Track& inTrack, int inPosY );
	void RenderTrackCurves( Track& inTrack, int inPosY );
	void RenderCurve( Curve& inCurve, int inPosY );
	void RenderSelectedTrackKeys();

	void ClearSelection();

	int KeyToScreen( int inPosition );
	int ScreenToKey( int inPosition );

	void OnMouseClickTrackArea( int button, int x, int y );

private:
	int		mCurrFrame;
	int		mFirstFrame;
	float	mFirstFrameDelta;
	Track	mTracks[6];
	float	mKeyValueScale;

	std::vector<KeySelection>	mSelectedKeys;
};