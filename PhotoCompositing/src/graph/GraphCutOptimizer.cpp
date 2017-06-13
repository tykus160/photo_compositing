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

    double dEnergy;
    double dOldEnergy = 100000; // almost infinity!

    for (unsigned int label = 0; label < mCapacity; ++label)
    {
        dEnergy = 0.0;


        Graph* graph = new Graph();

        // Preparation - creating nodes
        for (int i = 0; i < mMask->getLength(); ++i)
        {
            if (label == mMask->getLabelAtIndex(i)) // not active
            {
                mNodesActive[label][i] = false;
                mNodes[label][i] = nullptr;
            }
            else // active
            {
                mNodesActive[label][i] = true;
                mNodes[label][i] = graph->add_node();
            }
        }

        // add cost functions here

        for (int y = 0; y < mLabels[label]->getHeight(); ++y)
        {
            for (int x = 0; x < mLabels[label]->getWidth(); ++x)
            {
                int index = mLabels[label]->getCoordinatesAsIndex(x, y);
                if (x < mLabels[label]->getWidth() - 1) // HORIZONTAL
                {
                    double E00 = 0;
                    double E01 = 0;
                    double E10 = 0;

                    if (mNodesActive[label][index] && mNodesActive[label][index + 1])
                    {
                        //E00 =
                    }
                    if (mNodesActive[label][index])
                    {
                        //E01 =
                    }
                    if (mNodesActive[label][index + 1])
                    {
                        //E10 =
                    }
                    double E11 = 0; // sth else

                    if (mNodesActive[label][index])
                    {
                        if (mNodesActive[label][index + 1])
                        {

                        }
                        else
                        {

                        }
                    }
                    else
                    {
                        if (mNodesActive[label][index + 1])
                        {

                        }
                        else
                        {
                            // Nothing to add
                            dEnergy += E11;
                        }
                    }

                }

                if (y < mLabels[label]->getHeight() - 1) // VERTICAL
                {
                    int width = mLabels[label]->getWidth();

                    double E00 = 0;
                    double E01 = 0;
                    double E10 = 0;
                    
                    if (mNodesActive[label][index] && mNodesActive[label][index + width])
                    {
                        //E00 =
                    }
                    if (mNodesActive[label][index])
                    {
                        //E01 =
                    }
                    if (mNodesActive[label][index + width])
                    {
                        //E10 =
                    }
                    double E11 = 0; // sth else

                    if (mNodesActive[label][index])
                    {
                        if (mNodesActive[label][index + 1])
                        {

                        }
                        else
                        {

                        }
                    }
                    else
                    {
                        if (mNodesActive[label][index + 1])
                        {

                        }
                        else
                        {
                            // Nothing to add
                            dEnergy += E11;
                        }
                    }
                }
            }
        }

        const auto flow = graph->maxflow();
        dEnergy += flow;

        std::cout << "Energy for label " << label << ": " << dEnergy << std::endl;
        std::cout << "Old energy for label: " << dOldEnergy << std::endl;

        if (dEnergy < dOldEnergy)
        {
            // TODO more logic here
            for (int l = 0; l < 0; ++l) // change this loop later
            {

            }

            dOldEnergy = dEnergy;
        }

        delete graph;
    }    

#ifdef DEBUG_TIME
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();
    std::clog << "This optimization took " << duration << " milliseconds." << std::endl;
#endif // DEBUG_TIME
}
