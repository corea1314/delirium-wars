#include "Builder.h"

#include "Lair/Lair.h"

CBodyBuilder::CBodyBuilder( const std::string& in_szFilename ) : m_bValid(false)
{
	TiXmlDocument doc(in_szFilename);

	if( doc.LoadFile() )
	{
		m_FixtureDef.reserve(4);	//fixme

		// for( TiXmlElement* pElement = doc.FirstChildElement("body"); pElement != NULL; pElement = pElement->NextSiblingElement("body") )
		TiXmlElement* pElement = doc.FirstChildElement("body");
		{
			ProcessBody( pElement );
		}
	}
	else
	{
		if( doc.Error() )
		{
			Lair::GetLogMan()->Log( "Box2d", "XML parse error in %s: %s (Line %d)", doc.Value(), doc.ErrorDesc(), doc.ErrorRow() );
		}
	}
}

b2Body* CBodyBuilder::Create( b2World* in_pWorld, const b2Vec2& in_vPos, bool m_bActive )
{
	b2Body* pBody = 0;

	if( m_bValid )
	{
		m_BodyDef.position = in_vPos;
		m_BodyDef.active = m_bActive;
		pBody = in_pWorld->CreateBody(&m_BodyDef);
		for( std::vector<b2FixtureDef>::iterator it; it != m_FixtureDef.end(); it++ )
		{
			pBody->CreateFixture( &(*it) );
		}
	}

	return pBody;
}

// ============================================================================

void CBodyBuilder::ProcessBody( TiXmlElement* in_pxmlBody )
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

void CBodyBuilder::ProcessShapeRect( TiXmlElement* in_pxmlShape )
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

void CBodyBuilder::ProcessShapePoly( TiXmlElement* in_pxmlShape )
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

void CBodyBuilder::GetShapeProperties( TiXmlElement* in_pxmlShape, b2FixtureDef& out_b2FixtureDef )
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