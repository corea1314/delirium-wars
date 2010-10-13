#ifndef __MATRIX4X4_H_
#define __MATRIX4X4_H_


#include "Vector3.h"
#include "Vector4.h"

#include "Quaternion.h"

class	Matrix3x3;


// NOTE: Matrix is indexed colum major

class Matrix4x4
{
public:
	Matrix4x4();
	
	Matrix4x4( const float *mat );
	Matrix4x4( const Matrix3x3 &mat );
	Matrix4x4( const Matrix4x4 &mat );

	Matrix4x4( const Vector3& pos, const Vector3& at, const Vector3& up );
	Matrix4x4( const Vector3& pos, const Quaternion& rot ); //, const Vector3& scale );

	Matrix4x4	operator + ( const Matrix4x4 &m ) const;
	Matrix4x4	operator - ( const Matrix4x4 &m ) const;
	Matrix4x4	operator * ( const Matrix4x4 &mat ) const;
	Matrix4x4&	operator += ( const Matrix4x4 &m );
	Matrix4x4&	operator -= ( const Matrix4x4 &m );
	Matrix4x4&	operator *= ( const Matrix4x4 &m );


	Matrix4x4	operator * ( const float &a ) const;
	Matrix4x4	operator / ( const float &a ) const;
	Matrix4x4&	operator *= ( const float &a );
	Matrix4x4&	operator /= ( const float &a );

	Vector3	operator * ( const Vector3 &v ) const;
	Vector4	operator * ( const Vector4 &v ) const;

	float&		operator [] ( const int index )			{ return m[index]; }
	float		operator [] ( const int index )	const	{ return m[index]; }

	operator	float*()				{ return m; }
	operator	const float*() const	{ return m;	}
	
	friend Matrix4x4	operator * ( const float &a, const Matrix4x4 &m );
	friend Vector3		operator * ( const Vector3& v, const Matrix4x4& m );
	
	Vector3	GetAt() const		{ return Vector3( m[0], m[1], m[2] );		}
	Vector3	GetUp() const		{ return Vector3( m[4], m[5], m[6] );		}
	Vector3	GetRight() const	{ return Vector3( m[8], m[9], m[10] );		}


	Vector3		GetPosition() const			{ return Vector3( m[12], m[13], m[14] );	}

	void		ScaleDiagonal( const float &a );
	Matrix4x4	Transpose() const;
	Matrix4x4	InverseTransform() const;

	float		GetDeterminant() const;

	Quaternion	GetQuaternion() const;

	void		Rotate( const Vector3& Axis, const float Angle );
	void		Translate( const Vector3& Translation );
	void		Scale( const Vector3& Scaling );

	void		SetTransform( const Vector3& pos, const Vector3& at, const Vector3& up );
	void		SetTransform( const Vector3& pos, const Quaternion& rot );

	inline Vector3	TransformVector( const Vector3 & v ) const
	{
		return Vector3(	m[0] * v.x + m[4] * v.y + m[ 8] * v.z, 
						m[1] * v.x + m[5] * v.y + m[ 9] * v.z, 
						m[2] * v.x + m[6] * v.y + m[10] * v.z );
	}

	inline Vector3	TransformPoint( const Vector3 & v ) const
	{
		return Vector3(	m[0] * v.x + m[4] * v.y + m[ 8] * v.z + m[12], 
						m[1] * v.x + m[5] * v.y + m[ 9] * v.z + m[13], 
						m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] );
	}



	static const Matrix4x4		I;
	static const Matrix4x4		ZERO;

	const float*		GetM() const { return m; }

	static void LerpTransform( Matrix4x4& out_matResult, 
									const Matrix4x4 & in_matA, const Matrix4x4 & in_matB, float in_fRatio );
	static void LerpTransformFast( Matrix4x4 & out_matResult, 
									const Matrix4x4 & in_matA, const Matrix4x4 & in_matB, float in_fRatio );

protected:
	float	m[16];
};


#endif//__MATRIX4X4_H_

