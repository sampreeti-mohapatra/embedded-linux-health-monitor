#include "config_manager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

Config load_config(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Cannot open configuration: " + path);

    json j;
    file >> j;

    Config c;
    c.interval_seconds = j.value("interval_seconds", 5);

    if (j.contains("thresholds")) {
        auto t = j["thresholds"];
        c.thresholds.cpu_percent = t.value("cpu_percent", 90.0);
        c.thresholds.memory_percent = t.value("memory_percent", 85.0);
        c.thresholds.disk_percent = t.value("disk_percent", 90.0);
        c.thresholds.temperature_celsius = t.value("temperature_celsius", 80.0);
    }

    if (j.contains("network")) {
        auto n = j["network"];
        c.network.interface_name = n.value("interface", "eth0");
        c.network.ping_target = n.value("ping_target", "8.8.8.8");
    }

    if (j.contains("critical_service")) {
        auto s = j["critical_service"];
        c.service.name = s.value("name", "ssh");
        c.service.restart_on_failure = s.value("restart_on_failure", true);
    }

    if (j.contains("logging"))
        c.log_file = j["logging"].value("file", c.log_file);

    if (c.interval_seconds < 1) c.interval_seconds = 1;
    return c;
}
