#include "Tester.h"

#include "Engine/Engine.h"
#include "Engine/Entities/Clock/Clock.h"
#include "Lair/Lair.h"
#include "DebugDraw/DebugDraw.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsBody.h"
#include "Lair/Atlas/Atlas.h"

IMPLEMENT_CLASS_TYPE(CTester)

CTester::CTester()
{
	m_pSprite = Lair::GetSpriteMan()->GetSprite();
	m_pSpriteDiffusion = Lair::GetSpriteMan()->GetSprite();

	Lair::GetTextureMan()->Get("test1024.png");
//	Lair::GetTextureMan()->Get("test2048.png");
}

CTester::~CTester()
{
    SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pSpriteDiffusion);
}

void CTester::Update( float in_fDeltaTime )
{
	float t = GetEngine()->GetClock()->GetTotalTime();

	m_fAngle = t / 2;

	m_vPos.x = 0.0f; //cos( m_fAngle )	* 256.0f;
	m_vPos.y = 0.0f; //sin( m_fAngle * 2 ) * 256.0f;
	
	m_pSprite->Set( m_vPos.x, m_vPos.y );
	m_pSpriteDiffusion->Set( m_vPos.x, m_vPos.y );
		
	m_pSprite->Update( in_fDeltaTime );
	m_pSpriteDiffusion->Update( in_fDeltaTime );

	float s = 0; //(sin(t)+1)/2;

	m_pSpriteDiffusion->SetColor( s,s,s );
}

void CTester::RenderFrontLayer()
{
}

void CTester::RenderBackLayer()
{
	
}

void CTester::RenderDiffusionLayer()
{
}

void CTester::RenderDebug( CDebugDraw* in_pRD )
{
	in_pRD->SetColor( CDebugDraw::Color::eRED );
	in_pRD->DrawRectangle( Vector2(0,0), 8, 8, 0 );	

	/*
	in_pRD->SetColor( CDebugDraw::Color::eGREEN );
	in_pRD->DrawCircle( m_vPos, 32 );

	in_pRD->SetColor( CDebugDraw::Color::eRED );
	in_pRD->DrawRectangle( m_vPos, 128, 8, m_fAngle/4 );	

	in_pRD->SetColor( CDebugDraw::Color::eBLUE );
	in_pRD->DrawTriangle( m_vPos, 64, 64, -m_fAngle*2 );

	in_pRD->SetColor( CDebugDraw::Color::eCYAN );
	in_pRD->DrawEllipse( m_vPos, 64, 32, m_fAngle/2 );

	in_pRD->Text( static_cast<int>(m_vPos.x), static_cast<int>(m_vPos.y)-64, "Beware of the evil eye." );
	*/
}

void CTester::Keyboard( unsigned char in_cKey )
{
	switch(in_cKey)
	{
	case '1':	Lair::GetSoundMan()->Get("test.wav")->Play( false, true );	break;
	case '2':	Lair::GetSoundMan()->Get("test2.wav")->Play( true );		break;
	case '3':	
		m_pSprite->Play( "sample.spr" );	
		m_pSpriteDiffusion->Play( "sprite_glow.spr" );	
		break;
    case '4':	CreatePhysicsFallingBody();	break;
	}
}

void CTester::Connect( CEngine* in_pEngine )
{
	CEntity::Connect(in_pEngine);
	GetEngine()->Connect_OnUpdate( this, &CTester::Update );
	GetEngine()->Connect_OnRenderBackLayer( this, &CTester::RenderBackLayer );
	GetEngine()->Connect_OnRenderFrontLayer( this, &CTester::RenderFrontLayer );
	GetEngine()->Connect_OnRenderDiffusionLayer( this, &CTester::RenderDiffusionLayer );
	GetEngine()->Connect_OnRenderDebug( this, &CTester::RenderDebug );
	GetEngine()->Connect_OnKeyboard( this, &CTester::Keyboard );
}

void CTester::Disconnect( CEngine* in_pEngine )
{
	in_pEngine->Disconnect_OnUpdate( this );
	in_pEngine->Disconnect_OnRenderBackLayer( this );
	in_pEngine->Disconnect_OnRenderFrontLayer( this );
	in_pEngine->Disconnect_OnRenderDiffusionLayer( this );
	in_pEngine->Disconnect_OnRenderDebug( this );
	in_pEngine->Disconnect_OnKeyboard( this );
	CEntity::Disconnect(in_pEngine);
}

void CTester::BuildPhysicsScene()
{
    //Floor
    CPhysicsBody* pBody = GetEngine()->GetPhysicsMan()->CreatePhysicsBody("Floor");
    pBody->SetPhysicsPosition( Vector2(0.0,-300.0f) );
    pBody->SetShape( Vector2(1500.0f,300.0f) );
    pBody->SetInfiniteMass();
    pBody->SetSimulationEnable(false);
}

void CTester::CreatePhysicsFallingBody()
{
    srand ( (unsigned int)GetEngine()->GetClock()->GetTotalTime() );
    float fHorizontalSpeed = 20.0f * static_cast<float>((rand() % 20) - 10);
    float fVerticalSpeed =  100.0f * static_cast<float>((rand() % 4) + 4);
    //Floor
    CPhysicsBody* pBody = GetEngine()->GetPhysicsMan()->CreatePhysicsBody("Falling body");
    pBody->SetPhysicsPosition( Vector2(0.0,300.0f) );
    pBody->SetShape( Vector2(60.0f,60.0f) );
    pBody->SetMass( 1.0f );
    pBody->SetGravity( Vector2( 0.0f, -500.0f ) );
    pBody->SetVelocity( Vector2( fHorizontalSpeed, fVerticalSpeed ) );
    pBody->Connect_PhysicsBodyOnBodyCollision( this, &CTester::BodyOnBodyCollision );
}

void CTester::BodyOnBodyCollision( CCollisionBodyOnBodyResult* in_pCollisionResult )
{
    //Separate the calling body
    float fSeparationRatio = 1.0f / (in_pCollisionResult->GetForeignPhysicsBody()->GetInverseMass() + in_pCollisionResult->GetCallingPhysicsBody()->GetInverseMass());
    //Vector2 v2Delta( in_pCollisionResult->GetForeignPhysicsBody()->GetPhysicsPosition() - in_pCallingBody->GetPhysicsPosition() );
    in_pCollisionResult->GetCallingPhysicsBody()->TranslatePhysicsPosition( -fSeparationRatio* in_pCollisionResult->GetNormal() * in_pCollisionResult->GetSeparationDistance() );

    float fFriction = 0.5f;
    float fElasticity = 0.5f;

    Vector2 v2BounceVelocity( - Vector2::GetProjection( in_pCollisionResult->GetCallingPhysicsBody()->GetVelocity(), in_pCollisionResult->GetNormal() ) * fElasticity );
    Vector2 v2LateralVelocity(  Vector2::GetProjection( in_pCollisionResult->GetCallingPhysicsBody()->GetVelocity(), Vector2::GetPerpendicular( in_pCollisionResult->GetNormal() ) ) * fFriction );

    in_pCollisionResult->GetCallingPhysicsBody()->SetVelocity( v2BounceVelocity + v2LateralVelocity );
}
