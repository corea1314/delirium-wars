
#include "Tile.h"

#include "Lair/Lair.h"

CTileLevel::CTileLevel( unsigned int in_nTileWidth, unsigned int in_nTileHeight ) 
	: m_nTileWidth(in_nTileWidth), m_nTileHeight(in_nTileHeight)
{
}

void CTileLevel::AddLayer( const std::string& in_szTextureFilename, const std::string& in_szLayerFilename )
{
	Layer* pLayer = new Layer;
	m_vecLayer.push_back(pLayer);

	// Assign texture
	pLayer->pTexture = Lair::GetTextureMan()->Get( in_szTextureFilename.c_str() );
	
	// Create vertex buffer
	TileData* pTileData = new TileData[];	//fixme hardcoded
}

#define TILE_PER_TEXTURE_X	16
#define TILE_PER_TEXTURE_Y	16
#define TILE_SIZE_X			16
#define TILE_SIZE_Y			16


void CTileLevel::BuildVB( unsigned int in_nTileCountX, unsigned int in_nTileCountY, TileData* in_pTileData )
{
	TileData* pTileLine;

	for( unsigned int y=0; y<in_nTileCountY; y++ )	
	{
		pTileLine = in_pTileData + y * in_nTileCountY;

		for( unsigned int x=0; x<in_nTileCountX; x++ )
		{

		}
	}
}

unsigned int CTileLevel::Classify( TileData* in_pTileData )
{
	if( in_pTileData->nValue )
	{

	}

	return -1;
}

void CTileLevel::BuildTileGeometry( unsigned int in_nX, unsigned int in_nY, TileData* in_pTileData )
{
	unsigned int nTileId = Classify( in_pTileData );

	Vertex v[4];
	v[0].pos.x = (in_nX * 16.0f);
	v[0].pos.y = (in_nY * 16.0f);

	v[1].pos.x = (in_nX * 16.0f)+16.0f;
	v[1].pos.y = (in_nY * 16.0f);

	v[2].pos.x = (in_nX * 16.0f)+16.0f;
	v[2].pos.y = (in_nY * 16.0f)+16.0f;

	v[3].pos.x = (in_nX * 16.0f);
	v[3].pos.y = (in_nY * 16.0f)+16.0f;
}