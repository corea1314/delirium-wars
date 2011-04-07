#ifndef _LAIR_H
#define _LAIR_H

#define DEFINE( classname )													\
public:																		\
	static classname*	Get##classname() { return Get()->m_p##classname; }	\
private:																	\
	classname*	m_p##classname;

#include "Log/Log.h"
#include "Image/Image.h"
#include "Texture/Texture.h"
#include "Sound/Sound.h"
#include "Sequence/Sequence.h"
#include "Shader/ShaderGLSL.h"
#include "System/System.h"
#include "Sprite2/Sprite2.h"
#include "Atlas/Atlas.h"

class Lair
{	
public:
	Lair();
	virtual ~Lair();
	
	static void Create()	{ if( ms_pInstance == 0 ) ms_pInstance = new Lair; }
	static void Release()	{ if( ms_pInstance != 0 ) { delete ms_pInstance; ms_pInstance = 0; } }
	
	DEFINE(LogMan);
	DEFINE(ImageMan);
	DEFINE(TextureMan);
	DEFINE(SoundMan);
	DEFINE(SequenceMan);
	DEFINE(SysMan);
	DEFINE(AtlasMan);
	DEFINE(SpriteMan);
	
private:
	static Lair* Get() { return ms_pInstance; }

private:
	static Lair*	ms_pInstance;
};

#endif//_LAIR_H