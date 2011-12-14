#ifndef _TILE_H
#define _TILE_H

#include <string>
#include <vector>

#include "Math/Vector2.h"

class Texture;

class CTileLevel
{
public:
	CTileLevel( unsigned int in_nTileWidth, unsigned int in_nTileHeight );

	void AddLayer( const std::string& in_szTextureFilename, const std::string& in_szLayerFilename );

protected:

	class Vertex
	{
	public:
		Vector2 texcoord;
		Vector2 pos;
		float	depth;

		// format: GL_T2F_V3F
	};

	class TileData
	{
	public:
		enum AdjencyMask 
		{ 
			eE	=	0x01,
			eNE	=	0x02,
			eN	=	0x04,
			eNW	=	0x08,
			eW	=	0x10,
			eSW	=	0x20,
			eS	=	0x40,
			eSE	=	0x80	
		};

		unsigned int	nValue;
		AdjencyMask		eAdjency;
	};

	class Layer
	{
	public:
		Texture*	pTexture;
		std::vector<Vertex>	vecVB;
	};
	
	unsigned int Classify( TileData* in_pTileData );

	void BuildVB( unsigned int in_nTileCountX, unsigned int in_nTileCountY, TileData* in_pTileData );
	void BuildTileGeometry( unsigned int in_nX, unsigned int in_nY, TileData* in_pTileData );
	void BuildAdjencyMask( unsigned int in_nTileCountX, unsigned int in_nTileCountY, TileData* in_pTileData ); 

private:
	unsigned int m_nTileWidth, m_nTileHeight;
	std::vector<Layer*>	m_vecLayer;
};

#endif//_TILE_H

