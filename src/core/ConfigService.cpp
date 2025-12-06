#include "ConfigService.h"
#include <fstream>
#include <iostream>

ConfigService::ConfigService(const std::string& config_file) {
    std::ifstream f(config_file);
    if (f) {
        config = json::parse(f);
    } else {
        std::cerr << "Could not open config file: " << config_file << std::endl;
    }
}

ConfigService::~ConfigService() {}

const json& ConfigService::get(const std::string& key) const {
    return config.at(key);
}
