#pragma once

#include "Math/Vector2.h"

#include "Gizmo.h"

class GizmoScaling : public Gizmo
{
public:
	class Mode { public: enum E { NotDragging, Dragging, DoneDragging }; };
	class Axis { public: enum E { None, X, Y, Both }; };

private:
	Mode::E	mMode;
	Axis::E	mAxis;

	Vector2 mPos;
	Vector2 mAnchor;
	Vector2 mScale;
			
public:
	GizmoScaling( Editor* inEditor );

	virtual void Init( AnimatableElement* inAnimatable );

	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod, bool down );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );

protected:
	virtual void ProcessTextEntry( const std::string& inText );
};