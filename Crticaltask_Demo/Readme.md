# FreeRTOS Race Condition Demonstration (ESP32)

[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-10.4.3-blue)](https://www.freertos.org/)
[![ESP32](https://img.shields.io/badge/ESP32-DevKitC-green)](https://www.espressif.com/)

# FreeRTOS Concurrency Demos for ESP32

This project demonstrates **race conditions** and synchronization primitives (Critical Sections and Mutexes) on the ESP32 using the Arduino framework and FreeRTOS.

## 📁 Project Structure

* **`src/`**: The active build directory. **Keep only ONE `.cpp` file here at a time.**
* **`More_Src/`**: Repository of demo files:
    * `Demo_RaceCond.cpp`: Shows data corruption (lost increments).
    * `Race_Encounter.cpp`: Uses hardware spinlocks/critical sections.
    * `Race_Mutex.cpp`: Uses FreeRTOS semaphores (blocking).

## 🧪 Demo Details & Concepts

### 1. Race Condition (`Demo_RaceCond.cpp`)
* **Concept**: Occurs when multiple tasks access shared data simultaneously.
* **The Issue**: The increment operation (`counter++`) is not **atomic**. It involves a Read-Modify-Write cycle. If two cores read the same value before either writes back, one increment is lost.
* **Observation**: Final counter value will be lower than the target; serial output will be interleaved/garbled.

### 2. Critical Section (`Race_Encounter.cpp`)
* **Concept**: Uses `portENTER_CRITICAL` to provide exclusive access.
* **The Mechanics**: Disables interrupts on the local core and uses a **Spinlock** to prevent the other core from entering the same section.
* **Constraints**: Must be **extremely short**. Never use blocking functions (like `vTaskDelay` or `Serial.print`) inside, as the scheduler and interrupts are disabled.

### 3. Mutex (`Race_Mutex.cpp`)
* **Concept**: A synchronization primitive for managing shared resources (like Serial or I2C).
* **The Mechanics**: Tasks that cannot acquire the mutex enter a **Blocked state**, allowing the CPU to run other tasks.
* **Benefit**: Supports **Priority Inheritance** to prevent priority inversion. Safe for longer operations and functions that might block.

---

## 📊 Comparison Summary

| Feature | Race Condition | Critical Section | Mutex |
| :--- | :--- | :--- | :--- |
| **Data Integrity** | ❌ Corrupted | ✅ Protected | ✅ Protected |
| **CPU Method** | N/A | Disables Interrupts | Task Blocking |
| **Best Use Case** | To show what fails | Simple increments/flags | Peripherals (Serial/I2C) |
| **Speed** | Fastest (unsafe) | Very Fast | Moderate (OS overhead) |

---

## 🔀 How to Run

1.  **Clean `src/`**: Remove any existing `.cpp` files in the `src/` folder.
2.  **Select Demo**: Copy your chosen file from `More_Src/` to `src/`.
    * *Example (Linux/macOS):* `cp More_Src/Race_Mutex.cpp src/`
    * *Example (Windows):* `copy More_Src\Race_Mutex.cpp src\`
3.  **Deploy**: Build and Upload via PlatformIO.

> [!WARNING]
> Including multiple files in `src/` will cause **"multiple definition of setup/loop"** linker errors.

## 🛠 Requirements
* **Platform**: ESP32 (e.g., DevKit V1)
* **IDE**: PlatformIO
* **Framework**: Arduino / FreeRTOS
