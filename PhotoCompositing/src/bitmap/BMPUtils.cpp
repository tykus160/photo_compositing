#include "BMPUtils.h"
#include <fstream>

unsigned char* BMPUtils::readBmpFromFile(char* szFileName, int& riWidth, int& riHeight)
{
    BMPUtils::BITMAPFILEHEADER bfh;
    BMPUtils::BITMAPINFOHEADER bih;

    char* buff = nullptr;

    char* p_palette = nullptr;
    unsigned short n_colors = 0;

    unsigned char* pRGBBuffer = nullptr;

    riWidth = -1;
    riHeight = -1;

    std::ifstream hfile(szFileName, std::ios::binary);

    if (hfile.is_open())
    {
        hfile.read((char*)&bfh, sizeof(bfh));
        if (!(bfh.bfType != BMPUtils::BF_TYPE_BM || (bfh.bfOffBits < (sizeof(BMPUtils::BITMAPFILEHEADER) + sizeof(BMPUtils::BITMAPINFOHEADER)))))
        {
            hfile.read((char*) &bih, sizeof(bih));
            riWidth = bih.biWidth;
            riHeight = bih.biHeight;
            int lev = bih.biBitCount;

            pRGBBuffer = new unsigned char[riWidth * riHeight * 3]; //Zaalokowanie odpowiedniego buffora obrazu

                                                                  //Za³aduj Palete barw jesli jest

            if ((lev == 1) || (lev == 4) || (lev == 8))
            {
                n_colors = 1 << lev;
                p_palette = new char[4 * n_colors];
                hfile.read(p_palette, 4 * n_colors);
            }

            hfile.seekg(bfh.bfOffBits);

            buff = new char[riWidth * 4];

            int ls;

            switch (lev)
            {
            case 1:
                //Nie obs³ugiwane
                break;
            case 4:
                //nie Obs³ugiwane
                break;
            case 8: //Skala szaroœci
                ls = (riWidth + 3) & 0xFFFFFFFC;
                for (int j = (riHeight - 1); j >= 0; j--)
                {
                    hfile.read(buff, ls);
                    for (int i = 0, l = 0; i < riWidth; i++)
                    {
                        pRGBBuffer[((j * riWidth) + i) * 3 + 2] = p_palette[(buff[i] << 2) + 2];//R
                        pRGBBuffer[((j * riWidth) + i) * 3 + 1] = p_palette[(buff[i] << 2) + 1];//G
                        pRGBBuffer[((j * riWidth) + i) * 3 + 0] = p_palette[(buff[i] << 2) + 0];//B
                    }
                };
                break;
            case 24:
                //bitmapa RGB
                ls = (riWidth * 3 + 3) & 0xFFFFFFFC;
                for (int j = (riHeight - 1); j >= 0; j--)
                {
                    hfile.read(buff, ls);
                    for (int i = 0, l = 0; i < riWidth; i++, l += 3)
                    {
                        pRGBBuffer[((j * riWidth) + i) * 3 + 0] = buff[l + 0];
                        pRGBBuffer[((j * riWidth) + i) * 3 + 1] = buff[l + 1];
                        pRGBBuffer[((j * riWidth) + i) * 3 + 2] = buff[l + 2];
                    };
                };
                break;
            case 32:
                // RGBA bitmap 
                for (int j = (riHeight - 1); j >= 0; j--)
                {
                    hfile.read(buff, riWidth * 4);
                    for (int i = 0, l = 0; i < riWidth; i++, l += 4)
                    {
                        pRGBBuffer[((j * riWidth) + i) * 3 + 0] = buff[l + 0];
                        pRGBBuffer[((j * riWidth) + i) * 3 + 1] = buff[l + 1];
                        pRGBBuffer[((j * riWidth) + i) * 3 + 2] = buff[l + 2];
                    }
                };
                break;
            };
            delete buff;
            if (p_palette != nullptr)
            {
                delete p_palette;
            }
        }
        hfile.close();
    }
    return pRGBBuffer;
}

void BMPUtils::writeBmpToFile(char* szFileName, unsigned char* pRGBSamples, int iWidth, int iHeight)
{
    BMPUtils::BITMAPFILEHEADER bfh;
    BMPUtils::BITMAPINFOHEADER bih;
    char* buff;
    unsigned int ls, fb = 0;//w

    if ((iWidth) % 4 != 0)
    {
        fb = 4 - (((iWidth) * 3) % 4);
    }

    std::ofstream hfile(szFileName, std::ios::binary);

    if (hfile.is_open() && pRGBSamples != nullptr)
    {
        bfh.bfType = BMPUtils::BF_TYPE_BM;
        bfh.bfSize = ((3 * (iWidth) +fb)*(iHeight)) + sizeof(bih);
        bfh.bfReserved1 = 0;
        bfh.bfReserved2 = 0;
        bfh.bfOffBits = sizeof(BMPUtils::BITMAPFILEHEADER) + sizeof(BMPUtils::BITMAPINFOHEADER);

        bih.biSize = sizeof(BMPUtils::BITMAPINFOHEADER);
        bih.biWidth = iWidth;
        bih.biHeight = iHeight;
        bih.biPlanes = 1;
        bih.biBitCount = 24;
        bih.biCompression = BMPUtils::BI_RGB;
        bih.biSizeImage = 0;
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;

        hfile.write((char*) &bfh, sizeof(bfh));
        hfile.write((char*) &bih, sizeof(bih));

        ls = ((iWidth) * 3 + 3) & 0xFFFFFFFC;
        buff = new char[ls];
        for (int j = (iHeight - 1); j >= 0; j--)
        {
            for (int i = 0, l = 0; i < iWidth; i++, l += 3)
            {
                buff[l + 0] = pRGBSamples[((j * iWidth) + i) * 3 + 0];
                buff[l + 1] = pRGBSamples[((j * iWidth) + i) * 3 + 1];
                buff[l + 2] = pRGBSamples[((j * iWidth) + i) * 3 + 2];
            }
            hfile.write(buff, ls);
        }
        delete buff;
        hfile.close();
    }
}