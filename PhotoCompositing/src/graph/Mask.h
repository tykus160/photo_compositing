#ifndef __MASK_H__
#define __MASK_H__

#include <vector>

#include "../bitmap/BMP.h"

class Mask
{
private:
    static const int NO_LABEL;

    BMP* mBitmap;
    std::vector<int> mLabels;

public:
    Mask(BMP* bitmap);
    void createLabels();
    int getLength();
    int getLabelAtIndex(int index);
};

#endif // !__MASK_H__
