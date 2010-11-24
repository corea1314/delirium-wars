#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <map>
#include <string>

class Image;

class Texture
{
public:
	Texture();
	virtual ~Texture();

	void Bind() const;
	void Unbind() const;

	bool LoadFromImage( Image* );
	bool LoadFromParam(  unsigned int in_nWidth, unsigned int in_nHeight, unsigned int in_nBytePerPixel );
	
	class WrapCoord	{ public: enum Enum { U, V }; };
	class WrapMode	{ public: enum Enum { Mirror, Clamp, Repeat }; };
	class FilterMag	{ public: enum Enum { Nearest, Linear }; };
	class FilterMin { public: enum Enum { Nearest, Linear }; }; //, NearestMipMapNearest, LinearMipMapNearest, NearestMipMapLinear, LinearMipMapLinear }; };

	void SetWrap( WrapCoord::Enum, WrapMode::Enum );
	void SetFilterMag( FilterMag::Enum );
	void SetFilterMin( FilterMin::Enum );

	unsigned long GetWidth() const { return m_nWidth; }
	unsigned long GetHeight() const { return m_nHeight; }

	unsigned int GetId() { return m_nId; }
			
private:
	unsigned int m_nId;

	unsigned long m_nWidth;
	unsigned long m_nHeight;
};

class TextureMan
{
public:
	Texture* Get( const std::string& in_szFilename );
		
private:
	bool LoadFromImage( Image* in_pImage, Texture** out_pTexture );

private:
	std::map< std::string, Texture* >	m_mapTexture;
};

#endif//_TEXTURE_H