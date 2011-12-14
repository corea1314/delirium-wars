#ifndef _POD_PUMP_H
#define _POD_PUMP_H

#include "Engine/Entity.h"

#include "../Pump.h"

class CPodPump : public CEntity
{
	DECLARE_CLASS_TYPE(CPodPump, CEntity, CEntity);

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
