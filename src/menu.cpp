#include "fw.h"
#include "menu.h"
#include "app.h"

typedef struct 
{
	int hFileMenu;

} SMenuEntry;

SMenuEntry g_MenuEntry;

void menu_Create()
{
	g_MenuEntry.hFileMenu = glutCreateMenu( menu_Process );
	glutAddMenuEntry( "  Save  ",		EMenuEntries::eFile_Save );
	glutAddMenuEntry( "  Load  ",		EMenuEntries::eFile_Load );

	glutCreateMenu( menu_Process );
	glutAddSubMenu(   "  File... ",		g_MenuEntry.hFileMenu );
	glutAddMenuEntry( "  Help    ",		EMenuEntries::eHelp );
	glutAddMenuEntry( "  Quit    ",		EMenuEntries::eQuit );
	
	glutAttachMenu( GLUT_RIGHT_BUTTON );
}

void menu_Destroy()
{

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool lock = false;

static const char* OPENFILE_EXTENSION = "dwl";
static const char* OPENFILE_FILTER = "Delirium War Level\0*.dwl\0";

bool getSaveFilename( char filename[], int count )
{
	::OPENFILENAME openFilename = {0};
	
	openFilename.lStructSize     = sizeof(::OPENFILENAME);
	openFilename.hwndOwner       = 0;
	openFilename.lpstrFile       = filename;
	openFilename.nMaxFile        = count;
	openFilename.lpstrFilter     = OPENFILE_FILTER;
	openFilename.nFilterIndex    = 1;
	openFilename.lpstrFileTitle  = 0;
	openFilename.nMaxFileTitle   = 0;
	openFilename.lpstrInitialDir = 0;
	openFilename.lpstrDefExt     = OPENFILE_EXTENSION;
	openFilename.Flags           = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN;
	
	lock = true;
	bool result = ::GetSaveFileName(&openFilename) != 0;
	lock = false;
	return result;
}

void menu_FileSave()
{
	static int next_save = 0;

	static char filename[MAX_PATH] = "";

	if( !lock && getSaveFilename(filename,sizeof(filename)) )
	{
	}
}

bool getLoadFilename( char filename[], int count )
{
	::OPENFILENAME openFilename = {0};
	
	openFilename.lStructSize     = sizeof(::OPENFILENAME);
	openFilename.hwndOwner       = 0;//g_hWnd;
	openFilename.lpstrFile       = filename;
	openFilename.nMaxFile        = count;
	openFilename.lpstrFilter     = OPENFILE_FILTER;
	openFilename.nFilterIndex    = 1;
	openFilename.lpstrFileTitle  = 0;
	openFilename.nMaxFileTitle   = 0;
	openFilename.lpstrInitialDir = 0;
	openFilename.lpstrDefExt     = OPENFILE_EXTENSION;
	openFilename.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	
	lock = true;
	bool result = ::GetOpenFileName(&openFilename) != 0;
	lock = false;
	return result;
}

void menu_FileLoad()
{
	static char filename[MAX_PATH] = "";

	if( !lock && getLoadFilename(filename,sizeof(filename)) )
	{
		g_App.OnOpenFile( filename );
	}
}

void menu_Help()
{
	puts( "Shortcuts" );
	puts( "---------" );
	puts( "Delete:              Deletes a control point" );
	puts( "+/-:                 Increases / Decreases grid size" );
	puts( "Arrows:              Scrolls view" );
	puts( " " );
	puts( "Mouse" );
	puts( "-----" );
	puts( "Right Button:        Popup context menu" );
	puts( "Left Button:         Selects a control point" );
	puts( "Middle Button:       Adds a control point" );
}

void menu_Quit()
{
	exit(0);
}


void menu_Process( int item )
{
	switch( item )
	{
	// File
	case EMenuEntries::eFile_Load:		menu_FileLoad();	break;
	case EMenuEntries::eFile_Save:		menu_FileSave();	break;

	// Help
	case EMenuEntries::eHelp:			menu_Help();		break;

	// Quit
	case EMenuEntries::eQuit:			menu_Quit();		break;
	}
}
