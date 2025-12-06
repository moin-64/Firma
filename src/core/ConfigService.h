#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H

#include "json.hpp"
#include <string>

using json = nlohmann::json;

class ConfigService {
public:
    ConfigService(const std::string& config_file);
    ~ConfigService();

    const json& get(const std::string& key) const;

private:
    json config;
};

#endif // CONFIG_SERVICE_H
