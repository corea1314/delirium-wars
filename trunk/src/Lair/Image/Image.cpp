#include "Image.h"

#include "picoPNG/picoPNG.h"

#include "../Lair.h"

Image* ImageMan::Get( const std::string& in_szFilename )
{
	Image* pImage = new picoPNG;	//todo: for now it is only supporting the png image format

	if( pImage->Load( in_szFilename ) )
	{	
		Lair::GetLogMan()->Log( "ImageMan", "Loaded image named %s", in_szFilename.c_str() );
		return pImage;
	}
	Lair::GetLogMan()->Log( "ImageMan", "Could not load image named %s", in_szFilename.c_str() );

	delete pImage;
	return 0;
}