# FreeRTOS Race Condition Demonstration (ESP32)

[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-10.4.3-blue)](https://www.freertos.org/)
[![ESP32](https://img.shields.io/badge/ESP32-DevKitC-green)](https://www.espressif.com/)

## 📘 Overview

This project intentionally creates a **race condition** on an ESP32 to demonstrate one of the most critical pitfalls in multi‑tasking systems. Two identical tasks (`IncTask_1` and `IncTask_2`) running on the same core repeatedly increment a shared global counter without any synchronization mechanism. The result is unpredictable, non‑deterministic output that illustrates exactly why mutexes and critical sections are essential in FreeRTOS applications.

This code is a companion to the **Mutex** episode of the FreeRTOS learning series and serves as the “before” picture that shows what happens when concurrency is left unprotected.

## 🧠 Concept: What is a Race Condition?

A **race condition** occurs when two or more tasks access and modify a shared resource concurrently, and the final outcome depends on the unpredictable order of execution. In this demo, the line `sharedCounter++` in C is actually three assembly instructions:

1. **READ** the value from RAM into a CPU register.
2. **MODIFY** (increment) the register value.
3. **WRITE** the new value back to RAM.

If Task A is interrupted by Task B between the READ and WRITE steps, Task B will read the **old** value, increment it, and write it back. When Task A resumes, it also writes its incremented value, **overwriting** Task B’s update. One increment is lost forever.

## 🔧 Hardware Requirements

- Any ESP32 development board (DevKit V1, NodeMCU‑32S, etc.)
- USB cable for programming and Serial Monitor
- Arduino IDE or PlatformIO

## 🚀 How to Build and Run

### Arduino IDE
1. Install the ESP32 board package via Boards Manager.
2. Copy the full code into a new sketch.
3. Select your ESP32 board and the correct COM port.
4. Upload and open the Serial Monitor at **115200 baud**.

### PlatformIO
```bash
pio run --target upload
pio device monitor --baud 115200
