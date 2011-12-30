#pragma once

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
	virtual void OnMouseMotion( const Vector2& pos, const Vector2& delta, int mod );
	virtual void OnMouseClick( int button, int state, const Vector2& pos, int mod );
};