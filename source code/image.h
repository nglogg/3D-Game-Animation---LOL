
#ifndef IMAGE_H
#define IMAGE_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


class CImage
{
public:

	bool Load(char * szFilename, bool bMipmap = true);

	void Bind();

	void UnBind();

	void VFlip(unsigned char * ucpData, unsigned int uiHeight, unsigned int uiWidth, unsigned int uiBpp); //Vertically


	static void ScreenShot(char * szDstFile = "screenshot.tga");


	CImage()
	{
		m_szFilename = 0;
		m_ucpBuffer = 0;
	}
	CImage(char * szFile, bool bMipMap = true)
	{
		m_ucpBuffer = 0;
		m_szFilename = 0;
		Load(szFile, bMipMap);
	}
	~CImage()
	{
	}

private:



	bool LoadTGA(bool bCompressed);
	bool LoadBMP();
	bool LoadPCX();


	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	unsigned int m_uiFileLen; 

	unsigned int m_uiImage;

	unsigned char * m_ucpBuffer;

	bool m_bMipMap;

	char * m_szFilename;
};

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CImage::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_uiImage);
}

//-------------------------------------------------------------

//-------------------------------------------------------------
inline void CImage::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

#endif //IMAGE_H
