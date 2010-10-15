#include "Field.h"
#include "Engine.h"


void CField::Update( float in_fDeltaTime )
{
// todo
}

void CField::Render()
{
// todo
}

void CField::Connect( CEngine* in_pEngine )
{
	in_pEngine->Connect_Update( this, &CField::Update );
	in_pEngine->Connect_Render( this, &CField::Render );
}

void CField::Disconnect( CEngine* in_pEngine )
{
	in_pEngine->Disconnect_Update( this );
	in_pEngine->Disconnect_Render( this );
}