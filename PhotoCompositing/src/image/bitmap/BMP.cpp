#include "BMP.h"

#include <fstream>
#include <stdexcept>

#include "BMPUtils.h"

BMP::BMP() :
    Image()
{
}

BMP::BMP(int width, int height) :
    Image(width, height)
{
}

BMP::BMP(std::string filename) :
    BMP(filename.c_str())
{
}

BMP::BMP(const char* filename) :
    Image()
{
    std::ifstream hfile(filename, std::ios::binary);

    if (hfile.is_open())
    {
        BMPUtils::BITMAPFILEHEADER bfh;
        hfile.read((char*) &bfh, sizeof(bfh));
        if (!(bfh.bfType != BMPUtils::BF_TYPE_BM ||
            (bfh.bfOffBits < sizeof(BMPUtils::BITMAPFILEHEADER) + sizeof(BMPUtils::BITMAPINFOHEADER))))
        {
            BMPUtils::BITMAPINFOHEADER bih;

            char* buff = nullptr;

            char* p_palette = nullptr;
            unsigned short n_colors = 0;

            hfile.read((char*) &bih, sizeof(bih));
            width = bih.biWidth;
            height = bih.biHeight;
            int lev = bih.biBitCount;

            pixels = new RGBPixel*[width * height];

            //Za³aduj Palete barw jesli jest
            if (lev == 1 || lev == 4 || lev == 8)
            {
                n_colors = 1 << lev;
                p_palette = new char[4 * n_colors];
                hfile.read(p_palette, 4 * n_colors);
            }

            hfile.seekg(bfh.bfOffBits);

            buff = new char[width * 4];

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
                ls = (width + 3) & 0xFFFFFFFC;
                for (int j = height - 1; j >= 0; j--)
                {
                    hfile.read(buff, ls);
                    for (int i = 0; i < width; i++)
                    {
                        const int index = j * width + i;
                        pixels[index] = new RGBPixel;
                        pixels[index]->r = p_palette[(buff[i] << 2) + 2];//R
                        pixels[index]->g = p_palette[(buff[i] << 2) + 1];//G
                        pixels[index]->b = p_palette[(buff[i] << 2) + 0];//B
                    }
                };
                break;
            case 24:
                //bitmapa RGB
                ls = (width * 3 + 3) & 0xFFFFFFFC;
                for (int j = height - 1; j >= 0; j--)
                {
                    hfile.read(buff, ls);
                    for (int i = 0, l = 0; i < width; i++, l += 3)
                    {
                        const int index = j * width + i;
                        pixels[index] = new RGBPixel;
                        pixels[index]->b = buff[l + 0];
                        pixels[index]->g = buff[l + 1];
                        pixels[index]->r = buff[l + 2];
                    };
                };
                break;
            case 32:
                // RGBA bitmap 
                for (int j = height - 1; j >= 0; j--)
                {
                    hfile.read(buff, width * 4);
                    for (int i = 0, l = 0; i < width; i++, l += 4)
                    {
                        const int index = j * width + i;
                        pixels[index] = new RGBPixel;
                        pixels[index]->b = buff[l + 0];
                        pixels[index]->g = buff[l + 1];
                        pixels[index]->r = buff[l + 2];
                    }
                };
                break;
            };

            delete buff;
            delete p_palette;
        }
        else
        {
            throw std::runtime_error("file is not supported bmp");
        }
        hfile.close();
    }
    else
    {
        throw std::runtime_error("file is not supported bmp or is being interpreted by another program");
    }
}

void BMP::saveToFile(const char* filename)
{
    std::ofstream hfile(filename, std::ios::binary);

    if (hfile.is_open() && pixels != nullptr)
    {
        BMPUtils::BITMAPFILEHEADER bfh;
        BMPUtils::BITMAPINFOHEADER bih;
        char* buff;
        unsigned int ls, fb = 0;

        if (width % 4 != 0)
        {
            fb = 4 - ((width * 3) % 4);
        }

        bfh.bfType = BMPUtils::BF_TYPE_BM;
        bfh.bfSize = (3 * width + fb) * height + sizeof(bih);
        bfh.bfReserved1 = 0;
        bfh.bfReserved2 = 0;
        bfh.bfOffBits = sizeof(BMPUtils::BITMAPFILEHEADER) + sizeof(BMPUtils::BITMAPINFOHEADER);

        bih.biSize = sizeof(BMPUtils::BITMAPINFOHEADER);
        bih.biWidth = width;
        bih.biHeight = height;
        bih.biPlanes = 1;
        bih.biBitCount = 24;
        bih.biCompression = 0;
        bih.biSizeImage = 0;
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;

        hfile.write((char*) &bfh, sizeof(bfh));
        hfile.write((char*) &bih, sizeof(bih));

        ls = (width * 3 + 3) & 0xFFFFFFFC;
        buff = new char[ls];
        for (int j = (height - 1); j >= 0; j--)
        {
            for (int i = 0, l = 0; i < width; i++, l += 3)
            {
                const int index = j * width + i;
                buff[l + 0] = pixels[index]->b;
                buff[l + 1] = pixels[index]->g;
                buff[l + 2] = pixels[index]->r;
            }
            hfile.write(buff, ls);
        }
        delete buff;
        hfile.close();
    }
    else
    {
        throw std::runtime_error("cannot save bitmap to file");
    }
}
