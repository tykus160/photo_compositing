#ifndef __BMP_UTILS_H__
#define __BMP_UTILS_H__

namespace BMPUtils
{
    const unsigned short BF_TYPE_BM = 0x4D42;

    enum Compression
    {
        BI_RGB = 0L
    };

#pragma pack(2) 

    typedef struct                       /**** BMP file header structure ****/
    {
        unsigned short bfType;           /* Magic number for file */
        unsigned int   bfSize;           /* Size of file */
        unsigned short bfReserved1;      /* Reserved */
        unsigned short bfReserved2;      /* ... */
        unsigned int   bfOffBits;        /* Offset to bitmap data */
    } BITMAPFILEHEADER;

#pragma pack()

    typedef struct                       /**** BMP file info structure ****/
    {
        unsigned int   biSize;           /* Size of info header */
        int            biWidth;          /* Width of image */
        int            biHeight;         /* Height of image */
        unsigned short biPlanes;         /* Number of color planes */
        unsigned short biBitCount;       /* Number of bits per pixel */
        unsigned int   biCompression;    /* Type of compression to use */
        unsigned int   biSizeImage;      /* Size of image data */
        int            biXPelsPerMeter;  /* X pixels per meter */
        int            biYPelsPerMeter;  /* Y pixels per meter */
        unsigned int   biClrUsed;        /* Number of colors used */
        unsigned int   biClrImportant;   /* Number of important colors */
    } BITMAPINFOHEADER;

    unsigned char* readBmpFromFile(char* szFileName, int& riWidth, int& riHeight);

    void writeBmpToFile(char* szFileName, unsigned char* pRGBSamples, int iWidth, int iHeight);
}

#endif // !__BMP_UTILS_H__