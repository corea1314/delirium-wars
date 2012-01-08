#pragma once

#include "Math/Vector2.h"

#include "Gizmo.h"

class GizmoTranslation : public Gizmo
{
public:
	class Mode { public: enum E { NotDragging, Dragging, DoneDragging }; };
	class Axis { public: enum E { X, Y, Both }; };

	Mode::E	mMode;
	Axis::E	mAxis;

	Vector2 mPos;
	Vector2 mOrigin;
	Editor*	mEditor;
	
public:
	GizmoTranslation( Editor* inEditor );

	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );
};