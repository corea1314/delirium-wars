#pragma once

#include <map>
#include <vector>

class Editor;

class Menu
{
public:

	class Delegate
	{
	public:
		typedef void (Editor::*Function)( int );

		Delegate( Editor* inInstance, Function inFunction, int inArg0 ) : mpInstance(inInstance), mpFunction(inFunction), mArg0(inArg0) {}

		void operator()() 
		{ 
			(mpInstance->*mpFunction)( mArg0 ); 
		}

	private:
		int			mArg0;
		Editor*		mpInstance;
		Function	mpFunction;
	};


	Menu();
	virtual ~Menu();

	Menu* AddMenu( const char* inName );
	void AddItem( const char* inName, const Menu::Delegate& inItem );
	int GetHandle() { return mHandle; }

	void BindButton( int inButton );
	void UnbindButton( int inButton );

	static void ProcessMenu( int inItem );

private:
	int mHandle;
	std::vector<Menu*>	mVecSubMenu;

private:
	static int msItemCounter;
	static Menu* msActiveMenu;
	static std::map< int, Menu::Delegate >	msMapDelegate;
};

#define CREATE_MENU( menu, name )	\
	Menu*	menu = GetMenu()->AddMenu( name );

#define ADD_MENU_ITEM( menu, name, func, arg )	\
	menu->AddItem( name, Menu::Delegate( this, static_cast <void (Editor::*)(int)> (func), arg ) );

