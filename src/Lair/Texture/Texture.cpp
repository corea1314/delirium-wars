
#include "Texture.h"

#include "../../fw.h"	//todo: include opengl properly

#include "Lair/Lair.h"

Texture::Texture() : m_nWidth(0),m_nHeight(0)
{
	glGenTextures( 1, &m_nId );
}

Texture::~Texture()
{
	glDeleteTextures( 1, &m_nId );
}

void Texture::Bind() const
{
	glBindTexture( GL_TEXTURE_2D, m_nId );
}

void Texture::Unbind() const
{
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture::SetWrap( WrapCoord::Enum ec, WrapMode::Enum em )
{
	static GLenum eWrapCoord[] = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T };
	static GLenum eWrapMode[] = { GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_REPEAT };

	glBindTexture( GL_TEXTURE_2D, m_nId );
	glTexParameteri( GL_TEXTURE_2D, eWrapCoord[ec], eWrapMode[em] );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture::SetFilterMag( FilterMag::Enum e )
{
	static GLenum eFilterMag[] = { GL_NEAREST, GL_LINEAR };

	glBindTexture( GL_TEXTURE_2D, m_nId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, eFilterMag[e] );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture::SetFilterMin( FilterMin::Enum e )
{
	static GLenum eFilterMin[] = 
	{	
		GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, 
		GL_LINEAR_MIPMAP_NEAREST,GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR 
	};

	glBindTexture( GL_TEXTURE_2D, m_nId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, eFilterMin[e] );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

bool Texture::LoadFromImage( Image* in_pImage )
{
	if( in_pImage )
	{
		glBindTexture( GL_TEXTURE_2D, m_nId );
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	
		switch( in_pImage->GetBytesPerPixel() )
		{
			case 3:	glTexImage2D( GL_TEXTURE_2D, 0, 3, in_pImage->GetWidth(), in_pImage->GetHeight(), 0, GL_RGB,  GL_UNSIGNED_BYTE, in_pImage->GetPixelBuffer() ); break;
			case 4:	glTexImage2D( GL_TEXTURE_2D, 0, 4, in_pImage->GetWidth(), in_pImage->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, in_pImage->GetPixelBuffer() ); break;
			default:
				return false;	//not supported
		}
	
		SetFilterMin( FilterMin::Linear );
		SetFilterMag( FilterMag::Linear );

		glBindTexture( GL_TEXTURE_2D, 0 );

		m_nWidth = in_pImage->GetWidth();
		m_nHeight = in_pImage->GetHeight();

		return true;
	}

	return false;
}

bool Texture::LoadFromParam( unsigned int in_nWidth, unsigned int in_nHeight, unsigned int in_nBytePerPixel )
{
	glBindTexture( GL_TEXTURE_2D, m_nId );
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	switch( in_nBytePerPixel )
	{
	case 3:	glTexImage2D( GL_TEXTURE_2D, 0, 3, in_nWidth, in_nHeight, 0, GL_RGB,  GL_UNSIGNED_BYTE, 0 ); break;
	case 4:	glTexImage2D( GL_TEXTURE_2D, 0, 4, in_nWidth, in_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 ); break;
	default:
		return false;	//not supported
	}

	SetFilterMin( FilterMin::Linear );
	SetFilterMag( FilterMag::Linear );
		
	glBindTexture( GL_TEXTURE_2D, 0 );

	m_nWidth = in_nWidth;
	m_nHeight = in_nHeight;

	return true;
}

///////////////////////////////////////////////////////////////////////////////

Texture* TextureMan::Get( const std::string& in_szFilename )
{
	std::map< std::string, Texture* >::iterator it = m_mapTexture.find( in_szFilename );

	if( it != m_mapTexture.end() )
	{
		// found it, return it
//		Lair::GetLogMan()->Log( "TextureMan", "Loaded texture from map (%s).", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it

		Image* pImage = Lair::GetImageMan()->Get( in_szFilename );	//acquire image first

		unsigned long nTime = Lair::GetSysMan()->GetTime();

		Texture* pTexture = new Texture;
		if( pTexture->LoadFromImage( pImage ) )
		{
			nTime = Lair::GetSysMan()->GetTime() - nTime;

			m_mapTexture.insert( std::make_pair(in_szFilename,pTexture) );
			Lair::GetLogMan()->Log( "TextureMan", "Loaded texture from image named %s in %d ms.", in_szFilename.c_str(), nTime );
			return pTexture;
		}
		Lair::GetLogMan()->Log( "TextureMan", "Could not load texture from image named %s.", in_szFilename.c_str() );

		delete pTexture;
	}
	return 0;
}

bool TextureMan::LoadFromImage( Image* in_pImage, Texture** out_pTexture )
{
	// not found, load it, return it
	(*out_pTexture) = new Texture;
	
	if( (*out_pTexture)->LoadFromImage( in_pImage ) )
		return true;

	delete (*out_pTexture);
	(*out_pTexture) = 0;

	return false;
}
