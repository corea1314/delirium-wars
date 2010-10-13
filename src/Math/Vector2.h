#ifndef _VECTOR2_H
#define _VECTOR2_H

#include "../macros.h"
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
	
	inline Vector2&	operator+=(const Vector2& v)		{ x+=v.x; y+=v.y; return *this;	}
	inline Vector2&	operator-=(const Vector2& v)		{ x-=v.x; y-=v.y; return *this;	}
	inline Vector2&	operator*=(_tvec2 n)				{ x*=n; y*=n;  return *this;		}
	inline Vector2&	operator/=(_tvec2 n)				{ x/=n; y/=n;  return *this;		}
	
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
