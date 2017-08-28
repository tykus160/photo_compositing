#include "ImageOperations.h"

#include "bitmap/BMP.h"

int ImageOperations::sobelX[] = { 1,  0, -1,
                                  2,  0, -2,
                                  1,  0, -1 };

int ImageOperations::sobelY[] = { 1,  2,  1,
                                  0,  0,  0,
                                 -1, -2, -1 };

Image* ImageOperations::sobel(Image* input)
{
    Image* filtered = new BMP(input->getWidth(), input->getHeight());

    for (int x = 0; x < input->getWidth(); ++x)
    {
        filtered->set(x, 0, new RGBPixel);
        filtered->set(x, input->getHeight() - 1, new RGBPixel);
    }
    for (int y = 1; y < input->getHeight() - 1; ++y)
    {
        filtered->set(0, y, new RGBPixel);
        filtered->set(input->getWidth() - 1, y, new RGBPixel);
    }
    for (int y = 1; y < input->getHeight() - 1; ++y)
    {
        for (int x = 1; x < input->getWidth() - 1; ++x)
        {
            int rx = sobelX[0] * input->get(x - 1, y - 1)->r + sobelX[1] * input->get(x, y - 1)->r + sobelX[2] * input->get(x + 1, y - 1)->r
                + sobelX[3] * input->get(x - 1, y)->r + sobelX[4] * input->get(x, y)->r + sobelX[5] * input->get(x + 1, y)->r
                + sobelX[6] * input->get(x - 1, y + 1)->r + sobelX[7] * input->get(x, y + 1)->r + sobelX[8] * input->get(x + 1, y + 1)->r;
            int ry = sobelY[0] * input->get(x - 1, y - 1)->r + sobelY[1] * input->get(x, y - 1)->r + sobelY[2] * input->get(x + 1, y - 1)->r
                + sobelY[3] * input->get(x - 1, y)->r + sobelY[4] * input->get(x, y)->r + sobelY[5] * input->get(x + 1, y)->r
                + sobelY[6] * input->get(x - 1, y + 1)->r + sobelY[7] * input->get(x, y + 1)->r + sobelY[8] * input->get(x + 1, y + 1)->r;

            int r = std::sqrt(rx * rx + ry * ry);

            int gx = sobelX[0] * input->get(x - 1, y - 1)->g + sobelX[1] * input->get(x, y - 1)->g + sobelX[2] * input->get(x + 1, y - 1)->g
                + sobelX[3] * input->get(x - 1, y)->g + sobelX[4] * input->get(x, y)->g + sobelX[5] * input->get(x + 1, y)->g
                + sobelX[6] * input->get(x - 1, y + 1)->g + sobelX[7] * input->get(x, y + 1)->g + sobelX[8] * input->get(x + 1, y + 1)->g;
            int gy = sobelY[0] * input->get(x - 1, y - 1)->g + sobelY[1] * input->get(x, y - 1)->g + sobelY[2] * input->get(x + 1, y - 1)->g
                + sobelY[3] * input->get(x - 1, y)->g + sobelY[4] * input->get(x, y)->g + sobelY[5] * input->get(x + 1, y)->g
                + sobelY[6] * input->get(x - 1, y + 1)->g + sobelY[7] * input->get(x, y + 1)->g + sobelY[8] * input->get(x + 1, y + 1)->g;

            int g = std::sqrt(gx * gx + gy * gy);

            int bx = sobelX[0] * input->get(x - 1, y - 1)->b + sobelX[1] * input->get(x, y - 1)->b + sobelX[2] * input->get(x + 1, y - 1)->b
                + sobelX[3] * input->get(x - 1, y)->b + sobelX[4] * input->get(x, y)->b + sobelX[5] * input->get(x + 1, y)->b
                + sobelX[6] * input->get(x - 1, y + 1)->b + sobelX[7] * input->get(x, y + 1)->b + sobelX[8] * input->get(x + 1, y + 1)->b;
            int by = sobelY[0] * input->get(x - 1, y - 1)->b + sobelY[1] * input->get(x, y - 1)->b + sobelY[2] * input->get(x + 1, y - 1)->b
                + sobelY[3] * input->get(x - 1, y)->b + sobelY[4] * input->get(x, y)->b + sobelY[5] * input->get(x + 1, y)->b
                + sobelY[6] * input->get(x - 1, y + 1)->b + sobelY[7] * input->get(x, y + 1)->b + sobelY[8] * input->get(x + 1, y + 1)->b;

            int b = std::sqrt(bx * bx + by * by);

            RGBPixel* px = new RGBPixel;
            px->r = r;
            px->g = g;
            px->b = b;
            filtered->set(x, y, px);
        }
    }
    return filtered;
}
