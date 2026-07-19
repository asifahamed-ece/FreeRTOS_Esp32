<div align="center">

# 🚀 FreeRTOS on ESP32

![Language](https://img.shields.io/badge/Language-Embedded%20C-blue?logo=c)
![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-green?logo=linux)
![Hardware](https://img.shields.io/badge/Hardware-ESP32-orange?logo=espressif)
![Framework](https://img.shields.io/badge/Framework-ESP--IDF-purple)
![Status](https://img.shields.io/badge/Status-Active%20Learning-brightgreen)

*A structured learning journey and project portfolio documenting my mastery of FreeRTOS on the ESP32.*

</div>

---

# FreeRTOS on ESP32 - Critical Concepts & Learning Guide

A hands-on repository documenting essential Real-Time Operating System concepts through practical ESP32 implementations.

---

## 🎯 Core RTOS Concepts Covered

### **Task Management**
- Task Creation & Deletion (`xTaskCreate`, `vTaskDelete`)
- Task States: **Running**, **Ready**, **Blocked**, **Suspended**, **Terminated**
- **Priority-Based Preemptive Scheduling**
- **Time Slicing** (Round-Robin for equal priorities)
- **Context Switching** overhead and mechanics
- **Idle Task** and **Hook Functions**
- Stack size calculation & **Stack Overflow** detection

### **Inter-Task Communication (IPC)**
- **Queues**: `xQueueSend`, `xQueueReceive`, `xQueueCreate`
- Queue blocking vs non-blocking operations
- **Mailboxes** (pointer-based queues)
- **Task Notifications** (lightweight alternative to queues)
- Data copying vs pointer passing in queues

### **Synchronization Primitives**
- **Binary Semaphores**: Task synchronization, ISR signaling
- **Counting Semaphores**: Resource counting, event accumulation
- **Mutexes** (Mutual Exclusion): **Priority Inheritance** mechanism
- **Recursive Mutexes**: Nested locking scenarios
- **Critical Sections**: `taskENTER_CRITICAL()`, `taskEXIT_CRITICAL()`
- **Disabling Interrupts** vs task-level synchronization

### **Memory Management**
- **Heap_1**: Simple, no deletion
- **Heap_2**: Best-fit algorithm, no coalescing
- **Heap_3**: Wrapper for `malloc()`/`free()` (thread-safe)
- **Heap_4**: First-fit with **memory coalescing** (recommended)
- **Heap_5**: Multiple non-contiguous memory regions
- **Static vs Dynamic Allocation** trade-offs
- **Memory fragmentation** prevention

### **Real-Time Constraints & Timing**
- **Tick Rate** (`configTICK_RATE_HZ`)
- **Blocking Delays**: `vTaskDelay()`, `vTaskDelayUntil()`
- **Absolute vs Relative** delays
- **Jitter** and timing accuracy
- **Watchdog Timers** (Task Watchdog, Interrupt Watchdog)

### **Interrupt Service Routines (ISR)**
- **FromISR API** variants (`xQueueSendFromISR`, etc.)
- **Deferred Interrupt Processing** (ISR defers to task)
- **Interrupt Latency** minimization
- **Nested Interrupts** handling
- **Yield from ISR**: `portYIELD_FROM_ISR()`

---

## ⚠️ Critical RTOS Pitfalls & Solutions

### **Priority Inversion**
- **Problem**: Low-priority task holds mutex needed by high-priority task
- **Solution**: Use **Mutexes** (not semaphores) - they have **Priority Inheritance**

### **Deadlocks**
- **Cause**: Circular wait conditions, nested locks
- **Prevention**: Lock ordering, timeout mechanisms (`xSemaphoreTake(mutex, pdMS_TO_TICKS(100))`)

### **Race Conditions**
- **Cause**: Unsynchronized access to shared resources
- **Solution**: **Critical Sections**, **Mutexes**, or **Disabling Interrupts**

### **Stack Overflow**
- **Detection**: Enable `configCHECK_FOR_STACK_OVERFLOW`
- **Prevention**: Calculate worst-case stack usage, use `uxTaskGetStackHighWaterMark()`

### **Priority Inversion vs Starvation**
- **Starvation**: Low-priority task never gets CPU time
- **Inversion**: Medium-priority task blocks high-priority indirectly

### **Memory Leaks**
- **Cause**: Creating tasks/queues without deletion
- **Solution**: Track all dynamic allocations, use static allocation where possible

---

## 🔧 ESP32-Specific Considerations

### **Dual-Core Architecture**
- **Core 0**: WiFi/BT stack, FreeRTOS idle tasks
- **Core 1**: Application tasks (default)
- **Pinning tasks**: `xTaskCreatePinnedToCore()`

### **FreeRTOS Configuration** (`sdkconfig`)
- `configNUM_TASKS_READY`: Max ready tasks
- `configMINIMAL_STACK_SIZE`: Idle task stack
- `configTOTAL_HEAP_SIZE`: Available heap memory
- `configMAX_TASK_NAME_LEN`: Task name length

### **Power Management**
- **Light Sleep**: CPU suspended, RAM retained
- **Deep Sleep**: Most power domains off
- Task-aware sleep: `vTaskSuspendAll()` before sleep

---

## 📊 Performance Metrics to Monitor

- **Task Execution Time**: Use `esp_timer_get_time()`
- **Queue Wait Time**: Measure blocking duration
- **CPU Utilization**: Idle task hook function
- **Heap Usage**: `xPortGetFreeHeapSize()`, `xPortGetMinimumEverFreeHeapSize()`
- **High Water Mark**: `uxTaskGetStackHighWaterMark()`

---

## 🎓 Learning Progression Path

1. **Bare-Metal → RTOS**: Understand why RTOS is needed
2. **Single Task → Multi-Task**: Context switching basics
3. **Shared Resources → Mutex Protection**: Race condition prevention
4. **Polling → Event-Driven**: Semaphores and queues
5. **Simple Delays → Real-Time Scheduling**: Deadline awareness
6. **Dynamic → Static Allocation**: Predictability in safety-critical systems

---

##  Key FreeRTOS API Reference

| Category | Functions |
|----------|-----------|
| **Tasks** | `xTaskCreate()`, `vTaskDelete()`, `vTaskDelay()`, `vTaskSuspend()`, `vTaskResume()` |
| **Queues** | `xQueueCreate()`, `xQueueSend()`, `xQueueReceive()`, `xQueuePeek()` |
| **Semaphores** | `xSemaphoreCreateBinary()`, `xSemaphoreCreateCounting()`, `xSemaphoreTake()`, `xSemaphoreGive()` |
| **Mutexes** | `xSemaphoreCreateMutex()`, `xSemaphoreCreateRecursiveMutex()` |
| **Memory** | `pvPortMalloc()`, `vPortFree()`, `heap_caps_malloc()` |
| **Critical** | `taskENTER_CRITICAL()`, `taskEXIT_CRITICAL()`, `taskDISABLE_INTERRUPTS()` |

---

## 💡 Best Practices

✅ **Use meaningful task names** (16-char limit on ESP32)  
✅ **Always check return values** (`pdPASS`, `errQUEUE_FULL`)  
✅ **Prefer static allocation** for deterministic behavior  
✅ **Use `pdMS_TO_TICKS()`** for portable timing  
✅ **Monitor stack usage** during development  
✅ **Avoid `printf()` in ISRs** (use deferred processing)  
✅ **Set watchdog timeouts** appropriately  
✅ **Document priority assignments** (prevent inversion)  

❌ **Never block in ISRs** (use FromISR APIs)  
❌ **Avoid infinite loops** without `vTaskDelay()`  
❌ **Don't share stack memory** between tasks  
❌ **Never delete a mutex** while tasks are blocked on it  

---

## 📚 Recommended Resources

- **Official Docs**: [FreeRTOS Kernel Documentation]([https://www.freertos.org/Documentation/api.html](https://freertos.org/Documentation/02-Kernel/07-Books-and-manual/01-RTOS_book))
- **ESP32 Specific**: [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/)
- **Book**: "Mastering the FreeRTOS Real Time Kernel" by Richard Barry
- **Debugging**: Use `configUSE_TRACE_FACILITY` for task visualization

---

**Built with ❤️ by Asif Ahamed**  
*Embedded Systems Enthusiast | Electronics & Communication Engineering*  
*Learning through disciplined, incremental progress*
