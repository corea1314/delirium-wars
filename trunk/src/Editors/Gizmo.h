#pragma once

#include "Lair/Lair.h"

#include "Math/Vector2.h"

class Editor;
class EditorElement;

class Gizmo
{
public:
	Gizmo( Editor* inEditor ) : mEditor(inEditor), mElement(0) {}

	virtual void OnRender() {}
	virtual void OnRenderGUI() {}

	virtual void OnUpdate( float inDeltaTime ) {}

	virtual void OnKeyboard( unsigned char key, int mod ) {}
	virtual void OnMouseMotion( const MouseMotion& mm ) {}
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm ) {}

	void SetElement( EditorElement* inElement ) { mElement = inElement; }

protected:
	Editor*			mEditor;
	EditorElement*	mElement;	// element linked to this gizmo
};