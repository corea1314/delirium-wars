#include "Matrix4x4.h"

#include "Matrix3x3.h"
#include <math.h>
#include <memory.h>

const Matrix4x4 Matrix4x4::I	= Matrix4x4();
const Matrix4x4 Matrix4x4::ZERO	= Matrix4x4() - Matrix4x4();


Matrix4x4::Matrix4x4()
{
	m[0] = m[5] = m[10] = m[15] = 1;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11]= m[12] = m[13] = m[14] = 0;
}


Matrix4x4::Matrix4x4( const float *mat )
{
	memcpy( m, mat, 16 * sizeof( float ) );
}

Matrix4x4::Matrix4x4( const Matrix4x4 &mat )
{
	m[0]	= mat[0];
	m[1]	= mat[1];
	m[2]	= mat[2];
	m[3]	= mat[3];
	m[4]	= mat[4];
	m[5]	= mat[5];
	m[6]	= mat[6];
	m[7]	= mat[7];
	m[8]	= mat[8];
	m[9]	= mat[9];
	m[10]	= mat[10];
	m[11]	= mat[11];
	m[12]	= mat[12];
	m[13]	= mat[13];
	m[14]	= mat[14];
	m[15]	= mat[15];
}


Matrix4x4::Matrix4x4( const Matrix3x3 &mat )
{
	m[0]	= mat[0];
	m[1]	= mat[1];
	m[2]	= mat[2];
	m[3]	= 0;
	m[4]	= mat[3];
	m[5]	= mat[4];
	m[6]	= mat[5];
	m[7]	= 0;
	m[8]	= mat[6];
	m[9]	= mat[7];
	m[10]	= mat[8];
	m[11]	= 0;
	m[12]	= 0;
	m[13]	= 0;
	m[14]	= 0;
	m[15]	= 1;
}

Matrix4x4::Matrix4x4( const Vector3& pos, const Vector3& at, const Vector3& up )
{
	SetTransform( pos, at, up );
}

void Matrix4x4::SetTransform( const Vector3& pos, const Vector3& at, const Vector3& up )
{
	// Set matrix to identity
	m[0] = m[5] = m[10] = m[15] = 1;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11]= m[12] = m[13] = m[14] = 0;

//	Translate( pos );

	m[12] = pos.x;
	m[13] = pos.y;
	m[14] = pos.z;
	m[15] = 1;

	Vector3 rt = Vector3::CrossProduct(  at, up );

	float r[16];

	r[ 0] = at.x;
	r[ 1] = at.y;
	r[ 2] = at.z;
	
	r[ 4] = up.x;
	r[ 5] = up.y;
	r[ 6] = up.z;
	
	r[ 8] = rt.x;
	r[ 9] = rt.y;
	r[10] = rt.z;
	
	r[ 3] = r[ 7] = r[11] = r[12] = r[13] = r[14] = 0;
	r[15] = 1;

//  |   1    0    0    0   |   |   Rx    Ry    Rz    0   |
//  |   0    1    0    0   | * |   Ux    Uy    Uz    0   |
//  |   0    0    1    0   |   |   Vx    Vy    Vz    0   |
//  |   tx   ty   tz   1   |   |   0     0     0     1   |

	Matrix4x4 _R(r);
	(*this) *= _R;

	/*
// Here's what the final view matrix should look like:
//
//  |  rx   ry   rz  -(r.e) |
//  |  ux   uy   uz  -(u.e) |
//  | -lx  -ly  -lz   (l.e) |
//  |   0    0    0     1   |
//
// Where r = Right vector
//       u = Up vector
//       l = Look vector
//       e = Eye position in world space
//       . = Dot-product operation

	Vector3 right = Vector3::CrossProduct( at, up );

	// Set matrix to identity
	m[0] = m[5] = m[10] = m[15] = 1;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11]= m[12] = m[13] = m[14] = 0;
	
	m[0] =  right.x;
	m[1] =  up.x;
	m[2] = -at.x;
	m[3] =  0.0f;

	m[4] =  right.y;
	m[5] =  up.y;
	m[6] = -at.y;
	m[7] =  0.0f;

	m[8]  =  right.z;
	m[9]  =  up.z;
	m[10] = -at.z;
	m[11] =  0.0f;

	m[12] = (float)-Vector3::DotProduct(right, pos);
	m[13] = (float)-Vector3::DotProduct(up, pos);
	m[14] = (float) Vector3::DotProduct(at, pos);
	m[15] =  1.0f;
	*/
}

