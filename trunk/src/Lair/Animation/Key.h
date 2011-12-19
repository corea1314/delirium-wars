#pragma once

#include "Math/Vector2.h"

class Key
{
public:
	Key( float in_fValue, unsigned int in_nKey );
	Key( float in_fValue, unsigned int in_nKey, Vector2 in_vIn, Vector2 in_vOut );

private:
	const float m_fValue;
	const unsigned int m_nFrame;
	Vector2 m_vIn;
	Vector2 m_vOut;
};