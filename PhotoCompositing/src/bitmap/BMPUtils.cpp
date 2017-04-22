#include "BMPUtils.h"
#include <fstream>

unsigned char* BMPUtils::ReadBmpFromFile(char* szFileName, int& riWidth, int& riHeight)
{
    BMPUtils::BITMAPFILEHEADER bfh;
    BMPUtils::BITMAPINFOHEADER bih;

    int i, j, h, v, lev, l, ls;
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
        if (!(bfh.bfType != BMPUtils::BF_TYPE_MB || (bfh.bfOffBits < (sizeof(BMPUtils::BITMAPFILEHEADER) + sizeof(BMPUtils::BITMAPINFOHEADER)))))
        {

            hfile.read((char*) &bih, sizeof(bih));
            v = bih.biWidth;
            h = bih.biHeight;
            lev = bih.biBitCount;

            riWidth = v;
            riHeight = h;
            pRGBBuffer = new unsigned char[riWidth * riHeight * 3]; //Zaalokowanie odpowiedniego buffora obrazu

                                                                  //Za³aduj Palete barw jesli jest

            if ((lev == 1) || (lev == 4) || (lev == 8))
            {
                n_colors = 1 << lev;
                p_palette = new char[4 * n_colors];
                hfile.read(p_palette, 4 * n_colors);
            }

            hfile.seekg(bfh.bfOffBits);

            buff = new char[v * 4];

            switch (lev)
            {
            case 1:
                //Nie obs³ugiwane
                break;
            case 4:
                //nie Obs³ugiwane
                break;
            case 8: //Skala szaroœci
                ls = (v + 3) & 0xFFFFFFFC;
                for (j = (h - 1); j >= 0; j--)
                {
                    hfile.read(buff, ls);
                    for (i = 0, l = 0; i<v; i++)
                    {
                        pRGBBuffer[((j * riWidth) + i) * 3 + 2] = p_palette[(buff[i] << 2) + 2];//R
                        pRGBBuffer[((j * riWidth) + i) * 3 + 1] = p_palette[(buff[i] << 2) + 1];//G
                        pRGBBuffer[((j * riWidth) + i) * 3 + 0] = p_palette[(buff[i] << 2) + 0];//B
                    }
                };
                break;
            case 24:
                //bitmapa RGB
                ls = (v * 3 + 3) & 0xFFFFFFFC;
                for (j = (h - 1); j >= 0; j--)
                {
                    hfile.read(buff, ls);
                    for (i = 0, l = 0; i<v; i++, l += 3)
                    {
                        pRGBBuffer[((j * riWidth) + i) * 3 + 0] = buff[l + 0];
                        pRGBBuffer[((j * riWidth) + i) * 3 + 1] = buff[l + 1];
                        pRGBBuffer[((j * riWidth) + i) * 3 + 2] = buff[l + 2];
                    };
                };
                break;
            case 32:
                // RGBA bitmap 
                for (j = (h - 1); j >= 0; j--)
                {
                    hfile.read(buff, v * 4);
                    for (i = 0, l = 0; i<v; i++, l += 4)
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
