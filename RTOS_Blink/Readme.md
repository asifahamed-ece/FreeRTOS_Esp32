# ESP32 FreeRTOS: Basic LED Blink

## Overview
This is my very first FreeRTOS project on the ESP32. It serves as a foundational template for creating and scheduling basic tasks using the Arduino framework within PlatformIO. 

Instead of relying on the standard `loop()` function and blocking `delay()` calls, this code hands hardware control over to the FreeRTOS scheduler.

## Key Concepts Demonstrated
* **Task Creation:** Using `xTaskCreatePinnedToCore()` to generate an independent thread for the LED.
* **Core Pinning:** Dynamically checking `CONFIG_FREERTOS_UNICORE` to safely assign tasks to Core 0 or Core 1 without disrupting WiFi/system processes.
* **RTOS Delays:** Using `vTaskDelay()` and `portTICK_PERIOD_MS` to yield the processor to other potential tasks while waiting, rather than freezing the CPU.
* **Scheduler Lifecycle:** Understanding that the Arduino framework auto-starts the FreeRTOS scheduler under the hood, bypassing the need for `vTaskStartScheduler()`.

## Hardware & Software
* **Hardware:** ESP32 Development Board (using `LED_BUILTIN` / GPIO 2)
* **Environment:** PlatformIO 
* **Framework:** Arduino

## How to Run
1. Clone this repository.
2. Open the project folder in PlatformIO.
3. Build and upload to your ESP32 board.
4. The onboard LED will immediately begin blinking at a 1Hz interval (500ms ON, 500ms OFF), completely independent of the main `loop()`.
