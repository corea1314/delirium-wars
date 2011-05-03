#ifndef _ENGINE_H
#define _ENGINE_H

#include "SigSlot.h"

class CField;
class CClock;
class CCamera;
class CPhysicsManager;
class CDebugDraw;
class CTester;
class RenderTarget;
class Texture;

class SpriteRenderer;

class b2World;

//Objects should use the proxy when trying to connect with it
class CEngineProxy
{
    public :
        CEngineProxy(){}
        ~CEngineProxy(){}
		
    NEW_SIGNAL3( OnGamepad0, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( OnGamepad1, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( OnGamepad2, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( OnGamepad3, unsigned int, unsigned int, float* );	// button state, axis count, axis values

	NEW_SIGNAL1( OnKeyboard, unsigned char );	// key

    NEW_SIGNAL1( OnLoadFile, const char* ); // filename
	 
	NEW_PROTECTED_SIGNAL1( OnUpdate, float );	// delta time
	NEW_PROTECTED_SIGNAL0( OnRenderFrontLayer );
	NEW_PROTECTED_SIGNAL0( OnRenderBackLayer );
	NEW_PROTECTED_SIGNAL0( OnRenderDiffusionLayer );
	NEW_PROTECTED_SIGNAL0( OnRenderGUI );
    NEW_PROTECTED_SIGNAL1( OnRenderDebug, CDebugDraw* );
};

class CEngine : public CEngineProxy
{
public:	
	CEngine();
	virtual ~CEngine();

	const CClock*	GetClock() const { return m_pClock; }
	CCamera*		GetCamera() const { return m_pCamera; }
	CField*			GetField() const { return m_pField; }
    CPhysicsManager* GetPhysicsMan() const { return m_pPhysMan; }

	b2World*	GetWorld();
	
	void RenderDebugDraw();
	void Render();
	void RenderGUI();
	void Update( float in_fDeltaTime );

private:
	CField*		m_pField;
	CClock*		m_pClock;
	CCamera*	m_pCamera;
    CPhysicsManager* m_pPhysMan;
	CDebugDraw*	m_pDebugDraw;
	
	RenderTarget*	m_pRT;
	Texture*		m_pRTT[4];

	unsigned int	m_nCurrentDiffusion;

	enum E { eRTT_Diffusion0, eRTT_Diffusion1, eRTT_FrontLayer, eRTT_BackLayer, eRTT_Max };
	
	CTester*	m_pTester;		// quick entity to test some signal and game logic (todo: maybe a vector of those and have ppl derive from the base class)

	//todo: add entity factories here
};

#endif//_ENGINE_H
