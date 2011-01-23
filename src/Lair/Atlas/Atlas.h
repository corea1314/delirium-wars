#ifndef _ATLAS_H
#define _ATLAS_H

#include "Packing/GuillotineBinPack.h"

typedef GuillotineBinPack Bin;	// using guillotine

class Image;
class Texture;

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
		inline unsigned long GetX() { return x; }
		inline unsigned long GetY() { return y; }
		inline unsigned long GetWidth()		{ return w; }
		inline unsigned long GetHeight()	{ return h; }

	private:
		unsigned long	x,y;
		unsigned long	w,h;
		unsigned long	cx,cy;
		unsigned long	cw,ch;
		Pack*			pack;
		bool			rotated;	// if the image is rotated

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
	Index*	AddImageToPack( Pack*, const Rect&, Image*, int in_nCropOffsetX, int in_nCropOffsetY, Image* in_pCropImage );

public:
	Atlas( const unsigned long in_nBinWidth, const unsigned long in_nBinHeight ) ;

	Index*	Get( const std::string& in_szFilename );

	void ReloadTexture();
};

#endif//_ATLAS_H