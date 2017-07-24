#ifndef __MASK_H__
#define __MASK_H__

#include <vector>

#include "../image/Image.h"

class Mask
{
private:
    static const int NO_LABEL;

    Image* mImage = nullptr;
    int mWidth;
    int mHeight;
    std::vector<int> mLabels;

public:
    Mask(Image* image);

    Mask(int width, int height);

    void createLabels();

    int getLength();

    int getWidth();

    int getHeight();

    int getLabelAtIndex(int index);

    int getLabelAtCoordinate(int x, int y);

    void setLabelAtCoordinate(int x, int y, int label);

    void saveToImage(char* filename);
};

#endif // !__MASK_H__
