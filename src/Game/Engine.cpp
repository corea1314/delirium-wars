#include "Engine.h"

#include "Field.h"
#include "Clock.h"
#include "Camera.h"
#include "Tester/Tester.h"
#include "../Physics/PhysicsManager.h"
#include "../Lair/RenderTarget/RenderTarget.h"

#include "../DebugDraw/DebugDraw.h"

#include "../Lair/Lair.h"

#include "../macros.h"

#define RENDER_TARGET_SIZE_X	1024
#define RENDER_TARGET_SIZE_Y	1024

CEngine::CEngine() : m_nCurrentDiffusion(0)
{
	// Create lair
	Lair::Create();

	// Create the clock
	m_pClock = new CClock;
	m_pClock->Connect(this);

	// Create the field
	m_pField = new CField;
	m_pField->Connect(this);

	// Create the camera
	m_pCamera = new CCamera;
	m_pCamera->Connect(this);

    m_pPhysMan = new CPhysicsManager();
    m_pPhysMan->Connect( this );

	// Create debug draw interface
	m_pDebugDraw = new CDebugDraw( 1024, 1024, 3.0f );

	// Create tester entity
	m_pTester = new CTester;
	m_pTester->Connect( this );

    m_pTester->BuildPhysicsScene();

	m_pRT = new RenderTarget;
	m_pRT->Create( RENDER_TARGET_SIZE_X, RENDER_TARGET_SIZE_Y, true, false );

	m_pRTT[eRTT_Diffusion0] = m_pRT->SetTextureTarget( eRTT_Diffusion0, GL_TEXTURE_2D, GL_RGBA );	// diffusion 0
	m_pRTT[eRTT_Diffusion1] = m_pRT->SetTextureTarget( eRTT_Diffusion1, GL_TEXTURE_2D, GL_RGBA );	// diffusion 1
	m_pRTT[eRTT_BackLayer]	= m_pRT->SetTextureTarget( eRTT_BackLayer, GL_TEXTURE_2D, GL_RGBA );	// back layer
	m_pRTT[eRTT_FrontLayer] = m_pRT->SetTextureTarget( eRTT_FrontLayer, GL_TEXTURE_2D, GL_RGBA );	// front layer
}


CEngine::~CEngine()
{
	m_pRT->Destroy();
	SAFE_DELETE( m_pRT );	

	m_pTester->Disconnect( this );
	SAFE_DELETE(m_pTester);	

	SAFE_DELETE(m_pDebugDraw);

	m_pCamera->Disconnect(this);
	SAFE_DELETE(m_pCamera);

	m_pField->Disconnect(this);
	SAFE_DELETE(m_pField);

	m_pClock->Disconnect(this);
	SAFE_DELETE(m_pClock);

    SAFE_DELETE( m_pPhysMan );

	Lair::Release();
}

void CEngine::Update( float in_fDeltaTime )
{
	OnUpdate( in_fDeltaTime );
}

void CEngine::Render()
{
	Vector2 vPos =GetCamera()->GetPos();
	float fZoom = GetCamera()->GetZoom();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -RENDER_TARGET_SIZE_X/2*fZoom, RENDER_TARGET_SIZE_X/2*fZoom, -RENDER_TARGET_SIZE_Y/2*fZoom, RENDER_TARGET_SIZE_Y/2*fZoom );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( -vPos.x, -vPos.y, 0.0f );

	// todo: remove this test rendering
	//	m_Grid.Render();

	glEnable( GL_TEXTURE_2D );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	// Here we build all the frame layers
	m_pRT->Bind();
		
		m_pRT->SetActiveTextureTarget( eRTT_FrontLayer );
		glClear( GL_COLOR_BUFFER_BIT );
		OnRenderFrontLayer();

		
		m_pRT->SetActiveTextureTarget( eRTT_BackLayer );
		glClear( GL_COLOR_BUFFER_BIT );
		RenderDebugDraw();
		OnRenderBackLayer();

		m_nCurrentDiffusion = (++m_nCurrentDiffusion)%2;

		m_pRT->SetActiveTextureTarget( m_nCurrentDiffusion );	// here we need to cycle

		OnRenderDiffusionLayer();

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D(0,1,0,1);
		
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();


#define p	0.0f
#define f	0.5f
		glColor4f(f,f,f,1.0f);	
		m_pRTT[(m_nCurrentDiffusion+1)%2]->Bind();
		glBegin( GL_QUADS );
		glTexCoord2f(0,0+p);	glVertex2f(0,0);	//add effect here
		glTexCoord2f(1,0+p);	glVertex2f(1,0);
		glTexCoord2f(1,1+p);	glVertex2f(1,1);
		glTexCoord2f(0,1+p);	glVertex2f(0,1);
		glEnd();

		/*
		glColor4f(1,1,1,1);
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( -RENDER_TARGET_SIZE_X/2*fZoom, RENDER_TARGET_SIZE_X/2*fZoom, -RENDER_TARGET_SIZE_Y/2*fZoom, RENDER_TARGET_SIZE_Y/2*fZoom );

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef( -vPos.x, -vPos.y, 0.0f );
		*/
		//	
				
	m_pRT->Unbind();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
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

	m_pRTT[eRTT_BackLayer]->Bind();
	RENDER_QUAD();
	
	m_pRTT[m_nCurrentDiffusion]->Bind();
	RENDER_QUAD();

	m_pRTT[eRTT_FrontLayer]->Bind();
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