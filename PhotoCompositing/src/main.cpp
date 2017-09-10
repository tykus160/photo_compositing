#include "main.h"
#include "image/ImageOperations.h"
int main(int argc, char** argv)
{
    std::cout << "Hello world!" << std::endl;

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

            GraphCutOptimizer opt(numOfLabels, CostFunctions::crossCost, GRADIENT);
            BMP** bmps = new BMP*[numOfLabels];
            for (int i = 0; i < numOfLabels; ++i)
            {
                bmps[i] = new BMP(Properties::getInstance().get("image_" + std::to_string(i)));
                opt.addImage(bmps[i]);
            }

            opt.addMask(&mask);
            opt.optimize();

            opt.saveToImage(Properties::getInstance().get("dest_image"));

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
