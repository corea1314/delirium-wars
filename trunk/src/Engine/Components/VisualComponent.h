#pragma once

class Sprite;

class VisualComponent : public CEntity::Component
{
public:
	VisualComponent() {}
	virtual ~VisualComponent() {}

	void Play( const char* in_szSequenceName, bool in_bLoop=false );
//	void Play( const std::string& in_szSequenceName, bool in_bLoop=false );
	void Update( float in_fDeltaTime );

	bool IsPlaying() const;

	void Set( float x, float y, float a = 0.0f, float sx = 1.0f, float sy = 1.0f );
	void SetAlpha( float a );
	void SetColor( float r, float g, float b );

	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity ) ;

private:
	Sprite*		m_pSprite;
};