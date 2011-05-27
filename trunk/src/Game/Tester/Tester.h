#ifndef _TESTER_H
#define _TESTER_H

#include "../../Engine/Entity.h"
#include "../../Math/Vector2.h"

class CEngine;
class CDebugDraw;
class SpriteAnimator;
class CPhysicsManager;
class CPhysicsBody;
class CCollisionBodyOnBodyResult;
class Atlas;

class CTester : public CEntity
{
	DECLARE_CLASS_TYPE(CTester, CEntity, CEntity);

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
	float		m_fAngle;
	SpriteAnimator*		m_pSprite;
	SpriteAnimator*		m_pSpriteDiffusion;

};


#endif//_TESTER_H
