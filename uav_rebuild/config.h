#ifndef NET_CONFIG_H
#define NET_CONFIG_H

#include <string>

extern const std::string NET_CONFIG_PATH;

// Load value from config file
bool LoadConfigString(
    std::string &value, 
    const std::string &section, 
    const std::string &key, 
    const std::string &file);

bool LoadConfigInteger(
    int &value, 
    const std::string &section, 
    const std::string &key, 
    const std::string &file);

bool LoadConfigFloat(
    float &value, 
    const std::string &section, 
    const std::string &key, 
    const std::string &file);

bool LoadConfigBoolean(
    bool &value, 
    const std::string &section, 
    const std::string &key, 
    const std::string &file);

// Write value to config file
void WriteConfigString(
    const std::string &value, 
    const std::string &section, 
    const std::string &key, 
    const std::string &file);

void WriteConfigInteger(
    int value, 
    const std::string &section, 
    const std::string &key, 
    const std::string &file);

void WriteConfigFloat(
    float value, 
    const std::string &section, 
    const std::string &key, 
    const std::string &file);

void WriteConfigBoolean(
    bool value, 
    const std::string &section, 
    const std::string &key, 
    const std::string &file);

#endif /* NET_CONFIG_H */