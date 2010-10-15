#include "Engine.h"

#include "Field.h"
#include "Clock.h"

#include "../macros.h"

CEngine::CEngine()
{
	// Create the clock
	m_pClock = new CClock;
	m_pClock->Connect(this);

	// Create the field
	m_pField = new CField;
	m_pField->Connect(this);
}


CEngine::~CEngine()
{
	m_pField->Disconnect(this);
	SAFE_DELETE(m_pField);

	m_pClock->Disconnect(this);
	SAFE_DELETE(m_pClock);
}