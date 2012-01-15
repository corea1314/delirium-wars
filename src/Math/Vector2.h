#ifndef _VECTOR2_H
#define _VECTOR2_H

#include "macros.h"
#include <math.h>
#include <float.h>
#include <assert.h>

#ifndef _tvec2
#define _tvec2	float
#endif//_tvec2

#include "Matrix2x2.h"

inline double _atan( const double& x, const double& y )	// [0,2PI]
{
	// Triangle target angle with target coordinate
	double a = atan2(x,y);

	// Remove negative angle
	if( a < 0) a += 2*PI;

	// Reverse angle
	a = 2*PI - a;

	return a;
}

class Vector2
{
public:
	_tvec2	x;
	_tvec2	y;

	inline Vector2(void)								{ x = 0; y = 0; }
	inline Vector2(_tvec2 p_fX, _tvec2 p_fY)			{ x = p_fX; y = p_fY; }

	inline void Set(_tvec2 p_fX, _tvec2 p_fY)			{ x = p_fX; y = p_fY; }
	
	inline Vector2	operator-(void) const				{ return Vector2(-x, -y);			}
	inline Vector2	operator+(const Vector2& v)	const	{ return Vector2(x+v.x, y+v.y);		}
	inline Vector2	operator-(const Vector2& v)	const	{ return Vector2(x-v.x, y-v.y);		}
	inline Vector2	operator*(_tvec2 n)	const			{ return Vector2(x*n, y*n);			}
	inline Vector2	operator/(_tvec2 n)	const			{ return Vector2(x/n, y/n);			}
	
	inline Vector2	operator*(Vector2& v)	const		{ return Vector2(x*v.x, y*v.y);		}
	inline Vector2	operator/(Vector2& v)	const		{ return Vector2(x/v.x, y/v.y);		}
	
	inline Vector2&	operator+=(const Vector2& v)		{ x+=v.x; y+=v.y; return *this;		}
	inline Vector2&	operator-=(const Vector2& v)		{ x-=v.x; y-=v.y; return *this;		}
	inline Vector2&	operator*=(_tvec2 n)				{ x*=n; y*=n;  return *this;		}
	inline Vector2&	operator/=(_tvec2 n)				{ x/=n; y/=n;  return *this;		}

	inline Vector2&	operator*=( Vector2& v)				{ x*=v.x; y*=v.y;  return *this;	}
	inline Vector2&	operator/=( Vector2& v)				{ x/=v.x; y/=v.y;  return *this;	}
	
	inline _tvec2&	operator[]( const int i )			{ assert(i<2); return *(&x + i);	}
	inline _tvec2	operator[]( const int i ) const		{ assert(i<2); return *(&x + i);	}
	inline bool		operator==(const Vector2& v) const	{ return x==v.x && y==v.y;			}
	inline bool		operator!=(const Vector2& v) const	{ return !(*this==v);				}

	inline double	operator*( const Vector2& v) const		{ return Vector2::DotProduct(*this,v);	 }
	inline double	operator^( const Vector2& v) const		{ return Vector2::CrossProduct(*this,v); }

	inline bool operator<(const Vector2& v) const { return GetLengthSquare() < v.GetLengthSquare(); }
	
	Vector2& operator *= (const Matrix2x2& M)
	{
		Vector2 T = *this;
		x = T.x * M.e[0][0] + T.y * M.e[0][1];
		y = T.x * M.e[1][0] + T.y * M.e[1][1];
		return *this;
	}

	Vector2& operator ^= (const Matrix2x2& M)
	{
		Vector2 T = *this;
		x = T.x * M.e[0][0] + T.y * M.e[1][0];
		y = T.x * M.e[0][1] + T.y * M.e[1][1];
		return *this;
	}
	
	static double	DotProduct(const Vector2& a, const Vector2& b)	 { return(a.x*b.x + a.y*b.y); }
	static double	CrossProduct(const Vector2& a, const Vector2& b) { return(a.x*b.y - a.y*b.x); }
	static Vector2	GetPerpendicular( const Vector2& v ) {	return Vector2(-v.y,v.x); }

	static double Angle( const Vector2& v ) { return( atan2( (v.y) , (v.x) ) ); }

	static void Rotate( Vector2& v, float a )
	{
		float cosa = (float)cos(a);
		float sina = (float)sin(a);

		float tx = v.x;
		v.x = v.x * cosa - v.y * sina;
		v.y =  tx * sina + v.y * cosa;
	}

	static void RotateAroundCenter( Vector2& v, const Vector2& c, float a )
	{
		Vector2 D = v - c;
		Rotate(D,a);
		v = c + D;
	}

