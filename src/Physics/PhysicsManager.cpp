#include "PhysicsManager.h"

#include "PhysicsBody.h"
#include "../Game/Field.h"

CPhysicsManager::CPhysicsManager( float in_fTimeResolution , int in_mMaximumSubStep)
:m_fTimeResolution(in_fTimeResolution),
m_nMaxSubStep(in_mMaximumSubStep),
m_fTimeAccumulator(0.0f),
m_pGrid(NULL)
{

}
CPhysicsManager::~CPhysicsManager()
{

}

int CPhysicsManager::Update( float in_fDeltaTime )
{
	int nSubstepCounter = 0;
	m_fTimeAccumulator += in_fDeltaTime;

	float fTimeLeft = fmodf(m_fTimeAccumulator,in_fDeltaTime);
	float fFraction = fTimeLeft / in_fDeltaTime;
	//Do not want to use modf and convert from float to int, so I iterate using m_fTimeAccumulator

	while( m_fTimeAccumulator > GetTimeResolution() && nSubstepCounter <= GetMaximumSubStep() )
	{
		PhysicsSubStep( in_fDeltaTime, in_fDeltaTime );
		m_fTimeAccumulator -= in_fDeltaTime;
		nSubstepCounter++;
	}

	//Force time accumulator to the time left so in fact, we do get late in the simulation if update is taking too long)
	m_fTimeAccumulator = fTimeLeft;
	return nSubstepCounter;
}

void CPhysicsManager::PhysicsSubStep( float in_fDeltaTime, float in_fInterpolationRatio)
{
	unsigned int i;
	//First, update the list of body that are active
	m_vActivePhysicsBody.clear();

	for( i = 0; i < m_vPhysicsBody.size(); ++i)
	{
		if( m_vPhysicsBody[i]->IsActive() )
		{
			m_vActivePhysicsBody.push_back( m_vPhysicsBody[i] );
		}
	}

	//Pre-substep
	for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
	{
		m_vActivePhysicsBody[i]->PhysicsBodyOnPreSubStep(m_vPhysicsBody[i]);
	}
	//PreSync
	for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
	{
		m_vActivePhysicsBody[i]->PhysicsBodyOnPreSync(m_vPhysicsBody[i]);
	}

	ApplyPhysics(in_fDeltaTime,in_fInterpolationRatio);
	//ComputeInterpolatedPosition();
	HandleCollisionBodyOnBody();
    HandleCollisionBodyOnGrid();

	//PostSync
	for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
	{
		m_vActivePhysicsBody[i]->PhysicsBodyOnPostSync(m_vPhysicsBody[i]);
	}

	//Post-substep
	for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
	{
		m_vActivePhysicsBody[i]->PhysicsBodyOnPostSubStep(m_vPhysicsBody[i]);
	}

}

void CPhysicsManager::ApplyPhysics( float in_fDeltaTime, float in_fInterpolationRatio )
{
	unsigned int i;
	for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
	{
		CPhysicsBody* pBody = m_vPhysicsBody[i];
		if( pBody->IsPhysicsEnable() )
		{
			Vector2 v2OldPosition( pBody->GetPhysicsPosition() );
			//Simplest integration computation. We can refine this later...
			Vector2 v2impulseToApply( pBody->GetForce() * in_fDeltaTime + pBody->GetImpulse() );
			Vector2 v2DeltaVelocity( pBody->GetInverseMass() * v2impulseToApply + pBody->GetGravity() * in_fDeltaTime );
			pBody->SetVelocity( v2DeltaVelocity + m_vPhysicsBody[i]->GetVelocity() );
			Vector2 v2DeltaPosition(v2DeltaVelocity * in_fDeltaTime);
			pBody->TranslatePhysicsPosition( v2DeltaPosition );

			//Compute interpolation
			pBody->SetInterpolatedPosition( v2OldPosition + in_fInterpolationRatio * v2DeltaPosition );

			//Reset force and impulse accumulator
			pBody->ResetAccumulator();
		}
	}
}

