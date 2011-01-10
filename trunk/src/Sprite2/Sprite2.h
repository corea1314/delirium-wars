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
		
		Vector2 size;		// world space size (width,height)
		
		Vector2 offset;		// world space offset about pos
		float	angle;		// normalized angle

		Vector2 uv_min;		// texture coordinates
		
		Vector2 uv_max;
				
//		Vector2	vel;		// normalized velocity
//		float	avel;		// normalized angular velocity
		
		unsigned int	color;	// modulation color
	};

	std::vector<SpriteData>	m_vecSpriteDataBuffer;

	void Init( unsigned long in_nReservedSpriteCount = 8192 );
	void Exit();

	void Render();
	void RenderWithVBO();
	void RenderWithVA();

	ShaderGLSL*	m_pShader;

	unsigned int m_nVBO;
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
