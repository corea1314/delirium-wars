

#include "fw.h"

#include "app.h"

#include "Engine/Engine.h"
#include "Engine/Clock/Clock.h"

#include "Lair/Lair.h"

#include "Editors/Editor.h"
#include "Editors/Curve/CurveEditor.h"
#include "Editors/Visual/VisualEditor.h"
#include "Editors/Layout/LayoutEditor.h"

void Screen_2_App( int x, int y, Vector2& v )
{
	double P[16];
	double M[16];
	int V[4];

	Vector2 vPos = Lair::GetCameraMan()->GetActiveCamera()->GetPos();
	float fZoom = Lair::GetCameraMan()->GetActiveCamera()->GetZoom();
	float fAngle = Lair::GetCameraMan()->GetActiveCamera()->GetAngle();

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1024/2*fZoom, 1024/2*fZoom, -1024/2*fZoom,  1024/2*fZoom );
	glGetDoublev( GL_MODELVIEW_MATRIX, P );

	glLoadIdentity();
	glRotatef( (float)RAD_TO_DEG(fAngle), 0.0f, 0.0f, -1.0f );
	glTranslatef( -vPos.x, -vPos.y, 0.0f );
	glGetDoublev( GL_MODELVIEW_MATRIX, M );
	glPopMatrix();

	glGetIntegerv( GL_VIEWPORT, V );

	double X,Y,Z;

	gluUnProject( x,y,0, M, P, V, &X, &Y, &Z );

	v.x = (float)X;
	v.y = (float)Y;
}

void App_2_Screen( const Vector2& v, int& x, int& y )
{
	double P[16];
	double M[16];
	int V[4];

	Vector2 vPos = Lair::GetCameraMan()->GetActiveCamera()->GetPos();
	float fZoom = Lair::GetCameraMan()->GetActiveCamera()->GetZoom();
	float fAngle = Lair::GetCameraMan()->GetActiveCamera()->GetAngle();

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1024/2*fZoom,1024/2*fZoom, -1024/2*fZoom,  1024/2*fZoom );
	glGetDoublev( GL_MODELVIEW_MATRIX, P );

	glLoadIdentity();
	glRotatef( (float)RAD_TO_DEG(fAngle), 0.0f, 0.0f, -1.0f );
	glTranslatef( -vPos.x, -vPos.y, 0.0f );
	glGetDoublev( GL_MODELVIEW_MATRIX, M );
	glPopMatrix();

	glGetIntegerv( GL_VIEWPORT, V );

	double X,Y,Z;

	gluProject( v.x,v.y,0, M, P, V, &X, &Y, &Z );

	x = (int)X;
	y = (int)Y;
}

App g_App;

App::App()
{
	mMx=0;
	mMy=0;
	mLastMx=0;
	mLastMy=0;

	mFps = 0;
	mFpsAverage = 0;
	mFpsTime = 0.0f;
}

#include "objLoader/obj.h"

void App::Init()
{
	m_pEngine = new CEngine;

	m_pActiveEditor = 0;

	m_pEditors[0] = new CurveEditor;
	m_pEditors[1] = new VisualEditor;
	m_pEditors[2] = new LayoutEditor;
	m_pEditors[3] = 0;
	m_pEditors[4] = 0;
	m_pEditors[5] = 0;
	m_pEditors[6] = 0;
	m_pEditors[7] = 0;
	m_pEditors[8] = 0;
	m_pEditors[9] = 0;
	m_pEditors[10] = 0;
	m_pEditors[11] = 0;
}

void App::Exit()
{
	delete m_pEngine;

	m_pActiveEditor = 0;

	SAFE_DELETE(m_pEditors[0]);
	SAFE_DELETE(m_pEditors[1]);
	SAFE_DELETE(m_pEditors[2]);
	SAFE_DELETE(m_pEditors[3]);
	SAFE_DELETE(m_pEditors[4]);
	SAFE_DELETE(m_pEditors[5]);
	SAFE_DELETE(m_pEditors[6]);
	SAFE_DELETE(m_pEditors[7]);
	SAFE_DELETE(m_pEditors[8]);
	SAFE_DELETE(m_pEditors[9]);
	SAFE_DELETE(m_pEditors[10]);
	SAFE_DELETE(m_pEditors[11]);
}

