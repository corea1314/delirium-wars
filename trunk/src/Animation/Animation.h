#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <map>
#include <vector>
#include <string>

#include "Math/Vector2.h"
#include "Lair/Sequence/Sequence.h"
#include "Lair/Sprite/Sprite.h"

class SpriteAnimator
{
private:
	Sequence*			m_pCurrSequence;
	Sequence::Frame*	m_pCurrFrame;

	SpriteMan::Sprite*	m_pSprite;

	bool		m_bIsPlaying;
	bool		m_bIsLooping;

	float	m_fAnimTime;
		
	// World transform
	Vector2		m_vPos;
	float		m_fAngle;
	Vector2		m_vScale;

public:
	SpriteAnimator();
	virtual ~SpriteAnimator();

	void Play( std::string in_szSequenceName, bool in_bLoop=false );
	void Update( float in_fDeltaTime );

	bool IsPlaying() const;

	void Set( float x, float y, float a = 0.0f, float sx = 1.0f, float sy = 1.0f );

	//todo: connect to rendering signal
};


#endif//_ANIMATION_H
