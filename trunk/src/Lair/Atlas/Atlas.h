#ifndef _ATLAS_H
#define _ATLAS_H

#include "Packing/GuillotineBinPack.h"

typedef GuillotineBinPack Bin;	// using guillotine

class Image;
class Texture;

#include "../../Math/Vector2.h"

#include <map>
#include <vector>

class Atlas
{
private:
	class Pack
	{
	public:
		Pack( unsigned long w, unsigned long h );
		virtual ~Pack();

		void ReloadTexture();

		Bin*		bin;
		Image*		img;
		Texture*	tex;	// fixme, find a better place
	};

public:
	class Index
	{
	public:
		const Vector2& GetSize() const { return size;	}
		const Vector2& GetOffset() const { return offset; }
		const Vector2& GetMinUV() const { return uv_min; }
		const Vector2& GetMaxUV() const { return uv_max; }

	private:
		Vector2 size;			// world space size (width,height)
		Vector2 offset;			// world space offset about pos
		Vector2 uv_min, uv_max;	// texture coordinates
		Pack*			pack;

		friend class Atlas;
	};

private:
	const unsigned long m_nBinWidth;	// maximal width of the bins
	const unsigned long m_nBinHeight;	// maximal height of the bins

	std::vector<Pack*>	m_vecPacks;		// vector of all the allocated bins
	std::vector<Pack*>::iterator	m_itCurrPack;
	std::map<std::string,Index*>	m_mapIndices;	// map of all the individual index (leading to its bin, img and coordinates)

private:
	Index*	InsertFromFile( const std::string& in_szFilename );
	Index*	AddImageToPack( Pack* in_pPack, const Rect& in_rectCoords, Image* in_pImage, int in_nOffsetX, int in_nOffsetY );

public:
	Atlas( const unsigned long in_nBinWidth, const unsigned long in_nBinHeight ) ;

	Index*	Get( const std::string& in_szFilename );

	void BindTexture( unsigned int in_nPackIndex );
	void ReloadTexture();
};

#endif//_ATLAS_H