#include "RGBPixel.h"

// From CCIR 601
const double RGBPixel::WEIGHT_R = 0.2989;
const double RGBPixel::WEIGHT_G = 0.5870;
const double RGBPixel::WEIGHT_B = 0.1140;

const unsigned char RGBPixel::MAX_VALUE = 255;

unsigned char RGBPixel::getIntensity()
{
    return (unsigned char) (RGBPixel::WEIGHT_R * r + RGBPixel::WEIGHT_G * g + RGBPixel::WEIGHT_B * b);
}

bool operator!=(const RGBPixel &a, const RGBPixel &b)
{
    return !(a == b);
}

bool operator==(const RGBPixel &a, const RGBPixel &b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

std::ostream& operator<<(std::ostream &output, const RGBPixel &D)
{
    output << "RGBPixel[red: " << (int) D.r << ", green: " << (int) D.g << ", blue: " << (int) D.b << "]";
    return output;
}
