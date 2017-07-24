#include "main.h"

int main(int argc, char** argv)
{
    std::cout << "Hello world!" << std::endl;

    if (argc >= Utils::MIN_NUM_OF_ARGS)
    {
        try
        {
            std::cout << "Loading file: " << argv[Utils::MIN_NUM_OF_ARGS - 1] << std::endl;

            BMP bitmap(argv[Utils::MIN_NUM_OF_ARGS - 1]);

            Mask mask(&bitmap);
            mask.createLabels();

            GraphCutOptimizer opt(argc - Utils::MIN_NUM_OF_ARGS, CostFunctions::labeling);
            BMP** bmps = new BMP*[argc - Utils::MIN_NUM_OF_ARGS];
            for (int i = Utils::MIN_NUM_OF_ARGS; i < argc; ++i)
            {
                std::cout << "Loading file: " << argv[i] << std::endl;
                bmps[i - Utils::MIN_NUM_OF_ARGS] = new BMP(argv[i]);
                opt.addImage(bmps[i - Utils::MIN_NUM_OF_ARGS]);
            }

            opt.addMask(&mask);
            opt.optimize();

            opt.saveToImage("C:\\Users\\Wojciech\\Desktop\\photo_compositing\\result.bmp");

            for (int i = 0; i < argc - Utils::MIN_NUM_OF_ARGS; ++i)
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
