#ifndef _APP_H
#define _APP_H

#define MAX_GRID_SIZE		512

class CEngine;

class Grid
{
public:
	Grid();

	void IncreaseGridSize();
	void DecreaseGridSize();
	void UpdateGrid();

	void Render();

private:
	int grid_size;
	Vertex vb_grid[(MAX_GRID_SIZE+1)*(MAX_GRID_SIZE+1)];
	int vb_grid_size;
};

class App
{
public:
	App();
	
	void Init();
	void Exit();
	void Update( float dt );
	void Render();

	void OnWheelUp();
	void OnWheelDown();
	
	void OnLeftClick( int x, int y, const Vector2& v );
	void OnRightClick( int x, int y, const Vector2& v );
	void OnMiddleClick( int x, int y, const Vector2& v );

	void OnKeyboard( unsigned char key );
		
	// todo: better handle mouse position and button states from framework
	int last_mx;
	int last_my;
	int mx;
	int my;
	unsigned char Buttons[5];

	// todo: better handle window change event variables from framework
	int w;
	int h;
		
	CEngine* GetEngine() const { return m_pEngine; }

private:
	Grid	m_Grid;	//todo: remove at some point

	CEngine* m_pEngine;
	
};

extern App g_App;



#endif//_APP_H
