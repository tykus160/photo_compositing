#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <string>

namespace StringUtils
{
    void ltrim(std::string& s);
    void rtrim(std::string& s);
    std::string trim(std::string& s);
}

#endif // !__STRING_UTILS_H__
