#ifndef _IMAGE_H
#define _IMAGE_H

#include <string>

//todo: make this class clean. have image man use the proper loader. should never derived from image.

class Image
{
public:
	Image() {}
	Image( unsigned long in_nWidth, unsigned long in_nHeight, unsigned long in_nBytePerPixel ) {}
	
	virtual bool Load( const std::string& in_szFilename );
	virtual unsigned long GetWidth() const;
	virtual unsigned long GetHeight() const;
	virtual unsigned long GetBytesPerPixel();
	virtual unsigned char* GetPixelBuffer();

	static bool Clip( int& dx, int& dy, int dw, int dh, int& sx, int& sy, int& sw, int& sh );
	static bool Blit( Image* in_pDst, int in_nDstX, int in_nDstY, 
					  Image* in_pSrc, int in_nSrcX, int in_nSrcY, int in_SrcW, int in_nSrcH );
};

class ImageMan
{
public:
	Image* Get( const std::string& in_szFilename );
};

#endif//_IMAGE_H
