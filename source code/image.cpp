
//-------------------------------------------------------------
     
//-------------------------------------------------------------
#include <cstdio>

#include "image.h"

const char BMP_HEADER[] = "BM";
const char TGA_UHEADER[] = {0,0,2 ,0,0,0,0,0,0,0,0,0};
const char TGA_CHEADER[] = {0,0,10,0,0,0,0,0,0,0,0,0};

//-------------------------------------------------------------

//-------------------------------------------------------------
bool CImage::Load(char * szFilename, bool bMipMap)
{
	m_bMipMap = bMipMap;
	m_szFilename = szFilename;
	FILE * f;

	if(!(f = fopen(szFilename, "rb")))
	{
		printf("\nCould not open %s", szFilename);
		return false;
	}

	fseek(f, 0, SEEK_END);
	int iEnd = ftell(f);
	fseek(f, 0, SEEK_SET);
	int iStart = ftell(f);
	m_uiFileLen = iEnd - iStart;

	m_ucpBuffer = new unsigned char[m_uiFileLen];

	if(!m_ucpBuffer)
	{
		printf("\nCould not alloc memory for %s", szFilename);
		return false;
	}

	//get data
	if(fread(m_ucpBuffer, 1, m_uiFileLen, f) != m_uiFileLen)
	{
		printf("\nCould not read from %s", szFilename);
		if(m_ucpBuffer)
			delete [] m_ucpBuffer;
		return false;
	}

	if(memcmp(m_ucpBuffer, BMP_HEADER, 2) == 0)
	{
		return LoadBMP();
	}

	if(memcmp(m_ucpBuffer, TGA_UHEADER, 12) == 0)
	{
		
		return LoadTGA(false);
	}

	if(memcmp(m_ucpBuffer, TGA_CHEADER, 12) == 0)
	{
		return LoadTGA(true);
	}

	if(m_ucpBuffer[0] == 10)
	{
		return LoadPCX();
	}
	printf("\nCould not load %s, Invalid filetype", szFilename);
	return false;
}

//-------------------------------------------------------------


//-------------------------------------------------------------
bool CImage::LoadBMP()
{
	unsigned char * ucpPtr = m_ucpBuffer;
	unsigned char * ucpData = 0;


	struct SBMPFHeader
	{
		unsigned char m_ucType[2];	
		unsigned int m_uiSize;		
		unsigned int m_uiReserved;	
		unsigned int m_uiOffset;	
	};

	struct SBMPIHeader
	{
		unsigned int m_uiSize;		
		unsigned int m_uiWidth;	
		unsigned int m_uiHeight;		
		unsigned short m_uiPlanes;	
		unsigned short m_uiBpp;		
		unsigned int m_uiCompression;	
		unsigned int m_uiXPPM;			
		unsigned int m_uiYPPM;			
		unsigned int m_uiClrUsed;		
		unsigned int m_uiClrImp;		

	};

	SBMPFHeader * pFileHeader = (SBMPFHeader *)ucpPtr;
	ucpPtr += 14;	
	SBMPIHeader * pInfoHeader = (SBMPIHeader *)(ucpPtr);
	ucpPtr += 40;

 
	if(pInfoHeader->m_uiBpp != 24 || pInfoHeader->m_uiHeight == 0 || pInfoHeader->m_uiWidth == 0)
	{
		delete [] m_ucpBuffer;
		printf("\n%s is an invalid BMP file", m_szFilename);
		return false;
	}



	m_uiWidth = pInfoHeader->m_uiWidth;
	m_uiHeight = pInfoHeader->m_uiHeight;
	m_uiFileLen = pFileHeader->m_uiSize;


	ucpData = new unsigned char[m_uiHeight * m_uiWidth * 3];
	if(!ucpData)
	{
		printf("\nOut of memory");
		return false;
	}


	for(unsigned int i = 0; i < m_uiHeight * m_uiWidth * 3; i+=3)
	{
		ucpData[i] = ucpPtr[i+2];
		ucpData[i+1] = ucpPtr[i+1];
		ucpData[i+2] = ucpPtr[i];
	}

	VFlip(ucpData, m_uiWidth, m_uiHeight, 3);

	glGenTextures(1, &m_uiImage);
	glBindTexture(GL_TEXTURE_2D, m_uiImage);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);						

	if(m_bMipMap)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_uiWidth, m_uiHeight, GL_RGB, GL_UNSIGNED_BYTE, ucpData); 
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, m_uiWidth, m_uiHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucpData);
	}
	delete [] ucpData;
	delete [] m_ucpBuffer;

	printf("\nBMP Image: %s Loaded", m_szFilename);

	return true;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
