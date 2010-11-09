#ifndef _ENGINE_H
#define _ENGINE_H

#include "SigSlot.h"

class CField;
class CClock;
class CCamera;
class CPhysicsManager;
class CDebugDraw;


//Objects should use the proxy when trying to connect with it
class CEngineProxy
{
    public :
        CEngineProxy(){}
        ~CEngineProxy(){}

    NEW_SIGNAL1( Update, float );	// delta time
    NEW_SIGNAL0( Render );
    NEW_SIGNAL0( RenderGUI );

    NEW_SIGNAL3( Gamepad0, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( Gamepad1, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( Gamepad2, unsigned int, unsigned int, float* );	// button state, axis count, axis values
    NEW_SIGNAL3( Gamepad3, unsigned int, unsigned int, float* );	// button state, axis count, axis values

    NEW_SIGNAL1( LoadFile, const char* ); // filename

    NEW_PRIVATE_SIGNAL1( RenderDebug, CDebugDraw* );
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
	void RenderDebugDraw();
	
private:
	CField*		m_pField;
	CClock*		m_pClock;
	CCamera*	m_pCamera;
    CPhysicsManager* m_pPhysMan;
	CDebugDraw*	m_pDebugDraw;

	//todo: add entity factories here
};

#endif//_ENGINE_H