#pragma once

#include "Math/Vector2.h"
#include "gfx.h"
#include "Gizmo.h"

#define GIZMO_CIRCLE_POINT_COUNT	16

class GizmoRotation : public Gizmo
{
public:
	class Mode { public: enum E { NotDragging, Dragging, DoneDragging }; };

private:
	Mode::E		mMode;
	Vector2		mPos;
	Vector2		mAnchor;
	float		mAngle;
	Vector2		mDir;
	Vector2		mClickDir;
	
	Vertex		mVertexBuffer[GIZMO_CIRCLE_POINT_COUNT*2];
	
public:
	GizmoRotation( Editor* inEditor );

	virtual void Init( Animatable* inAnimatable );

	virtual void OnRender();
	virtual void OnRenderGUI();

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );

protected:
	virtual void ProcessTextEntry( const char* inText );

};