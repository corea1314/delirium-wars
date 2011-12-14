#ifndef _ENTITY_H
#define _ENTITY_H

#include "SigSlot.h"

#include <vector>
#include <assert.h>

#include <Engine/ClassType.h>
#include <luawrapper/LuaContext.h>
#include <Math/Vector2.h>

class CEngine;
class GotoComponent;
class TurnComponent;
class VisualComponent; 
class InputComponent;
class EngineComponent;

class CDebugDraw;

// this class defines a basic entity of the engine.
// all objects that needs to be processed by the engine needs to be derived from this.

class CEntity : public has_slots<>
{
	DECLARE_CLASS_TYPE_ROOT(CEntity);

public:
	CEntity();
	virtual ~CEntity();

	// connects object to game engine
	virtual void Connect( CEngine* in_pEngine );
	
	// disconnects object from game engine
	virtual void Disconnect( CEngine* in_pEngine ) ;

	// loading
	bool CEntity::Load( const std::string& in_szLuaFilename );

	// get access engine
	CEngine*	GetEngine() { return m_pEngine; }

	// get access to lua context
	Lua::LuaContext& GetLuaContext() { return m_LuaContext; }

	inline Vector2& GetPos() { return m_vPos; }
	inline float& GetAngle() { return m_fAngle; }

protected:
	// engine callbacks
	void OnRenderDebug( CDebugDraw* in_pRD );
	
public:
	// component
	class Component : public has_slots<>
	{
	public:
		Component() {}
		virtual ~Component() {}

		// engine connection
		virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity ) ;
		virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity ) ;

		// engine callbacks
		virtual void OnUpdate( float in_fDeltaTime ) {}

		// helpers
		inline CEntity*	GetEntity() { return m_pEntity; }
		inline CEngine*	GetEngine() { return m_pEngine; }

	protected:
		typedef struct
		{
			bool	bEnabled;
			const char*	szName;

			inline bool IsEnabled() { return bEnabled; }
			inline const char*	GetName() { return szName; }

		}  LuaCallbackInfo;

		void BindCallback( const char* in_szName, LuaCallbackInfo& in_LuaCB );

	private:
		CEntity*	m_pEntity;
		CEngine*	m_pEngine;
	};

	friend class Component;
		
protected:
	 template<typename U>
	 std::shared_ptr<U>		CreateComponent()
	 {
		 std::shared_ptr<U> ptr = std::shared_ptr<U>(new U);
		 m_vecComponents.push_back(ptr);
		 return ptr;
	 }
	 	
private:
	std::string	m_szLuaFilename;

	Vector2	m_vPos;
	float	m_fAngle;
	CEngine* m_pEngine;
	Lua::LuaContext	m_LuaContext;
	std::vector<std::shared_ptr<Component>>	m_vecComponents;
};


/*
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
*/

#endif//_ENTITY_H
