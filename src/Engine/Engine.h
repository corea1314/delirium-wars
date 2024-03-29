#ifndef _ENGINE_H
#define _ENGINE_H

#include <map>

#include "SigSlot.h"

class CClock;
class CPhysicsManager;
class CDebugDraw;
class CWorld;
class CTester;

class RenderTarget;
class Texture;
class SpriteRenderer;

class CPlayer;
class CEntity;

#include "Lair/Shader/ShaderGLSL.h"

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

	NEW_SIGNAL2( OnKeyboard, unsigned char, bool );	// key, down state

	NEW_SIGNAL4( OnMouseClick, unsigned int, int, int, const Vector2& );	// button, state, screen space x, screen space y, world position
	NEW_SIGNAL1( OnMouseWheel, unsigned int );	// wheel value

    NEW_SIGNAL1( OnLoadFile, const char* ); // filename
	 
	NEW_PROTECTED_SIGNAL1( OnUpdate, float );	// delta time
	NEW_PROTECTED_SIGNAL0( OnPostUpdate );
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
	CWorld*			GetWorld() const { return m_pWorld; }
	
	void RenderDebugDraw();
	void Render();
	void RenderGUI();
	void Update( float in_fDeltaTime );

	CEntity* GetEntity( const std::string& in_szEntityName );
	CEntity* GetEntity( const std::string& in_szEntityName, const std::string& in_szLuaScript, const Vector2& in_vPos, float in_fAngle = 0.0f );
	void FreeEntity( const std::string& in_szEntityName );

private:
	CClock*		m_pClock;
	CDebugDraw*	m_pDebugDraw;
	CWorld*		m_pWorld;

	CEntity*	m_pEntryPointEntity;

	CPlayer*		m_pPlayer;
	
	RenderTarget*	m_pRT;
	Texture*		m_pRTT[4];
		
	ShaderGLSL*		m_pShaderGodRays;
	ShaderGLSL*		m_pShaderDiffusion;

	//
	std::shared_ptr<ShaderGLSL::Uniform> m_pExposure;
	std::shared_ptr<ShaderGLSL::Uniform> m_pDecay;
	std::shared_ptr<ShaderGLSL::Uniform> m_pDensity;
	std::shared_ptr<ShaderGLSL::Uniform> m_pWeight;
	std::shared_ptr<ShaderGLSL::Uniform> m_pLightPositionOnScreen;
	//

	unsigned int	m_nCurrentDiffusion;

	enum ERTT { eRTT_Diffusion0, eRTT_Diffusion1, eRTT_Diffusion2, eRTT_FrontLayer, eRTT_BackLayer, eRTT_Max };

	ERTT	m_eDiffusionLast;	// Result of last composition
	ERTT	m_eDiffusionCurr;	// Current composition
	ERTT	m_eDiffusionDest;	// Destination composition

	void	ProcessDiffusionTextures();

	
	//todo: refactor later
	std::map<std::string, CEntity*>	m_mapEntity;
};

#endif//_ENGINE_H
