
#include "PhysicsBody.h"

CPhysicsBody::CPhysicsBody( const char * in_szName )
:m_strName(in_szName),
m_nMyMask(-1), //All
m_nCollideWithMask(-1), //All
m_v2Shape(1.0f,1.0f),
m_v2HalfShape(0.5f,0.5f),
m_v2ForceAccumulator(Vector2(0.0f,0.0f)),
m_v2ImpulseAccumulator(Vector2(0.0f,0.0f)),
m_v2Gravity(Vector2(0.0f,0.0f)),
m_v2Velocity(Vector2(0.0f,0.0f)),
m_v2PhysicsPosition(Vector2(0.0f,0.0f)),
m_v2InterpolatedPosition(Vector2(0.0f,0.0f)),
m_fMass(1.0f),
m_fInverseMass(1.0f),
m_bIsSimulationEnable(true),
m_bIsCollisionEnable(true),
m_bIsActive(true),
m_bCollideWithGrid(true)
{
}


CPhysicsBody::~CPhysicsBody()
{

}

void CPhysicsBody::SetMass( float in_fMass)
{
	if( in_fMass <= 0.0000f )
	{
		SetInfiniteMass();
	}else
	{
		m_fMass = in_fMass;
		m_fInverseMass = 1 / in_fMass;
	}
/*
    //Draw debug
    if(  IsActive() )
    {
        Vector2 v2Lower = m_v2PhysicsPosition - m_v2HalfShape;
        Vector2 v2Higher = m_v2PhysicsPosition + m_v2HalfShape;

        //When everything is on, it is with
        Vector3 v3Color(0.5f,0.5f,0.5f);

        if( !IsCollisionEnable() )
        {
            v3Color.x = 1.0f;
        }

        if( !IsSimulationEnable() )
        {
            v3Color.y = 1.0f;
        }

        if( !IsCollideWithGrid )
        {
            v3Color.z = 1.0f;
        }
    }*/
}

