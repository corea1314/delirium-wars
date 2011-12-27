#pragma once

#include "../Editor.h"
#include "CurveSelection.h"
#include "Lair/Animation/Curve.h"
#include "../Menu.h"

class CurveEditor : public Editor
{
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
	void OnMenuPreLoop( int inPreLoop );
	void OnMenuPostLoop( int inPostLoop );
	void OnMenuAnimate( int inUnused );
	void OnMenuTexture( int inUnused );

private:
	// Local methods
	void RenderCurve();

private:
	CurveSelection	mDummyCurveSelection;
	CurveSelection*	mCurveSelection;

	Curve mCurve;

	bool mAnimate;
};