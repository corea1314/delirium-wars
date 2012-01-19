
#include "Lair/Lair.h"
#include "Engine/Engine.h"
#include "Engine/Entity.h"

#include "VisualComponent.h"

// component interface ////////////////////////////////////////////////////////

void VisualComponent::Play( const char* in_szSequenceName, bool in_bLoop )
{
	m_pSprite->Play( in_szSequenceName, in_bLoop );
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

// engine connection //////////////////////////////////////////////////////////

void VisualComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	in_pEngine->Connect_OnUpdate( this, &VisualComponent::OnUpdate );

	m_pSprite = Lair::GetSpriteMan()->GetSprite();

	GetEntity()->GetLuaContext().registerFunction("play", &VisualComponent::Play);
	GetEntity()->GetLuaContext().registerFunction("is_playing", &VisualComponent::IsPlaying);
	GetEntity()->GetLuaContext().registerFunction("set_alpha", &VisualComponent::SetAlpha);
	GetEntity()->GetLuaContext().registerFunction("set_color", &VisualComponent::SetColor);
}

void VisualComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
	in_pEngine->Disconnect_OnUpdate( this );

	if( m_pSprite )
	{
		Lair::GetSpriteMan()->FreeSprite( m_pSprite );
		m_pSprite = 0;
	}

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void VisualComponent::OnUpdate( float in_fDeltaTime )
{
	// TODO add mechanism to check if we linked 'set' methods to sync with entity
	m_pSprite->Set( GetEntity()->GetPos().x, GetEntity()->GetPos().y, GetEntity()->GetAngle(), 1.0f, 1.0f );
	m_pSprite->Update(in_fDeltaTime);
}
