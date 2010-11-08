

#include "fw.h"

#include "app.h"

#include "Game/Engine.h"
#include "Game/Camera.h"
#include "Game/Field.h"

#include "Sprite/Sequence.h"

void Screen_2_App( int x, int y, Vector2& v );
void App_2_Screen( const Vector2& v, int& x, int& y );

App g_App;

#include "objLoader/obj.h"

void DrawModelUsingFixedFuncPipeline( ModelOBJ* in_pModel, bool in_bEnableModelTexture );

App::App()
{
	mx=0;
	my=0;
	last_mx=0;
	last_my=0;
	Buttons[0] = 0;
	Buttons[1] = 0;
	Buttons[2] = 0;
}

#include "objLoader/obj.h"

void App::Init()
{
	m_pEngine = new CEngine;

	Sequence	seq;

	seq.Load( "sample.spr" );
}

void App::Exit()
{
	delete m_pEngine;
}

void App::OnLeftClick( int x, int y, const Vector2& v )
{
	GetEngine()->GetCamera()->Goto( v, 1.0f );
}

void App::OnRightClick( int x, int y, const Vector2& v )
{

}

void App::OnMiddleClick( int x, int y, const Vector2& v )
{

}

void App::OnWheelUp()
{
	float fCurrZoom = g_App.GetEngine()->GetCamera()->GetZoom();
	if( fCurrZoom >= 0.25f )
	{
		GetEngine()->GetCamera()->ZoomTo( fCurrZoom / 2.0f, 1.0f );
	}
}

void App::OnWheelDown()
{
	float fCurrZoom = g_App.GetEngine()->GetCamera()->GetZoom();
	if( fCurrZoom <= 4.0f )
	{
		GetEngine()->GetCamera()->ZoomTo( fCurrZoom * 2, 1.0f );
	}
}

void App::OnKeyboard( unsigned char key )
{
	switch(key)
	{
	case 127:	// delete
		break;
	case '-':
		m_Grid.DecreaseGridSize();
		break;

	case '=':
		m_Grid.IncreaseGridSize();
		break;

	case ' ':
		GetEngine()->GetCamera()->Goto( Vector2(0,0), 1.0f );
		GetEngine()->GetCamera()->ZoomTo( 1.0f, 1.0f );
		break;
	}
}

void App::OnGamepad( unsigned int gamepad, unsigned int buttons, int axis_count, float* axis_values )
{
	switch( gamepad )
	{
	case 0:		GetEngine()->Gamepad0( buttons, axis_count, axis_values );	break;
	case 1:		GetEngine()->Gamepad0( buttons, axis_count, axis_values );	break;
	case 2:		GetEngine()->Gamepad0( buttons, axis_count, axis_values );	break;
	case 3:		GetEngine()->Gamepad0( buttons, axis_count, axis_values );	break;
	}
}

void App::OnOpenFile( const char* in_szFilename )
{
	m_pEngine->LoadFile( in_szFilename );
}

void App::Render()
{
	Vector2 vPos = GetEngine()->GetCamera()->GetPos();
	float fZoom = GetEngine()->GetCamera()->GetZoom();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -g_App.w/2*fZoom, g_App.w/2*fZoom, -g_App.h/2*fZoom,  g_App.h/2*fZoom );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( -vPos.x, -vPos.y, 0.0f );
	
	// todo: remove this test rendering
//	m_Grid.Render();

	m_pEngine->Render();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
		
	m_pEngine->RenderGUI();

	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %0.1fX -- w:%d, %d", m_pEngine->GetCamera()->GetZoom(), w, h );
}

void App::Update( float dt )
{
	//todo: emit any other update related signals
	m_pEngine->Update( dt );
}


// TEST GRID


Grid::Grid()
{
	grid_size = 16;
	UpdateGrid();
}

void Grid::IncreaseGridSize()
{
	grid_size *= 2;
	grid_size = MIN( grid_size, MAX_GRID_SIZE );
	UpdateGrid();
}

void Grid::DecreaseGridSize()
{
	grid_size /= 2;
	grid_size = MAX( grid_size, 1 );
	UpdateGrid();
}

void Grid::UpdateGrid()
{
	Vertex* p = vb_grid;

	int c = grid_size + 1;

	float d = 1000.0f / grid_size;

	int i,j;
	for( i=0; i<c; i++ )
	{
		for( j=0; j<c; j++ )
		{
			p->x = i * d - 500.0f;
			p->y = j * d- 500.0f;
			p->z = 0;
			p->color = COLORS::eYELLOW;
			++p;
		}
	}

	vb_grid_size = c * c;
}

void Grid::Render()
{
	gl_RenderPoints( vb_grid, 0, vb_grid_size, 2 );
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