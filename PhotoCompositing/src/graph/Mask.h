#ifndef __MASK_H__
#define __MASK_H__

#include <list>
#include <ostream>

#include "../bitmap/BMP.h"

class Mask
{
private:
    BMP* mBitmap;
    std::list<int> mRedFields;
    std::list<int> mGreenFields;

public:
    Mask(BMP* bitmap);
    void createLabels();
    int getLength();
    friend std::ostream& operator<<(std::ostream &output, const Mask &D);
};

#endif // !__MASK_H__
