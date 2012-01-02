
#ifndef _CURVE_H
#define _CURVE_H

#include "Math/Vector2.h"

#define CRV_VERSION_MAIN	1
#define CRV_VERSION_MAJOR	0
#define CRV_VERSION_MINOR	0

#include <vector>

class CurveLoopType { public: enum E {
	Constant,
	Cycle,
	CycleOffset,
	Oscillate,
	Linear
}; };

class CurveContinuity {  public: enum E {
	Smooth,
	Step
}; };


class Curve
{
public:
	Curve();

	class Key
	{
	public:
		Key(){}
		Key( int inPosition, float inValue, const Vector2& inTangentInVector, const Vector2& inTangentOutVector, CurveContinuity::E inContinuity ) 
			: mPosition(inPosition)
			, mValue(inValue)
			, mTangentInVector(inTangentInVector)
			, mTangentIn(0.0f)
			, mTangentOutVector(inTangentOutVector)
			, mTangentOut(0.0f)
			, mContinuity(inContinuity)
		{
		}

		CurveContinuity::E mContinuity;
		float mValue;
		int mPosition;
		Vector2 mTangentInVector, mTangentOutVector;
		float mTangentIn, mTangentOut;	// slope y/dx (dx being distance between next key)

		inline bool operator<( const Key& inKey )	{ return mPosition < inKey.mPosition;	}
	};

	void AddKey( int inPosition, float inValue );
	void AddKey( int inPosition, float inValue, const Vector2& inTangentIn, const Vector2& inTangentOut, CurveContinuity::E inContinuity );
	void CalculateTangents();
	void ClearKeys() { mKeys.clear(); }

	void Update();

	float Evaluate(float position);
	float EvaluateTangent(float position);

	Vector2 GetTangentInVector( unsigned int inIndex );
	Vector2 GetTangentOutVector( unsigned int inIndex );

	inline void SetPreLoop( CurveLoopType::E inPreLoop ) { mPreLoop = inPreLoop; }
	inline void SetPostLoop( CurveLoopType::E inPostLoop) { mPostLoop = inPostLoop; }

	inline Key& GetKey( unsigned int inIndex ) { return mKeys[inIndex]; }
	inline unsigned int GetKeyCount() const { return mKeys.size(); }

	inline CurveLoopType::E	GetPreLoop() const { return mPreLoop; }
	inline CurveLoopType::E	GetPostLoop() const { return mPostLoop; }

protected:

	int GetNumberOfCycle(float position);

	float GetCurveValue(float position);
	float GetCurveTangent(float position);

	float GetValueAt( float inPosition, Key inKeyPrev, Key inKeyNext );
	float GetTangentAt( float inPosition, Key inKeyPrev, Key inKeyNext );

private:
	std::vector<Key>	mKeys;
	CurveLoopType::E	mPreLoop;
	CurveLoopType::E	mPostLoop;

};

#endif//_CURVE_H
