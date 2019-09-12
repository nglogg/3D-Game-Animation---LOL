
#ifndef VECTOR_INL
#define VECTOR_INL


#include <cmath>


inline float CVector2::Magnitude() const
{
	return (sqrtf( (m_fVec[0] * m_fVec[0]) + (m_fVec[1] * m_fVec[1]) ));
}


inline void CVector2::Normalize()
{
	float fInvMag = 1/Magnitude();
	m_fVec[0] *= fInvMag;
	m_fVec[1] *= fInvMag;
}


inline void CVector2::Negate()
{
	m_fVec[0] = -m_fVec[0];
	m_fVec[1] = -m_fVec[1];
}


inline float CVector2::Dot(const CVector2& vec) const
{
	return( (vec.m_fVec[0] * m_fVec[0]) + (vec.m_fVec[1] * m_fVec[1]) );
}



inline float& CVector2::operator [](const int iIdx)
{
	return m_fVec[iIdx];
}


inline const CVector2 CVector2::operator +(const CVector2& vec) const
{
	return( CVector2(
		( vec.m_fVec[0] + m_fVec[0] ),
		( vec.m_fVec[1] + m_fVec[1] )));
}


inline const CVector2 CVector2::operator -(const CVector2& vec) const
{
	return( CVector2(
		( m_fVec[0] - vec.m_fVec[0] ),
		( m_fVec[1] - vec.m_fVec[1] )));
}


inline const void CVector2::operator += (const CVector2& vec)
{
	m_fVec[0] += vec.m_fVec[0];
	m_fVec[1] += vec.m_fVec[1];
}


inline const void CVector2::operator -= (const CVector2& vec)
{
	m_fVec[0] -= vec.m_fVec[0];
	m_fVec[1] -= vec.m_fVec[1];
}


inline const CVector2 CVector2::operator -()
{
	return( CVector2(-m_fVec[0], -m_fVec[1]) );
}


inline const CVector2 CVector2::operator *(const float fScalar) const
{
	return( CVector2(
		( m_fVec[0] * fScalar ),
		( m_fVec[1] * fScalar )));
}


inline const CVector2 CVector2::operator /(const float fScalar) const
{
	float fInvScale = 1/fScalar;

	return( CVector2(
		( m_fVec[0] * fInvScale ),
		( m_fVec[1] * fInvScale )));
}


inline const void CVector2::operator *=(const float fScalar)
{
	m_fVec[0] *= fScalar;
	m_fVec[1] *= fScalar;
}


inline const void CVector2::operator /=(const float fScalar)
{
	float fInvScale = 1/fScalar;

	m_fVec[0] *= fInvScale;
	m_fVec[1] *= fInvScale;
}


inline const void CVector2::operator =(const CVector2& vec)
{
	m_fVec[0] = vec.m_fVec[0];
	m_fVec[1] = vec.m_fVec[1];
}


inline const bool CVector2::operator ==(const CVector2& vec) const
{
	return ( 
		(m_fVec[0] == vec.m_fVec[0]) &&
		(m_fVec[1] == vec.m_fVec[1]));
}


inline const bool CVector2::operator !=(const CVector2& vec) const
{
	return (!(*this == vec));
}


inline CVector2::CVector2()
{
	m_fVec[0] = 0;
	m_fVec[1] = 0;
}

inline CVector2::CVector2(const CVector2& vec)
{
	m_fVec[0] = vec.m_fVec[0];
	m_fVec[1] = vec.m_fVec[1];
}

inline CVector2::CVector2(float fX, float fY)
{
	m_fVec[0] = fX;
	m_fVec[1] = fY;
}

inline CVector2::CVector2(float * vec)
{
	m_fVec[0] = vec[0];
	m_fVec[1] = vec[1];
}


inline float * CVector2::Get()
{
	return m_fVec;
}

inline void CVector2::Set(float fX, float fY)
{
	m_fVec[0] = fX;
	m_fVec[1] = fY;
}

