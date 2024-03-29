#ifndef _DEPOSIT_H
#define _DEPOSIT_H

#include "Engine/Entity.h"

#include <set>

class CPump;

class CDeposit : public CEntity
{
	DECLARE_CLASS_TYPE(CDeposit, CEntity, CEntity);

private:
	// set of connected pump rod
	std::set<CPump*>	m_psetPumpRod;

public:
	bool	CanPump()
	{
		//todo
		return true;
	}

	bool	TryPumping( float& io_fAmount )
	{
		// pump requested this amount, return how much it can get (if we can)
		if( CanPump() )
		{
			// modify the amount here, if needed
			return true;
		}

		io_fAmount = 0.0f;

		return false;
	}

	float	GetDepth()
	{
		// return the current depth of the deposit
		// depth being the distance between the uppermost level of deposit and the ground
		return 0.0f;
	}
};

#endif//_DEPOSIT_H
