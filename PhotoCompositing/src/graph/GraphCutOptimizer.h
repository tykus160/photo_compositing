#ifndef __GRAPH_CUT_OPTIMIZER_H__
#define __GRAPH_CUT_OPTIMIZER_H__

#include <string>
#include <vector>

#include "graph.h"
#include "../image/Image.h"
#include "Mask.h"
#include "CostFunctions.h"

class GraphCutOptimizer
{
private:
    Graph::node_id* mNodes;
    unsigned int mNumberOfLabels;
    std::vector<Image*> mImages;
    Mask* mMask = nullptr;
    bool optimizationComplete = false;
    CostFunction costFunction;

    /** Called at the beginning of optimize() */
    void init();

    RGBPixel* getOptimizedValue(int x, int y);

    double calculateEnergy(
        Graph& graph,
        int indexOfSource,
        int x1,
        int y1,
        int x2,
        int y2);

    /** Helper random generator */
    int random();

public:
    GraphCutOptimizer(unsigned int numberOfLabels, CostFunction function);

    ~GraphCutOptimizer();

    void addImage(Image* image);

    void addMask(Mask* mask);

    void optimize();

    void saveToImage(std::string filename);
};

#endif // !__GRAPH_CUT_OPTIMIZER_H__