Vector3	Matrix4x4::operator * ( const Vector3 &v ) const
{
	Vector3 v2;

	v2.x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12];
	v2.y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13];
	v2.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];

	return v2;
}


Vector4	Matrix4x4::operator * ( const Vector4 &v ) const
{
	Vector4 v2;

	v2.x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
	v2.y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
	v2.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
	v2.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;

	return v2;
}

Vector3		operator * ( const Vector3& v, const Matrix4x4& m )
{
	Vector3	v2;

	v2.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12];
	v2.x = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13];
	v2.x = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];

	return v2;
}


Matrix4x4 Matrix4x4::operator * ( const Matrix4x4 &mat ) const
{
	Matrix4x4 ret;

	ret.m[0]	= m[0] * mat.m[0] + m[4] * mat.m[1] + m[8] * mat.m[2] + m[12] * mat.m[3];
	ret.m[4]	= m[0] * mat.m[4] + m[4] * mat.m[5] + m[8] * mat.m[6] + m[12] * mat.m[7];
	ret.m[8]	= m[0] * mat.m[8] + m[4] * mat.m[9] + m[8] * mat.m[10] + m[12] * mat.m[11];
	ret.m[12]	= m[0] * mat.m[12] + m[4] * mat.m[13] + m[8] * mat.m[14] + m[12] * mat.m[15];

	ret.m[1]	= m[1] * mat.m[0] + m[5] * mat.m[1] + m[9] * mat.m[2] + m[13] * mat.m[3];
	ret.m[5]	= m[1] * mat.m[4] + m[5] * mat.m[5] + m[9] * mat.m[6] + m[13] * mat.m[7];
	ret.m[9]	= m[1] * mat.m[8] + m[5] * mat.m[9] + m[9] * mat.m[10] + m[13] * mat.m[11];
	ret.m[13]	= m[1] * mat.m[12] + m[5] * mat.m[13] + m[9] * mat.m[14] + m[13] * mat.m[15];

	ret.m[2]	= m[2] * mat.m[0] + m[6] * mat.m[1] + m[10] * mat.m[2] + m[14] * mat.m[3];
	ret.m[6]	= m[2] * mat.m[4] + m[6] * mat.m[5] + m[10] * mat.m[6] + m[14] * mat.m[7];
	ret.m[10]	= m[2] * mat.m[8] + m[6] * mat.m[9] + m[10] * mat.m[10] + m[14] * mat.m[11];
	ret.m[14]	= m[2] * mat.m[12] + m[6] * mat.m[13] + m[10] * mat.m[14] + m[14] * mat.m[15];

	ret.m[3]	= m[3] * mat.m[0] + m[7] * mat.m[1] + m[11] * mat.m[2] + m[15] * mat.m[3];
	ret.m[7]	= m[3] * mat.m[4] + m[7] * mat.m[5] + m[11] * mat.m[6] + m[15] * mat.m[7];
	ret.m[11]	= m[3] * mat.m[8] + m[7] * mat.m[9] + m[11] * mat.m[10] + m[15] * mat.m[11];
	ret.m[15]	= m[3] * mat.m[12] + m[7] * mat.m[13] + m[11] * mat.m[14] + m[15] * mat.m[15];

	return ret;
}


Matrix4x4	Matrix4x4::operator * ( const float &a ) const
{
	Matrix4x4 ret( *this );

	ret.m[0]	*= a;
	ret.m[1]	*= a;
	ret.m[2]	*= a;
	ret.m[3]	*= a;
	ret.m[4]	*= a;
	ret.m[5]	*= a;
	ret.m[6]	*= a;
	ret.m[7]	*= a;
	ret.m[8]	*= a;
	ret.m[9]	*= a;
	ret.m[10]	*= a;
	ret.m[11]	*= a;
	ret.m[12]	*= a;
	ret.m[13]	*= a;
	ret.m[14]	*= a;
	ret.m[15]	*= a;

	return ret;
}


