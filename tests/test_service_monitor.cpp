#include "service_monitor.h"
#include <cassert>
#include <iostream>

int main() {
    // systemd may not be available in every CI/container environment.
    // Verify that the function can be called without crashing.
    const bool active = is_service_active("nonexistent-health-monitor-test-service");
    assert(!active);
    std::cout << "Service monitor test passed.\n";
}
