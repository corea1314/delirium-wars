#include "Tester.h"

#include "../Engine.h"
#include "../Clock.h"
#include "../../Lair/Lair.h"
#include "../../DebugDraw/DebugDraw.h"
#include "../../Physics/PhysicsManager.h"
#include "../../Physics/PhysicsBody.h"
#include "../../Sprite/Sprite.h"

CTester::CTester()
{
	m_pSprite = new Sprite;

}

CTester::~CTester()
{
    SAFE_DELETE(m_pSprite);
}

void CTester::Update( float in_fDeltaTime )
{
	float t = m_pEngine->GetClock()->GetTotalTime();

	m_fAngle = t;

	m_vPos.x = cos( m_fAngle )	* 256.0f;
	m_vPos.y = sin( m_fAngle * 2 ) * 256.0f;

	
	m_pSprite->Update( in_fDeltaTime );
}

void CTester::Render()
{
	m_pSprite->Set( m_vPos.x, m_vPos.y );
	m_pSprite->Render();
}

void CTester::RenderDebug( CDebugDraw* in_pRD )
{
	in_pRD->SetColor( CDebugDraw::Color::eGREEN );
	in_pRD->DrawCircle( m_vPos, 32 );

	in_pRD->SetColor( CDebugDraw::Color::eRED );
	in_pRD->DrawRectangle( m_vPos, 128, 8, m_fAngle/4 );	

	in_pRD->SetColor( CDebugDraw::Color::eBLUE );
	in_pRD->DrawTriangle( m_vPos, 64, 64, -m_fAngle*2 );

	in_pRD->SetColor( CDebugDraw::Color::eCYAN );
	in_pRD->DrawEllipse( m_vPos, 64, 32, m_fAngle/2 );

	in_pRD->Text( static_cast<int>(m_vPos.x), static_cast<int>(m_vPos.y)-64, "Beware of the evil eye." );
}

void CTester::Keyboard( unsigned char in_cKey )
{
	switch(in_cKey)
	{
	case '1':	Lair::GetSoundMan()->Get("test.wav")->Play( false, true );	break;
	case '2':	Lair::GetSoundMan()->Get("test2.wav")->Play( true );		break;
	case '3':	m_pSprite->Play( "sample.spr" );	break;
    case '4':	CreatePhysicsFallingBody();	break;
	}
}

void CTester::Connect( CEngine* in_pEngine )
{
	m_pEngine = in_pEngine;
	m_pEngine->Connect_Update( this, &CTester::Update );
	m_pEngine->Connect_Render( this, &CTester::Render );
	m_pEngine->Connect_RenderDebug( this, &CTester::RenderDebug );
	m_pEngine->Connect_Keyboard( this, &CTester::Keyboard );
}

void CTester::Disconnect( CEngine* in_pEngine )
{
	assert( m_pEngine == in_pEngine );
	in_pEngine->Disconnect_Update( this );
	in_pEngine->Disconnect_Render( this );
	in_pEngine->Disconnect_RenderDebug( this );
	in_pEngine->Disconnect_Keyboard( this );
	m_pEngine = 0;
}

void CTester::BuildPhysicsScene()
{
    //Floor
    CPhysicsBody* pBody = m_pEngine->GetPhysicsMan()->CreatePhysicsBody("Floor");
    pBody->SetPhysicsPosition( Vector2(0.0,-300.0f) );
    pBody->SetShape( Vector2(1500.0f,300.0f) );
    pBody->SetInfiniteMass();
    pBody->SetSimulationEnable(false);
}

void CTester::CreatePhysicsFallingBody()
{
    srand ( m_pEngine->GetClock()->GetTotalTime() );
    float fHorizontalSpeed = 20.0f * static_cast<float>((rand() % 20) - 10);
    float fVerticalSpeed =  100.0f * static_cast<float>((rand() % 4) + 4);
    //Floor
    CPhysicsBody* pBody = m_pEngine->GetPhysicsMan()->CreatePhysicsBody("Falling body");
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
