#ifndef _APP_H
#define _APP_H

class CEngine;
class Editor;

class App
{
public:
	App();
	
	void Init();
	void Exit();
	void Update( float dt );
	void Render();

	void OnMouseWheel( int v );
	void OnMouseClick( int button, int x, int y, Vector2& v );
	void OnMouseMotion( int x, int y, Vector2& v, int dx, int dy, Vector2& d );

	void OnKeyboard( unsigned char key );
	void OnSpecialKey( int key );

	void OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values );

	void OnOpenFile( const char* in_szFilename );
		
	// todo: better handle mouse position and button states from framework
	int last_mx;
	int last_my;
	int mx;
	int my;
	unsigned char Buttons[5];

	// todo: better handle window change event variables from framework
	int w;
	int h;

	int fps;
	int fps_average;
	float fps_time;
		
	CEngine* GetEngine() const { return m_pEngine; }

	void SwitchEditor( int inEditorId );
	
private:
	CEngine* m_pEngine;

	Editor*	m_pActiveEditor;
	Editor*	m_pEditors[12];	// for F-keys
};

extern App g_App;



#endif//_APP_H
