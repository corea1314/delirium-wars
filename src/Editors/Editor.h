#pragma once

#include "Menu.h"
#include "Math/Vector2.h"

class Grid;

class Editor
{
public:
	Editor();
	virtual ~Editor();

	virtual void Init();
	virtual void Exit();
	virtual void Update( float dt );
	virtual void Render();

	virtual void OnMouseWheel( int v );
	virtual void OnMouseMotion( int x, int y, Vector2& v, int dx, int dy, Vector2& d );
	virtual void OnMouseClick( int button, int x, int y, Vector2& v );
	virtual void OnKeyboard( unsigned char key );
	virtual void OnSpecialKey( int key );
	virtual void OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values );

protected:
	virtual void OnCreateMenu(){}
	virtual void OnDestroyMenu(){}

	const char* GetFileSave( const char* extension, const char* filter );
	const char* GetFileLoad( const char* extension, const char* filter );

	void ScreenToEditor( int x, int y, Vector2& v );
	void EditorToScreen( const Vector2& v, int& x, int& y );

	Grid*	GetGrid() { return m_pGrid; }
	Menu*	GetMenu() { return mMenu; }
	float	GetTime() { return timeSeconds; }
	void	ResetTime() { timeSeconds = 0.0f; }

private:
	bool getSaveFilename( char filename[], int count, const char* filter, const char* extension );
	bool getLoadFilename( char filename[], int count, const char* filter, const char* extension );

private:
	bool	lock;
	Grid*	m_pGrid;
	Menu*	mMenu;
	float timeSeconds;
	
protected:
	float cam_pos_x;
	float cam_pos_y;
	float cam_zoom;

	void ZoomIn();
	void ZoomOut();
	void ZoomReset();

	int w;
	int h;

	void UpdateZoom();
	int zoom;
};