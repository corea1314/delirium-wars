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
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnCreateMenu();

	// Menu callbacks
	void OnMenuPreLoop( int inPreLoop );
	void OnMenuPostLoop( int inPostLoop );
	void OnMenuAnimate( int inUnused );
	void OnMenuTexture( int inUnused );

	// File management
	virtual const char*	GetFileExtension() { return "xcrv"; }
	virtual const char*	GetFileFilter() { return "XML Curve File\0*.xcrv\0"; }
	virtual void OnSerializeSave( TiXmlElement* inNode );
	virtual void OnSerializeLoad( TiXmlElement* inNode );

private:
	// Local methods
	void RenderCurve();

private:
	CurveSelection	mDummyCurveSelection;
	CurveSelection*	mCurveSelection;

	Curve mCurve;

	bool mAnimate;
};