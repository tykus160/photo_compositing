#include "main.h"

#include "bitmap\BMP.h"

int main(int argc, char** argv)
{
    std::cout << "Hello world!" << std::endl;

    // test with local image - do not try this at home!
    BMP bitmap("E:/Obrazy/maze.bmp");
    std::cout << "width = " << bitmap.getWidth() << ", height = " << bitmap.getHeight() << std::endl;
    int i = 3;
    int j = 3;
    std::cout << "pixel value in " << i << "," << j << " = " << *bitmap(i, j) << std::endl;

    bitmap.saveToFile("E:/Obrazy/maze_out.bmp");
    

    // Type enter to close
    std::cout << "Press enter to close..." << std::endl;
    std::cin.get();
}
