#ifndef _MENU_H
#define _MENU_H

class EMenuEntries
{
public:
	enum E
	{
		// File
		eFile_Load,
		eFile_Save,

		// Grid
		eGrid_Increase,
		eGrid_Decrease,
		eGrid_Snap,

		// Zoom
		eZoom_In,
		eZoom_Out,
		eZoom_Reset,

		// Animate
		eAnimate,

		// Help
		eHelp,

		// Quit
		eQuit
	};
};

void menu_Create();
void menu_Destroy();

void menu_Process( int item );


#endif//_MENU_H