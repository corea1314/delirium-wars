#pragma once

class Sprite;

class FaderComponent : public CEntity::Component
{
public:
	FaderComponent() {}
	virtual ~FaderComponent() {}

	// exposed methods
	void FadeIn( unsigned int in_nColor, float in_fDelay );
	void FadeOut( unsigned int in_nColor, float in_fDelay );
	bool IsFading() const;

	// engine callbacks
	void OnUpdate( float in_fDeltaTime );

	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

private:
	// callbacks
	LuaCallbackInfo		m_cbOnFadeOut;
	LuaCallbackInfo		m_cbOnFadeIn;
	LuaCallbackInfo		m_cbOnFadingIn;
	LuaCallbackInfo		m_cbOnFadingOut;
};