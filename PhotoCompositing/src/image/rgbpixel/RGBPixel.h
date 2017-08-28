/**
 * Class representing pixel as 3-channel value (8 bits per every channel). Available channels: red, green, blue.
 * All channels are stored as public fields to simplify operations in one channel only.
 *
 * @author W.Tyczynski
 */
#ifndef __RGBPIXEL_H__
#define __RGBPIXEL_H__

#include <ostream>

class RGBPixel
{
private:
    static const double WEIGHT_R;
    static const double WEIGHT_G;
    static const double WEIGHT_B;

public:
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    RGBPixel();

    RGBPixel(const RGBPixel& other);

    unsigned char getIntensity();

    double distance(const RGBPixel& other);

    unsigned int toHex();

    friend bool operator!=(const RGBPixel& a, const RGBPixel& b);
    friend bool operator==(const RGBPixel& a, const RGBPixel& b);
    friend std::ostream& operator<<(std::ostream& output, const RGBPixel& D);
};

#endif
