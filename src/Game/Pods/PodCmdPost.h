#ifndef _POD_CMDPOST_H
#define _POD_CMDPOST_H

#include "../Entity.h"
#include "../Pump.h"

class CPodCommandPost : public CEntity
{
private:
	float		m_fCurrentDepositCollected;
	CPump		m_Pump;

public:
	void OnUpdate( const CTimer& in_Timer )
	{
		m_fCurrentDepositCollected += m_Pump.TryCollecting();
	}

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );
};



#endif//_POD_CMDPOST_H
