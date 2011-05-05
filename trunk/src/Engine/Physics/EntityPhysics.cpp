
#include "EntityPhysics.h"

IMPLEMENT_CLASS_TYPE(CEntityPhysics)

CEntityPhysics::CEntityPhysics() : m_pBody(0)
{

}

CEntityPhysics::~CEntityPhysics()
{
}

void CEntityPhysics::Connect( CEngine* in_pEngine )
{
	CEntity::Connect( in_pEngine );
}

void CEntityPhysics::Disconnect( CEngine* in_pEngine )
{
	CEntity::Disconnect( in_pEngine );
}