void CPhysicsManager::HandleCollisionBodyOnBody()
{
	unsigned int i,j;
	std::vector<CCollisionBodyOnBodyResult*> vBodyOnBody;

	//test in a n^2 fashion the bodies
	//Collision is hardcoded to be square-square, but we could eventually encapsulate shapes in a object and use strategy
	for( i = 0; i < m_vActivePhysicsBody.size()-1; ++i )
	{
		if( m_vActivePhysicsBody[i]->IsCollisionEnable() )
		{
			for( j = i + 1; j < m_vActivePhysicsBody.size(); ++j )
			{
				if( m_vActivePhysicsBody[j]->IsCollisionEnable() && ((m_vActivePhysicsBody[i]->GetMyMask() & m_vActivePhysicsBody[j]->GetCollideWithMask()) != 0) && ((m_vActivePhysicsBody[j]->GetMyMask() & m_vActivePhysicsBody[i]->GetCollideWithMask()) != 0))
				{
					//Try colliding body i and j with aab
					Vector2 v2FirstBodyLower( m_vActivePhysicsBody[i]->GetPhysicsPosition() - m_vActivePhysicsBody[i]->GetHalfShape() );
					Vector2 v2FirstBodyHigher( m_vActivePhysicsBody[i]->GetPhysicsPosition() + m_vActivePhysicsBody[i]->GetHalfShape() );
					Vector2 v2SecondBodyLower( m_vActivePhysicsBody[j]->GetPhysicsPosition() - m_vActivePhysicsBody[j]->GetHalfShape() );
					Vector2 v2SecondBodyHigher( m_vActivePhysicsBody[j]->GetPhysicsPosition() + m_vActivePhysicsBody[j]->GetHalfShape() );
					//Assume most of the time bodies will not collide
					if( (v2FirstBodyHigher.x >= v2SecondBodyLower.x && v2FirstBodyLower.x <= v2SecondBodyHigher.x) || (v2SecondBodyHigher.x >= v2FirstBodyLower.x && v2SecondBodyLower.x <= v2FirstBodyHigher.x) )
					{
						if( (v2FirstBodyHigher.y >= v2SecondBodyLower.y && v2FirstBodyLower.y <= v2SecondBodyHigher.y) || (v2SecondBodyHigher.y >= v2FirstBodyLower.y && v2SecondBodyLower.y <= v2FirstBodyHigher.y) )
						{
							//We collide! Cheezus!
							//todo: have dynamically extendable pool of collision results and a cache to have collision lifetime.
							CCollisionBodyOnBodyResult* pNewResult = new CCollisionBodyOnBodyResult();

							bool bFirstLeft = m_vActivePhysicsBody[i]->GetPhysicsPosition().x < m_vActivePhysicsBody[j]->GetPhysicsPosition().x;
							bool bFirstHigher = m_vActivePhysicsBody[i]->GetPhysicsPosition().y < m_vActivePhysicsBody[j]->GetPhysicsPosition().y;
							/*
							if( bFirstLeft )
							{
								if( bFirstHigher ) //left/higher
								{



								}else //left-lower
								{

								}
							}
							else
							{
								if( bFirstHigher )//right-lower
								{

								}else//right-higher
								{

								}

							}*/

							//Compute the normal. It goes away from the first body for now.
							//will do later
							//pNewResult->SetContactPoint();
							//pNewResult->SetNormal();
							pNewResult->SetListenerPhysicsBody( m_vActivePhysicsBody[i] );
							pNewResult->SetForeignPhysicsBody( m_vActivePhysicsBody[i] );
							vBodyOnBody.push_back( pNewResult );
						}
					}
				}

			}
		}

	}

	//All collision are handled. now call the callbacks
	//OnCollision
	for( i = 0; i < vBodyOnBody.size(); ++i)
	{
		vBodyOnBody[i]->GetListenerPhysicsBody()->PhysicsBodyOnBodyCollision( vBodyOnBody[i], vBodyOnBody[i]->GetListenerPhysicsBody() );

		//swapping body info
		CPhysicsBody* pTmp = vBodyOnBody[i]->GetListenerPhysicsBody();
		vBodyOnBody[i]->SetListenerPhysicsBody( vBodyOnBody[i]->GetForeignPhysicsBody() );
		vBodyOnBody[i]->SetForeignPhysicsBody( pTmp );

		vBodyOnBody[i]->GetListenerPhysicsBody()->PhysicsBodyOnBodyCollision( vBodyOnBody[i],vBodyOnBody[i]->GetListenerPhysicsBody() );

		//clean up the temporary structure while being here
		SAFE_DELETE( vBodyOnBody[i] );
	}

	//Clean up the temporary structure
	vBodyOnBody.clear();
}


