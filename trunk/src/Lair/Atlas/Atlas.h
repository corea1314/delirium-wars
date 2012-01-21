#ifndef _ATLAS_H
#define _ATLAS_H

class Atlas;
class AtlasPack;

#include <string>
#include <vector>

#include "Math/Vector2.h"

class TiXmlElement;

class AtlasFrame
{
public:
	const Vector2& GetSize() const { return size;	}
	const Vector2& GetOffset() const { return offset; }
	const Vector2& GetMinUV() const { return uv_min; }
	const Vector2& GetMaxUV() const { return uv_max; }

private:
	Vector2 size;			// world space size (width,height)
	Vector2 offset;			// world space offset about pos
	Vector2 uv_min, uv_max;	// texture coordinates
	AtlasPack*	pack;

	friend class Atlas;
};

class AtlasFrameSequence
{
public:
	void AddFrame( const char* inFilename );
	AtlasFrame*	GetFrame( unsigned int inIndex );
	unsigned int GetFrameCount();

	void SerializeLoad( TiXmlElement* inNodeCurve );
	void SerializeSave( TiXmlElement* inNodeCurve );

protected:
	void Clear();

private:
	std::vector< AtlasFrame* >	mFrames;
	std::vector< std::string >	mNames;
};

class AtlasMan
{
public:
	AtlasMan();
	virtual ~AtlasMan();
	AtlasFrame* Get( const std::string& in_szFilename );

	void Bind();	//fixme

private:
	Atlas*	m_pAtlas;
};

#endif//_ATLAS_H