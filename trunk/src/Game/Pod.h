
#ifndef _ENTITY_H
#define _ENTITY_H

#include "Entity.h"

class CPumpRod;


class CPod : public CEntity
{
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
