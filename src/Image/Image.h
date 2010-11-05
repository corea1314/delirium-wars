#ifndef _IMAGE_H
#define _IMAGE_H

#include <string>

class Image
{
public:
	virtual bool Load( const std::string& in_szFilename ) = 0;
	virtual unsigned long GetWidth() const = 0;
	virtual unsigned long GetHeight() const = 0;
	virtual unsigned long GetBytePerPixel()  = 0;
	virtual unsigned char* GetPixelBuffer() = 0;
};

class ImageMan
{
public:
	bool Load( Image** out_pImage, const std::string& in_szFilename );
	bool Unload( Image** in_pImage );

	bool Preload( const std::string& in_szFilename );
};

#endif//_IMAGE_H