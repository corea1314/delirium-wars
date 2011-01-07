#ifndef _SPRITE_H
#define _SPRITE_H

#include <map>
#include <vector>
#include <string>

#include "../Math/Vector2.h"

class ShaderGLSL;

class SpriteRenderer
{
public:
	class SpriteData
	{
	public:
		Vector2 pos;		// world space position
		float	depth;		// world space depth
		float	angle;		// normalized angle

		Vector2 size;		// world space size (width,height)
		Vector2 offset;		// world space offset about pos

		Vector2 uv_min;		// texture coordinates
		Vector2 uv_max;
				
		Vector2	vel;		// normalized velocity
		float	avel;		// normalized angular velocity
		
		unsigned int	color;	// modulation color
	};

	std::vector<SpriteData>	m_vecSpriteDataBuffer;

	void Init( unsigned long in_nReservedSpriteCount = 8192 )
	{
		m_vecSpriteDataBuffer.reserve( in_nReservedSpriteCount );
	}

	void Render()
	{
		// bind shader
		m_pShader->Bind();

		// bind texture (need to be atlas)
		
		// bind attributes
		glVertexPointer(4, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].pos );			// pos, depth, angle
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].color );	// color

		glActiveTexture( GL_TEXTURE0 );
		glTexCoordPointer(4, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].size );		// size and offset
		glActiveTexture( GL_TEXTURE1 );
		glTexCoordPointer(4, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].uv_min );		// texture coordinates
		glActiveTexture( GL_TEXTURE2 );
		glTexCoordPointer(3, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].vel );			// vel, avel

		// draw
		glDrawArrays( GL_POINTS, 0, m_vecSpriteDataBuffer.size() );

		m_pShader->Unbind();
		
	}

	ShaderGLSL*	m_pShader;
};

/*

#version 120
#extension GL_EXT_geometry_shader4 : enable

uniform vec4 color;

uniform float size_x;
uniform float size_y;

void main( void )
{
	gl_FrontColor  = gl_FrontColorIn[0];
	gl_Position    = gl_PositionIn  [0];
	gl_TexCoord[0] = vec4(0,0,1,1);
	gl_Position.x += -size_x;
	gl_Position.y += -size_y;
	EmitVertex();

	gl_Position    = gl_PositionIn  [0];
	gl_Position.x += -size_x;
	gl_Position.y +=  size_y;
	gl_TexCoord[0] = vec4(0,1,1,1);;
	EmitVertex();

	gl_Position    = gl_PositionIn  [0];
	gl_Position.x +=  size_x;
	gl_Position.y += -size_y;
	gl_TexCoord[0] = vec4(1,0,1,1);;
	EmitVertex();

	gl_Position    = gl_PositionIn  [0];
	gl_Position.x += size_x;
	gl_Position.y += size_y;
	gl_TexCoord[0] = vec4(1,1,1,1);;
	EmitVertex();
	
	EndPrimitive();
}

*/

#endif//_SPRITE_H
