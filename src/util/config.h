#ifndef HRPC_UTIL_CONFIG_H
#define HRPC_UTIL_CONFIG_H

#include <unordered_map>
#include <string>

namespace hrpc
{
namespace util
{

class Config {
public:
    void LoadConfigFile(const char* config_file);
    std::string Load(const std::string& key) const;
private:
    std::unordered_map<std::string, std::string> m_configMap;

    void Trim(std::string& src_buf);
};

} // namespace util
} // namespace hrpc

#endif // HRPC_UTIL_CONFIG_H