#ifndef __GRAPH_CUT_OPTIMIZER_H__
#define __GRAPH_CUT_OPTIMIZER_H__

#include "graph.h"
#include "../bitmap/BMP.h"
#include "Mask.h"

class GraphCutOptimizer
{
private:
    Graph::node_id** mNodes;
    bool** mNodesActive;
    unsigned int mCapacity;
    unsigned int mActualLength = 0;
    BMP** mLabels;
    Mask* mMask = nullptr;

    /** Called at the beginning of optimize() */
    void init();
    
public:
    GraphCutOptimizer(unsigned int capacity);
    void addLabel(BMP* bitmap);
    void addMask(Mask* mask);
    void optimize();
};

#endif // !__GRAPH_CUT_OPTIMIZER_H__
