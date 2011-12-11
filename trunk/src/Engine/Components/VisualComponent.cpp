
#include "Lair/Lair.h"
#include "Engine/Engine.h"
#include "Engine/Entity.h"

#include "VisualComponent.h"

void VisualComponent::Play( const char* in_szSequenceName, bool in_bLoop )
//void VisualComponent::Play( const std::string& in_szSequenceName, bool in_bLoop )
{
	m_pSprite->Play( in_szSequenceName, in_bLoop );
}

void VisualComponent::Update( float in_fDeltaTime )
{
	m_pSprite->Update(in_fDeltaTime);
}

bool VisualComponent::IsPlaying() const
{
	return m_pSprite->IsPlaying();
}

void VisualComponent::Set( float x, float y, float a, float sx, float sy )
{
	m_pSprite->Set(x,y,a,sx,sy);
}

void VisualComponent::SetAlpha( float a )
{
	m_pSprite->SetAlpha(a);
}

void VisualComponent::SetColor( float r, float g, float b )
{
	m_pSprite->SetColor(r,g,b);
}

void VisualComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	m_pSprite = Lair::GetSpriteMan()->GetSprite();

	GetEntity()->GetLuaContext().registerFunction("play", &VisualComponent::Play);
	GetEntity()->GetLuaContext().registerFunction("is_playing", &VisualComponent::IsPlaying);
	GetEntity()->GetLuaContext().registerFunction("set_alpha", &VisualComponent::SetAlpha);
	GetEntity()->GetLuaContext().registerFunction("set_color", &VisualComponent::SetColor);
}
