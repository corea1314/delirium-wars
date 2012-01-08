#pragma once

#include "Math/Vector2.h"

#include "Gizmo.h"

class GizmoRotation : public Gizmo
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
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );
};