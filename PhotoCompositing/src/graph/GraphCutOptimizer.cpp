#include "GraphCutOptimizer.h"

#include <stdexcept>
#include <iostream>

#ifdef DEBUG_TIME
#include <chrono>
#endif // DEBUG_TIME

#include "../image/bitmap/BMP.h"

double GraphCutOptimizer::MAX_WEIGHT = std::numeric_limits<double>::max();

GraphCutOptimizer::GraphCutOptimizer(unsigned int capacity, CostFunction function)
{
    if (capacity == 0)
    {
        throw std::invalid_argument("capacity must be greater than 0");
    }
    if (function == nullptr)
    {
        throw std::invalid_argument("function cannot be null");
    }
    mCapacity = capacity;
    mLabels.reserve(mCapacity);
    costFunction = function;
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

    for (unsigned int i = 0; i < mCapacity; ++i)
    {
        mNodes[i] = new Graph::node_id[mMask->getLength()];
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
        double dOldEnergy = MAX_WEIGHT; // almost infinity!

        /*for (int indexOfSource = 0; indexOfSource < mCapacity; ++indexOfSource) // loop for every label
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
                    }
                    else
                    {
                        mNodes[indexOfSource][image->getCoordinatesAsIndex(x, y)] = graph->add_node();
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
                            double eH = costFunction(*px1, *px2, mMask);
                            graph->add_edge(actualNode, nextNodeH, eH, eH);
                        }

                        auto nextNodeV = mNodes[indexOfSource][image->getCoordinatesAsIndex(x, y + 1)];
                        if (nextNodeV != nullptr) // VERTICAL
                        {
                            auto px3 = image->get(x, y + 1);
                            double eV = costFunction(*px1, *px3, mMask);
                            graph->add_edge(actualNode, nextNodeV, eV, eV);
                        }
                    }
                }
            }

            dEnergy += graph->maxflow();

            std::cout << "Energy: " << dEnergy << std::endl;
            
            //for (int y = 0; y < mMask->getHeight(); ++y)
            //{
            //    for (int x = 0; x < mMask->getWidth(); ++x)
            //    {
            //        auto node = mNodes[indexOfSource][image->getCoordinatesAsIndex(x, y)];
            //        if (node != nullptr && graph->what_segment(node) != Graph::SOURCE)
            //        {
            //            mMask->setLabelAtCoordinate(x, y, indexOfSource);
            //        }
            //    }
            //}
            
            if (dEnergy > dOldEnergy)
            {
                std::cout << "Something went terribly wrong!" << std::endl;
            }

            dOldEnergy = dEnergy;
            delete graph;
        }*/

        dEnergy = optimizationFirstPhase();

        std::cout << "Energy: " << dEnergy << std::endl;

#ifdef DEBUG_TIME
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();
        std::clog << "This optimization took " << duration << " milliseconds." << std::endl;
#endif // DEBUG_TIME

        optimizationComplete = true;
    }
}

double GraphCutOptimizer::optimizationFirstPhase()
{
    Graph* graph = new Graph();
    Graph::node_id* nodes = new Graph::node_id[mMask->getLength()];
    for (int y = 0; y < mMask->getHeight(); ++y)
    {
        for (int x = 0; x < mMask->getWidth(); ++x)
        {
            nodes[y * mMask->getWidth() + x] = graph->add_node();
        }
    }
    for (int y = 0; y < mMask->getHeight(); ++y)
    {
        for (int x = 0; x < mMask->getWidth(); ++x)
        {
            auto actualNode = nodes[y * mMask->getWidth() + x];
            const int label = mMask->getLabelAtCoordinate(x, y);
            if (label == 0)
            {
                graph->add_tweights(actualNode, MAX_WEIGHT, 0); // attach to source
            }
            else if (label == 1)
            {
                graph->add_tweights(actualNode, 0, MAX_WEIGHT); // attach to sink
            }

            if (x < mMask->getWidth() - 1) // horizontal edge
            {
                auto nextNodeH = nodes[y * mMask->getWidth() + x + 1];
                int labelH = mMask->getLabelAtCoordinate(x + 1, y);
                double eH = label == labelH ? 1 : 0;
                graph->add_edge(actualNode, nextNodeH, eH, eH);
            }

            if (y < mMask->getHeight() - 1) // vertical edge
            {
                auto nextNodeV = nodes[(y + 1) * mMask->getWidth() + x];
                int labelV = mMask->getLabelAtCoordinate(x, y + 1);
                double eV = label == labelV ? 1 : 0;
                graph->add_edge(actualNode, nextNodeV, eV, eV);
            }
        }
    }

    double energy = graph->maxflow();

    outputMask = new Mask(mMask->getWidth(), mMask->getHeight());

    for (int y = 0; y < outputMask->getHeight(); ++y)
    {
        for (int x = 0; x < outputMask->getWidth(); ++x)
        {
            auto actualNode = nodes[y * outputMask->getWidth() + x];
            auto label = graph->what_segment(actualNode);
            outputMask->setLabelAtCoordinate(x, y, label);
        }
    }

    delete graph;

    saveToImage("E:\\GoogleDrive\\magisterka\\tests\\test1\\sample_result.bmp");

    return energy;
}

void GraphCutOptimizer::saveToImage(char* filename)
{
    if (filename == nullptr)
    {
        throw std::logic_error("Filename must be specified");
    }
    BMP image(outputMask->getWidth(), outputMask->getHeight());
    for (int y = 0; y < outputMask->getHeight(); ++y)
    {
        for (int x = 0; x < outputMask->getWidth(); ++x)
        {
            image.set(x, y, getOptimizedValue(x, y));
        }
    }
    image.saveToFile(filename); 
}

RGBPixel* GraphCutOptimizer::getOptimizedValue(int x, int y)
{
    int label = outputMask->getLabelAtCoordinate(x, y);
    RGBPixel* pixel = mLabels.at(label)->get(x, y);
    return new RGBPixel(*pixel);
}
