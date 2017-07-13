#ifndef __GRAPH_CUT_OPTIMIZER_H__
#define __GRAPH_CUT_OPTIMIZER_H__

#include <vector>

#include "graph.h"
#include "../image/Image.h"
#include "Mask.h"
#include "CostFunctions.h"

class GraphCutOptimizer
{
private:
    static double MAX_WEIGHT;

    Graph::node_id** mNodes;
    unsigned int mCapacity;
    std::vector<Image*> mLabels;
    Mask* mMask = nullptr;
    Mask* outputMask = nullptr;
    bool optimizationComplete = false;
    CostFunction costFunction;

    /** Called at the beginning of optimize() */
    void init();

    double optimizationFirstPhase();

    RGBPixel* getOptimizedValue(int x, int y);

public:
    GraphCutOptimizer(unsigned int capacity, CostFunction function);

    void addLabel(Image* image);

    void addMask(Mask* mask);

    void optimize();

    void saveToImage(char* filename);
};

#endif // !__GRAPH_CUT_OPTIMIZER_H__
