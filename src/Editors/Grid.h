#pragma once

#include "gfx.h"
#include "Math/Vector2.h"

#define MAX_GRID_SIZE		512

class Grid
{
public:
	Grid();

	void IncreaseGridSize();
	void DecreaseGridSize();
	void UpdateGrid();

	void ToggleSnap();
	Vector2 Snap( const Vector2& inPos );

	void Render();

	int GetGridSize() { return mSize; }
	bool GetSnap() { return mIsSnapping; }

	void SetScale( float inScale ) { mScale = inScale; }

private:
	int		mSize;
	Vertex	mVertexBuffer[(MAX_GRID_SIZE+1)*(MAX_GRID_SIZE+1)];
	int		mVertexBufferSize;
	bool	mIsSnapping;
	float	mScale;
	float	mHalfGridDelta;
};
