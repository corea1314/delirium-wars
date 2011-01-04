#ifndef _PICOPNG_H
#define _PICOPNG_H

//todo: rename this class properly

class Image;

class ImageLoaderPNG
{
public:
	static bool Load( Image** out_pImage, const std::string& in_szFilename );
};

#endif//_PICOPNG_H