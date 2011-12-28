#include "CurveSelection.h"

#include "Lair/Animation/Curve.h"

void CurveSelection::OnDrag( Vector2 v, Vector2 d )
{

#define SNAP( _v )			\
	if( g_CurveEditor.mIsSnapping )		\
	{						\
	float d = 0.5f * (1.0f / g_CurveEditor.mSize);\
	(_v).x = ((int)(((_v).x + d) * g_CurveEditor.mSize)) / (float)g_CurveEditor.mSize;	\
	(_v).y = ((int)(((_v).y + d) * g_CurveEditor.mSize)) / (float)g_CurveEditor.mSize;	\
}

	if( selected == 1 ) // point manipulation
	{
		pCurve->GetKey(index).mPosition = (int)v.x;	//fixme cast might better rounded
		pCurve->GetKey(index).mValue = v.y;

		pCurve->Update();
	}
	else
	{
		// handle manipulation
		// TODO
	}

	//TODO	pCurve->Update();
}

void CurveSelection::Erase()
{
	// todo
}

int CurveSelection::GetSelectedPointIndex()
{
	return index;
}

void CurveSelection::Set( Curve* c, int _index )
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
