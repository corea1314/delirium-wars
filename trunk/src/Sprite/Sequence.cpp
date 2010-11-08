#include "Sequence.h"

#include "../Lair/Lair.h"

#include "../minIni/minIni.h"

#include <string>

Frame::Frame()
{

}

Frame::~Frame()
{
	Unload();
}

bool Frame::Load( const std::string& in_szFilename )
{
	m_pTexture = Lair::GetTextureMan()->Get( in_szFilename );
	
	// todo: validate that the texture was loaded properly

	return true;
}

bool Frame::Unload()
{
	//todo: release texture here
	return true;
}

///////////////////////////////////////////////////////////////////////////////



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

	} while( ParseFrameScript( szValue ) );

	return false;
}

bool Sequence::Unload()
{
	for( std::vector<Frame*>::iterator it = m_vecFrame.begin(); it != m_vecFrame.end(); it++ )
	{
		delete (*it);
	}
	m_vecFrame.clear();
	return true;
}

bool Sequence::ParseFrameScript( std::string& in_szFrameScript )
{
//	<string:image> <int:duration(milliseconds)> <number:offset_x(pixel)> <number:offset_y(pixel)> <number:angle(degrees)>  <number:scale_X> <number:scale_y> 

	char szFilename[64];

	unsigned long	nDuration;
	Vector2		vOffset;
	float		fAngle;
	Vector2		vScale;

	if( sscanf( in_szFrameScript.c_str(), "%s %d %f %f %f %f %f", szFilename, &nDuration, &vOffset.x, &vOffset.y, &fAngle, &vScale.x, &vScale.y ) == 7 )
	{		
		Frame* pFrame = new Frame;
		
		if( pFrame->Load(szFilename) )
		{
			pFrame->SetAngle(fAngle);
			pFrame->SetDuration(nDuration);
			pFrame->SetOffset(vOffset);

			Vector2 vSize( pFrame->GetTexture()->GetWidth()*vScale.x, pFrame->GetTexture()->GetHeight()*vScale.y );
			pFrame->SetSize(vSize);

			m_vecFrame.push_back(pFrame);
			return true;
		}
		// something went wrong at this point, delete the allocated frame
		delete pFrame;
	}
	return false;
}


// ============================================================================

Sequence* SequenceMan::Get( const std::string& in_szFilename )
{
	std::map< std::string, Sequence* >::iterator it = m_mapSequence.find( in_szFilename );

	if( it != m_mapSequence.end() )
	{
		// found it, return it
		Lair::GetLogMan()->Log( "SequenceMan", "Loaded sequence from map (%s)", in_szFilename.c_str() );
		return it->second;
	}
	else
	{
		// not found, load it, return it
		Sequence* pSequence = new Sequence;
		if( pSequence->Load( in_szFilename ) )
		{
			Lair::GetLogMan()->Log( "SequenceMan", "Loaded sequence from file named %s", in_szFilename.c_str() );
			return pSequence;
		}
		Lair::GetLogMan()->Log( "SequenceMan", "Could not load sequence from file named %s", in_szFilename.c_str() );
		delete pSequence;
	}	
	return 0;
}

