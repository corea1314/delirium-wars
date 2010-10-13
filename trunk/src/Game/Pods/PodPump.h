#ifndef _POD_PUMP_H
#define _POD_PUMP_H

#include "../Entity.h"

class CPodPump : public CEntity
{
private:
	float	m_fCurrentDepositCollected;
	CPump	m_Pump;

public:
	float TryCollecting()
	{
		float fReturn = m_fCurrentDepositCollected;
		m_fCurrentDepositCollected = 0.0f;
		return fReturn;
	}

	void OnUpdate( const CTimer& in_Timer )
	{
		m_fCurrentDepositCollected += m_Pump.TryCollecting();
	}
};


#endif//_POD_PUMP_H
