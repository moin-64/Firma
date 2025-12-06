#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static void init(const std::string& filename);
    static void log(LogLevel level, const std::string& message);

private:
    static std::ofstream logfile;
    static std::mutex mtx;
};

#endif // LOGGER_H
