#include "GraphCutOptimizer.h"

#include <stdexcept>
#include <iostream>

#ifdef DEBUG_TIME
#include <chrono>
#endif // DEBUG_TIME

#include "../image/bitmap/BMP.h"
#include "../image/ImageOperations.h"
#include "../utils/Properties.h"

/////////////////////////////////////////////////////////////////////////////////

GraphCutOptimizer::GraphCutOptimizer(unsigned int numberOfLabels, CostFunction function, Method method)
{
    if (numberOfLabels == 0)
    {
        throw std::invalid_argument("numberOfLabels must be greater than 0");
    }
    if (function == nullptr)
    {
        throw std::invalid_argument("function cannot be null");
    }
    this->method = method;
    mNumberOfLabels = numberOfLabels;
    mImages.reserve(mNumberOfLabels);
    if (method == GRADIENT)
    {
        mImagesGradients.reserve(mNumberOfLabels);
    }
    costFunction = function;
}

/////////////////////////////////////////////////////////////////////////////////

GraphCutOptimizer::~GraphCutOptimizer()
{
    mImages.clear();
    mImagesGradients.clear();
    delete[] mNodes;
    delete mMaskOrg;
    delete mMinGradient;
}

/////////////////////////////////////////////////////////////////////////////////

void GraphCutOptimizer::addImage(Image* image)
{
    if (mImages.size() < mNumberOfLabels)
    {
        if (method == GRADIENT)
        {
            mImagesGradients.push_back(ImageOperations::sobel(image));
        }
        mImages.push_back(image);
    }
    else
    {
        std::cout << "You cannot add more labels." << std::endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////

void GraphCutOptimizer::addMask(Mask* mask)
{
    mMask = mask;
}

/////////////////////////////////////////////////////////////////////////////////

void GraphCutOptimizer::init()
{
    if (mImages.empty())
    {
        throw std::logic_error("Cannot optimize without any label");
    }
    if (mMask == nullptr)
    {
        throw std::logic_error("Cannot optimize without mask");
    }
    mMaskOrg = new Mask(*mMask);
    mMask->fill();
    mNodes = new Graph::node_id[mMask->getLength()];
    if (method == GRADIENT)
    {
        mMinGradient = ImageOperations::minimum(mImagesGradients);
    }
}

/////////////////////////////////////////////////////////////////////////////////

void GraphCutOptimizer::optimize()
{
    if (!optimizationComplete)
    {
        init();

#ifdef DEBUG_TIME
        auto t1 = std::chrono::high_resolution_clock::now();
#endif // DEBUG_TIME

        double dOldEnergy = std::numeric_limits<double>::max(); // almost infinity!

        for (int iCycle = 0; iCycle < 2; ++iCycle) // convergence may not come in first cycle, more needed
        {
            for (int indexOfSource = 0; indexOfSource < mNumberOfLabels; ++indexOfSource) // loop for every label
            {
                double dEnergy = 0.0;
                Graph graph;
                Image* image = mImages.at(indexOfSource);

                // Preparing nodes
                for (int y = 0; y < mMask->getHeight(); ++y)
                {
                    for (int x = 0; x < mMask->getWidth(); ++x)
                    {
                        if (mMask->getLabelAtCoordinate(x, y) == indexOfSource)
                        {
                            mNodes[image->getCoordinatesAsIndex(x, y)] = nullptr;
                        }
                        else
                        {
                            mNodes[image->getCoordinatesAsIndex(x, y)] = graph.add_node();
                        }
                    }
                }

                // do the calculation
                for (int y = 0; y < mMask->getHeight(); ++y)
                {
                    for (int x = 0; x < mMask->getWidth(); ++x)
                    {
                        if (x < mMask->getWidth() - 1) // horizontal
                        {
                            dEnergy += calculateEnergy(graph, indexOfSource, x, y, x + 1, y);
                        }

                        if (y < mMask->getHeight() - 1) // vertical
                        {
                            dEnergy += calculateEnergy(graph, indexOfSource, x, y, x, y + 1);
                        }
                        calculateData(graph, indexOfSource, x, y);
                    }
                }

                dEnergy += graph.maxflow();

                std::cout << "Cycle: " << iCycle << ", index of source: "
                    << indexOfSource << ", Energy: " << dEnergy << std::endl;

                if (dEnergy < dOldEnergy) // validity check
                {
                    // assign new label
                    for (int y = 0; y < mMask->getHeight(); ++y)
                    {
                        for (int x = 0; x < mMask->getWidth(); ++x)
                        {
                            auto node = mNodes[image->getCoordinatesAsIndex(x, y)];
                            if (node != nullptr && graph.what_segment(node) == Graph::SINK)
                            {
                                mMask->setLabelAtCoordinate(x, y, indexOfSource);
                            }
                        }
                    }
                    dOldEnergy = dEnergy;
                }
                else
                {
                    std::cout << "Something went terribly wrong!" << std::endl;
                }
            } // source
        } // cycle

#ifdef DEBUG_TIME
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();
        std::clog << "This optimization took " << duration << " milliseconds." << std::endl;
#endif // DEBUG_TIME

        optimizationComplete = true;
    }
}

/////////////////////////////////////////////////////////////////////////////////

double GraphCutOptimizer::calculateEnergy(
    Graph& graph,
    int indexOfSource,
    int x1,
    int y1,
    int x2,
    int y2)
{
    double result = 0.0;
    auto imagesArray = method == DEFAULT ? &mImages : &mImagesGradients;
    auto image = imagesArray->at(indexOfSource);
    int label1 = mMask->getLabelAtCoordinate(x1, y1);
    int label2 = mMask->getLabelAtCoordinate(x2, y2);

    auto colorM0 = label1 != Mask::NO_LABEL ? imagesArray->at(label1)->get(x1, y1) : nullptr;
    auto colorM1 = image->get(x1, y1);
    auto colorN0 = label2 != Mask::NO_LABEL ? imagesArray->at(label2)->get(x2, y2) : nullptr;
    auto colorN1 = image->get(x2, y2);

    // check if nodes at provided coordinates are active
    bool active1 = mNodes[image->getCoordinatesAsIndex(x1, y1)] != nullptr;
    bool active2 = mNodes[image->getCoordinatesAsIndex(x2, y2)] != nullptr;

    double lowestDiff = 0;
    if (method == GRADIENT)
    {
        // additional operations for gradient minimization
        auto gradM = mMinGradient->get(x1, y1);
        auto gradN = mMinGradient->get(x2, y2);
        RGBPixel pxBlack;
        double diffM = pxBlack.distance(*gradM);
        double diffN = pxBlack.distance(*gradN);
        lowestDiff = diffM < diffN ? diffM : diffN;
    }

    // calculate energy
    double e00 = active1 && active2 ? (label1 != label2 ? colorM0->distance(*colorN0) - lowestDiff : 0.0) : 0.0;
    double e01 = active1            ? (indexOfSource != label2 ? colorM1->distance(*colorN0) - lowestDiff : 0.0) : 0.0;
    double e10 = active2            ? (indexOfSource != label1 ? colorM0->distance(*colorN1) - lowestDiff : 0.0) : 0.0;
    double e11 = method == DEFAULT  ? colorM1->distance(*colorN1) : 0.0;

    // add results to graph
    if (active1)
    {
        if (active2)
        {
            graph.add_term2(
                mNodes[image->getCoordinatesAsIndex(x1, y1)],
                mNodes[image->getCoordinatesAsIndex(x2, y2)],
                e00,
                e01,
                e10,
                e11);
        }
        else
        {
            graph.add_term1(mNodes[image->getCoordinatesAsIndex(x1, y1)], e01, e11);
        }
    }
    else if (active2)
    {
        graph.add_term1(mNodes[image->getCoordinatesAsIndex(x2, y2)], e10, e11);
    }
    else
    {
        // nothing to add
        result = e11;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////

void GraphCutOptimizer::calculateData(Graph& graph, int indexOfSource, int x, int y)
{
    auto node = mNodes[mImages.at(indexOfSource)->getCoordinatesAsIndex(x, y)];
    if (node != nullptr)
    {
        int e0 = mMask->getLabelAtCoordinate(x, y) == mMaskOrg->getLabelAtCoordinate(x, y) ? 0 : 1000;
        int e1 =                     indexOfSource == mMaskOrg->getLabelAtCoordinate(x, y) ? 0 : 1000;
        graph.add_term1(node, e0, e1);
    }
}

/////////////////////////////////////////////////////////////////////////////////

void GraphCutOptimizer::saveToImage(std::string filename)
{
    Image* image = new BMP(mMask->getWidth(), mMask->getHeight());
    for (int y = 0; y < mMask->getHeight(); ++y)
    {
        for (int x = 0; x < mMask->getWidth(); ++x)
        {
            image->set(x, y, getOptimizedValue(x, y));
        }
    }
    image->saveToFile(filename);
    mMask->saveToImage(Properties::getInstance().get("debug_mask"));
    delete image;
}

/////////////////////////////////////////////////////////////////////////////////

RGBPixel* GraphCutOptimizer::getOptimizedValue(int x, int y)
{
    int label = mMask->getLabelAtCoordinate(x, y);
    RGBPixel* pixel = mImages.at(label)->get(x, y);
    return new RGBPixel(*pixel);
}
