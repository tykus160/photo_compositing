#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__

#include <string>
#include <map>

class Properties
{
private:
    
    std::map<const std::string, std::string> mMap;

    Properties() {}

public:
    static Properties& getInstance()
    {
        static Properties instance;
        return instance;
    }

    ~Properties();
    
    Properties(const Properties& other) = delete;
    
    void operator=(const Properties& other) = delete;
    
    void loadFile(char* filename);
    
    std::string& get(std::string key);
    
    int getInt(std::string key);
};

#endif // !__PROPERTIES_H__
