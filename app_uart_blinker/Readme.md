# ESP32 FreeRTOS: Dynamic LED Blinker via UART

## Overview
This project demonstrates concurrent task management and inter-task communication on an ESP32 using FreeRTOS. It solves the challenge of running a user interface (listening for serial input) concurrently with a hardware control task (blinking an LED) without using blocking delays like a standard `delay()`.

## The Challenge
Using FreeRTOS, create a multi-threaded system with two separate tasks:
1.  **UI Task:** Listens for an integer over UART (Serial Monitor) and updates a shared variable.
2.  **Control Task:** Blinks the onboard LED at a rate specified by that integer.

## Key Features
* **Multi-threading:** Runs two distinct FreeRTOS tasks (`SerialTask` and `BlinkTask`) independently.
* **Task Prioritization:** Prioritizes the UART serial reading over the LED blinking to prevent dropping incoming serial data.
* **Non-Blocking Delays:** Utilizes `vTaskDelay` to yield the processor to other tasks, preventing task starvation.
* **Core Pinning:** Demonstrates pinning tasks to a specific application core (UniCore configuration).

## Hardware Requirements
* ESP32 Development Board (e.g., ESP32 NodeMCU)
* USB Cable for programming and Serial communication
* *(Optional)* External LED and resistor if not using the onboard LED (GPIO 2)

## Software Environment
* **PlatformIO** (Recommended) or Arduino IDE.
* Espressif 32 platform core.

## Task Breakdown

### 1. `SerialTask` (Priority: 2)
This task continuously polls the serial buffer. When an integer is sent via the Serial Monitor (terminated by a newline), it parses the string, validates the integer (ensuring it's greater than 0), and updates the global `rate` variable. It runs at a higher priority to ensure no serial data is missed while the ESP32 is busy.

### 2. `BlinkTask` (Priority: 1)
This task handles the hardware interaction. It toggles GPIO 2 HIGH and LOW, using `vTaskDelay` calculated from the current `rate` variable. Because it uses RTOS delays, it freely hands control back to the CPU to check for serial inputs during its "wait" periods.

## Getting Started (PlatformIO)

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/asifahamed-ece/FreeRTOSEsp32.git](https://github.com/asifahamed-ece/FreeRTOSEsp32.git)
    cd app_uart_blinker
    ```
2.  **Open in PlatformIO:** Open the folder in VS Code with the PlatformIO extension installed.
3.  **Build and Upload:** Connect your ESP32 and click the "Upload" button (or run `pio run -t upload`).
4.  **Monitor:** Open the Serial Monitor and set the baud rate to `115200`. 
5.  **Interact:** Type an integer (e.g., `500`, `250`, `2000`) into the Serial Monitor and press Enter to dynamically change the blink rate in milliseconds.

## License
This project is open-source and available under the [MIT License](LICENSE).
