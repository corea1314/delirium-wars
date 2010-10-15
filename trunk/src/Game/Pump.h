#ifndef _PUMP_H
#define _PUMP_H

#include "Entity.h"

#include "../Math/Vector2.h"

class CDeposit;
class CTimer;

class CPump : public CEntity
{
private:
	CDeposit*	m_pDeposit;		// Rod can only pump one deposit

	enum State { eDeploying, ePumping };	
	State		m_eState;

	const float		m_fPumpRate;	// mineral pumped per second
	const float		m_fDigRate;		// units per second

	float		m_fCurrentDepositCollected;
	float		m_fDigDepth;

	Vector2		m_vPos;	// origin of the pump


private:
	bool PumpDepthReached();

	void Dig( const CTimer& in_Timer );

	void OnStateDeploying( const CTimer& in_Timer );

	void OnStatePumping( const CTimer& in_Timer );


public:

	float TryCollecting();

	void Deploy( CDeposit* in_pDeposit );

	void OnUpdate( const CTimer& in_Timer );

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );
};

#endif//_PUMP_H
