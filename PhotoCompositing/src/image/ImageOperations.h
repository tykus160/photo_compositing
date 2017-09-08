#ifndef __IMAGE_OPERATIONS_H__
#define __IMAGE_OPERATIONS_H__

#include "Image.h"

namespace ImageOperations
{
    int sobelX[];
    int sobelY[];

    /** performs horizontal and vertical filtering */
    Image* sobel(Image* input);

    Image* minimum(Image** inputs);
}

#endif // !__IMAGE_OPERATIONS_H__
