#ifndef _TESTER_H
#define _TESTER_H

#include "../Entity.h"
#include "../../Math/Vector2.h"

class CEngine;
class CDebugDraw;

class CTester : public CEntity
{
public:
	CTester();

	void Update( float in_fDeltaTime );

	void RenderDebug( CDebugDraw* );

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );

private:
	Vector2	m_vPos;
	CEngine* m_pEngine;
	float	m_fAngle;
};


#endif//_TESTER_H
