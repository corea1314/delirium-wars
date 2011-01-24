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
class Atlas;

class CTester : public CEntity
{
public:
	CTester();
    ~CTester();

	virtual void Update( float in_fDeltaTime );
	virtual void RenderFrontLayer();
	virtual void RenderBackLayer();
	virtual void RenderDiffusionLayer();
	virtual void RenderDebug( CDebugDraw* );
		
	virtual void Keyboard( unsigned char in_cKey );

    void BodyOnBodyCollision(CCollisionBodyOnBodyResult* in_pCollisionResult );

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );

    void BuildPhysicsScene();
    void CreatePhysicsFallingBody();

protected:
	Vector2		m_vPos;
	CEngine*	m_pEngine;
	float		m_fAngle;
	Sprite*		m_pSprite;
	Sprite*		m_pSpriteDiffusion;

};


#endif//_TESTER_H