bool CImage::LoadTGA(bool bCompressed)
{
	unsigned int uiBpp = 0;
	unsigned int uiType = 0;	
	unsigned int uiImageSize = 0;
	unsigned char * ucpPtr = m_ucpBuffer;
	unsigned char * ucpData = 0;

	ucpPtr += 12;

	m_uiWidth = (ucpPtr[1] << 8) + ucpPtr[0];
	m_uiHeight = (ucpPtr[3] << 8) + ucpPtr[2];
	uiBpp = ucpPtr[4];
	ucpPtr += 6;

	if(uiBpp == 24)
		uiType = GL_RGB;
	else if(uiBpp == 32)
		uiType = GL_RGBA;
	else
	{
		printf("\n%s is an invalid TGA file", m_szFilename);
		delete [] m_ucpBuffer;
		return false;
	}

	uiImageSize = m_uiHeight * m_uiWidth * (uiBpp / 8);
	ucpData = new unsigned char[uiImageSize];

	if(!ucpData)
	{
		printf("\nOut of memory");
		delete [] m_ucpBuffer;
		return false;
	}


	if(!bCompressed)
	{
		for(unsigned int i = 0; i < uiImageSize; i+= (uiBpp / 8))
		{
		
			ucpData[i] = ucpPtr[i+2];
			ucpData[i+1] = ucpPtr[i+1];
			ucpData[i+2] = ucpPtr[i];

			if(uiBpp == 32)
				ucpData[i+3] = ucpPtr[i+3];
		}
	}

	else
	{
		unsigned int uiDataPos = 0;

		do
		{

			if(ucpPtr[0] < 128)
			{
				unsigned int uiCount = ucpPtr[0] * (uiBpp / 8) + 1;
				ucpPtr++;
				for(unsigned int i = 0; i < uiCount; i+=(uiBpp/8))
				{
					ucpData[uiDataPos] = ucpPtr[i+2];
					ucpData[uiDataPos+1] = ucpPtr[i+1];
					ucpData[uiDataPos+2] = ucpPtr[i];
					ucpPtr += 3;
					uiDataPos += 3;

					if(uiBpp == 32)
					{
						ucpData[uiDataPos] = ucpPtr[i];
						ucpPtr ++;
						uiDataPos ++;
					}
				}
			}
			else
			{
				unsigned int uiCount = (ucpPtr[0] -= 127) * (uiBpp / 8);
				ucpPtr++;
				for(unsigned int i = 0; i < uiCount; i+=(uiBpp/8))
				{
					ucpData[uiDataPos] = ucpPtr[2];
					ucpData[uiDataPos+1] = ucpPtr[1];
					ucpData[uiDataPos+2] = ucpPtr[0];
					uiDataPos += 3;

					if(uiBpp == 32)
					{
						ucpData[uiDataPos] = ucpPtr[3];
						uiDataPos ++;
					}
				}
				ucpPtr += 3;
				if(uiBpp == 32)
					ucpPtr++;
			}
		}
		while(uiDataPos < uiImageSize);

	}


	glGenTextures(1, &m_uiImage);
	glBindTexture(GL_TEXTURE_2D, m_uiImage);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);						


	if(m_bMipMap)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, uiType, m_uiWidth, m_uiHeight, uiType, GL_UNSIGNED_BYTE, ucpData); 
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, uiType, m_uiWidth, m_uiHeight, 0, uiType, GL_UNSIGNED_BYTE, ucpData);
	}


	delete [] ucpData;
	delete [] m_ucpBuffer;
	printf("\nTGA Image: %s Loaded", m_szFilename);
	return true;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
