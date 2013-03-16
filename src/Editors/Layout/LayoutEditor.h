#pragma once

#include "../Editor.h"
#include "../Menu.h"

class WidgetRect;

class Layout
{
	class Point
	{

	};

	class Rect
	{

	};

	class Path
	{
		std::vector<Point> mPoints;
	};

	std::vector<Rect>	mRects;
	std::vector<Point>	mPoints;
	std::vector<Path>	mPaths;
};

class LayoutEditor : public Editor
{
private:
	class Mode
	{
	public:
		enum E { Selection, Point, Rect };
	};
	
protected:
	// Editor callbacks
	virtual void OnInit();
	virtual void OnExit();
	virtual void OnRender();
	virtual void OnUpdate( float inDeltaTime );
	virtual void OnRenderGUI();
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnKeyboard( unsigned char key, int mod, bool down );
//	virtual void OnSpecialKey( int key, int mod, bool down );
	virtual void OnCreateMenu();
	
	// Menu callbacks
	void OnMenuMode( int inMode );
	void OnSerializeSave( TiXmlElement* inNode );
	void OnSerializeLoad( TiXmlElement* inNode );

	// File management
	virtual const char*	GetFileExtension() { return "xlay"; }
	virtual const char*	GetFileFilter() { return "XML Layout File\0*.xlay\0"; }
	
private:
	// Local methods
	void SetMode( Mode::E inMode );
	void CleanupDeleted();
	
private:
	Mode::E						mMode;
	WidgetRect*					mWidgetRect;
	Layout*						mLayout;
};