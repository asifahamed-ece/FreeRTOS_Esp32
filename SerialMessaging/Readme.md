# ESP31 FreeRTOS: Dynamic Memory & Queue UART Messenger

## Overview
This project demonstrates advanced inter-task communication on an ESP32 using FreeRTOS. It solves the challenge of capturing variable-length UART input (from a Serial Monitor), dynamically allocating memory for it, and safely passing it to a second task for processing.

## The Challenge
1. **Task 1 (Receiver):** Listens to the Serial Monitor. When the user hits Enter, it dynamically allocates heap memory for the exact size of the string, copies the string into the heap, and passes the memory address (pointer) to a Queue.
2. **Task 2 (Printer):** Waits completely asleep. When a pointer arrives in the Queue, it wakes up, prints the message from the heap, and destroys the memory allocation to prevent leaks.

## Notes to Future Me (Core Concepts to Review)
* **Killing Global Variables:** Instead of relying on a massive  global array that wastes RAM, this code uses a small local buffer and  to use exactly the RAM needed.
* **Queueing Pointers:** The FreeRTOS Queue is configured with . We aren't sending the heavy string through the queue; we are just sending the lightweight 32-bit map coordinate (the pointer).
* **The  Trap:** The code is hardened to catch both Carriage Returns () and Newlines () and includes an  safety check to prevent the Serial Monitor from triggering double/empty messages.
* **The Cleanup:** Using  in Task 2 is mandatory. Without it, the ESP32 would run out of heap memory and hard-crash after a few messages.

## Hardware & Environment
* **Hardware:** ESP32 Development Board
* **Framework:** Arduino Core for ESP32
* **IDE:** PlatformIO / VS Code
* **Baud Rate:** 

## How to Run
1. Build and upload to your ESP32.
2. Open the Serial Monitor at 115200 baud.
3. Type a message and hit Enter. Task 1 will catch it, allocate memory, and Queue it. Task 2 will print it and clean up the RAM.
