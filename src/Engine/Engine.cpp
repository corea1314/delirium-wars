#include "Engine.h"

#include "Engine/Clock/Clock.h"
#include "Engine/Physics/World.h"
#include "Lair/RenderTarget/RenderTarget.h"

#include "Engine/Entities/Player/Player.h"

#include "DebugDraw/DebugDraw.h"

#include "Lair/Lair.h"

#include "macros.h"

#include "glee/GLee.h"

#define RENDER_TARGET_SIZE_X	1024
#define RENDER_TARGET_SIZE_Y	1024

CEngine::CEngine() : m_nCurrentDiffusion(0)
{
	// Create lair
	Lair::Create();

	// Create the clock
	m_pClock = new CClock;
	m_pClock->Connect(this);

	// Create the world
	m_pWorld = new CWorld;
	m_pWorld->Connect( this );

	// Entry point entity
	m_pEntryPointEntity = GetEntity( "%entry_point%", "scripts/entrypoint.lua", Vector2(0,0) );

	// Create debug draw interface
	m_pDebugDraw = new CDebugDraw( 1024, 1024, 3.0f );

	// Create player
//	m_pPlayer = new CPlayer;
//	m_pPlayer->Connect( this );

	// Render targets
	m_pRT = new RenderTarget;
	m_pRT->Create( RENDER_TARGET_SIZE_X, RENDER_TARGET_SIZE_Y, true, false );

	m_pRTT[eRTT_Diffusion0] = m_pRT->SetTextureTarget( eRTT_Diffusion0, GL_TEXTURE_2D, GL_RGBA );	// diffusion 0
	m_pRTT[eRTT_Diffusion1] = m_pRT->SetTextureTarget( eRTT_Diffusion1, GL_TEXTURE_2D, GL_RGBA );	// diffusion 1
	m_pRTT[eRTT_Diffusion2] = m_pRT->SetTextureTarget( eRTT_Diffusion2, GL_TEXTURE_2D, GL_RGBA );	// diffusion 1
	m_pRTT[eRTT_BackLayer]	= m_pRT->SetTextureTarget( eRTT_BackLayer, GL_TEXTURE_2D, GL_RGBA );	// back layer
	m_pRTT[eRTT_FrontLayer] = m_pRT->SetTextureTarget( eRTT_FrontLayer, GL_TEXTURE_2D, GL_RGBA );	// front layer

	m_pRTT[eRTT_Diffusion0]->SetWrap( Texture::WrapCoord::U, Texture::WrapMode::Clamp );
	m_pRTT[eRTT_Diffusion0]->SetWrap( Texture::WrapCoord::V, Texture::WrapMode::Clamp );
	m_pRTT[eRTT_Diffusion1]->SetWrap( Texture::WrapCoord::U, Texture::WrapMode::Clamp );
	m_pRTT[eRTT_Diffusion1]->SetWrap( Texture::WrapCoord::V, Texture::WrapMode::Clamp );
	m_pRTT[eRTT_Diffusion2]->SetWrap( Texture::WrapCoord::U, Texture::WrapMode::Clamp );
	m_pRTT[eRTT_Diffusion2]->SetWrap( Texture::WrapCoord::V, Texture::WrapMode::Clamp );

	m_pRT->Bind();
	glClearColor(0,0,0,0);
		m_pRT->SetActiveTextureTarget( eRTT_Diffusion0 );
		glClear( GL_COLOR_BUFFER_BIT );
		m_pRT->SetActiveTextureTarget( eRTT_Diffusion1 );
		glClear( GL_COLOR_BUFFER_BIT );
		m_pRT->SetActiveTextureTarget( eRTT_Diffusion2 );
		glClear( GL_COLOR_BUFFER_BIT );
	m_pRT->Unbind();

	m_pShaderGodRays = new ShaderGLSL;
	m_pShaderGodRays->Create( "shaders/godrays" );

	m_pShaderDiffusion = new ShaderGLSL;
	m_pShaderDiffusion->Create( "shaders/diffusion" );

	m_pExposure = m_pShaderGodRays->GetUniform("exposure");
	m_pDecay = m_pShaderGodRays->GetUniform("decay");
	m_pDensity = m_pShaderGodRays->GetUniform("density");
	m_pWeight = m_pShaderGodRays->GetUniform("weight");
	m_pLightPositionOnScreen = m_pShaderGodRays->GetUniform("lightPositionOnScreen");

	m_eDiffusionLast = eRTT_Diffusion0;
	m_eDiffusionCurr = eRTT_Diffusion1;
	m_eDiffusionDest = eRTT_Diffusion2;
}

