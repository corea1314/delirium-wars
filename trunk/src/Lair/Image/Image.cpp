#include "Image.h"

#include "picoPNG/picoPNG.h"
#include "raw/raw.h"

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
	
	return new ImageRaw; // return a dummy image if it fails
}

bool Image::Clip( int& dx, int& dy, int dw, int dh, int& sx, int& sy, int& sw, int& sh )
{
	assert( dx >= 0 && dy >= 0 && "Image::Clip - Parameters out of range." );
	assert( sx >= 0 && sy >= 0 && "Image::Clip - Parameters out of range." );

	// assumed sw and sh are <= than width and height of image

	sx = MAX( 0, sx );		// min: prevent reading outside of src
	sy = MAX( 0, sy );

	dx = MAX( 0, dx );		// min: prevent writing outside of dst
	dy = MAX( 0, dy );

	sw = MIN( sw, dw-dx );	// max: prevent writing outside of dst
	sh = MIN( sh, dh-dy );

	if( sw <= 0 || sh <= 0 )
		return false; // returns null blit rect params

	return true; // returns valid blit rect params
}

bool Image::Blit(	Image* in_pDst, int in_nDstX, int in_nDstY, 
	Image* in_pSrc, int in_nSrcX, int in_nSrcY, int in_nSrcW, int in_nSrcH )
{
	if( !Clip( in_nDstX, in_nDstY, in_pDst->GetWidth(), in_pDst->GetHeight(), in_nSrcX, in_nSrcY, in_nSrcW, in_nSrcH ) )
		return false;

	const int nSrcP = in_pSrc->GetWidth()*in_pSrc->GetBytesPerPixel();
	const int nDstP = in_pDst->GetWidth()*in_pDst->GetBytesPerPixel();

	unsigned char* pDataSrc = in_pSrc->GetPixelBuffer() + (in_nSrcY * nSrcP) + in_nSrcX * in_pSrc->GetBytesPerPixel();
	unsigned char* pDataDst = in_pDst->GetPixelBuffer() + (in_nDstY * nDstP) + in_nDstX * in_pDst->GetBytesPerPixel();

	while(in_nSrcH--)
	{
		memcpy(pDataDst, pDataSrc, in_nSrcW * in_pSrc->GetBytesPerPixel() );

		pDataSrc += nSrcP;
		pDataDst += nDstP;
	}

	return true;
}