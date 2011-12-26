#include "Grid.h"

#include <algorithm>

Grid::Grid()
{
	snap = false;
	grid_size = 16;
	UpdateGrid();
}

void Grid::ToggleSnap()
{
	snap = !snap;
}

void Grid::IncreaseGridSize()
{
	grid_size *= 2;
	grid_size = std::min( grid_size, MAX_GRID_SIZE );
	UpdateGrid();
}

void Grid::DecreaseGridSize()
{
	grid_size /= 2;
	grid_size = std::max( grid_size, 1 );
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
