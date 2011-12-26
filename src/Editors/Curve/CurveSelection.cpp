#include "CurveSelection.h"

#include "Lair/Animation/Curve.h"

void CCurveExSelection::OnDrag( float x, float y, float dx, float dy )
{

#define SNAP( _v )			\
	if( g_CurveEditor.snap )		\
	{						\
	float d = 0.5f * (1.0f / g_CurveEditor.grid_size);\
	(_v).x = ((int)(((_v).x + d) * g_CurveEditor.grid_size)) / (float)g_CurveEditor.grid_size;	\
	(_v).y = ((int)(((_v).y + d) * g_CurveEditor.grid_size)) / (float)g_CurveEditor.grid_size;	\
}

	if( selected == 1 ) // point manipulation
	{
		pCurve->GetKey(index).mPosition = x;
		pCurve->GetKey(index).mValue = y;

		pCurve->Update();
	}
	else
	{
		// handle manipulation
		// TODO
	}

	//TODO	pCurve->Update();
}

void CCurveExSelection::Erase()
{
	// todo
}

int CCurveExSelection::GetSelectedPointIndex()
{
	return index;
}

void CCurveExSelection::Set( Curve* c, int _index )
{
	index = _index;
	pCurve = c;
	
//TODO	if( c->IsPoint(index) )
	{
		selected = 1;
		/*
		if( index > 0 )
			pPoint[0] = &pCurve->PointList[index-1];
		else
			pPoint[0] = 0;

		pPoint[1] = &pCurve->PointList[index];

		if( index < pCurve->PointList.size()-1 )
			pPoint[2] = &pCurve->PointList[index+1];
		else
			pPoint[2] = 0;
		*/
	}
/*
//TODO
	else
	{
		if( index == 1 )
		{
			// right handle: 2
			selected = 2;
			pPoint[0] = 0;
			pPoint[1] = &pCurve->PointList[index-1];
			pPoint[2] = &pCurve->PointList[index];
		}
		else if( index == pCurve->PointList.size()-2 )
		{
			// right handle: 0
			selected = 0;
			pPoint[0] = &pCurve->PointList[index];
			pPoint[1] = &pCurve->PointList[index+1];
			pPoint[2] = 0;
		}
		else
		{
			if( (index + 1)%3 == 0 )
			{
				selected = 0;
				pPoint[0] = &pCurve->PointList[index];
				pPoint[1] = &pCurve->PointList[index+1];
				pPoint[2] = &pCurve->PointList[index+2];
			}
			else if( (index - 1)%3 == 0 )
			{
				selected = 2;
				pPoint[0] = &pCurve->PointList[index-2];
				pPoint[1] = &pCurve->PointList[index-1];
				pPoint[2] = &pCurve->PointList[index];
			}

		}
	}
*/
}
