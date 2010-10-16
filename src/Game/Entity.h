#ifndef _ENTITY_H
#define _ENTITY_H

#include "SigSlot.h"

#include <vector>

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
	bool Create( T** out_pEntity ) 
	{
		if( m_vecFree.size() != 0 )
		{
			(*out_pEntity) =  m_vecFree.pop_back();
			return true;
		}
		(*out_pEntity) = 0;
		return false;
	}
	bool Destroy( T** in_pEntity )
	{
		if( *in_pEntity != 0 )
		{
			m_vecFree.push_back((*in_pEntity));
			return true;
		}
		
		return false;
	}

	void Precache( unsigned int in_nCount )
	{
		T::Precache();

		m_vecFree.ReservedStream0(in_nCount);

		for( int i=0; i<in_nCount; i++ )
		{
			m_vecFree.push_back( new T );
		}
	}

	void Uncache()
	{
		T::Uncache();

		m_lstFree.begin()
		
		while( m_vecFree.size() != 0 ) 
		{
			delete m_vecFree.back();
			m_vecFree.pop_back();
		}		
		m_vecFree.clear();
	}

private:
	std::vector<T*>	m_vecFree;
};

#endif//_ENTITY_H
