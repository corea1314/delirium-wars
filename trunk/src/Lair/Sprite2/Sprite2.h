#ifndef _SPRITE2_H
#define _SPRITE2_H

#include <map>
#include <vector>
#include <string>

#include "../../Math/Vector2.h"
#include "../Atlas/Atlas.h"


class ShaderGLSL;

class SpriteMan
{
public:
	class SpriteData	//DO NOT MOVE MEMBERS AROUND
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

	typedef AtlasIndex Frame;

	class Sprite
	{
	public:
		unsigned long m_nIndex;
//		unsigned long m_nGroup;	//
	};

public:
	Frame* GetFrame( const char* in_szFilename );

	Sprite* GetSprite();
		
	void Init( unsigned long in_nReservedSpriteCount = 8192 );
	void Exit();

	void Render();
	void RenderWithVBO();
	void RenderWithVA();

	SpriteMan() { Init(); }
	virtual ~SpriteMan() { Exit(); }

private:
	ShaderGLSL*	m_pShader;
	unsigned int m_nVBO;
		
	// test
	std::vector<AtlasIndex*>	m_vecAtlasIndex;
};

#endif//_SPRITE2_H