Matrix4x4 operator * ( const float &a, const Matrix4x4 &m )
{
	Matrix4x4	ret( m );

	ret.m[0]	*= a;
	ret.m[1]	*= a;
	ret.m[2]	*= a;
	ret.m[3]	*= a;
	ret.m[4]	*= a;
	ret.m[5]	*= a;
	ret.m[6]	*= a;
	ret.m[7]	*= a;
	ret.m[8]	*= a;
	ret.m[9]	*= a;
	ret.m[10]	*= a;
	ret.m[11]	*= a;
	ret.m[12]	*= a;
	ret.m[13]	*= a;
	ret.m[14]	*= a;
	ret.m[15]	*= a;

	return ret;
}


Matrix4x4	Matrix4x4::operator / ( const float &a ) const
{
	Matrix4x4 ret( *this );

	ret.m[0]	/= a;
	ret.m[1]	/= a;
	ret.m[2]	/= a;
	ret.m[3]	/= a;
	ret.m[4]	/= a;
	ret.m[5]	/= a;
	ret.m[6]	/= a;
	ret.m[7]	/= a;
	ret.m[8]	/= a;
	ret.m[9]	/= a;
	ret.m[10]	/= a;
	ret.m[11]	/= a;
	ret.m[12]	/= a;
	ret.m[13]	/= a;
	ret.m[14]	/= a;
	ret.m[15]	/= a;

	return ret;
}


Matrix4x4	Matrix4x4::operator + ( const Matrix4x4 &m ) const
{
	Matrix4x4 ret(*this);

	ret.m[0] += m.m[0];
	ret.m[1] += m.m[1];
	ret.m[2] += m.m[2];
	ret.m[3] += m.m[3];
	ret.m[4] += m.m[4];
	ret.m[5] += m.m[5];
	ret.m[6] += m.m[6];
	ret.m[7] += m.m[7];
	ret.m[8] += m.m[8];
	ret.m[9] += m.m[9];
	ret.m[10] += m.m[10];
	ret.m[11] += m.m[11];
	ret.m[12] += m.m[12];
	ret.m[13] += m.m[13];
	ret.m[14] += m.m[14];
	ret.m[15] += m.m[15];

	return ret;
}


Matrix4x4&	Matrix4x4::operator += ( const Matrix4x4 &m )
{
	return ( *this = *this + m );
}


Matrix4x4	Matrix4x4::operator - ( const Matrix4x4 &m ) const
{
	Matrix4x4 ret(*this);

	ret.m[0] -= m.m[0];
	ret.m[1] -= m.m[1];
	ret.m[2] -= m.m[2];
	ret.m[3] -= m.m[3];
	ret.m[4] -= m.m[4];
	ret.m[5] -= m.m[5];
	ret.m[6] -= m.m[6];
	ret.m[7] -= m.m[7];
	ret.m[8] -= m.m[8];
	ret.m[9] -= m.m[9];
	ret.m[10] -= m.m[10];
	ret.m[11] -= m.m[11];
	ret.m[12] -= m.m[12];
	ret.m[13] -= m.m[13];
	ret.m[14] -= m.m[14];
	ret.m[15] -= m.m[15];

	return ret;
}


Matrix4x4&	Matrix4x4::operator -= ( const Matrix4x4 &m )
{
	return ( *this = *this - m );
}


Matrix4x4&	Matrix4x4::operator *= ( const Matrix4x4 &m )
{
	return ( *this = *this * m );
}


Matrix4x4&	Matrix4x4::operator *= ( const float &a )
{
	return ( *this = *this * a );
}


Matrix4x4&	Matrix4x4::operator /= ( const float &a )
{
	return ( *this = *this * a );
}


void	Matrix4x4::ScaleDiagonal( const float &a )
{
	m[0]	*= a;
	m[5]	*= a;
	m[10]	*= a;
	m[15]	*= a;
}


Matrix4x4 Matrix4x4::Transpose() const
{
	Matrix4x4	ret;

	ret.m[0]	= m[0];
	ret.m[1]	= m[4];
	ret.m[2]	= m[8];
	ret.m[3]	= m[12];
	ret.m[4]	= m[1];
	ret.m[5]	= m[5];
	ret.m[6]	= m[9];
	ret.m[7]	= m[13];
	ret.m[8]	= m[2];
	ret.m[9]	= m[6];
	ret.m[10]	= m[10];
	ret.m[11]	= m[14];
	ret.m[12]	= m[3];
	ret.m[13]	= m[7];
	ret.m[14]	= m[11];
	ret.m[15]	= m[15];

	return ret;
}

