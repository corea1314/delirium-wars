#ifndef _ENTITY_H
#define _ENTITY_H

#include "../Math/Vector2.h"
#include "Box2.h"

class CEntity
{
private:
	Vector2	m_Position;
	Box2	m_Extent;

public:
	const Box2& GetExtent() { return m_Extent; }
	const Vector2& GetPosition() { return m_Position; }

	void OnDamage( CEntity* in_pInflictor, float in_fDamage );
};


#endif//_ENTITY_H
