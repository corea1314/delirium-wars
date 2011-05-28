#include "Atlas.h"

#include "Lair/Lair.h"

#include "Packing/GuillotineBinPack.h"

#define ATLAS_TEXTURE_SIZE	2048

typedef GuillotineBinPack Bin;	// using guillotine


class AtlasPack
{
public:
	AtlasPack( unsigned long w, unsigned long h );
	virtual ~AtlasPack();

	Bin*		bin;
	Texture*	tex;	// fixme, find a better place
};


class Atlas
{
private:
	const unsigned long m_nBinWidth;	// maximal width of the bins
	const unsigned long m_nBinHeight;	// maximal height of the bins

	std::vector<AtlasPack*>	m_vecPacks;		// vector of all the allocated bins
	std::vector<AtlasPack*>::iterator	m_itCurrPack;
	std::map<std::string,AtlasFrame*>	m_mapFrames;	// map of all the individual index (leading to its bin, img and coordinates)

private:
	AtlasFrame*	InsertFromFile( const std::string& in_szFilename );
	AtlasFrame*	AddImageToPack( AtlasPack* in_pPack, const Rect& in_rectCoords, Image* in_pImage, int in_nUncroppedWidth, int in_nUncroppedHeight, int in_nOffsetX, int in_nOffsetY );

public:
	Atlas( const unsigned long in_nBinWidth, const unsigned long in_nBinHeight ) ;

	AtlasFrame*	Get( const std::string& in_szFilename );

	void BindTexture( unsigned int in_nPackIndex );
};



// PACK =======================================================================

AtlasPack::AtlasPack( unsigned long w, unsigned long h )
{
	bin = new Bin(w,h);
	tex = new Texture;
	tex->LoadFromParam(w,h,4);
	tex->SetFilterMin( Texture::FilterMin::Nearest );
	tex->SetFilterMag( Texture::FilterMag::Nearest );
}

AtlasPack::~AtlasPack()
{
	delete bin;
	delete tex;
}


// ATLAS ======================================================================

Atlas::Atlas( const unsigned long in_nBinWidth, const unsigned long in_nBinHeight ) 
	: m_nBinWidth(in_nBinWidth), m_nBinHeight(in_nBinHeight)
{	
}

AtlasFrame*	Atlas::InsertFromFile( const std::string& in_szFilename )
{
	Image* pImage;

	if( pImage = Lair::GetImageMan()->Get(in_szFilename) )
	{
		Rect rectCoords;
		m_itCurrPack = m_vecPacks.begin();

		int nMinX=0, nMinY=0, nMaxX=0, nMaxY=0;

#define USE_CROPPING //fixme: cropping is broken

#ifdef USE_CROPPING
		Image* pCropImage = Image::Crop( pImage, nMinX, nMinY, nMaxX, nMaxY );
#else
		Image* pCropImage = pImage;
#endif

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
		
		AtlasFrame* pIndex = AddImageToPack( (*m_itCurrPack), rectCoords, pCropImage, pImage->GetWidth(), pImage->GetHeight(), nMinX, pImage->GetHeight()-(nMaxY+1) ); 
																									// offset Y is calculated taking into accounts that the image is inverted.
#ifdef USE_CROPPING
		delete pCropImage;	// flush cropped image
#endif
		return pIndex;
	}
	
	Lair::GetLogMan()->Log( "Atlas", "Could not load image named %s.", in_szFilename.c_str() );
	return 0;
}

AtlasFrame*	Atlas::AddImageToPack( AtlasPack* in_pPack, const Rect& in_rectCoords, Image* in_pImage, int in_nUncroppedWidth, int in_nUncroppedHeight, int in_nOffsetX, int in_nOffsetY )
{
	AtlasFrame* pIndex = new AtlasFrame;

	Vector2 vCenter( in_nUncroppedWidth/2.0f, in_nUncroppedHeight/2.0f);

	pIndex->size.x = (float)in_pImage->GetWidth();
	pIndex->size.y = (float)in_pImage->GetHeight();
	pIndex->offset.x = (in_rectCoords.width/2.0f)-vCenter.x + in_nOffsetX; 
	pIndex->offset.y = (in_rectCoords.height/2.0f)-vCenter.y + in_nOffsetY;

	Lair::GetLogMan()->Log( "Atlas", "Image inserted at %d, %d size(%d, %d).", in_rectCoords.x, in_rectCoords.y, in_rectCoords.width, in_rectCoords.height );

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

	in_pPack->tex->LoadFromImage( in_pImage, in_rectCoords.x, in_rectCoords.y );

	return pIndex;
}

AtlasFrame*	Atlas::Get( const std::string& in_szFilename )
{
	std::map< std::string, AtlasFrame* >::iterator it = m_mapFrames.find( in_szFilename );

	if( it != m_mapFrames.end() )
	{
		// found it, return it
//		Lair::GetLogMan()->Log( "Atlas", "Loaded index from map (%s).", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it
		AtlasFrame* pIndex;

		unsigned long nTime = Lair::GetSysMan()->GetTime();

		if( pIndex = InsertFromFile( in_szFilename ) )
		{
			m_mapFrames.insert( std::make_pair(in_szFilename,pIndex) );

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
	if( m_vecPacks.size() > in_nPackIndex )
		m_vecPacks[in_nPackIndex]->tex->Bind();

}


// ATLASMAN ===================================================================

AtlasMan::AtlasMan() 
{ 
	m_pAtlas = new Atlas(ATLAS_TEXTURE_SIZE,ATLAS_TEXTURE_SIZE); 
}

AtlasMan::~AtlasMan()
{
	delete m_pAtlas;
}

AtlasFrame* AtlasMan::Get( const std::string& in_szFilename )
{
	return m_pAtlas->Get( in_szFilename );
}

void AtlasMan::Bind()
{
	m_pAtlas->BindTexture(0);
}

