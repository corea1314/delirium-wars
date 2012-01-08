#include "CurveSelection.h"

#include "Lair/Animation/Curve.h"

void CurveSelection::OnDrag( const MouseMotion& mm )
{
	if( selected == 1 ) // point manipulation
	{
		pCurve->GetKey(index).mPosition = (int)mm.pos.x;	//fixme cast might better rounded
		pCurve->GetKey(index).mValue = mm.pos.y;

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
