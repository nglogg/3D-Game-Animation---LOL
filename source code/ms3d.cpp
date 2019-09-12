#include "ms3d.h"

#include <stdio.h>
static float LastTime[10]= {0};
static bool FirstTime[10] = {true};

bool CMs3d::Load(const char * szFilename)
{
	unsigned char * ucpBuffer = 0;
	unsigned char * ucpPtr = 0;

	//Open the file
	FILE * f = fopen(szFilename, "rb");
	if(!f)
	{
		printf("\nCould not open %s", szFilename);
		return false;
	}

	//Lay kich thuoc file
	fseek(f, 0, SEEK_END);
	int iEnd = ftell(f);
	fseek(f, 0, SEEK_SET);
	int iStart = ftell(f);
	unsigned int uiSize = iEnd - iStart;
	//Cap phat bo nho
	ucpBuffer = new unsigned char[uiSize];
	ucpPtr = ucpBuffer;
	if(!ucpBuffer)
	{
		printf("\nCould not allocate memory");
		return false;
	}
	//Doc fi le vao bo dem
	if(fread(ucpBuffer, 1, uiSize, f) != uiSize)
	{
		printf("\nCould not read %s", szFilename);
		delete [] ucpBuffer;
		return false;
	}

	//kiem tra header, MS3D000000
	if(memcmp(ucpPtr, "MS3D000000", 10) != 0)
	{
		printf("\n%s is not a valid Milkshape 3D file", szFilename);
		delete [] ucpBuffer;
		return false;
	}
	//kiem tra verson
	ucpPtr += 10;
	if(*(int *)ucpPtr != 3 && *(int *)ucpPtr != 4)
	{
		printf("\n%s is the wrong version, should be 3 or 4", szFilename);
		delete [] ucpBuffer;
		return false;
	}
	ucpPtr += 4;

	//Doc cac dinh
	//So dinh
	m_usNumVerts = *(unsigned short *)ucpPtr;
	ucpPtr += 2;
	//Cap phat bo nho
	m_pVertices = new SMs3dVertex[m_usNumVerts];
	//Copy cac dinh
	memcpy(m_pVertices, ucpPtr, m_usNumVerts * sizeof(SMs3dVertex));
	ucpPtr += m_usNumVerts * sizeof(SMs3dVertex);

	//Doc triangles
	m_usNumTriangles = *(unsigned short *)ucpPtr;
	ucpPtr += 2;
	//cap phat bo nho cho cac triangles
	m_pTriangles = new SMs3dTriangle[m_usNumTriangles];
	//Copy triangles
	memcpy(m_pTriangles, ucpPtr, m_usNumTriangles * sizeof(SMs3dTriangle));
	ucpPtr += m_usNumTriangles * sizeof(SMs3dTriangle);

	//Load mesh groups
	m_usNumMeshes = *(unsigned short *)ucpPtr;
	ucpPtr += 2;
	//cap phat bo nho mesh data
	m_pMeshes = new SMs3dMesh[m_usNumMeshes];
	//Copy  mesh data
	for(int x = 0; x < m_usNumMeshes; x++)
	{
		//Copy phan dau cua du lieu
		memcpy(&m_pMeshes[x], ucpPtr, 35);
		ucpPtr += 35;
		//cap phat bo nho	
		m_pMeshes[x].m_uspIndices = new unsigned short[m_pMeshes[x].m_usNumTris];
		//copy du lieu
		memcpy(m_pMeshes[x].m_uspIndices, ucpPtr, m_pMeshes[x].m_usNumTris * 2);
		ucpPtr += m_pMeshes[x].m_usNumTris * 2;
		m_pMeshes[x].m_cMaterial = ucpPtr[0];
		ucpPtr ++;
	}

	//docdu lieu material 
	m_usNumMaterials = *(unsigned short *)ucpPtr;
	ucpPtr += 2;
	//Cap phat bo nho
	m_pMaterials = new SMs3dMaterial[m_usNumMaterials];
	//Copy du lieu material 


	for(int x = 0; x < m_usNumMaterials; x++)
	{
		memcpy(&m_pMaterials[x], ucpPtr, 361);
		ucpPtr += 361;
		//Load texture images
		if(m_pMaterials[x].m_cTexture[0] != '\0')
		{
			m_pMaterials[x].m_Texture.Load(m_pMaterials[x].m_cTexture);
		}
	}
	
	

	//Bo qua 1 so du lieu khong can thiet
	ucpPtr += 4;
	ucpPtr += 8;

	//Doc du lieu joint va animation
	m_usNumJoints = *(unsigned short *)ucpPtr;
	ucpPtr += 2;
	//Cap phat bo nho
	m_pJoints = new SMs3dJoint[m_usNumJoints];
	//Doc du lieu joint
	for(int x = 0; x < m_usNumJoints; x++)
	{
		memcpy(&m_pJoints[x], ucpPtr, 93);
		ucpPtr += 93;
		//Cap phat bo nho 
		m_pJoints[x].m_RotKeyFrames = new SMs3dKeyFrame[m_pJoints[x].m_usNumRotFrames];
		m_pJoints[x].m_TransKeyFrames = new SMs3dKeyFrame[m_pJoints[x].m_usNumTransFrames];
		//copy du lieu keyframe 
		memcpy(m_pJoints[x].m_RotKeyFrames, ucpPtr, m_pJoints[x].m_usNumRotFrames * sizeof(SMs3dKeyFrame));
		ucpPtr += m_pJoints[x].m_usNumRotFrames * sizeof(SMs3dKeyFrame);
		memcpy(m_pJoints[x].m_TransKeyFrames, ucpPtr, m_pJoints[x].m_usNumTransFrames * sizeof(SMs3dKeyFrame));
		ucpPtr += m_pJoints[x].m_usNumTransFrames * sizeof(SMs3dKeyFrame);

	}

	//tim chi so joint cha
	for(int x = 0; x < m_usNumJoints; x++)
	{
		//Neu bone co cha
		if(m_pJoints[x].m_cParent[0] != '\0')
		{
			//Sao sanh ten cua joint cha voi cac joint con lai
			for(int y = 0; y < m_usNumJoints; y++)
			{
				//da tim thay ten phu hop
				if(strcmp(m_pJoints[y].m_cName, m_pJoints[x].m_cParent) == 0)
				{
					m_pJoints[x].m_sParent = y;
				}
			}
		}
		//bone khong co cha
		else
		{
			m_pJoints[x].m_sParent = -1;
		}
	}

	//Setup joints 
	Setup();

	//File load thanh cong
	delete [] ucpBuffer;
	printf("\nMS3D File: %s Loaded", szFilename);
	return true;
}
void CMs3d::Render(bool anim, int i, float speed )
{
	//ham nay khong duoc su dung nhieu
	float endtime = 0;
	if(anim)
	 endtime = m_pJoints[0].m_RotKeyFrames[m_pJoints[0].m_usNumRotFrames-1].m_fTime;
		
	Animate(speed, 0.0f, endtime, true, i );
}

