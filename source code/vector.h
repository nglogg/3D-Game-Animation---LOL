
#ifndef VECTOR_H
#define VECTOR_H

class CQuaternion;
class CMatrix3X3;
class CMatrix4X4;


class CVector2
{
public:

	
	float Magnitude() const;
	
	void Normalize();
	
	void Negate();

	float Dot(const CVector2& vec) const;

	
	float& operator[](const int iIdx);


	const CVector2 operator+ (const CVector2& vec) const;

	const CVector2 operator- (const CVector2& vec) const;

	const void operator += (const CVector2& vec);

	const void operator -= (const CVector2& vec);

	const CVector2 operator-();


	const CVector2 operator *  (const float fScalar) const;

	const CVector2 operator /  (const float fScalar) const;

	const void operator *= (const float fScalar);

	const void operator /= (const float fScalar);


	const void operator =  (const CVector2& vec);
	
	const bool	  operator == (const CVector2& vec)  const;
	
	const bool	  operator != (const CVector2& vec)  const;


	CVector2();
	CVector2(const CVector2& vecCopy);
	CVector2(float * fpVec);
	CVector2(float fX, float fY);


	float * Get();
	void Set(float fX, float fY);
	void Set(float * fpVec);

private:


	float m_fVec[2];

};


class CVector3
{
	friend CQuaternion;

public:

	float Magnitude() const;

	void Normalize();
	
	void Negate();


	float Dot(const CVector3& vec) const;

	CVector3 Cross(const CVector3& vec) const;

	void Transform3(const CMatrix3X3& mat);

	void Transform3(const CMatrix4X4& mat);

	void Transform4(const CMatrix4X4& mat);

	void TransformQ(const CQuaternion& quat);


	float& operator[](const int iIdx);

	const CVector3 operator+ (const CVector3& vec) const;

	const CVector3 operator- (const CVector3& vec) const;

	const void operator += (const CVector3& vec);

	const void operator -= (const CVector3& vec);

	const CVector3 operator-();


	const CVector3 operator *  (const float fScalar) const;

	const CVector3 operator /  (const float fScalar) const;
	
	const void operator *= (const float fScalar);

	const void operator /= (const float fScalar);


	const void operator =  (const CVector3& vec);

	const bool	  operator == (const CVector3& vec)  const;

	const bool	  operator != (const CVector3& vec)  const;


	const CVector3 operator * (const CQuaternion& quat) const;

	CVector3();
	CVector3(const CVector3& vecCopy);
	CVector3(float * fpVec);
	CVector3(float fX, float fY, float fZ);


	float * Get();
	void Set(float fX, float fY, float fZ);
	void Set(float * fpVec);

private:

	float m_fVec[3];

};


inline float DotProduct(CVector2& vec0, CVector2& vec1)
{
	return (
		(vec0[0] * vec1[0]) +
		(vec0[1] * vec1[1]));
}

inline float DotProduct(CVector3& vec0, CVector3& vec1)
{
	return (
		(vec0[0] * vec1[0])+
		(vec0[1] * vec1[1])+
		(vec0[2] * vec1[2]));
}


inline CVector3 CrossProduct(CVector3& vec0, CVector3& vec1)
{
	return ( CVector3(
		(vec0[1] * vec1[2] - 
		vec0[2] * vec1[1]),
		(vec0[2] * vec1[0] - 
		vec0[0] * vec1[2]),
		(vec0[0] * vec1[1] - 
		vec0[1] * vec1[0])));
}


#include "vector.inl"

#endif //VECTOR_H