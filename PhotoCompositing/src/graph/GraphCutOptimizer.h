#ifndef __GRAPH_CUT_OPTIMIZER_H__
#define __GRAPH_CUT_OPTIMIZER_H__

#include <vector>

#include "graph.h"
#include "../image/Image.h"
#include "Mask.h"

class GraphCutOptimizer
{
private:
    Graph::node_id** mNodes;
    bool** mNodesActive;
    unsigned int mCapacity;
    std::vector<Image*> mLabels;
    Mask* mMask = nullptr;
    bool optimizationComplete = false;

    /** Called at the beginning of optimize() */
    void init();

    RGBPixel* getOptimizedValue(int x, int y);

public:
    GraphCutOptimizer(unsigned int capacity);

    void addLabel(Image* image);

    void addMask(Mask* mask);

    void optimize();

    void saveToImage(char* filename);
};

#endif // !__GRAPH_CUT_OPTIMIZER_H__
