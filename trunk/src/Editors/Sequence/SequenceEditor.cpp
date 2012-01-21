
//TODO list for Sequence Editor
//- Implement frame naming scheme
//- Have a film-strip like display + controller


#include "SequenceEditor.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"

#include <algorithm>

void SequenceEditor::OnInit()
{
	mShowFrame = false;
	mShowAtlas = false;
	mCurrFrame = 0;
	mCurrTime = 0.0f;
	mAnimate = false;
	mFPS = 30;

	GetCamera()->GetPos().Set( 0.0f, 0.0f );
	
	mSprite = GetSpriteMan()->GetSprite();
}

void SequenceEditor::OnExit()
{
	if( mSprite )
	{
		GetSpriteMan()->FreeSprite(mSprite);
		mSprite = 0;
	}
}

void SequenceEditor::SetCurrFrame( int inFrame ) 
{ 
	mCurrFrame = inFrame; 
	mSprite->SetFrame( mSequence.GetFrame(mCurrFrame) );
}

void SequenceEditor::OnUpdate( float inDeltaTime )
{
	if( mAnimate && mSequence.GetFrameCount() != 0  )
	{
		mCurrTime += inDeltaTime;

		SetCurrFrame( (int)( mCurrTime * mFPS ) % mSequence.GetFrameCount() );
	}
}

void SequenceEditor::OnRender()
{
	Lair::GetAtlasMan()->Bind();

	if(mShowAtlas)	//todo cycle atlas pages
	{
#define AW	2048/2	//fixme should fetch from atlas
		gl_SetColor( COLORS::eWHITE );
		glEnable(GL_TEXTURE_2D );
		glBegin( GL_QUADS );
			glTexCoord2f(0,0); glVertex2f( -AW,-AW );
			glTexCoord2f(1,0); glVertex2f(  AW,-AW );
			glTexCoord2f(1,1); glVertex2f(  AW, AW );
			glTexCoord2f(0,1); glVertex2f( -AW, AW );
		glEnd();
		glDisable(GL_TEXTURE_2D );

		if( mShowFrame )
		{
			gl_SetColor( COLORS::eCYAN );

			glBegin( GL_LINE_LOOP );
			glVertex2f( -AW,-AW );
			glVertex2f(  AW,-AW );
			glVertex2f(  AW, AW );
			glVertex2f( -AW, AW );
			glEnd();
		}
	}
	else
	{
		if( mSequence.GetFrameCount() != 0 )// dont try to render empty sequence
		{
			GetSpriteMan()->Render();
		
			if( mShowFrame )
			{
				gl_SetColor( COLORS::eCYAN );

				float wx = mSequence.GetFrame(mCurrFrame)->GetSize().x;
				float wy = mSequence.GetFrame(mCurrFrame)->GetSize().y;
				float ox = mSequence.GetFrame(mCurrFrame)->GetOffset().x;
				float oy = mSequence.GetFrame(mCurrFrame)->GetOffset().y;

				glBegin( GL_LINE_LOOP );
				glVertex2f( -wx/2+ox, -wy/2+oy );
				glVertex2f(  wx/2+ox, -wy/2+oy );
				glVertex2f(  wx/2+ox,  wy/2+oy );
				glVertex2f( -wx/2+ox,  wy/2+oy );
				glEnd();
			}
		}
	}
}

void SequenceEditor::OnRenderGUI()
{
	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %f -- Grid: %d -- FPS: %d", GetCamera()->GetZoom(), GetGrid()->GetGridSize(), mFPS );
	gl_RenderText( 8, 720-16-8, "Sequence v%d.%d.%d (%s at %s)", 1, 0, 0, __DATE__, __TIME__ ); //todo display something better
}

void SequenceEditor::OnMouseClick( int button, int state, const MouseMotion& mm )
{
	Editor::OnMouseClick( button, state, mm );
}

void SequenceEditor::OnMouseMotion( const MouseMotion& mm )
{
	Editor::OnMouseMotion( mm );
}

void SequenceEditor::OnSpecialKey( int key, int mod )
{
	Editor::OnSpecialKey(key, mod);

	switch( key )
	{
	case SK_HOME:	SetCurrFrame(0);	break;
	case SK_LEFT:	
		mCurrFrame--; 
		mCurrFrame = std::max( 0, mCurrFrame ); 
		SetCurrFrame(mCurrFrame); 
		break;
	case SK_RIGHT:	
		mCurrFrame++; 
		mCurrFrame = std::min( (int)mSequence.GetFrameCount()-1, mCurrFrame); 
		SetCurrFrame(mCurrFrame); 
		break;
	
	case SK_PAGE_UP:	
		mFPS++;	
		break;
	case SK_PAGE_DOWN:	
		mFPS--;	
		mFPS = std::max( 1, mFPS ); 
		break;
	}
}

void SequenceEditor::OnCreateMenu()
{	
	Editor::OnCreateMenu();

	CREATE_MENU( pShow, "  Show..." );
		ADD_MENU_ITEM( pShow, "  Frame  ", &SequenceEditor::OnMenuShowFrame, 0 );
		ADD_MENU_ITEM( pShow, "  Atlas  ", &SequenceEditor::OnMenuShowAtlas, 0 );
			
	ADD_MENU_ITEM( GetMenu(), "  Add Frame(s)  ",	&SequenceEditor::OnMenuAddFrame, 0 );
	ADD_MENU_ITEM( GetMenu(), "  Animate  ",	&SequenceEditor::OnMenuAnimate, 0 );
}

void SequenceEditor::OnMenuAnimate( int inUnused )
{
	mAnimate = !mAnimate;
	ResetTime();
}

void SequenceEditor::OnMenuShowFrame( int inUnused )
{
	mShowFrame = !mShowFrame;
}

void SequenceEditor::OnMenuShowAtlas( int inUnused )
{
	mShowAtlas = !mShowAtlas;
}

void SequenceEditor::OnMenuAddFrame( int inUnused )
{
	FileSelection fs = GetFileLoad( "png", "PNG Image File\0*.png\0", true );

	for( FileSelection::iterator it = fs.begin(); it != fs.end(); it++ )
	{
		mSequence.AddFrame( it->c_str() );
		SetCurrFrame( mSequence.GetFrameCount() - 1 );
	}
}

void SequenceEditor::OnSerializeSave( TiXmlElement* inNode )
{
	TiXmlElement* pxmlSequence = new TiXmlElement("sequence");

	pxmlSequence->SetAttribute( "fps", mFPS );
	
	mSequence.SerializeSave(pxmlSequence);

	inNode->LinkEndChild(pxmlSequence);
}

void SequenceEditor::OnSerializeLoad( TiXmlElement* inNode )
{
	TiXmlElement* pxmlSequence = inNode->FirstChildElement("sequence");

	if( pxmlSequence )
	{
		pxmlSequence->Attribute( "fps", &mFPS );
		mSequence.SerializeLoad(pxmlSequence);
	}
}