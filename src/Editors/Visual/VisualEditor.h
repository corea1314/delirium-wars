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
		const char*	mName;
		Curve		mCurve;
	};	

protected:
	// Editor callbacks
	virtual void OnInit();
	virtual void OnExit();
	virtual void OnRender();
	virtual void OnRenderGUI();
	virtual void OnMouseClick( int button, int x, int y );
	virtual void OnMouseMotion( int x, int y, int dx, int dy );
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

	int KeyToScreen( int inPosition );

private:
	int		mCurrFrame;
	int		mFirstFrame;
	float	mFirstFrameDelta;
	Track	mTracks[6];
	float	mKeyValueScale;
};