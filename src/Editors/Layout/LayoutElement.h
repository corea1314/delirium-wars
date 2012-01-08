#pragma once

#include <string>
#include "Math/Vector2.h"

#include "../EditorElement.h"

class Editor;

class LayoutElement : public EditorElement
{
public:
	LayoutElement( int inX, int inY, Vector2 inPos, Editor* inEditor ) : EditorElement(inX,inY,inPos,inEditor), mName("") {}
	LayoutElement( Editor* inEditor ) : EditorElement(inEditor), mName("") {}

public:
	std::string	mName;

public:
	virtual void OnRender();
	virtual void OnRenderGUI();
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );

	virtual void OnTranslate( const Vector2& inNewPos, const Vector2& inDelta );
	virtual void OnScale( const Vector2& inNewScale, const Vector2& inDelta );
	virtual void OnRotate( float inAngle, float inDelta );
};