bool CImage::LoadPCX()
{

	struct SPCXPal
	{	
		unsigned char m_ucColor[3];
	};

	SPCXPal Palette[256];
	unsigned char * ucpPtr = m_ucpBuffer;
	unsigned char * ucpPixData = 0;
	unsigned char * ucpData = 0;

	if(ucpPtr[0] != 10 || ucpPtr[1] != 5 || ucpPtr[2] != 1 || ucpPtr[3] != 8)
	{
		printf("\n%s is not a valid PCX texture", m_szFilename);
		delete [] m_ucpBuffer;
	}
	ucpPtr += 4;


	short sXMin = (ucpPtr[1] << 8) + ucpPtr[0];
	short sYMin = (ucpPtr[3] << 8) + ucpPtr[2];
	short sXMax = (ucpPtr[5] << 8) + ucpPtr[4];
	short sYMax = (ucpPtr[7] << 8) + ucpPtr[6];

	m_uiWidth = sXMax - sXMin + 1;
	m_uiHeight = sYMax - sYMin + 1;

	
	ucpPtr += 124;

	ucpPixData = new unsigned char[m_uiWidth * m_uiHeight];


	unsigned int uiIdx = 0;
	unsigned int uiPixel = 0;
	while(uiPixel < m_uiWidth * m_uiHeight)
	{
		if(ucpPtr[uiIdx] > 191)
		{
			uiIdx++;
			for(int i = 0; i < (63 & ucpPtr[uiIdx-1]); i++)
			{
				ucpPixData[uiPixel] = ucpPtr[uiIdx];
				uiPixel++;
			}
			uiIdx++;
		}
		else
		{
			ucpPixData[uiPixel] = ucpPtr[uiIdx];
			uiIdx++;
			uiPixel++;
		}
	}


	ucpPtr = m_ucpBuffer + m_uiFileLen - 769;

	if(ucpPtr[0] != 12)
	{
		printf("\nBad palette in %s", m_szFilename);
		delete [] ucpPixData;
		return false;
	}
	ucpPtr++;

	memcpy(Palette, ucpPtr, sizeof(SPCXPal[256]));


	ucpData = new unsigned char[m_uiWidth * m_uiHeight * 3];

	uiIdx = 0;
	for(unsigned int i = 0; i < m_uiWidth * m_uiHeight; i++)
	{
		ucpData[uiIdx  ] = Palette[ucpPixData[i]].m_ucColor[0];
		ucpData[uiIdx+1] = Palette[ucpPixData[i]].m_ucColor[1];
		ucpData[uiIdx+2] = Palette[ucpPixData[i]].m_ucColor[2];
		uiIdx+=3;
	}

	m_uiImage = 0;

	glGenTextures(1, &m_uiImage);
	glBindTexture(GL_TEXTURE_2D, m_uiImage);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);						

	if(m_bMipMap)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_uiWidth, m_uiHeight, GL_RGB, GL_UNSIGNED_BYTE, ucpData); 
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, m_uiWidth, m_uiHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucpData);
	}

	delete [] m_ucpBuffer;
	delete [] ucpPixData;
	delete [] ucpData;	

	printf("\nPCX Image: %s Loaded", m_szFilename);

	return true;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
void CImage::VFlip(unsigned char * ucpData, unsigned int uiHeight, unsigned int uiWidth, unsigned int uiBpp)
{
	unsigned char * ucpCopy = new unsigned char[uiWidth * uiHeight * uiBpp];
	if(!ucpCopy)
		return;
	for(unsigned int i = 0; i < uiHeight; i++)
		memcpy(ucpCopy + (uiWidth * uiBpp * i), ucpData + (uiWidth * uiBpp * (uiHeight - i)), uiWidth * uiBpp);
	memcpy(ucpData, ucpCopy, uiWidth * uiHeight * uiBpp);
	delete [] ucpCopy;
}

//-------------------------------------------------------------

//-------------------------------------------------------------
void CImage::ScreenShot(char * szDstFile)
{

}












