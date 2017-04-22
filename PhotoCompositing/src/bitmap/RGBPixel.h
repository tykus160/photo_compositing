#ifndef __RGBPIXEL_H__
#define __RGBPIXEL_H__

class RGBPixel
{
public:
    char r;
    char g;
    char b;

    RGBPixel(char red, char green, char blue, char alpha) : r(red), g(green), b(blue) {}

    friend bool operator!=(const RGBPixel &a, const RGBPixel &b);
    friend bool operator==(const RGBPixel &a, const RGBPixel &b);
};

#endif