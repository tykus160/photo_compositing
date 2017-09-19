#include "GraphCutOptimizer.h"

#include <limits>
#include <stdexcept>
#include <iostream>

#ifdef DEBUG_TIME
#include <chrono>
#endif // DEBUG_TIME

#include "../image/bitmap/BMP.h"
#include "../image/ImageOperations.h"
#include "../utils/Properties.h"

/////////////////////////////////////////////////////////////////////////////////

GraphCutOptimizer::GraphCutOptimizer(unsigned int numberOfLabels, Method method)
{
    if (numberOfLabels == 0)
    {
        throw std::invalid_argument("numberOfLabels must be greater than 0");
    }
    this->method = method;
    mNumberOfLabels = numberOfLabels;
    mImages.reserve(mNumberOfLabels);
    if (method == GRADIENT || method == GRADIENT_2ND || method == COLOR_GRADIENT_MIXED)
    {
        mImagesGradientsH.reserve(mNumberOfLabels);
        mImagesGradientsV.reserve(mNumberOfLabels);
    }
}

/////////////////////////////////////////////////////////////////////////////////

GraphCutOptimizer::~GraphCutOptimizer()
{
    mImages.clear();
    mImagesGradientsH.clear();
    mImagesGradientsV.clear();
    delete[] mNodes;
    delete mMaskOrg;
    delete mMinGradientH;
    delete mMinGradientV;
}

/////////////////////////////////////////////////////////////////////////////////

