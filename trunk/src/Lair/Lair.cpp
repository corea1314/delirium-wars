
#include "Lair.h"

#define __NEW( classname )			\
	m_p##classname = new classname;

#define __DELETE( classname )			\
	delete m_p##classname;

Lair*	Lair::ms_pInstance = 0;

Lair::Lair()
{
	ms_pInstance = this;

	__NEW(LogMan);
	__NEW(SysMan);
	__NEW(ImageMan);
	__NEW(TextureMan);
	__NEW(SoundMan);
	__NEW(SequenceMan);
	__NEW(SpriteMan);
}

Lair::~Lair()
{
	__DELETE(SpriteMan);
	__DELETE(SequenceMan);
	__DELETE(SoundMan);
	__DELETE(TextureMan);
	__DELETE(ImageMan);
	__DELETE(SysMan);
	__DELETE(LogMan);
}

