#include "Sequence.h"

#include "../Lair.h"

#include "glee/GLee.h"

#include <minIni/minIni.h>

#include <algorithm>
#include <string>

Frame::Frame() : m_nDuration(0), m_nFrameTime(0), m_pTexture(0)
{
	m_pVB[0].depth = 0.0f;
	m_pVB[1].depth = 0.0f;
	m_pVB[2].depth = 0.0f;
	m_pVB[3].depth = 0.0f;

	m_pVB[0].color = 0xFFFFFFFF;
	m_pVB[1].color = 0xFFFFFFFF;
	m_pVB[2].color = 0xFFFFFFFF;
	m_pVB[3].color = 0xFFFFFFFF;

	m_pVB[0].texcoord.Set(0,1);
	m_pVB[1].texcoord.Set(0,0);
	m_pVB[2].texcoord.Set(1,1);
	m_pVB[3].texcoord.Set(1,0);
}

Frame::~Frame()
{
	Unload();
}

bool Frame::Load( const std::string& in_szFilename, unsigned long in_nDuration, unsigned long in_nFrameTime, const Vector2& in_vOffset, float in_fAngle, const Vector2& in_vScale )
{
	m_pTexture = Lair::GetTextureMan()->Get( in_szFilename );

	m_nDuration = in_nDuration;
	m_nFrameTime = in_nFrameTime;
	
	// todo: validate that the texture was loaded properly
	Vector2 vSize( m_pTexture->GetWidth()*in_vScale.x, m_pTexture->GetHeight()*in_vScale.y );

	Vector2 vExtent[4];
	vExtent[0] = Vector2(0,0);
	vExtent[1] = Vector2(0,vSize.y);
	vExtent[2] = Vector2(vSize.x,0);
	vExtent[3] = Vector2(vSize.x,vSize.y);

	Vector2 vOffset;
	vOffset.x = in_vOffset.x * in_vScale.x;
	vOffset.y = in_vOffset.y * in_vScale.y;

	m_vVertex[0] = ( vExtent[0] - vOffset ).GetRotate( DEG_TO_RAD(in_fAngle) );
	m_vVertex[1] = ( vExtent[1] - vOffset ).GetRotate( DEG_TO_RAD(in_fAngle) );
	m_vVertex[2] = ( vExtent[2] - vOffset ).GetRotate( DEG_TO_RAD(in_fAngle) );
	m_vVertex[3] = ( vExtent[3] - vOffset ).GetRotate( DEG_TO_RAD(in_fAngle) );

	return true;
}

bool Frame::Unload()
{
	//todo: release texture here
	return true;
}

void Frame::Transform( const Vector2& in_vTranslate, float in_fRotate, const Vector2& in_vScale )
{
	for( int i=0; i<4; i++ )
	{
		m_pVB[i].pos.x = m_vVertex[i].x * in_vScale.x; 
		m_pVB[i].pos.y = m_vVertex[i].y * in_vScale.y; 
		Vector2::Rotate( m_pVB[i].pos, in_fRotate); 
		m_pVB[i].pos += in_vTranslate;
	}	
}


void Frame::Render()
{	
	m_pTexture->Bind();

	//todo: push this vb down a bigger vb
#define VB_FORMAT	GL_T2F_C4UB_V3F
	glInterleavedArrays( VB_FORMAT, sizeof(Vertex), m_pVB );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

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

	} while( ParseFrameScript( in_szFilename, szKey, szValue ) );

	if( m_vecFrame.size() > 0 )
		return true;

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

bool Sequence::ParseFrameScript( const std::string& in_szFilename, char* in_szKey, std::string& in_szFrameScript )
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

		m_nDuration += nDuration;
		
		if( pFrame->Load(szFilename, nDuration, m_nDuration, vOffset, fAngle, vScale ) )
		{
			m_vecFrame.push_back(pFrame);
			return true;
		}
		// something went wrong at this point, delete the allocated frame
		Lair::GetLogMan()->Log( "Sequence", "Error parsing frame script file: %s key: %s script: %s)", in_szFilename.c_str(), in_szKey, in_szFrameScript.c_str() );
		delete pFrame;
	}
	return false;
}

Frame* Sequence::GetFrame( unsigned long in_nTime )
{
	FrameVectorIterator itFrame = m_vecFrame.begin();

	while( itFrame != m_vecFrame.end() )
	{
		if( in_nTime <= (*itFrame)->GetFrameTime() )
			break;
		++itFrame;
	}

	if( itFrame != m_vecFrame.end() )
		return *itFrame;

	Lair::GetLogMan()->Log( "Sequence", "Error computing frame index." );
	return m_vecFrame[0];	// return first frame in case of error
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

