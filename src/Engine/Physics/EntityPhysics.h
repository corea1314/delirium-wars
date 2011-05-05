#ifndef _ENTITY_PHYSICS_H
#define _ENTITY_PHYSICS_H

#include "Engine/Entity.h"

class b2Body;

class CEntityPhysics : public CEntity
{
	DECLARE_CLASS_TYPE(CEntityPhysics, CEntity, CEntity);

public:
	CEntityPhysics();
	virtual ~CEntityPhysics();

	virtual void Connect( CEngine* );		// connects object to game engine
	virtual void Disconnect( CEngine* );	// disconnects object from game engine

	b2Body* GetBody() { return m_pBody; }

	virtual void OnContactAdd() {}
	virtual void OnContactPersist() {}
	virtual void OnContactRemove() {}
		
private:
	b2Body*		m_pBody;
	CEngine*	m_pEngine;
};

// those macros will only work in connect and disconnect
#define EntityCreateBody( pos, active )	\
	m_pBody = ms_pBodyBuilder->Create( m_pEngine->GetWorld(), pos, active );	// create body

#define EntityDestroyBody()	\
	m_pEngine->GetWorld()->DestroyBody(m_pBody);	\
	m_pBody = 0;


#endif//_ENTITY_PHYSICS_H

