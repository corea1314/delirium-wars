
#ifndef _ENTITY_H
#define _ENTITY_H

#include "Entity.h"

class CPumpRod;


class CPod : public CEntity
{
};

class CPumpRod : public CEntity
{
private:
	CDeposit*	m_pDeposit;		// Rod can only pump one deposit

	enum State { eDeploying, ePumping };	
	State		m_eState;

	const float		m_fPumpRate;	// mineral pumped per second
	const float		m_fDigRate;		// units per second

	float		m_fCurrentDepositCollected;
	float		m_fDigDepth;

	Vector2		m_vPos;	// origin of the pump rod


private:
	bool PumpDepthReached()
	{
		if( m_vPos.y - m_fDigDepth < m_pDeposit->GetExtent().vMax.y )
			return true;

		return false;
	}

	void Dig( const CTimer& in_Timer )
	{
		m_fDigDepth += m_fDigRate * in_Timer.GetDeltaTime();
	}

	void OnStateDeploying( const CTimer& in_Timer )
	{
		// in this state we have the rod move down to the mineral deposit

		if( PumpDepthReached() )
		{
			Dig( in_Timer );
		}
		else
		{
			// seems we reached the deposit
			m_eState = ePumping;
		}
	}

	void OnStatePumping( const CTimer& in_Timer )
	{
		// in this state, we pump the mineral deposit

		float fWishPump = in_Timer.GetDeltaTime() * m_fPumpRate;
		if( m_pDeposit->TryPumping( fWishPump ) )
			m_fCurrentDepositCollected += fWishPump;
	}


public:

	float TryCollecting()
	{
		float fReturn = m_fCurrentDepositCollected;
		m_fCurrentDepositCollected = 0.0f;
		return fReturn;
	}

	void Deploy( CDeposit* in_pDeposit )
	{
		m_pDeposit = in_pDeposit;
		m_eState = eDeploying;
	}

	void OnUpdate( const CTimer& in_Timer )
	{
		switch( m_eState )
		{
		case eDeploying:	OnStateDeploying( in_Timer );	break;
		case ePumping:		OnStatePumping( in_Timer );		break;
		}
	}
};


class CPodPump : public CEntity
{
private:
	float		m_fCurrentDepositCollected;
	CPumpRod	m_PumpRod;

public:
	float TryCollecting()
	{
		float fReturn = m_fCurrentDepositCollected;
		m_fCurrentDepositCollected = 0.0f;
		return fReturn;
	}

	void OnUpdate( const CTimer& in_Timer )
	{
		m_fCurrentDepositCollected += m_PumpRod.TryCollecting();
	}
};

class CPodHQ : public CEntity
{
private:
	float		m_fCurrentDepositCollected;
	CPumpRod	m_PumpRod;

public:
	void OnUpdate( const CTimer& in_Timer )
	{
		m_fCurrentDepositCollected += m_PumpRod.TryCollecting();
	}
};

#endif//_ENTITY_H
