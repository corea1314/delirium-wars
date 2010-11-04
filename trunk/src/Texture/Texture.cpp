
#include "Texture.h"

#include "../fw.h"	//todo: include opengl properly

#include "../Image/Image.h"

void Texture::Bind()
{
	glBindTexture( GL_TEXTURE_2D, m_nId );
}

void Texture::Unbind()
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
	
		switch( in_pImage->GetBytePerPixel() )
		{
			case 3:	glTexImage2D( GL_TEXTURE_2D, 0, 3, in_pImage->GetWidth(), in_pImage->GetHeight(), 0, GL_RGB,  GL_UNSIGNED_BYTE, in_pImage->GetPixelBuffer() ); break;
			case 4:	glTexImage2D( GL_TEXTURE_2D, 0, 4, in_pImage->GetWidth(), in_pImage->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, in_pImage->GetPixelBuffer() ); break;
			default:
				return false;	//not supported
		}
	
		SetFilterMin( FilterMin::Linear );
		SetFilterMag( FilterMag::Linear );

		glBindTexture( GL_TEXTURE_2D, 0 );

		return false;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////

bool TextureMan::Load( Image* in_pImage, Texture** out_pTexture )
{
	std::map< Image*, Texture* >::iterator it = m_mapTexture.find( in_pImage );
	
	if( it != m_mapTexture.end() && (*out_pTexture) == 0 )
	{
		// found it, return it
		(*out_pTexture) = it->second;
		return true;
	}
	else
	{
		// not found, load it, return it
		(*out_pTexture) = new Texture;
		if( (*out_pTexture)->LoadFromImage( in_pImage ) )
			return true;

		delete (*out_pTexture);
	}
	(*out_pTexture) = 0;
	return false;
}
