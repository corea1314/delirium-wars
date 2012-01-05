#pragma once

#include <string>
#include "Math/Vector2.h"

class Editor;

class LayoutElement
{
public:
	std::string	mName;
	Vector2		mPos;
	float		mAngle;
	bool		mSelected;
	Editor*		mEditor;
	bool		mDeleteRequest;

	LayoutElement( Vector2 inPos, Editor* inEditor ) 
		: mName("elem"), mPos(inPos), mAngle(0.0f), mEditor(inEditor), mSelected(false), mDeleteRequest(false) {}

	LayoutElement( Editor* inEditor ) 
		: mName("elem"), mAngle(0.0f), mEditor(inEditor), mSelected(false), mDeleteRequest(false) {}

public:
	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual bool OnMouseMotion( const Vector2& pos, const Vector2& delta, int mod );
	virtual bool OnMouseClick( int button, int state, const Vector2& pos, int mod );	// returns true if handled
};