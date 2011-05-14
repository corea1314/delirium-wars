#include "Player.h"

#include "Engine/Engine.h"
#include "Engine/Physics/World.h"

IMPLEMENT_CLASS_TYPE(CPlayer)


void CPlayer::Connect( CEngine* in_pEngine )
{
	CEntity::Connect( in_pEngine );

	CreateBody( "Player.body.xml", 0, 0, true );
}

void CPlayer::Disconnect( CEngine* in_pEngine )
{
	DestroyBody();

	CEntity::Disconnect( in_pEngine );
}