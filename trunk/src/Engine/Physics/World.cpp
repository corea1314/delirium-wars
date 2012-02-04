
#include "World.h"

#include <Box2D/Box2D.h>
#include <Engine/Physics/BodyDef.h>

#include "Lair/Lair.h"
#include "Engine/Engine.h"

static const int kVelocityIterations = 10; //8;
static const int kPositionIterations = 10; //3;

CWorld::CWorld() 
	: CEntity("world")
	, m_fHz(60.0f)
	, m_nVelocityIterations(kVelocityIterations)
	, m_nPositionIterations(kPositionIterations)
	, m_bEnableWarmStarting(true)
	, m_bEnableContinuous(true)
	, m_pWorld(0)
	, m_pContactListener(0)
	, m_pContactFilter(0)
	, m_pDestructionListener(0)
	, m_pBodyDefMan(0)
{
}

void CWorld::Connect( CEngine* in_pEngine )
{
	Init( b2Vec2(0,-9.8f), true );
	in_pEngine->Connect_OnUpdate( this, &CWorld::Update );
	in_pEngine->Connect_OnRenderDebug( this, &CWorld::RenderDebug );
}

void CWorld::Disconnect( CEngine* in_pEngine )
{
	in_pEngine->Disconnect_OnUpdate( this );
	in_pEngine->Disconnect_OnRenderDebug( this );
	Exit();
}

void CWorld::Init( const b2Vec2& in_v2Gravity, bool in_bAllowSleep )
{
	m_pWorld = new b2World( in_v2Gravity, in_bAllowSleep );

	m_pContactListener = new ContactListener(m_pWorld);
	m_pContactFilter = new ContactFilter(m_pWorld);
	m_pDestructionListener = new DestructionListener(m_pWorld);
	m_pDebugDraw = new DebugDraw;
	m_pDebugDraw->SetFlags( b2DebugDraw::e_shapeBit | b2DebugDraw::e_jointBit | 
							// b2DebugDraw::e_aabbBit | b2DebugDraw::e_pairBit | 
							b2DebugDraw::e_centerOfMassBit );
		
	m_pWorld->SetDebugDraw( m_pDebugDraw );
	m_pWorld->SetContactListener( m_pContactListener );
	m_pWorld->SetContactFilter( m_pContactFilter );
	m_pWorld->SetDestructionListener( m_pDestructionListener );

	m_pBodyDefMan = new BodyDefinitionMan;
}

void  CWorld::Exit()
{
	delete m_pDebugDraw; m_pDebugDraw = 0;
	delete m_pBodyDefMan; m_pBodyDefMan = 0;
	delete m_pWorld; m_pWorld = 0;
	delete m_pContactListener; m_pContactListener = 0;
	delete m_pContactFilter; m_pContactFilter = 0;
	delete m_pDestructionListener; m_pDestructionListener = 0;
}

void CWorld::RenderDebug( CDebugDraw* in_pDebugDraw )
{
	m_pDebugDraw->SetDebugDraw( in_pDebugDraw );
	m_pWorld->DrawDebugData();
}

void  CWorld::Update( float in_fDeltaTime )
{
	float32 timeStep = in_fDeltaTime; //m_fHz > 0.0f ? 1.0f / m_fHz : float32(0.0f);

	m_pWorld->SetWarmStarting(m_bEnableWarmStarting);
	m_pWorld->SetContinuousPhysics(m_bEnableContinuous);

	m_pWorld->Step(timeStep, m_nVelocityIterations, m_nPositionIterations);
}

b2Body* CWorld::CreateBody( CEntity* in_pEntity, const std::string& in_szBodyDefinitionFilename, const Vector2& in_vPos, bool in_bActive )
{
	BodyDefinition* pBodyDef = m_pBodyDefMan->Get( in_szBodyDefinitionFilename );
	if( pBodyDef )
	{
		pBodyDef->GetBody().position.x = in_vPos.x;
		pBodyDef->GetBody().position.y = in_vPos.y;
		pBodyDef->GetBody().active = in_bActive;
		b2Body* pBody = m_pWorld->CreateBody( &pBodyDef->GetBody() );

		pBody->SetUserData( in_pEntity );
		
		std::vector<b2FixtureDef>::iterator itEnd = pBodyDef->GetFixtures().end();

		for( std::vector<b2FixtureDef>::iterator it = pBodyDef->GetFixtures().begin(); it != itEnd; it++ )
		{
			pBody->CreateFixture( &(*it) );
		}

		return pBody;
	}

	return 0;	
}

void CWorld::DestroyBody( b2Body** in_ppBody )
{
	m_pWorld->DestroyBody( *in_ppBody );
	*in_ppBody = 0;
}

// ============================================================================


void CWorld::DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for (int i = 0; i < vertexCount; ++i)
	{
		v[i].x = vertices[i].x;
		v[i].y = vertices[i].y;
	}

	m_pDebugDraw->DrawPoly( Vector2(0,0), v, vertexCount, 0.0f );

	/*
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
	*/
}

void CWorld::DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
//	m_pDebugDraw->SetColor( Color(color.r,color.g,color.b) );
	for (int i = 0; i < vertexCount; ++i)
	{
		v[i].x = vertices[i].x;
		v[i].y = vertices[i].y;
	}

	m_pDebugDraw->DrawPoly( Vector2(0,0), v, vertexCount, 0.0f );
	
	/*
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
	*/
}

void CWorld::DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
//	m_pDebugDraw->SetColor( Color(color.r,color.g,color.b) );
	m_pDebugDraw->DrawCircle( Vector2(center.x, center.y), radius );

	/*
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
	*/
}

void CWorld::DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
//	m_pDebugDraw->SetColor( Color(color.r,color.g,color.b) );
	m_pDebugDraw->DrawCircle( Vector2(center.x, center.y), radius );
	/*
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();

	b2Vec2 p = center + radius * axis;
	glBegin(GL_LINES);
	glVertex2f(center.x, center.y);
	glVertex2f(p.x, p.y);
	glEnd();
	*/
}

void CWorld::DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
//	m_pDebugDraw->SetColor( Color(color.r,color.g,color.b) );
	m_pDebugDraw->DrawLine( Vector2(p1.x, p1.y), Vector2(p2.x, p2.y) );
	/*
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
	*/
}

void CWorld::DebugDraw::DrawTransform(const b2Transform& xf)
{
	/*
	b2Vec2 p1 = xf.position, p2;
	const float32 k_axisScale = 0.4f;
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col1;
	glVertex2f(p2.x, p2.y);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col2;
	glVertex2f(p2.x, p2.y);

	glEnd();
	*/
}

void CWorld::DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
//	m_pDebugDraw->SetColor( Color(color.r,color.g,color.b) );
	m_pDebugDraw->DrawRectangle( Vector2(p.x,p.y), size, size, (float)PI/4 );
	/*
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p.x, p.y);
	glEnd();
	glPointSize(1.0f);
	*/
}

void CWorld::DebugDraw::DrawString(int x, int y, const char *string, ...)
{
//	m_pDebugDraw->Text( x, y, string );

	/*
	char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	*/
}

void CWorld::DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
//	m_pDebugDraw->SetColor( Color(c.r,c.g,c.b) );
	m_pDebugDraw->DrawRectangle( Vector2(aabb->GetCenter().x, aabb->GetCenter().y), aabb->upperBound.x-aabb->lowerBound.x, aabb->upperBound.y-aabb->lowerBound.y, 0.0f );
	/*
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb->lowerBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->upperBound.y);
	glVertex2f(aabb->lowerBound.x, aabb->upperBound.y);
	glEnd();
	*/
}
