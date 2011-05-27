#ifndef _ATLAS_H
#define _ATLAS_H

class Atlas;
class AtlasPack;

#include <string>

#include "Math/Vector2.h"

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