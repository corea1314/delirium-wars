#pragma once

class InputMan
{
private:
	typedef struct
	{
		bool	bState;
		float	fTimeStamp;	// time when it changed status
	
	} MouseButtonState;

public:
	InputMan()
	{
		mMouseButtonState[0].bState = false;
		mMouseButtonState[1].bState = false;
		mMouseButtonState[2].bState = false;
	}

	void UpdateMouseButtonState( int inButtonIndex, bool inState )
	{
		if( mMouseButtonState[inButtonIndex].bState = inState )
			mMouseButtonState[inButtonIndex].fTimeStamp = 0.0f; //todo

		mMouseButtonState[inButtonIndex].bState = inState;
	}

	const MouseButtonState& GetMouseButtonState( int inButtonIndex ) { return mMouseButtonState[inButtonIndex]; }

private:
	MouseButtonState	mMouseButtonState[3];	// support only 3 buttons
};

typedef struct  
{
	int x,y;		// mouse position in screen space
	int dx,dy;		// delta since last frame
	Vector2 pos;	// mouse position in app space
	Vector2 delta;	// delta since last frame
	int mod;		// modifier key mask (shift,alt,ctrl)

} MouseMotion;

#define SK_F1			0x0001	//GLUT_KEY_F1
#define SK_F2 			0x0002	//GLUT_KEY_F2
#define SK_F3 			0x0003	//GLUT_KEY_F3
#define SK_F4 			0x0004	//GLUT_KEY_F4
#define SK_F5 			0x0005	//GLUT_KEY_F5
#define SK_F6 			0x0006	//GLUT_KEY_F6
#define SK_F7 			0x0007	//GLUT_KEY_F7
#define SK_F8 			0x0008	//GLUT_KEY_F8
#define SK_F9 			0x0009	//GLUT_KEY_F9
#define SK_F10 			0x000A	//GLUT_KEY_F10
#define SK_F11 			0x000B	//GLUT_KEY_F11
#define SK_F12 			0x000C	//GLUT_KEY_F12

#define SK_LEFT			0x0064	//GLUT_KEY_LEFT
#define SK_DOWN			0x0067	//GLUT_KEY_DOWN
#define SK_RIGHT		0x0066	//GLUT_KEY_RIGHT
#define SK_UP			0x0065	//GLUT_KEY_UP

#define SK_PAGE_UP		0x0068	//GLUT_KEY_PAGE_UP
#define SK_PAGE_DOWN	0x0069	//GLUT_KEY_PAGE_DOWN
#define SK_HOME			0x006A	//GLUT_KEY_HOME
#define SK_END			0x006B	//GLUT_KEY_END
#define SK_INSERT		0x006C	//GLUT_KEY_INSERT

#define SK_MOD_SHIFT	0x0001	//GLUT_ACTIVE_SHIFT
#define SK_MOD_CTRL		0x0002	//GLUT_ACTIVE_CTRL
#define SK_MOD_ALT		0x0004	//GLUT_ACTIVE_ALT 
