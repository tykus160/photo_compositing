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

            GraphCutOptimizer opt(2); // index = argc - Utils::MIN_NUM_OF_ARGS
            opt.addLabel(&bitmap); // remove
            opt.addLabel(&bitmap); // remove

            for (int i = Utils::MIN_NUM_OF_ARGS; i < argc; ++i)
            {
                std::cout << "Loading file: " << argv[i] << std::endl;
                BMP bmp(argv[i]);
                opt.addLabel(&bmp);
            }

            opt.addMask(&mask);
            opt.optimize();
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
