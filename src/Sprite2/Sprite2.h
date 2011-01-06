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
		Vector2 pos;
		Vector2 size;
		Vector2 center;
		float	angle;
		unsigned int	color;
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

		// bind attributes
		glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteData), &m_vecSpriteDataBuffer[0].size );
		glVertexPointer(3, GL_FLOAT, sizeof(SpriteData), &pVB[0].pos );
		glNormalPointer(GL_FLOAT, sizeof(SpriteData), &pVB[0].normal );
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SpriteData), & )

		// draw

		
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
