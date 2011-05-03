#pragma once

#include "Engine/Physics/EntityPhysics.h"

class CBodyBuilder;

class CPlayer : public CEntityPhysics
{
public:
	static void Cache();
	static void Uncache();

	static	CBodyBuilder* ms_pBodyBuilder;
};