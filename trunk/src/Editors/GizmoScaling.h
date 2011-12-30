#pragma once

#include "Math/Vector2.h"

class Editor;

class GizmoScaling
{
public:
	class Mode { public: enum E { NotDragging, Dragging, DoneDragging }; };
	class Axis { public: enum E { X, Y, Both }; };

	Mode::E	mMode;
	Axis::E	mAxis;

	Vector2 mPos;
	Vector2 mAnchor;
	Vector2 mScale;
	Editor*	mEditor;
	
public:
	GizmoScaling( Editor* inEditor );

	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnMouseMotion( const Vector2& pos, const Vector2& delta, int mod );
	virtual void OnMouseClick( int button, int state, const Vector2& pos, int mod );
};