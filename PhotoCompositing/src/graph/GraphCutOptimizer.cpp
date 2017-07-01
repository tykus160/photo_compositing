#include "GraphCutOptimizer.h"

#include <stdexcept>
#include <iostream>

#ifdef DEBUG_TIME
#include <chrono>
#endif // DEBUG_TIME

#include "CostFunctions.h"

GraphCutOptimizer::GraphCutOptimizer(unsigned int capacity)
{
    if (capacity == 0)
    {
        throw std::invalid_argument("capacity must be greater than 0");
    }
    mCapacity = capacity;
    mLabels = new Image*[mCapacity];
}

void GraphCutOptimizer::addLabel(Image* image)
{
    if (mActualLength < mCapacity)
    {
        mLabels[mActualLength++] = image;
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

void GraphCutOptimizer::init()
{
    if (mActualLength == 0)
    {
        throw std::logic_error("Cannot optimize without any label");
    }
    if (mMask == nullptr)
    {
        throw std::logic_error("Cannot optimize without mask");
    }

    mNodes = new Graph::node_id*[mCapacity];
    mNodesActive = new bool*[mCapacity];

    for (unsigned int i = 0; i < mCapacity; ++i)
    {
        mNodes[i] = new Graph::node_id[mMask->getLength()];
        mNodesActive[i] = new bool[mMask->getLength()];
    }
}

void GraphCutOptimizer::optimize()
{
    init();

#ifdef DEBUG_TIME
    auto t1 = std::chrono::high_resolution_clock::now();
#endif // DEBUG_TIME

    double dEnergy = 0.0;
    double dOldEnergy = 100000; // almost infinity!

    for (int i = 0; i < mCapacity; ++i)
    {
        Graph* graph = new Graph();

        Image* image = mLabels[i];

        for (int y = 0; y < mMask->getHeight(); ++y)
        {
            for (int x = 0; x < mMask->getWidth(); ++x)
            {
                //std::cout << "x = " << x << " y = " << y << " idx = " << image->getCoordinatesAsIndex(x, y);
                mNodes[i][image->getCoordinatesAsIndex(x, y)] = graph->add_node();
            }
        }

        for (int y = 0; y < mMask->getHeight() - 1; ++y)
        {
            for (int x = 0; x < mMask->getWidth() - 1; ++x)
            {
                double eH = CostFunctions::distance(*(image->get(x, y)), *(image->get(x + 1, y)), mMask);
                double eV = CostFunctions::distance(*(image->get(x, y)), *(image->get(x, y + 1)), mMask);
                if (eH != 0 || eV != 0)
                std::cout << "eH = " << eH << ", eV = " << eV << std::endl;

                graph->add_edge(mNodes[i][image->getCoordinatesAsIndex(x, y)],
                    mNodes[i][image->getCoordinatesAsIndex(x + 1, y)],
                    eH,
                    eH);
                graph->add_edge(mNodes[i][image->getCoordinatesAsIndex(x, y)],
                    mNodes[i][image->getCoordinatesAsIndex(x, y + 1)],
                    eV,
                    eV);
            }
        }

        dEnergy = graph->maxflow();

        std::cout << "Energy: " << dEnergy << std::endl;
        if (dEnergy > dOldEnergy)
        {
            std::cout << "Something went terribly wrong!" << std::endl;
        }

        dOldEnergy = dEnergy;
        delete graph;
    }

    

#ifdef DEBUG_TIME
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();
    std::clog << "This optimization took " << duration << " milliseconds." << std::endl;
#endif // DEBUG_TIME
}