CEngine::~CEngine()
{
	SAFE_DELETE( m_pShaderGodRays );
	SAFE_DELETE( m_pShaderDiffusion );

	m_pRT->Destroy();
	SAFE_DELETE( m_pRT );	

	SAFE_DELETE(m_pDebugDraw);

//	m_pPlayer->Disconnect( this );
//	SAFE_DELETE(m_pPlayer);

	m_pWorld->Disconnect(this);
	SAFE_DELETE(m_pWorld);

	m_pClock->Disconnect(this);
	SAFE_DELETE(m_pClock);

//	SAFE_DELETE( m_pEntryPointEntity );

	Lair::Release();
}

CEntity* CEngine::GetEntity( const std::string& in_szEntityName )
{
	std::map< std::string, CEntity* >::iterator it = m_mapEntity.find( in_szEntityName );

	if( it != m_mapEntity.end() )
	{
		// found it, return it
		return it->second;
	}
	
	return 0;
}

CEntity* CEngine::GetEntity( const std::string& in_szEntityName, const std::string& in_szLuaScript, const Vector2& in_vPos, float in_fAngle )
{
	CEntity* pEntity = GetEntity(in_szEntityName);

	if( pEntity != 0 )
	{
		// found it, return it
		return pEntity;
	}
	else
	{
		// not found, load it, return it
		pEntity = new CEntity( in_szEntityName );
		pEntity->GetPos() = in_vPos;
		pEntity->GetAngle() = in_fAngle;

		if( pEntity->Load(in_szLuaScript) )
		{
			pEntity->Connect( this );
			m_mapEntity.insert( std::make_pair(in_szEntityName,pEntity) );

			Lair::GetLogMan()->Log( "Engine", "Created entity named %s.", in_szEntityName.c_str() );
			return pEntity;
		}
		Lair::GetLogMan()->Log( "Engine", "Could not create new entity named %s.", in_szEntityName.c_str() );
	}
	return 0;
}

void CEngine::FreeEntity( const std::string& in_szEntityName )
{
	std::map< std::string, CEntity* >::iterator it = m_mapEntity.find( in_szEntityName );

	if( it != m_mapEntity.end() )
	{
		// found it, free it
		CEntity* pEntity = it->second;
		pEntity->Disconnect(this);
		m_mapEntity.erase(it);
	}
}

void CEngine::Update( float in_fDeltaTime )
{
	OnUpdate( in_fDeltaTime );
	OnPostUpdate();
}

void CEngine::Render()
{
	Vector2 vPos = Lair::GetCameraMan()->GetActiveCamera()->GetPos();
	float fZoom = Lair::GetCameraMan()->GetActiveCamera()->GetZoom();
	float fAngle = Lair::GetCameraMan()->GetActiveCamera()->GetAngle();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( -RENDER_TARGET_SIZE_X/2*fZoom, RENDER_TARGET_SIZE_X/2*fZoom, -RENDER_TARGET_SIZE_Y/2*fZoom, RENDER_TARGET_SIZE_Y/2*fZoom, -1.0f, 1.0f );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef( (float)RAD_TO_DEG(fAngle), 0.0f, 0.0f, -1.0f );
	glTranslatef( -vPos.x, -vPos.y, 0.0f );

	glEnable( GL_TEXTURE_2D );

	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	
	// Here we build all the frame layers
	m_pRT->Bind();
		
		m_pRT->SetActiveTextureTarget( eRTT_FrontLayer );
		glClear( GL_COLOR_BUFFER_BIT );
		OnRenderFrontLayer();
		
		m_pRT->SetActiveTextureTarget( eRTT_BackLayer );
		glClear( GL_COLOR_BUFFER_BIT );
		
		OnRenderBackLayer();

		Lair::GetSpriteMan()->Render();

		RenderDebugDraw();

		/*
		m_nCurrentDiffusion = (++m_nCurrentDiffusion)%2;

		m_pRT->SetActiveTextureTarget( m_nCurrentDiffusion );	// here we need to cycle

		OnRenderDiffusionLayer();

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D(0,1,0,1);
		
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();


#define p	0.0f //1.0f / 1024.0f
#define f	1.0f
		glColor4f(f,f,f,1);	

		m_pRTT[(m_nCurrentDiffusion+1)%2]->Bind();
		glBegin( GL_QUADS );
		glTexCoord2f(0,0+p);	glVertex2f(0,0+p);	//add effect here
		glTexCoord2f(1,0+p);	glVertex2f(1,0+p);
		glTexCoord2f(1,1+p);	glVertex2f(1,1+p);
		glTexCoord2f(0,1+p);	glVertex2f(0,1+p);
		glEnd();

		
		glColor4f(1,1,1,1);
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( -RENDER_TARGET_SIZE_X/2*fZoom, RENDER_TARGET_SIZE_X/2*fZoom, -RENDER_TARGET_SIZE_Y/2*fZoom, RENDER_TARGET_SIZE_Y/2*fZoom );

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef( -vPos.x, -vPos.y, 0.0f );
		
		//
		*/

		ProcessDiffusionTextures();
								
	m_pRT->Unbind();
		
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
}

