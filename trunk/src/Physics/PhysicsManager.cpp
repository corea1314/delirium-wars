#include "PhysicsManager.h"

#include "PhysicsBody.h"
#include "Game/Field.h"
#include "Engine/Engine.h"
#include "DebugDraw/DebugDraw.h"
#include <vector>

CPhysicsManager::CPhysicsManager(float in_fTimeResolution , int in_mMaximumSubStep)
:m_fTimeResolution(in_fTimeResolution),
m_nMaxSubStep(in_mMaximumSubStep),
m_fTimeAccumulator(0.0f),
m_pGrid(NULL),
m_pEngineProxy(m_pEngineProxy),
m_unNbCollision(0)
{
}
CPhysicsManager::~CPhysicsManager()
{
    void DestroyAllBody();
}

void CPhysicsManager::Update( float in_fDeltaTime )
{
	int nSubstepCounter = 0;
	m_fTimeAccumulator += in_fDeltaTime;

	float fTimeLeft = fmodf(m_fTimeAccumulator,m_fTimeResolution);
	float fFraction = fTimeLeft / m_fTimeResolution;
	//Do not want to use modf and convert from float to int, so I iterate using m_fTimeAccumulator

    if( m_fTimeAccumulator > GetTimeResolution() )
    {
	    while( m_fTimeAccumulator > GetTimeResolution() && nSubstepCounter != GetMaximumSubStep() )
	    {
		    PhysicsSubStep( m_fTimeResolution, fFraction );
		    m_fTimeAccumulator -= m_fTimeResolution;
		    nSubstepCounter++;
	    }
    }else
    {
        ComputeInterpolationAndSync(fFraction);
    }
	//Force time accumulator to the time left so in fact, we do get late in the simulation if update is taking too long)
	m_fTimeAccumulator = fTimeLeft;
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
		if( pBody->IsSimulationEnable() )
		{
            Vector2 v2OldPosition( pBody->GetPhysicsPosition() );
            //Simplest integration computation. We can refine this later...
            Vector2 v2impulseToApply( (pBody->GetForce() + pBody->GetGravity() * pBody->GetMass()) * in_fDeltaTime + pBody->GetImpulse()  );
            Vector2 v2DeltaVelocity( v2impulseToApply * pBody->GetInverseMass() );
            pBody->SetVelocity( v2DeltaVelocity + m_vPhysicsBody[i]->GetVelocity() );
            //x(new) = x(old) + v * dt + 1/2 * F * dt^2
            Vector2 v2DeltaPosition( (pBody->GetVelocity() + 0.5f * v2DeltaVelocity ) * in_fDeltaTime);
            pBody->SetLastPhysicsPosition( pBody->GetPhysicsPosition() );
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
                    CPhysicsBody* pCallingBody = m_vActivePhysicsBody[i];
                    CPhysicsBody* pForeignBody = m_vActivePhysicsBody[j];
					//Try colliding body i and j with aab
					Vector2 v2CallingBodyLower( pCallingBody->GetPhysicsPosition() - pCallingBody->GetHalfShape() );
					Vector2 v2CallingBodyHigher( pCallingBody->GetPhysicsPosition() + pCallingBody->GetHalfShape() );
					Vector2 v2ForeignBodyLower( pForeignBody->GetPhysicsPosition() - pForeignBody->GetHalfShape() );
					Vector2 v2ForeignBodyHigher( pForeignBody->GetPhysicsPosition() + pForeignBody->GetHalfShape() );
					//Assume most of the time bodies will not collide
					if( (v2CallingBodyHigher.x > v2ForeignBodyLower.x && v2CallingBodyLower.x < v2ForeignBodyHigher.x) || (v2ForeignBodyHigher.x > v2CallingBodyLower.x && v2ForeignBodyLower.x < v2CallingBodyHigher.x) )
					{
						if( (v2CallingBodyHigher.y > v2ForeignBodyLower.y && v2CallingBodyLower.y < v2ForeignBodyHigher.y) || (v2ForeignBodyHigher.y > v2CallingBodyLower.y && v2ForeignBodyLower.y < v2CallingBodyHigher.y) )
						{
							//We collide! Cheezus!
							//todo: have dynamically extendable pool of collision results and a cache to have collision lifetime.
							CCollisionBodyOnBodyResult* pNewResult = new CCollisionBodyOnBodyResult();

                            Vector2 v2SeparationVector;
							bool bCallingLeft = pCallingBody->GetPhysicsPosition().x < pForeignBody->GetPhysicsPosition().x;
							bool bCallingHigher = pCallingBody->GetPhysicsPosition().y > pForeignBody->GetPhysicsPosition().y;
                            //Todo: Might need to be more precise here. Take account of the last position to have a more precise separation vector
                            //Separation goes away from calling body.
                            if( bCallingLeft )
                            {
                               v2SeparationVector.x = v2CallingBodyHigher.x- v2ForeignBodyLower.x;
                            }else
                            {
                               v2SeparationVector.x = v2CallingBodyLower.x - v2ForeignBodyHigher.x;
                            }

                            if( bCallingHigher )
                            {
                                v2SeparationVector.y = v2CallingBodyLower.y - v2ForeignBodyHigher.y;
                            }else
                            {
                                v2SeparationVector.y = v2CallingBodyHigher.y - v2ForeignBodyLower.y;
                            }

                            if( v2SeparationVector.x != 0.0f || v2SeparationVector.y != 0.0f )
                            {
                                //Take the shortest distance for now... assume no tunneling :-/
                                if( abs(v2SeparationVector.x) < abs(v2SeparationVector.y) )
                                {
                                    v2SeparationVector.y = 0;
                                }else
                                {
                                    v2SeparationVector.x = 0;
                                }
                            }

                            if( v2SeparationVector.x != 0.0f && v2SeparationVector.y != 0.0f )
                            {
                                assert("CPhysicsManager::HandleCollisionBodyOnBody - Separation vector is null!");
                            }

                            double dDistance =  Vector2::Normalize(v2SeparationVector);

							pNewResult->SetCallingPhysicsBody( pCallingBody );
							pNewResult->SetForeignPhysicsBody( pForeignBody );
                            pNewResult->SetNormal( v2SeparationVector );
                            pNewResult->SetSeparationDistance( static_cast<float>(dDistance) );
							vBodyOnBody.push_back( pNewResult );
						}
					}
				}

			}
		}

	}
    m_unNbCollision = vBodyOnBody.size();

	//All collision are handled. now call the callbacks
	//OnCollision
	for( i = 0; i < vBodyOnBody.size(); ++i)
	{
		vBodyOnBody[i]->GetCallingPhysicsBody()->PhysicsBodyOnBodyCollision( vBodyOnBody[i] );
        vBodyOnBody[i]->SwapBodyInformation();
        vBodyOnBody[i]->GetCallingPhysicsBody()->PhysicsBodyOnBodyCollision( vBodyOnBody[i] );

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
                       CCollisionBodyOnGridResult* pNewCollisionResult = new CCollisionBodyOnGridResult();
                       vBodyOnGridCollide.push_back( pNewCollisionResult );
                       pNewCollisionResult->SetPhysicsBody( m_vActivePhysicsBody[i] );
                       pNewCollisionResult->SetFromX(unLeft);
                       pNewCollisionResult->SetToX(unRight);
                       pNewCollisionResult->SetFromY(unDown);
                       pNewCollisionResult->SetToY(unUp);
                    }
                }      
            }
        }

        for( i = 0; i < vBodyOnGridCollide.size(); ++i)
        {
            vBodyOnGridCollide[i]->GetPhysicsBody()->PhysicsBodyOnGridCollision( vBodyOnGridCollide[i] );
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
    m_vActivePhysicsBody.clear();
}

