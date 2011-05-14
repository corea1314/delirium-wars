#include "BodyDef.h"

#include "Lair/Lair.h"

#include <tinyxml/tinyxml.h>

BodyDefinition::BodyDefinition()
{
}

bool BodyDefinition::Load( const std::string& in_szFilename )
{
	TiXmlDocument doc(in_szFilename);

	if( doc.LoadFile() )
	{
		m_FixtureDef.reserve(8);	//fixme

		// for( TiXmlElement* pElement = doc.FirstChildElement("body"); pElement != NULL; pElement = pElement->NextSiblingElement("body") )
		TiXmlElement* pElement = doc.FirstChildElement("body");
		{
			ProcessBody( pElement );
		}
		return true;
	}
	else
	{
		if( doc.Error() )
		{
			Lair::GetLogMan()->Log( "Box2d", "XML parse error in %s: %s (Line %d)", doc.Value(), doc.ErrorDesc(), doc.ErrorRow() );
		}
	}
	return false;
}

// ============================================================================

void BodyDefinition::ProcessBody( TiXmlElement* in_pxmlBody )
{
	std::string szName, szValue;

	for( TiXmlElement* pElement = in_pxmlBody->FirstChildElement("property"); pElement != NULL; pElement = pElement->NextSiblingElement("property") )
	{
		pElement->QueryStringAttribute( "name", &szName );
		pElement->QueryStringAttribute( "value", &szValue );

		if( szName == "fixedRotation" )
		{			
			if( szValue == "true" )
				m_BodyDef.fixedRotation = true;
			else if( szValue == "false" )
				m_BodyDef.fixedRotation = false;
		}
		else if( szName == "angularDamping" )
		{
			pElement->QueryFloatAttribute( "value", &m_BodyDef.angularDamping );
		}
		else if( szName == "allowSleep" )
		{
			if( szValue == "true" )
				m_BodyDef.allowSleep = true;
			else if( szValue == "false" )
				m_BodyDef.allowSleep = false;
		}
		else if( szName == "active" )
		{
			if( szValue == "true" )
				m_BodyDef.active = true;
			else if( szValue == "false" )
				m_BodyDef.active = false;
		}
		else if( szName == "awake" )
		{
			if( szValue == "true" )
				m_BodyDef.awake = true;
			else if( szValue == "false" )
				m_BodyDef.awake = false;
		}
		else if( szName == "type" )
		{
			if( szValue == "static" )
				m_BodyDef.type = b2_staticBody;
			else if( szValue == "dynamic" )
				m_BodyDef.type = b2_dynamicBody;
			else if( szValue == "kinematic" )
				m_BodyDef.type = b2_kinematicBody;
		}
		else if( szName == "linearDamping" )
		{
			pElement->QueryFloatAttribute( "value", &m_BodyDef.linearDamping );
		}
		else if( szName == "bullet" )
		{
			if( szValue == "true" )
				m_BodyDef.bullet = true;
			else if( szValue == "false" )
				m_BodyDef.bullet = false;
		}
	}

	for( TiXmlElement* pElement = in_pxmlBody->FirstChildElement("rect"); pElement != NULL; pElement = pElement->NextSiblingElement("rect") )
	{
		ProcessShapeRect( pElement );
	}

	for( TiXmlElement* pElement = in_pxmlBody->FirstChildElement("poly"); pElement != NULL; pElement = pElement->NextSiblingElement("poly") )
	{
		ProcessShapePoly( pElement );
	}
}

void BodyDefinition::ProcessShapeRect( TiXmlElement* in_pxmlShape )
{
	b2FixtureDef def;
	b2PolygonShape* shape = new b2PolygonShape;
	def.shape = shape;

	float x,y,w,h;

	in_pxmlShape->QueryFloatAttribute( "x", &x );
	in_pxmlShape->QueryFloatAttribute( "y", &y );
	in_pxmlShape->QueryFloatAttribute( "w", &w );
	in_pxmlShape->QueryFloatAttribute( "h", &h );

	b2Vec2 center(x+w/2,y+h/2);

	shape->SetAsBox( w/2, h/2, center, 0.0f );

	GetShapeProperties( in_pxmlShape, def );

	m_FixtureDef.push_back(def);
}

void BodyDefinition::ProcessShapePoly( TiXmlElement* in_pxmlShape )
{
	b2FixtureDef def;
	b2PolygonShape* shape = new b2PolygonShape;
	def.shape = shape;
	shape->m_vertexCount = 0;

	for( TiXmlElement* pElement = in_pxmlShape->FirstChildElement("point"); pElement != NULL; pElement = pElement->NextSiblingElement("point") )
	{
		pElement->QueryFloatAttribute( "x", &(shape->m_vertices[shape->m_vertexCount].x) );
		pElement->QueryFloatAttribute( "y", &(shape->m_vertices[shape->m_vertexCount].y) );
		++(shape->m_vertexCount);
	}

	GetShapeProperties( in_pxmlShape, def );

	m_FixtureDef.push_back(def);
}

void BodyDefinition::GetShapeProperties( TiXmlElement* in_pxmlShape, b2FixtureDef& out_b2FixtureDef )
{
	std::string szName, szValue;

	for( TiXmlElement* pElement = in_pxmlShape->FirstChildElement("property"); pElement != NULL; pElement = pElement->NextSiblingElement("property") )
	{
		pElement->QueryStringAttribute( "name", &szName );

		if( szName == "friction" )
		{			
			pElement->QueryFloatAttribute( "value", &out_b2FixtureDef.friction );
		}
		else if( szName == "restitution" )
		{
			pElement->QueryFloatAttribute( "value", &out_b2FixtureDef.restitution );
		}
		else if( szName == "density" )
		{
			pElement->QueryFloatAttribute( "value", &out_b2FixtureDef.density );
		}
		else if( szName == "isSensor" )
		{
			if( szValue == "true" )
				out_b2FixtureDef.isSensor = true;
			else if( szValue == "false" )
				out_b2FixtureDef.isSensor = false;
		}		
	}
}


// ============================================================================

BodyDefinition* BodyDefinitionMan::Get( const std::string& in_szFilename )
{
	std::map< std::string, BodyDefinition* >::iterator it = m_mapBodyDef.find( in_szFilename );

	if( it != m_mapBodyDef.end() )
	{
		// found it, return it
		//		Lair::GetLogMan()->Log( "BodyDefMan", "Loaded texture from map (%s).", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it
		unsigned long nTime = Lair::GetSysMan()->GetTime();

		BodyDefinition* pBodyDef = new BodyDefinition;
		if( pBodyDef->Load( in_szFilename ) )
		{
			nTime = Lair::GetSysMan()->GetTime() - nTime;

			m_mapBodyDef.insert( std::make_pair(in_szFilename,pBodyDef) );
			Lair::GetLogMan()->Log( "BodyDefMan", "Loaded Box2d body definition from file named %s in %d ms.", in_szFilename.c_str(), nTime );
			return pBodyDef;
		}
		Lair::GetLogMan()->Log( "BodyDefMan", "Could not load Box2d body definition from file named %s.", in_szFilename.c_str() );

		delete pBodyDef;
	}
	return 0;
}