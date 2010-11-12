#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include <map>
#include <vector>
#include <string>

#include "../../Math/Vector2.h"

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
	unsigned long GetFrameTime() const { return m_nFrameTime; }	// returns time of frame (in ms)

	void SetSize( Vector2& in_vSize ) 
	{ 
		m_vSize = in_vSize; 
		m_vExtent[0] = Vector2(0,0);
		m_vExtent[1] = Vector2(0,m_vSize.y);
		m_vExtent[2] = Vector2(m_vSize.x,0);
		m_vExtent[3] = Vector2(m_vSize.x,m_vSize.y);
	}
	void SetOffset( Vector2& in_vOffset ) { m_vOffset = in_vOffset;	}
	void SetAngle( float in_fAngle ) { m_fAngle = in_fAngle; }
	void SetDuration( unsigned long in_nDuration ) { m_nDuration = in_nDuration; }	// returns duration of frame (in ms)
	void SetFrameTime( unsigned long in_nFrameTime ) { m_nFrameTime = in_nFrameTime; }	// returns time of frame (in ms)

	const Texture* GetTexture() const { return m_pTexture; }
	
	bool operator < ( const Frame& rhs )  { return GetFrameTime() < rhs.GetFrameTime(); }
	bool operator < ( const Frame& rhs ) const { return GetFrameTime() < rhs.GetFrameTime(); }

	class Color 
	{
	public:

		enum E
		{
			eBLACK	= 0xFF000000, eWHITE	= 0xFFFFFFFF, eGREY	= 0xFF7F7F7F, eRED	= 0xFF0000FF, eGREEN	= 0xFF00FF00,
			eBLUE	= 0xFFFF0000, eORANGE = 0xFF007FFF,	eYELLOW = 0xFF00FFFF, ePURPLE = 0xFFFF00FF,	eLIME	= 0xFF00FF7F,
			eCYAN	= 0xFFFFFF00
		};

		unsigned long c;
		Color & operator = (const unsigned long & other ) { c = other; return *this; } 
		Color & operator = (const Color::E & other ) { c = other; return *this; } 

		static Color Random();	
	};

	class Vertex
	{
	public:
		// GL_T2F_C4UB_V3F
		Vector2 texcoord;
		Color color;
		Vector2 pos;
		float depth;
	};

	const Vertex*	GetVB() const { return m_pVB; }

	void BuildVB();

private:
	unsigned long	m_nDuration;
	unsigned long	m_nFrameTime;

	Vector2		m_vOffset;
	float		m_fAngle;
	Vector2		m_vSize;
	Vector2		m_vExtent[4];

	Texture*	m_pTexture;
	
	Vertex	m_pVB[4];
};

/*
Sequence File Spec

<string:image> <int:duration(milliseconds)> <number:offset_x(pixel)> <number:offset_y(pixel)> <number:angle(degrees)>  <number:scale_X> <number:scale_y> 
...


*/

class Sequence
{
public:
	Sequence();
	virtual ~Sequence();

	bool Load( const std::string& in_szFilename );
	bool Unload();

	Frame* GetFrame( unsigned long in_nTime );	// time is in ms

	unsigned long GetDuration() const { return m_nDuration; }

private:
	bool ParseFrameScript( const std::string& in_szFilename, char* in_szKey, std::string& in_szFrameScript );

private:
	typedef std::vector<Frame*> FrameVector;
	typedef FrameVector::iterator FrameVectorIterator;

	FrameVector		m_vecFrame;
	unsigned long	m_nDuration;	// duration is ms
};


class SequenceMan
{
public:
	Sequence* Get( const std::string& in_szFilename );

private:
	std::map< std::string, Sequence* >	m_mapSequence;		//todo: move to SequenceMan and have sequence shared among everybody
};

#endif//_SEQUENCE_H