void CMs3d::Render(float fTime)
{
	Animate(3.0f, fTime, fTime, true, 0 );
}

void CMs3d::Animate(float fSpeed, float fStartTime, float fEndTime, bool bLoop,int i )
{
	float fTime =0;
	//toi da 7 chuyen dong khac nhau xay ra cung 1 luc
	if(i==0)
	{
		static bool bFirstTime = true;
		//lan dau animate duoc goi
		if(bFirstTime)
		{
			Reset();
			bFirstTime = false;
		}
		static float fLastTime = fStartTime;

	
		 fTime = CTimer::Instance()->GetSeconds() * fSpeed;
		fTime += fLastTime;
		fLastTime = fTime;

		//looping
		if(fTime > fEndTime)
		{
			if(bLoop)
			{
				Reset();
				fLastTime = fStartTime;
				fTime = fStartTime;
			}
			else
				fTime = fEndTime;
		}	
	}
	else if(i==1)
	{
		static bool bFirstTime1 = true;
		//lan dau animate duoc goi
		if(bFirstTime1)
		{
			Reset();
			bFirstTime1 = false;
		}
		static float fLastTime1 = fStartTime;


		fTime = CTimer::Instance()->GetSeconds() * fSpeed;
		fTime += fLastTime1;
		fLastTime1 = fTime;

		//looping
		if(fTime > fEndTime)
		{
			if(bLoop)
			{
				Reset();
				fLastTime1 = fStartTime;
				fTime = fStartTime;
			}
			else
				fTime = fEndTime;
		}	
	}
	else if(i==2)
	{
		static bool bFirstTime2 = true;
		//lan dau animate duoc goi
		if(bFirstTime2)
		{
			Reset();
			bFirstTime2 = false;
		}
		static float fLastTime2 = fStartTime;


		fTime = CTimer::Instance()->GetSeconds() * fSpeed;
		fTime += fLastTime2;
		fLastTime2 = fTime;

		//looping
		if(fTime > fEndTime)
		{
			if(bLoop)
			{
				Reset();
				fLastTime2 = fStartTime;
				fTime = fStartTime;
			}
			else
				fTime = fEndTime;
		}	
	}
	else if(i==3)
	{
		static bool bFirstTime3 = true;
		//lan dau animate duoc goi
		if(bFirstTime3)
		{
			Reset();
			bFirstTime3 = false;
		}
		static float fLastTime3 = fStartTime;


		fTime = CTimer::Instance()->GetSeconds() * fSpeed;
		fTime += fLastTime3;
		fLastTime3 = fTime;

		//looping
		if(fTime > fEndTime)
		{
			if(bLoop)
			{
				Reset();
				fLastTime3 = fStartTime;
				fTime = fStartTime;
			}
			else
				fTime = fEndTime;
		}	
	}
	else if(i==4)
	{
		static bool bFirstTime4 = true;
		//lan dau animate duoc goi
		if(bFirstTime4)
		{
			Reset();
			bFirstTime4 = false;
		}
		static float fLastTime4 = fStartTime;


		fTime = CTimer::Instance()->GetSeconds() * fSpeed;
		fTime += fLastTime4;
		fLastTime4 = fTime;

		//looping
		if(fTime > fEndTime)
		{
			if(bLoop)
			{
				Reset();
				fLastTime4 = fStartTime;
				fTime = fStartTime;
			}
			else
				fTime = fEndTime;
		}	
	}
	else if(i==5)
	{
		static bool bFirstTime5 = true;
		//lan dau animate duoc goi
		if(bFirstTime5)
		{
			Reset();
			bFirstTime5 = false;
		}
		static float fLastTime5 = fStartTime;


		fTime = CTimer::Instance()->GetSeconds() * fSpeed;
		fTime += fLastTime5;
		fLastTime5 = fTime;

		//looping
		if(fTime > fEndTime)
		{
			if(bLoop)
			{
				Reset();
				fLastTime5 = fStartTime;
				fTime = fStartTime;
			}
			else
				fTime = fEndTime;
		}	
	}
	else if(i==6)
	{
		static bool bFirstTime6 = true;
		//lan dau animate duoc goi
		if(bFirstTime6)
		{
			Reset();
			bFirstTime6 = false;
		}
		static float fLastTime6 = fStartTime;


		fTime = CTimer::Instance()->GetSeconds() * fSpeed;
		fTime += fLastTime6;
		fLastTime6 = fTime;

		//looping
		if(fTime > fEndTime)
		{
			if(bLoop)
			{
				Reset();
				fLastTime6 = fStartTime;
				fTime = fStartTime;
			}
			else
				fTime = fEndTime;
		}	
	}



	for(int x = 0; x < m_usNumJoints; x++)
	{
		//Transformation matrix
		CMatrix4X4 matTmp;
		//Current joint
		SMs3dJoint * pJoint = &m_pJoints[x];
		//Current frame
		unsigned int uiFrame = 0;

		//Neu khong co key frame thi khong can bien doi
		if(pJoint->m_usNumRotFrames == 0 && pJoint->m_TransKeyFrames == 0  )
		{
			pJoint->m_matFinal = pJoint->m_matAbs;
			continue;
		}
		//Tinh toan frame hien tai
		//Translation
		while(uiFrame < pJoint->m_usNumTransFrames && pJoint->m_TransKeyFrames[uiFrame].m_fTime < fTime)
			uiFrame++;
		pJoint->m_usCurTransFrame = uiFrame;

		float fTranslation[3];
		float fDeltaT = 1;
		float fInterp = 0;

		
		if(uiFrame == 0)
			memcpy(fTranslation, pJoint->m_TransKeyFrames[0].m_fParam, sizeof(float[3]));
		else if(uiFrame == pJoint->m_usNumTransFrames)
			memcpy(fTranslation, pJoint->m_TransKeyFrames[uiFrame-1].m_fParam, sizeof(float[3]));
	
		else
		{
			SMs3dKeyFrame * pkCur = &pJoint->m_TransKeyFrames[uiFrame];
			SMs3dKeyFrame * pkPrev = &pJoint->m_TransKeyFrames[uiFrame-1];

			fDeltaT = pkCur->m_fTime - pkPrev->m_fTime;
			fInterp = (fTime - pkPrev->m_fTime) / fDeltaT;

			//noi suy cua translations
			fTranslation[0] = pkPrev->m_fParam[0] + (pkCur->m_fParam[0] - pkPrev->m_fParam[0]) * fInterp;
			fTranslation[1] = pkPrev->m_fParam[1] + (pkCur->m_fParam[1] - pkPrev->m_fParam[1]) * fInterp;
			fTranslation[2] = pkPrev->m_fParam[2] + (pkCur->m_fParam[2] - pkPrev->m_fParam[2]) * fInterp;
		}
		//tinh toan rotation
		uiFrame = 0;
		while(uiFrame < pJoint->m_usNumRotFrames && pJoint->m_RotKeyFrames[uiFrame].m_fTime < fTime)
			uiFrame++;



		if(uiFrame == 0)
			matTmp.SetRotation(pJoint->m_RotKeyFrames[0].m_fParam);
		else if(uiFrame == pJoint->m_usNumTransFrames)
			matTmp.SetRotation(pJoint->m_RotKeyFrames[uiFrame-1].m_fParam);
		
		else
		{
			SMs3dKeyFrame * pkCur = &pJoint->m_RotKeyFrames[uiFrame];
			SMs3dKeyFrame * pkPrev = &pJoint->m_RotKeyFrames[uiFrame-1];

			fDeltaT = pkCur->m_fTime - pkPrev->m_fTime;
			fInterp = (fTime - pkPrev->m_fTime) / fDeltaT;

			//tạo quaternion 
			CQuaternion qCur;
			CQuaternion qPrev;
			qCur.FromEulers(pkCur->m_fParam);
			qPrev.FromEulers(pkPrev->m_fParam);
			//SLERP giua 2 frame
			CQuaternion qFinal = SLERP(qPrev, qCur, fInterp);

			//chuyen doi snag ma tran
			matTmp = qFinal.ToMatrix4();
		}


		matTmp.SetTranslation(fTranslation);

		//tinh toan ma tran bien doi cuoi cung
		CMatrix4X4 matFinal = pJoint->m_matLocal * matTmp;

		
		if(pJoint->m_sParent == -1)
			pJoint->m_matFinal = matFinal;

		else
			pJoint->m_matFinal = m_pJoints[pJoint->m_sParent].m_matFinal * matFinal;
	}


	//Bien doi va render meshs
	if(m_bDrawMesh)
		RenderT();

	if(m_bDrawBones) //m_bDrawBones
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glLineWidth(5);
		//Draw the bones
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);	
		for(int x = 1; x < m_usNumJoints; x++)
		{
			float * fMat = m_pJoints[x].m_matFinal.Get();
			float * fMatParent = m_pJoints[m_pJoints[x].m_sParent].m_matFinal.Get();
			glVertex3f(fMat[12], fMat[13], fMat[14]);
			glVertex3f(fMatParent[12], fMatParent[13], fMatParent[14]);
		}
		glEnd();

		glEnable(GL_DEPTH_TEST);
	}
}

