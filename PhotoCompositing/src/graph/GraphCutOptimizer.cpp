#include "GraphCutOptimizer.h"

#include <stdexcept>

#ifdef DEBUG_TIME
#include <chrono>
#include <iostream>
#endif // DEBUG_TIME

GraphCutOptimizer::GraphCutOptimizer(unsigned int capacity)
{
    if (capacity == 0)
    {
        throw std::invalid_argument("capacity must be greater than 0");
    }
    mCapacity = capacity;
    mLabels = new BMP*[mCapacity];
}

void GraphCutOptimizer::addLabel(BMP* bitmap)
{
    if (mActualLength < mCapacity)
    {
        mLabels[mActualLength++] = bitmap;
    }
}

void GraphCutOptimizer::addMask(Mask* mask)
{
    mMask = mask;
}

void GraphCutOptimizer::optimize()
{
#ifdef DEBUG_TIME
    auto t1 = std::chrono::high_resolution_clock::now();
#endif // DEBUG_TIME
        
    if (mActualLength == 0)
    {
        throw std::logic_error("Cannot optimize without any label");
    }
    if (mMask == nullptr)
    {
        throw std::logic_error("Cannot optimize without mask");
    }

    mNodes = new Graph::node_id[mMask->getLength()];

    double dEnergy;
    double dOldEnergy = 100000; // almost infinity!

    Graph* graph = new Graph();

    // There should be something here, I assume :(

    graph->maxflow();

    delete graph;

#ifdef DEBUG_TIME
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::cout << "This optimization took " << duration << " seconds." << std::endl;
#endif // DEBUG_TIME
}
