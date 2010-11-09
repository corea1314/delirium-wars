#ifndef _ENGINE_H
#define _ENGINE_H

#include "SigSlot.h"

class CField;
class CClock;
class CCamera;
class CDebugDraw;

class CEngine
{
public:	
	CEngine();
	virtual ~CEngine();

	const CClock*	GetClock() const { return m_pClock; }
	CCamera*		GetCamera() const { return m_pCamera; }
	CField*			GetField() const { return m_pField; }

	void RenderDebugDraw();

	NEW_SIGNAL1( Update, float );	// delta time
	NEW_SIGNAL0( Render );
	NEW_SIGNAL0( RenderGUI );
	

	NEW_SIGNAL3( Gamepad0, unsigned int, unsigned int, float* );	// button state, axis count, axis values
	NEW_SIGNAL3( Gamepad1, unsigned int, unsigned int, float* );	// button state, axis count, axis values
	NEW_SIGNAL3( Gamepad2, unsigned int, unsigned int, float* );	// button state, axis count, axis values
	NEW_SIGNAL3( Gamepad3, unsigned int, unsigned int, float* );	// button state, axis count, axis values

	NEW_SIGNAL1( LoadFile, const char* ); // filename
		
	NEW_PRIVATE_SIGNAL1( RenderDebug, CDebugDraw* );
	
private:
	CField*		m_pField;
	CClock*		m_pClock;
	CCamera*	m_pCamera;
	CDebugDraw*	m_pDebugDraw;

	//todo: add entity factories here
};

#endif//_ENGINE_H