void CEngine::ProcessDiffusionTextures()
{
	// Rotate diffusion RT textures
	ERTT eTemp = m_eDiffusionCurr;
	m_eDiffusionCurr = m_eDiffusionLast;
	m_eDiffusionLast = m_eDiffusionDest;	
	m_eDiffusionDest = eTemp;

	// Render current diffusion
	m_pRT->SetActiveTextureTarget( m_eDiffusionCurr );
	glClear( GL_COLOR_BUFFER_BIT );
	Lair::GetSpriteMan()->Render();

	// Now composite last and current diffusion
	m_pRT->SetActiveTextureTarget( m_eDiffusionDest );
	glClear( GL_COLOR_BUFFER_BIT );

	m_pShaderDiffusion->Bind();

//	m_pShaderDiffusion->GetUniform("fadeout")->Set(0.99f);
	float displacement[2] = { -4.0f/1024.0f, -4.0f/1024.0f };
//	m_pShaderDiffusion->GetUniform("displacement")->Set( 2, displacement );

	m_pRTT[m_eDiffusionLast]->Bind(0);
	m_pRTT[m_eDiffusionCurr]->Bind(1);
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(0,1,0,1,-1,1);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
#define p	0.0f //1.0f / 1024.0f
#define f	1.0f
	glColor4f(f,f,f,1);	
		
	glBegin( GL_QUADS );
	glTexCoord2f(0,0+p);	glVertex2f(0,0+p);	//add effect here
	glTexCoord2f(1,0+p);	glVertex2f(1,0+p);
	glTexCoord2f(1,1+p);	glVertex2f(1,1+p);
	glTexCoord2f(0,1+p);	glVertex2f(0,1+p);
	glEnd();

	m_pRTT[m_eDiffusionLast]->Unbind();
	m_pRTT[m_eDiffusionCurr]->Unbind();

	m_pShaderDiffusion->Unbind();
}

void CEngine::RenderGUI()
{
	// Here we perform compositing
#define W	1280
#define H	 720
#define O	((W-H)/2)

#define RENDER_QUAD()	\
	glBegin( GL_QUADS );	glTexCoord2f(0,0);	glVertex2f(0,0-O);	glTexCoord2f(1,0);	glVertex2f(W,0-O);	glTexCoord2f(1,1);	glVertex2f(W,W-O);	glTexCoord2f(0,1);	glVertex2f(0,W-O);	glEnd();
	
	glEnable( GL_TEXTURE_2D );

	glColor4f(1,1,1,1);
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	m_pShaderGodRays->Bind();

	m_pExposure->Set( 0.0014f );
	m_pDecay->Set( 1.0f );
	m_pDensity->Set( 0.44f );
	m_pWeight->Set( 5.65f );
	float fPos[2] = { 0.5f, 0.5f };
	m_pLightPositionOnScreen->Set( 2, fPos );
		
	m_pRTT[eRTT_BackLayer]->Bind();
//	RENDER_QUAD();
	
//	m_pRTT[m_nCurrentDiffusion]->Bind();
//	RENDER_QUAD();
	
	m_pShaderGodRays->Unbind();


	// diffusion effect, combine previous and current into result
	
	m_pRTT[m_eDiffusionDest]->Bind();// m_pRTT[eRTT_FrontLayer]->Bind();
	RENDER_QUAD();
	
	// Then we render GUI on top
	OnRenderGUI();
}

void CEngine::RenderDebugDraw()
{
	glDisable( GL_TEXTURE_2D );
	m_pDebugDraw->BeginFrame();
		OnRenderDebug( m_pDebugDraw );
	m_pDebugDraw->EndFrame();
	glEnable( GL_TEXTURE_2D );
}