float Matrix4x4::GetDeterminant() const
{
#define ACCESS_M(i,j) m[(i << 2) + j]
return  ACCESS_M(0, 0) * (
         ACCESS_M(1, 1) * (ACCESS_M(2, 2) * ACCESS_M(3, 3) - ACCESS_M(2, 3) * ACCESS_M(3, 2)) -
         ACCESS_M(2, 1) * (ACCESS_M(1, 2) * ACCESS_M(3, 3) - ACCESS_M(1, 3) * ACCESS_M(3, 2)) +
         ACCESS_M(3, 1) * (ACCESS_M(1, 2) * ACCESS_M(2, 3) - ACCESS_M(1, 3) * ACCESS_M(2, 2))
         ) -
        ACCESS_M(1, 0) * (
         ACCESS_M(0, 1) * (ACCESS_M(2, 2) * ACCESS_M(3, 3) - ACCESS_M(2, 3) * ACCESS_M(3, 2)) -
         ACCESS_M(2, 1) * (ACCESS_M(0, 2) * ACCESS_M(3, 3) - ACCESS_M(0, 3) * ACCESS_M(3, 2)) +
         ACCESS_M(3, 1) * (ACCESS_M(0, 2) * ACCESS_M(2, 3) - ACCESS_M(0, 3) * ACCESS_M(2, 2))
         ) +
        ACCESS_M(2, 0) * (
         ACCESS_M(0, 1) * (ACCESS_M(1, 2) * ACCESS_M(3, 3) - ACCESS_M(1, 3) * ACCESS_M(3, 2)) -
         ACCESS_M(1, 1) * (ACCESS_M(0, 2) * ACCESS_M(3, 3) - ACCESS_M(0, 3) * ACCESS_M(3, 2)) +
         ACCESS_M(3, 1) * (ACCESS_M(0, 2) * ACCESS_M(1, 3) - ACCESS_M(0, 3) * ACCESS_M(1, 2))
         ) -
        ACCESS_M(3, 0) * (
         ACCESS_M(0, 1) * (ACCESS_M(1, 2) * ACCESS_M(2, 3) - ACCESS_M(1, 3) * ACCESS_M(2, 2)) -
         ACCESS_M(1, 1) * (ACCESS_M(0, 2) * ACCESS_M(2, 3) - ACCESS_M(0, 3) * ACCESS_M(2, 2)) +
         ACCESS_M(2, 1) * (ACCESS_M(0, 2) * ACCESS_M(1, 3) - ACCESS_M(0, 3) * ACCESS_M(1, 2))
         );
#undef ACCESS_M
}


