#ifndef _SPRITE_H
#define _SPRITE_H

#include <map>
#include <vector>
#include <string>

#include "../Math/Vector2.h"

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
	
	Sequence* m_pCurrSequence;
	bool	m_bIsPlaying;

public:
	void Play( std::string in_szSequenceName, bool in_bLoop=false );
	void Update( float in_fDeltaTime );

	bool IsPlaying() const;

	//todo: connect to rendering signal
};





#endif//_SPRITE_H