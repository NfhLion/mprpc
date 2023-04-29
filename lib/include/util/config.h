#pragma once

#include <unordered_map>
#include <string>

namespace hrpc
{
namespace util
{

class Config {
public:
    void LoadConfigFile(const char* config_file);
    std::string Load(const std::string& key);
private:
    std::unordered_map<std::string, std::string> m_configMap;

    void Trim(std::string& src_buf);
};

} // namespace util
} // namespace hrpc