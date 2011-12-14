#ifndef _ENGINE_H
#define _ENGINE_H

#include <map>

#include "SigSlot.h"

class CClock;
class CCamera;
class CPhysicsManager;
class CDebugDraw;
class CWorld;
class CTester;

class RenderTarget;
class Texture;
class SpriteRenderer;

class CPlayer;
class CEntity;

#include <Math/Vector2.h>

//Objects should use the proxy when trying to connect with it
class CEngineProxy
{
    public :
        CEngineProxy(){}
        virtual ~CEngineProxy(){}
		
    NEW_SIGNAL3( OnGamepad0, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( OnGamepad1, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( OnGamepad2, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( OnGamepad3, unsigned int, unsigned int, float* );	// button state, axis count, axis values

	NEW_SIGNAL1( OnKeyboard, unsigned char );	// key

	NEW_SIGNAL4( OnMouseClick, unsigned int, int, int, const Vector2& );	// button, state, screen space x, screen space y, world position
	NEW_SIGNAL1( OnMouseWheel, unsigned int );	// wheel value

    NEW_SIGNAL1( OnLoadFile, const char* ); // filename
	 
	NEW_PROTECTED_SIGNAL1( OnUpdate, float );	// delta time
	NEW_PROTECTED_SIGNAL0( OnRenderFrontLayer );
	NEW_PROTECTED_SIGNAL0( OnRenderBackLayer );
	NEW_PROTECTED_SIGNAL0( OnRenderDiffusionLayer );
	NEW_PROTECTED_SIGNAL0( OnRenderGUI );
    NEW_PROTECTED_SIGNAL1( OnRenderDebug, CDebugDraw* );
	NEW_PROTECTED_SIGNAL1( OnRenderDebugGUI, CDebugDraw* );
};

class CEngine : public CEngineProxy
{
public:	
	CEngine();
	virtual ~CEngine();

	const CClock*	GetClock() const { return m_pClock; }
	CCamera*		GetCamera() const { return m_pCamera; }
	CWorld*			GetWorld() const { return m_pWorld; }
	
	void RenderDebugDraw();
	void Render();
	void RenderGUI();
	void Update( float in_fDeltaTime );

	CEntity* CEngine::GetEntity( const std::string& in_szEntityName, const std::string& in_szLuaScript );

private:
	CClock*		m_pClock;
	CCamera*	m_pCamera;
	CDebugDraw*	m_pDebugDraw;
	CWorld*		m_pWorld;

	CEntity*	m_pEntryPointEntity;

	CPlayer*		m_pPlayer;
	
	RenderTarget*	m_pRT;
	Texture*		m_pRTT[4];

	unsigned int	m_nCurrentDiffusion;

	enum E { eRTT_Diffusion0, eRTT_Diffusion1, eRTT_FrontLayer, eRTT_BackLayer, eRTT_Max };
	
	//todo: refactor later
	std::map<std::string, CEntity*>	m_mapEntity;
};

#endif//_ENGINE_H
