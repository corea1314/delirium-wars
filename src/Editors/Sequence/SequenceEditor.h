#pragma once

#include "../Editor.h"
#include "../Menu.h"

class Sprite;
class SpriteMan;
class AtlasFrame;

class SequenceEditor : public Editor
{
protected:
	class Frame
	{
	public:
		Frame( const std::string& inFilename, AtlasFrame* inAtlasFrame ) : mFilename(inFilename), mAtlasFrame(inAtlasFrame) {}

		AtlasFrame*	mAtlasFrame;
		std::string mFilename;
	};


	// Editor callbacks
	virtual void OnInit();
	virtual void OnExit();
	virtual void OnUpdate( float inDeltaTime );
	virtual void OnRender();
	virtual void OnRenderGUI();
	virtual void OnMouseClick( int button, int state, int x, int y, int mod );
	virtual void OnMouseMotion( int x, int y, int dx, int dy, int mod );
	virtual void OnSpecialKey( int key, int mod );
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
	SpriteMan*	mSpriteMan;

	std::vector<Frame>	mFrames;

	bool mAnimate;
	bool mShowFrame;
	bool mShowAtlas;

	int		mFPS;
	int		mCurrFrame;
	float	mCurrTime;
};