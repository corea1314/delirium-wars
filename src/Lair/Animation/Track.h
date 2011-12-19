#pragma once

#include <vector>
#include "Key.h"

class Track
{
public:

	void AddKey( Key in_Key );

private:
	std::vector<Key>	m_vecKeys;
};