
#include "Sound.h"

#include "../Lair.h"

bool Sound::Load( const std::string& in_szFilename, FMOD::System* in_pFmodSystem )
{
	m_pFmodSystem = in_pFmodSystem;
//	SoundMan::ErrorCheck( m_pFmodSystem->createStream( in_szFilename.c_str(), FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &m_pFmodSound ) );
	SoundMan::ErrorCheck( m_pFmodSystem->createSound( in_szFilename.c_str(), FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &m_pFmodSound ) );

	return true;
}

void Sound::Play( bool in_bLoop, bool in_bNewChannel )
{
//	if( in_bNewChannel )
//		m_pFmodChannel = 0;

	SoundMan::ErrorCheck( m_pFmodSystem->playSound( FMOD_CHANNEL_FREE, m_pFmodSound, true, &m_pFmodChannel ) );
	SoundMan::ErrorCheck( m_pFmodChannel->setLoopCount( in_bLoop ? -1 : 0 ) );
	SoundMan::ErrorCheck( m_pFmodChannel->setPaused( false ) );		// This is where the sound really starts.
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

// ============================================================================

#define SOUNDMAN_MAX_CHANNEL	128

SoundMan::SoundMan()
{
	unsigned int      nVersion;

	SoundMan::ErrorCheck( FMOD::System_Create(&m_pFmodSystem) );
	SoundMan::ErrorCheck( m_pFmodSystem->getVersion(&nVersion) );

	if (nVersion < FMOD_VERSION)
	{
		Lair::GetLogMan()->Log( "SoundMan", "Error! You are using an old version of FMOD %08x. This program requires %08x.", nVersion, FMOD_VERSION);
		return;
	}

	SoundMan::ErrorCheck( m_pFmodSystem->init(SOUNDMAN_MAX_CHANNEL, FMOD_INIT_NORMAL, 0) );
}

SoundMan::~SoundMan()
{
	SoundMan::ErrorCheck( m_pFmodSystem->close() );
	SoundMan::ErrorCheck( m_pFmodSystem->release() );

	m_pFmodSystem = 0;
}

Sound* SoundMan::Get( const std::string& in_szFilename )
{
	std::map< std::string, Sound* >::iterator it = m_mapSound.find( in_szFilename );

	if( it != m_mapSound.end() )
	{
		// found it, return it
		Lair::GetLogMan()->Log( "SoundMan", "Loaded sound from map (%s).", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it
		Sound* pSound = new Sound;
		if( pSound->Load( in_szFilename, m_pFmodSystem ) )
		{
			m_mapSound.insert( std::make_pair(in_szFilename,pSound) );
			Lair::GetLogMan()->Log( "SoundMan", "Loaded sound from file named %s.", in_szFilename.c_str() );
			return pSound;
		}
		Lair::GetLogMan()->Log( "SoundMan", "Could not load sound from file named %s.", in_szFilename.c_str() );

		delete pSound;
	}
	return 0;
}

void SoundMan::ErrorCheck( FMOD_RESULT in_FmodResult )
{
	if (in_FmodResult != FMOD_OK)
	{
		Lair::GetLogMan()->Log( "SoundMan",  "FMOD error! (%d) %s.", in_FmodResult, FMOD_ErrorString(in_FmodResult));
	}
}