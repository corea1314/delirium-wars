#include "Atlas.h"

#include "../Lair.h"


// PACK =======================================================================

Atlas::Pack::Pack( unsigned long w, unsigned long h )
{
	bin = new Bin(w,h);
	img = new Image(w,h,4);	//fixme
	tex = new Texture;
}

Atlas::Pack::~Pack()
{
	delete bin;
	delete img;
	delete tex;
}

void Atlas::Pack::ReloadTexture()
{
	tex->LoadFromImage( img );
}

// ATLAS ======================================================================

Atlas::Atlas( const unsigned long in_nBinWidth, const unsigned long in_nBinHeight ) 
	: m_nBinWidth(in_nBinWidth), m_nBinHeight(in_nBinHeight) 
{	
}

Atlas::Index*	Atlas::InsertFromFile( const std::string& in_szFilename )
{
	Image* pImage;

	if( pImage = Lair::GetImageMan()->Get(in_szFilename) )
	{
		Rect rectCoords;
		m_itCurrPack = m_vecPacks.begin();

		int nMinX, nMinY, nMaxX, nMaxY;
		Image* pCropImage = Image::Crop( pImage, nMinX, nMinY, nMaxX, nMaxY );

		do 
		{
			if( m_itCurrPack == m_vecPacks.end() )
			{
				// add new pack
				m_vecPacks.push_back( new Pack(m_nBinWidth, m_nBinHeight) );
				
				m_itCurrPack = m_vecPacks.end()-1;
				
				Lair::GetLogMan()->Log( "Atlas", "Added new pack on image named %s. Pack count is now %d.", in_szFilename.c_str(), m_vecPacks.size() );
			}

			rectCoords = (*m_itCurrPack)->bin->Insert( pCropImage->GetWidth(), pCropImage->GetHeight(), true, Bin::RectBestShortSideFit, Bin::SplitMinimizeArea );

		} while ( rectCoords.height == 0 );	// if height is 0 then we could not fit it in bin so try next one
		
		return AddImageToPack( (*m_itCurrPack), rectCoords, pImage, nMinX, nMinY, pCropImage );
	}
	
	Lair::GetLogMan()->Log( "Atlas", "Could not load image named %s.", in_szFilename.c_str() );
	return 0;
}

Atlas::Index*	Atlas::AddImageToPack( Atlas::Pack* in_pPack, const Rect& in_rectCoords, Image* in_pImage, int in_nCropOffsetX, int in_nCropOffsetY, Image* in_pCropImage )
{
	Index* pIndex = new Index;

	pIndex->pack = in_pPack;
	pIndex->x = in_rectCoords.x;
	pIndex->y = in_rectCoords.y;
	pIndex->w = in_rectCoords.width;
	pIndex->h = in_rectCoords.height;
	pIndex->cx = in_nCropOffsetX;
	pIndex->cy = in_nCropOffsetY;
	pIndex->cw = in_pCropImage->GetWidth();
	pIndex->ch = in_pCropImage->GetHeight();
	pIndex->rotated = (in_rectCoords.width != in_pImage->GetWidth());

	Image::Blit( in_pPack->img, pIndex->x, pIndex->y, in_pCropImage, 0,0, in_pCropImage->GetWidth(), in_pCropImage->GetHeight() );

	return pIndex;
}

Atlas::Index*	Atlas::Get( const std::string& in_szFilename )
{
	std::map< std::string, Index* >::iterator it = m_mapIndices.find( in_szFilename );

	if( it != m_mapIndices.end() )
	{
		// found it, return it
//		Lair::GetLogMan()->Log( "Atlas", "Loaded index from map (%s).", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it
		Index* pIndex;

		if( pIndex = InsertFromFile( in_szFilename ) )
		{
			m_mapIndices.insert( std::make_pair(in_szFilename,pIndex) );
			Lair::GetLogMan()->Log( "Atlas", "Inserted image named %s into atlas.", in_szFilename.c_str() );
			return pIndex;
		}
		
		Lair::GetLogMan()->Log( "Atlas", "Could insert image named %s in atlas.", in_szFilename.c_str() );
	}
	return 0;

}

void Atlas::ReloadTexture()
{
	for( std::vector<Pack*>::iterator it = m_vecPacks.begin(); it != m_vecPacks.end(); it++ )
	{
		(*it)->ReloadTexture();
	}
}