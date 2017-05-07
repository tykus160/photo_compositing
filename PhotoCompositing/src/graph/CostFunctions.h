#ifndef __COST_FUNCTIONS_H__
#define __COST_FUNCTIONS_H__

#include "../main.h"

namespace CostFunctions
{
    int crossCostLinear(int a, int b)
    {
        return std::abs(a - b);
    }

    double secondOrderCostLinear(int a, int b, int c)
    {
        return 0.2 * std::abs(2 * b - a - c);
    }
}

#endif // !__COST_FUNCTIONS_H__
