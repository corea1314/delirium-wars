
#include "Quaternion.h"

#include <math.h>

const Quaternion Quaternion::identity;

Quaternion::Quaternion( const Vector3& Axis, const float Angle )
{
	s	= cos( Angle / 2.0f );
	v	= sin( Angle / 2.0f ) * Axis;
}


Quaternion::Quaternion( const Vector3& From, const Vector3& To )
{
	Vector3	Cross	= From ^ To;
	float	Dot		= (float)(From * To);
	float	Factor	= sqrtf( 2.0f * ( 1.0f + Dot ) );

	v	= Cross / Factor;
	s	= Factor / 2.0f;
}

Quaternion::Quaternion( const float qx, const float qy, const float qz, const float qw )
{
	v.Set(qx,qy,qz);
	s = qw;
}

Quaternion Quaternion::operator + ( const Quaternion& q ) const
{
	return	Quaternion( s + q.s, v + q.v );
}


Quaternion Quaternion::operator - ( const Quaternion& q ) const
{
	return	Quaternion( s - q.s, v - q.v );
}


Quaternion Quaternion::operator * ( const Quaternion& q ) const
{
	return Quaternion( (float)(s * q.s - v * q.v), s * q.v + q.s * v + ( v ^ q.v ) );
}


Quaternion Quaternion::operator * ( const float x ) const
{
	return Quaternion( s * x, v * x );
}


Quaternion Quaternion::operator / ( const float x ) const
{
	return Quaternion( s / x, v / x );
}


float	Quaternion::GetNorm() const
{
	return	sqrtf( s*s + v.x * v.x + v.y * v.y + v.z * v.z );
}


Quaternion&	Quaternion::Normalize()
{
	float	n	= GetNorm();
	
	s	/= n;
	v	/= n;

	return	*this;
}

Quaternion Quaternion::GetConjugate() const
{
	return	Quaternion( s, -v );
}

void Quaternion::Conjugate()
{
	v = -v;
}

Quaternion Quaternion::GetNegate() const
{
	Quaternion q;
	q.v = -v;
	q.s = -s;
	return q;
}

void Quaternion::Negate()
{
	v = -v;
	s = -s;
}

Quaternion Quaternion::GetInverse() const
{
	return GetConjugate() / GetNorm();
}

void Quaternion::Inverse()
{
	Quaternion q = GetConjugate() / GetNorm();
	v = q.v;
	s = q.s;
}


Vector3	Quaternion::Rotate( const Vector3& v ) const
{	 
	return	( (*this) * Quaternion( 0, v ) * this->GetConjugate() ).v;
}


float	Quaternion::GetRotationAngle() const
{
	if( fabsf( s ) < 1.0f )
		return	acos( s ) * 2.0f;
	else
		return	0.0f;
}


Vector3	Quaternion::GetRotationAxis() const
{
	float	Sin = sqrtf( 1.0f - s * s );

	if( fabsf( Sin ) < 0.0001f )
		Sin	= 1.0f;

	return	v / Sin;
}

Matrix3x3	Quaternion::ToMatrix() const
{
	Matrix3x3	m;
	float		c2x2	= 2.0f * v.x * v.x;
	float		c2y2	= 2.0f * v.y * v.y;
	float		c2z2	= 2.0f * v.z * v.z;
	float		c2xy	= 2.0f * v.x * v.y;
	float		c2xz	= 2.0f * v.x * v.z;
	float		c2yz	= 2.0f * v.y * v.z;
	float		c2sx	= 2.0f * s * v.x;
	float		c2sy	= 2.0f * s * v.y;
	float		c2sz	= 2.0f * s * v.z;

	m[0]	= 1.0f - c2y2 - c2z2;
	m[1]	= c2xy + c2sz;
	m[2]	= c2xz - c2sy;
	m[3]	= c2xy - c2sz;
	m[4]	= 1.0f - c2x2 - c2z2;
	m[5]	= c2yz + c2sx;
	m[6]	= c2xz + c2sy;
	m[7]	= c2yz - c2sx;
	m[8]	= 1.0f - c2x2 - c2y2;

	return	m;
}

double Quaternion::Cosom( const Quaternion& q0, const Quaternion& q1 )
{
	return q0.v.x * q1.v.x + q0.v.y * q1.v.y + q0.v.z * q1.v.z + q0.s * q1.s;
}

Quaternion Quaternion::Slerp( const Quaternion& qFrom, Quaternion const &qTo, double t )
{
	Quaternion qTemp;
	double        omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = Quaternion::Cosom( qFrom, qTo );

	// adjust signs (if necessary)
	if ( cosom < 0.0 )
	{
		cosom = -cosom; 
		qTemp.v.x = - qTo.v.x;
		qTemp.v.y = - qTo.v.y;
		qTemp.v.z = - qTo.v.z;
		qTemp.s = - qTo.s;
	} 
	else  
	{
		qTemp.v.x = qTo.v.x;
		qTemp.v.y = qTo.v.y;
		qTemp.v.z = qTo.v.z;
		qTemp.s = qTo.s;
	}

	// calculate coefficients
	if ( (1.0 - cosom) > QEPSILON ) 
	{
		// standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	}
	else 
	{
		// "from" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation
		scale0 = 1.0 - t;
		scale1 = t;
	}
	// calculate final values
	Quaternion qResult;
	qResult.v.x = (float)(scale0 * qFrom.v.x + scale1 * qTemp.v.x);
	qResult.v.y = (float)(scale0 * qFrom.v.y + scale1 * qTemp.v.y);
	qResult.v.z = (float)(scale0 * qFrom.v.z + scale1 * qTemp.v.z);
	qResult.s = (float)(scale0 * qFrom.s + scale1 * qTemp.s);

	return qResult;
}

