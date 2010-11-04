#ifndef _PICOPNG_H
#define _PICOPNG_H

//todo: rename this class properly

#include "../Image.h"

#include <vector>

class picoPNG : public Image
{
public:
	bool Load( std::string in_szFilename );
	unsigned long GetWidth() const { return m_nWidth; }
	unsigned long GetHeight() const { return m_nHeight; }
	unsigned long GetBytePerPixel() { return 4; }	// hardcoded to 32 bits, sorry, picoPNG limitation

	unsigned char* GetPixelBuffer() { return (unsigned char*)&(m_vecPixel[0]); }

private:
	unsigned long m_nWidth;
	unsigned long m_nHeight;
	std::vector<unsigned char> m_vecPixel;	//todo: sad sad implementation of picopng forces this vector
};

#endif//_PICOPNG_H