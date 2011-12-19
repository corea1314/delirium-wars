#include "Key.h"


Key::Key( float in_fValue, unsigned int in_nKey )
	: m_fValue(in_fValue)
	, m_nFrame(in_nKey)
{
	m_vIn.Set(1.0f,0.0f);
	m_vOut.Set(-1.0f,0.0f);
}

Key::Key( float in_fValue, unsigned int in_nKey, Vector2 in_vIn, Vector2 in_vOut )
	: m_fValue(in_fValue)
	, m_nFrame(in_nKey)
	, m_vIn(in_vIn)
	, m_vOut(in_vOut)
{
}
