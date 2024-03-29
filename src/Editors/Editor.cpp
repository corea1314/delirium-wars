

//TODO list for editor
//- 


#include "gfx.h"

#include "Editor.h"
#include "Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"

#include "Editors/Gizmo/GizmoAlpha.h"
#include "Editors/Gizmo/GizmoScaling.h"
#include "Editors/Gizmo/GizmoRotation.h"
#include "Editors/Gizmo/GizmoTranslation.h"
#include "Editors/Gizmo/GizmoSelection.h"

#include "EditorElement.h"

#define CAMERA_MID_ZOOM_LEVEL	100.0f

void ConvertMultiToArray(LPCSTR inPath, std::vector<std::string> &outArray);

Editor::Editor() 
	: lock(false)
{
	mTextEntryRequest = false;
	mGrid = new Grid;
	mZoomLevel = 100;
	mCamera = new Camera;
	mCamera->GetZoom() = CAMERA_MID_ZOOM_LEVEL / mZoomLevel;

	mViewportSize.x = ( 1280.0f ) / 2.0f;
	mViewportSize.y = (  720.0f ) / 2.0f;

	mGizmoAlpha = new GizmoAlpha(this);
	mGizmoScaling = new GizmoScaling(this);	
	mGizmoRotation = new GizmoRotation(this);
	mGizmoTranslation = new GizmoTranslation(this);
	mGizmoSelection = new GizmoSelection(this);

	ActivateGizmo( GizmoType::Selection );
}

Editor::~Editor()
{
	delete mCamera;
	delete mGrid;
	delete mGizmoSelection;
	delete mGizmoTranslation;
	delete mGizmoRotation;
	delete mGizmoScaling;
	delete mGizmoAlpha;
}


void Editor::ActivateGizmo( GizmoType::E inGizmoType )
{
	mActiveGizmoType = inGizmoType;
	switch( mActiveGizmoType )
	{
	case GizmoType::Scaling:		mActiveGizmo = mGizmoScaling;		break;
	case GizmoType::Rotation:		mActiveGizmo = mGizmoRotation;		break;
	case GizmoType::Translation:	mActiveGizmo = mGizmoTranslation;	break;
	case GizmoType::Alpha:			mActiveGizmo = mGizmoAlpha;			break;
	case GizmoType::Selection:		mActiveGizmo = mGizmoSelection;		break;
	}

	OnActivateGizmo();
}

void Editor::AddElement( EditorElement* inElement )
{
	mElements.push_back( inElement );
}

void Editor::Init()
{
	ResetTime();

	mSpriteMan = new SpriteMan;
	mSpriteMan->Init(64);
	
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

	mSpriteMan->Exit();
	delete mSpriteMan;
	mSpriteMan = 0;
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

		for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
			(*it)->OnRender();

		// Render in editor space
		OnRender();

		if( mActiveGizmo )
			mActiveGizmo->OnRender();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW);
	glPopMatrix();

	// Render in screen space
	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
		(*it)->OnRenderGUI();

	OnRenderGUI();

	if( mActiveGizmo )
		mActiveGizmo->OnRenderGUI();
}

void Editor::Update( float dt )
{
	mTimeSeconds += dt;

	OnUpdate( dt );

	if( mActiveGizmo )
		mActiveGizmo->OnUpdate( dt );
}


///////////////////////////////////////////////////////////////////////////////
// OnSelect: Called by Selection Gizmo on click
//
void Editor::OnSelect( const Vector2& inPos )
{
	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
	{
		(*it)->mSelected = (*it)->OnSelect( inPos );
	}
}


