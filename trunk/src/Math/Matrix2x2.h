
#ifndef _MATRIX2X2_H
#define _MATRIX2X2_H

#include <math.h>

class Matrix2x2
{
public:

	float e[2][2];

	Matrix2x2(float _e00, float _e01, float _e10, float _e11)
	{
		e[0][0] = _e00;
		e[0][1] = _e01;
		e[1][0] = _e10;
		e[1][1] = _e11;
	}
	
	Matrix2x2()
	{
		// create identity matrix
		e[0][0] = 1.0f;
		e[0][1] = 0.0f;
		e[1][0] = 0.0f;
		e[1][1] = 1.0f;
	}

	Matrix2x2(float angle)
	{
		float c = (float)cos(angle);
		float s = (float)sin(angle);

		e[0][0] = c; e[0][1] = s;
		e[1][0] =-s; e[1][1] = c;
	}

	float  operator()(int i, int j) const { return e[i][j]; }
	float& operator()(int i, int j)       { return e[i][j]; }

	/*
	const Vector2& operator[](int i) const
	{
		return reinterpret_cast<const Vector2&>(e[i][0]);
	}
	
	Vector2& operator[](int i)
	{
		return reinterpret_cast<Vector2&>(e[i][0]);
	}
	*/

	Matrix2x2 Tranpose() const
	{
		Matrix2x2 T;

		T.e[0][0] = e[0][0];
		T.e[1][0] = e[0][1];
		T.e[0][1] = e[1][0];
		T.e[1][1] = e[1][1];

		return T;
	}

	Matrix2x2 operator * (const Matrix2x2& M) const 
	{
		Matrix2x2 T;

		T.e[0][0] = e[0][0] * M.e[0][0] + e[0][1] * M.e[1][0];
		T.e[1][0] = e[1][0] * M.e[0][0] + e[1][1] * M.e[1][0];
		T.e[0][1] = e[0][0] * M.e[0][1] + e[0][1] * M.e[1][1];
		T.e[1][1] = e[1][0] * M.e[0][1] + e[1][1] * M.e[1][1];
		
		return T;
	}

	Matrix2x2 operator ^ (const Matrix2x2& M) const 
	{
		Matrix2x2 T;

		T.e[0][0] = e[0][0] * M.e[0][0] + e[0][1] * M.e[0][1];
		T.e[1][0] = e[1][0] * M.e[0][0] + e[1][1] * M.e[0][1];
		T.e[0][1] = e[0][0] * M.e[1][0] + e[0][1] * M.e[1][1];
		T.e[1][1] = e[1][0] * M.e[1][0] + e[1][1] * M.e[1][1];
		
		return T;
	}

	inline Matrix2x2 operator * ( float s) const
	{
		Matrix2x2 T;

		T.e[0][0] = e[0][0] * s;
		T.e[1][0] = e[1][0] * s;
		T.e[0][1] = e[0][1] * s;
		T.e[1][1] = e[1][1] * s;
		
		return T;
	}

	/*
	Vector2 operator * (const Vector2& V) const
	{
		Vector2 T;
		T.x = V.x * e[0][0] + V.y * e[0][1];
		T.y = V.x * e[1][0] + V.y * e[1][1];
		return T;
	}

	Vector2 operator ^ (const Vector2& V) const
	{
		Vector2 T;
		T.x = V.x * e[0][0] + V.y * e[1][0];
		T.y = V.x * e[0][1] + V.y * e[1][1];
		return T;
	}
	*/

	static const	Matrix2x2	I;
	static const	Matrix2x2	ZERO;
};

#endif//_MATRIX2X2_H
