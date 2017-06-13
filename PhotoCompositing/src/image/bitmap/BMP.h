#ifndef __BMP_H__
#define __BMP_H__

#include "../Image.h"

class BMP : public Image
{
public:
    BMP(char* filename);

    void saveToFile(char* filename);
};

#endif
