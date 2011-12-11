#ifndef _ENTITY_H
#define _ENTITY_H

#include "SigSlot.h"

#include <vector>
#include <assert.h>

#include <Engine/ClassType.h>
#include <luawrapper/LuaContext.h>

class CEngine;
class GotoComponent;
class VisualComponent;

// this class defines a basic entity of the engine.
// all objects that needs to be processed by the engine needs to be derived from this.

class CEntity : public has_slots<>
{
	DECLARE_CLASS_TYPE_ROOT(CEntity);

public:
	// connects object to game engine
	virtual void Connect( CEngine* in_pEngine );
	
	// disconnects object from game engine
	virtual void Disconnect( CEngine* in_pEngine ) ;

	// get access engine
	CEngine*	GetEngine() { return m_pEngine; }

	// get access to lua context
	Lua::LuaContext& GetLuaContext() { return m_LuaContext; } 

protected:
	// component management
	virtual void CreateComponents();
	virtual void DestroyComponents();
	
public:
	// component
	class Component
	{
	public:
		Component() {}
		virtual ~Component() {}

		virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
		{	
			m_pEngine = in_pEngine; 
			m_pEntity = in_pEntity; 
		}
		virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity ) 
		{ 
			m_pEngine = NULL; 
			m_pEntity = NULL; 
		}

		virtual void Update( float in_fDeltaTime ) {}

		CEntity*	GetEntity() { return m_pEntity; }
		CEngine*	GetEngine() { return m_pEngine; }

	private:
		CEntity*	m_pEntity;
		CEngine*	m_pEngine;
	};

	friend class Component;
		
protected:
	std::shared_ptr<GotoComponent>		CreateGotoComponent();
	std::shared_ptr<VisualComponent>	CreateVisualComponent();
	
private:
	CEngine* m_pEngine;
	Lua::LuaContext	m_LuaContext;
	std::vector<std::shared_ptr<Component>>	m_vecComponents;
};

template <class T>
class CEntityFactory
{
	bool Create( CEntity** out_pEntity ) 
	{
		if( m_vecFree.size() != 0 )
		{
			(*out_pEntity) =  m_vecFree.pop_back();
			return true;
		}
		(*out_pEntity) = 0;
		return false;
	}
	bool Destroy( CEntity** in_pEntity )
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

		m_vecFree.reserve(in_nCount);

		for( int i=0; i<in_nCount; i++ )
		{
			m_vecFree.push_back( T::New() );
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
