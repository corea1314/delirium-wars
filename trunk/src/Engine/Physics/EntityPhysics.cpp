
#include "EntityPhysics.h"

#include "Engine/Engine.h"
#include "Engine/Physics/World.h"

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
	assert(m_pEngine==in_pEngine); 
	m_pEngine = 0; 
}

void CEntityPhysics::CreateBody( const std::string& in_szBodyDefinitionFilename, float x, float y, bool m_bActive )
{
	m_pBody = GetEngine()->GetWorld()->CreateBody( this, in_szBodyDefinitionFilename, b2Vec2(x,y), m_bActive );
}

void CEntityPhysics::DestroyBody()
{
	if( m_pBody )
		GetEngine()->GetWorld()->DestroyBody( &m_pBody );
}
