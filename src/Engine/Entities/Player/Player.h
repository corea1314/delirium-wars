#pragma once

#include "Engine/Physics/EntityPhysics.h"

class CPlayer : public CEntityPhysics
{
public:
	virtual void Connect( CEngine* );		// connects object to game engine
	virtual void Disconnect( CEngine* );	// disconnects object from game engine

public:
	virtual void OnContactAdd() {}
	virtual void OnContactPersist() {}
	virtual void OnContactRemove() {}
};