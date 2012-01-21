#pragma once

#include "gfx.h"

#include <string>
#include "Math/Vector2.h"
#include "Lair/Input/Input.h"

class Editor;

class EditorElement
{
public:
	bool		mSelected;
	Editor*		mEditor;
	bool		mDeleteRequest;
	
//	int			mX, mY;		//screen space position
	Vector2		mPos;		//app space position
	float		mAngle;	
	float		mAlpha;
	std::string	mName;

	EditorElement( int inX, int inY, Vector2 inPos, Editor* inEditor ) 
		: mEditor(inEditor), mSelected(false), mDeleteRequest(false)/*, mX(inX), mY(inY)*/, mPos(inPos), mAngle(0.0f), mAlpha(1.0f), mName("element") {}

	EditorElement( Editor* inEditor ) 
		: mEditor(inEditor), mSelected(false), mDeleteRequest(false), mAngle(0.0f), mAlpha(1.0f), mName("element") {}

	virtual ~EditorElement() {}

public:
	virtual void OnUpdate( float inPosition );
	virtual bool OnSelect( const Vector2& inPos );
	virtual bool OnSelectRect( const Vector2& inMin, const Vector2& inMax );
	virtual void OnRender() = 0;
	virtual void OnRenderGUI() = 0;

	virtual void OnKeyboard( unsigned char key, int mod );
	virtual void OnSpecialKey( int key, int mod );
	virtual bool OnMouseMotion( const MouseMotion& mm );
	virtual bool OnMouseClick( int button, int state, const MouseMotion& mm );

	virtual void OnTranslate( const Vector2& inNewPos, const Vector2& inDelta );
	virtual void OnScale( const Vector2& inNewScale, const Vector2& inDelta );
	virtual void OnRotate( float inNewAngle, float inDelta );
	virtual void OnAlpha( float inNewAlpha, float inDelta );
};
