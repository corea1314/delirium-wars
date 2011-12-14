#include "Player.h"

#include "Engine/Engine.h"
#include "Engine/Physics/World.h"

void CPlayer::Connect( CEngine* in_pEngine )
{
	CEntityPhysics::Connect( in_pEngine );

	CreateBody( "Player.body.xml", 0, 0, true );
}

void CPlayer::Disconnect( CEngine* in_pEngine )
{
	DestroyBody();

	CEntityPhysics::Disconnect( in_pEngine );
}