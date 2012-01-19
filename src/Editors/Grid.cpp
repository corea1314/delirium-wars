#include "Grid.h"

#include <algorithm>

#define GRID_SIZE	1024.0f

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

Vector2 Grid::Snap( const Vector2& inPos )
{
	if( mIsSnapping )
	{
		Vector2 vNewPos;
		vNewPos.x = (float)((int)((inPos.x + NUM_SIGN(inPos.x) * mGridDelta/2) / mGridDelta) * mGridDelta);
		vNewPos.y = (float)((int)((inPos.y + NUM_SIGN(inPos.y) * mGridDelta/2) / mGridDelta) * mGridDelta);
		return vNewPos;
	}
	return inPos;	
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

	float d = GRID_SIZE / mSize;

	int i,j;
	for( i=0; i<c; i++ )
	{
		for( j=0; j<c; j++ )
		{
			p->x = i * d - GRID_SIZE/2;
			p->y = j * d - GRID_SIZE/2;
			p->z = 0;
			p->color = COLORS::eYELLOW;
			++p;
		}
	}

	mVertexBufferSize = c * c;

	mGridDelta = (GRID_SIZE / mSize );
}

void Grid::Render()
{
	glPushMatrix();
		glScalef( mScale, mScale, 1.0f );

		gl_SetColor( COLORS::eDARKGREY );
		glBegin( GL_LINES );
		glVertex2f( 0, -MAX_GRID_SIZE*2 );
		glVertex2f( 0,  MAX_GRID_SIZE*2 );
		glVertex2f( -MAX_GRID_SIZE*2, 0 );
		glVertex2f(  MAX_GRID_SIZE*2, 0 );
		glEnd();

		gl_RenderPoints( mVertexBuffer, 0, mVertexBufferSize, 2 );
				
	glPopMatrix();
}