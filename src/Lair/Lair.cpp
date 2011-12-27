
#include "Lair.h"

#define __NEW( classname )			\
	m_p##classname = new classname;

#define __DELETE( classname )			\
	delete m_p##classname;

Lair*	Lair::ms_pInstance = 0;

Lair::Lair()
{
	ms_pInstance = this;

	__NEW(InputMan);
	__NEW(LogMan);
	__NEW(SysMan);
	__NEW(ImageMan);
	__NEW(TextureMan);
	__NEW(SoundMan);
	__NEW(AtlasMan);
	__NEW(SequenceMan);
	__NEW(SpriteMan);
	__NEW(CameraMan);	
}

Lair::~Lair()
{
	__DELETE(CameraMan);
	__DELETE(SpriteMan);
	__DELETE(SequenceMan);
	__DELETE(AtlasMan);
	__DELETE(SoundMan);
	__DELETE(TextureMan);
	__DELETE(ImageMan);
	__DELETE(SysMan);
	__DELETE(LogMan);
	__DELETE(InputMan);
}

