
#include "gfx.h"

#include "Editor.h"
#include "Grid.h"

Editor::Editor() 
	: lock(false)
{
	m_pGrid = new Grid;
}

Editor::~Editor()
{
	delete m_pGrid;
}

void Editor::Init()
{
	ResetTime();

	mMenu = new Menu();
	mMenu->BindButton(2);	// right button

	OnCreateMenu();
}

void Editor::Exit()
{
	OnDestroyMenu();
	
	mMenu->UnbindButton(2);	// right button
	delete mMenu;
	mMenu = 0;
}

void Editor::Render()
{

}

void Editor::Update( float dt )
{
	timeSeconds += dt;
}

void Editor::OnKeyboard( unsigned char key )
{
	switch(key)
	{
	case '-':	GetGrid()->DecreaseGridSize();		break;
	case '=':	GetGrid()->IncreaseGridSize();		break;
	case 127:	/* delete */						break;
	}
}

void Editor::OnMouseWheel( int v )
{

}

void Editor::OnMouseClick( int button, int x, int y, Vector2& v )
{

}

void Editor::OnMouseMotion( int x, int y, Vector2& v, int dx, int dy, Vector2& d )
{

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

	// Construct matrices used in curver rendering and grab them to undo the transformations
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -0.5f / cam_zoom, 0.5f / cam_zoom, -0.5f / cam_zoom, 0.5f / cam_zoom );
	glGetDoublev( GL_MODELVIEW_MATRIX, P );

	glLoadIdentity();
	glTranslatef( -cam_pos_x, -cam_pos_y, 0.0f );
	glGetDoublev( GL_MODELVIEW_MATRIX, M );
	glPopMatrix();

	glGetIntegerv( GL_VIEWPORT, V );

	double X,Y,Z;

	gluUnProject( x,y,0, M, P, V, &X, &Y, &Z );

	v.x = X;
	v.y = Y;
}

void Editor::EditorToScreen( const Vector2& v, int& x, int& y )
{
	double P[16];
	double M[16];
	int V[4];

	// Construct matrices used in curver rendering and grab them to undo the transformations
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -0.5f / cam_zoom, 0.5f / cam_zoom, -0.5f / cam_zoom, 0.5f / cam_zoom );
	glGetDoublev( GL_MODELVIEW_MATRIX, P );

	glLoadIdentity();
	glTranslatef( -cam_pos_x, -cam_pos_y, 0.0f );
	glGetDoublev( GL_MODELVIEW_MATRIX, M );
	glPopMatrix();

	glGetIntegerv( GL_VIEWPORT, V );

	double X,Y,Z;

	gluProject( v.x,v.y,0, M, P, V, &X, &Y, &Z );

	x = X;
	y = Y;
}