void CMs3d::RenderT()
{
	glEnable(GL_TEXTURE_2D);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for(int x = 0; x < m_usNumMeshes; x++)
	{
		//thiet lap materials
		if(m_pMeshes[x].m_cMaterial >= 0)
		{  
			SMs3dMaterial * pCurMat = &m_pMaterials[m_pMeshes[x].m_cMaterial];
			//thiet lap alpha cho transparency
			pCurMat->m_fDiffuse[3] = pCurMat->m_fTransparency;

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pCurMat->m_fAmbient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pCurMat->m_fDiffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pCurMat->m_fSpecular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pCurMat->m_fEmissive);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pCurMat->m_fShininess);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//texture mapping
			pCurMat->m_Texture.Bind();
		}
		else
			glDisable(GL_BLEND);

		//Ve mesh
		//Bien doi cac dinh va normals
		CVector3 vecNormal;
		CVector3 vecVertex;

		glBegin(GL_TRIANGLES);
		
		for(int y = 0; y < m_pMeshes[x].m_usNumTris; y++)
		{
			

			SMs3dTriangle * pTri = &m_pTriangles[m_pMeshes[x].m_uspIndices[y]];
		
			for(int z = 0; z < 3; z++)
			{
				
				SMs3dVertex * pVert = &m_pVertices[pTri->m_usVertIndices[z]];

				//Neu khong co cha
				if(pVert->m_cBone == -1)
				{
					//render khong co transform nao dc thuc hien
					glTexCoord2f(pTri->m_fTexCoords[0][z],1- pTri->m_fTexCoords[1][z]);
					glVertex3fv(pVert->m_vVert.Get());
					glNormal3fv(pTri->m_vNormals[z].Get());
				}
				//neu khong bien doi truoc khi render
				else
				{
					
					glTexCoord2f(pTri->m_fTexCoords[0][z], 1- pTri->m_fTexCoords[1][z]);

					SMs3dJoint * pJoint = &m_pJoints[pVert->m_cBone];
					//Bien doi normals
					vecNormal = pTri->m_vNormals[z];
					
					vecNormal.Transform3(pJoint->m_matFinal);
					
					glNormal3fv(vecNormal.Get());

			
					vecVertex = pVert->m_vVert;
				
					vecVertex.Transform4(pJoint->m_matFinal);
				
					glVertex3fv(vecVertex.Get());

				}
			}
		}

		glEnd();
	}
}

