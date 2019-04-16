#ifndef __CFcsBitmap_h_
#define __CFcsBitmap_h_

#pragma pack(push, Enter__CFcsBitmap_h, 1)

struct CFcsBitmapFileHeader
{
   unsigned char Identifier[2]; // Characters Identifying the bitmap 'BM' for windows 9x, NT
   unsigned long int iFileSize; //Complete file size in bytes
   unsigned long int iReserved; //Reserved for later use
   unsigned long int iOffset; //Offset from beginning of file to beginning of bitmap data

};

struct CFcsBitmapInfoHeader
{
   unsigned long int iInfoLength; // Length of the Bitmap info header 0x28 for windows 9x, NT
   unsigned long int iWidth; // Width of Bitmap in pixels
   unsigned long int iHeight; // Height of Bitmap in pixels
   unsigned short int iNrPlanes; // Number of planes in this bitmap
   unsigned short int iBitsPerPixel; // Nr of bits per pixel 1, 4, 8, 16, 24 or 32
   unsigned long int iCompression; // Compresssion sepcifications 
                                  // 0-None also BI_RGB
                                  // 1-RLE 4 bit also BI_RLE4
                                  // 1-RLE 8 bit also BI_RLE8
                                  // 4-Bitfields also BI_BITFIELDS
   unsigned long int iDataSize; //Size of the bitmap data in bytes, rounded to 4 byte boundary
   unsigned long int iHorRes; // Horizontal resolution in pixels/meter
   unsigned long int iVerRes; // Vertical resolution in pixels/meter
   unsigned long int iNrColors; // Number of important colors

};

#define NR_BYTES_PER_PIXEL 4

class CFcsBitmap
{
public:
   CFcsBitmap();
   CFcsBitmap(char *pBitmapFilePath, bool Load, unsigned char Alpha);
   ~CFcsBitmap();
   
   void Load(unsigned char Alpha);
   void SetAlpha(unsigned char Alpha);

public:   
   CFcsBitmapFileHeader m_tFileHeader;
   CFcsBitmapInfoHeader m_tInfoHeader;

   char m_sFileName[128];
   
   unsigned char *m_pData;



};


#pragma pack(pop, Enter__CFcsBitmap_h)

#endif //__CFcsBitmap_h_