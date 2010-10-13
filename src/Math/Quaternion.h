#ifndef	__QUATERNION_H_
#define	__QUATERNION_H_

#include "Vector3.h"
#include "Matrix3x3.h"

#define QEPSILON 0.000001

class	Quaternion
{
private:
	Quaternion( const float _s, const Vector3& _v ) : v( _v ), s( _s )		{}
public:
	Quaternion() : v( 0.0f,1.0f,0.0f ), s( 0.0f )				{}
	Quaternion( const Vector3& Axis, const float Angle );
	Quaternion( const Vector3& From, const Vector3& To );
	Quaternion( const float qx, const float qy, const float qz, const float qw );
	
	Quaternion		operator + ( const Quaternion& q ) const;
	Quaternion		operator - ( const Quaternion& q ) const;
	Quaternion		operator * ( const Quaternion& q ) const;
	Quaternion		operator * ( const float x ) const;
	Quaternion		operator / ( const float x ) const;

	float			GetNorm() const;
	Quaternion&		Normalize();
	
	Quaternion		GetConjugate() const;
	void			Conjugate();

	Quaternion		GetNegate() const;
	void			Negate();

	Quaternion		GetInverse() const;
	void			Inverse();

	Vector3			Rotate( const Vector3& v ) const;
	Matrix3x3		ToMatrix() const;

	float			GetRotationAngle() const;
	Vector3			GetRotationAxis() const;

	static double Cosom( const Quaternion& q0, const Quaternion& q1 );
	static Quaternion Slerp( const Quaternion& qFrom, Quaternion const &qTo, double t );
	
public:
	Vector3	v;	// This is not the rotation axis v = xi + yj + zk
	float	s;	// This is not the rotation angle s = w

	static const Quaternion identity;
};

// TODO add operator float*quaternion

#endif//__QUATERNION_H_


