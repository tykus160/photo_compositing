#ifndef __GRAPH_CUT_OPTIMIZER_H__
#define __GRAPH_CUT_OPTIMIZER_H__

#include <string>
#include <vector>

#include "graph.h"
#include "../image/Image.h"
#include "Mask.h"
#include "CostFunctions.h"

enum Method
{
    DEFAULT, GRADIENT
};

class GraphCutOptimizer
{
private:
    Graph::node_id* mNodes;
    unsigned int mNumberOfLabels;
    std::vector<Image*> mImages;
    std::vector<Image*> mImagesGradients;
    Mask* mMask = nullptr;
    Mask* mMaskOrg = nullptr;
    bool optimizationComplete = false;
    CostFunction costFunction;

    Method method;

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

    void calculateData(Graph& graph, int indexOfSource, int x, int y);

    /** Helper random generator */
    int random();

public:
    GraphCutOptimizer(unsigned int numberOfLabels, CostFunction function, Method method = DEFAULT);

    ~GraphCutOptimizer();

    void addImage(Image* image);

    void addMask(Mask* mask);

    void optimize();

    void saveToImage(std::string filename);
};

#endif // !__GRAPH_CUT_OPTIMIZER_H__
