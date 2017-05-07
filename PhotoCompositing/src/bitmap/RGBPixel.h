#ifndef __RGBPIXEL_H__
#define __RGBPIXEL_H__

#include "../main.h"

class RGBPixel
{
private:
    static const double WEIGHT_R;
    static const double WEIGHT_G;
    static const double WEIGHT_B;

public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    unsigned char getIntensity();

    friend bool operator!=(const RGBPixel &a, const RGBPixel &b);
    friend bool operator==(const RGBPixel &a, const RGBPixel &b);
    friend std::ostream& operator<<(std::ostream &output, const RGBPixel &D);
};

#endif
