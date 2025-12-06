#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

std::ofstream Logger::logfile;
std::mutex Logger::mtx;

void Logger::init(const std::string& filename) {
    logfile.open(filename, std::ios_base::app);
    if (!logfile.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

void Logger::log(LogLevel level, const std::string& message) {
    if (!logfile.is_open()) return;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

    std::lock_guard<std::mutex> lock(mtx);
    logfile << "[" << ss.str() << "] [" << logLevelToString(level) << "] " << message << std::endl;
}
