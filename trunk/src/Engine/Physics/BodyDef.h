#ifndef _BODYDEF_H
#define _BODYDEF_H

#include <map>
#include <vector>
#include <string>

#include <Box2D/Box2D.h>

class TiXmlElement;

class BodyDefinition
{
public:
	BodyDefinition();
	bool Load( const std::string& in_szFilename );

	b2BodyDef& GetBody() { return m_BodyDef; }
	std::vector< b2FixtureDef >& GetFixtures() { return m_FixtureDef; }

private:
	void ProcessBody( TiXmlElement* in_pxmlBody );
	void ProcessShapeRect( TiXmlElement* in_pxmlShape );
	void ProcessShapePoly( TiXmlElement* in_pxmlShape );
	void GetShapeProperties( TiXmlElement* in_pxmlShape, b2FixtureDef& out_FixtureDef );

	b2BodyDef m_BodyDef;
	std::vector< b2FixtureDef >	m_FixtureDef;

	bool m_bValid;
};

class BodyDefinitionMan
{
public:
	BodyDefinition* Get( const std::string& in_szFilename );
private:
	std::map< std::string, BodyDefinition* > m_mapBodyDef;
};

#endif//_BODYDEF_H
