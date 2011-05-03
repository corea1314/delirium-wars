#include "Player.h"

#include "Engine/Physics/Builder.h"

CBodyBuilder* CPlayer::ms_pBodyBuilder = 0;

void CPlayer::Cache()
{
	ms_pBodyBuilder = new CBodyBuilder("player.body.xml");
}

void CPlayer::Uncache()
{
	delete ms_pBodyBuilder;
	ms_pBodyBuilder = 0;
}