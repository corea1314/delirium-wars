#pragma once

#include "Lair/Lair.h"

#include "Math/Vector2.h"

class Curve;

class CurveSelection
{
public:
	void Set( Curve* c, int index );
	void OnDrag( const MouseMotion& mm );
	void Erase();

	//	Vector2 GetSelectedPoint();
	int		GetSelectedPointIndex();

private:
	int index;
	Curve* pCurve;
	//	Vector2* pPoint[3];
	int selected;	// 0: handle A
	// 1: point
	// 2: handle B
};