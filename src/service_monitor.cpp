#include "service_monitor.h"
#include <cstdlib>
#include <string>

bool is_service_active(const std::string& service_name) {
    const std::string command = "systemctl is-active --quiet " + service_name;
    return std::system(command.c_str()) == 0;
}

bool restart_service(const std::string& service_name) {
    const std::string command = "systemctl restart " + service_name;
    return std::system(command.c_str()) == 0;
}
