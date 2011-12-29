

#include "LayoutEditor.h"
#include "LayoutElement.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"
#include "Lair/Input/Input.h"

static const char* OPENFILE_BCF_EXTENSION = "lay";
static const char* OPENFILE_BCF_FILTER = "Layout File\0*.lay\0";

#define LAY_VERSION_MAIN	1
#define LAY_VERSION_MAJOR	0
#define LAY_VERSION_MINOR	0


void LayoutEditor::OnInit()
{
	GetCamera()->GetPos().Set( 256.0f, 256.0f );

	mElements.push_back( new LayoutElement( Vector2( 210, 440), this ) );
	mElements.push_back( new LayoutElement( Vector2( 350, 350), this ) );
	mElements.push_back( new LayoutElement( Vector2( 570, 120), this ) );
	mElements.push_back( new LayoutElement( Vector2( 380,  90), this ) );
	mElements.push_back( new LayoutElement( Vector2( 190, 130), this ) );
	mElements.push_back( new LayoutElement( Vector2( 220, 340), this ) );
}

void LayoutEditor::OnExit()
{
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
}

void LayoutEditor::OnRenderGUI()
{
	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %f -- Grid: %d -- Snap: %s", GetCamera()->GetZoom(), GetGrid()->GetGridSize(), GetGrid()->GetSnap() ? "true" : "false" );
	gl_RenderText( 8, 720-16-8, "Layout Editor v%d.%d.%d (%s at %s)", LAY_VERSION_MAIN, LAY_VERSION_MAJOR, LAY_VERSION_MINOR, __DATE__, __TIME__ );

	for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
		(*it)->OnRenderGUI();
}

void LayoutEditor::OnMouseClick( int button, int x, int y, int mod )
{
	Vector2 v;	
	ScreenToEditor( x, y, v );

	for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
		(*it)->OnMouseClick( button, v, mod );	
}

void LayoutEditor::OnMouseMotion( int x, int y, int dx, int dy, int mod )
{
	Vector2 last; 
	ScreenToEditor( x-dx, y-dy, last );

	Vector2 v;	
	ScreenToEditor( x, y, v );
	Vector2 d = v - last;

	bool bMovingElement = false;

	for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
		bMovingElement |= (*it)->OnMouseMotion( v, d, mod );

	if( bMovingElement == false )
		Editor::OnMouseMotion( x,y,dx,dy,mod );
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

	for( std::list<LayoutElement*>::iterator it=mElements.begin(); it != mElements.end(); it++ )
		(*it)->OnKeyboard( key, mod );
}

void LayoutEditor::OnCreateMenu()
{	
	CREATE_MENU( pFile, "  File...  " );
		ADD_MENU_ITEM( pFile, "  Save  ", &LayoutEditor::OnMenuFileSave, 0 );
		ADD_MENU_ITEM( pFile, "  Load  ", &LayoutEditor::OnMenuFileSave, 0 );
		
//	ADD_MENU_ITEM( GetMenu(), "Animate",	&VisualEditor::OnMenuAnimate, 0 );
//	ADD_MENU_ITEM( GetMenu(), "Texture",	&VisualEditor::OnMenuTexture, 0 );	
}

void LayoutEditor::OnMenuFileSave( int inUnused )
{

}

void LayoutEditor::OnMenuFileLoad( int inUnused )
{

}
