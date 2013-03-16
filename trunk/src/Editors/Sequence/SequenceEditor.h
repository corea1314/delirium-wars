#pragma once

#include "../Editor.h"
#include "../Menu.h"

class Sprite;
class SpriteMan;


class SequenceEditor : public Editor
{
protected:

	// Editor callbacks
	virtual void OnInit();
	virtual void OnExit();
	virtual void OnUpdate( float inDeltaTime );
	virtual void OnRender();
	virtual void OnRenderGUI();
	virtual void OnMouseClick( int button, int state, const MouseMotion& mm );
	virtual void OnMouseMotion( const MouseMotion& mm );
	virtual void OnSpecialKey( int key, int mod, bool down );
	virtual void OnCreateMenu();

	// Menu callbacks
	void OnMenuAnimate( int inUnused );
	void OnMenuShowFrame( int inUnused );
	void OnMenuShowAtlas( int inUnused );
	void OnMenuAddFrame( int inUnused );

	// File management
	virtual const char*	GetFileExtension() { return "xseq"; }
	virtual const char*	GetFileFilter() { return "XML Sequence File\0*.xseq\0"; }
	virtual void OnSerializeSave( TiXmlElement* inNode );
	virtual void OnSerializeLoad( TiXmlElement* inNode );

private:
	// Local methods
	void SetCurrFrame( int inFrame );

private:
	Sprite*		mSprite;

	AtlasFrameSequence	mSequence;

	bool mAnimate;
	bool mShowFrame;
	bool mShowAtlas;

	int		mFPS;
	int		mCurrFrame;
	float	mCurrTime;
};