#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include <map>
#include <vector>
#include <string>

#include "../Math/Vector2.h"

class Texture;

class Frame
{
public:
	Frame();
	virtual ~Frame();

	bool Load( const std::string& in_szFilename );
	bool Unload();

	// all of those are in pixel space
	Vector2 GetSize() const		{ return m_vSize;	}
	Vector2 GetOffset() const	{ return m_vOffset;	}
	float	GetAngle() const	{ return m_fAngle;	}
	unsigned long GetDuration() const { return m_nDuration; }	// returns duration of frame (in ms)

	void SetSize( Vector2& in_vSize ) { m_vSize = in_vSize; }
	void SetOffset( Vector2& in_vOffset ) { m_vOffset = in_vOffset;	}
	void SetAngle( float in_fAngle ) { m_fAngle = in_fAngle; }
	void SetDuration( unsigned long in_nDuration ) { m_nDuration = in_nDuration; }	// returns duration of frame (in ms)

	const Texture* GetTexture() const { return m_pTexture; }

private:
	void RebuildVB()	//if internal params of a frame change, we need to rebuild the reference VB
	{
		Vector2 vExtent[4] = { Vector2(0,0), Vector2(0,m_vSize.y), Vector2(m_vSize.x,m_vSize.y), Vector2(m_vSize.x,0) };

		for(int i=0;i<4;i++)
		{
			vExtent[i] -= m_vOffset;
			Vector2::Rotate( vExtent[i], m_fAngle );
		}
	}

private:
	unsigned long	m_nDuration;
	Vector2		m_vOffset;
	float		m_fAngle;
	Vector2		m_vSize;	

	Texture*	m_pTexture;

	//todo: add reference VB
};

/*
Sequence File Spec

<string:image> <int:duration(milliseconds)> <number:offset_x(pixel)> <number:offset_y(pixel)> <number:angle(degrees)>  <number:scale_X> <number:scale_y> 
...


*/

class Sequence
{
public:
	virtual ~Sequence();

	bool Load( const std::string& in_szFilename );
	bool Unload();

	Frame* GetFrame();

private:
	void ParseFrameScript( std::string& in_szFrameScript );

private:
	std::vector< Frame* >	m_vecFrame;
};


class SequenceMan
{
public:
	bool Load( Sequence** out_pSequence, const std::string& in_szFilename );
	bool Unload( Sequence** in_pSequence );

	bool Preload( const std::string& in_szFilename );

private:
	std::map< std::string, Sequence* >	m_mapSequence;		//todo: move to SequenceMan and have sequence shared among everybody
};

#endif//_SEQUENCE_H