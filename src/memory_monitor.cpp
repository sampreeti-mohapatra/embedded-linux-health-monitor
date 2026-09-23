#include "memory_monitor.h"
#include <fstream>
#include <string>

double get_memory_usage() {
    std::ifstream file("/proc/meminfo");
    std::string key, unit;
    unsigned long long value;
    unsigned long long total = 0, available = 0;

    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") total = value;
        else if (key == "MemAvailable:") available = value;
    }

    if (total == 0) return -1.0;
    return 100.0 * (1.0 - static_cast<double>(available) / total);
}
