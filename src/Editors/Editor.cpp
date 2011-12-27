
#include "gfx.h"

#include "Editor.h"
#include "Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"

#define CAMERA_MID_ZOOM_LEVEL	100.0f

Editor::Editor() 
	: lock(false)
{
	mGrid = new Grid;
	mZoomLevel = 100;
	mCamera = new Camera;
	mCamera->GetZoom() = CAMERA_MID_ZOOM_LEVEL / mZoomLevel;

	mViewportSize.x = ( 1280.0f / 720.0f ) / 2.0f;
	mViewportSize.y = (  720.0f / 720.0f ) / 2.0f;
}

Editor::~Editor()
{
	delete mCamera;
	delete mGrid;
}

void Editor::Init()
{
	ResetTime();

	mMenu = new Menu();
	mMenu->BindButton(2);	// right button

	OnCreateMenu();

	OnInit();
}

void Editor::Exit()
{
	OnExit();

	OnDestroyMenu();
	
	mMenu->UnbindButton(2);	// right button
	delete mMenu;
	mMenu = 0;
}

void Editor::Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	Vector2 vPos = mCamera->GetPos();
	float fZoom =  mCamera->GetZoom();
	float fAngle = mCamera->GetAngle();

	// Projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -mViewportSize.x * fZoom, mViewportSize.x * fZoom, -mViewportSize.y * fZoom, mViewportSize.y * fZoom );

	// Model view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef( (float)RAD_TO_DEG(fAngle), 0.0f, 0.0f, -1.0f );
	glTranslatef( -vPos.x, -vPos.y, 0.0f );

		mGrid->Render();

		// Render in editor space
		OnRender();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW);
	glPopMatrix();

	// Render in screen space
	OnRenderGUI();
}

void Editor::Update( float dt )
{
	mTimeSeconds += dt;
}

void Editor::OnKeyboard( unsigned char key )
{
	switch(key)
	{
	case '-':	GetGrid()->DecreaseGridSize();		break;
	case '=':	GetGrid()->IncreaseGridSize();		break;
	case 127:	/* delete */						break;
	case ' ':	GetGrid()->ToggleSnap();			break;
	}
}

void Editor::OnMouseWheel( int v )
{
	mZoomLevel += v;

	if( mZoomLevel < 1 )
		mZoomLevel = 1;

	mCamera->GetZoom() = CAMERA_MID_ZOOM_LEVEL / mZoomLevel;
}

void Editor::OnMouseClick( int button, int x, int y )
{
	// todo translate from screen to editor space
}

void Editor::OnMouseMotion( int x, int y, int dx, int dy )
{
	Vector2 last; 
	ScreenToEditor( x-dx, y-dy, last );

	Vector2 v;	
	ScreenToEditor( x, y, v );
	Vector2 d = v - last;

	if( Lair::GetInputMan()->GetMouseButtonState(0).bState )	// if left mouse button is down
	{
		mCamera->GetPos() -= d;
	}
}

void Editor::OnSpecialKey( int key )
{

}

void Editor::OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values )
{

}

const char* Editor::GetFileSave( const char* extension, const char* filter )
{
	static char filename[MAX_PATH] = "";

	if( !lock && getSaveFilename(filename,sizeof(filename), filter, extension ) )
	{
		return filename;
	}
	return 0;
}

const char* Editor::GetFileLoad( const char* extension, const char* filter )
{
	static char filename[MAX_PATH] = "";

	if( !lock && getLoadFilename(filename,sizeof(filename),filter,extension) )
	{
		return filename;
	}
	return 0;
}

bool Editor::getSaveFilename( char filename[], int count, const char* filter, const char* extension )
{
	::OPENFILENAME openFilename = {0};

	openFilename.lStructSize     = sizeof(::OPENFILENAME);
	openFilename.hwndOwner       = 0;
	openFilename.lpstrFile       = filename;
	openFilename.nMaxFile        = count;
	openFilename.lpstrFilter     = filter;
	openFilename.nFilterIndex    = 1;
	openFilename.lpstrFileTitle  = 0;
	openFilename.nMaxFileTitle   = 0;
	openFilename.lpstrInitialDir = 0;
	openFilename.lpstrDefExt     = extension;
	openFilename.Flags           = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN;

	lock = true;
	bool result = ::GetSaveFileName(&openFilename) != 0;
	lock = false;
	return result;
}

bool Editor::getLoadFilename( char filename[], int count, const char* filter, const char* extension )
{
	::OPENFILENAME openFilename = {0};

	openFilename.lStructSize     = sizeof(::OPENFILENAME);
	openFilename.hwndOwner       = 0;//g_hWnd;
	openFilename.lpstrFile       = filename;
	openFilename.nMaxFile        = count;
	openFilename.lpstrFilter     = filter;
	openFilename.nFilterIndex    = 1;
	openFilename.lpstrFileTitle  = 0;
	openFilename.nMaxFileTitle   = 0;
	openFilename.lpstrInitialDir = 0;
	openFilename.lpstrDefExt     = extension;
	openFilename.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	lock = true;
	bool result = ::GetOpenFileName(&openFilename) != 0;
	lock = false;
	return result;
}

void Editor::ScreenToEditor( int x, int y, Vector2& v )
{
	double P[16];
	double M[16];
	int V[4];

	Vector2 vPos = mCamera->GetPos();
	float fZoom =  mCamera->GetZoom();
	float fAngle = mCamera->GetAngle();

	// Construct matrices used in rendering and grab them to undo the transformations
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -mViewportSize.x * fZoom, mViewportSize.x * fZoom, -mViewportSize.y * fZoom, mViewportSize.y * fZoom );
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

void Editor::EditorToScreen( const Vector2& v, int& x, int& y )
{
	double P[16];
	double M[16];
	int V[4];

	Vector2 vPos = mCamera->GetPos();
	float fZoom =  mCamera->GetZoom();
	float fAngle = mCamera->GetAngle();

	// Construct matrices used in rendering and grab them to undo the transformations
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -mViewportSize.x * fZoom, mViewportSize.x * fZoom, -mViewportSize.y * fZoom, mViewportSize.y * fZoom );
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