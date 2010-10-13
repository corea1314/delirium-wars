

#include "Matrix3x3.h"

#include "Matrix4x4.h"
#include "Vector3.h"

#include <memory.h>


const Matrix3x3 Matrix3x3::I	= Matrix3x3();
const Matrix3x3 Matrix3x3::ZERO	= Matrix3x3() - Matrix3x3();


Matrix3x3::Matrix3x3()
{
	m[0] = m[4] = m[8] = 1;
	m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0;
}


Matrix3x3::Matrix3x3( const float *mat )
{
	memcpy( m, mat, 9 * sizeof( float ) );
}


Matrix3x3::Matrix3x3( const Matrix4x4 &mat )
{
	m[0]	= mat[0];
	m[1]	= mat[1];
	m[2]	= mat[2];
	m[3]	= mat[4];
	m[4]	= mat[5];
	m[5]	= mat[6];
	m[6]	= mat[8];
	m[7]	= mat[9];
	m[8]	= mat[10];
}


Vector3	Matrix3x3::operator * ( const Vector3 &v ) const
{
	Vector3 v2;

	v2.x = m[0] * v.x + m[3] * v.y + m[6] * v.z;
	v2.y = m[1] * v.x + m[4] * v.y + m[7] * v.z;
	v2.z = m[2] * v.x + m[5] * v.y + m[8] * v.z;

	return v2;
}


Matrix3x3 Matrix3x3::operator * ( const Matrix3x3 &mat ) const
{
	Matrix3x3 ret;

	ret.m[0]	= m[0] * mat.m[0]	+ m[3] * mat.m[1] + m[6] * mat.m[2];
	ret.m[1]	= m[1] * mat.m[0]	+ m[4] * mat.m[1] + m[7] * mat.m[2];
	ret.m[2]	= m[2] * mat.m[0]	+ m[5] * mat.m[1] + m[8] * mat.m[2];
	ret.m[3]	= m[0] * mat.m[3]	+ m[3] * mat.m[4] + m[6] * mat.m[5];
	ret.m[4]	= m[1] * mat.m[3]	+ m[4] * mat.m[4] + m[7] * mat.m[5];
	ret.m[5]	= m[2] * mat.m[3]	+ m[5] * mat.m[4] + m[8] * mat.m[5];
	ret.m[6]	= m[0] * mat.m[6]	+ m[3] * mat.m[7] + m[6] * mat.m[8];
	ret.m[7]	= m[1] * mat.m[6]	+ m[4] * mat.m[7] + m[7] * mat.m[8];
	ret.m[8]	= m[2] * mat.m[6]	+ m[5] * mat.m[7] + m[8] * mat.m[8];

	return ret;
}


Matrix3x3& Matrix3x3::operator *= ( const Matrix3x3 &mat )
{
	return ( *this = *this * mat );
}


Matrix3x3	Matrix3x3::operator * ( const float &a ) const
{
	Matrix3x3 ret( *this );

	ret.m[0]	*= a;
	ret.m[1]	*= a;
	ret.m[2]	*= a;
	ret.m[3]	*= a;
	ret.m[4]	*= a;
	ret.m[5]	*= a;
	ret.m[6]	*= a;
	ret.m[7]	*= a;
	ret.m[8]	*= a;

	return ret;
}


Matrix3x3&	Matrix3x3::operator *= ( const float &a )
{
	return ( *this = *this * a );
}


Matrix3x3 operator * ( const float &a, const Matrix3x3 &m )
{
	Matrix3x3	ret( m );

	ret.m[0]	*= a;
	ret.m[1]	*= a;
	ret.m[2]	*= a;
	ret.m[3]	*= a;
	ret.m[4]	*= a;
	ret.m[5]	*= a;
	ret.m[6]	*= a;
	ret.m[7]	*= a;
	ret.m[8]	*= a;

	return ret;
}


Matrix3x3	Matrix3x3::operator / ( const float &a ) const
{
	Matrix3x3 ret( *this );

	ret.m[0]	/= a;
	ret.m[1]	/= a;
	ret.m[2]	/= a;
	ret.m[3]	/= a;
	ret.m[4]	/= a;
	ret.m[5]	/= a;
	ret.m[6]	/= a;
	ret.m[7]	/= a;
	ret.m[8]	/= a;

	return ret;
}


Matrix3x3&	Matrix3x3::operator /= ( const float &a )
{
	return ( *this = *this * a );
}


