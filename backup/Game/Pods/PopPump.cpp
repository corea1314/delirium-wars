#include "PodPump.h"

IMPLEMENT_CLASS_TYPE(CPodPump)

float CPodPump::TryCollecting()
{
	float fReturn = m_fCurrentDepositCollected;
	m_fCurrentDepositCollected = 0.0f;
	return fReturn;
}

void CPodPump::OnUpdate( const CTimer& in_Timer )
{
	m_fCurrentDepositCollected += m_Pump.TryCollecting();
}