Matrix4x4	Matrix4x4::InverseTransform() const
{
#define ACCESS_M(i,j) m[(i << 2) + j]

    Matrix4x4	Result;

    float Det = GetDeterminant();
    if(Det == 0.0f)
         return Matrix4x4::I;
    float RDet = 1.0f / Det;
    Result.ACCESS_M(0, 0) = RDet * (
                 ACCESS_M(1, 1) * (ACCESS_M(2, 2) * ACCESS_M(3, 3) - ACCESS_M(2, 3) * ACCESS_M(3, 2)) -
                 ACCESS_M(2, 1) * (ACCESS_M(1, 2) * ACCESS_M(3, 3) - ACCESS_M(1, 3) * ACCESS_M(3, 2)) +
                 ACCESS_M(3, 1) * (ACCESS_M(1, 2) * ACCESS_M(2, 3) - ACCESS_M(1, 3) * ACCESS_M(2, 2))
                 );
    Result.ACCESS_M(0, 1) = -RDet * (
                 ACCESS_M(0, 1) * (ACCESS_M(2, 2) * ACCESS_M(3, 3) - ACCESS_M(2, 3) * ACCESS_M(3, 2)) -
                 ACCESS_M(2, 1) * (ACCESS_M(0, 2) * ACCESS_M(3, 3) - ACCESS_M(0, 3) * ACCESS_M(3, 2)) +
                 ACCESS_M(3, 1) * (ACCESS_M(0, 2) * ACCESS_M(2, 3) - ACCESS_M(0, 3) * ACCESS_M(2, 2))
                 );
    Result.ACCESS_M(0, 2) = RDet * (
                 ACCESS_M(0, 1) * (ACCESS_M(1, 2) * ACCESS_M(3, 3) - ACCESS_M(1, 3) * ACCESS_M(3, 2)) -
                 ACCESS_M(1, 1) * (ACCESS_M(0, 2) * ACCESS_M(3, 3) - ACCESS_M(0, 3) * ACCESS_M(3, 2)) +
                 ACCESS_M(3, 1) * (ACCESS_M(0, 2) * ACCESS_M(1, 3) - ACCESS_M(0, 3) * ACCESS_M(1, 2))
                 );
    Result.ACCESS_M(0, 3) = -RDet * (
                 ACCESS_M(0, 1) * (ACCESS_M(1, 2) * ACCESS_M(2, 3) - ACCESS_M(1, 3) * ACCESS_M(2, 2)) -
                 ACCESS_M(1, 1) * (ACCESS_M(0, 2) * ACCESS_M(2, 3) - ACCESS_M(0, 3) * ACCESS_M(2, 2)) +
                 ACCESS_M(2, 1) * (ACCESS_M(0, 2) * ACCESS_M(1, 3) - ACCESS_M(0, 3) * ACCESS_M(1, 2))
                 );



    Result.ACCESS_M(1, 0) = -RDet * (
                 ACCESS_M(1, 0) * (ACCESS_M(2, 2) * ACCESS_M(3, 3) - ACCESS_M(2, 3) * ACCESS_M(3, 2)) -
                 ACCESS_M(2, 0) * (ACCESS_M(1, 2) * ACCESS_M(3, 3) - ACCESS_M(1, 3) * ACCESS_M(3, 2)) +
                 ACCESS_M(3, 0) * (ACCESS_M(1, 2) * ACCESS_M(2, 3) - ACCESS_M(1, 3) * ACCESS_M(2, 2))
                 );

    Result.ACCESS_M(1, 1) = RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(2, 2) * ACCESS_M(3, 3) - ACCESS_M(2, 3) * ACCESS_M(3, 2)) -
                 ACCESS_M(2, 0) * (ACCESS_M(0, 2) * ACCESS_M(3, 3) - ACCESS_M(0, 3) * ACCESS_M(3, 2)) +
                 ACCESS_M(3, 0) * (ACCESS_M(0, 2) * ACCESS_M(2, 3) - ACCESS_M(0, 3) * ACCESS_M(2, 2))
                 );

    Result.ACCESS_M(1, 2) = -RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(1, 2) * ACCESS_M(3, 3) - ACCESS_M(1, 3) * ACCESS_M(3, 2)) -
                 ACCESS_M(1, 0) * (ACCESS_M(0, 2) * ACCESS_M(3, 3) - ACCESS_M(0, 3) * ACCESS_M(3, 2)) +
                 ACCESS_M(3, 0) * (ACCESS_M(0, 2) * ACCESS_M(1, 3) - ACCESS_M(0, 3) * ACCESS_M(1, 2))
                 );

    Result.ACCESS_M(1, 3) = RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(1, 2) * ACCESS_M(2, 3) - ACCESS_M(1, 3) * ACCESS_M(2, 2)) -
                 ACCESS_M(1, 0) * (ACCESS_M(0, 2) * ACCESS_M(2, 3) - ACCESS_M(0, 3) * ACCESS_M(2, 2)) +
                 ACCESS_M(2, 0) * (ACCESS_M(0, 2) * ACCESS_M(1, 3) - ACCESS_M(0, 3) * ACCESS_M(1, 2))
                 ); 

    Result.ACCESS_M(2, 0) = RDet * (
                 ACCESS_M(1, 0) * (ACCESS_M(2, 1) * ACCESS_M(3, 3) - ACCESS_M(2, 3) * ACCESS_M(3, 1)) -
                 ACCESS_M(2, 0) * (ACCESS_M(1, 1) * ACCESS_M(3, 3) - ACCESS_M(1, 3) * ACCESS_M(3, 1)) +
                 ACCESS_M(3, 0) * (ACCESS_M(1, 1) * ACCESS_M(2, 3) - ACCESS_M(1, 3) * ACCESS_M(2, 1))
                 );

    Result.ACCESS_M(2, 1) = -RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(2, 1) * ACCESS_M(3, 3) - ACCESS_M(2, 3) * ACCESS_M(3, 1)) -
                 ACCESS_M(2, 0) * (ACCESS_M(0, 1) * ACCESS_M(3, 3) - ACCESS_M(0, 3) * ACCESS_M(3, 1)) +
                 ACCESS_M(3, 0) * (ACCESS_M(0, 1) * ACCESS_M(2, 3) - ACCESS_M(0, 3) * ACCESS_M(2, 1))
                 );

    Result.ACCESS_M(2, 2) = RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(1, 1) * ACCESS_M(3, 3) - ACCESS_M(1, 3) * ACCESS_M(3, 1)) -
                 ACCESS_M(1, 0) * (ACCESS_M(0, 1) * ACCESS_M(3, 3) - ACCESS_M(0, 3) * ACCESS_M(3, 1)) +
                 ACCESS_M(3, 0) * (ACCESS_M(0, 1) * ACCESS_M(1, 3) - ACCESS_M(0, 3) * ACCESS_M(1, 1))
                 );

    Result.ACCESS_M(2, 3) = -RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(1, 1) * ACCESS_M(2, 3) - ACCESS_M(1, 3) * ACCESS_M(2, 1)) -
                 ACCESS_M(1, 0) * (ACCESS_M(0, 1) * ACCESS_M(2, 3) - ACCESS_M(0, 3) * ACCESS_M(2, 1)) +
                 ACCESS_M(2, 0) * (ACCESS_M(0, 1) * ACCESS_M(1, 3) - ACCESS_M(0, 3) * ACCESS_M(1, 1))
                 );


    Result.ACCESS_M(3, 0) = -RDet * (
                 ACCESS_M(1, 0) * (ACCESS_M(2, 1) * ACCESS_M(3, 2) - ACCESS_M(2, 2) * ACCESS_M(3, 1)) -
                 ACCESS_M(2, 0) * (ACCESS_M(1, 1) * ACCESS_M(3, 2) - ACCESS_M(1, 2) * ACCESS_M(3, 1)) +
                 ACCESS_M(3, 0) * (ACCESS_M(1, 1) * ACCESS_M(2, 2) - ACCESS_M(1, 2) * ACCESS_M(2, 1))
                 );

    Result.ACCESS_M(3, 1) = RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(2, 1) * ACCESS_M(3, 2) - ACCESS_M(2, 2) * ACCESS_M(3, 1)) -
                 ACCESS_M(2, 0) * (ACCESS_M(0, 1) * ACCESS_M(3, 2) - ACCESS_M(0, 2) * ACCESS_M(3, 1)) +
                 ACCESS_M(3, 0) * (ACCESS_M(0, 1) * ACCESS_M(2, 2) - ACCESS_M(0, 2) * ACCESS_M(2, 1))
                 );

    Result.ACCESS_M(3, 2) = -RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(1, 1) * ACCESS_M(3, 2) - ACCESS_M(1, 2) * ACCESS_M(3, 1)) -
                 ACCESS_M(1, 0) * (ACCESS_M(0, 1) * ACCESS_M(3, 2) - ACCESS_M(0, 2) * ACCESS_M(3, 1)) +
                 ACCESS_M(3, 0) * (ACCESS_M(0, 1) * ACCESS_M(1, 2) - ACCESS_M(0, 2) * ACCESS_M(1, 1))
                 );

    Result.ACCESS_M(3, 3) = RDet * (
                 ACCESS_M(0, 0) * (ACCESS_M(1, 1) * ACCESS_M(2, 2) - ACCESS_M(1, 2) * ACCESS_M(2, 1)) -
                 ACCESS_M(1, 0) * (ACCESS_M(0, 1) * ACCESS_M(2, 2) - ACCESS_M(0, 2) * ACCESS_M(2, 1)) +
                 ACCESS_M(2, 0) * (ACCESS_M(0, 1) * ACCESS_M(1, 2) - ACCESS_M(0, 2) * ACCESS_M(1, 1))
                 );