void GraphCutOptimizer::addImage(Image* image)
{
    if (mImages.size() < mNumberOfLabels)
    {
        if (method == GRADIENT || method == GRADIENT_2ND || method == COLOR_GRADIENT_MIXED)
        {
            mImagesGradientsH.push_back(ImageOperations::sobelH(image));
            mImagesGradientsV.push_back(ImageOperations::sobelV(image));
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
#ifdef DEBUG_TIME
    auto t1 = std::chrono::high_resolution_clock::now();
#endif // DEBUG_TIME

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
        mMinGradientH = ImageOperations::minimum(mImagesGradientsH);
        mMinGradientV = ImageOperations::minimum(mImagesGradientsV);
    }

#ifdef DEBUG_TIME
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();
    std::clog << "Initialization phase took " << duration << " milliseconds." << std::endl;
#endif // DEBUG_TIME
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

        for (int iCycle = 0; iCycle < 5; ++iCycle) // convergence may not come in first cycle, more needed
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
                            dEnergy += calculateEnergy(graph, indexOfSource, x, y, true);
                        }

                        if (y < mMask->getHeight() - 1) // vertical
                        {
                            dEnergy += calculateEnergy(graph, indexOfSource, x, y, false);
                        }
                        dEnergy += calculateData(graph, indexOfSource, x, y, 1000);
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

                    std::string path = Properties::getInstance().get("save_path");
                    if (path.back() != '/' && path.back() != '\\')
                    {
                        path += '/';
                    }

                    Image* result = getImage();
                    result->saveToFile(path + "img_" + std::to_string(iCycle) + std::to_string(indexOfSource) + ".bmp");
                    Image* sobel0 = ImageOperations::sobelH(result);
                    sobel0->saveToFile(path + "img_sobel0_" + std::to_string(iCycle) + std::to_string(indexOfSource) + ".bmp");
                    delete sobel0;
                    Image* sobel1 = ImageOperations::sobelV(result);
                    sobel1->saveToFile(path + "img_sobel1_" + std::to_string(iCycle) + std::to_string(indexOfSource) + ".bmp");
                    delete sobel1;
                    delete result;
                    mMask->saveToImage(path + "img_mask_" + std::to_string(iCycle) + std::to_string(indexOfSource) + ".bmp");
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
    bool horizontal)
{
    double result = 0.0;
    int x2 = horizontal ? x1 + 1 : x1;
    int y2 = horizontal ? y1 : y1 + 1;
    auto imagesArray = method == COLOR || method == COLOR_2ND ? &mImages : (horizontal ? &mImagesGradientsH : &mImagesGradientsV);
    auto image = imagesArray->at(indexOfSource);
    int labelM = mMask->getLabelAtCoordinate(x1, y1);
    int labelN = mMask->getLabelAtCoordinate(x2, y2);

    auto colorM0 = imagesArray->at(labelM)->get(x1, y1);
    auto colorM1 = image->get(x1, y1);
    auto colorN0 = imagesArray->at(labelN)->get(x2, y2);
    auto colorN1 = image->get(x2, y2);

    // check if nodes at provided coordinates are active
    bool active1 = mNodes[image->getCoordinatesAsIndex(x1, y1)] != nullptr;
    bool active2 = mNodes[image->getCoordinatesAsIndex(x2, y2)] != nullptr;

    // calculate energy
    double e00, e01, e10, e11;

    switch (method)
    {
    case COLOR:
        e00 = active1 && active2 ? colorM0->distance(*colorN0) : 0.0;
        e01 = active1            ? colorM0->distance(*colorN1) : 0.0;
        e10 =            active2 ? colorM1->distance(*colorN0) : 0.0;
        e11 =                      colorM1->distance(*colorN1);
        break;
    case GRADIENT:
    {
        // additional operations for gradient minimization
        auto gradM = horizontal ? mMinGradientH->get(x1, y1) : mMinGradientV->get(x1, y1);
        auto gradN = horizontal ? mMinGradientH->get(x2, y2) : mMinGradientV->get(x2, y2);
        RGBPixel pxBlack;
        double diffM = pxBlack.distance(*gradM);
        double diffN = pxBlack.distance(*gradN);
        double lowestDiff = diffM < diffN ? diffM : diffN;

        e00 = active1 && active2 && labelM != labelN ? colorM0->distance(*colorN0) - lowestDiff : 0.0;
        e01 = active1 &&     indexOfSource != labelM ? colorM0->distance(*colorN1) - lowestDiff : 0.0;
        e10 = active2 &&     indexOfSource != labelN ? colorM1->distance(*colorN0) - lowestDiff : 0.0;
        e11 = 0.0;
    }
        break;
    case COLOR_2ND:
    case GRADIENT_2ND:
    case COLOR_GRADIENT_MIXED:
    {
        // if COLOR_GRADIENT_MIXED -> this is color phase
        auto colorMN = imagesArray->at(labelM)->get(x2, y2);
        auto colorNM = imagesArray->at(labelN)->get(x1, y1);
        e00 = active1 && active2 && labelM != labelN ? colorM0->distance(*colorNM) + colorMN->distance(*colorN0) : 0.0;
        e01 = active1 &&     indexOfSource != labelM ? colorM0->distance(*colorM1) + colorMN->distance(*colorN1) : 0.0;
        e10 = active2 &&     indexOfSource != labelN ? colorM1->distance(*colorNM) + colorN1->distance(*colorMN) : 0.0;
        e11 =                                                                                                      0.0;

        if (method == COLOR_GRADIENT_MIXED) // do the same calculation as above using gradients
        {
            auto gradient = horizontal ? &mImagesGradientsH : &mImagesGradientsV;
            colorM0 = gradient->at(labelM)->get(x1, y1);
            colorM1 = gradient->at(indexOfSource)->get(x1, y1);
            colorN0 = gradient->at(labelN)->get(x2, y2);
            colorN1 = gradient->at(indexOfSource)->get(x2, y2);
            colorMN = gradient->at(labelM)->get(x2, y2);
            colorNM = gradient->at(labelN)->get(x1, y1);

            e00 += active1 && active2 && labelM != labelN ? colorM0->distance(*colorNM) + colorMN->distance(*colorN0) : 0.0;
            e01 += active1 &&     indexOfSource != labelM ? colorM0->distance(*colorM1) + colorMN->distance(*colorN1) : 0.0;
            e10 += active2 &&     indexOfSource != labelN ? colorM1->distance(*colorNM) + colorN1->distance(*colorMN) : 0.0;
        }
    }
        break;
    default:
        e00 = e01 = e10 = e11 = 0.0;
        break;
    }

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

int GraphCutOptimizer::calculateData(Graph& graph, int indexOfSource, int x, int y, int weight)
{
    int e1 = indexOfSource == mMaskOrg->getLabelAtCoordinate(x, y) ? 0 : weight;
    auto node = mNodes[mImages.at(indexOfSource)->getCoordinatesAsIndex(x, y)];
    if (node != nullptr)
    {
        int e0 = mMask->getLabelAtCoordinate(x, y) == mMaskOrg->getLabelAtCoordinate(x, y) ? 0 : weight;
        graph.add_term1(node, e0, e1);
        e1 = 0; // setting to 0 as we don't want to add this to energy, since value is already in graph
    }
    return e1;
}

/////////////////////////////////////////////////////////////////////////////////

Image* GraphCutOptimizer::getImage()
{
    Image* image = new BMP(mMask->getWidth(), mMask->getHeight());
    for (int y = 0; y < mMask->getHeight(); ++y)
    {
        for (int x = 0; x < mMask->getWidth(); ++x)
        {
            image->set(x, y, getOptimizedValue(x, y));
        }
    }
    return image;
}

/////////////////////////////////////////////////////////////////////////////////

RGBPixel* GraphCutOptimizer::getOptimizedValue(int x, int y)
{
    int label = mMask->getLabelAtCoordinate(x, y);
    RGBPixel* pixel = mImages.at(label)->get(x, y);
    return new RGBPixel(*pixel);
}
