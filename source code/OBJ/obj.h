
#ifndef OBJ_H
#define OBJ_H


//-------------------------------------------------------------
#include <vector>
#include "image.h"

#include "model.h"


using std::vector;


struct Vector3
{
	float m_fVec[3];

	Vector3(float * fVec)
	{
		memcpy(m_fVec, fVec, sizeof(float[3]));
	}
};
struct Vector2
{
	float m_fVec[2];

	Vector2(float * fVec)
	{
		memcpy(m_fVec, fVec, sizeof(float[2]));
	}
};


//-------------------------------------------------------------

struct SObjFace
{
	unsigned int m_uiVertIdx[3];
	unsigned int m_uiTexCoordIdx[3];
	unsigned int m_uiNormalIdx[3];

	SObjFace(int * ipData)
	{
	
		m_uiVertIdx[0] = ipData[0]-1;
		m_uiVertIdx[1] = ipData[1]-1;
		m_uiVertIdx[2] = ipData[2]-1;

		m_uiTexCoordIdx[0] = ipData[3]-1;
		m_uiTexCoordIdx[1] = ipData[4]-1;
		m_uiTexCoordIdx[2] = ipData[5]-1;

		m_uiNormalIdx[0] = ipData[6]-1;
		m_uiNormalIdx[1] = ipData[7]-1;
		m_uiNormalIdx[2] = ipData[8]-1;
	}
};


class CObj : public CModel
{
public:


	bool Load(const char * szFilename, char* fileimage);

	void Render();

	void SetSkin(CImage& rSkin);
	bool Load(const char * szFilename) { return  true;}

	CObj();

	CObj(const char * szFilename, char* fileimage);
	~CObj(){};


private:


	vector<Vector3> m_vVertices;

	vector<Vector3> m_vNormals;

	vector<Vector2> m_vTexCoords;

	vector<SObjFace> m_vFaces;



	CImage * m_pImage;


	Vector3 * m_pVerts;
	Vector3 * m_pNormals;
	Vector2 * m_pTexCoords;
	SObjFace * m_pFaces;

	bool m_bHasTexCoords;
	bool m_bHasNormals;

};


#endif //OBJ_H