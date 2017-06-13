#ifndef __PIXEL_H__
#define __PIXEL_H__

class Pixel
{
public:
    /** x coordinate of pixel */
    const int x;
    /** y coordinate of pixel */
    const int y;

    Pixel(int x, int y) :
        x(x), y(y)
    {
    }

    virtual unsigned char getIntensity() = 0;
};

#endif // !__PIXEL_H__