#undef ACCESS_M
	return Result;
	
/*
	Matrix4x4	ret;
	
	ret.m[0]	= m[0];
	ret.m[1]	= m[4];
	ret.m[2]	= m[8];
	ret.m[3]	= m[3];
	ret.m[4]	= m[1];
	ret.m[5]	= m[5];
	ret.m[6]	= m[9];
	ret.m[7]	= m[7];
	ret.m[8]	= m[2];
	ret.m[9]	= m[6];
	ret.m[10]	= m[10];
	ret.m[11]	= m[11];
	ret.m[12]	= -m[12];
	ret.m[13]	= -m[13];
	ret.m[14]	= -m[14];
	ret.m[15]	= m[15];

	return ret;
*/
}

/*
00 01 02 03
04 05 06 07
08 09 10 11
12 13 14 15

 00  04  08  03
 01  05  09  07
 02  06  10  11
-12 -13 -14  15
*/



void Matrix4x4::Rotate( const Vector3& Axis, const float Angle )
{
	// Code inspired from Mesa3D
	float s, c;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
	Matrix4x4 mat;

	s = sin( Angle );
	c = cos( Angle );

	xx = Axis.x * Axis.x;
	yy = Axis.y * Axis.y;
	zz = Axis.z * Axis.z;
	xy = Axis.x * Axis.y;
	yz = Axis.y * Axis.z;
	zx = Axis.z * Axis.x;
	xs = Axis.x * s;
	ys = Axis.y * s;
	zs = Axis.z * s;
	one_c = 1.0F - c;

	mat[0] = (one_c * xx) + c;
	mat[4] = (one_c * xy) - zs;
	mat[8] = (one_c * zx) + ys;
	mat[12] = 0.0F;

	mat[1] = (one_c * xy) + zs;
	mat[5] = (one_c * yy) + c;
	mat[9] = (one_c * yz) - xs;
	mat[13] = 0.0F;

	mat[2] = (one_c * zx) - ys;
	mat[6] = (one_c * yz) + xs;
	mat[10] = (one_c * zz) + c;
	mat[14] = 0.0F;

	mat[3] = 0.0F;
	mat[7] = 0.0F;
	mat[11] = 0.0F;
	mat[15] = 1.0F;

	*this = (*this) * mat;
}


