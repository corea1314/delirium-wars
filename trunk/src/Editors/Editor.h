#pragma once

#include "Menu.h"
#include "Math/Vector2.h"

class Grid;
class Camera;

class Editor
{
public:
	Editor();
	virtual ~Editor();

			void Init();
			void Exit();
			void Update( float dt );
			void Render();

	virtual void OnInit() {}
	virtual void OnExit() {}
	virtual void OnUpdate( float inDeltaTime ) {}
	virtual void OnRender() {}
	virtual void OnRenderGUI() {}

	virtual void OnMouseWheel( int v, int mod );
	virtual void OnMouseMotion( int x, int y, int dx, int dy, int mod );
	virtual void OnMouseClick( int button, int x, int y, int mod );
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual void OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values );
	
	void ScreenToEditor( int x, int y, Vector2& v );
	void EditorToScreen( const Vector2& v, int& x, int& y );
	
	Grid*	GetGrid() { return mGrid; }

protected:
	virtual void OnCreateMenu(){}
	virtual void OnDestroyMenu(){}

	const char* GetFileSave( const char* extension, const char* filter );
	const char* GetFileLoad( const char* extension, const char* filter );
	
	Menu*	GetMenu() { return mMenu; }
	float	GetTime() { return mTimeSeconds; }
	void	ResetTime() { mTimeSeconds = 0.0f; }

	Camera*	GetCamera() { return mCamera; }
	void	SetViewportSize( Vector2 inViewportSize ) { mViewportSize = inViewportSize; }

private:
	bool getSaveFilename( char filename[], int count, const char* filter, const char* extension );
	bool getLoadFilename( char filename[], int count, const char* filter, const char* extension );

private:
	bool			lock;

	Grid*			mGrid;		//todo render grid in screen space
	Menu*			mMenu;
	float			mTimeSeconds;
	unsigned int	mZoomLevel;
	Camera*			mCamera;

	Vector2			mViewportSize;
};