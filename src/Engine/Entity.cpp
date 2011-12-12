#include "Entity.h"

#include <fstream>

IMPLEMENT_CLASS_TYPE(CEntity)

#include "Engine/Engine.h"

#include "Components/GotoComponent.h"
#include "Components/VisualComponent.h"
#include "Components/TurnComponent.h"

#include <luawrapper/LuaContext.h>

#include <Lair/Lair.h>

#include "DebugDraw/DebugDraw.h"

///////////////////////////////////////////////////////////////////////////////
// CEntity
//

CEntity::CEntity()
: m_fAngle(0.0f)
{
}

CEntity::~CEntity()
{

}

void CEntity::Connect( CEngine* in_pEngine ) 
{ 
	m_pEngine = in_pEngine; 

	m_pEngine->Connect_OnRenderDebug( this, &CEntity::OnRenderDebug );

	std::ifstream is;
	is.open ("scripts/entity.lua", std::ifstream::in);

	// todo add component and register them to lua
	try
	{		
		m_LuaContext.registerFunction("createGotoComponent", &CEntity::CreateGotoComponent);
		m_LuaContext.registerFunction("createTurnComponent", &CEntity::CreateTurnComponent);
		m_LuaContext.registerFunction("createVisualComponent", &CEntity::CreateVisualComponent);
		m_LuaContext.writeVariable("this", std::shared_ptr<CEntity>(this) );
			
		m_LuaContext.executeCode(is);

		for( std::vector<std::shared_ptr<Component>>::iterator it = m_vecComponents.begin(); it != m_vecComponents.end(); it++ )
		{
			(*it)->Connect( in_pEngine, this );
		}

		m_LuaContext.callLuaFunction<void>("init");
	}
	catch ( Lua::LuaContext::ExecutionErrorException e )
	{
		Lair::GetLogMan()->Log( "Lua Binding Exception",  e.what() );
	}
	catch ( Lua::LuaContext::VariableDoesntExistException  e )
	{
		Lair::GetLogMan()->Log( "Lua Binding Exception",  e.what() );
	}
	catch ( Lua::LuaContext::SyntaxErrorException e )
	{
		Lair::GetLogMan()->Log( "Lua Binding Exception",  e.what() );
	}
	catch ( Lua::LuaContext::WrongTypeException e )
	{
		Lair::GetLogMan()->Log( "Lua Binding Exception",  e.what() );
	}
		
	/*
	m_LuaContext.writeVariable("increment", [](std::shared_ptr<MyType> ptr) { ptr->increment(); });
	m_LuaContext.registerFunction("increment", &MyType::increment);
	m_LuaContext.registerFunction("decrement", &MyType::decrement);
	m_LuaContext.registerFunction("read", &MyType::read);
	m_LuaContext.registerFunction("getValue", [](std::shared_ptr<MyType> ptr) -> int { return ptr->getVariable(); });
	std::shared_ptr<MyType> ptr(new MyType(5));
	m_LuaContext.writeVariable("create", [ptr]() { return ptr; });
	m_LuaContext.executeCode(		
		"function inc()\n"
		"t:increment()\n"
		"t:increment()\n"
		"t:increment()\n"
		"end\n"
		"t = create(); increment(t); x = t:getValue(); t:increment();")

		;
	assert(ptr);
	assert(ptr->getVariable() == 7);
	m_LuaContext.callLuaFunction<void>("inc");
	assert(ptr->getVariable() == 10);
	assert(m_LuaContext.readVariable<int>("x") == 6);
	*/
}

void CEntity::Disconnect( CEngine* in_pEngine ) 
{ 
	assert(m_pEngine==in_pEngine); 

	for( std::vector<std::shared_ptr<Component>>::iterator it = m_vecComponents.begin(); it != m_vecComponents.end(); it++ )
	{
		(*it)->Disconnect( in_pEngine, this );
	}

	m_pEngine->Disconnect_OnRenderDebug( this );

	m_pEngine = 0; 
}

std::shared_ptr<VisualComponent> CEntity::CreateVisualComponent()
{
	std::shared_ptr<VisualComponent> ptr = std::shared_ptr<VisualComponent>(new VisualComponent);
	m_vecComponents.push_back(ptr);
	return ptr;
}

std::shared_ptr<GotoComponent> CEntity::CreateGotoComponent()
{
	std::shared_ptr<GotoComponent> ptr = std::shared_ptr<GotoComponent>(new GotoComponent);
	m_vecComponents.push_back(ptr);
	return ptr;
}

std::shared_ptr<TurnComponent> CEntity::CreateTurnComponent()
{
	std::shared_ptr<TurnComponent> ptr = std::shared_ptr<TurnComponent>(new TurnComponent);
	m_vecComponents.push_back(ptr);
	return ptr;
}

void CEntity::OnRenderDebug( CDebugDraw* in_pRD )
{
	in_pRD->SetColor( CDebugDraw::Color::eRED );
	in_pRD->DrawRectangle( m_vPos, 8, 8, m_fAngle );	
}


///////////////////////////////////////////////////////////////////////////////
// Component
//
void CEntity::Component::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	m_pEngine = in_pEngine; 
	m_pEntity = in_pEntity; 
}

void CEntity::Component::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{ 
	m_pEngine = NULL; 
	m_pEntity = NULL; 
}

typedef struct
{
	bool	bEnabled;
	const char*	szName;

}  LuaCallbackInfo;

void CEntity::Component::BindCallback( const char* in_szName, LuaCallbackInfo& in_LuaCB )
{
	in_LuaCB.szName = in_szName;
	in_LuaCB.bEnabled = GetEntity()->GetLuaContext().doesFunctionExist( in_LuaCB.szName );
}

/*

/*
void CLuaEntity::OnUpdate( float in_fDeltaTime )
{
	m_LuaContext.callLuaFunction("OnUpdate", in_fDeltaTime);
}

void CLuaEntity::OnKeyboard( unsigned char in_cKey )
{
	m_LuaContext.callLuaFunction("OnKeyboard", in_cKey);
}

if( GetLuaContext().doesFunctionExist("OnUpdate") ) 
in_pEngine->Disconnect_OnUpdate( this );
if( GetLuaContext().doesFunctionExist("OnKeyboard") ) 
in_pEngine->Disconnect_OnKeyboard( this );


*/