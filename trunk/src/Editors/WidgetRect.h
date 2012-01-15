#pragma once

#include "Lair/Lair.h"

#include "Math/Vector2.h"

class Editor;

class WidgetRect
{
public:
	class Mode { public: enum E { NotDragging, Dragging, DoneDragging }; };

	Mode::E	mMode;
	Vector2 mRect[2];
	
	Editor*	mEditor;
	
public:
	WidgetRect( Editor* inEditor );

	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );
};