#include "Sprite.h"

#include "../Lair/Lair.h"
#include "../Lair/Sequence/Sequence.h"

#include "glee/GLee.h"

Sprite::Sprite() : m_pCurrSequence(0), m_pCurrFrame(0), m_bIsPlaying(false), m_bIsLooping(false), m_fAnimTime(0)
{
	SetTransform( 0.0f, 0.0f );
}

void Sprite::Set( float x, float y, float a, float sx, float sy )
{
	m_vScale.x = sx;
	m_vScale.y = sy;
	m_fAngle = a;
	m_vPos.x = x;
	m_vPos.y = y;
}

void Sprite::Play( std::string in_szSequenceName, bool in_bLoop )
{
	m_pCurrSequence = Lair::GetSequenceMan()->Get( in_szSequenceName );

	if( m_pCurrSequence )
	{
		m_fAnimTime = 0.0f;
		m_bIsPlaying = true;
		m_bIsLooping = in_bLoop;
	}
}

void Sprite::Update( float in_fDeltaTime )
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

void Sprite::Render()
{
	if( m_pCurrFrame )
	{		
		ApplyTransform();

		m_pCurrFrame->GetTexture()->Bind();

		//todo: push this vb down a bigger vb
		#define VB_FORMAT	GL_T2F_C4UB_V3F
		glInterleavedArrays( VB_FORMAT, sizeof(Frame::Vertex), m_pVB );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
}

void Sprite::ApplyTransform()
{
	memcpy( m_pVB, m_pCurrFrame->GetVB(), sizeof(Frame::Vertex)*4 );
	m_pVB[0].pos*=m_vScale; Vector2::Rotate( m_pVB[0].pos, m_fAngle); m_pVB[0].pos += m_vPos;
	m_pVB[1].pos*=m_vScale; Vector2::Rotate( m_pVB[1].pos, m_fAngle); m_pVB[1].pos += m_vPos;
	m_pVB[2].pos*=m_vScale; Vector2::Rotate( m_pVB[2].pos, m_fAngle); m_pVB[2].pos += m_vPos;
	m_pVB[3].pos*=m_vScale; Vector2::Rotate( m_pVB[3].pos, m_fAngle); m_pVB[3].pos += m_vPos;
}
