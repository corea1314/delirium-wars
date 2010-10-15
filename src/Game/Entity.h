#ifndef _ENTITY_H
#define _ENTITY_H

#include "SigSlot.h"

class CEngine;

// this class defines a basic entity of the engine.
// all objects that needs to be processed by the engine needs to be derived from this.

class CEntity : public has_slots<>
{
public:
	virtual void Connect( CEngine* ) = 0;		// connects object to game engine
	virtual void Disconnect( CEngine* ) = 0;	// disconnects object from game engine
};

template <class T>
class CEntityFactory
{
	//todo

};

#endif//_ENTITY_H
