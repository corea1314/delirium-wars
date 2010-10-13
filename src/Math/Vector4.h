#ifndef __VECTOR4_H_
#define __VECTOR4_H_ 

#include "Macros.h"
#include <float.h>
#include <assert.h>

#ifndef _tvec4
#define _tvec4	float
#endif//_tvec4

#include "Vector3.h"

class Vector4
{		
public:
	// Construction/destruction
	inline Vector4(void)										{ x= 0; y= 0; z= 0;	w = 1;				}
	inline Vector4(_tvec4 X, _tvec4 Y, _tvec4 Z, _tvec4 W = 1)	{ x = X; y = Y; z = Z;	w = W;			}
	inline Vector4(const Vector3& v)							{ x = v.x; y = v.y; z = v.z; w = 1;		} 
	inline Vector4(const Vector4& v)							{ x = v.x; y = v.y; z = v.z; w = v.w;	} 
	inline Vector4(_tvec4 rgfl[3])								{ x = rgfl[0]; y = rgfl[1]; z = rgfl[2]; w = rgfl[3];}

	// Operators
	inline Vector4 operator-(void) const				{ return Vector4(-x,-y,-z,-w);			}
	inline Vector4 operator+(const Vector4& v) const	{ return Vector4(x+v.x, y+v.y, z+v.z, w+v.w);	}
	inline Vector4 operator-(const Vector4& v) const	{ return Vector4(x-v.x, y-v.y, z-v.z,w-v.w);	}
	inline Vector4 operator*(float fl) const			{ return Vector4(x*fl, y*fl, z*fl, w*fl);		}
	inline Vector4 operator/(float fl) const			{ return Vector4(x/fl, y/fl, z/fl, w/fl);		}

	inline Vector4& operator+=(const Vector4& v) 			{ x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this;	}
	inline Vector4& operator-=(const Vector4& v) 			{ x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this;	}
	inline Vector4& operator*=(_tvec4 fl) 					{ x*=fl; y*=fl, z*=fl; w*=fl; return *this;		}
	inline Vector4& operator/=(_tvec4 fl) 					{ x/=fl; y/=fl, z/=fl; w*=fl; return *this;		}
//	inline Vector4& operator =(const Vector4& v) 			{ x=v.x; y=v.y;z = v.z; w = v.w; return *this;}
	
	inline _tvec4&	operator[]( const int i )			{ assert(i<4); return *(&x + i);	}
	inline _tvec4	operator[]( const int i ) const		{ assert(i<4); return *(&x + i);	}

	inline bool operator==(const Vector4& v) const		{ return x==v.x && y==v.y && z==v.z && w ==v.w;	}
	inline bool operator!=(const Vector4& v) const		{ return !(*this==v);					}

	inline bool operator<(const Vector4& v) const { return GetLengthSquare() < v.GetLengthSquare(); }

	// Methods
	inline void		Set( _tvec4 X, _tvec4 Y, _tvec4 Z, _tvec4 W = 1 )	{ x = X; y = Y; z = Z;	w = W;	}
	
	inline float	GetLength(void) const				{ return (float)sqrt(x*x + y*y + z*z + w*w);	}
	inline float	GetLengthSquare() const				{ return (x*x + y*y + z*z + w*w); }
		
	inline Vector4 GetNormal(void) const
	{
		float flLen = GetLength();
		if (flLen == 0) return Vector4(0,0,0); // ????
		float fLenInverse = 1 / flLen;
		return Vector4(x * fLenInverse, y * fLenInverse, z * flLen, w*fLenInverse);
	}

	// Members
	_tvec4 x; 
	_tvec4 y; 
	_tvec4 z;
	_tvec4 w;

	static void		Normalize( Vector4& in_vToNormalize ) { in_vToNormalize = in_vToNormalize.GetNormal(); }
};

inline Vector4		operator*(float fl, const Vector4& v)	{ return v * fl; }

#endif	// __VECTOR4_H_
