#pragma once

#include "../Editor.h"
#include "../Menu.h"

#include <list>

class LayoutElement;

class LayoutEditor : public Editor
{
private:
	
protected:
	// Editor callbacks
	virtual void OnInit();
	virtual void OnExit();
	virtual void OnRender();
	virtual void OnUpdate( float inDeltaTime );
	virtual void OnRenderGUI();
	virtual void OnMouseClick( int button, int x, int y, int mod );
	virtual void OnMouseMotion( int x, int y, int dx, int dy, int mod );
	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual void OnCreateMenu();
	
	// Menu callbacks
	void OnMenuFileSave( int inUnused );
	void OnMenuFileLoad( int inUnused );
	
private:
	// Local methods
	
private:
	std::list<LayoutElement*>	mElements;
};