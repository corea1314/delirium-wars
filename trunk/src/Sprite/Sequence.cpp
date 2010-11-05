#include "Sequence.h"

#include "../Texture/Texture.h"

Frame::Frame()
{

}

Frame::~Frame()
{
	Unload();
}

bool Frame::Load( const std::string& in_szFilename )
{
	//todo: load texture here
	return false;
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

void Sequence::ParseFrameScript( std::string& in_szFrameScript )
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
			return;
		}
		// something went wrong at this point, delete the allocated frame
		delete pFrame;
	}
}

bool SequenceMan::Load( Sequence** out_pSequence, const std::string& in_szFilename )
{
	std::map< std::string, Sequence* >::iterator it = m_mapSequence.find( in_szFilename );

	if( it != m_mapSequence.end() && (*out_pSequence) == 0 )
	{
		// found it, return it
		(*out_pSequence) = it->second;
		return true;
	}
	else
	{
		// not found, load it, return it
		(*out_pSequence) = new Sequence;
		if( (*out_pSequence)->Load( in_szFilename ) )
			return true;

		delete (*out_pSequence);
	}
	(*out_pSequence) = 0;
	return false;
}
