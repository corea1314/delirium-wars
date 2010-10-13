#ifdef _MSC_VER
#	pragma warning( disable : 4530 )
#	pragma warning( disable : 4786 )
#endif

#ifndef _VECTOR3_H_
#define _VECTOR3_H_ 

#include "Macros.h"

#include <math.h>
#include <float.h>
#include <assert.h>

#ifndef _tvec3
#define _tvec3	float
#endif//_tvec3

class Vector3
{		
public:
	// Construction/destruction
	inline Vector3(void)								{ x= 0; y= 0; z= 0;						}
	inline Vector3(_tvec3 X, _tvec3 Y, _tvec3 Z)		{ x = X; y = Y; z = Z;					}
	inline Vector3(const Vector3& v)					{ x = v.x; y = v.y; z = v.z;			} 
	inline Vector3(_tvec3 rgfl[3])						{ x = rgfl[0]; y = rgfl[1]; z = rgfl[2];}

	// Operators
	inline Vector3 operator-(void) const				{ return Vector3(-x,-y,-z);			}
	inline Vector3 operator+(const Vector3& v) const	{ return Vector3(x+v.x, y+v.y, z+v.z);	}
	inline Vector3 operator-(const Vector3& v) const	{ return Vector3(x-v.x, y-v.y, z-v.z);	}
	inline Vector3 operator*(_tvec3 fl) const			{ return Vector3(x*fl, y*fl, z*fl);		}
	inline Vector3 operator/(_tvec3 fl) const			{ return Vector3(x/fl, y/fl, z/fl);		}

	inline Vector3& operator+=(const Vector3& v) 		{ x+=v.x; y+=v.y; z+=v.z;  return *this; 	}
	inline Vector3& operator-=(const Vector3& v) 		{ x-=v.x; y-=v.y; z-=v.z;  return *this; 	}
	inline Vector3& operator*=(_tvec3 fl) 				{ x*=fl; y*=fl, z*=fl;  return *this; 		}
	inline Vector3& operator/=(_tvec3 fl) 				{ x/=fl; y/=fl, z/=fl;  return *this; 		}
	inline Vector3& operator =(const Vector3& v) 		{ x = v.x; y = v.y; z = v.z; return *this;	}

	inline double operator*( const Vector3& v) const	{ return Vector3::DotProduct(*this,v);	 }
	inline Vector3 operator^( const Vector3& v) const	{ return Vector3::CrossProduct(*this,v); }

	inline _tvec3&	operator[]( const int i )			{ assert(i<3); return *(&x + i);	}
	inline _tvec3	operator[]( const int i ) const		{ assert(i<3); return *(&x + i);	}

	inline bool operator==(const Vector3& v) const		{ return x==v.x && y==v.y && z==v.z;	}
	inline bool operator!=(const Vector3& v) const		{ return !(*this==v);					}

	inline bool operator<(const Vector3& v) const { return GetLengthSquare() < v.GetLengthSquare(); }
	
	// Methods
	inline void		Set( _tvec3 X, _tvec3 Y, _tvec3 Z )	{ x = X; y = Y; z = Z;					}
	
	inline double	GetLength(void) const				{ return sqrt(x*x + y*y + z*z);	}
	inline float	GetLengthFast(void) const				
	{
		// based on Taylor-Maclaurin serie of expansion
		int temp;  // used for swaping
		int fax,fay,faz; // used for algorithm

		// make sure values are all positive
		fax = /*(float)*/(int)(ABS(x) * 1024);
		fay = /*(float)*/(int)(ABS(y) * 1024);
		faz = /*(float)*/(int)(ABS(z) * 1024);

		// sort values
		if (fay < fax) SWAP(fax,fay,temp)
		if (faz < fay) SWAP(fay,faz,temp)
		if (fay < fax) SWAP(fax,fay,temp)
		int dist = (faz + 11*(fay >> 5) + (fax >> 2) );

		// compute length with 8% error
		return((float)(dist >> 10));
	}
	inline double	GetLengthSquare() const			{ return (x*x + y*y + z*z); }
		
	inline int AreOrthogonal(Vector3& v)		{ return (x*v.x + y*v.y + z*v.z) != 0.0f; }

	inline Vector3 GetNormal( double& out_fLen ) const
	{
		out_fLen = GetLength();
		if (out_fLen == 0) return Vector3(0,0,0); // ????
		double fLenInverse = 1 / out_fLen;
		return Vector3( (float)(x * fLenInverse), (float)(y * fLenInverse), (float)(z * fLenInverse) );
	}

	inline Vector3 GetNormal(void) const
	{
		double flLen;
		return GetNormal(flLen);
	}

	// Multiply, add, and assign to this (ie: *this = *this + v * k). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	inline void MultiplyAdd( float k, const Vector3 v )
	{
		x = x*k + v.x;
		y = y*k + v.y;
		z = z*k + v.z;
	}

	// Members
	_tvec3 x; 
	_tvec3 y; 
	_tvec3 z;

	static double		DotProduct(const Vector3& a, const Vector3& b)  { return(a.x*b.x+a.y*b.y+a.z*b.z); }
	static Vector3		CrossProduct(const Vector3& a, const Vector3& b)  { return Vector3( a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x ); }
	static double		AngleBetweenVectors( const Vector3& in_vVec1, const Vector3& in_vVec2) 
	{							
		// Get the dot product of the vectors
		double fDotProduct = DotProduct(in_vVec1, in_vVec2);				

		// Get the product of both of the vectors magnitudes
		double fVectorsMagnitude = in_vVec1.GetLength() * in_vVec2.GetLength();

		double fArc = fDotProduct / fVectorsMagnitude;

		if( fArc > 1.0f )
			return 0.0f;
		else if( fArc < -1.0f )
			return PI;
		
		return acos( fArc );
	}
	static Vector3 GetProjection( const Vector3& in_vFrom, const Vector3& in_vTo )
	{
		Vector3 vProj(in_vTo);
		vProj *= (float)(Vector3::DotProduct( in_vFrom, in_vTo ) / in_vTo.GetLengthSquare());
		return vProj;
	}
	static double	Normalize( Vector3& in_vToNormalize ) 
	{
		double fLen;
		in_vToNormalize = in_vToNormalize.GetNormal(fLen); 
		return fLen;
	}
};

inline Vector3		operator*(float fl, const Vector3& v)	{ return v * fl; }

#endif	// _VECTOR3_H_
