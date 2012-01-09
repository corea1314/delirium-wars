#pragma once

#include "Lair/Lair.h"

#include "Math/Vector2.h"

#include <math.h>

class Editor;
class EditorElement;

class Gizmo
{
public:
	Gizmo( Editor* inEditor ) : mEditor(inEditor) {}

	virtual void OnRender();
	virtual void OnRenderGUI();
	virtual void OnUpdate( float inDeltaTime );
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );

protected:
	bool IsAcceptableCharForText( char inKey );
	virtual void ProcessTextEntry( const char* inText ) {}

protected:
	Editor*			mEditor;
	std::string		mTextEntry;
};