///////////////////////////////////////////////////////////////////////////////
// OnSelectRect: Called by Selection Gizmo on drag
//
void Editor::OnSelectRect( const Vector2& inMin, const Vector2& inMax )
{
	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
	{
		(*it)->mSelected = (*it)->OnSelectRect( inMin, inMax );
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnSelectTextEntry: Called by Selection Gizmo on text entry
//
void Editor::OnSelectTextEntry( const std::string& inText )
{
	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
	{
		if( (*it)->mSelected )
			(*it)->mName = inText;
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnKeyboard: Called by app when keys are pressed
//
void Editor::OnKeyboard( unsigned char key, int mod, bool down )
{
	if( !down ) 
		return; // process key down only

	if( mTextEntryRequest )
	{		
		switch(key)
		{
			case 27: 	// Escape key
			case 13:	// Enter key
				mTextEntryRequest = false;
				break;
			default:
				{
					for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
					{
						if( (*it)->mSelected )
							(*it)->OnKeyboard( key, mod, down );
					}
				}
				break;
		}
	}
	else
	{
		if( mActiveGizmo )
			mActiveGizmo->OnKeyboard( key, mod, down );

		switch(key)
		{
		case 'q':	ActivateGizmo( GizmoType::Selection );		break;
		case 'w':	ActivateGizmo( GizmoType::Translation );	break;
		case 'e':	ActivateGizmo( GizmoType::Rotation );		break;
		case 'r':	ActivateGizmo( GizmoType::Scaling );		break;
		case 'a':	ActivateGizmo( GizmoType::Alpha );			break;
		case 't':	mTextEntryRequest = true;			break;

		case '[':	GetGrid()->DecreaseGridSize();		break;
		case ']':	GetGrid()->IncreaseGridSize();		break;
		case 127:	CleanupDeleted();					break;			
		case 's':	GetGrid()->ToggleSnap();			break;
		}
	}
}

// a predicate implemented as a function:
bool ShouldDelete (const EditorElement* inElement ) { return inElement->mDeleteRequest; }

void Editor::CleanupDeleted()
{
	mElements.remove_if(ShouldDelete);
}

///////////////////////////////////////////////////////////////////////////////
// OnMouseWheel: Called by app on mouse wheel
//
void Editor::OnMouseWheel( int v, int mod )
{
	mZoomLevel += v;

	if( mZoomLevel < 1 )
		mZoomLevel = 1;

	mCamera->GetZoom() = CAMERA_MID_ZOOM_LEVEL / mZoomLevel;
}

void Editor::BuildMouseMotion( MouseMotion& mm, int x, int y, int dx, int dy, int mod )
{
	mm.x = x; mm.y = y; mm.dx = dx; mm.dy = dy; mm.mod = mod;

	ScreenToEditor( x, y, mm.pos );
	
	if( dx==0 && dy==0 )
	{
		mm.delta.Set(0.0f,0.0f);
	}
	else
	{
		Vector2 last; 
		ScreenToEditor( x-dx, y-dy, last );
		mm.delta = mm.pos - last;
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnMouseClick: Called by app on mouse click
//
void Editor::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	if( mActiveGizmo )
		mActiveGizmo->OnMouseClick( button, state, mm );
	else
	{
		for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
			(*it)->OnMouseClick( button, state, mm );
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnMouseMotion: Called by app on mouse drag
//
void Editor::OnMouseMotion( const MouseMotion& mm )
{
	if( mActiveGizmo )
		mActiveGizmo->OnMouseMotion( mm );

	// On drag with middle button, move camera
	if( Lair::GetInputMan()->IsMouseButtonDown( InputMan::MouseButton::Middle ) )	// if left mouse button is down
		mCamera->GetPos() -= mm.delta;
}

///////////////////////////////////////////////////////////////////////////////
// OnSpecialKey: Called by app on keyboard entry
//
void Editor::OnSpecialKey( int key, int mod, bool down )
{
	if( !down )
		return; // process key down only

	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
	{
		if( (*it)->mSelected )
			(*it)->OnSpecialKey( key, mod, down );
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnGamepad: Called by app on gamepad data change
//
void Editor::OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values )
{
//	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
//	{
//		if( (*it)->mSelected )
//			(*it)->OnGamepad( gamepad, buttons, axis_count, axis_values );
//	}
}

///////////////////////////////////////////////////////////////////////////////
// OnTranslate: Called by Translation Gizmo on manipulation
//
void Editor::OnTranslate( const Vector2& inNewPos, const Vector2& inDelta )
{
	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
	{
		if( (*it)->mSelected )
			(*it)->OnTranslate( inNewPos, inDelta );
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnScale: Called by Scaling Gizmo on manipulation
//
void Editor::OnScale( const Vector2& inNewScale, const Vector2& inDelta )
{
	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
	{
		if( (*it)->mSelected )
			(*it)->OnScale( inNewScale, inDelta );
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnRotation: Called by Rotation Gizmo on manipulation
//
void Editor::OnRotate( float inAngle, float inDelta )
{
	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
	{
		if( (*it)->mSelected )
			(*it)->OnRotate( inAngle, inDelta );
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnAlpha: Called by Alpha Gizmo on manipulation
//
void Editor::OnAlpha( float inAlpha, float inDelta )
{
	for( std::list<EditorElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
	{
		if( (*it)->mSelected )
			(*it)->OnRotate( inAlpha, inDelta );
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnCreateMneu: Called by Init
//
void Editor::OnCreateMenu()
{	
	CREATE_MENU( pFile, "  File...  " );
		ADD_MENU_ITEM( pFile, "  Save  ", &Editor::OnMenuFileSave, 0 );
		ADD_MENU_ITEM( pFile, "  Load  ", &Editor::OnMenuFileLoad, 0 );

	CREATE_MENU( pTransform, "  Transform...  " );
		ADD_MENU_ITEM( pTransform, "  Selection  ", &Editor::OnMenuTransform, GizmoType::Selection );
		ADD_MENU_ITEM( pTransform, "  Translation  ", &Editor::OnMenuTransform, GizmoType::Translation );
		ADD_MENU_ITEM( pTransform, "  Rotation  ", &Editor::OnMenuTransform, GizmoType::Rotation );
		ADD_MENU_ITEM( pTransform, "  Scaling  ", &Editor::OnMenuTransform, GizmoType::Scaling );
		ADD_MENU_ITEM( pTransform, "  Alpha  ", &Editor::OnMenuTransform, GizmoType::Alpha );
}

///////////////////////////////////////////////////////////////////////////////
// OnMenuTransform: Called by menu on selection
//
void Editor::OnMenuTransform( int inGizmoType )
{
	ActivateGizmo( (GizmoType::E)inGizmoType );
}

///////////////////////////////////////////////////////////////////////////////
// GetFileSave: Helper called by menu on saving
//
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

///////////////////////////////////////////////////////////////////////////////
// GetFileLoad: Helper called by menu on loading
//
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

///////////////////////////////////////////////////////////////////////////////
// ScreenToEditor: Helper to convert from screen to editor space
//
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

///////////////////////////////////////////////////////////////////////////////
// EditorToScreen: Helper to convert from editor to screen space
//
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
