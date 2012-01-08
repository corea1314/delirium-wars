
//TODO
// - Finish rect insertion/deletion/manipulation


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
}

void LayoutEditor::OnExit()
{
	SAFE_DELETE(mWidgetRect);


	glPointSize( 8.0f );
	glLineWidth( 2.0f );
}

void LayoutEditor::OnUpdate( float inDeltaTime )
{
}

void LayoutEditor::OnRender()
{
	Editor::OnRender();

	mWidgetRect->OnRender();	//todo render rect widget in 2d
//	mGizmoScaling->OnRender();
//	mGizmoRotation->OnRender();
//	mGizmoTranslation->OnRender();
}

void LayoutEditor::OnRenderGUI()
{
	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %f -- Grid: %d -- Snap: %s", GetCamera()->GetZoom(), GetGrid()->GetGridSize(), GetGrid()->GetSnap() ? "true" : "false" );
	gl_RenderText( 8, 720-16-8, "Layout Editor v%d.%d.%d (%s at %s) - Mode: %s", LAY_VERSION_MAIN, LAY_VERSION_MAJOR, LAY_VERSION_MINOR, __DATE__, __TIME__, kModeString[mMode] );
		
	mWidgetRect->OnRenderGUI();
//	mGizmoScaling->OnRenderGUI();
//	mGizmoRotation->OnRenderGUI();
//	mGizmoTranslation->OnRenderGUI();
}

void LayoutEditor::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	switch( mMode )
	{
	case Mode::Selection:
		{
			Editor::OnMouseClick( button, state, mm );
		}		
		break;
	case Mode::Point:
		{			
			if( state ) // On click down, TODO create a real object and its associated editor element
				AddElement( new LayoutElement( mm.x, mm.y, mm.pos, this ) );
		}
		break;
	case Mode::Rect:
		{
			mWidgetRect->OnMouseClick( button, state, mm );	
		}
		break;
	}	
}

void LayoutEditor::OnMouseMotion( const MouseMotion& mm )
{
	switch( mMode )
	{
	case Mode::Selection:
		{
			Editor::OnMouseMotion( mm );
		}
		break;
	case Mode::Rect:
		{
			mWidgetRect->OnMouseMotion( mm );
		}
		break;
	}
}

void LayoutEditor::OnKeyboard( unsigned char key, int mod )
{
	Editor::OnKeyboard(key, mod);

	switch( mMode )
	{
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

void LayoutEditor::OnSerializeSave( TiXmlElement* inNode )
{
	TiXmlElement* pxmlLayout = new TiXmlElement("layout");

	//fixme: here we should save actual layout objects, not editor elements.

	/*
	TiXmlElement* pxmlElement;
	for( std::list<EditorElement*>::iterator it = mElements.begin(); it != mElements.end(); it++ )
	{
		pxmlElement = new TiXmlElement("point");
		pxmlElement->SetAttribute( "name", (*it)->mName );
		pxmlElement->SetDoubleAttribute( "position.x", (*it)->mPos.x );
		pxmlElement->SetDoubleAttribute( "position.y", (*it)->mPos.y );
		pxmlElement->SetDoubleAttribute( "angle", (*it)->mAngle );

		pxmlLayout->LinkEndChild(pxmlElement);
	}
	*/

	inNode->LinkEndChild(pxmlLayout);
}

void LayoutEditor::OnSerializeLoad( TiXmlElement* inNode )
{
	TiXmlElement* pxmlLayout = inNode->FirstChildElement("layout");

	if( pxmlLayout )
	{
//		const char* szPreLoop = pxmlLayout->Attribute( "PreLoop" );
		double fAttribute;
		
		for( TiXmlElement* pxmlElement = pxmlLayout->FirstChildElement("point"); pxmlElement; pxmlElement = pxmlElement->NextSiblingElement("point") )
		{	
			LayoutElement* pElement = new LayoutElement(this);

			pElement->mName = pxmlElement->Attribute( "name" );
			pxmlElement->Attribute( "position.x", &fAttribute );	pElement->mPos.x = (float)fAttribute;
			pxmlElement->Attribute( "position.y", &fAttribute );	pElement->mPos.y = (float)fAttribute;
			pxmlElement->Attribute( "angle", &fAttribute );			pElement->mAngle = (float)fAttribute;

			mElements.push_back( pElement );
		}	
	}
}