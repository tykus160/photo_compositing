#ifndef __MASK_H__
#define __MASK_H__

#include <list>
#include "../bitmap/BMP.h"

class Mask
{
private:
    BMP* mBitmap;
    std::list<RGBPixel*> mFields;

public:
    Mask(BMP* bitmap);
    void createLabels();
};

#endif // !__MASK_H__
