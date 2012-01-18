#include "Curve.h"

#include <algorithm>

#include <tinyxml/tinyxml.h>


Curve::Curve()
	: mPostLoop(CurveLoopType::Linear)
	, mPreLoop(CurveLoopType::Linear)
{
	AddKey( 0, 0.0f );	//fixme, should never have to add a key, if curve has no keys, then it should not exist
}

void Curve::AddKey( int inPosition, float inValue )
{
	std::vector<Key>::iterator itKey = FindKey(inPosition);

	if( itKey != mKeys.end() )
	{
		// key at this position already exists, so update value
		itKey->mValue = inValue;
	}
	else
	{
		// no key found to add it
		mKeys.push_back( Key(inPosition, inValue, Vector2(-1,0), Vector2(1,0), CurveContinuity::Smooth ) );
	}

	Update();	//fixme, overkill to recompute everything for a single key add
}

void Curve::AddKey( int inPosition, float inValue, const Vector2& inTangentIn, const Vector2& inTangentOut, CurveContinuity::E inContinuity )
{
	std::vector<Key>::iterator itKey = FindKey(inPosition);

	if( itKey != mKeys.end() )
	{
		// key at this position already exists, so update value
		itKey->mValue = inValue;
		itKey->mTangentInVector = inTangentIn;
		itKey->mTangentOutVector = inTangentOut;
		itKey->mContinuity = inContinuity;
	}
	else
	{
		// no key found to add it
		mKeys.push_back( Key(inPosition, inValue, inTangentIn.GetNormal(), inTangentOut.GetNormal(), inContinuity ) );
	}
	
	Update();	//fixme, overkill to recompute everything for a single key add
}

std::vector<Curve::Key>::iterator Curve::FindKey( int inPosition )
{
	std::vector<Curve::Key>::iterator itKey = std::find( mKeys.begin(), mKeys.end(), Key(inPosition) );
	return itKey;
}

void Curve::Update()
{
	if( mKeys.size() < 2 )	//FIXME
		return;

	std::sort( mKeys.begin(), mKeys.end() );
	CalculateTangents();
}

void Curve::CalculateTangents()
{		
	float fDx;
	const unsigned int nKeyCount = mKeys.size();
	for(unsigned int i=0;i<nKeyCount-1;i++)
	{
		fDx = (float)mKeys[i+1].mPosition - mKeys[i].mPosition;
		mKeys[i].mTangentOut = mKeys[i].mTangentOutVector.GetSlope() * fDx;
		mKeys[i+1].mTangentIn = mKeys[i+1].mTangentInVector.GetSlope() * fDx;
	}

	// first key in tangent and last key out tangent
	mKeys[0].mTangentIn = mKeys[0].mTangentInVector.GetSlope();
	mKeys[nKeyCount-1].mTangentOut = mKeys[nKeyCount-1].mTangentOutVector.GetSlope();
}

