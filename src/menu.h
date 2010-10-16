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