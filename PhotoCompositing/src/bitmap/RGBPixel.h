#ifndef __RGBPIXEL_H__
#define __RGBPIXEL_H__

#include "../main.h"

class RGBPixel
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    friend bool operator!=(const RGBPixel &a, const RGBPixel &b)
    {
        return !(a == b);
    }

    friend bool operator==(const RGBPixel &a, const RGBPixel &b)
    {
        return a.r == b.r && a.g == b.g && a.b == b.b;
    }

    friend std::ostream &operator<<(std::ostream &output, const RGBPixel &D)
    {
        output << "RGBPixel[red: " << (int) D.r << ", green: " << (int) D.g << ", blue: " << (int) D.b << "]";
        return output;
    }
};

#endif
