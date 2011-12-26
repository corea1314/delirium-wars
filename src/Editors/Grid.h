#pragma once

#include "gfx.h"

#define MAX_GRID_SIZE		512

class Grid
{
public:
	Grid();

	void IncreaseGridSize();
	void DecreaseGridSize();
	void UpdateGrid();

	void ToggleSnap();

	void Render();

	int GetGridSize() { return grid_size; }
	bool GetSnap() { return snap; }

private:
	int grid_size;
	Vertex vb_grid[(MAX_GRID_SIZE+1)*(MAX_GRID_SIZE+1)];
	int vb_grid_size;
	bool snap;
};
