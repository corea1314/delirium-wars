#ifndef _APP_H
#define _APP_H

#define MAX_ZOOM_LEVEL		64
#define MAX_GRID_SIZE		512

class CEngine;

class App
{
public:
	App();
	
	void Init();
	void Exit();
	void Update( float dt );
	void Render();
	
// todo: remove those
	void ZoomIn();
	void ZoomOut();
	void ZoomReset();
	
	void IncreaseGridSize();
	void DecreaseGridSize();
	
	int last_mx;
	int last_my;
	int mx;
	int my;
	int w;
	int h;
	unsigned char Buttons[5];	//

	float cam_pos_x;
	float cam_pos_y;
	float cam_zoom;

	bool snap;
	int grid_size;

	bool animate;
	float timeSeconds;

private:
	void UpdateZoom();
	int zoom;

	void UpdateGrid();
	
	Vertex vb_grid[(MAX_GRID_SIZE+1)*(MAX_GRID_SIZE+1)];
	int vb_grid_size;

	CEngine* m_pEngine;
	
};

extern App g_App;



#endif//_APP_H
