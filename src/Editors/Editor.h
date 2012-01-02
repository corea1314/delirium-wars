#pragma once

#include "Menu.h"
#include "Math/Vector2.h"
#include "tinyxml/tinyxml.h"

class Grid;
class Camera;

class Editor : public MenuUser
{
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

	virtual void OnMouseWheel( int v, int mod );
	virtual void OnMouseMotion( int x, int y, int dx, int dy, int mod );
	virtual void OnMouseClick( int button, int state, int x, int y, int mod );
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual void OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values );

	// Menu callbacks
	virtual void OnMenuFileSave( int unused );
	virtual void OnMenuFileLoad( int unused );
	
	// Helpers
	void ScreenToEditor( int x, int y, Vector2& v );
	void EditorToScreen( const Vector2& v, int& x, int& y );
	
	// Accessors
	Grid*	GetGrid() { return mGrid; }

protected:
	// Helpers and accessors
	float	GetTime() { return mTimeSeconds; }
	void	ResetTime() { mTimeSeconds = 0.0f; }

	Camera*	GetCamera() { return mCamera; }
	void	SetViewportSize( Vector2 inViewportSize ) { mViewportSize = inViewportSize; }
	
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

	Vector2			mViewportSize;
	std::vector<std::string>		mSelectedFilename;
};