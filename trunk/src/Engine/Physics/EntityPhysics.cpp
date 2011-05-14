
#include "EntityPhysics.h"

#include "Engine/Engine.h"
#include "Engine/Physics/World.h"

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

void CEntityPhysics::CreateBody( const std::string& in_szBodyDefinitionFilename, float x, float y, bool m_bActive )
{
	m_pBody = GetEngine()->GetWorld()->CreateBody( this, in_szBodyDefinitionFilename, b2Vec2(x,y), m_bActive );
}

void CEntityPhysics::DestroyBody()
{
	GetEngine()->GetWorld()->DestroyBody( &m_pBody );
}
