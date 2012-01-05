
#include "gfx.h"

#include "Editor.h"
#include "Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"

#define CAMERA_MID_ZOOM_LEVEL	100.0f

void ConvertMultiToArray(LPCSTR inPath, std::vector<std::string> &outArray);

Editor::Editor() 
	: lock(false)
{
	mGrid = new Grid;
	mZoomLevel = 100;
	mCamera = new Camera;
	mCamera->GetZoom() = CAMERA_MID_ZOOM_LEVEL / mZoomLevel;

	mViewportSize.x = ( 1280.0f ) / 2.0f;
	mViewportSize.y = (  720.0f ) / 2.0f;
}

Editor::~Editor()
{
	delete mCamera;
	delete mGrid;
}

void Editor::Init()
{
	ResetTime();
	
	MenuUser::OnCreateMenu();
	OnCreateMenu();
	GetMenu()->BindButton(2);	// right button
	
	OnInit();
}

void Editor::Exit()
{
	OnExit();

	GetMenu()->UnbindButton(2);	// right button
	OnDestroyMenu();
	MenuUser::OnDestroyMenu();
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

	OnUpdate( dt );
}

void Editor::OnKeyboard( unsigned char key, int mod )
{
	switch(key)
	{
	case '-':	GetGrid()->DecreaseGridSize();		break;
	case '=':	GetGrid()->IncreaseGridSize();		break;
	case 127:	/* delete */						break;
	case ' ':	GetGrid()->ToggleSnap();			break;
	}
}

void Editor::OnMouseWheel( int v, int mod )
{
	mZoomLevel += v;

	if( mZoomLevel < 1 )
		mZoomLevel = 1;

	mCamera->GetZoom() = CAMERA_MID_ZOOM_LEVEL / mZoomLevel;
}

void Editor::OnMouseClick( int button, int state, int x, int y, int mod )
{
	// todo translate from screen to editor space
}

void Editor::OnMouseMotion( int x, int y, int dx, int dy, int mod )
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

void Editor::OnSpecialKey( int key, int mod )
{
	/*
	switch( key )
	{
	default:
		break;
	}
	*/
}

void Editor::OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values )
{

}

const Editor::FileSelection& Editor::GetFileSave( const char* extension, const char* filter )
{
	static char filename[MAX_PATH];
	filename[0] = 0; // requirement of ::OpenFilename

	mSelectedFilename.clear();

	if( !lock && GetSaveFilename(filename,sizeof(filename), filter, extension ) )
	{
		return mSelectedFilename;
	}
	return mSelectedFilename;
}

const Editor::FileSelection& Editor::GetFileLoad( const char* extension, const char* filter, bool multiple )
{
	static char filename[MAX_PATH*32];
	filename[0] = 0; // requirement of ::OpenFilename

	mSelectedFilename.clear();

	if( !lock && GetLoadFilename(filename,sizeof(filename),filter,extension,multiple) )
	{
		return mSelectedFilename;
	}
	return mSelectedFilename;
}

bool Editor::GetSaveFilename( char filename[], int count, const char* filter, const char* extension )
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
	openFilename.Flags           = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN | OFN_EXPLORER;

	lock = true;
	bool result = ::GetSaveFileName(&openFilename) != 0;
	ConvertMultiToArray( filename, mSelectedFilename );
	lock = false;
	
	return result;
}

bool Editor::GetLoadFilename( char filename[], int count, const char* filter, const char* extension, bool multiple )
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
	openFilename.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;

	if( multiple )
		openFilename.Flags |= OFN_ALLOWMULTISELECT;

	lock = true;
	bool result = ::GetOpenFileName(&openFilename) != 0;
	ConvertMultiToArray( filename, mSelectedFilename );
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

void Editor::OnMenuFileSave( int unused )
{
	FileSelection fs = GetFileSave( GetFileExtension(), GetFileFilter() );
	if( fs.size() != 0 )
	{
		TiXmlDocument doc;

		TiXmlElement * pxmlRoot = new TiXmlElement( GetFileExtension() );
		
		int nVersion = 0;	// todo save version
		pxmlRoot->SetAttribute("version", nVersion);

			OnSerializeSave( pxmlRoot );

			// todo save editor options (camera, grid, zoom)
		
		doc.LinkEndChild( pxmlRoot );

		doc.SaveFile( fs[0].c_str() );
	}
}

void Editor::OnMenuFileLoad( int unused )
{
	FileSelection fs = GetFileLoad( GetFileExtension(), GetFileFilter() );
	if( fs.size() != 0 )
	{
		TiXmlDocument doc;
		doc.LoadFile( fs[0].c_str() );

		if ( doc.Error() )
		{
			Lair::GetLogMan()->Log( "Editor", "OnFileLoad - XML parse error in file %s - %s: %s (Line %d)\n", fs[0].c_str(), doc.Value(), doc.ErrorDesc(), doc.ErrorRow() );
			return;
		}

		TiXmlElement* pxmlRoot = doc.FirstChildElement( GetFileExtension() );

		if( pxmlRoot )
		{
			// todo load and validate version
			int nVersion;
			pxmlRoot->Attribute("version", &nVersion);

			// todo load editor options (camera, grid, zoom)

			OnSerializeLoad( pxmlRoot );
		}
	}
}


void ConvertMultiToArray(LPCSTR inPath, std::vector<std::string> &outArray)
{
	char	szDir[MAX_PATH+1] = "";
	char	szFile[MAX_PATH+1];
	LPCSTR	p;

	outArray.clear();

	for (p = inPath; *p; p += lstrlen(p) + 1)
	{
		if (szDir[0] == '\0')
		{
			lstrcpyn(szDir, p, sizeof(szDir));
		}
		else
		{
			_snprintf_s(szFile, sizeof(szFile), "%s\\%s", szDir, p);
			outArray.push_back(szFile);
		}
	}

	// If there is only one file its here
	if (outArray.size() == 0 && szDir[0] != '\0')
	{
		outArray.push_back(szDir);
	}

	// The file names come back in an unpredictable order (not reversed) so sorting is the best we can do - thanks Microsoft
	std::sort(outArray.begin(), outArray.end());
}