void App::OnReshape( int inNewWindowWidth, int inNewWindowHeight )
{
	mWindowWidth = inNewWindowWidth;
	mWindowHeight = inNewWindowHeight;
	
	glViewport(0,0,mWindowWidth,mWindowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( 0, mWindowWidth, 0, mWindowHeight );
	glMatrixMode(GL_MODELVIEW);
}

void App::OnMouseClick( int button, int state, int x, int y, int mod )
{
	y = mWindowHeight - y;

	mMx = x;
	mMy = y;
	mLastMx=x;
	mLastMy=y;

	Vector2 v; Screen_2_App( x, y, v );

	Lair::GetInputMan()->UpdateMouseButtonState( button, state == 1 );

	if( m_pActiveEditor )
	{
		m_pActiveEditor->OnMouseClick( button, state, x, y, mod );
	}
	else
		GetEngine()->OnMouseClick( button, x, y, v );	
}

void App::OnMouseMotion( int x, int y, int mod )
{
	// TODO: have the app handle all the translation between app and screen space
	y = mWindowHeight - y;

	int dx=x-mLastMx;
	int dy=y-mLastMy;

	Vector2 last; 
	Screen_2_App( mLastMx, mLastMy, last );

	mMx=x;
	mMy=y;
	mLastMx=x;
	mLastMy=y;

	if( m_pActiveEditor )
		m_pActiveEditor->OnMouseMotion( x, y, dx, dy, mod );
}

void App::OnMouseWheel( int v, int mod )
{
	if( m_pActiveEditor )
		m_pActiveEditor->OnMouseWheel( v, mod );
	else
		GetEngine()->OnMouseWheel( v );
}

void App::OnKeyboard( unsigned char key, int mod )
{
	if( m_pActiveEditor )
		m_pActiveEditor->OnKeyboard( key, mod );
	else
		GetEngine()->OnKeyboard( key );
}

void App::OnSpecialKey( int key, int mod )
{
	if( m_pActiveEditor )
		m_pActiveEditor->OnSpecialKey( key, mod );

	switch( key )
	{
	case SK_F1:	
	case SK_F2:
	case SK_F3:
	case SK_F4:
	case SK_F5:
	case SK_F6:
	case SK_F7:
	case SK_F8:
	case SK_F9:
	case SK_F10:		
	case SK_F11:		
	case SK_F12:	
		SwitchEditor(key-SK_F1);
		break;

	case SK_LEFT:		
	case SK_DOWN:		
	case SK_RIGHT:	
	case SK_UP:	

	case SK_PAGE_UP:
	case SK_PAGE_DOWN:
	case SK_HOME:
	case SK_END:	
	case SK_INSERT:
		break;
	}
}

void App::OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values )
{
	if( m_pActiveEditor )
		m_pActiveEditor->OnGamepad( gamepad, buttons, axis_count, axis_values );
	else
	{
		switch( gamepad )
		{
		case 0:		GetEngine()->OnGamepad0( buttons, axis_count, axis_values );	break;
		case 1:		GetEngine()->OnGamepad1( buttons, axis_count, axis_values );	break;
		case 2:		GetEngine()->OnGamepad2( buttons, axis_count, axis_values );	break;
		case 3:		GetEngine()->OnGamepad3( buttons, axis_count, axis_values );	break;
		}
	}	
}

void App::OnOpenFile( const char* in_szFilename )
{
	m_pEngine->OnLoadFile( in_szFilename );
}

void App::Render()
{
	++mFps;
	if( mFpsTime < m_pEngine->GetClock()->GetTotalTime() )
	{
		mFpsAverage += mFps;
		mFpsAverage /= 2;
		mFpsTime = m_pEngine->GetClock()->GetTotalTime() + 1.0f;
		mFps = 0;
	}

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	if( m_pActiveEditor )
		m_pActiveEditor->Render();
	else
	{		
		m_pEngine->Render();
		m_pEngine->RenderGUI();

		glDisable( GL_TEXTURE_2D );
		gl_SetColor(COLORS::eWHITE);
		gl_RenderText( 8, 8, "FPS: %d (%d) - Zoom: %0.1fX -- w:%d, %d", mFps, mFpsAverage, Lair::GetCameraMan()->GetActiveCamera()->GetZoom(), mWindowWidth, mWindowHeight );
	}	
}

void App::Update( float dt )
{
	if( m_pActiveEditor )
		m_pActiveEditor->Update(dt);
	else
	{
		//todo: emit any other update related signals
		m_pEngine->Update( dt );
	}
}

void App::SwitchEditor( int inEditorId )
{
	if( m_pActiveEditor )
	{
		if( m_pActiveEditor != m_pEditors[inEditorId] )
		{
			m_pActiveEditor->Exit();					// Exit the previous editor
			m_pActiveEditor = m_pEditors[inEditorId];	// Set the new editor
			
			if( m_pActiveEditor )						// Check if there is an editor in the slot
				m_pActiveEditor->Init();				// Init the new editor
		}
		else if( m_pActiveEditor == m_pEditors[inEditorId] )
		{
			m_pActiveEditor->Exit();	// Exit the current editor
			m_pActiveEditor = 0;		// Deactivate editor mode
		}
	}
	else
	{
		if( m_pEditors[inEditorId] )	// Only if there is an editor in the slot
		{
			m_pActiveEditor = m_pEditors[inEditorId];	// Set new editor
			m_pActiveEditor->Init();					// Init the new editor
		}		
	}	
}
