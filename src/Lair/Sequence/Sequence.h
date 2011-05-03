#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include <map>
#include <vector>
#include <string>

#include "Math/Vector2.h"

class AtlasIndex;

/*
Sequence File Spec

<string:image> <int:duration(milliseconds)> <number:offset_x(pixel)> <number:offset_y(pixel)> <number:angle(degrees)> <number:scale_X> <number:scale_y> 
...


*/

class Sequence
{
public:
	Sequence();
	virtual ~Sequence();

	bool Load( const std::string& in_szFilename );
	bool Unload();
	unsigned long GetDuration() const { return m_nDuration; }

	class Frame
	{
	public:
		std::string		filename;
		unsigned long	duration;
		unsigned long	frametime;
		Vector2			offset; 
		float			angle; 
		Vector2			scale;
		AtlasIndex*		index;
	};
	
	Frame* GetFrame( unsigned long in_nTime );	// time is in ms

private:
	bool ParseFrameScript( const std::string& in_szFilename, char* in_szKey, std::string& in_szFrameScript );

private:
	typedef std::vector<Frame> FrameVector;
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