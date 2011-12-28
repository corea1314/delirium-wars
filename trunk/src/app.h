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

	void OnMouseWheel( int v, int mod );
	void OnMouseClick( int button, int state, int x, int y, int mod );	// down is 1, up is 0
	void OnMouseMotion( int x, int y, int mod );

	void OnKeyboard( unsigned char key, int mod );
	void OnSpecialKey( int key, int mod );

	void OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values );

	void OnOpenFile( const char* in_szFilename );

	CEngine* GetEngine() const { return m_pEngine; }
	void SwitchEditor( int inEditorId );

	void OnReshape( int inNewWindowWidth, int inNewWindowHeight );
		
private:
	// todo: better handle mouse position and button states from framework
	int mLastMx, mLastMy;			// last mouse position
	int mMx, mMy;					// current mouse position
	int mWindowWidth, mWindowHeight;

	int mFps;
	int mFpsAverage;
	float mFpsTime;
	
private:
	CEngine* m_pEngine;

	Editor*	m_pActiveEditor;
	Editor*	m_pEditors[12];	// for F-keys
};

extern App g_App;



#endif//_APP_H
