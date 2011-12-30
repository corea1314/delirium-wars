#pragma once

#include "Math/Vector2.h"

class Editor;

class GizmoRotation
{
public:
	class Mode { public: enum E { NotDragging, Dragging, DoneDragging }; };

	Mode::E	mMode;
	Vector2 mPos;
	Vector2 mAnchor;
	float	mAngle;
	Vector2	mDir;
	Editor*	mEditor;
	
public:
	GizmoRotation( Editor* inEditor );

	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnMouseMotion( const Vector2& pos, const Vector2& delta, int mod );
	virtual void OnMouseClick( int button, int state, const Vector2& pos, int mod );
};