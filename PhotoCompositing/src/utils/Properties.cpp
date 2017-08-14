#include "Properties.h"

#include <fstream>
#include "StringUtils.h"

void Properties::loadFile(char* filename)
{
    std::ifstream hfile(filename);
    if (hfile.is_open())
    {
        mMap.clear(); // we don't want to make mess
        std::string line;
        while (std::getline(hfile, line))
        {
            if (line[0] != '#') // hack for comments in file
            {
                auto index = line.find('=');
                if (index != std::string::npos)
                {
                    mMap[StringUtils::trim(line.substr(0, index))] = StringUtils::trim(line.substr(index + 1));
                }
            }
        }
        hfile.close();
    }
}

std::string& Properties::get(std::string key)
{
    return mMap[key];
}

int Properties::getInt(std::string key)
{
    return std::stoi(get(key));
}

Properties::~Properties()
{
    mMap.clear();
}
