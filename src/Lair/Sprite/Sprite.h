#ifndef _SPRITE2_H
#define _SPRITE2_H

#include <map>
#include <vector>
#include <string>

#include "Math/Vector2.h"
#include "Lair/Atlas/Atlas.h"


class Sprite;
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

	SpriteData*		m_pSpriteDataBuffer;
	SpriteData*		m_pCurrSpriteData;

public:
//	Frame* GetFrame( const char* in_szFilename );

	Sprite* GetSprite();
		
	void Init( unsigned long in_nReservedSpriteCount = 8192 );
	void Exit();

	void Render();
	void RenderWithVBO();
	void RenderWithVA();

	SpriteMan() { Init(); }
	virtual ~SpriteMan() { Exit(); }

	unsigned int GetCurrSpriteCount() const { return (m_pCurrSpriteData - m_pSpriteDataBuffer); }

private:
	ShaderGLSL*	m_pShader;
	unsigned int m_nVBO;
	unsigned int m_nMaxSpriteCount;

	
	void Test();
	std::vector<AtlasFrame*> m_vecAtlasFrame;
};

#include "Lair/Sequence/Sequence.h"

class Sprite
{
private:
	Sequence*			m_pCurrSequence;
	Sequence::Frame*	m_pCurrFrame;

	SpriteMan::SpriteData*	m_pSD;

	bool		m_bIsPlaying;
	bool		m_bIsLooping;

	float	m_fAnimTime;

	// World transform
	Vector2		m_vPos;
	float		m_fAngle;
	Vector2		m_vScale;

	unsigned char*	m_pColor;

protected:
	void UpdateFromFrame();

public:
	Sprite (SpriteMan::SpriteData* in_pSD );
	virtual ~Sprite();

	void Play( const std::string& in_szSequenceName, bool in_bLoop=false );
	void Update( float in_fDeltaTime );

	bool IsPlaying() const;

	void Set( float x, float y, float a = 0.0f, float sx = 1.0f, float sy = 1.0f );
	void SetAlpha( float a );
	void SetColor( float r, float g, float b );
	void SetFrame( AtlasFrame* in_pFrame );
};

#endif//_SPRITE_H
