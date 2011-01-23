#ifndef _IMAGE_H
#define _IMAGE_H

#include <string>

//todo: make this class clean. have image man use the proper loader. should never derived from image.

class Image
{
public:
	Image( unsigned long in_nWidth, unsigned long in_nHeight, unsigned long in_nBytesPerPixel, unsigned char* in_pPixelBuffer = 0 );
	virtual ~Image();
	
	bool Load( const std::string& in_szFilename );
	unsigned long GetWidth() const { return m_nWidth; }
	unsigned long GetHeight() const { return m_nHeight; }
	unsigned long GetBytesPerPixel() const { return m_nBytesPerPixel; }
	unsigned char* GetPixelBuffer() const { return m_pPixelBuffer; }

	void GetCropRect( int& out_nMinX, int& out_nMinY, int& out_nMaxX, int& out_nMaxY, int in_nBorder = 0 );

	static bool Clip( int& dx, int& dy, int dw, int dh, int& sx, int& sy, int& sw, int& sh );
	static bool Blit( Image* in_pDst, int in_nDstX, int in_nDstY, 
					  Image* in_pSrc, int in_nSrcX, int in_nSrcY, int in_SrcW, int in_nSrcH );
	static Image* Crop( Image* in_pImage, int& out_nMinX, int& out_nMinY, int& out_nMaxX, int& out_nMaxY, int in_nBorder = 0 );
	static bool IsColorKey( unsigned char* in_pPixel, unsigned long in_nBytesPerPixel );

private:
	unsigned long	m_nWidth;
	unsigned long	m_nHeight;
	unsigned long	m_nBytesPerPixel;	// 3:BGR, 4:ABGR
	unsigned char*	m_pPixelBuffer;
};

class ImageMan
{
public:
	Image* Get( const std::string& in_szFilename );
};

#endif//_IMAGE_H