float Curve::Evaluate(float position)
{		
	Key first = mKeys[0];
	Key last = mKeys[mKeys.size() - 1];

	if (position < first.mPosition)
	{
		int cycle;
		float virtualPos;
		switch (mPreLoop)
		{
		case CurveLoopType::Constant:
			//constant
			return first.mValue;

		case CurveLoopType::Linear:
			// linear y = a*x +b with a tangeant of last point
			return first.mValue - first.mTangentIn * (first.mPosition - position);

		case CurveLoopType::Cycle:
			//start -> end / start -> end
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			return GetCurveValue(virtualPos);

		case CurveLoopType::CycleOffset:
			//make the curve continue (with no step) so must up the curve each cycle of delta(value)
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			return (GetCurveValue(virtualPos) + cycle * (last.mValue - first.mValue));

		case CurveLoopType::Oscillate:
			//go back on curve from end and target start 
			// start-> end / end -> start
			cycle = GetNumberOfCycle(position);
			if (0 == cycle % 2)//if pair
				virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			else
				virtualPos = last.mPosition - position + first.mPosition + (cycle * (last.mPosition - first.mPosition));
			return GetCurveValue(virtualPos);
		}
	}
	else if (position > last.mPosition)
	{
		int cycle;
		float virtualPos;
		switch (mPostLoop)
		{
		case CurveLoopType::Constant:
			//constant
			return last.mValue;

		case CurveLoopType::Linear:
			// linear y = a*x +b with a tangeant of last point
			return last.mValue + last.mTangentOut * (position - last.mPosition);

		case CurveLoopType::Cycle:
			//start -> end / start -> end
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			return GetCurveValue(virtualPos);

		case CurveLoopType::CycleOffset:
			//make the curve continue (with no step) so must up the curve each cycle of delta(value)
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			return (GetCurveValue(virtualPos) + cycle * (last.mValue - first.mValue));

		case CurveLoopType::Oscillate:
			//go back on curve from end and target start 
			// start-> end / end -> start
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			if (0 == cycle % 2)//if pair
				virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			else
				virtualPos = last.mPosition - position + first.mPosition + (cycle * (last.mPosition - first.mPosition));
			return GetCurveValue(virtualPos);
		}
	}

	//in curve
	return GetCurveValue(position);
}

float Curve::EvaluateTangent(float position)
{		
	Key first = mKeys[0];
	Key last = mKeys[mKeys.size() - 1];
	
	if (position < first.mPosition)
	{
		int cycle;
		float virtualPos;
		float cycleDir;
		switch (mPreLoop)
		{
		case CurveLoopType::Constant:
			//constant
			return 0.0f;

		case CurveLoopType::Linear:
			// linear y = a*x +b with a tangeant of last point
			return first.mTangentIn;

		case CurveLoopType::Cycle:
			//start -> end / start -> end
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			return GetCurveTangent(virtualPos);

		case CurveLoopType::CycleOffset:
			//make the curve continue (with no step) so must up the curve each cycle of delta(value)
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			return GetCurveTangent(virtualPos);

		case CurveLoopType::Oscillate:
			//go back on curve from end and target start 
			// start-> end / end -> start
			cycle = GetNumberOfCycle(position);
			if (0 == cycle % 2)//if pair
			{
				cycleDir = 1.0f;
				virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			}
			else
			{
				cycleDir = -1.0f;
				virtualPos = last.mPosition - position + first.mPosition + (cycle * (last.mPosition - first.mPosition));
			}
			return cycleDir * GetCurveTangent(virtualPos);
		}
	}
	else if (position > last.mPosition)
	{
		int cycle;
		float virtualPos;
		float cycleDir;

		switch (mPostLoop)
		{
		case CurveLoopType::Constant:
			//constant
			return 0.0f;

		case CurveLoopType::Linear:
			// linear y = a*x +b with a tangeant of last point
			return last.mTangentOut;

		case CurveLoopType::Cycle:
			//start -> end / start -> end
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			return GetCurveTangent(virtualPos);

		case CurveLoopType::CycleOffset:
			//make the curve continue (with no step) so must up the curve each cycle of delta(value)
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			return GetCurveTangent(virtualPos);

		case CurveLoopType::Oscillate:
			//go back on curve from end and target start 
			// start-> end / end -> start
			cycle = GetNumberOfCycle(position);
			virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			if (0 == cycle % 2)//if pair
			{
				cycleDir = 1.0f;
				virtualPos = position - (cycle * (last.mPosition - first.mPosition));
			}
			else
			{
				cycleDir = -1.0f;
				virtualPos = last.mPosition - position + first.mPosition + (cycle * (last.mPosition - first.mPosition));
			}
			return cycleDir * GetCurveTangent(virtualPos);
		}
	}

	//in curve
	return GetCurveTangent(position);
}

int Curve::GetNumberOfCycle(float position)
{
	float cycle = (position - mKeys[0].mPosition) / (mKeys[mKeys.size() - 1].mPosition - mKeys[0].mPosition);
	if (cycle < 0.0f)
		cycle--;
	return (int)cycle;
}

