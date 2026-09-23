#include "logger.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

Logger::Logger(const std::string& file_path) : file_path_(file_path) {}

void Logger::log(LogLevel level, const std::string& message) {
    const char* names[] = {"INFO", "WARNING", "ERROR", "CRITICAL"};
    const auto now = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");

    const std::string line = "[" + timestamp.str() + "] [" +
                             names[static_cast<int>(level)] + "] " + message;

    std::cout << line << '\n';

    std::ofstream file(file_path_, std::ios::app);
    if (file) file << line << '\n';
}
