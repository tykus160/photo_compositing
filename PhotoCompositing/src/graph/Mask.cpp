#include "Mask.h"

Mask::Mask(BMP* bitmap) :
    mBitmap(bitmap)
{
}

void Mask::createLabels()
{
    for (int j = 0; j < mBitmap->getHeight(); ++j)
    {
        for (int i = 0; i < mBitmap->getWidth(); ++i)
        {
            RGBPixel* pixel = mBitmap->get(i, j);
            if (pixel->isRed())
            {
                mRedFields.push_back(mBitmap->getCoordinatesAsIndex(i, j));
            }
            else if (pixel->isGreen())
            {
                mGreenFields.push_back(mBitmap->getCoordinatesAsIndex(i, j));
            }
        }
    }
}

int Mask::getLength()
{
    return mBitmap->getWidth() * mBitmap->getHeight();
}

std::ostream& operator<<(std::ostream &output, const Mask &D)
{
    output << "Mask[numOfFields red: " << D.mRedFields.size() << ", green : " << D.mGreenFields.size() << "]";
    return output;
}
