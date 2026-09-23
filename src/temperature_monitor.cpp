#include "temperature_monitor.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>

double get_system_temperature() {
    namespace fs = std::filesystem;
    const std::string base = "/sys/class/thermal";

    try {
        for (const auto& entry : fs::directory_iterator(base)) {
            if (entry.path().filename().string().rfind("thermal_zone", 0) != 0)
                continue;

            std::ifstream type_file(entry.path() / "type");
            std::string type;
            std::getline(type_file, type);

            std::ifstream temp_file(entry.path() / "temp");
            long long milli = 0;
            if (temp_file >> milli) {
                return static_cast<double>(milli) / 1000.0;
            }
        }
    } catch (...) {
        return -1.0;
    }
    return -1.0;
}
