#pragma once

#include "Math/Vector2.h"

#include "Gizmo.h"

class GizmoTranslation : public Gizmo
{
public:
	class Mode { public: enum E { NotDragging, Dragging, DoneDragging }; };
	class Axis { public: enum E { None, X, Y, Both }; };

private:
	Mode::E	mMode;
	Axis::E	mAxis;

	Vector2 mPos;
	Vector2 mOrigin, mClickOrigin;
	Vector2 mDelta;

	float	mWidgetAppSize;
			
public:
	GizmoTranslation( Editor* inEditor );

	virtual void Init( Animatable* inAnimatable );

	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );

protected:
	virtual void ProcessTextEntry( const char* inText );
};