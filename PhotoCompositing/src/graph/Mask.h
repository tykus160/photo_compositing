#ifndef __MASK_H__
#define __MASK_H__

#include <vector>

#include "../image/Image.h"

class Mask
{
private:
    static const int NO_LABEL;

    Image* mImage;
    std::vector<int> mLabels;

public:
    Mask(Image* image);

    void createLabels();

    int getLength();

    int getLabelAtIndex(int index);
};

#endif // !__MASK_H__
