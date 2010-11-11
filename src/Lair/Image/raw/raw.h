#ifndef _RAW_H
#define _RAW_H

#include "../Image.h"

class ImageRaw : public Image
{
public:
	ImageRaw();
	bool Load( const std::string& in_szFilename );
	unsigned long GetWidth() const { return m_nWidth; }
	unsigned long GetHeight() const { return m_nHeight; }
	unsigned long GetBytePerPixel() { return 4; }	// hardcoded to 32 bits, sorry, picoPNG limitation

	unsigned char* GetPixelBuffer() { return (unsigned char*)&(m_vecPixel[0]); }

private:
	unsigned long m_nWidth;
	unsigned long m_nHeight;
	unsigned int m_vecPixel[2*2];
};

#endif//_RAW_H