inline void CVector2::Set(float * vec)
{
	m_fVec[0] = vec[0];
	m_fVec[1] = vec[1];
}


inline float CVector3::Magnitude() const
{
	return (sqrtf( (m_fVec[0] * m_fVec[0]) + (m_fVec[1] * m_fVec[1]) + (m_fVec[2] * m_fVec[2])));
}


inline void CVector3::Normalize()
{
	float fInvMag = 1/Magnitude();
	m_fVec[0] *= fInvMag;
	m_fVec[1] *= fInvMag;
	m_fVec[2] *= fInvMag;
}


inline void CVector3::Negate()
{
	m_fVec[0] = -m_fVec[0];
	m_fVec[1] = -m_fVec[1];
	m_fVec[2] = -m_fVec[2];
}


inline float CVector3::Dot(const CVector3& vec) const
{
	return( (vec.m_fVec[0] * m_fVec[0]) + (vec.m_fVec[1] * m_fVec[1]) + (vec.m_fVec[2] * m_fVec[2]) );
}


inline CVector3 CVector3::Cross(const CVector3& vec) const
{
		return ( CVector3(
			(m_fVec[1] * vec.m_fVec[2] - 
	         m_fVec[2] * vec.m_fVec[1]),
            (m_fVec[2] * vec.m_fVec[0] - 
	         m_fVec[0] * vec.m_fVec[2]),
            (m_fVec[0] * vec.m_fVec[1] - 
	         m_fVec[1] * vec.m_fVec[0])));
}


inline void CVector3::Transform3(const CMatrix3X3& mat)
{
	CVector3 temp (
		(m_fVec[0] * mat.m_fMat[0] + 
	     m_fVec[1] * mat.m_fMat[3] + 
	     m_fVec[2] * mat.m_fMat[6]),
	
	    (m_fVec[0] * mat.m_fMat[1] +
	     m_fVec[1] * mat.m_fMat[4] + 
         m_fVec[2] * mat.m_fMat[7]),
	
	    (m_fVec[0] * mat.m_fMat[2] +
	     m_fVec[1] * mat.m_fMat[5] +
	     m_fVec[2] * mat.m_fMat[8]));
	
	*this = temp;
}

inline void CVector3::Transform3(const CMatrix4X4& mat)
{
	CVector3 temp( 
		(m_fVec[0] * mat.m_fMat[0] + 
	    m_fVec[1] * mat.m_fMat[4] +
	    m_fVec[2] * mat.m_fMat[8]),

		(m_fVec[0] * mat.m_fMat[1] + 
	    m_fVec[1] * mat.m_fMat[5] + 
	    m_fVec[2] * mat.m_fMat[9]),

	    (m_fVec[0] * mat.m_fMat[2] + 
	    m_fVec[1] * mat.m_fMat[6] + 
	    m_fVec[2] * mat.m_fMat[10]) );

	*this = temp;
}


inline void CVector3::Transform4(const CMatrix4X4& mat)
{
	CVector3 temp( 
		(m_fVec[0] * mat.m_fMat[0] + 
	    m_fVec[1] * mat.m_fMat[4] +
	    m_fVec[2] * mat.m_fMat[8] + 
	    mat.m_fMat[12] ),

		(m_fVec[0] * mat.m_fMat[1] + 
	    m_fVec[1] * mat.m_fMat[5] + 
	    m_fVec[2] * mat.m_fMat[9] + 
	    mat.m_fMat[13] ),

	    (m_fVec[0] * mat.m_fMat[2] + 
	    m_fVec[1] * mat.m_fMat[6] + 
	    m_fVec[2] * mat.m_fMat[10]+
	    mat.m_fMat[14]) );

	*this = temp;
}


inline void CVector3::TransformQ(const CQuaternion& quat)
{
	CQuaternion qv(m_fVec[0], m_fVec[1], m_fVec[2], 0);
	CQuaternion qF = quat * qv * (~quat);
	*this = qF.GetVector();
}


inline float& CVector3::operator [](const int iIdx)
{
	return m_fVec[iIdx];
}


