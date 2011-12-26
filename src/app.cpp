

#include "fw.h"

#include "app.h"

#include "keys.h"

#include "Engine/Engine.h"
#include "Engine/Clock/Clock.h"

#include "Lair/Lair.h"

#include "Editors/Editor.h"
#include "Editors/Curve/CurveEdit.h"

void Screen_2_App( int x, int y, Vector2& v );
void App_2_Screen( const Vector2& v, int& x, int& y );

App g_App;

App::App()
{
	mx=0;
	my=0;
	last_mx=0;
	last_my=0;
	Buttons[0] = 0;
	Buttons[1] = 0;
	Buttons[2] = 0;

	fps = 0;
	fps_average = 0;
	fps_time = 0.0f;
}

#include "objLoader/obj.h"

void App::Init()
{
	m_pEngine = new CEngine;

	m_pActiveEditor = 0;

	m_pEditors[0] = new CurveEditor;
	m_pEditors[1] = 0;
	m_pEditors[2] = 0;
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

void App::OnMouseClick( int button, int x, int y, Vector2& v )
{
	GetEngine()->OnMouseClick( button, x, y, v );

	if( m_pActiveEditor )
		m_pActiveEditor->OnMouseClick( button, x, y, v );
}

void App::OnMouseMotion( int x, int y, Vector2& v, int dx, int dy, Vector2& d )
{
	if( m_pActiveEditor )
		m_pActiveEditor->OnMouseMotion( x, y, v, dx, dy, d );
}

void App::OnMouseWheel( int v )
{
	GetEngine()->OnMouseWheel( v );

	if( m_pActiveEditor )
		m_pActiveEditor->OnMouseWheel( v );
}

void App::OnKeyboard( unsigned char key )
{
	GetEngine()->OnKeyboard( key );

	if( m_pActiveEditor )
		m_pActiveEditor->OnKeyboard( key );
}

void App::OnSpecialKey( int key )
{
	if( m_pActiveEditor )
		m_pActiveEditor->OnSpecialKey( key );

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
	switch( gamepad )
	{
	case 0:		GetEngine()->OnGamepad0( buttons, axis_count, axis_values );	break;
	case 1:		GetEngine()->OnGamepad1( buttons, axis_count, axis_values );	break;
	case 2:		GetEngine()->OnGamepad2( buttons, axis_count, axis_values );	break;
	case 3:		GetEngine()->OnGamepad3( buttons, axis_count, axis_values );	break;
	}

	if( m_pActiveEditor )
		m_pActiveEditor->OnGamepad( gamepad, buttons, axis_count, axis_values );
}

void App::OnOpenFile( const char* in_szFilename )
{
	m_pEngine->OnLoadFile( in_szFilename );
}

void App::Render()
{
	++fps;
	if( fps_time < m_pEngine->GetClock()->GetTotalTime() )
	{
		fps_average += fps;
		fps_average /= 2;
		fps_time = m_pEngine->GetClock()->GetTotalTime() + 1.0f;
		fps = 0;
	}

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
	m_pEngine->Render();
	m_pEngine->RenderGUI();
	
	glDisable( GL_TEXTURE_2D );
	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "FPS: %d (%d) - Zoom: %0.1fX -- w:%d, %d", fps, fps_average, Lair::GetCameraMan()->GetActiveCamera()->GetZoom(), w, h );

	if( m_pActiveEditor )
		m_pActiveEditor->Render();
}

void App::Update( float dt )
{
	//todo: emit any other update related signals
	m_pEngine->Update( dt );
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

		if( m_pActiveEditor == m_pEditors[inEditorId] )
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



void DrawModelUsingFixedFuncPipeline( ModelOBJ* in_pModel, bool in_bEnableModelTexture )
{
	const ModelOBJ::Mesh *pMesh = 0;
	const ModelOBJ::Material *pMaterial = 0;
	const ModelOBJ::Vertex *pVertices = 0;
//	ModelTextures::const_iterator iter;

	for (int i = 0; i < in_pModel->getNumberOfMeshes(); ++i)
	{
		pMesh = &in_pModel->getMesh(i);
		pMaterial = pMesh->pMaterial;
		pVertices = in_pModel->getVertexBuffer();

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pMaterial->ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pMaterial->diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pMaterial->specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pMaterial->shininess * 128.0f);

		/*
		if (in_bEnableModelTexture)
		{
			iter = g_modelTextures.find(pMaterial->colorMapFilename);

			if (iter == g_modelTextures.end())
			{
				glDisable(GL_TEXTURE_2D);
			}
			else
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, iter->second);
			}
		}
		*/
		
		if (in_pModel->hasPositions())
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, in_pModel->getVertexSize(),
				in_pModel->getVertexBuffer()->position);
		}

		if (in_pModel->hasTextureCoords())
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, in_pModel->getVertexSize(),
				in_pModel->getVertexBuffer()->texCoord);
		}

		if (in_pModel->hasNormals())
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, in_pModel->getVertexSize(),
				in_pModel->getVertexBuffer()->normal);
		}

		glDrawElements(GL_TRIANGLES, pMesh->triangleCount * 3, GL_UNSIGNED_INT,
			in_pModel->getIndexBuffer() + pMesh->startIndex);

		if (in_pModel->hasNormals())
			glDisableClientState(GL_NORMAL_ARRAY);

		if (in_pModel->hasTextureCoords())
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (in_pModel->hasPositions())
			glDisableClientState(GL_VERTEX_ARRAY);
	}
}