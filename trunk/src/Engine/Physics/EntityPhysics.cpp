
#include "EntityPhysics.h"

CEntityPhysics::CEntityPhysics() : m_pBody(0)
{

}

CEntityPhysics::~CEntityPhysics()
{
}

void CEntityPhysics::Connect( CEngine* in_pEngine )
{
	m_pEngine = in_pEngine;
}

void CEntityPhysics::Disconnect( CEngine* in_pEngine )
{
	m_pEngine = 0;
}