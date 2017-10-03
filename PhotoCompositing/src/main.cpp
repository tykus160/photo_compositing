#include "main.h"

int main(int argc, char** argv)
{
    std::cout << "Graph Cut Optimization" << std::endl;

    if (argc >= Utils::MIN_NUM_OF_ARGS)
    {
        try
        {
            std::cout << "Loading file: " << argv[Utils::MIN_NUM_OF_ARGS - 1] << std::endl;
            Properties::getInstance().loadFile(argv[Utils::MIN_NUM_OF_ARGS - 1]);

            BMP bitmap(Properties::getInstance().get("mask"));

            Mask mask(&bitmap);
            mask.createLabels();

            int numOfLabels = mask.getNumberOfLabels();

            GraphCutOptimizer opt(numOfLabels, Utils::selectMethod());
            BMP** bmps = new BMP*[numOfLabels];
            for (int i = 0; i < numOfLabels; ++i)
            {
                bmps[i] = new BMP(Properties::getInstance().get("image_" + std::to_string(i)));
                opt.addImage(bmps[i]);
            }

            opt.addMask(&mask);
            opt.optimize();

            for (int i = 0; i < numOfLabels; ++i)
            {
                delete bmps[i];
            }
            delete[] bmps;
        }
        catch (std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
            Utils::endMessage();
            return ErrorCodes::WRONG_INPUT_FILE;
        }
    }

    Utils::endMessage();
}

void Utils::endMessage()
{
    std::cout << "Press enter to close..." << std::endl;
    std::cin.get();
}

Method Utils::selectMethod()
{
    std::string methodName = Properties::getInstance().get("method");
    std::cout << "Selected method: " << methodName << std::endl;
    Method method = UNDEFINED;
    if (methodName == "COLOR")
    {
        method = COLOR;
    }
    else if (methodName == "GRADIENT")
    {
        method = GRADIENT;
    }
    else if (methodName == "COLOR_2ND")
    {
        method = COLOR_2ND;
    }
    else if (methodName == "GRADIENT_2ND")
    {
        method = GRADIENT_2ND;
    }
    else if (methodName == "COLOR_GRADIENT_MIXED")
    {
        method = COLOR_GRADIENT_MIXED;
    }
    else
    {
        std::cout << "You did not add any of available methods!" << std::endl;
    }
    return method;
}
