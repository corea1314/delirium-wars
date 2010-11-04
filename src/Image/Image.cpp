#include "Image.h"

#include "picoPNG/picoPNG.h"

bool ImageMan::Load( Image** out_pImage, std::string in_szFilename )
{
	if( (*out_pImage) == 0 )
	{
		(*out_pImage) = new picoPNG;	//todo: for now it is only supporting the png image format
		return (*out_pImage)->Load( in_szFilename );
	}
	return false;
}

bool ImageMan::Unload( Image** in_pImage )
{
	if( (*in_pImage) != 0 )
	{
		delete (*in_pImage);
		return true;
	}
	return false;
}
