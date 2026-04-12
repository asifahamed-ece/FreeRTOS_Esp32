# FreeRTOS Queue Challenge: Bidirectional Task Communication (ESP32)

[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-10.4.3-blue)](https://www.freertos.org/)
[![ESP32](https://img.shields.io/badge/ESP32-DevKitC-green)](https://www.espressif.com/)

## Overview
Two FreeRTOS tasks communicate bidirectionally using two queues on an ESP32.  
**Task A** reads UART input, echoes it, parses `delay <ms>` commands, and sends them to **Queue 1**. It also receives telemetry from **Queue 2** and prints it.  
**Task B** blinks the onboard LED at a configurable rate, reads new delays from **Queue 1**, and every 100 blinks sends a message to **Queue 2**.

## Features
- Two independent tasks with two queues  
- UART command parsing (`delay 200`)  
- Input echo for immediate feedback  
- Runtime‑adjustable LED blink rate  
- Telemetry reporting every 100 blink cycles  
- Robust handling of `\r` and `\n` line endings  
- Memory‑safe string handling  

## Hardware
- ESP32 development board (any)  
- Built‑in LED (GPIO 2)  
- USB‑UART for Serial Monitor  

## Build & Run
### Arduino IDE
1. Install ESP32 board package  
2. Open `src/main.cpp`  
3. Select board, upload, open Serial Monitor @ 115200 baud  

### PlatformIO
```bash
pio run --target upload
pio device monitor --baud 115200
```

### Bugs Fixed
Buffer overflow (rxBuffer[32] → rxBuffer[idx])
Double‑trigger on Enter (only process when idx > 0)
Missing idx++ (last char overwritten)
Queue creation at global scope (moved inside setup())

### Learning Outcomes
Queue creation, sending, receiving
Bidirectional command‑telemetry pattern
Non‑blocking queue reads
Safe string handling in C

### Next Steps
Replace blocking vTaskDelay() with non‑blocking state machine
Use structs for telemetry
Implement dynamic memory for strings
