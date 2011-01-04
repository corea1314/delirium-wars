#ifndef _ATLAS_H
#define _ATLAS_H

#include "Packing/GuillotineBinPack.h"

typedef GuillotineBinPack Bin;	// using guillotine

class Image;

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

		Bin*	bin;
		Image*	img;
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
	Index*	AddImageToPack( Pack*, const Rect&, Image* );

public:
	Atlas( const unsigned long in_nBinWidth, const unsigned long in_nBinHeight ) ;

	Index*	Insert( const std::string& in_szFilename );
};

#endif//_ATLAS_H