#pragma once

class Sprite;

class VisualComponent : public CEntity::Component
{
public:
	VisualComponent() {}
	virtual ~VisualComponent() {}

	// exposed methods
	void Play( const char* in_szSequenceName, bool in_bLoop=false );
		
	bool IsPlaying() const;

	void Set( float x, float y, float a = 0.0f, float sx = 1.0f, float sy = 1.0f );
	void SetAlpha( float a );
	void SetColor( float r, float g, float b );
	
	// engine callbacks
	void OnUpdate( float in_fDeltaTime );

	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

private:
	Sprite*		m_pSprite;

	// callbacks
	LuaCallbackInfo		m_cbOnEndOfSequence;	//todo: implement this
};