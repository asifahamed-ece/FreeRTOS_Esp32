# ESP32 FreeRTOS: Producer-Consumer Architecture via Direct Task Notifications

## 📖 Overview
This repository serves as an educational template for implementing a **Producer-Consumer architecture** on an ESP32 using FreeRTOS. 

Instead of relying on standard Arduino  polling or wasteful global variables, this code demonstrates how to handle asynchronous data (UART serial input) by dynamically allocating heap memory and passing the memory address between independent tasks using high-speed Direct Task Notifications.

## 🏗️ System Architecture

The system is split into two independent FreeRTOS tasks:

### 1. The Producer (Task 1 - Priority 1)
* **Role:** Monitors the hardware UART for incoming serial data.
* **Mechanism:** It collects characters into a small, temporary local buffer. Once a complete message is detected (via a Newline character), it calculates the exact length of the string and allocates a precisely sized block of memory in the FreeRTOS heap. 
* **Handoff:** It copies the string into the heap and sends the 32-bit memory address (pointer) directly to the Consumer task using .

### 2. The Consumer (Task 2 - Priority 2)
* **Role:** Processes the data and cleans up memory.
* **Mechanism:** This task spends most of its time in a Blocked state, consuming **0% CPU**. It waits indefinitely via . 
* **Execution:** When a pointer arrives, it wakes up, reads the string directly from the heap, prints it, and critically, uses  to destroy the memory block to prevent memory leaks.

## 🧠 Key FreeRTOS Concepts Demonstrated

### 1. Direct Task Notifications vs. Queues
While Queues are excellent for buffering multiple items, **Direct Task Notifications** are significantly faster and use less RAM. Because a memory pointer on the ESP32 is exactly 32 bits, it fits perfectly into a task notification value. The Producer taps the Consumer on the shoulder and hands it the map coordinate of the data.

### 2. Dynamic Memory Management ( & )
In embedded systems, creating massive global arrays (e.g., ) permanently locks up valuable RAM. This code demonstrates Just-In-Time memory allocation. The system only consumes RAM when a message exists, and it only consumes exactly as much RAM as the message requires. 

### 3. Preemptive Scheduling in Action
This code is configured to visually demonstrate FreeRTOS preemption. 
Because the Consumer is Priority 2 and the Producer is Priority 1, the exact microsecond the Producer sends the notification, the RTOS forcefully pauses the Producer. The Consumer wakes up, prints the message, frees the memory, and goes back to sleep—only then does the Producer resume. 

### 4. Handling UART Line Endings (The Ghost Message Bug)
A common trap in embedded serial communication is the invisible Carriage Return () and Newline () characters sent by terminals. This code implements a robust safety filter:
* It actively ignores  to prevent terminal cursor overwrites.
* It checks if the buffer actually contains data () before allocating memory, preventing the system from generating empty ghost allocations if multiple line-ending characters are received.

## 🛠️ Hardware & Software Environment
* **Microcontroller:** ESP32 Development Board
* **Framework:** Arduino Core for ESP32
* **Build Environment:** PlatformIO / VS Code
* **Serial Baud Rate:** 115200

## 🚀 How to Run & Test
1. Clone the repository and open it in PlatformIO.
2. Build and upload the code to your ESP32.
3. Open your Serial Monitor (set to 115200 baud).
4. Type a message and hit Enter. Watch the logs to observe the dynamic allocation, task preemption, and memory cleanup sequence.
