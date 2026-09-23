#include "config_manager.h"
#include <cassert>
#include <iostream>

int main() {
    const Config c = load_config("../config/health_monitor.json");
    assert(c.interval_seconds > 0);
    assert(c.thresholds.cpu_percent > 0);
    assert(!c.service.name.empty());
    std::cout << "Config test passed.\n";
}
