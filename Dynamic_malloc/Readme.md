# ESP32 FreeRTOS: Stack & Heap Memory Management

## Overview
This project demonstrates how to monitor and manage memory (Stack and Heap) inside a FreeRTOS task. It provides real-time serial output to track how much memory is being consumed by local variables and dynamic allocations, serving as a practical tool to detect and prevent memory leaks.

## Key FreeRTOS Concepts Demonstrated
* **Stack Monitoring:** Uses  to check how close the task is to a Stack Overflow. The value returned is the minimum amount of remaining stack space (in words) since the task started.
* **Heap Monitoring:** Uses  to track available global RAM.
* **Dynamic Allocation:** Uses  to safely allocate memory in the FreeRTOS heap, completely separate from the task's stack.
* **Memory Leak Prevention:** Uses  to release memory back to the system. 

## The Memory Leak Experiment
Inside , there is a commented line: . 

1. **Run 1 (Normal):** Upload the code as-is. Watch the Serial Monitor. The Free Heap size will dip during allocation and recover, remaining stable forever.
2. **Run 2 (The Leak):** Comment out  and upload again. Watch the Serial Monitor. The heap will rapidly drain by ~4096 bytes every loop until it hits  and the allocation fails, demonstrating a fatal memory leak.

## Hardware & Environment
* **Hardware:** ESP32 Development Board (using GPIO 2 for status LED).
* **Environment:** PlatformIO / Arduino Core
* **Baud Rate:**  (Note: Ensure your Serial Monitor matches this rate).

## How to Run
1. Build and upload to your ESP32.
2. Open the Serial Monitor at 9600 baud.
3. Observe the stack water mark and heap size logs printing every 100ms.