Matrix3x3	Matrix3x3::operator + ( const Matrix3x3 &m ) const
{
	Matrix3x3 ret(*this);

	ret.m[0] += m.m[0];
	ret.m[1] += m.m[1];
	ret.m[2] += m.m[2];
	ret.m[3] += m.m[3];
	ret.m[4] += m.m[4];
	ret.m[5] += m.m[5];
	ret.m[6] += m.m[6];
	ret.m[7] += m.m[7];
	ret.m[8] += m.m[8];

	return ret;
}


Matrix3x3&	Matrix3x3::operator += ( const Matrix3x3 &m )
{
	return ( *this = *this + m );
}


Matrix3x3	Matrix3x3::operator - ( const Matrix3x3 &m ) const
{
	Matrix3x3 ret(*this);

	ret.m[0] -= m.m[0];
	ret.m[1] -= m.m[1];
	ret.m[2] -= m.m[2];
	ret.m[3] -= m.m[3];
	ret.m[4] -= m.m[4];
	ret.m[5] -= m.m[5];
	ret.m[6] -= m.m[6];
	ret.m[7] -= m.m[7];
	ret.m[8] -= m.m[8];

	return ret;
}


Matrix3x3&	Matrix3x3::operator -= ( const Matrix3x3 &m )
{
	return ( *this = *this - m );
}


Matrix3x3 operator / ( const float &a, const Matrix3x3 &m )
{
	Matrix3x3	ret( m );

	ret.m[0]	/= a;
	ret.m[1]	/= a;
	ret.m[2]	/= a;
	ret.m[3]	/= a;
	ret.m[4]	/= a;
	ret.m[5]	/= a;
	ret.m[6]	/= a;
	ret.m[7]	/= a;
	ret.m[8]	/= a;

	return ret;
}


void	Matrix3x3::ScaleDiagonal( const float &a )
{
	m[0]	*= a;
	m[4]	*= a;
	m[8]	*= a;
}


void	Matrix3x3::NormalizeColumns()
{
	float	c1 = sqrtf( m[0] * m[0] + m[1] * m[1] + m[2] * m[2] );
	float	c2 = sqrtf( m[3] * m[3] + m[4] * m[4] + m[5] * m[5] );
	float	c3 = sqrtf( m[6] * m[6] + m[7] * m[7] + m[8] * m[8] );

	m[0] /= c1;
	m[1] /= c1;
	m[2] /= c1;
	m[3] /= c2;
	m[4] /= c2;
	m[5] /= c2;
	m[6] /= c3;
	m[7] /= c3;
	m[8] /= c3;
}


Matrix3x3	Matrix3x3::Transpose() const
{
	Matrix3x3	ret;

	ret.m[0]	= m[0];
	ret.m[1]	= m[3];
	ret.m[2]	= m[6];
	ret.m[3]	= m[1];
	ret.m[4]	= m[4];
	ret.m[5]	= m[7];
	ret.m[6]	= m[2];
	ret.m[7]	= m[5];
	ret.m[8]	= m[8];

	return ret;
}


Matrix3x3	Matrix3x3::Inverse() const
{
	Matrix3x3	ret;

	ret.m[0] = m[4] * m[8] - m[7] * m[5];
	ret.m[1] = m[2] * m[7] - m[1] * m[8];
	ret.m[2] = m[1] * m[5] - m[2] * m[4];
	ret.m[3] = m[5] * m[6] - m[3] * m[8];
	ret.m[4] = m[0] * m[8] - m[2] * m[6];
	ret.m[5] = m[2] * m[3] - m[0] * m[5];
	ret.m[6] = m[3] * m[7] - m[4] * m[6];
	ret.m[7] = m[1] * m[6] - m[0] * m[7];
	ret.m[8] = m[0] * m[4] - m[3] * m[1];

	return ret / Determinant();
}


float		Matrix3x3::Determinant() const
{
	return	m[0] * ( m[4] * m[8] - m[5] * m[7] ) -
			m[3] * ( m[1] * m[8] - m[2] * m[7] ) +
			m[6] * ( m[1] * m[5] - m[2] * m[4] );
}


Vector3	Matrix3x3::GetXDirection() const
{
	return Vector3( m[0], m[1], m[2] );
}


Vector3	Matrix3x3::GetYDirection() const
{
	return Vector3( m[3], m[4], m[5] );
}


Vector3	Matrix3x3::GetZDirection() const
{
	return Vector3( m[6], m[7], m[8] );
}
