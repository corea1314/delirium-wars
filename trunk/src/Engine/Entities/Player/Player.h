#pragma once

#include "Engine/Physics/EntityPhysics.h"

class CPlayer : public CEntityPhysics
{
	DECLARE_CLASS_TYPE(CPlayer, CEntityPhysics, CEntity);

public:
	virtual void OnContactAdd() {}
	virtual void OnContactPersist() {}
	virtual void OnContactRemove() {}
};