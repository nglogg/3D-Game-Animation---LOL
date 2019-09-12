
#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

class CVector3;
class CMatrix3X3;
class CMatrix4X4;


class CQuaternion
{
	friend CVector3;
	friend CMatrix3X3;
	friend CMatrix4X4;

public:


	float Magnitude() const;

	void Normalize();


	void Rotate(const CQuaternion& rQuat);

	float GetAngle() const;
	CVector3 GetAxis();

	void FromAxisAngle(CVector3& rAxis, float fAngle);


	void FromEulers(float * fpAngles);
	void FromEulers(float fX, float fY, float fZ);

	CVector3 GetEulers() const;

	void ToMatrix(CMatrix3X3& rDstMat) const;
	CMatrix4X4 ToMatrix4() const;

	void FromMatrix(CMatrix3X3& rMat);
	void FromMatrix(CMatrix4X4& rMat);


	float& operator[](const int iIdx);

	const CQuaternion operator+(const CQuaternion& rQuat) const;

	const CQuaternion operator-(const CQuaternion& rQuar) const;

	const CQuaternion operator*(const float fScalar) const;

	const CQuaternion operator/(const float fScalar) const;


	const void operator+=(const CQuaternion& rQuat);

	const void operator-=(const CQuaternion& rQuat);

	const void operator*=(const float fScalar);

	const void operator/=(const float fScalar);


	const CQuaternion operator*(const CQuaternion& rQuat) const;
	
	const void operator*=(const CQuaternion& rQuat);


	const CQuaternion operator*(const CVector3& rVec) const;
	
	const void operator*=(const CVector3& rVec);


	const CQuaternion operator-(void) const;

	const CQuaternion operator~(void) const;

	const void operator=(const CQuaternion& rQuat);

	const bool operator==(const CQuaternion& rQuat) const;

	const bool operator!=(const CQuaternion& rQuat) const;


	float * Get();
	CVector3 GetVector();
	float GetScalar();
	void Set(float * fpQuat);
	void Set(float fX, float fY, float fZ, float fW);


	CQuaternion();
	CQuaternion(float * fpQuat);
	CQuaternion(float fX, float fY, float fZ, float fW);
	CQuaternion(const CQuaternion& rQuat);

private:


	float m_fQuat[4];
};


CQuaternion LERP(CQuaternion& rQuat0, CQuaternion& rQuat1, float fInterp);
//-------------------------------------------------------------

//-------------------------------------------------------------
inline CQuaternion SLERP(CQuaternion& rQuat0, CQuaternion& rQuat1, float fInterp)
{
	float * q0 = rQuat0.Get();
	float * q1 = rQuat1.Get();


	float fDot = q0[0] * q1[0] + q0[1] * q1[1] + q0[2] * q1[2] + q0[3] * q1[3];

	if(fDot < 0.0f)
	{
		rQuat1 = -rQuat1;
		fDot = -fDot;
	}


	if(fDot < 1.00001f && fDot > 0.99999f)
	{
		return LERP(rQuat0, rQuat1, fInterp);
	}


	float fTheta = acosf(fDot);

	return (rQuat0 * sinf(fTheta * (1 - fInterp)) + rQuat1 * sinf(fTheta * fInterp))/sinf(fTheta);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline CQuaternion LERP(CQuaternion& rQuat0, CQuaternion& rQuat1, float fInterp)
{
	CQuaternion ret(((rQuat1 - rQuat0) * fInterp) + rQuat0);
	ret.Normalize();
	return ret;
}

//Inlined functions
#include "quaternion.inl"

#endif //QUATERNION_H