float Curve::GetCurveValue(float position)
{
	//only for position in curve
	Key prev = mKeys[0];
	Key next;
	for (unsigned int i = 1; i < mKeys.size(); i++)
	{
		next = mKeys[i];
		if (next.mPosition >= position)
		{
			if (prev.mContinuity == CurveContinuity::Step)
			{
				if (position >= 1.0f)
				{
					return next.mValue;
				}
				return prev.mValue;
			}
			return GetValueAt( position, prev, next );
		}
		prev = next;
	}
	return mKeys[0].mValue;
}

float Curve::GetCurveTangent(float position)
{
	//only for position in curve
	Key prev = mKeys[0];
	Key next;
	for (unsigned int i = 1; i < mKeys.size(); i++)
	{
		next = mKeys[i];
		if (next.mPosition >= position)
		{
			/* TODO
			if (prev.Continuity == CurveContinuity.Step)
			{
				if (position >= 1.0f)
				{
					return next.mValue;
				}
				return prev.mValue;
			}
			*/
			return GetTangentAt( position, prev, next );
		}
		prev = next;
	}
	return mKeys[0].mTangentOut;
}


float Curve::GetValueAt( float inPosition, Key inKeyPrev, Key inKeyNext )
{
	float t = (inPosition - inKeyPrev.mPosition) / (inKeyNext.mPosition - inKeyPrev.mPosition); //to have t in [0,1]
	float ts = t * t;
	float tss = ts * t;
	
	//P(t) = (2*t^3 - 3t^2 + 1)*P0 + (t^3 - 2t^2 + t)m0 + (-2t^3 + 3t^2)P1 + (t^3-t^2)m1
	//with P0.mValue = prev.mValue , m0 = prev.mTangentOut, P1= next.mValue, m1 = next.mTangentIn
	return (2.0f * tss - 3.0f * ts + 1.0f) * inKeyPrev.mValue + (tss - 2.0f * ts + t) * inKeyPrev.mTangentOut + (3.0f * ts - 2.0f * tss) * inKeyNext.mValue + (tss - ts) * inKeyNext.mTangentIn;
}

float Curve::GetTangentAt( float inPosition, Key inKeyPrev, Key inKeyNext )
{
	float dv = (float)(inKeyNext.mPosition - inKeyPrev.mPosition);
	float t = (inPosition - inKeyPrev.mPosition) / dv; //to have t in [0,1]
	float ts = t * t;

	// T(t) = ( (6*t^2 - 6t)*P0 + (3t^2 - 4t + 1)m0 + (-6t^2 + 6t)P1 + (3t^2-2t)m1 ) / (Xmax - Xmin)

	// T(t) = dy/dx
	// y = P(t)
	// x = (Xmax - Xmin) * t + Xmin
	
	return ((6.0f * ts - 6.0f * t) * inKeyPrev.mValue + (3.0f*ts - 4.0f * t + 1.0f) * inKeyPrev.mTangentOut + (6.0f * t - 6.0f * ts) * inKeyNext.mValue + (3.0f*ts - 2.0f*t) * inKeyNext.mTangentIn) / dv;
}

