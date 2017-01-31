#include "config.h"

#include <windows.h>
#include <sstream>

using namespace std;

#pragma warning (disable : 4329)

const std::string NET_CONFIG_PATH = "..\\net_config.ini";

bool LoadConfigString(std::string &value, const std::string &section, const std::string &key, const std::string &file)
{
    char result[MAX_PATH];
    GetPrivateProfileStringA(section.c_str(), key.c_str(), "failed", result, MAX_PATH, file.c_str());
    if((value = result) == "failed") return false;
    return true;
}

bool LoadConfigInteger(int &value, const std::string &section, const std::string &key, const std::string &file)
{
    value = GetPrivateProfileIntA(section.c_str(), key.c_str(), -1, file.c_str());
    if(value == -1) return false;
    return true;
}

bool LoadConfigFloat(float &value, const std::string &section, const std::string &key, const std::string &file)
{
    char result[MAX_PATH];
    GetPrivateProfileStringA(section.c_str(), key.c_str(), "failed", result, MAX_PATH, file.c_str());
    
    std::string strValue = result;
    if(strValue == "failed")
        return false;

    std::stringstream strm;
    strm << strValue;
    strm >> value;

    return true;
}

bool LoadConfigBoolean(bool &value, const std::string &section, const std::string &key, const std::string &file)
{
    char result[MAX_PATH];
    GetPrivateProfileStringA(section.c_str(), key.c_str(), "failed", result, MAX_PATH, file.c_str());
    
    std::string strValue = result;
    if(strValue == "failed")
        return false;
    
    if(strValue == "False" || strValue == "false" || strValue == "0")
        value = false;
    else
        value = true;
    
    return true;
}

void WriteConfigString(const std::string &value, const std::string &section, const std::string &key, const std::string &file)
{
    WritePrivateProfileStringA(section.c_str(), key.c_str(), value.c_str(), file.c_str());
}

void WriteConfigInteger(int value, const std::string &section, const std::string &key, const std::string &file)
{
    stringstream strm;
    strm << value;
    WriteConfigString(strm.str(), section, key, file);
}

void WriteConfigFloat(float value, const std::string &section, const std::string &key, const std::string &file)
{
    stringstream strm;
    strm << value;
    WriteConfigString(strm.str(), section, key, file);
}

void WriteConfigBoolean(bool value, const std::string &section, const std::string &key, const std::string &file)
{
    string str = "True";
    if(!value) str = "False";
    WriteConfigString(str, section, key, file);
}