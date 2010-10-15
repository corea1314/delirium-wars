#ifndef _FIELD_H
#define _FIELD_H

#include "SigSlot.h"

class CEngine;

class CField : public has_slots<>
{
public:
	void Update( float in_fDeltaTime );
	void Render();

	virtual void Connect( CEngine* );
	virtual void Disconnect( CEngine* );
};

#endif//_FIELD_H
