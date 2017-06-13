#ifndef __RGBPIXEL_H__
#define __RGBPIXEL_H__

#include <ostream>
#include "../Pixel.h"

class RGBPixel : public Pixel
{
private:
    static const double WEIGHT_R;
    static const double WEIGHT_G;
    static const double WEIGHT_B;

public:
    static const unsigned char MAX_VALUE;

    unsigned char r;
    unsigned char g;
    unsigned char b;

    RGBPixel(int x, int y);

    unsigned char getIntensity();

    bool isRed()
    {
        return r == MAX_VALUE && g == 0 && b == 0;
    }

    bool isGreen()
    {
        return r == 0 && g == MAX_VALUE && b == 0;
    }

    bool isBlue()
    {
        return r == 0 && g == 0 && b == MAX_VALUE;
    }

    friend bool operator!=(const RGBPixel &a, const RGBPixel &b);
    friend bool operator==(const RGBPixel &a, const RGBPixel &b);
    friend std::ostream& operator<<(std::ostream &output, const RGBPixel &D);
};

#endif
