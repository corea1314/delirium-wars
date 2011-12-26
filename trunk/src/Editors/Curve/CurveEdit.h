#pragma once

#include "../Editor.h"
#include "CurveSelection.h"
#include "Lair/Animation/Curve.h"
#include "../Menu.h"

class CurveEditor : public Editor
{
public:
	virtual void Init();
	virtual void Exit();
	virtual void Render();

	void OnMenuFileSave( int inUnused );
	void OnMenuFileLoad( int inUnused );
	void OnMenuPreLoop( int inPreLoop );
	void OnMenuPostLoop( int inPostLoop );
	void OnMenuAnimate( int inUnused );
	void OnMenuTexture( int inUnused );


protected:
	virtual void OnCreateMenu();


private:
	void RenderCurve();

private:
	CCurveExSelection cs;
	CCurveExSelection* selection;

	Curve c;

	bool animate;
};