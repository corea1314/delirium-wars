#pragma once

#include "../Editor.h"
#include "CurveSelection.h"
#include "Lair/Animation/Curve.h"
#include "../Menu.h"
#include "../EditorElement.h"

class EditorElementCurve : public EditorElement
{
public:
	EditorElementCurve( Editor* inEditor ) : EditorElement(inEditor) {}

public:
	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual bool OnMouseMotion( const MouseMotion& mm );
	virtual bool OnMouseClick( int button, int state, const MouseMotion& mm );

	virtual void OnTranslate( const Vector2& inNewPos, const Vector2& inDelta );
	virtual void OnScale( const Vector2& inNewScale, const Vector2& inDelta );
	virtual void OnRotate( float inAngle, float inDelta );
};

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