void Curve::SerializeLoad( TiXmlElement* inNodeCurve )
{
	const char* szPreLoop = inNodeCurve->Attribute( "PreLoop" );
	if( szPreLoop )
	{
			 if( _stricmp(szPreLoop, "Constant" ) == 0 )	SetPreLoop( CurveLoopType::Constant );
		else if( _stricmp(szPreLoop, "Cycle" ) == 0 )		SetPreLoop( CurveLoopType::Cycle );
		else if( _stricmp(szPreLoop, "CycleOffset" ) == 0 )	SetPreLoop( CurveLoopType::CycleOffset );
		else if( _stricmp(szPreLoop, "Oscillate" ) == 0 )	SetPreLoop( CurveLoopType::Oscillate );
		else if( _stricmp(szPreLoop, "Linear" ) == 0 )		SetPreLoop( CurveLoopType::Linear );
		else												SetPreLoop( CurveLoopType::Linear );
	}

	const char* szPostLoop = inNodeCurve->Attribute( "PostLoop" );
	if( szPostLoop )
	{
			 if( _stricmp(szPostLoop, "Constant" ) == 0 )	SetPostLoop( CurveLoopType::Constant );
		else if( _stricmp(szPostLoop, "Cycle" ) == 0 )		SetPostLoop( CurveLoopType::Cycle );
		else if( _stricmp(szPostLoop, "CycleOffset" ) == 0 )SetPostLoop( CurveLoopType::CycleOffset );
		else if( _stricmp(szPostLoop, "Oscillate" ) == 0 )	SetPostLoop( CurveLoopType::Oscillate );
		else if( _stricmp(szPostLoop, "Linear" ) == 0 )		SetPostLoop( CurveLoopType::Linear );
		else												SetPostLoop( CurveLoopType::Linear );
	}		

	int nPosition;
	float fValue;
	double fAttribute;
	Vector2 vTangentIn;
	Vector2 vTangentOut;
	CurveContinuity::E eCurveContinuity;
	const char* szCurveContinuity;

	ClearKeys();	// fixme should be better handled

	for( TiXmlElement* pxmlKey = inNodeCurve->FirstChildElement("key"); pxmlKey; pxmlKey = pxmlKey->NextSiblingElement("key") )
	{			
		pxmlKey->Attribute( "position", &nPosition );
		pxmlKey->Attribute( "value", &fAttribute ); fValue = (float)fAttribute;
		pxmlKey->Attribute( "tangent.in.x", &fAttribute );	vTangentIn.x = (float)fAttribute;
		pxmlKey->Attribute( "tangent.in.y", &fAttribute );	vTangentIn.y = (float)fAttribute;
		pxmlKey->Attribute( "tangent.out.x", &fAttribute );	vTangentOut.x = (float)fAttribute;
		pxmlKey->Attribute( "tangent.out.y", &fAttribute );	vTangentOut.y = (float)fAttribute;
		szCurveContinuity = pxmlKey->Attribute( "continuity" );

		if( szCurveContinuity )
		{
				 if( _stricmp( szCurveContinuity, "Smooth" ) == 0 )		eCurveContinuity = CurveContinuity::Smooth;
			else if( _stricmp( szCurveContinuity, "Step" ) == 0 )		eCurveContinuity = CurveContinuity::Step;
		}

		AddKey( nPosition, fValue, vTangentIn, vTangentOut, eCurveContinuity );
	}		
}

void Curve::SerializeSave( TiXmlElement* inNodeCurve )
{
	const char *  szCurveLoopType[] = { "Constant", "Cycle", "CycleOffset", "Oscillate", "Linear" };

	inNodeCurve->SetAttribute( "PreLoop", szCurveLoopType[GetPreLoop()] );
	inNodeCurve->SetAttribute( "PostLoop", szCurveLoopType[GetPostLoop()] );

	const char *  szCurveContinuity[] = { "Smooth", "Step" };

	TiXmlElement* pxmlKey;
	for( unsigned int i=0;i<GetKeyCount(); i++ )
	{
		pxmlKey = new TiXmlElement("key");
		pxmlKey->SetAttribute( "position", GetKey(i).mPosition );
		pxmlKey->SetDoubleAttribute( "value", GetKey(i).mValue );
		pxmlKey->SetDoubleAttribute( "tangent.in.x", GetKey(i).mTangentInVector.x );
		pxmlKey->SetDoubleAttribute( "tangent.in.y", GetKey(i).mTangentInVector.y );
		pxmlKey->SetDoubleAttribute( "tangent.out.x", GetKey(i).mTangentOutVector.x );
		pxmlKey->SetDoubleAttribute( "tangent.out.y", GetKey(i).mTangentOutVector.y );
		pxmlKey->SetAttribute( "continuity", szCurveContinuity[GetKey(i).mContinuity] );

		inNodeCurve->LinkEndChild(pxmlKey);
	}
}
