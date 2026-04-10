# ESP32 FreeRTOS: Stack Overflow Demonstration

## Overview
This project is intentionally designed to fail. It demonstrates what happens when a FreeRTOS task runs out of allocated memory, causing a classic **Stack Overflow** and an ESP32 system crash.

## The Engineering Trap
When creating , the stack size is explicitly set to a very small  bytes. 

Inside the task:
1.  instantly consumes 400 bytes of that stack space.
2. FreeRTOS context switching and task overhead consume more background memory.
3.  is a complex function that requires a surprisingly large amount of stack space to execute. 

When  is called, the task exceeds its 1024-byte limit, overwrites restricted memory, and triggers a fatal hardware fault.

*(Note: The task also lacks a . Even if the stack didn't overflow, this tight  loop would eventually trigger a Task Watchdog Timer (TWDT) crash for starving the CPU!)*

## Expected Output (The Crash)
When you open the Serial Monitor at  baud, you will likely not see a stable loop. Instead, the ESP32 will panic, throw a **Guru Meditation Error** (specifically a  error), and endlessly reboot.

## How to Fix It
To stabilize the code:
1. Increase the stack size in  from  to .
2. Add a  inside the  loop to yield the CPU and prevent a watchdog reset.
