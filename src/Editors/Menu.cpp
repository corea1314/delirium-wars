
#include "Menu.h"

#include "gfx.h"

void __MenuProcess__( int item )
{
	Menu::ProcessMenu( item );
}

void Menu::ProcessMenu( int inItem )
{
	std::map< int, Menu::Delegate >::iterator it = msMapDelegate.find(inItem);

	if( it != msMapDelegate.end() )
	{
		it->second(); // call delegate
	}
}

int Menu::msItemCounter = 0;
Menu* Menu::msActiveMenu = 0;
std::map< int, Menu::Delegate >	Menu::msMapDelegate;


///////////////////////////////////////////////////////////////////////////////
// Menu
//

Menu::Menu()
{
	mHandle = glutCreateMenu( __MenuProcess__ );
}

Menu::~Menu()
{
	std::vector<Menu*>::iterator it;

	for( it = mVecSubMenu.begin(); it != mVecSubMenu.end(); it++ )
	{
		delete (*it);
	}
}

Menu* Menu::AddMenu( const char* inName )
{
	Menu* pNewMenu = new Menu();
	mVecSubMenu.push_back(pNewMenu);

	glutSetMenu( GetHandle() );
	glutAddSubMenu( inName, pNewMenu->GetHandle() );
	return pNewMenu;
}

void Menu::AddItem( const char* inName, const Menu::Delegate& inItem )
{
	glutSetMenu( GetHandle() );
	glutAddMenuEntry( inName, msItemCounter );

	msMapDelegate.insert( std::make_pair( msItemCounter, inItem ) );

	++msItemCounter;
}

void Menu::BindButton( int inButton )
{
	glutSetMenu( GetHandle() );
	switch( inButton )
	{
	case 0:	glutAttachMenu(GLUT_LEFT_BUTTON);	break;
	case 1:	glutAttachMenu(GLUT_MIDDLE_BUTTON);	break;
	case 2: glutAttachMenu(GLUT_RIGHT_BUTTON);	break;
	}
}

void Menu::UnbindButton( int inButton )
{
	glutSetMenu( GetHandle() );
	switch( inButton )
	{
	case 0:	glutDetachMenu(GLUT_LEFT_BUTTON);	break;
	case 1:	glutDetachMenu(GLUT_MIDDLE_BUTTON);	break;
	case 2: glutDetachMenu(GLUT_RIGHT_BUTTON);	break;
	}
}