inline const CVector3 CVector3::operator +(const CVector3& vec) const
{
	return( CVector3(
		( vec.m_fVec[0] + m_fVec[0] ),
		( vec.m_fVec[1] + m_fVec[1] ),
		( vec.m_fVec[2] + m_fVec[2] )));
}


inline const CVector3 CVector3::operator -(const CVector3& vec) const
{
	return( CVector3(
		( m_fVec[0] - vec.m_fVec[0] ),
		( m_fVec[1] - vec.m_fVec[1] ),
		( m_fVec[2] - vec.m_fVec[2] )));
}


inline const void CVector3::operator += (const CVector3& vec)
{
	m_fVec[0] += vec.m_fVec[0];
	m_fVec[1] += vec.m_fVec[1];
	m_fVec[2] += vec.m_fVec[2];
}


inline const void CVector3::operator -= (const CVector3& vec)
{
	m_fVec[0] -= vec.m_fVec[0];
	m_fVec[1] -= vec.m_fVec[1];
	m_fVec[2] -= vec.m_fVec[2];
}


inline const CVector3 CVector3::operator -()
{
	return( CVector3(-m_fVec[0], -m_fVec[1], -m_fVec[2]) );
}


inline const CVector3 CVector3::operator *(const float fScalar) const
{
	return( CVector3(
		( m_fVec[0] * fScalar ),
		( m_fVec[1] * fScalar ),
		( m_fVec[2] * fScalar )));
}


inline const CVector3 CVector3::operator /(const float fScalar) const
{
	float fInvScale = 1/fScalar;

	return( CVector3(
		( m_fVec[0] * fInvScale ),
		( m_fVec[1] * fInvScale ),
		( m_fVec[2] * fInvScale )));
}


inline const void CVector3::operator *=(const float fScalar)
{
	m_fVec[0] *= fScalar;
	m_fVec[1] *= fScalar;
	m_fVec[2] *= fScalar;
}


inline const void CVector3::operator /=(const float fScalar)
{
	float fInvScale = 1/fScalar;

	m_fVec[0] *= fInvScale;
	m_fVec[1] *= fInvScale;
	m_fVec[2] *= fInvScale;
}


inline const void CVector3::operator =(const CVector3& vec)
{
	m_fVec[0] = vec.m_fVec[0];
	m_fVec[1] = vec.m_fVec[1];
	m_fVec[2] = vec.m_fVec[2];
}


inline const bool CVector3::operator ==(const CVector3& vec) const
{
	return ( 
		(m_fVec[0] == vec.m_fVec[0]) &&
		(m_fVec[1] == vec.m_fVec[1]) &&
		(m_fVec[2] == vec.m_fVec[2]));
}


inline const bool CVector3::operator !=(const CVector3& vec) const
{
	return (!(*this == vec));
}


inline CVector3::CVector3()
{
	m_fVec[0] = 0;
	m_fVec[1] = 0;
	m_fVec[2] = 0;
}

inline CVector3::CVector3(const CVector3& vec)
{
	m_fVec[0] = vec.m_fVec[0];
	m_fVec[1] = vec.m_fVec[1];
	m_fVec[2] = vec.m_fVec[2];
}

inline CVector3::CVector3(float fX, float fY, float fZ)
{
	m_fVec[0] = fX;
	m_fVec[1] = fY;
	m_fVec[2] = fZ;
}

inline CVector3::CVector3(float * vec)
{
	m_fVec[0] = vec[0];
	m_fVec[1] = vec[1];
	m_fVec[2] = vec[2];
}


inline float * CVector3::Get()
{
	return m_fVec;
}

inline void CVector3::Set(float fX, float fY, float fZ)
{
	m_fVec[0] = fX;
	m_fVec[1] = fY;
	m_fVec[2] = fZ;
}

inline void CVector3::Set(float * vec)
{
	m_fVec[0] = vec[0];
	m_fVec[1] = vec[1];
	m_fVec[2] = vec[2];
}



#endif //VECTOR_INL