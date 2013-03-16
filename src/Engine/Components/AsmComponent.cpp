
#include "Lair/Lair.h"
#include "Engine/Engine.h"
#include "Engine/Entity.h"

#include "AsmComponent.h"

// component interface ////////////////////////////////////////////////////////

void AsmComponent::Load(  const char* inFileName )
{
	mpMachine->Load( inFileName );
}

void AsmComponent::SetBool(  const char* inParameterName, bool inValue )
{
	mpMachine->SetParameter( inParameterName, inValue );
}

void AsmComponent::Set( float x, float y, float a, float sx, float sy )
{
}

void AsmComponent::SetAlpha( float a )
{
}

void AsmComponent::SetColor( float r, float g, float b )
{
}

// engine connection //////////////////////////////////////////////////////////

void AsmComponent::Connect( CEngine* in_pEngine, CEntity* in_pEntity ) 
{	
	Component::Connect( in_pEngine, in_pEntity );

	in_pEngine->Connect_OnUpdate( this, &AsmComponent::OnUpdate );

	mpMachine = new ASM::CMachine;

	GetEntity()->GetLuaContext().registerFunction("load", &AsmComponent::Load);
	GetEntity()->GetLuaContext().registerFunction("set_bool", &AsmComponent::SetBool);
}

void AsmComponent::Disconnect( CEngine* in_pEngine, CEntity* in_pEntity )
{
	in_pEngine->Disconnect_OnUpdate( this );

	if( mpMachine )
	{
		delete mpMachine;
		mpMachine = 0;
	}

	Component::Disconnect( in_pEngine, in_pEntity );
}

// engine callbacks ///////////////////////////////////////////////////////////

void AsmComponent::OnUpdate( float in_fDeltaTime )
{
	// TODO add mechanism to check if we linked 'set' methods to sync with entity
	mpMachine->Set( GetEntity()->GetPos().x, GetEntity()->GetPos().y, GetEntity()->GetAngle(), 1.0f, 1.0f );
	mpMachine->Update(in_fDeltaTime);
}
