# ESP32 FreeRTOS: Dual Task Resource Contention

## Overview
This project explores what happens when **two distinct FreeRTOS tasks** attempt to control the **same hardware resource** (the onboard LED) simultaneously.

The code generates `ToggleLED1` (blinking at a 500ms rate) and `ToggleLED2` (blinking at a 333ms rate). Both are assigned the exact same priority level (1) and are pinned to the same CPU core.

## The Educational Focus: "Resource Clashing"
Unlike standard, sequential Arduino code, these two tasks run concurrently. Because they both issue commands to `GPIO 2` without any locking mechanism (like a Mutex), they "fight" over the hardware. 

* **Expected LED Behavior:** The LED will not blink cleanly at 500ms or 333ms. It will blink irregularly as the tasks constantly overwrite each other's state.
* **Expected Serial Output:** The Serial Monitor will show an interleaved mix of "Hello from Task 1" and "Hello from Task 2" as the FreeRTOS scheduler rapidly time-slices between the two competing threads.

## Key Concepts Demonstrated
* **Multiple Task Creation:** Spawning more than one independent RTOS thread using `xTaskCreatePinnedToCore`.
* **Equal Priorities & Time-Slicing:** Observing how the scheduler rapidly switches between tasks that share the same priority level.
* **Unprotected Shared Resources:** A practical demonstration of why shared hardware or memory in RTOS usually requires synchronization tools (like Semaphores or Mutexes) to prevent chaotic behavior.

## Hardware & Environment
* **Hardware:** ESP32 Development Board (using `LED_BUILTIN` / GPIO 2).
* **Framework:** Arduino Core for ESP32.
* **IDE:** PlatformIO / VS Code.

## How to Run & Observe
1. Build and upload the code to your ESP32.
2. **Observe the hardware:** Watch the physical LED to see the irregular, clashing blink pattern.
3. **Observe the software:** Open the Serial Monitor at `115200` baud to watch the scheduler actively bounce between Task 1 and Task 2.
