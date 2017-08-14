#ifndef __BMP_H__
#define __BMP_H__

#include "../Image.h"

class BMP : public Image
{
public:
    BMP();

    BMP(int width, int height);

    BMP(const char* filename);

    BMP(std::string filename);

    void saveToFile(const char* filename);
};

#endif
