#ifndef _BUILDER_H
#define _BUILDER_H

#include <vector>
#include <string>
#include <Box2D/Box2D.h>
#include <tinyxml/tinyxml.h>

class CBodyBuilder
{
public:
	CBodyBuilder( const std::string& in_szFilename );
	b2Body* Create( b2World* in_pWorld, const b2Vec2& in_vPos, bool m_bActive = false );

private:
	void ProcessBody( TiXmlElement* in_pxmlBody );
	void ProcessShapeRect( TiXmlElement* in_pxmlShape );
	void ProcessShapePoly( TiXmlElement* in_pxmlShape );
	void GetShapeProperties( TiXmlElement* in_pxmlShape, b2FixtureDef& out_FixtureDef );

	b2BodyDef m_BodyDef;
	std::vector< b2FixtureDef >	m_FixtureDef;

	bool m_bValid;
};

#endif//_BUILDER_H

