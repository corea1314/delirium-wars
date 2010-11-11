
#include "raw.h"

ImageRaw::ImageRaw() : m_nWidth(0), m_nHeight(0)
{
}

bool ImageRaw::Load( const std::string& in_szFilename )
{
	m_vecPixel[0] = 0xffffffff;
	m_vecPixel[1] = 0xffffffff;
	m_vecPixel[2] = 0xffffffff;
	m_vecPixel[3] = 0xffffffff;

	return true;
}