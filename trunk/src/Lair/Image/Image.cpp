#include "Image.h"

#include "picoPNG/picoPNG.h"

#include "../Lair.h"

// Image ======================================================================

#define IMAGE_COLOR_KEY_R	255
#define IMAGE_COLOR_KEY_G	0
#define IMAGE_COLOR_KEY_B	255

unsigned char g_pDummyPixelBuffer[2*2*4] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };

Image::Image( unsigned long in_nWidth, unsigned long in_nHeight, unsigned long in_nBytesPerPixel, unsigned char* in_pPixelBuffer ) 
	: m_nWidth(in_nWidth), m_nHeight(in_nHeight), m_nBytesPerPixel(in_nBytesPerPixel)
{
	m_pPixelBuffer = new unsigned char[m_nWidth * m_nHeight * m_nBytesPerPixel];

	if( in_pPixelBuffer )
	{
		memcpy( m_pPixelBuffer, in_pPixelBuffer, m_nWidth * m_nHeight * m_nBytesPerPixel );
	}
}

Image::~Image()
{
	SAFE_DELETE_ARRAY(m_pPixelBuffer);
}


// ImageMan ===================================================================

Image* ImageMan::Get( const std::string& in_szFilename )
{
	Image* pImage;;

	if( ImageLoaderPNG::Load( &pImage, in_szFilename ) )	//todo: for now it is only supporting the png image format
	{	
		Lair::GetLogMan()->Log( "ImageMan", "Loaded image named %s", in_szFilename.c_str() );
		return pImage;
	}
	Lair::GetLogMan()->Log( "ImageMan", "Could not load image named %s", in_szFilename.c_str() );

	delete pImage;
		
	return new Image(2,2,4,g_pDummyPixelBuffer); // return a dummy image if it fails
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

void Image::GetCropRect( int& in_nMinX, int& in_nMinY, int& in_nMaxX, int& in_nMaxY )
{
	in_nMinX = GetWidth();
	in_nMinY = GetHeight();
	in_nMaxX = 0;
	in_nMaxY = 0;

	unsigned char* pBuffer = GetPixelBuffer();
	int x,y;

	for(y=0;y<(int)GetHeight();y++)
	{
		for(x=0;x<(int)GetWidth();x++)
		{
			if( !IsColorKey(pBuffer,GetBytesPerPixel()) )
			{
				in_nMinX = MIN( in_nMinX, x );
				in_nMaxX = MAX( in_nMaxX, x );
				in_nMinY = MIN( in_nMinY, y );
				in_nMaxY = MAX( in_nMaxY, y );
			}
			pBuffer += GetBytesPerPixel();
		}
	}
}

Image* Image::Crop( Image* in_pImage )
{
	int minx,miny,maxx,maxy;
	
	Image::GetCropRect( in_pImage, minx, miny, maxx, maxy );

	int sx = maxx-minx+1, 
		sy = maxy-miny+1;

	Image* pImage = new Image( sx, sy, in_pImage->GetBytesPerPixel() );

	Image::Blit( pImage, 0, 0, in_pImage, minx, miny, sx, sy );

	return pImage;
}

bool Image::IsColorKey( unsigned char* in_pPixel, unsigned long in_nBytesPerPixel )
{
	if( in_nBytesPerPixel == 3 )
		return	in_pPixel[0] == IMAGE_COLOR_KEY_R &&
				in_pPixel[1] == IMAGE_COLOR_KEY_G && 
				in_pPixel[2] == IMAGE_COLOR_KEY_B;
	// else if( in_nBytesPerPixel == 4 )
		return in_pPixel[0] == 0;
}