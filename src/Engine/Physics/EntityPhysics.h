#ifndef _ENTITY_PHYSICS_H
#define _ENTITY_PHYSICS_H

#include "Engine/Entity.h"
#include <string>

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

protected:
	void CreateBody(const std::string& in_szBodyDefinitionFilename, float x, float y, bool m_bActive);
	void DestroyBody();
		
private:
	b2Body*		m_pBody;
};

#endif//_ENTITY_PHYSICS_H

