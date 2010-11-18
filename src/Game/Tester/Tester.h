#ifndef _TESTER_H
#define _TESTER_H

#include "../Entity.h"
#include "../../Math/Vector2.h"

class CEngine;
class CDebugDraw;
class Sprite;
class CPhysicsManager;
class CPhysicsBody;
class CCollisionBodyOnBodyResult;

class CTester : public CEntity
{
public:
	CTester();
    ~CTester();

	void Update( float in_fDeltaTime );
	void Render();
	void RenderDebug( CDebugDraw* );
	void Keyboard( unsigned char in_cKey );

    void BodyOnBodyCollision(CCollisionBodyOnBodyResult* in_pCollisionResult );

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );

    void BuildPhysicsScene();
    void CreatePhysicsFallingBody();

private:
	Vector2		m_vPos;
	CEngine*	m_pEngine;
	float		m_fAngle;
	Sprite*		m_pSprite;
};


#endif//_TESTER_H
