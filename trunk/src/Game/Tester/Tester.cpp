#include "Tester.h"

#include "../Engine.h"
#include "../Clock.h"
#include "../../Lair/Lair.h"
#include "../../DebugDraw/DebugDraw.h"

#include "../../Sprite/Sprite.h"

CTester::CTester()
{
	m_pSprite = new Sprite;
}

void CTester::Update( float in_fDeltaTime )
{
	float t = m_pEngine->GetClock()->GetTotalTime();

	m_fAngle = t;

	m_vPos.x = cos( m_fAngle )	* 256;
	m_vPos.y = sin( m_fAngle * 2 ) * 256;

	m_pSprite->Update( in_fDeltaTime );
}

void CTester::Render()
{
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

	in_pRD->Text( m_vPos.x, m_vPos.y-64, "Beware of the evil eye." );

}

void CTester::Keyboard( unsigned char in_cKey )
{
	switch(in_cKey)
	{
	case '1':	Lair::GetSoundMan()->Get("test.wav")->Play( false, true );	break;
	case '2':	Lair::GetSoundMan()->Get("test2.wav")->Play( true );		break;
	case '3':	m_pSprite->Play( "sample.spr" );	break;
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
