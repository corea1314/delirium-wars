#pragma once

#include "../Editor.h"
#include "../Menu.h"

#include <list>

class LayoutElement;
class WidgetRect;
class GizmoScaling;
class GizmoRotation;
class GizmoTranslation;

class LayoutEditor : public Editor
{
private:
	class Mode
	{
	public:
		enum E { Selection, Point, Rect };
	};
	
protected:
	// Editor callbacks
	virtual void OnInit();
	virtual void OnExit();
	virtual void OnRender();
	virtual void OnUpdate( float inDeltaTime );
	virtual void OnRenderGUI();
	virtual void OnMouseClick( int button, int state, int x, int y, int mod );
	virtual void OnMouseMotion( int x, int y, int dx, int dy, int mod );
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual void OnCreateMenu();
	
	// Menu callbacks
	void OnMenuMode( int inMode );

	// File management
	virtual const char*	GetFileExtension() { return "xlay"; }
	virtual const char*	GetFileFilter() { return "XML Layout File\0*.xlay\0"; }
	
private:
	// Local methods
	void SetMode( Mode::E inMode );
	void CleanupDeleted();
	
private:
	Mode::E						mMode;
	
	WidgetRect*					mWidgetRect;
	
	GizmoScaling*				mGizmoScaling;
	GizmoRotation*				mGizmoRotation;
	GizmoTranslation*			mGizmoTranslation;

	std::list<LayoutElement*>	mElements;
};