void Matrix4x4::Translate( const Vector3& Translation )
{
	m[12]	= m[0] * Translation.x + m[4] * Translation.y + m[8] * Translation.z + m[12];
	m[13]	= m[1] * Translation.x + m[5] * Translation.y + m[9] * Translation.z + m[13];
	m[14]	= m[2] * Translation.x + m[6] * Translation.y + m[10] * Translation.z + m[14];
	m[15]	= m[3] * Translation.x + m[7] * Translation.y + m[11] * Translation.z + m[15];
}


void Matrix4x4::Scale( const Vector3& Scaling )
{
	m[0]	*= Scaling.x;
	m[4]	*= Scaling.y;
	m[8]	*= Scaling.z;

	m[1]	*= Scaling.x;
	m[5]	*= Scaling.y;
	m[9]	*= Scaling.z;

	m[2]	*= Scaling.x;
	m[6]	*= Scaling.y;
	m[10]	*= Scaling.z;

	m[3]	*= Scaling.x;
	m[7]	*= Scaling.y;
	m[11]	*= Scaling.z;
}

/*
// Test Suite
{
	Vector3 vV(4,5,6);
	
	Matrix4x4 matTT;
	matTT.Translate( Vector3(1,1,1) );
	Vector3 vTT = matTT * vV;
	
	Matrix4x4 matTS;
	matTS.Scale( Vector3(2,1,1) );
	Vector3 vTS = matTS * vV;
	
	Matrix4x4 matTR;
	matTR.Rotate( Vector3(0,1,0), PI/2 );
	Vector3 vTR = matTR * vV;
}
*/

void Matrix4x4::LerpTransform( Matrix4x4 & out_matResult, const Matrix4x4 & A, const Matrix4x4 & B, float t ) 
{
	Vector3 newPosition = LERP( A.GetPosition(), B.GetPosition(), t );
	Quaternion newRotation = Quaternion::Slerp( A.GetQuaternion(), B.GetQuaternion(), t );
	out_matResult.SetTransform( newPosition, newRotation ); // newScale ); 

//	Vector3 newScale = LERP(	Vector3(A.GetXDirection().length(), A.GetYDirection().length(), A.GetZDirection().length() ), 
//								Vector3(B.GetXDirection().length(), B.GetYDirection().length(), B.GetZDirection().length()), t);
}

