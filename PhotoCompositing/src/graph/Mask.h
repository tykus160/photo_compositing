#ifndef __MASK_H__
#define __MASK_H__

#include <vector>

#include "../image/Image.h"

class Mask
{
private:
    Image* mImage = nullptr;
    int mWidth;
    int mHeight;
    std::vector<int> mLabels;

public:
    static const int NO_LABEL;

    Mask(Image* image);

    Mask(int width, int height);

    ~Mask();

    void createLabels();

    int getLength();

    int getWidth();

    int getHeight();

    int getLabelAtIndex(int index);

    int getLabelAtCoordinate(int x, int y);

    void setLabelAtCoordinate(int x, int y, int label);

    void saveToImage(std::string filename);
};

#endif // !__MASK_H__
