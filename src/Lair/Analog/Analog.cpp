#include "Analog.h"

#include <tinyxml/tinyxml.h>

void AnalogInstance::ParseProperties( TiXmlElement* in_pxmlInstance )
{	
	std::string szName, szValue;

	for( TiXmlElement* pElement = in_pxmlInstance->FirstChildElement("property"); pElement != NULL; pElement = pElement->NextSiblingElement("property") )
	{
		pElement->QueryStringAttribute( "name", &szName );
		pElement->QueryStringAttribute( "value", &szValue );
		m_mapProperties.insert( std::make_pair(szName,szValue) );
	}
}

void AnalogInstancePoint::Load( TiXmlElement* in_pxmlInstance )
{
	in_pxmlInstance->QueryFloatAttribute( "x", &x );
	in_pxmlInstance->QueryFloatAttribute( "y", &y );
	in_pxmlInstance->QueryFloatAttribute( "z", &z );

	ParseProperties(in_pxmlInstance);
}

void AnalogInstanceRect::Load( TiXmlElement* in_pxmlInstance )
{
	in_pxmlInstance->QueryFloatAttribute( "x", &x );
	in_pxmlInstance->QueryFloatAttribute( "y", &y );
	in_pxmlInstance->QueryFloatAttribute( "w", &w );
	in_pxmlInstance->QueryFloatAttribute( "h", &h );

	ParseProperties(in_pxmlInstance);
}

void AnalogInstancePoly::Load( TiXmlElement* in_pxmlInstance )
{
	in_pxmlInstance->QueryFloatAttribute( "z", &z );

	Point p;
	for( TiXmlElement* pElement = in_pxmlInstance->FirstChildElement("point"); pElement != NULL; pElement = pElement->NextSiblingElement("point") )
	{
		pElement->QueryFloatAttribute( "x", &p.x );
		pElement->QueryFloatAttribute( "y", &p.y );
		m_vecPoint.push_back( p );
	}
}

void AnalogMap::Load( std::string in_szName )
{

}

