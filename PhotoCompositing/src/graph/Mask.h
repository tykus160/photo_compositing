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
    int mLabelCounter = 0;
    std::vector<int> mLabels;

public:
    static const int NO_LABEL;

    Mask(Image* image);

    Mask(int width, int height);

    Mask(const Mask& other);

    ~Mask();

    void createLabels();

    int getLength();

    int getWidth();

    int getHeight();

    int getLabelAtIndex(int index);

    int getLabelAtCoordinate(int x, int y);

    void setLabelAtCoordinate(int x, int y, int label);

    void saveToImage(std::string filename);

    /** replaces every NO_LABEL with random data */
    void fill();

    int getNumberOfLabels();
};

#endif // !__MASK_H__
