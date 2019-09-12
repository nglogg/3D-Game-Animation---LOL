
#ifndef MATRIX_H
#define MATRIX_H



class CMatrix3X3
{
	friend class CVector3;
	friend class CQuaternion;

public:



	void Zero();
	
	void Identity();
	
	void Transpose();

	void Invert();

	void SetRotation(float * fpRot);
	void SetRotation(float fX, float fY, float fZ);
	void SetInvRotation(float * fpInvRot);
	void SetInvRotation(float fX, float fY, float fZ);
	void FromAxisAngle(CVector3& vec, float fAngle);

	void FromQuaternion(const CQuaternion& rQuat);

	float Determinant();

	void InverseRotateVec(float * m_fVec);


	float& operator [] (const int iIdx);
	
	const CMatrix3X3 operator + (const CMatrix3X3& rMat) const;

	const CMatrix3X3 operator - (const CMatrix3X3& rMat) const;

	const void operator += (const CMatrix3X3& rMat);

	const void operator -= (const CMatrix3X3& rMat);


	const CMatrix3X3 operator * (const float fScalar) const;
	
	const CMatrix3X3 operator / (const float fScalar) const;
	
	const void operator *= (const float fScalar);

	const void operator /= (const float fScalar);

	
	const CMatrix3X3 operator * (const CMatrix3X3& rMat) const;

	const void operator *= (const CMatrix3X3& rMat);


	const void operator = (const CMatrix3X3& rMat);

	const bool operator == (const CMatrix3X3& rMat) const;

	const bool operator != (const CMatrix3X3& rMat) const;


	CMatrix3X3();
	CMatrix3X3(const CMatrix3X3& rMat);
	CMatrix3X3(float f11, float f12, float f13,
		float f21, float f22, float f23,
		float f31, float f32, float f33);
	CMatrix3X3(float * fpMat);


	float * Get();
	void Set(float f11, float f12, float f13,
		float f21, float f22, float f23,
		float f31, float f32, float f33);
	void Set(float * fpMat);

private:


	float m_fMat[9];
};


//-------------------------------------------------------------

//-------------------------------------------------------------
class CMatrix4X4
{
	friend class CVector3;
	friend class CQuaternion;

public:


	void Identity();

	void Zero();
	
	void SetRotation(float * fpRot);
	void SetRotation(float fX, float fY, float fZ);
	void SetInvRotation(float * fpInvRot);
	void SetInvRotation(float fX, float fY, float fZ);

	void SetTranslation(float * fpTrans);
	void SetTranslation(float fX, float fY, float fZ);
	void SetInvTranslation(float * fpTrans);
	void SetInvTranslation(float fX, float fY, float fZ);
	void FromAxisAngle(CVector3& vec, float fAngle);

	void FromQuaternion(CQuaternion& quat);

	float Determinant();
	
	void Transpose();
	
	void Invert();

	void InverseRotateVec(float * m_fVec);

	void InverseTranslateVec(float * m_fVec);


	float& operator[] (const int iIdx);

	const CMatrix4X4 operator+ (const CMatrix4X4& mat) const;

	const CMatrix4X4 operator- (const CMatrix4X4& mat) const;

	const CMatrix4X4 operator* (const float fScalar) const;

	const CMatrix4X4 operator/ (const float fScalar) const;


	const void operator += (const CMatrix4X4& mat);

	const void operator -= (const CMatrix4X4& mat);

	const void operator *= (const float fScalar);

	const void operator /= (const float fScalar);


	const CMatrix4X4 operator * (const CMatrix4X4& mat) const;

	const void operator *= (const CMatrix4X4& mat);


	const void operator= (const CMatrix4X4& mat);

	const bool operator== (const CMatrix4X4& mat) const;

	const bool operator!= (const CMatrix4X4& mat) const;


	CMatrix4X4();
	CMatrix4X4(const CMatrix4X4& mat);
	CMatrix4X4(float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44);
	CMatrix4X4(float * fpMat);


	float * Get();
	void Set(float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44);
	void Set(float * fpMat);

private:


	float m_fMat[16];
};

//Inline functions
#include "matrix.inl"

#endif