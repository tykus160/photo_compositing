#ifndef __RGBAPIXEL_H__
#define __RGBAPIXEL_H__

class RGBAPixel
{
public:
    char r;
    char g;
    char b;
    char a;
    RGBAPixel(char red, char green, char blue) : RGBAPixel(red, green, blue, 255) {}
    RGBAPixel(char red, char green, char blue, char alpha) : r(red), g(green), b(blue), a(alpha) {}
};

#endif