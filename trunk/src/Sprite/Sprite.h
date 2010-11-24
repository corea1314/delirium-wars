#ifndef _SPRITE_H
#define _SPRITE_H

#include <map>
#include <vector>
#include <string>

#include "../Math/Vector2.h"

#include "../Lair/Sequence/Sequence.h"

class Sprite
{
private:
	Sequence*	m_pCurrSequence;
	Frame*		m_pCurrFrame;
	bool		m_bIsPlaying;
	bool		m_bIsLooping;

	float	m_fAnimTime;
		
	Frame::Vertex	m_pVB[4];

	// World transform
	Vector2		m_vPos;
	float		m_fAngle;
	Vector2		m_vScale;

public:
	Sprite();

	void Play( std::string in_szSequenceName, bool in_bLoop=false );
	void Update( float in_fDeltaTime );

	bool IsPlaying() const;

	void Render();

	void Set( float x, float y, float a = 0.0f, float sx = 1.0f, float sy = 1.0f );

	//todo: connect to rendering signal
};


#endif//_SPRITE_H
