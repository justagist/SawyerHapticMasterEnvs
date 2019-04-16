#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <glut.h>

#include "FcsGLBitmap.h"


CFcsGLBitmap::CFcsGLBitmap()
{
   m_pData = NULL;
}

CFcsGLBitmap::CFcsGLBitmap(char *pBitmapFilePath, bool bLoad=false)
{
   strcpy(m_sFileName, pBitmapFilePath);
   m_pData = NULL;
   m_iAlpha = 48;
   if (bLoad == true)
      this->Load();
}

CFcsGLBitmap::CFcsGLBitmap(char *pBitmapFilePath, bool bLoad=false, int iNum=0)
{
   strcpy(m_sFileName, pBitmapFilePath);
   m_pData = NULL;
   m_iAlpha = 48;
   if (bLoad == true) {
      this->Load();   
	  this->BindTexture(iNum);
   }
}

CFcsGLBitmap::~CFcsGLBitmap()
{
   if (m_pData != NULL)
      free(m_pData);
}

void CFcsGLBitmap::Load()
{
   FILE *pFile;
   unsigned int x, y, Width, Height, NrPixels, RedIndex, GreenIndex, BlueIndex, AlphaIndex, Skip;
   unsigned char Red, Green, Blue, SkipVal;

   if (strcmp(m_sFileName, "") != 0)
      pFile = fopen(m_sFileName, "rb");
   else
      pFile = NULL;

   if (pFile != NULL)
   {
      fread((void*)&m_tFileHeader, sizeof(m_tFileHeader), 1, pFile);      
      fread((void*)&m_tInfoHeader, sizeof(m_tInfoHeader), 1, pFile);

      NrPixels = m_tInfoHeader.iWidth*m_tInfoHeader.iHeight;
      m_pData = (unsigned char *)calloc(NrPixels*NR_BYTES_PER_PIXEL, 1);
      fseek(pFile, m_tFileHeader.iOffset, SEEK_SET);

      Width = m_tInfoHeader.iWidth;
      Height = m_tInfoHeader.iHeight;
      
      for (y=0; y<Height; y++)
      {
         for(x=0; x<Width; x++)
         {
            RedIndex = ((Height-1-y)*Width*NR_BYTES_PER_PIXEL) + (x*NR_BYTES_PER_PIXEL) + 0;
            GreenIndex = ((Height-1-y)*Width*NR_BYTES_PER_PIXEL) + (x*NR_BYTES_PER_PIXEL) + 1;
            BlueIndex = ((Height-1-y)*Width*NR_BYTES_PER_PIXEL) + (x*NR_BYTES_PER_PIXEL) + 2;
            if (NR_BYTES_PER_PIXEL > 3)
               AlphaIndex = ((Height-1-y)*Width*NR_BYTES_PER_PIXEL) + (x*NR_BYTES_PER_PIXEL) + 3;

            fread(&Blue, 1, 1, pFile);	// Read Blue value
            fread(&Green, 1, 1, pFile); // Read Green value
            fread(&Red, 1, 1, pFile);	// Read Red value
            
            m_pData[BlueIndex] = Blue;
            m_pData[GreenIndex] = Green;
            m_pData[RedIndex] = Red;
            if (NR_BYTES_PER_PIXEL > 3)
               m_pData[AlphaIndex] = m_iAlpha; //Generate Alpha Value
         }

         if (((Width*NR_BYTES_PER_PIXEL) % 4) != 0)
            for(Skip = 0; Skip < 4-((Width*NR_BYTES_PER_PIXEL) % 4); Skip++)
               fread(&SkipVal, 1, 1, pFile);
      }

      fclose(pFile);
   }
}

void CFcsGLBitmap::SetAlpha(unsigned char Alpha)
{
    unsigned int i;

    if (NR_BYTES_PER_PIXEL > 3)
    {
        for(i=3; i<sizeof(*m_pData); i+=4)
            m_pData[i] = Alpha;

    }

}

void CFcsGLBitmap::BindTexture(int m_iNum)
{	
	glEnable(GL_TEXTURE_2D);		
	glBindTexture(GL_TEXTURE_2D, m_iNum);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_tInfoHeader.iWidth, m_tInfoHeader.iHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
	glDisable(GL_TEXTURE_2D);
}
