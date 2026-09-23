#pragma once
#include <string>
bool is_service_active(const std::string& service_name);
bool restart_service(const std::string& service_name);