void Matrix4x4::LerpTransformFast( Matrix4x4 & out_matResult, const Matrix4x4 & A, const Matrix4x4 & B, float t ) 
{
	Vector3 newAt = LERP( A.GetAt(), B.GetAt(), t ).GetNormal();
	Vector3 newUp = LERP( A.GetUp(), B.GetUp(), t ).GetNormal();
    Vector3 newPosition = LERP(A.GetPosition(), B.GetPosition(), t);
	
    out_matResult.SetTransform( newPosition, newAt, newUp ); // newScale ); 
}
 
Quaternion Matrix4x4::GetQuaternion() const 
{ 
	//This function is not for the type of matrix this engine uses, so I copy and transpose the matrix...
	Matrix4x4 newMatrix = Transpose();
	
	float a[16];
	memcpy( a, newMatrix.GetM(), 16 * sizeof( float ) );
	
	//The rest of the function... (as on: http://vamos.sourceforge.net/matrixfaq.htm#Q45)
	float qw;
	float qx;
	float qy;
	float qz;
	
	float trace = a[0] + a[5] + a[10] + 1.0f;  
	
	if ( trace > 0.000001f ) //if( trace > M_EPSILON )
	{
		float s = sqrtf(trace) * 2.0f;
		qx = ( a[9] - a[6] ) / s;
		qy = ( a[2] - a[8] ) / s;
		qz = ( a[4] - a[1] ) / s;
		qw = 0.25f * s;
	} 
	else 
	{
		if ( a[0] > a[5] && a[0] > a[10] )  // column 0
		{
			float s = sqrtf( 1.0f + a[0] - a[5] - a[10]) * 2.0f;
			qx = 0.25f * s;
			qy = (a[1] + a[4] ) / s;
			qz = (a[2] + a[8] ) / s;
			qw = (a[9] - a[6] ) / s;    
		} 
		else if (a[5] > a[10])  // column 1
		{
			float s = 2.0f * sqrtf( 1.0f + a[5] - a[0] - a[10]);
			qx = (a[1] + a[4] ) / s;
			qy = 0.25f * s;
			qz = (a[6] + a[9] ) / s;
			qw = (a[2] - a[8] ) / s;    
		} 
		else // column 2
		{
			float s = 2.0f * sqrtf( 1.0f + a[10] - a[0] - a[5] );
			qx = (a[2] + a[8] ) / s;
			qy = (a[6] + a[9] ) / s;
			qz = 0.25f * s;
			qw = (a[4] - a[1] ) / s;
		}
	}

	Quaternion resultQuat(qx,qy,qz,qw); 
	resultQuat = Quaternion( resultQuat.GetRotationAxis(), resultQuat.GetRotationAngle() );
	return resultQuat; 
}
 
Matrix4x4::Matrix4x4( const Vector3& pos, const Quaternion& rot )
{
	SetTransform( pos, rot );
}

void Matrix4x4::SetTransform( const Vector3& pos, const Quaternion& rot )
{
	//Rotate the at, up and rt... (mine)
	Vector3 tempAt(1,0,0);
	Vector3 tempUp(0,1,0);
	Vector3 tempRt(0,0,1);

	tempAt = rot.Rotate(tempAt);
	tempUp = rot.Rotate(tempUp);
	tempRt = rot.Rotate(tempRt);

//	seems unnecessary
//	Vector3::Normalize(tempAt);
//	Vector3::Normalize(tempUp);
//	Vector3::Normalize(tempRt);

	//Set the at, up and rt
	m[0] = tempAt.x;
	m[1] = tempAt.y;
	m[2] = tempAt.z;

	m[4] = tempUp.x;
	m[5] = tempUp.y;
	m[6] = tempUp.z;

	m[8] = tempRt.x;
	m[9] = tempRt.y;
	m[10] = tempRt.z;

	//Set the position
	m[12] = pos.x;
	m[13] = pos.y;
	m[14] = pos.z;

	//Set the scale
//    Scale(scale);
}
