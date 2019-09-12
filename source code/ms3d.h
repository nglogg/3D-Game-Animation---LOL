#ifndef MS3D_H
#define MS3D_H
#include "math.h"
#include "model.h"
#include "image.h"
#include "timer.h"
#pragma pack(push, packing)
#pragma pack(1)

struct SMs3dVertex
{
	unsigned char m_ucFlags; 
	CVector3 m_vVert;       
	char m_cBone;            
	unsigned char m_ucUnused;
};
struct SMs3dTriangle
{
	unsigned short m_usFlags;          
	unsigned short m_usVertIndices[3]; 
	CVector3 m_vNormals[3];            
	float m_fTexCoords[2][3];          
	unsigned char m_ucSmoothing;      
	unsigned char m_ucGroup;           
};

struct SMs3dMesh
{
	unsigned char m_ucFlags;  
	char m_cName[32];         
	unsigned short m_usNumTris;
	unsigned short * m_uspIndices; 
	char m_cMaterial;         

	
	SMs3dMesh()
	{
		m_uspIndices = 0;
	}
	~SMs3dMesh()
	{
		if(m_uspIndices)
		{
			delete [] m_uspIndices;
			m_uspIndices = 0;
		}
	}
};

struct SMs3dMaterial
{
	char m_cName[32];         
	float m_fAmbient[4];      
	float m_fDiffuse[4];      
	float m_fSpecular[4];     
	float m_fEmissive[4];     
	float m_fShininess;       
	float m_fTransparency;    
	char m_cMode;             
	char m_cTexture[128];     
	char m_cAlpha[128];       
	CImage m_Texture;
};

struct SMs3dKeyFrame
{
	float m_fTime;     
	float m_fParam[3]; 
};

struct SMs3dJoint
{
	//Data from file
	unsigned char m_ucpFlags;             
	char m_cName[32];                     
	char m_cParent[32];                   
	float m_fRotation[3];                 
	float m_fPosition[3];                 
	unsigned short m_usNumRotFrames;      
	unsigned short m_usNumTransFrames;    

	SMs3dKeyFrame * m_RotKeyFrames;       
	SMs3dKeyFrame * m_TransKeyFrames;    

	//Data not loaded from file
	short m_sParent;                     

	CMatrix4X4 m_matLocal;       
	CMatrix4X4 m_matAbs;
	CMatrix4X4 m_matFinal;

	unsigned short m_usCurRotFrame;
	unsigned short m_usCurTransFrame;

	
	SMs3dJoint()
	{
		m_RotKeyFrames = 0;
		m_TransKeyFrames = 0;
		m_usCurRotFrame = 0;
		m_usCurTransFrame = 0;
	}
	~SMs3dJoint()
	{
		if(m_RotKeyFrames)
		{
			delete [] m_RotKeyFrames;
			m_RotKeyFrames = 0;
		}
		if(m_TransKeyFrames)
		{
			delete [] m_TransKeyFrames;
			m_TransKeyFrames = 0;
		}
	}
};
#pragma pack(pop, packing)


class CMs3d
{
public:

	bool Load(const char * szFilename);

	void Render(bool anim, int i, float speed );
	
	void Render(float fTime);

	void Animate(float fSpeed, float fStartTime, float fEndTime, bool bLoop, int i );


	void SwapBoneDraw();
	
	void SwapMeshDraw();
	
	bool AreBonesEnabled();
	bool AreMeshesEnabled();


	//Constructors
	CMs3d();
	CMs3d(const char * szFilename);
	//Destructor
	~CMs3d();
	
private:


	float m_fTotalTime;
	bool bFirstTime;
	float fLastTime; 

	bool m_bDrawBones;

	bool m_bDrawMesh;


	void Setup();
	//Reset timer
	void Reset();
	
	void RenderT();

	//Data
	unsigned short m_usNumVerts;
	unsigned short m_usNumTriangles;
	unsigned short m_usNumMeshes;
	unsigned short m_usNumMaterials;
	unsigned short m_usNumJoints;

	SMs3dVertex * m_pVertices;
	SMs3dTriangle * m_pTriangles;
	SMs3dMesh * m_pMeshes;
	SMs3dMaterial * m_pMaterials;
	SMs3dJoint * m_pJoints;

	//Timer
	CTimer m_Timer;
	
};

#endif