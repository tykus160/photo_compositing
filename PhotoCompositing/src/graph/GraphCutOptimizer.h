#ifndef __GRAPH_CUT_OPTIMIZER_H__
#define __GRAPH_CUT_OPTIMIZER_H__

#include <string>
#include <vector>

#include "graph.h"
#include "../image/Image.h"
#include "Mask.h"

// TODO: describe methods
enum Method
{
    COLOR,
    GRADIENT,
    COLOR_2ND,
    GRADIENT_2ND,
    COLOR_GRADIENT_MIXED,
    UNDEFINED,
};

class GraphCutOptimizer
{
private:
    Graph::node_id* mNodes;
    unsigned int mNumberOfLabels;
    std::vector<Image*> mImages;
    std::vector<Image*> mImagesGradientsH;
    std::vector<Image*> mImagesGradientsV;
    Image* mMinGradientH = nullptr;
    Image* mMinGradientV = nullptr;
    Mask* mMask = nullptr;
    Mask* mMaskOrg = nullptr;
    bool optimizationComplete = false;

    Method method;

    /** Called at the beginning of optimize() */
    void init();

    RGBPixel* getOptimizedValue(int x, int y);

    double calculateEnergy(
        Graph& graph,
        int indexOfSource,
        int x1,
        int y1,
        bool horizontal);

    int calculateData(Graph& graph, int indexOfSource, int x, int y, int weight);

public:
    GraphCutOptimizer(unsigned int numberOfLabels, Method method = COLOR);

    ~GraphCutOptimizer();

    void addImage(Image* image);

    void addMask(Mask* mask);

    void optimize();

    Image* getImage();
};

#endif // !__GRAPH_CUT_OPTIMIZER_H__