	static double AngleBetweenVectors( const Vector2& in_vVec1, const Vector2& in_vVec2) 
	{							
		double fDot = in_vVec1 * in_vVec2;
		double fCross = in_vVec1 ^ in_vVec2;
		
		// angle between segments
		double fAngle = atan2(fCross, fDot);

		return fAngle;
	}

	static Vector2 GetProjection( Vector2 in_vFrom, Vector2 in_vTo )
	{
		Vector2 vProj(in_vTo);
		vProj = vProj * (float)(Vector2::DotProduct( in_vFrom, in_vTo ) / in_vTo.GetLengthSquare());
		return vProj;
	}
	static double	Clamp( Vector2& _v, float _min, float _max )
	{
		Vector2 c(_v);
		double l  = Vector2::Normalize(c);

		if		( l < _min ) l = _min;
		else if	( _max < l ) l = _max;	 

		_v = ((float)(l)*c);

		return l;
	}

	static double	Normalize( Vector2& in_vToNormalize ) 
	{ 
		double fLen;
		in_vToNormalize = in_vToNormalize.GetNormal(fLen); 
		return fLen;
	}

	static bool IsPointInRect( const Vector2& in_vPoint, const Vector2& in_vMin, const Vector2& in_vMax )
	{
		return	in_vMin.x < in_vPoint.x && in_vPoint.x < in_vMax.x && 
				in_vMin.y < in_vPoint.y && in_vPoint.y < in_vMax.y;
	}

	inline double	GetLength(void) const				{ return sqrt(x*x + y*y );			}
	inline _tvec2	GetLengthFast(void) const	
	{	
		// based on Taylor-Maclaurin serie of expansion
		_tvec2 ax;
		_tvec2 ay;
		
		ax = (_tvec2)fabs(x);
		ay = (_tvec2)fabs(y);

		// compute the minimum of x,y
		_tvec2 mn = MIN(ax,ay);
		
		// length distance with 3.5% error
		return(ax+ay-(mn/2.0f)-(mn/4.0f)+(mn/16.0f));
	}
	inline double	GetLengthSquare(void) const	{ return (x*x + y*y);			}

	inline bool AreOrthogonal(Vector2& v)		{ return (x*v.x + y*v.y) != 0.0f; }

	inline double GetTetaAngle(Vector2& v)
	{
		return( atan( (v.y-y) / (v.x-x) ) );
	}
	inline Vector2 GetNormal () const
	{
		double fDummy;
		return GetNormal(fDummy);
	}
	inline float GetSlope () const
	{
		return y/x;
	}

	inline Vector2 GetNormal ( double& out_fLen ) const
	{		
		out_fLen = GetLength();

		if ( out_fLen == 0 )
		{
			return Vector2( 0, 0 );
		}
		else
		{
			double fLenInverse = 1 / out_fLen;
			return Vector2( x * (float)fLenInverse, y * (float)fLenInverse );
		}
	}

	inline Vector2 GetRotate( float a )
	{
		Vector2 v;
		float cosa = (float)cos(a);
		float sina = (float)sin(a);
		v.x = x * cosa - y * sina;
		v.y = x * sina + y * cosa;
		return v;
	}

	Vector2 Barycentric( const Vector2& value1, const Vector2& value2, const Vector2& value3, float amount1, float amount2 )
	{
		Vector2 vector;
		vector.x = (value1.x + (amount1 * (value2.x - value1.x))) + (amount2 * (value3.x - value1.x));
		vector.y = (value1.y + (amount1 * (value2.y - value1.y))) + (amount2 * (value3.y - value1.y));
		return vector;
	}

	void Barycentric( const Vector2& value1, const Vector2& value2, const Vector2& value3, float amount1, float amount2, Vector2& result )
	{
		result = Vector2((value1.x + (amount1 * (value2.x - value1.x))) + (amount2 * (value3.x - value1.x)),
			(value1.y + (amount1 * (value2.y - value1.y))) + (amount2 * (value3.y - value1.y)) );
	}

	Vector2 CatmullRom( const Vector2& value1, const Vector2& value2, const Vector2& value3, const Vector2& value4, float amount )
	{
		Vector2 vector;
		float squared = amount * amount;
		float cubed = amount * squared;

		vector.x = 0.5f * ((((2.0f * value2.x) + ((-value1.x + value3.x) * amount)) + 
			(((((2.0f * value1.x) - (5.0f * value2.x)) + (4.0f * value3.x)) - value4.x) * squared)) + 
			((((-value1.x + (3.0f * value2.x)) - (3.0f * value3.x)) + value4.x) * cubed));

		vector.y = 0.5f * ((((2.0f * value2.y) + ((-value1.y + value3.y) * amount)) + 
			(((((2.0f * value1.y) - (5.0f * value2.y)) + (4.0f * value3.y)) - value4.y) * squared)) + 
			((((-value1.y + (3.0f * value2.y)) - (3.0f * value3.y)) + value4.y) * cubed));

		return vector;
	}

