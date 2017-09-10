#ifndef __IMAGE_OPERATIONS_H__
#define __IMAGE_OPERATIONS_H__

#include <vector>
#include "Image.h"

namespace ImageOperations
{
    int sobelX[];
    int sobelY[];

    /** performs horizontal and vertical filtering */
    Image* sobel(Image* input);

    Image* sobelH(Image* input);

    Image* sobelV(Image* input);

    Image* minimum(std::vector<Image*>& inputs);
}

#endif // !__IMAGE_OPERATIONS_H__