void CPhysicsManager::ComputeInterpolationAndSync( float in_fInterpolationRatio )
{
    unsigned int i;
    //PreSync
    for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
    {
        m_vActivePhysicsBody[i]->PhysicsBodyOnPreSync(m_vPhysicsBody[i]);
    }


    for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
    {
        CPhysicsBody* pBody = m_vActivePhysicsBody[i];
        if( pBody->IsSimulationEnable() )
        {
            //Compute interpolation
            pBody->SetInterpolatedPosition( pBody->GetLastPhysicsPosition() + in_fInterpolationRatio * ( pBody->GetPhysicsPosition() - pBody->GetLastPhysicsPosition() ) );
        }
    }

    //PostSync
    for( i = 0; i < m_vActivePhysicsBody.size(); ++i)
    {
        m_vActivePhysicsBody[i]->PhysicsBodyOnPostSync(m_vPhysicsBody[i]);
    }
}

void CPhysicsManager::RenderDebug( CDebugDraw* in_pDebugDrawer )
{
    unsigned int i;
    for( i = 0; i < m_vActivePhysicsBody.size(); ++i )
    {
        CPhysicsBody* pPhysicsBody = m_vActivePhysicsBody[i];
        //When everything is on, it is with
        Color oColor = CDebugDraw::Color::eGREY;

        if( !pPhysicsBody->IsCollisionEnable() )
        {
            oColor -= 0x007F0000UL;
        }

        if( !pPhysicsBody->IsSimulationEnable() )
        {
            oColor -= 0x00007F00UL;
        }

        if( !pPhysicsBody->IsCollideWithGrid() )
        {
            oColor -= 0x0000007FUL;
        }
       in_pDebugDrawer->SetColor( oColor );
       in_pDebugDrawer->DrawRectangle( pPhysicsBody->GetPhysicsPosition(), pPhysicsBody->GetShape().x, pPhysicsBody->GetShape().y, 0.0f );
    }

    in_pDebugDrawer->Text( -600, 350, "Active: %u",m_vActivePhysicsBody.size());
    in_pDebugDrawer->Text( -600, 330, "Total: %u",m_vPhysicsBody.size());
    in_pDebugDrawer->Text( -600, 310, "Collision: %u",m_unNbCollision);
}

void CPhysicsManager::Connect( CEngineProxy* in_pEngineProxy )
{
    m_pEngineProxy = in_pEngineProxy;
    m_pEngineProxy->Connect_OnRenderDebug( this, &CPhysicsManager::RenderDebug );
    m_pEngineProxy->Connect_OnUpdate( this, &CPhysicsManager::Update );
}

void CPhysicsManager::Disconnect()
{
    m_pEngineProxy->Disconnect_OnUpdate( this );
    m_pEngineProxy->Disconnect_OnRenderDebug( this );
    m_pEngineProxy = NULL;
}