	void CatmullRom( const Vector2& value1, const Vector2& value2, const Vector2& value3, const Vector2& value4, float amount, Vector2& result )
	{
		float squared = amount * amount;
		float cubed = amount * squared;
		Vector2 r;

		r.x = 0.5f * ((((2.0f * value2.x) + ((-value1.x + value3.x) * amount)) + 
			(((((2.0f * value1.x) - (5.0f * value2.x)) + (4.0f * value3.x)) - value4.x) * squared)) + 
			((((-value1.x + (3.0f * value2.x)) - (3.0f * value3.x)) + value4.x) * cubed));

		r.y = 0.5f * ((((2.0f * value2.y) + ((-value1.y + value3.y) * amount)) + 
			(((((2.0f * value1.y) - (5.0f * value2.y)) + (4.0f * value3.y)) - value4.y) * squared)) + 
			((((-value1.y + (3.0f * value2.y)) - (3.0f * value3.y)) + value4.y) * cubed));

		result = r;
	}
		
	Vector2 Hermite( const Vector2& value1, const Vector2& tangent1, const Vector2& value2, const Vector2& tangent2, float amount )
	{
		Vector2 vector;
		float squared = amount * amount;
		float cubed = amount * squared;
		float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
		float part2 = (-2.0f * cubed) + (3.0f * squared);
		float part3 = (cubed - (2.0f * squared)) + amount;
		float part4 = cubed - squared;

		vector.x = (((value1.x * part1) + (value2.x * part2)) + (tangent1.x * part3)) + (tangent2.x * part4);
		vector.y = (((value1.y * part1) + (value2.y * part2)) + (tangent1.y * part3)) + (tangent2.y * part4);

		return vector;
	}	

	void Hermite( const Vector2& value1, const Vector2& tangent1, const Vector2& value2, const Vector2& tangent2, float amount, Vector2& result )
	{
		float squared = amount * amount;
		float cubed = amount * squared;
		float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
		float part2 = (-2.0f * cubed) + (3.0f * squared);
		float part3 = (cubed - (2.0f * squared)) + amount;
		float part4 = cubed - squared;

		Vector2 r;
		r.x = (((value1.x * part1) + (value2.x * part2)) + (tangent1.x * part3)) + (tangent2.x * part4);
		r.y = (((value1.y * part1) + (value2.y * part2)) + (tangent1.y * part3)) + (tangent2.y * part4);

		result = r;
	}

	Vector2 Lerp( Vector2 start, Vector2 end, float factor )
	{
		Vector2 vector;

		vector.x = start.x + ((end.x - start.x) * factor);
		vector.y = start.y + ((end.y - start.y) * factor);

		return vector;
	}

	void Lerp( const Vector2& start, const Vector2& end, float factor, Vector2& result )
	{
		Vector2 r;
		r.x = start.x + ((end.x - start.x) * factor);
		r.y = start.y + ((end.y - start.y) * factor);

		result = r;
	}

	Vector2 SmoothStep( const Vector2& start, const Vector2& end, float amount )
	{
		Vector2 vector;

		amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
		amount = (amount * amount) * (3.0f - (2.0f * amount));

		vector.x = start.x + ((end.x - start.x) * amount);
		vector.y = start.y + ((end.y - start.y) * amount);

		return vector;
	}

	void SmoothStep( const Vector2& start, const Vector2& end, float amount, Vector2& result )
	{
		amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
		amount = (amount * amount) * (3.0f - (2.0f * amount));

		Vector2 r;
		r.x = start.x + ((end.x - start.x) * amount);
		r.y = start.y + ((end.y - start.y) * amount);

		result = r;
	}

	friend Vector2 operator*(float fl, const Vector2& v) ;	
	friend Vector2 operator*( const Vector2& v, const Matrix2x2& M);
	friend Vector2 operator^( const Vector2& v, const Matrix2x2& M);

};

inline Vector2 operator*(float fl, const Vector2& v)  { return v * fl; }

inline Vector2 operator*( const Vector2& v, const Matrix2x2& M)
{
	Vector2 T;
	T.x = v.x * (M.e[0][0]) + v.y * (M.e[0][1]);
	T.y = v.x * (M.e[1][0]) + v.y * (M.e[1][1]);
	return T;
}

inline Vector2 operator^( const Vector2& v, const Matrix2x2& M) 
{
	Vector2 T;
	T.x = v.x * (M.e[0][0]) + v.y * (M.e[1][0]);
	T.y = v.x * (M.e[0][1]) + v.y * (M.e[1][1]);
	return T;
}

#endif//_VECTOR2_H
