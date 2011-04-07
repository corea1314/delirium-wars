#include "Atlas.h"

#include "../Lair.h"

#include "Packing/GuillotineBinPack.h"

typedef GuillotineBinPack Bin;	// using guillotine

/*
class Image;
class Texture;

#include "../../Math/Vector2.h"

#include <map>
#include <vector>
*/

class AtlasPack
{
public:
	AtlasPack( unsigned long w, unsigned long h );
	virtual ~AtlasPack();

	void ReloadTexture();

	Bin*		bin;
	Image*		img;
	Texture*	tex;	// fixme, find a better place
};


class Atlas
{
private:
	const unsigned long m_nBinWidth;	// maximal width of the bins
	const unsigned long m_nBinHeight;	// maximal height of the bins

	std::vector<AtlasPack*>	m_vecPacks;		// vector of all the allocated bins
	std::vector<AtlasPack*>::iterator	m_itCurrPack;
	std::map<std::string,AtlasIndex*>	m_mapIndices;	// map of all the individual index (leading to its bin, img and coordinates)

private:
	AtlasIndex*	InsertFromFile( const std::string& in_szFilename );
	AtlasIndex*	AddImageToPack( AtlasPack* in_pPack, const Rect& in_rectCoords, Image* in_pImage, int in_nOffsetX, int in_nOffsetY );

public:
	Atlas( const unsigned long in_nBinWidth, const unsigned long in_nBinHeight ) ;

	AtlasIndex*	Get( const std::string& in_szFilename );

	void BindTexture( unsigned int in_nPackIndex );
	void ReloadTexture();
};



// PACK =======================================================================

AtlasPack::AtlasPack( unsigned long w, unsigned long h )
{
	bin = new Bin(w,h);
	img = new Image(w,h,4);	//fixme
	tex = new Texture;
	tex->SetFilterMin( Texture::FilterMin::Nearest );
	tex->SetFilterMag( Texture::FilterMag::Nearest );
}

AtlasPack::~AtlasPack()
{
	delete bin;
	delete img;
	delete tex;
}

void AtlasPack::ReloadTexture()
{
	tex->LoadFromImage( img );
}

// ATLAS ======================================================================

Atlas::Atlas( const unsigned long in_nBinWidth, const unsigned long in_nBinHeight ) 
	: m_nBinWidth(in_nBinWidth), m_nBinHeight(in_nBinHeight)
{	
}

AtlasIndex*	Atlas::InsertFromFile( const std::string& in_szFilename )
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
				m_vecPacks.push_back( new AtlasPack(m_nBinWidth, m_nBinHeight) );
				
				m_itCurrPack = m_vecPacks.end()-1;
				
				Lair::GetLogMan()->Log( "Atlas", "Added new pack on image named %s. Pack count is now %d.", in_szFilename.c_str(), m_vecPacks.size() );
			}

			rectCoords = (*m_itCurrPack)->bin->Insert( pCropImage->GetWidth(), pCropImage->GetHeight(), true, Bin::RectBestShortSideFit, Bin::SplitMinimizeArea );

		} while ( rectCoords.height == 0 );	// if height is 0 then we could not fit it in bin so try next one
		
		AtlasIndex* pIndex = AddImageToPack( (*m_itCurrPack), rectCoords, pCropImage, nMinX, nMinY );

		delete pCropImage;	// flush cropped image

		return pIndex;
	}
	
	Lair::GetLogMan()->Log( "Atlas", "Could not load image named %s.", in_szFilename.c_str() );
	return 0;
}

AtlasIndex*	Atlas::AddImageToPack( AtlasPack* in_pPack, const Rect& in_rectCoords, Image* in_pImage, int in_nOffsetX, int in_nOffsetY )
{
	AtlasIndex* pIndex = new AtlasIndex;

	pIndex->size.x = (float)in_pImage->GetWidth();
	pIndex->size.y = (float)in_pImage->GetHeight();
	pIndex->offset.x = (float)in_nOffsetX;
	pIndex->offset.y = (float)in_nOffsetY;

	pIndex->uv_min.x = (in_rectCoords.x + 0.5f) / (float)m_nBinWidth;		// might need to add centroid offset here
	pIndex->uv_min.y = (in_rectCoords.y + 0.5f) / (float)m_nBinHeight;

	pIndex->uv_max.x = (in_rectCoords.x+in_rectCoords.width - 0.5f) / (float)m_nBinWidth;
	pIndex->uv_max.y = (in_rectCoords.y+in_rectCoords.height - 0.5f) / (float)m_nBinHeight;

	// fixme, need to do this because the image of the atlas is inverted
	float temp;
	SWAP( pIndex->uv_max.y, pIndex->uv_min.y, temp );

	if( in_rectCoords.width != in_pImage->GetWidth() )
	{
		// swap uv coordinates as the rect was rotated
		assert(0); //fixme, disabled because blit rotated doesnt exists yet
	}
	
	pIndex->pack = in_pPack;

	Image::Blit( in_pPack->img, in_rectCoords.x, in_rectCoords.y, in_pImage, 0,0, in_pImage->GetWidth(), in_pImage->GetHeight() );

	return pIndex;
}

AtlasIndex*	Atlas::Get( const std::string& in_szFilename )
{
	std::map< std::string, AtlasIndex* >::iterator it = m_mapIndices.find( in_szFilename );

	if( it != m_mapIndices.end() )
	{
		// found it, return it
//		Lair::GetLogMan()->Log( "Atlas", "Loaded index from map (%s).", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it
		AtlasIndex* pIndex;

		unsigned long nTime = Lair::GetSysMan()->GetTime();

		if( pIndex = InsertFromFile( in_szFilename ) )
		{
			m_mapIndices.insert( std::make_pair(in_szFilename,pIndex) );

			nTime = Lair::GetSysMan()->GetTime() - nTime;

			Lair::GetLogMan()->Log( "Atlas", "Inserted image named %s into atlas in %d ms.", in_szFilename.c_str(), nTime );
			return pIndex;
		}
		
		Lair::GetLogMan()->Log( "Atlas", "Could not insert image named %s in atlas.", in_szFilename.c_str() );
	}
	return 0;

}

void Atlas::BindTexture( unsigned int in_nPackIndex )
{
	m_vecPacks[in_nPackIndex]->tex->Bind();

}

void Atlas::ReloadTexture()
{
	for( std::vector<AtlasPack*>::iterator it = m_vecPacks.begin(); it != m_vecPacks.end(); it++ )
	{
		(*it)->ReloadTexture();
	}
}


// ATLASMAN ===================================================================

AtlasMan::AtlasMan() 
{ 
	m_pAtlas = new Atlas(1024,1024); 
}

AtlasMan::~AtlasMan()
{
	delete m_pAtlas;
}

AtlasIndex* AtlasMan::Get( const std::string& in_szFilename )
{
	return m_pAtlas->Get( in_szFilename );
}

void AtlasMan::Bind()
{
	m_pAtlas->BindTexture(0);
}

void AtlasMan::Reload()
{
	m_pAtlas->ReloadTexture();
}