void CMs3d::Reset()
{
	CTimer::Instance()->Init();
	CTimer::Instance()->GetSeconds();
}

void CMs3d::SwapBoneDraw()
{
	m_bDrawBones = !m_bDrawBones;
}

void CMs3d::SwapMeshDraw()
{
	m_bDrawMesh = !m_bDrawMesh;
}

bool CMs3d::AreBonesEnabled()
{
	return m_bDrawBones;
}

bool CMs3d::AreMeshesEnabled()
{
	return m_bDrawMesh;
}

void CMs3d::Setup()
{
	
	for(int x = 0; x < m_usNumJoints; x++)
	{
		m_pJoints[x].m_matLocal.SetRotation(m_pJoints[x].m_fRotation);
		m_pJoints[x].m_matLocal.SetTranslation(m_pJoints[x].m_fPosition);

		
		if(m_pJoints[x].m_sParent != -1)
		{
			m_pJoints[x].m_matAbs = m_pJoints[m_pJoints[x].m_sParent].m_matAbs * m_pJoints[x].m_matLocal;

		}
		
		else
		{
			m_pJoints[x].m_matAbs = m_pJoints[x].m_matLocal;
		}
		m_pJoints[x].m_matFinal = m_pJoints[x].m_matAbs;
	}

	for(int x = 0; x < m_usNumVerts; x++)
	{

		if(m_pVertices[x].m_cBone== -1)
			continue;

		CMatrix4X4 * mat = &m_pJoints[m_pVertices[x].m_cBone].m_matFinal;


		mat->InverseTranslateVec(m_pVertices[x].m_vVert.Get());
		mat->InverseRotateVec(m_pVertices[x].m_vVert.Get());
	}

	for(int x = 0; x < m_usNumTriangles; x++)
	{
		SMs3dTriangle * pTri = &m_pTriangles[x];

	
		for(int z = 0; z < 3; z++)
		{
		
			SMs3dVertex * pVert = &m_pVertices[pTri->m_usVertIndices[z]];

		
			if(pVert->m_cBone == -1)
				continue;

			SMs3dJoint * pJoint = &m_pJoints[pVert->m_cBone];

	
			pJoint->m_matFinal.InverseRotateVec(pTri->m_vNormals[z].Get());

		}
	}
}

CMs3d::CMs3d()
{
	m_pVertices = 0;
	m_pTriangles = 0;
	m_pMeshes = 0;
	m_pMaterials = 0;
	m_bDrawMesh = true;
	m_bDrawBones = false;
	m_Timer.Init();
}

CMs3d::CMs3d(const char * szFilename )
{
	m_pVertices = 0;
	m_pTriangles = 0;
	m_pMeshes = 0;
	m_pMaterials = 0;
	m_bDrawMesh = true;
	m_bDrawBones = false;
	m_Timer.Init();
	Load(szFilename);
}

CMs3d::~CMs3d()
{
	if(m_pVertices)
	{
		delete [] m_pVertices;
		m_pVertices = 0;
	}
	if(m_pTriangles)
	{
		delete [] m_pTriangles;
		m_pTriangles = 0;
	}
	if(m_pMeshes)
	{
		delete [] m_pMeshes;
		m_pMeshes = 0;
	}
	if(m_pMaterials)
	{
		delete [] m_pMaterials;
		m_pMaterials = 0;
	}
}