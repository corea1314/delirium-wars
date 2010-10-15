#ifndef _POD_PUMP_H
#define _POD_PUMP_H

#include "../Entity.h"

#include "../Pump.h"

class CPodPump : public CEntity
{
private:
	float	m_fCurrentDepositCollected;
	CPump	m_Pump;

public:
	float TryCollecting();
	void OnUpdate( const CTimer& in_Timer );

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );

};


#endif//_POD_PUMP_H
