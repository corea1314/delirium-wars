#include "Engine.h"

#include "Field.h"
#include "Clock.h"
#include "Camera.h"

#include "../Lair/Lair.h"

#include "../macros.h"

CEngine::CEngine()
{
	// Create lair
	Lair::Create();

	// Create the clock
	m_pClock = new CClock;
	m_pClock->Connect(this);

	// Create the field
	m_pField = new CField;
	m_pField->Connect(this);

	// Create the camera
	m_pCamera = new CCamera;
	m_pCamera->Connect(this);
}


CEngine::~CEngine()
{
	m_pCamera->Disconnect(this);
	SAFE_DELETE(m_pCamera);

	m_pField->Disconnect(this);
	SAFE_DELETE(m_pField);

	m_pClock->Disconnect(this);
	SAFE_DELETE(m_pClock);

	Lair::Release();
}