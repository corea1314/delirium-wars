#include "Entity.h"

#include <fstream>

IMPLEMENT_CLASS_TYPE(CEntity)

#include "Engine/Engine.h"

#include "Components/GotoComponent.h"
#include "Components/VisualComponent.h"
#include "Components/TurnComponent.h"
#include "Components/InputComponent.h"
#include "Components/EngineComponent.h"
#include "Components/CameraComponent.h"
#include "Components/PhysicsComponent.h"

#include <luawrapper/LuaContext.h>

#include <Lair/Lair.h>

#include "DebugDraw/DebugDraw.h"

///////////////////////////////////////////////////////////////////////////////
// CEntity
//

CEntity::CEntity( const std::string in_szName ) : m_fAngle(0.0f), m_szName(in_szName)
{
	m_vecComponents.reserve(8);	//FIXME arbitrary number of components
}

CEntity::~CEntity()
{
}

bool CEntity::Load( const std::string& in_szLuaFilename )
{
	m_szLuaFilename = in_szLuaFilename;

	unsigned long nTime = Lair::GetSysMan()->GetTime();

	std::ifstream is;
	is.open( m_szLuaFilename, std::ifstream::in );

	if( is.is_open() == false )
	{
		Lair::GetLogMan()->Log( "Lua Exception", "File named %s not found.", m_szLuaFilename.c_str() );
		return false;
	}

	try
	{		
		m_LuaContext.setChunkName( in_szLuaFilename );

		m_LuaContext.registerFunction("createGotoComponent",	&CEntity::CreateComponent<GotoComponent> );
		m_LuaContext.registerFunction("createTurnComponent",	&CEntity::CreateComponent<TurnComponent> );
		m_LuaContext.registerFunction("createVisualComponent",	&CEntity::CreateComponent<VisualComponent> );
		m_LuaContext.registerFunction("createInputComponent",	&CEntity::CreateComponent<InputComponent> );
		m_LuaContext.registerFunction("createEngineComponent",	&CEntity::CreateComponent<EngineComponent> );
		m_LuaContext.registerFunction("createCameraComponent",	&CEntity::CreateComponent<CameraComponent> );
		m_LuaContext.registerFunction("createPhysicsComponent",	&CEntity::CreateComponent<PhysicsComponent> );

		m_LuaContext.writeVariable("this", std::shared_ptr<CEntity>(this) );

		m_LuaContext.executeCode(is);
	}
	catch ( Lua::LuaContext::ExecutionErrorException e )
	{
		Lair::GetLogMan()->Log( "Lua Exception", "Execution error while binding script file named %s. (%s)", m_szLuaFilename.c_str(), e.what() );
		return false;
	}
	catch ( Lua::LuaContext::VariableDoesntExistException  e )
	{
		Lair::GetLogMan()->Log( "Lua Exception", "Variable not found while binding script file named %s. (%s)", m_szLuaFilename.c_str(), e.what() );
		return false;
	}
	catch ( Lua::LuaContext::SyntaxErrorException e )
	{
		Lair::GetLogMan()->Log( "Lua Exception", "Syntax error found while binding script file named %s. (%s)", m_szLuaFilename.c_str(), e.what() );
		return false;
	}
	catch ( Lua::LuaContext::WrongTypeException e )
	{
		Lair::GetLogMan()->Log( "Lua Binding Exception", "Type error found while binding script file named %s. (%s)", m_szLuaFilename.c_str(), e.what() );
		return false;
	}

	nTime = Lair::GetSysMan()->GetTime() - nTime;
	Lair::GetLogMan()->Log( "Lua", "Loaded entity script named %s in %d ms.", m_szLuaFilename.c_str(), nTime );

	return true;
}

void CEntity::Connect( CEngine* in_pEngine ) 
{ 
	unsigned long nTime = Lair::GetSysMan()->GetTime();

	m_pEngine = in_pEngine; 

	m_pEngine->Connect_OnRenderDebug( this, &CEntity::OnRenderDebug );

	try
	{		
		for( std::vector<std::shared_ptr<Component>>::iterator it = m_vecComponents.begin(); it != m_vecComponents.end(); it++ )
		{
			(*it)->Connect( in_pEngine, this );
		}		
		m_LuaContext.callLuaFunction<void>("OnConnect");
	}
	catch ( Lua::LuaContext::ExecutionErrorException e )
	{
		Lair::GetLogMan()->Log( "Lua Exception", "Execution error while binding script file named %s. (%s)", m_szLuaFilename.c_str(), e.what() );
	}
	catch ( Lua::LuaContext::VariableDoesntExistException  e )
	{
		Lair::GetLogMan()->Log( "Lua Exception", "Variable not found while binding script file named %s. (%s)", m_szLuaFilename.c_str(), e.what() );
	}
	catch ( Lua::LuaContext::SyntaxErrorException e )
	{
		Lair::GetLogMan()->Log( "Lua Exception", "Syntax error found while binding script file named %s. (%s)", m_szLuaFilename.c_str(), e.what() );
	}
	catch ( Lua::LuaContext::WrongTypeException e )
	{
		Lair::GetLogMan()->Log( "Lua Binding Exception", "Type error found while binding script file named %s. (%s)", m_szLuaFilename.c_str(), e.what() );
	}

	nTime = Lair::GetSysMan()->GetTime() - nTime;
	Lair::GetLogMan()->Log( "Lua", "Connected entity script named %s in %d ms.", m_szLuaFilename.c_str(), nTime );
}

void CEntity::Disconnect( CEngine* in_pEngine ) 
{ 
	assert(m_pEngine==in_pEngine); 

	for( std::vector<std::shared_ptr<Component>>::iterator it = m_vecComponents.begin(); it != m_vecComponents.end(); it++ )
	{
		(*it)->Disconnect( in_pEngine, this );
	}
	m_vecComponents.clear();

	m_pEngine->Disconnect_OnRenderDebug( this );
	m_pEngine = 0; 
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


///////////////////////////////////////////////////////////////////////////////
// CEntityDefinition
// 