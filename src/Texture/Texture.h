#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <map>
#include <string>

class Image;

class Texture
{
public:
	void Bind();
	void Unbind();

	bool LoadFromImage( Image* );
	
	class WrapCoord	{ public: enum Enum { U, V }; };
	class WrapMode	{ public: enum Enum { Mirror, Clamp, Repeat }; };
	class FilterMag	{ public: enum Enum { Nearest, Linear }; };
	class FilterMin { public: enum Enum { Nearest, Linear }; }; //, NearestMipMapNearest, LinearMipMapNearest, NearestMipMapLinear, LinearMipMapLinear }; };

	void SetWrap( WrapCoord::Enum, WrapMode::Enum );
	void SetFilterMag( FilterMag::Enum );
	void SetFilterMin( FilterMin::Enum );
			
private:
	unsigned int m_nId;
};

class TextureMan
{
public:
	bool Load( Image* in_pImage, Texture** out_pTexture );
	
private:
	std::map< Image*, Texture* >	m_mapTexture;
};

#endif//_TEXTURE_H