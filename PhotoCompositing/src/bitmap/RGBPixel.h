#ifndef __RGBPIXEL_H__
#define __RGBPIXEL_H__

#include <ostream>

class RGBPixel
{
public:
    char r;
    char g;
    char b;

    RGBPixel(char red, char green, char blue, char alpha) : r(red), g(green), b(blue) {}

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
        output << "RGBPixel[red: " << D.r << ", green: " << D.g << ", blue: " << D.b << "]";
        return output;
    }
};

#endif