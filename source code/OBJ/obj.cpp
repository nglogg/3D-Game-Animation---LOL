
//-------------------------------------------------------------

//-------------------------------------------------------------
#include "obj.h"

#include <cstdio>

//-------------------------------------------------------------

//-------------------------------------------------------------
bool CObj::Load(const char * szFilename, char* fileimage )
{
	char cLine[256];	


	FILE * fp = fopen(szFilename, "rt");
	if(!fp)
	{
		printf("\nCould not open %s", szFilename);
		return false;
	}

	while(!feof(fp))
	{
	
		int iStart = fgetc(fp);

	
		if(iStart == 'v')
		{
		
			int iNext = fgetc(fp);
			float fTemp[3];

			
			if(iNext == ' ' || iNext == '\t')
			{
			
				fgets(cLine, 256, fp);
				
				sscanf(cLine, " %f %f %f", &fTemp[0], &fTemp[1], &fTemp[2]);
				
				m_vVertices.push_back(fTemp);
			}
		
			else if(iNext == 't')
			{
			
				fgets(cLine, 256, fp);
			
				sscanf(cLine, " %f %f", &fTemp[0], &fTemp[1]);
		
				m_vTexCoords.push_back(fTemp);
				m_bHasTexCoords = true;
			}
	
			else if(iNext == 'n')
			{
		
				fgets(cLine, 256, fp);
		
				sscanf(cLine, " %f %f %f", &fTemp[0], &fTemp[1], &fTemp[2]);
		
				m_vNormals.push_back(fTemp);
				m_bHasNormals = true;
			}
		
			else
			{
			
				fgets(cLine, 256, fp);
			}


		}
	
		else if(iStart == 'f')
		{
		
			int iTemp[3][3];
			memset(iTemp, 0, 36);
		
			fgets(cLine, 256, fp);

		
			if(m_bHasTexCoords && m_bHasNormals)
			{
		
				sscanf(cLine, " %i/%i/%i %i/%i/%i %i/%i/%i", &iTemp[0][0], &iTemp[1][0], &iTemp[2][0], 
					&iTemp[0][1], &iTemp[1][1], &iTemp[2][1],
					&iTemp[0][2], &iTemp[1][2], &iTemp[2][2]);
			
				m_vFaces.push_back(&iTemp[0][0]);

			}
		
			else if(m_bHasTexCoords && !m_bHasNormals)
			{
		
				sscanf(cLine, " %i/%i %i/%i %i/%i", &iTemp[0][0], &iTemp[1][0], 
					&iTemp[0][1], &iTemp[1][1], 
					&iTemp[0][2], &iTemp[1][2]);

			
				m_vFaces.push_back(&iTemp[0][0]);
			}
	
			else if(!m_bHasTexCoords && m_bHasNormals)
			{
				sscanf(cLine, " %i//%i %i//%i %i//%i", &iTemp[0][0], &iTemp[2][0], 
					&iTemp[0][1], &iTemp[2][1], 
					&iTemp[0][2], &iTemp[2][2]);
			
				m_vFaces.push_back(&iTemp[0][0]);

			}
		
			else
			{
			
				sscanf(cLine, " %i ", &iTemp[0][0], &iTemp[1][0], &iTemp[2][0]);
			
				m_vFaces.push_back(&iTemp[0][0]);

			}			

		}
	
		else
		{
		
			fgets(cLine, 256, fp);
		}
	}

	m_pVerts = &m_vVertices[0];
	m_pTexCoords = &m_vTexCoords[0];
	m_pNormals = &m_vNormals[0];
	m_pFaces = &m_vFaces[0];
	m_pImage = new CImage();
	m_pImage->Load(fileimage);
	return true;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
void CObj::Render()
{
	static int iNumFaces = m_vFaces.size();

	printf("\nTriangles: %i", iNumFaces);


	glEnable(GL_TEXTURE_2D);


	if(m_pImage)
		m_pImage->Bind();

	glBegin(GL_TRIANGLES);


	if(m_bHasTexCoords && m_bHasNormals)
	{
		for(int i = 0; i < iNumFaces; i++)
		{
			//printf("\nTexture: %.20f    %.20f",m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[0])].m_fVec[0], m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[0])].m_fVec[1]  );
			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[0])].m_fVec);
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[0])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[0])].m_fVec);

			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[1])].m_fVec);
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[1])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[1])].m_fVec);

			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[2])].m_fVec);
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[2])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[2])].m_fVec);
		}
	}


	else if(!m_bHasTexCoords && m_bHasNormals)
	{
		for(int i = 0; i < iNumFaces; i++)
		{
			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[0])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[0])].m_fVec);

			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[1])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[1])].m_fVec);

			glNormal3fv(m_pNormals[(m_pFaces[i].m_uiNormalIdx[2])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[2])].m_fVec);
		}
	}


	else if(m_bHasTexCoords && !m_bHasNormals)
	{
		for(int i = 0; i < iNumFaces; i++)
		{
			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[0])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[0])].m_fVec);

			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[1])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[1])].m_fVec);

			glTexCoord2fv(m_pTexCoords[(m_pFaces[i].m_uiTexCoordIdx[2])].m_fVec);
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[2])].m_fVec);
		}
	}

	else
	{
		for(int i = 0; i < iNumFaces; i++)
		{
			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[0])].m_fVec);

			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[1])].m_fVec);

			glVertex3fv(m_pVerts[(m_pFaces[i].m_uiVertIdx[2])].m_fVec);
		}
	}

	glEnd();
}

//-------------------------------------------------------------

//-------------------------------------------------------------
void CObj::SetSkin(CImage& rSkin)
{
	m_pImage = &rSkin;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
CObj::CObj()
{
	m_pImage = 0;
	m_pVerts = 0;
	m_pTexCoords = 0;
	m_pNormals = 0;
	m_pFaces = 0;
	m_bHasTexCoords = false;
	m_bHasNormals = false;
}

CObj::CObj(const char * szFilename, char* fileimage)
{
	m_pImage = 0;
	m_pTexCoords = 0;
	m_pNormals = 0;
	m_pVerts = 0;
	m_pFaces = 0;
	m_bHasTexCoords = false;
	m_bHasNormals = false;
	Load(szFilename, fileimage);
}