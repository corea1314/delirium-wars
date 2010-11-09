#ifndef _SOUND_H
#define _SOUND_H

#include <string>

#include <fmod.hpp>
#include <fmod_errors.h>


class Sound
{
public:
	bool Load( const std::string& in_szFilename, FMOD::System* in_pFmodSys );

	void Play( bool in_bLoop = false );
	void Stop();
	void Pause( bool in_bPause = true );
	bool IsPaused();

private:
	FMOD::Sound*	m_pFmodSound;
	FMOD::Channel*	m_pFmodChannel;
	FMOD::System*	m_pFmodSystem;
};


class SoundMan
{
public:
	SoundMan();
	virtual ~SoundMan();

	Sound* Get( const std::string& in_szFilename );

	void Update( float in_fDeltaTime );

	static void ErrorCheck( FMOD_RESULT in_FmodResult );

private:
	FMOD::System* m_pFmodSystem;
};

#endif//_SOUND_H