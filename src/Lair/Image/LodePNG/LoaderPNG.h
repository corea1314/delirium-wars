#ifndef _LOADERPNG_H
#define _LOADERPNG_H

//todo: rename this class properly

#include <string>

class Image;

class ImageLoaderPNG
{
public:
	static bool Load( Image** out_pImage, const std::string& in_szFilename );
};

#endif//_LOADERPNG_H