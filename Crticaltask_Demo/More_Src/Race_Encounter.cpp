#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
  const BaseType_t app_cpu = 0;
#else
  const BaseType_t app_cpu = 1;
#endif

// Create a Shared Global Variable
volatile int sharedCounter = 0;

// Declaring SpinLock variable since Esp32 is DualCore 
portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

void incTask(void *Parameters){
  
  while(1){

    // Critical Section to protect the shared resource
    portENTER_CRITICAL(&myMutex); // Enter critical section
    sharedCounter++; // Increment the shared counter
    portEXIT_CRITICAL(&myMutex); // Exit critical section

    Serial.println("Counter: " + String(sharedCounter) + " from core " 
    + String(xPortGetCoreID())+ "Task :" + String(pcTaskGetName(NULL)) + "\n");
    
    vTaskDelay(pdMS_TO_TICKS(1)); // Smaller delay for Seeing Race Condition
  }
}


void setup() {
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for the serial monitor to open

  Serial.println("Hello world from core " + String(xPortGetCoreID()));

  // Create a task that will run on the other core
  xTaskCreatePinnedToCore(
    incTask,      // Function to implement the task
    "IncTask_1",    // Name of the task
    2048,        // Stack size in words
    NULL,         // Task input parameter
    1,            // Priority of the task
    NULL,         // Task handle
    app_cpu       // Core where the task should run
  );

  xTaskCreatePinnedToCore(
    incTask,      // Function to implement the task
    "IncTask_2",   // Name of the task
    2048,        // Stack size in words
    NULL,         // Task input parameter
    1,            // Priority of the task
    NULL,         // Task handle
    app_cpu       // Core where the task should run
  );

}

void loop() {
  // Nothing...
}