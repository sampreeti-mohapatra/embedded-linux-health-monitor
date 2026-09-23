#pragma once
#include <string>

enum class LogLevel { INFO, WARNING, ERROR, CRITICAL };

class Logger {
public:
    explicit Logger(const std::string& file_path);
    void log(LogLevel level, const std::string& message);
private:
    std::string file_path_;
};