void CPhysicsManager::HandleCollisionBodyOnGrid()
{

    if( m_pGrid != NULL )
    {
        unsigned int i;
        std::vector<CCollisionBodyOnGridResult*> vBodyOnGridCollide;

        for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
        {
            if( m_vActivePhysicsBody[i]->IsCollideWithGrid() )
            {
                //Test with the grid and fill result
                unsigned int unLeft = 0;
                unsigned int unRight = 0;
                unsigned int unDown = 0;
                unsigned int unUp = 0;

                Vector2 v2BodyLower( m_vActivePhysicsBody[i]->GetPhysicsPosition() - m_vActivePhysicsBody[i]->GetHalfShape() );
                Vector2 v2BodyHigher( m_vActivePhysicsBody[i]->GetPhysicsPosition() + m_vActivePhysicsBody[i]->GetHalfShape() );
                Vector2 v2GridLower( m_pGrid->GetPosition() );
                Vector2 v2GridHigher( m_pGrid->GetPosition() + m_pGrid->GetGridWorldSize() );

                //redo that!!!1111oneone
                //Does the grid ans the body overlap? If yes, we gotta see wassup.
                if( (v2BodyHigher.x >= v2GridLower.x && v2BodyLower.x <= v2GridHigher.x) || (v2GridHigher.x >= v2BodyLower.x && v2GridLower.x <= v2BodyHigher.x) )
                {
                    if( (v2BodyHigher.y >= v2GridLower.y && v2BodyLower.y <= v2GridHigher.y) || (v2GridHigher.y >= v2GridLower.y && v2GridLower.y <= v2BodyHigher.y) )
                    {
                       if( !m_pGrid->WorldPositionToIndexHorizontal(v2BodyLower.x,unLeft) )
                       {
                            unLeft = 0 ;
                       }

                       if( !m_pGrid->WorldPositionToIndexHorizontal(v2BodyHigher.x,unRight) )
                       {
                           unRight = m_pGrid->GetGridSizeX()-1 ;
                       }

                       if( !m_pGrid->WorldPositionToIndexVertical(v2BodyLower.y,unDown) )
                       {
                           unDown = 0 ;
                       }

                       if( !m_pGrid->WorldPositionToIndexVertical(v2BodyHigher.y,unUp) )
                       {
                           unUp = m_pGrid->GetGridSizeY()-1 ;
                       }

                       //Todo:: fill the collision result structure
                    }
                }      
            }
        }

        for( i = 0; i < vBodyOnGridCollide.size(); ++i)
        {
            vBodyOnGridCollide[i]->GetPhysicsBody()->PhysicsBodyOnGridCollision( vBodyOnGridCollide[i], vBodyOnGridCollide[i]->GetPhysicsBody() );
            //Delete the temporary structure while being here
            SAFE_DELETE( vBodyOnGridCollide[i] );
        }
        //Clean up the temporary structure
        vBodyOnGridCollide.clear();
    }
}

CPhysicsBody* CPhysicsManager::CreatePhysicsBody(const char * in_szName)
{

	CPhysicsBody* pNewBody = new CPhysicsBody(in_szName);
	m_vPhysicsBody.push_back(pNewBody);
	return pNewBody;
}

CPhysicsBody* CPhysicsManager::GetPhysicsBody( int in_nIndex ) const
{
	CPhysicsBody* preturn = NULL;
	if( preturn != NULL)
	{
		preturn = m_vPhysicsBody[in_nIndex];
	}
	return preturn;
}

void CPhysicsManager::DestroyPhysicsbody( CPhysicsBody* in_pPhysicsbody )
{
	std::vector<CPhysicsBody*>::iterator i = m_vPhysicsBody.begin();
	for(;i != m_vPhysicsBody.end(); ++i)
	{
		if( (*i) == in_pPhysicsbody )
		{
			SAFE_DELETE(*i);
			m_vPhysicsBody.erase(i);
			break;
		}
	}
}

void CPhysicsManager::DestroyAllBody()
{
	unsigned int i = 0;
	for( ; i < m_vPhysicsBody.size(); ++i)
	{
		SAFE_DELETE( m_vPhysicsBody[i] );
	}
	m_vPhysicsBody.clear();
}

