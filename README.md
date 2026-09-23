# Embedded Linux Device Health Monitor & Auto-Recovery Agent

A C++17 Linux daemon that continuously monitors device health and reacts to unhealthy conditions.

## Monitored metrics

1. CPU usage
2. RAM usage
3. Disk usage
4. CPU/system temperature
5. Network status
6. One critical systemd service

## Recovery behavior

- CPU/RAM/disk/temperature: log warnings or critical conditions.
- Network: log loss of interface/connectivity and optionally run a recovery command.
- Critical service: automatically restart it through `systemctl` when enabled.

## Requirements

Ubuntu/Debian example:

```bash
sudo apt update
sudo apt install -y build-essential cmake nlohmann-json3-dev
```

The project uses C++17 and CMake.

## Build

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Run

From the project root:

```bash
sudo ./build/device-health-monitor
```

The default configuration monitors the `ssh` service. Change `config/health_monitor.json` if your machine uses another service, for example `sshd`.

For a normal user run, change the log path to a writable location.

## Install as a systemd service

```bash
sudo ./scripts/install.sh
```

Check:

```bash
systemctl status device-health-monitor
journalctl -u device-health-monitor -f
```

Uninstall:

```bash
sudo ./scripts/uninstall.sh
```

## Simulate service failure

Set the critical service in the JSON configuration first, then:

```bash
sudo ./scripts/simulate_failure.sh
```

The monitor should detect the stopped service and restart it.

## Project architecture

See `docs/architecture.md`.

## Tests

```bash
cd build
ctest --output-on-failure
```

The tests use small executable programs with standard C++ assertions, so GoogleTest is not required.

## Safety note

The monitor runs privileged commands only where required. Do not configure an arbitrary recovery command from an untrusted configuration file. The service name should refer to a known systemd service.
