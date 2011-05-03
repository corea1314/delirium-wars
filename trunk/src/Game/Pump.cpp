#include "Pump.h"
#include "Deposit.h"
#include "Engine/Entities/Clock/Timer.h"


bool CPump::PumpDepthReached()
{
	return false;
}

void CPump::Dig( const CTimer& in_Timer )
{
	m_fDigDepth += m_fDigRate * in_Timer.GetDeltaTime();
}

void CPump::OnStateDeploying( const CTimer& in_Timer )
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

void CPump::OnStatePumping( const CTimer& in_Timer )
{
	// in this state, we pump the mineral deposit

	float fWishPump = in_Timer.GetDeltaTime() * m_fPumpRate;
	if( m_pDeposit->TryPumping( fWishPump ) )
		m_fCurrentDepositCollected += fWishPump;
}


float CPump::TryCollecting()
{
	float fReturn = m_fCurrentDepositCollected;
	m_fCurrentDepositCollected = 0.0f;
	return fReturn;
}

void CPump::Deploy( CDeposit* in_pDeposit )
{
	m_pDeposit = in_pDeposit;
	m_eState = eDeploying;
}

void CPump::OnUpdate( const CTimer& in_Timer )
{
	switch( m_eState )
	{
	case eDeploying:	OnStateDeploying( in_Timer );	break;
	case ePumping:		OnStatePumping( in_Timer );		break;
	}
}
