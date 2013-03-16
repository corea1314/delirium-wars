#pragma once

#include "Engine\Entity.h"
#include "Engine\Components\VisualComponent.h"



class CPlayer : public CEntity
{
public:
	virtual void Connect( CEngine* );		// connects object to game engine
	virtual void Disconnect( CEngine* );	// disconnects object from game engine

	virtual void OnUpdate( float in_fDeltaTime );

private:

	std::shared_ptr<VisualComponent>	mVisualComponent;
};
