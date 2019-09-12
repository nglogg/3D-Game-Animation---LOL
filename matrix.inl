

#ifndef MATRIX_INL
#define MATRIX_INL

#include <memory.h>
#include "quaternion.h"

#define SQU(x) x * x

//-------------------------------------------------------------
//                       CMatrix3X3                           -
//-------------------------------------------------------------
//-------------------------------------------------------------

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix3X3::Zero()
{
	memset(m_fMat, 0, sizeof(float[9]));
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix3X3::Identity()
{
	Zero();
	m_fMat[0] = 1.0f;
	m_fMat[4] = 1.0f;
	m_fMat[8] = 1.0f;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix3X3::Transpose()
{
	CMatrix3X3 tmp(m_fMat[0], m_fMat[3], m_fMat[6],
					m_fMat[1], m_fMat[4], m_fMat[7],
					m_fMat[2], m_fMat[5], m_fMat[8]);
	*this = tmp;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix3X3::Invert()
{
	float fDet = Determinant();
	float fNewMat[9];
	
	if(fDet == 0)
		fDet = 1;
	

	float fInvDet = 1/fDet;
	
	fNewMat[0] =  (m_fMat[4] * m_fMat[8] - m_fMat[5] * m_fMat[8]) * fInvDet;
	fNewMat[1] = -(m_fMat[1] * m_fMat[8] - m_fMat[2] * m_fMat[8]) * fInvDet;
	fNewMat[2] =  (m_fMat[1] * m_fMat[5] - m_fMat[2] * m_fMat[4]) * fInvDet;

	fNewMat[3] = -(m_fMat[3] * m_fMat[8] - m_fMat[5] * m_fMat[6]) * fInvDet;
	fNewMat[4] =  (m_fMat[0] * m_fMat[8] - m_fMat[2] * m_fMat[6]) * fInvDet;
	fNewMat[5] = -(m_fMat[0] * m_fMat[5] - m_fMat[2] * m_fMat[3]) * fInvDet;

	fNewMat[6] =  (m_fMat[3] * m_fMat[7] - m_fMat[4] * m_fMat[6]) * fInvDet;
	fNewMat[7] = -(m_fMat[0] * m_fMat[7] - m_fMat[1] * m_fMat[6]) * fInvDet;
	fNewMat[8] =  (m_fMat[0] * m_fMat[4] - m_fMat[1] * m_fMat[3]) * fInvDet;
	
	memcpy(m_fMat, fNewMat, sizeof(float[16]));
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix3X3::SetRotation(float fX, float fY, float fZ)
{
	double cx = cos(fX);
	double sx = sin(fX);
	double cy = cos(fY);
	double sy = sin(fY);
	double cz = cos(fZ);
	double sz = sin(fZ);

	m_fMat[0] = (float)(cy * cz);
	m_fMat[1] = (float)(cy * sz);
	m_fMat[2] = (float)(-sy);

	m_fMat[3] = (float)(sx * sy * cz - cx * sz);
	m_fMat[4] = (float)(sx * sy * sz + cx * cz);
	m_fMat[5] = (float)(sx * cy);

	m_fMat[6] = (float)(cx * sy * cz + sx * sz);
	m_fMat[7] = (float)(cx * sy * sz - sx * cz);
	m_fMat[8] = (float)(cx * cy);
}

inline void CMatrix3X3::SetRotation(float * fpAngles)
{
	SetRotation(fpAngles[0], fpAngles[1], fpAngles[2]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix3X3::SetInvRotation(float fX, float fY, float fZ)
{
	double cx = cos(fX);
	double sx = sin(fX);
	double cy = cos(fY);
	double sy = sin(fY);
	double cz = cos(fZ);
	double sz = sin(fZ);

	m_fMat[0] = (float)(cy * cz);
	m_fMat[3] = (float)(cy * sz);
	m_fMat[6] = (float)(-sy );

	m_fMat[1] = (float)(sx * sy * cz - cx * sz);
	m_fMat[4] = (float)(sx * sy * sz + cx * cz);
	m_fMat[7] = (float)(sx * cy);

	m_fMat[2] = (float)(cx * sy * cz + sx * sz);
	m_fMat[5] = (float)(cx * sy * sz - sx * cz);
	m_fMat[8] = (float)(cx * cz);
}

inline void CMatrix3X3::SetInvRotation(float * fpAngles)
{
	SetInvRotation(fpAngles[0], fpAngles[1], fpAngles[2]);
}

inline void CMatrix3X3::FromAxisAngle(CVector3& vec, float fAngle)
{
	float fCos = cosf(fAngle);
	float fSin = sinf(fAngle);
	float fOMC = 1.0f - fCos;

	m_fMat[0] = fCos + SQU(vec[0]) * fOMC;
	m_fMat[4] = fCos + SQU(vec[1]) * fOMC;
	m_fMat[8] = fCos + SQU(vec[2]) * fOMC;

	m_fMat[1] = vec[0] * vec[1] * fOMC + vec[2] * fSin;
	m_fMat[3] = vec[0] * vec[1] * fOMC - vec[2] * fSin;
	m_fMat[2] = vec[0] * vec[2] * fOMC + vec[1] * fSin;
	m_fMat[6] = vec[0] * vec[2] * fOMC - vec[1] * fSin;
	m_fMat[5] = vec[0] * vec[2] * fOMC + vec[0] * fSin;
	m_fMat[7] = vec[0] * vec[2] * fOMC - vec[0] * fSin;

}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix3X3::FromQuaternion(const CQuaternion& rQuat)
{
	m_fMat[0] = SQU(rQuat.m_fQuat[3]) + SQU(rQuat.m_fQuat[0]) - SQU(rQuat.m_fQuat[1]) - SQU(rQuat.m_fQuat[2]);
	m_fMat[1] = 2 * (rQuat.m_fQuat[0] * rQuat.m_fQuat[1] - rQuat.m_fQuat[2] * rQuat.m_fQuat[3]);
	m_fMat[2] = 2 * (rQuat.m_fQuat[0] * rQuat.m_fQuat[2] + rQuat.m_fQuat[1] * rQuat.m_fQuat[3]);
	m_fMat[3] = 2 * (rQuat.m_fQuat[0] * rQuat.m_fQuat[1] + rQuat.m_fQuat[2] * rQuat.m_fQuat[3]);
	m_fMat[4] = SQU(rQuat.m_fQuat[3]) - SQU(rQuat.m_fQuat[0]) + SQU(rQuat.m_fQuat[1]) - SQU(rQuat.m_fQuat[2]);
	m_fMat[5] = 2 * (rQuat.m_fQuat[1] * rQuat.m_fQuat[2] - rQuat.m_fQuat[0] * rQuat.m_fQuat[3]);
	m_fMat[6] = 2 * (rQuat.m_fQuat[2] * rQuat.m_fQuat[0] - rQuat.m_fQuat[1] * rQuat.m_fQuat[3]);
	m_fMat[7] = 2 * (rQuat.m_fQuat[1] * rQuat.m_fQuat[2] + rQuat.m_fQuat[0] * rQuat.m_fQuat[3]);
	m_fMat[8] = SQU(rQuat.m_fQuat[3]) - SQU(rQuat.m_fQuat[0]) - SQU(rQuat.m_fQuat[1]) + SQU(rQuat.m_fQuat[2]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline float CMatrix3X3::Determinant()
{
	return (m_fMat[0] * (m_fMat[4] * m_fMat[8] - m_fMat[7] * m_fMat[5])) -
		   (m_fMat[1] * (m_fMat[3] * m_fMat[8] - m_fMat[6] * m_fMat[5])) + 
		   (m_fMat[2] * (m_fMat[3] * m_fMat[7] - m_fMat[6] * m_fMat[4]));
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix3X3::InverseRotateVec(float * fpVec)
{
	float tmp[3];

	tmp[0] = fpVec[0] * m_fMat[0] + fpVec[1] * m_fMat[1] + fpVec[2] * m_fMat[2];
	tmp[1] = fpVec[0] * m_fMat[3] + fpVec[1] * m_fMat[4] + fpVec[2] * m_fMat[5];
	tmp[2] = fpVec[0] * m_fMat[6] + fpVec[1] * m_fMat[7] + fpVec[2] * m_fMat[8];

	memcpy(fpVec, tmp, sizeof(float[3]));
}

//-------------------------------------------------------------
       
//-------------------------------------------------------------
//-------------------------------------------------------------

//-------------------------------------------------------------
inline float& CMatrix3X3::operator [](const int iIdx)
{
	return m_fMat[iIdx];
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix3X3 CMatrix3X3::operator +(const CMatrix3X3& rMat) const
{
	return CMatrix3X3(m_fMat[0] + rMat.m_fMat[0], m_fMat[1] + rMat.m_fMat[1], m_fMat[2] + rMat.m_fMat[2], 
					m_fMat[3] + rMat.m_fMat[3], m_fMat[4] + rMat.m_fMat[4], m_fMat[5] + rMat.m_fMat[5], 
					m_fMat[6] + rMat.m_fMat[6], m_fMat[7] + rMat.m_fMat[7], m_fMat[8] + rMat.m_fMat[8]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix3X3 CMatrix3X3::operator -(const CMatrix3X3& rMat) const
{
	return CMatrix3X3(m_fMat[0] - rMat.m_fMat[0], m_fMat[1] - rMat.m_fMat[1], m_fMat[2] - rMat.m_fMat[2], 
					m_fMat[3] - rMat.m_fMat[3], m_fMat[4] - rMat.m_fMat[4], m_fMat[5] - rMat.m_fMat[5], 
					m_fMat[6] - rMat.m_fMat[6], m_fMat[7] - rMat.m_fMat[7], m_fMat[8] - rMat.m_fMat[8]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix3X3::operator +=(const CMatrix3X3& rMat)
{
	m_fMat[0] += rMat.m_fMat[0];
	m_fMat[1] += rMat.m_fMat[1];
	m_fMat[2] += rMat.m_fMat[2];
	m_fMat[3] += rMat.m_fMat[3];
	m_fMat[4] += rMat.m_fMat[4];
	m_fMat[5] += rMat.m_fMat[5];
	m_fMat[6] += rMat.m_fMat[6];
	m_fMat[7] += rMat.m_fMat[7];
	m_fMat[8] += rMat.m_fMat[8];
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix3X3::operator -=(const CMatrix3X3& rMat)
{
	m_fMat[0] -= rMat.m_fMat[0];
	m_fMat[1] -= rMat.m_fMat[1];
	m_fMat[2] -= rMat.m_fMat[2];
	m_fMat[3] -= rMat.m_fMat[3];
	m_fMat[4] -= rMat.m_fMat[4];
	m_fMat[5] -= rMat.m_fMat[5];
	m_fMat[6] -= rMat.m_fMat[6];
	m_fMat[7] -= rMat.m_fMat[7];
	m_fMat[8] -= rMat.m_fMat[8];
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix3X3 CMatrix3X3::operator *(const float fScalar) const
{
	return CMatrix3X3(m_fMat[0] * fScalar, m_fMat[1] * fScalar, m_fMat[2] * fScalar, 
					m_fMat[3] * fScalar, m_fMat[4] * fScalar, m_fMat[5] * fScalar, 
					m_fMat[6] * fScalar, m_fMat[7] * fScalar, m_fMat[8] * fScalar);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix3X3 CMatrix3X3::operator /(const float fScalar) const
{
	float fInvScl = 1/fScalar;
	return CMatrix3X3(m_fMat[0] * fInvScl, m_fMat[1] * fInvScl, m_fMat[2] * fInvScl, 
					m_fMat[3] * fInvScl, m_fMat[4] * fInvScl, m_fMat[5] * fInvScl, 
					m_fMat[6] * fInvScl, m_fMat[7] * fInvScl, m_fMat[8] * fInvScl);
}

//-------------------------------------------------------------

inline const void CMatrix3X3::operator *=(const float fScalar)
{
	m_fMat[0] *= fScalar;
	m_fMat[1] *= fScalar;
	m_fMat[2] *= fScalar;
	m_fMat[3] *= fScalar;
	m_fMat[4] *= fScalar;
	m_fMat[5] *= fScalar;
	m_fMat[6] *= fScalar;
	m_fMat[7] *= fScalar;
	m_fMat[8] *= fScalar;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix3X3::operator /=(const float fScalar)
{
	float fInvScl = 1/fScalar;
	m_fMat[0] *= fInvScl;
	m_fMat[1] *= fInvScl;
	m_fMat[2] *= fInvScl;
	m_fMat[3] *= fInvScl;
	m_fMat[4] *= fInvScl;
	m_fMat[5] *= fInvScl;
	m_fMat[6] *= fInvScl;
	m_fMat[7] *= fInvScl;
	m_fMat[8] *= fInvScl;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix3X3 CMatrix3X3::operator *(const CMatrix3X3& rMat) const
{
	const float * m1 = m_fMat;
	const float * m2 = rMat.m_fMat;

	return CMatrix3X3(

		(m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2]),
		(m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2]),
		(m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2]),
		
		(m1[3] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5]),
		(m1[4] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5]),
		(m1[5] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5]),

		(m1[6] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8]),
		(m1[7] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8]),
		(m1[8] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8])
	
		);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix3X3::operator *=(const CMatrix3X3& rMat)
{
	float fNewMat[9];

	const float * m1 = m_fMat;
	const float * m2 = rMat.m_fMat;

	fNewMat[0] = (m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2]);
	fNewMat[1] = (m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2]);
	fNewMat[2] = (m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2]);
		
	fNewMat[3] = (m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5]);
	fNewMat[4] = (m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5]);
	fNewMat[5] = (m1[2] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5]);

	fNewMat[6] = (m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8]);
	fNewMat[7] = (m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8]);
	fNewMat[8] = (m1[2] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8]);

	memcpy(fNewMat, m_fMat, sizeof(float[16]));
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix3X3::operator =(const CMatrix3X3& rMat)
{
	memcpy(m_fMat, rMat.m_fMat, sizeof(float[9]));
}


//-------------------------------------------------------------

//-------------------------------------------------------------
inline const bool CMatrix3X3::operator ==(const CMatrix3X3& rMat) const
{
	return (m_fMat[0] == rMat.m_fMat[0] && m_fMat[1] == rMat.m_fMat[1] && m_fMat[2] == rMat.m_fMat[2] && 
		m_fMat[3] == rMat.m_fMat[3] && m_fMat[4] == rMat.m_fMat[4] && m_fMat[5] == rMat.m_fMat[5] && 
		m_fMat[6] == rMat.m_fMat[6] && m_fMat[7] == rMat.m_fMat[7] && m_fMat[8] == rMat.m_fMat[8]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const bool CMatrix3X3::operator != (const CMatrix3X3& rMat) const
{
	return !(*this == rMat);
}
//-------------------------------------------------------------

//-------------------------------------------------------------
inline CMatrix3X3::CMatrix3X3()
{
	memset(m_fMat, 0, sizeof(float[9]));
}

inline CMatrix3X3::CMatrix3X3(const CMatrix3X3& rMat)
{
	*this = rMat;
}

inline CMatrix3X3::CMatrix3X3(float f11, float f12, float f13,
				float f21, float f22, float f23,
				float f31, float f32, float f33)
{
	m_fMat[0] = f11;
	m_fMat[1] = f12;
	m_fMat[2] = f13;
	m_fMat[3] = f21;
	m_fMat[4] = f22;
	m_fMat[5] = f23;
	m_fMat[6] = f31;
	m_fMat[7] = f32;
	m_fMat[8] = f33;
}

inline CMatrix3X3::CMatrix3X3(float * fpMat)
{
	memcpy(m_fMat, fpMat, sizeof(float[9]));
}
//-------------------------------------------------------------

//-------------------------------------------------------------
inline float * CMatrix3X3::Get()
{
	return m_fMat;
}

inline void CMatrix3X3::Set(float f11, float f12, float f13,
				float f21, float f22, float f23,
				float f31, float f32, float f33)
{
	m_fMat[0] = f11;
	m_fMat[1] = f12;
	m_fMat[2] = f13;
	m_fMat[3] = f21;
	m_fMat[4] = f22;
	m_fMat[5] = f23;
	m_fMat[6] = f31;
	m_fMat[7] = f32;
	m_fMat[8] = f33;
}

inline void CMatrix3X3::Set(float * fpMat)
{
	memcpy(m_fMat, fpMat, sizeof(float[9]));
}


//-------------------------------------------------------------
//                    END CMatrix3X3                          -
//-------------------------------------------------------------



//-------------------------------------------------------------
//                       CMatrix4X4                           -
//-------------------------------------------------------------
//-------------------------------------------------------------
               
//-------------------------------------------------------------
//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix4X4::Zero()
{
	memset(m_fMat, 0, sizeof(float[16]));
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix4X4::Identity()
{
	Zero();
	m_fMat[0] = 1.0f;
	m_fMat[5] = 1.0f;
	m_fMat[10] = 1.0f;
	m_fMat[15] = 1.0f;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix4X4::Transpose()
{
	CMatrix4X4 tmp(m_fMat[0], m_fMat[4], m_fMat[8], m_fMat[12],
					m_fMat[1], m_fMat[5], m_fMat[9], m_fMat[13],
					m_fMat[2], m_fMat[6], m_fMat[10], m_fMat[14],
					m_fMat[3], m_fMat[7], m_fMat[11], m_fMat[15]);
	*this = tmp;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix4X4::Invert()
{
	CMatrix4X4 matA(*this);
	CMatrix4X4 matB;
	matB.Identity();

	unsigned int uiRow, uiColumn;
	unsigned int uiRowMax; 
	float tmp;


	for(uiColumn = 0; uiColumn < 4; uiColumn++)
	{

		uiRowMax = uiColumn;
		for(uiRow = uiColumn+1; uiRow < 4; uiRow++)
		{
			if(fabs(matA[4 * uiRow + uiColumn]) > fabs(matA[4 * uiRowMax + uiColumn]))
			{
				uiRowMax = uiRow;
			}
	}

 
	if(matA[4 * uiColumn + uiRowMax] == 0.0F)
	{
		Identity();
		return;
	}


	for(int cc = 0; cc < 4; cc++)
	{
		tmp = matA[4 * uiColumn + cc];
		matA[4 * uiColumn + cc] = matA[4 * uiRowMax + cc];
		matA[4 * uiRowMax + cc] = tmp;
    
		tmp = matB[4 * uiColumn + cc];
		matB[4 * uiColumn + cc] = matB[4 * uiRowMax + cc];
		matB[4 * uiRowMax + cc] = tmp;
	}

 
	tmp = matA[4 * uiColumn + uiColumn];
	for(int cc = 0; cc < 4; cc++)
	{
		matA[4 * uiColumn + cc] /= tmp;
		matB[4 * uiColumn + cc] /= tmp;
	}


	for(uiRow = 0; uiRow < 4; uiRow++)
	{
		if (uiRow != uiColumn)
		{
			tmp = matA[4 * uiRow + uiColumn];
			for(int cc = 0; cc < 4; cc++)
			{
				matA[4 * uiRow + cc] -= matA[4 * uiColumn + cc] * tmp;
				matB[4 * uiRow + cc] -= matB[4 * uiColumn + cc] * tmp;
			}
		}
	}
}

	*this = matB;

}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix4X4::SetRotation(float fX, float fY, float fZ)
{
	double cx = cos(fX);
	double sx = sin(fX);
	double cy = cos(fY);
	double sy = sin(fY);
	double cz = cos(fZ);
	double sz = sin(fZ);

	m_fMat[0] = (float)(cy * cz);
	m_fMat[1] = (float)(cy * sz);
	m_fMat[2] = (float)(-sy);

	m_fMat[4] = (float)(sx * sy * cz - cx * sz);
	m_fMat[5] = (float)(sx * sy * sz + cx * cz);
	m_fMat[6] = (float)(sx * cy);

	m_fMat[8] = (float)(cx * sy * cz + sx * sz);
	m_fMat[9] = (float)(cx * sy * sz - sx * cz);
	m_fMat[10] = (float)(cx * cy);

	m_fMat[15] = 1.0f;
}

inline void CMatrix4X4::SetRotation(float * fpAngles)
{
	SetRotation(fpAngles[0], fpAngles[1], fpAngles[2]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CMatrix4X4::SetInvRotation(float fX, float fY, float fZ)
{
	double cx = cos(fX);
	double sx = sin(fX);
	double cy = cos(fY);
	double sy = sin(fY);
	double cz = cos(fZ);
	double sz = sin(fZ);

	m_fMat[0] = (float)(cy * cz);
	m_fMat[4] = (float)(cy * sz);
	m_fMat[8] = (float)(-sy );

	m_fMat[1] = (float)(sx * sy * cz - cx * sz);
	m_fMat[5] = (float)(sx * sy * sz + cx * cz);
	m_fMat[9] = (float)(sx * cy);

	m_fMat[2] = (float)(cx * sy * cz + sx * sz);
	m_fMat[6] = (float)(cx * sy * sz - sx * cz);
	m_fMat[10] = (float)(cx * cz);
}

inline void CMatrix4X4::SetInvRotation(float * fpAngles)
{
	SetInvRotation(fpAngles[0], fpAngles[1], fpAngles[2]);
}

inline void CMatrix4X4::FromAxisAngle(CVector3& vec, float fAngle)
{
	float fCos = cosf(fAngle);
	float fSin = sinf(fAngle);
	float fOMC = 1.0f - fCos;

	m_fMat[0] = fCos + SQU(vec[0]) * fOMC;
	m_fMat[5] = fCos + SQU(vec[1]) * fOMC;
	m_fMat[10] = fCos + SQU(vec[2]) * fOMC;
	m_fMat[15] = 1.0f;
	m_fMat[4] = vec[0] * vec[1] * fOMC + vec[2] * fSin;
	m_fMat[1] = vec[0] * vec[1] * fOMC - vec[2] * fSin;
	m_fMat[8] = vec[0] * vec[2] * fOMC + vec[1] * fSin;
	m_fMat[2] = vec[0] * vec[2] * fOMC - vec[1] * fSin;
	m_fMat[9] = vec[1] * vec[2] * fOMC + vec[0] * fSin;
	m_fMat[6] = vec[1] * vec[2] * fOMC - vec[0] * fSin;

}


//-------------------------------------------------------------
inline void CMatrix4X4::SetTranslation(float fX, float fY, float fZ)
{
	m_fMat[12] = fX;
	m_fMat[13] = fY;
	m_fMat[14] = fZ;
}

inline void CMatrix4X4::SetTranslation(float * fpValues)
{
	SetTranslation(fpValues[0], fpValues[1], fpValues[2]);
}


//-------------------------------------------------------------
inline void CMatrix4X4::SetInvTranslation(float fX, float fY, float fZ)
{
	m_fMat[12] = -fX;
	m_fMat[13] = -fY;
	m_fMat[14] = -fZ;
}

inline void CMatrix4X4::SetInvTranslation(float * fpValues)
{
	SetInvTranslation(fpValues[0], fpValues[1], fpValues[2]);
}


//-------------------------------------------------------------
inline void CMatrix4X4::FromQuaternion(CQuaternion& rQuat)
{
	float * fQ = rQuat.Get();
	m_fMat[0] = 1.0f - 2.0f * (fQ[1] * fQ[1] + fQ[2] * fQ[2]); 
	m_fMat[1] = 2.0f * (fQ[0] * fQ[1] - fQ[2] * fQ[3]);
	m_fMat[2] = 2.0f * (fQ[0] * fQ[2] + fQ[1] * fQ[3]);

	m_fMat[4] = 2.0f * (fQ[0] * fQ[1] + fQ[2] * fQ[3]);
	m_fMat[5] = 1.0f - 2.0f * (fQ[0] * fQ[0] + fQ[2] * fQ[2]);
	m_fMat[6] = 2.0f * (fQ[1] * fQ[2] - fQ[0] * fQ[3]);
	
	m_fMat[8] = 2.0f * (fQ[0] * fQ[2] - fQ[1] * fQ[3]);
	m_fMat[9] = 2.0f * (fQ[1] * fQ[2] + fQ[0] * fQ[3]);
	m_fMat[10] = 1.0f - 2.0f * (fQ[0] * fQ[0] + fQ[1] * fQ[1]);


	m_fMat[15] = 1.0f;
}


//-------------------------------------------------------------
inline float CMatrix4X4::Determinant()
{
	return( (m_fMat[0] * m_fMat[5] * m_fMat[10] * m_fMat[15]) +
		    (m_fMat[1] * m_fMat[6] * m_fMat[11] * m_fMat[12]) +
			(m_fMat[2] * m_fMat[7] * m_fMat[8] * m_fMat[13]) +
			(m_fMat[3] * m_fMat[4] * m_fMat[9] * m_fMat[14]) - 
			(m_fMat[3] * m_fMat[6] * m_fMat[9] * m_fMat[12]) - 
			(m_fMat[7] * m_fMat[10] * m_fMat[13] * m_fMat[0]) - 
			(m_fMat[11] * m_fMat[14] * m_fMat[1] * m_fMat[4]) - 
			(m_fMat[15] * m_fMat[2] * m_fMat[5] * m_fMat[8]));

}


//-------------------------------------------------------------
inline void CMatrix4X4::InverseRotateVec(float * fpVec)
{
	float tmp[3];

	tmp[0] = fpVec[0] * m_fMat[0] + fpVec[1] * m_fMat[1] + fpVec[2] * m_fMat[2];
	tmp[1] = fpVec[0] * m_fMat[4] + fpVec[1] * m_fMat[5] + fpVec[2] * m_fMat[6];
	tmp[2] = fpVec[0] * m_fMat[8] + fpVec[1] * m_fMat[9] + fpVec[2] * m_fMat[10];

	memcpy(fpVec, tmp, sizeof(float[3]));
}


//-------------------------------------------------------------
#include <assert.h>
inline void CMatrix4X4::InverseTranslateVec(float * fpVec)
{
	assert(fpVec);
	fpVec[0] = fpVec[0] - m_fMat[12];
	fpVec[1] = fpVec[1] - m_fMat[13];
	fpVec[2] = fpVec[2] - m_fMat[14];
}

//-------------------------------------------------------------
//-                      OPERATORS                            -
//-------------------------------------------------------------
//-------------------------------------------------------------

//-------------------------------------------------------------
inline float& CMatrix4X4::operator [](const int iIdx)
{
	return m_fMat[iIdx];
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix4X4 CMatrix4X4::operator +(const CMatrix4X4& rMat) const
{
	return CMatrix4X4(m_fMat[0] + rMat.m_fMat[0], m_fMat[1] + rMat.m_fMat[1], m_fMat[2] + rMat.m_fMat[2], m_fMat[3] + rMat.m_fMat[3], 
		              m_fMat[4] + rMat.m_fMat[4], m_fMat[5] + rMat.m_fMat[5], m_fMat[6] + rMat.m_fMat[6], m_fMat[7] + rMat.m_fMat[7], 
					  m_fMat[8] + rMat.m_fMat[8], m_fMat[9] + rMat.m_fMat[9], m_fMat[10] + rMat.m_fMat[10], m_fMat[11] + rMat.m_fMat[11], 
					  m_fMat[12] + rMat.m_fMat[12], m_fMat[13] + rMat.m_fMat[13], m_fMat[14] + rMat.m_fMat[14], m_fMat[15] + rMat.m_fMat[15]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix4X4 CMatrix4X4::operator -(const CMatrix4X4& rMat) const
{
	return CMatrix4X4(m_fMat[0] - rMat.m_fMat[0], m_fMat[1] - rMat.m_fMat[1], m_fMat[2] - rMat.m_fMat[2], m_fMat[3] - rMat.m_fMat[3], 
		              m_fMat[4] - rMat.m_fMat[4], m_fMat[5] - rMat.m_fMat[5], m_fMat[6] - rMat.m_fMat[6], m_fMat[7] - rMat.m_fMat[7], 
					  m_fMat[8] - rMat.m_fMat[8], m_fMat[9] - rMat.m_fMat[9], m_fMat[10] - rMat.m_fMat[10], m_fMat[11] - rMat.m_fMat[11], 
					  m_fMat[12] - rMat.m_fMat[12], m_fMat[13] - rMat.m_fMat[13], m_fMat[14] - rMat.m_fMat[14], m_fMat[15] - rMat.m_fMat[15]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix4X4::operator +=(const CMatrix4X4& rMat)
{
	m_fMat[0] += rMat.m_fMat[0];
	m_fMat[1] += rMat.m_fMat[1];
	m_fMat[2] += rMat.m_fMat[2];
	m_fMat[3] += rMat.m_fMat[3];
	m_fMat[4] += rMat.m_fMat[4];
	m_fMat[5] += rMat.m_fMat[5];
	m_fMat[6] += rMat.m_fMat[6];
	m_fMat[7] += rMat.m_fMat[7];
	m_fMat[8] += rMat.m_fMat[8];
	m_fMat[9] += rMat.m_fMat[9];
	m_fMat[10] += rMat.m_fMat[10];
	m_fMat[11] += rMat.m_fMat[11];
	m_fMat[12] += rMat.m_fMat[12];
	m_fMat[13] += rMat.m_fMat[13];
	m_fMat[14] += rMat.m_fMat[14];
	m_fMat[15] += rMat.m_fMat[15];
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix4X4::operator -=(const CMatrix4X4& rMat)
{
	m_fMat[0] -= rMat.m_fMat[0];
	m_fMat[1] -= rMat.m_fMat[1];
	m_fMat[2] -= rMat.m_fMat[2];
	m_fMat[3] -= rMat.m_fMat[3];
	m_fMat[4] -= rMat.m_fMat[4];
	m_fMat[5] -= rMat.m_fMat[5];
	m_fMat[6] -= rMat.m_fMat[6];
	m_fMat[7] -= rMat.m_fMat[7];
	m_fMat[8] -= rMat.m_fMat[8];
	m_fMat[9] -= rMat.m_fMat[9];
	m_fMat[10] -= rMat.m_fMat[10];
	m_fMat[11] -= rMat.m_fMat[11];
	m_fMat[12] -= rMat.m_fMat[12];
	m_fMat[13] -= rMat.m_fMat[13];
	m_fMat[14] -= rMat.m_fMat[14];
	m_fMat[15] -= rMat.m_fMat[15];
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix4X4 CMatrix4X4::operator *(const float fScalar) const
{
	return CMatrix4X4(m_fMat[0] * fScalar, m_fMat[1] * fScalar, m_fMat[2] * fScalar, m_fMat[3] * fScalar, 
		              m_fMat[4] * fScalar, m_fMat[5] * fScalar, m_fMat[6] * fScalar, m_fMat[7] * fScalar, 
					  m_fMat[8] * fScalar, m_fMat[9] * fScalar, m_fMat[10] * fScalar, m_fMat[11] * fScalar,
					  m_fMat[12] * fScalar, m_fMat[13] * fScalar, m_fMat[14] * fScalar, m_fMat[15] * fScalar);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix4X4 CMatrix4X4::operator /(const float fScalar) const
{
	float fInvScl = 1/fScalar;
	return CMatrix4X4(m_fMat[0] * fInvScl, m_fMat[1] * fInvScl, m_fMat[2] * fInvScl, m_fMat[3] * fInvScl, 
		              m_fMat[4] * fInvScl, m_fMat[5] * fInvScl, m_fMat[6] * fInvScl, m_fMat[7] * fInvScl, 
					  m_fMat[8] * fInvScl, m_fMat[9] * fInvScl, m_fMat[10] * fInvScl, m_fMat[11] * fInvScl,
					  m_fMat[12] * fInvScl, m_fMat[13] * fInvScl, m_fMat[14] * fInvScl, m_fMat[15] * fInvScl);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix4X4::operator *=(const float fScalar)
{
	m_fMat[0] *= fScalar;
	m_fMat[1] *= fScalar;
	m_fMat[2] *= fScalar;
	m_fMat[3] *= fScalar;
	m_fMat[4] *= fScalar;
	m_fMat[5] *= fScalar;
	m_fMat[6] *= fScalar;
	m_fMat[7] *= fScalar;
	m_fMat[8] *= fScalar;
	m_fMat[9] *= fScalar;
	m_fMat[10] *= fScalar;
	m_fMat[11] *= fScalar;
	m_fMat[12] *= fScalar;
	m_fMat[13] *= fScalar;
	m_fMat[14] *= fScalar;
	m_fMat[15] *= fScalar;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix4X4::operator /=(const float fScalar)
{
	float fInvScl = 1/fScalar;
	m_fMat[0] *= fInvScl;
	m_fMat[1] *= fInvScl;
	m_fMat[2] *= fInvScl;
	m_fMat[3] *= fInvScl;
	m_fMat[4] *= fInvScl;
	m_fMat[5] *= fInvScl;
	m_fMat[6] *= fInvScl;
	m_fMat[7] *= fInvScl;
	m_fMat[8] *= fInvScl;
	m_fMat[9] *= fInvScl;
	m_fMat[10] *= fInvScl;
	m_fMat[11] *= fInvScl;
	m_fMat[12] *= fInvScl;
	m_fMat[13] *= fInvScl;
	m_fMat[14] *= fInvScl;
	m_fMat[15] *= fInvScl;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const CMatrix4X4 CMatrix4X4::operator *(const CMatrix4X4& rMat) const
{

	const float * m1 = m_fMat;
	const float * m2 = rMat.m_fMat;

	return CMatrix4X4(

	(m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3]),
	(m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3]),
	(m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3]),
	(m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3]),

	(m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7]),
	(m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7]),
	(m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7]),
	(m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7]),

	(m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11]),
	(m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11]),
	(m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11]),
	(m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11]),

	(m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15]),
	(m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15]),
	(m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15]),
	(m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15])
	);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix4X4::operator *=(const CMatrix4X4& rMat)
{
	float fNewMat[16];

	const float * m1 = m_fMat;
	const float * m2 = rMat.m_fMat;

	fNewMat[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	fNewMat[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	fNewMat[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	fNewMat[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

	fNewMat[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	fNewMat[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	fNewMat[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	fNewMat[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

	fNewMat[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	fNewMat[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	fNewMat[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	fNewMat[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

	fNewMat[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
	fNewMat[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
	fNewMat[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	fNewMat[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

	memcpy(m_fMat, fNewMat, sizeof(float[16]));
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const void CMatrix4X4::operator =(const CMatrix4X4& rMat)
{
	memcpy(m_fMat, rMat.m_fMat, sizeof(float[16]));
}


//-------------------------------------------------------------

//-------------------------------------------------------------
inline const bool CMatrix4X4::operator ==(const CMatrix4X4& rMat) const
{
	return (m_fMat[0] == rMat.m_fMat[0] && m_fMat[1] == rMat.m_fMat[1] && m_fMat[2] == rMat.m_fMat[2] && 
		m_fMat[3] == rMat.m_fMat[3] && m_fMat[4] == rMat.m_fMat[4] && m_fMat[5] == rMat.m_fMat[5] && 
		m_fMat[6] == rMat.m_fMat[6] && m_fMat[7] == rMat.m_fMat[7] && m_fMat[8] == rMat.m_fMat[8] &&
		m_fMat[9] == rMat.m_fMat[9] && m_fMat[10] == rMat.m_fMat[10] && m_fMat[11] == rMat.m_fMat[11] &&
		m_fMat[12] == rMat.m_fMat[12] && m_fMat[13] == rMat.m_fMat[13] && m_fMat[14] == rMat.m_fMat[14] && m_fMat[15] == rMat.m_fMat[15]);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline const bool CMatrix4X4::operator != (const CMatrix4X4& rMat) const
{
	return !(*this == rMat);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline CMatrix4X4::CMatrix4X4()
{
	memset(m_fMat, 0, sizeof(float[16]));
}

inline CMatrix4X4::CMatrix4X4(const CMatrix4X4& rMat)
{
	*this = rMat;
}

inline CMatrix4X4::CMatrix4X4(float f11, float f12, float f13, float f14,
				float f21, float f22, float f23, float f24, 
				float f31, float f32, float f33, float f34,
				float f41, float f42, float f43, float f44)
{
	m_fMat[0] = f11;
	m_fMat[1] = f12;
	m_fMat[2] = f13;
	m_fMat[3] = f14;
	m_fMat[4] = f21;
	m_fMat[5] = f22;
	m_fMat[6] = f23;
	m_fMat[7] = f24;
	m_fMat[8] = f31;
	m_fMat[9] = f32;
	m_fMat[10] = f33;
	m_fMat[11] = f34;
	m_fMat[12] = f41;
	m_fMat[13] = f42;
	m_fMat[14] = f43;
	m_fMat[15] = f44;
}

inline CMatrix4X4::CMatrix4X4(float * fpMat)
{
	memcpy(m_fMat, fpMat, sizeof(float[16]));
}
//-------------------------------------------------------------

//-------------------------------------------------------------
inline float * CMatrix4X4::Get()
{
	return m_fMat;
}

inline void CMatrix4X4::Set(float f11, float f12, float f13, float f14,
				float f21, float f22, float f23, float f24, 
				float f31, float f32, float f33, float f34,
				float f41, float f42, float f43, float f44)
{
	m_fMat[0] = f11;
	m_fMat[1] = f12;
	m_fMat[2] = f13;
	m_fMat[3] = f14;
	m_fMat[4] = f21;
	m_fMat[5] = f22;
	m_fMat[6] = f23;
	m_fMat[7] = f24;
	m_fMat[8] = f31;
	m_fMat[9] = f32;
	m_fMat[10] = f33;
	m_fMat[11] = f34;
	m_fMat[12] = f41;
	m_fMat[13] = f42;
	m_fMat[14] = f43;
	m_fMat[15] = f44;
}

inline void CMatrix4X4::Set(float * fpMat)
{
	memcpy(m_fMat, fpMat, sizeof(float[16]));
}
//-------------------------------------------------------------
//                    END CMatrix4X4                          -
//-------------------------------------------------------------

#endif //MATRIX_INL