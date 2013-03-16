
#include "Player.h"

#include "Engine/Engine.h"
#include "Engine/Physics/World.h"

void CPlayer::Connect( CEngine* in_pEngine )
{
	mVisualComponent = CreateComponent<VisualComponent>();

	CEntity::Connect( in_pEngine );

	in_pEngine->Connect_OnUpdate( this, &CPlayer::OnUpdate );
}

void CPlayer::Disconnect( CEngine* in_pEngine )
{
	in_pEngine->Disconnect_OnUpdate( this );

	CEntity::Disconnect( in_pEngine );
}


void CPlayer::OnUpdate( float in_fDeltaTime ) 
{

}