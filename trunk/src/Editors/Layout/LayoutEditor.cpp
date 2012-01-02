

#include "LayoutEditor.h"
#include "LayoutElement.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"
#include "Lair/Input/Input.h"

#include "../WidgetRect.h"
#include "../GizmoScaling.h"
#include "../GizmoRotation.h"
#include "../GizmoTranslation.h"

static const char* OPENFILE_BCF_EXTENSION = "lay";
static const char* OPENFILE_BCF_FILTER = "Layout File\0*.lay\0";

#define LAY_VERSION_MAIN	1
#define LAY_VERSION_MAJOR	0
#define LAY_VERSION_MINOR	0

static const char* kModeString[] = { "Selection", "Point", "Rect" };


void LayoutEditor::OnInit()
{
	SetMode(Mode::Selection);

	GetCamera()->GetPos().Set( 256.0f, 256.0f );

	mWidgetRect = new WidgetRect(this);
	
	mGizmoScaling = new GizmoScaling(this);	
	mGizmoScaling->mPos.Set(-256.0f, 256.0f);

	mGizmoRotation = new GizmoRotation(this);
	mGizmoRotation->mPos.Set( 256.0f, 256.0f);

	mGizmoTranslation = new GizmoTranslation(this);
}

void LayoutEditor::OnExit()
{
	SAFE_DELETE(mWidgetRect);
	SAFE_DELETE(mGizmoScaling);
	SAFE_DELETE(mGizmoRotation);
	SAFE_DELETE(mGizmoTranslation);

	glPointSize( 8.0f );
	glLineWidth( 2.0f );
}

void LayoutEditor::OnUpdate( float inDeltaTime )
{
}

void LayoutEditor::OnRender()
{
	GetGrid()->Render();

	for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
		(*it)->OnRender();

	mWidgetRect->OnRender();
	mGizmoScaling->OnRender();
	mGizmoRotation->OnRender();
	mGizmoTranslation->OnRender();
}

void LayoutEditor::OnRenderGUI()
{
	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %f -- Grid: %d -- Snap: %s", GetCamera()->GetZoom(), GetGrid()->GetGridSize(), GetGrid()->GetSnap() ? "true" : "false" );
	gl_RenderText( 8, 720-16-8, "Layout Editor v%d.%d.%d (%s at %s) - Mode: %s", LAY_VERSION_MAIN, LAY_VERSION_MAJOR, LAY_VERSION_MINOR, __DATE__, __TIME__, kModeString[mMode] );

	for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
		(*it)->OnRenderGUI();

	mWidgetRect->OnRenderGUI();
	mGizmoScaling->OnRenderGUI();
	mGizmoRotation->OnRenderGUI();
	mGizmoTranslation->OnRenderGUI();
}

void LayoutEditor::OnMouseClick( int button, int state, int x, int y, int mod )
{
	Vector2 v;	
	ScreenToEditor( x, y, v );

	switch( mMode )
	{
	case Mode::Selection:
		{
			for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
				(*it)->OnMouseClick( button, state, v, mod );
			mGizmoScaling->OnMouseClick( button, state, v, mod );
			mGizmoRotation->OnMouseClick( button, state, v, mod );
			mGizmoTranslation->OnMouseClick( button, state, v, mod );
		}		
		break;
	case Mode::Point:
		{
			mElements.push_back( new LayoutElement(v,this) );
		}
		break;
	case Mode::Rect:
		{
			mWidgetRect->OnMouseClick( button, state, v, mod );	
		}
		break;
	}	
}

void LayoutEditor::OnMouseMotion( int x, int y, int dx, int dy, int mod )
{
	Vector2 last; 
	ScreenToEditor( x-dx, y-dy, last );

	Vector2 v;	
	ScreenToEditor( x, y, v );
	Vector2 d = v - last;

	switch( mMode )
	{
	case Mode::Selection:
		{
			bool bMovingElement = false;

			for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
				bMovingElement |= (*it)->OnMouseMotion( v, d, mod );

// fixme			if( bMovingElement == false )
//				Editor::OnMouseMotion( x,y,dx,dy,mod );
			mGizmoScaling->OnMouseMotion( v, d, mod );
			mGizmoRotation->OnMouseMotion( v, d, mod );
			mGizmoTranslation->OnMouseMotion( v, d, mod );
		}
		break;
	case Mode::Rect:
		{
			mWidgetRect->OnMouseMotion( v, d, mod );
		}
		break;
	}
}

void LayoutEditor::OnSpecialKey( int key, int mod )
{
	Editor::OnSpecialKey(key, mod);

	for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
		(*it)->OnSpecialKey( key, mod );
}

void LayoutEditor::OnKeyboard( unsigned char key, int mod )
{
	Editor::OnKeyboard(key, mod);

	switch( mMode )
	{
	case Mode::Selection:
		{
			for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
				(*it)->OnKeyboard( key, mod );

			if( key == 127 ) // Delete ley
				CleanupDeleted();

			mGizmoScaling->OnKeyboard( key, mod );
			mGizmoRotation->OnKeyboard( key, mod );
			mGizmoTranslation->OnKeyboard( key, mod );
		}
		break;
	case Mode::Rect:
		{
			mWidgetRect->OnKeyboard( key, mod );
		}
		break;
	}	
}

void LayoutEditor::SetMode( Mode::E inMode )
{
	mMode = inMode;
}

// a predicate implemented as a function:
bool ShouldDelete (const LayoutElement* inElement ) { return inElement->mDeleteRequest; }

void LayoutEditor::CleanupDeleted()
{
	mElements.remove_if(ShouldDelete);
}

void LayoutEditor::OnCreateMenu()
{	
	CREATE_MENU( pFile, "  File...  " );
		ADD_MENU_ITEM( pFile, "  Save  ", &Editor::OnMenuFileSave, 0 );
		ADD_MENU_ITEM( pFile, "  Load  ", &Editor::OnMenuFileLoad, 0 );

	CREATE_MENU( pMode, "  Mode...  " );
		ADD_MENU_ITEM( pMode, "  Selection  ", &LayoutEditor::OnMenuMode, Mode::Selection );
		ADD_MENU_ITEM( pMode, "  Point  ", &LayoutEditor::OnMenuMode, Mode::Point );
		ADD_MENU_ITEM( pMode, "  Rect  ", &LayoutEditor::OnMenuMode, Mode::Rect );
		
//	ADD_MENU_ITEM( GetMenu(), "Animate",	&VisualEditor::OnMenuAnimate, 0 );
//	ADD_MENU_ITEM( GetMenu(), "Texture",	&VisualEditor::OnMenuTexture, 0 );	
}

void LayoutEditor::OnMenuMode( int inMode )
{
	SetMode( (Mode::E)inMode );
}
