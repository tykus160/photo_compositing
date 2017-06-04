#include "GraphCutOptimizer.h"

#include <stdexcept>
#include <iostream>

#ifdef DEBUG_TIME
#include <chrono>
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
    else
    {
        std::cout << "You cannot add more labels." << std::endl;
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

    for (int label = 0; label < mCapacity; ++label)
    {
        dEnergy = 0.0;


        Graph* graph = new Graph();

        // There should be something here, I assume :(

        const auto flow = graph->maxflow();
        dEnergy += flow;

        std::cout << "Energy for " << label << " label: " << dEnergy << std::endl;
        if (dEnergy < dOldEnergy)
        {
            // TODO more logic here
            dOldEnergy = dEnergy;
        }
        else
        {
            std::cout << "Something went terribly wrong!" << std::endl;
        }

        delete graph;
    }    

#ifdef DEBUG_TIME
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds> (t2 - t1).count();
    std::cout << "This optimization took " << duration << " seconds." << std::endl;
#endif // DEBUG_TIME
}
