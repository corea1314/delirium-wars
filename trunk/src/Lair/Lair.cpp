
#include "Lair.h"

#define NEW( classname )			\
	m_p##classname = new classname;

#define DELETE( classname )			\
	delete m_p##classname;

Lair*	Lair::ms_pInstance = 0;

Lair::Lair()
{
	NEW(LogMan);
	NEW(ImageMan);
	NEW(TextureMan);
	NEW(SoundMan);
	NEW(SequenceMan);
}

Lair::~Lair()
{
	DELETE(SequenceMan);
	DELETE(SoundMan);
	DELETE(TextureMan);
	DELETE(ImageMan);
	DELETE(LogMan);
}

