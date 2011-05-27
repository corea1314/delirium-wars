#include "Animation.h"

#include "Lair/Lair.h"
#include "Lair/Sequence/Sequence.h"

SpriteAnimator::SpriteAnimator() : m_pCurrSequence(0), m_pCurrFrame(0), m_bIsPlaying(false), m_bIsLooping(false), m_fAnimTime(0)
{
	Set( 0.0f, 0.0f );

	m_pSprite = Lair::GetSpriteMan()->GetSprite();
}

SpriteAnimator::~SpriteAnimator()
{

}

void SpriteAnimator::Set( float x, float y, float a, float sx, float sy )
{
	m_vScale.x = sx;
	m_vScale.y = sy;
	m_fAngle = a;
	m_vPos.x = x;
	m_vPos.y = y;
}

void SpriteAnimator::Play( std::string in_szSequenceName, bool in_bLoop )
{
	m_pCurrSequence = Lair::GetSequenceMan()->Get( in_szSequenceName );

	if( m_pCurrSequence )
	{
		m_fAnimTime = 0.0f;
		m_bIsPlaying = true;
		m_bIsLooping = in_bLoop;
	}
}

void SpriteAnimator::Update( float in_fDeltaTime )
{
	if( m_pCurrSequence && m_bIsPlaying )
	{
		m_fAnimTime += in_fDeltaTime;

		float fDuration = m_pCurrSequence->GetDuration()/1000.0f;	// duration in secs

		if( m_fAnimTime > fDuration )
		{
			if( m_bIsLooping )
				m_fAnimTime = fmodf( m_fAnimTime, fDuration );
			else
			{
				m_fAnimTime = fDuration;
				m_bIsPlaying = false;
			}
		}
		m_pCurrFrame = m_pCurrSequence->GetFrame( (unsigned long)(m_fAnimTime*1000) );
	}
}
