#include "GraphCutOptimizer.h"

#include <stdexcept>
#include <iostream>

#ifdef DEBUG_TIME
#include <chrono>
#endif // DEBUG_TIME

#include "CostFunctions.h"
#include "../image/bitmap/BMP.h"

GraphCutOptimizer::GraphCutOptimizer(unsigned int capacity)
{
    if (capacity == 0)
    {
        throw std::invalid_argument("capacity must be greater than 0");
    }
    mCapacity = capacity;
    mLabels.reserve(mCapacity);
}

void GraphCutOptimizer::addLabel(Image* image)
{
    if (mLabels.size() < mCapacity)
    {
        mLabels.push_back(image);
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
    if (mLabels.empty())
    {
        throw std::logic_error("Cannot optimize without any label");
    }
    if (mMask == nullptr)
    {
        throw std::logic_error("Cannot optimize without mask");
    }

    mNodes = new Graph::node_id*[mCapacity];
    //mNodesActive = new bool*[mCapacity];

    for (unsigned int i = 0; i < mCapacity; ++i)
    {
        mNodes[i] = new Graph::node_id[mMask->getLength()];
        //mNodesActive[i] = new bool[mMask->getLength()];
    }
}

void GraphCutOptimizer::optimize()
{
    if (!optimizationComplete)
    {
        init();

#ifdef DEBUG_TIME
        auto t1 = std::chrono::high_resolution_clock::now();
#endif // DEBUG_TIME

        double dEnergy = 0.0;
        double dOldEnergy = 100000; // almost infinity!

        for (int indexOfSource = 0; indexOfSource < mCapacity; ++indexOfSource) // loop for every label
        {
            Graph* graph = new Graph();

            Image* image = mLabels.at(indexOfSource);

            // Preparing nodes
            for (int y = 0; y < mMask->getHeight(); ++y)
            {
                for (int x = 0; x < mMask->getWidth(); ++x)
                {
                    if (mMask->getLabelAtCoordinate(x, y) == indexOfSource)
                    {
                        mNodes[indexOfSource][image->getCoordinatesAsIndex(x, y)] = nullptr;
                        //mNodesActive[indexOfSource][image->getCoordinatesAsIndex(x, y)] = false;
                    }
                    else
                    {
                        mNodes[indexOfSource][image->getCoordinatesAsIndex(x, y)] = graph->add_node();
                        //mNodesActive[indexOfSource][image->getCoordinatesAsIndex(x, y)] = true;
                    }
                }
            }

            for (int y = 0; y < mMask->getHeight() - 1; ++y)
            {
                for (int x = 0; x < mMask->getWidth() - 1; ++x)
                {
                    auto actualNode = mNodes[indexOfSource][image->getCoordinatesAsIndex(x, y)];
                    if (actualNode != nullptr)
                    {
                        auto px1 = image->get(x, y);
                        auto nextNodeH = mNodes[indexOfSource][image->getCoordinatesAsIndex(x + 1, y)];
                        if (nextNodeH != nullptr) // HORIZONTAL
                        {
                            auto px2 = image->get(x + 1, y);
                            double eH = CostFunctions::labeling(*px1, *px2, mMask);
                            graph->add_edge(actualNode, nextNodeH, eH, eH);
                        }

                        auto nextNodeV = mNodes[indexOfSource][image->getCoordinatesAsIndex(x, y + 1)];
                        if (nextNodeV != nullptr) // VERTICAL
                        {
                            auto px3 = image->get(x, y + 1);
                            double eV = CostFunctions::labeling(*px1, *px3, mMask);
                            graph->add_edge(actualNode, nextNodeV, eV, eV);
                        }
                    }
                }
            }

            dEnergy += graph->maxflow();

            std::cout << "Energy: " << dEnergy << std::endl;
            /*
            for (int y = 0; y < mMask->getHeight(); ++y)
            {
                for (int x = 0; x < mMask->getWidth(); ++x)
                {
                    auto node = mNodes[indexOfSource][image->getCoordinatesAsIndex(x, y)];
                    if (node != nullptr && graph->what_segment(node) != Graph::SOURCE)
                    {
                        mMask->setLabelAtCoordinate(x, y, indexOfSource);
                    }
                }
            }
            */
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

        optimizationComplete = true;
    }
}

void GraphCutOptimizer::saveToImage(char* filename)
{
    if (filename == nullptr)
    {
        throw std::logic_error("Filename must be specified");
    }
    BMP image(mMask->getWidth(), mMask->getHeight());
    for (int y = 0; y < mMask->getHeight(); ++y)
    {
        for (int x = 0; x < mMask->getWidth(); ++x)
        {
            image.set(x, y, getOptimizedValue(x, y));
        }
    }
    image.saveToFile(filename); 
}

RGBPixel* GraphCutOptimizer::getOptimizedValue(int x, int y)
{
    int label = mMask->getLabelAtCoordinate(x, y);
    RGBPixel* pixel = mLabels.at(label)->get(x, y);
    return new RGBPixel(*pixel);
}
