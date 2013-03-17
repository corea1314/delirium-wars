#pragma once

class Sprite;

namespace ASM { class CMachine; }

class AsmComponent : public CEntity::Component
{
public:
	AsmComponent() {}
	virtual ~AsmComponent() {}

	// exposed methods
	void Load( const char* inFileName );
	void SetParameterInt( const char* inParameterName, int inValue );
	void SetParameterFloat( const char* inParameterName, float inValue );
	void SetParameterBool( const char* inParameterName, bool inValue );

	void Set( float x, float y, float a = 0.0f, float sx = 1.0f, float sy = 1.0f );
	void SetAlpha( float a );
	void SetColor( float r, float g, float b );
	
	// engine callbacks
	void OnUpdate( float in_fDeltaTime );

	// engine connection
	virtual void Connect( CEngine* in_pEngine, CEntity* in_pEntity );
	virtual void Disconnect( CEngine* in_pEngine, CEntity* in_pEntity );

private:
	ASM::CMachine*		mpMachine;

	// callbacks
//	LuaCallbackInfo		m_cbOnEndOfSequence;	//todo: implement this
};