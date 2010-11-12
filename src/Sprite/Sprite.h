#ifndef _SPRITE_H
#define _SPRITE_H

#include <map>
#include <vector>
#include <string>

#include "../Math/Vector2.h"

class Frame;
class Sequence;

class Sprite
{
private:
	class Transform
	{
	public:
		// perform in those order
		Vector2		vScale;	
		float		fRotate;
		Vector2		vTranslate;		
	};
	
	Sequence*	m_pCurrSequence;
	Frame*		m_pCurrFrame;
	bool		m_bIsPlaying;
	bool		m_bIsLooping;

	float	m_fAnimTime;

public:
	Sprite();

	void Play( std::string in_szSequenceName, bool in_bLoop=false );
	void Update( float in_fDeltaTime );

	bool IsPlaying() const;

	void Render();

	//todo: connect to rendering signal
};





#endif//_SPRITE_H