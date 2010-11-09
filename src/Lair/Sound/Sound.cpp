
#include "Sound.h"

#include "../Lair.h"


bool Sound::Load( const std::string& in_szFilename, FMOD::System* in_pFmodSystem )
{
	m_pFmodSystem = in_pFmodSystem;
	SoundMan::ErrorCheck( m_pFmodSystem->createStream( in_szFilename.c_str(), FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &m_pFmodSound ) );

	return true;
}

void Sound::Play( bool in_bLoop )
{
	SoundMan::ErrorCheck( m_pFmodSystem->playSound(FMOD_CHANNEL_FREE, m_pFmodSound, false, &m_pFmodChannel ) );
}

void Sound::Stop()
{
	if( m_pFmodChannel )
	{
		m_pFmodChannel->stop();
		m_pFmodChannel = 0;
	}
}

void Sound::Pause( bool in_bPause )
{
	if( m_pFmodChannel )
		m_pFmodChannel->setPaused(in_bPause);
}

bool Sound::IsPaused()
{
	bool bPaused = true;
	if( m_pFmodChannel )
	{
		m_pFmodChannel->getPaused(&bPaused);
	}	
	return bPaused;
}

SoundMan::SoundMan()
{
	unsigned int      nVersion;

	SoundMan::ErrorCheck( FMOD::System_Create(&m_pFmodSystem) );
	SoundMan::ErrorCheck( m_pFmodSystem->getVersion(&nVersion) );

	if (nVersion < FMOD_VERSION)
	{
		Lair::GetLogMan()->Log( "SoundMan",  "Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", nVersion, FMOD_VERSION);
		return;
	}

	SoundMan::ErrorCheck( m_pFmodSystem->init(1, FMOD_INIT_NORMAL, 0) );
}

SoundMan::~SoundMan()
{
	SoundMan::ErrorCheck( m_pFmodSystem->close() );
	SoundMan::ErrorCheck( m_pFmodSystem->release() );

	m_pFmodSystem = 0;
}


void SoundMan::ErrorCheck( FMOD_RESULT in_FmodResult )
{
	if (in_FmodResult != FMOD_OK)
	{
		Lair::GetLogMan()->Log( "SoundMan",  "FMOD error! (%d) %s\n", in_FmodResult, FMOD_ErrorString(in_FmodResult));
	}
}