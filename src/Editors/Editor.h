#pragma once

#include <list>

#include "Menu.h"
#include "Math/Vector2.h"
#include "tinyxml/tinyxml.h"

#include "Lair/Lair.h"

class Grid;
class Camera;
class Gizmo;
class GizmoScaling;
class GizmoTranslation;
class GizmoRotation;
class EditorElement;

class Editor : public MenuUser
{
public:
	class GizmoType { public: enum E { Translation, Rotation, Scaling, None }; };

public:
	Editor();
	virtual ~Editor();

	// App callbacks
			void Init();
			void Exit();
			void Update( float dt );
			void Render();

	// Editor callbacks
	virtual void OnInit() {}
	virtual void OnExit() {}
	virtual void OnUpdate( float inDeltaTime ) {}
	virtual void OnRender() {}
	virtual void OnRenderGUI() {}

	virtual void OnActivateGizmo() {}

	virtual void OnMouseWheel( int v, int mod );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual void OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values );
	
	virtual void OnTranslate( const Vector2& inNewPos, const Vector2& inDelta );
	virtual void OnScale( const Vector2& inNewScale, const Vector2& inDelta );
	virtual void OnRotate( float inAngle, float inDelta );
	
	// Menu callbacks
	virtual void OnCreateMenu();
	virtual void OnMenuFileSave( int unused );	// Save back to resmap, to xml and binary
	virtual void OnMenuFileLoad( int unused );	// Load from resmap, otherwise from binary or xml
	virtual void OnMenuTransform( int inGizmoType );
	
	// Helpers
	void ScreenToEditor( int x, int y, Vector2& v );
	void EditorToScreen( const Vector2& v, int& x, int& y );
	
	Camera*	GetCamera() { return mCamera; }

	void BuildMouseMotion( MouseMotion& mm, int x, int y, int dx, int dy, int mod );

	void ActivateGizmo( GizmoType::E inGizmoType );
	
	// Accessors
	Grid*	GetGrid() { return mGrid; }
	Gizmo*	GetActiveGizmo() { return mActiveGizmo; }

protected:
	// Helpers and accessors
	float	GetTime() { return mTimeSeconds; }
	void	ResetTime() { mTimeSeconds = 0.0f; }

	void	CleanupDeleted();
		
	void	SetViewportSize( Vector2 inViewportSize ) { mViewportSize = inViewportSize; }

	void AddElement( EditorElement* inElement );
		
	// File management
	virtual void OnSerializeSave( TiXmlElement* inNode ) {}
	virtual void OnSerializeLoad( TiXmlElement* inNode ) {}
	virtual const char*	GetFileExtension() = 0;
	virtual const char*	GetFileFilter() = 0;

	typedef std::vector<std::string> FileSelection;

	const FileSelection& GetFileSave( const char* extension, const char* filter );
	const FileSelection& GetFileLoad( const char* extension, const char* filter, bool multiple = false );
	
private:
	bool GetSaveFilename( char filename[], int count, const char* filter, const char* extension );
	bool GetLoadFilename( char filename[], int count, const char* filter, const char* extension, bool multiple = false );

private:
	bool			lock;

	Grid*			mGrid;		//todo render grid in screen space
	float			mTimeSeconds;
	unsigned int	mZoomLevel;
	Camera*			mCamera;

	GizmoScaling*		mGizmoScaling;
	GizmoRotation*		mGizmoRotation;
	GizmoTranslation*	mGizmoTranslation;
	Gizmo*				mActiveGizmo;

	Vector2			mViewportSize;
	std::vector<std::string>		mSelectedFilename;

protected:
	std::list<EditorElement*>	mElements;
};