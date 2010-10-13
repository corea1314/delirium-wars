

#include "fw.h"

#include "app.h"

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
	
	snap = false;
	grid_size = 16;
	UpdateGrid();

	cam_pos_x = 0.5f;
	cam_pos_y = 0.5f;
	zoom = -1;
	UpdateZoom();

	animate = false;
	timeSeconds = 0.0f;
}

void App::ZoomIn()
{
	zoom = MIN( ++zoom,  MAX_ZOOM_LEVEL );
	UpdateZoom();
}

void App::ZoomOut()
{
	zoom = MAX( --zoom, -MAX_ZOOM_LEVEL );
	UpdateZoom();
}

void App::ZoomReset()
{
	cam_pos_x = 0.5f;
	cam_pos_y = 0.5f;
	zoom = -1;
	UpdateZoom();
}

void App::UpdateZoom()
{
	if( zoom > 0 )
		cam_zoom = 1.0f + zoom * 0.05f;
	else if( zoom < 0 )
		cam_zoom = 1.0f / (1.0f + (-1 * zoom * 0.05f));
	else
		cam_zoom = 1.0f;
}

void App::IncreaseGridSize()
{
	grid_size *= 2;
	grid_size = MIN( grid_size, MAX_GRID_SIZE );
	UpdateGrid();
}

void App::DecreaseGridSize()
{
	grid_size /= 2;
	grid_size = MAX( grid_size, 1 );
	UpdateGrid();
}

void App::UpdateGrid()
{
	Vertex* p = vb_grid;
	
	int c = g_App.grid_size + 1;

	float d = 1.0f / g_App.grid_size;

	int i,j;
	for( i=0; i<c; i++ )
	{
		for( j=0; j<c; j++ )
		{
			p->x = i * d;
			p->y = j * d;
			p->z = 0;
			p->color = COLORS::eYELLOW;
			++p;
		}
	}

	vb_grid_size = c * c;
}

void App::Render()
{
	// Projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -0.5f / cam_zoom, 0.5f / cam_zoom, -0.5f / cam_zoom, 0.5f / cam_zoom );
	
	// Model view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef( -g_App.cam_pos_x, -g_App.cam_pos_y, 0.0f );

	{
		// Render grid
		gl_RenderPoints( vb_grid, 0, vb_grid_size, 2 );
		
		glPointSize( 8.0f );
		glLineWidth( 2.0f );
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW);
	glPopMatrix();

	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %d -- Grid: %d -- Snap: %s", zoom, grid_size, snap ? "true" : "false" );
}


