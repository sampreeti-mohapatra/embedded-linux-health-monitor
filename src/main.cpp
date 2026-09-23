#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "temperature_monitor.h"
#include "network_monitor.h"
#include "service_monitor.h"
#include "logger.h"
#include "config_manager.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <csignal>
#include <sstream>

namespace {
std::atomic<bool> running{true};

void handle_signal(int) {
    running = false;
}

std::string pct(double value) {
    std::ostringstream out;
    out << value << "%";
    return out.str();
}
}

int main(int argc, char* argv[]) {
    const std::string config_path = argc > 1 ? argv[1] : "../config/health_monitor.json";

    try {
        Config config = load_config(config_path);
        Logger logger(config.log_file);

        std::signal(SIGINT, handle_signal);
        std::signal(SIGTERM, handle_signal);

        logger.log(LogLevel::INFO, "Device Health Monitor started.");

        while (running) {
            const double cpu = get_cpu_usage();
            const double memory = get_memory_usage();
            const double disk = get_disk_usage("/");
            const double temperature = get_system_temperature();
            const bool interface_up = is_interface_up(config.network.interface_name);
            const bool network_ok = interface_up && can_reach_host(config.network.ping_target);
            const bool service_ok = is_service_active(config.service.name);

            if (cpu >= 0) {
                logger.log(cpu > config.thresholds.cpu_percent ? LogLevel::WARNING : LogLevel::INFO,
                           "CPU usage: " + pct(cpu));
            }

            if (memory >= 0) {
                logger.log(memory > config.thresholds.memory_percent ? LogLevel::WARNING : LogLevel::INFO,
                           "RAM usage: " + pct(memory));
            }

            if (disk >= 0) {
                logger.log(disk > config.thresholds.disk_percent ? LogLevel::WARNING : LogLevel::INFO,
                           "Disk usage: " + pct(disk));
            }

            if (temperature >= 0) {
                logger.log(temperature > config.thresholds.temperature_celsius ? LogLevel::CRITICAL : LogLevel::INFO,
                           "System temperature: " + pct(temperature).replace(pct(temperature).find("%"), 1, " C"));
            } else {
                logger.log(LogLevel::WARNING, "Temperature sensor unavailable.");
            }

            if (!network_ok) {
                logger.log(LogLevel::ERROR, "Network unhealthy: interface=" +
                           config.network.interface_name + ", target=" + config.network.ping_target);
            } else {
                logger.log(LogLevel::INFO, "Network healthy.");
            }

            if (!service_ok) {
                logger.log(LogLevel::CRITICAL, "Critical service is inactive: " + config.service.name);
                if (config.service.restart_on_failure) {
                    logger.log(LogLevel::WARNING, "Attempting service restart...");
                    if (restart_service(config.service.name))
                        logger.log(LogLevel::INFO, "Service restarted successfully.");
                    else
                        logger.log(LogLevel::CRITICAL, "Service restart failed.");
                }
            } else {
                logger.log(LogLevel::INFO, "Critical service healthy: " + config.service.name);
            }

            std::this_thread::sleep_for(std::chrono::seconds(config.interval_seconds));
        }

        logger.log(LogLevel::INFO, "Device Health Monitor stopped.");
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
