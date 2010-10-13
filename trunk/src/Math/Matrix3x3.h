#ifndef __MATRIX3X3_H_
#define __MATRIX3X3_H_

class Vector2;
class Vector3;
class Matrix4x4;

// NOTE: Matrix is indexed colum major

class Matrix3x3
{
public:
	/// Default constructor.
	Matrix3x3();
	
	Matrix3x3( const float *mat );
	Matrix3x3( const Matrix4x4 &mat );

	Matrix3x3	operator + ( const Matrix3x3 &m ) const;
	Matrix3x3	operator - ( const Matrix3x3 &m ) const;
	Matrix3x3	operator * ( const Matrix3x3 &m ) const;
		
	Matrix3x3&	operator += ( const Matrix3x3 &m );
	Matrix3x3&	operator -= ( const Matrix3x3 &m );
	Matrix3x3&	operator *= ( const Matrix3x3 &m );

	Matrix3x3	operator * ( const float &a ) const;
	Matrix3x3	operator / ( const float &a ) const;
	
	Matrix3x3&	operator *= ( const float &a );
	Matrix3x3&	operator /= ( const float &a );

	Vector3	operator * ( const Vector3 &v ) const;

	operator	float*()				{ return m; }
	operator	const float*() const	{ return m;	}

	float&		operator [] ( const int index )			{ return m[index]; }
	float		operator [] ( const int index ) const	{ return m[index]; }

	friend Matrix3x3	operator * ( const float &a, const Matrix3x3 &m );
	friend Matrix3x3	operator / ( const float &a, const Matrix3x3 &m );


	Vector3	GetXDirection() const;
	Vector3	GetYDirection() const;
	Vector3	GetZDirection() const;

	
	Matrix3x3	Transpose() const;
	Matrix3x3	Inverse() const;

	float		Determinant() const;
	void		ScaleDiagonal( const float &a );
	void		NormalizeColumns();

	static const	Matrix3x3	I;
	static const	Matrix3x3	ZERO;

protected:

	float	m[9];
};

#endif//__MATRIX3X3_H_

