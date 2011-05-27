#include "Sequence.h"

#include "Lair/Lair.h"

#include "glee/GLee.h"

#include <minIni/minIni.h>

#include <algorithm>
#include <string>


///////////////////////////////////////////////////////////////////////////////

Sequence::Sequence() : m_nDuration(0)
{

}


Sequence::~Sequence()
{
	Unload();
}

bool Sequence::Load( const std::string& in_szFilename )
{
	minIni iniSequence( in_szFilename.c_str() );

	std::string szValue;
	char szKey[64];
	
	int i=0;
	
	do
	{
		sprintf_s( szKey, "frame%d", i );
		szValue = iniSequence.gets( "Frames", szKey );
		++i;

		if( szValue == "" )
			break;	// cannot find key

	} while( ParseFrameScript( in_szFilename, szKey, szValue ) );

	if( m_vecFrame.size() > 0 )
		return true;

	return false;
}

bool Sequence::Unload()
{
	m_vecFrame.clear();
	return true;
}

bool Sequence::ParseFrameScript( const std::string& in_szFilename, char* in_szKey, std::string& in_szFrameScript )
{
//	<string:image> <int:duration(milliseconds)> <number:offset_x(pixel)> <number:offset_y(pixel)> <number:angle(degrees)>  <number:scale_X> <number:scale_y> 

	char szFilename[64];

	Frame	frame;

	if( sscanf( in_szFrameScript.c_str(), "%s %d %f %f %f %f %f", szFilename, &frame.duration, &frame.offset.x, &frame.offset.y, &frame.angle, &frame.scale.x, &frame.scale.y ) == 7 )
	{		
		m_nDuration += frame.duration;

		frame.filename = szFilename;
		frame.frame = Lair::GetAtlasMan()->Get(szFilename);

		frame.angle = DEG_TO_RAD(frame.angle);
		frame.frametime = m_nDuration;
				
		m_vecFrame.push_back(frame);
		return true;
	}
	
	// something went wrong at this point
	Lair::GetLogMan()->Log( "Sequence", "Error parsing frame script file: %s key: %s script: %s).", in_szFilename.c_str(), in_szKey, in_szFrameScript.c_str() );

	return false;
}

Sequence::Frame* Sequence::GetFrame( unsigned long in_nTime )
{
	FrameVectorIterator itFrame = m_vecFrame.begin();

	while( itFrame != m_vecFrame.end() )
	{
		if( in_nTime <= itFrame->frametime )
			break;
		++itFrame;
	}

	if( itFrame != m_vecFrame.end() )
		return &(*itFrame);

	Lair::GetLogMan()->Log( "Sequence", "Error computing frame index." );
	return &m_vecFrame[0];	// return first frame in case of error
}


// ============================================================================

Sequence* SequenceMan::Get( const std::string& in_szFilename )
{
	std::map< std::string, Sequence* >::iterator it = m_mapSequence.find( in_szFilename );

	if( it != m_mapSequence.end() )
	{
		// found it, return it
//		Lair::GetLogMan()->Log( "SequenceMan", "Loaded sequence from map (%s).", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it
		Sequence* pSequence = new Sequence;

		unsigned long nTime = Lair::GetSysMan()->GetTime();

		if( pSequence->Load( in_szFilename ) )
		{
			nTime = Lair::GetSysMan()->GetTime() - nTime;

			Lair::GetLogMan()->Log( "SequenceMan", "Loaded sequence from file named %s in %d ms.", in_szFilename.c_str(), nTime );
			return pSequence;
		}
		Lair::GetLogMan()->Log( "SequenceMan", "Could not load sequence from file named %s.", in_szFilename.c_str() );
		delete pSequence;
	}	
	return 0;
}

