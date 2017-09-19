/*
* Self-automatic image compositing using Graph Cut optimization.
* Program input:
* - 1st arg - path to prepared mask
* - next args - paths to images which would take participance in optimization
*
* @author: W.Tyczynski
*/
#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>

#include "utils/Properties.h"
#include "ErrorCodes.h"
#include "image/bitmap/BMP.h"
#include "graph/Mask.h"
#include "graph/GraphCutOptimizer.h"

namespace Utils
{
    const int MIN_NUM_OF_ARGS = 2;

    void endMessage();

    Method selectMethod();
}

#endif // !__MAIN_H__
