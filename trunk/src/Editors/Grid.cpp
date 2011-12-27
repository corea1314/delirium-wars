#include "Grid.h"

#include <algorithm>

Grid::Grid()
{
	mScale = 1.0f;
	mIsSnapping = false;
	mSize = 16;
	UpdateGrid();
}

void Grid::ToggleSnap()
{
	mIsSnapping = !mIsSnapping;
}

void Grid::IncreaseGridSize()
{
	mSize *= 2;
	mSize = std::min( mSize, MAX_GRID_SIZE );
	UpdateGrid();
}

void Grid::DecreaseGridSize()
{
	mSize /= 2;
	mSize = std::max( mSize, 1 );
	UpdateGrid();
}

void Grid::UpdateGrid()
{
	Vertex* p = mVertexBuffer;

	int c = mSize + 1;

	float d = 1000.0f / mSize;

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

	mVertexBufferSize = c * c;
}

void Grid::Render()
{
	glPushMatrix();
	glScalef( mScale, mScale, 1.0f );
	gl_RenderPoints( mVertexBuffer, 0, mVertexBufferSize, 2 );
	glPopMatrix();
}
