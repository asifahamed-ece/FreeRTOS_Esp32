#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
  const BaseType_t app_cpu = 0;
#else
  const BaseType_t app_cpu = 1;
#endif

// Create a Shared Global Variable
volatile int sharedCounter = 0;

SemaphoreHandle_t counterMutex;

void incTask(void *Parameters){
  
  while(1){
    if(xSemaphoreTake(counterMutex, portMAX_DELAY) == pdTRUE){

      sharedCounter++;  // Increment only if the Current Task has the Mutex Token

      Serial.println("Counter: " + String(sharedCounter) + " from core " 
      + String(xPortGetCoreID())+ "Task :" + String(pcTaskGetName(NULL)) + "\n");
      
      xSemaphoreGive(counterMutex);  // Give back the Mutex Token for other Tasks to use.
    }
    
    vTaskDelay(pdMS_TO_TICKS(1)); // Smaller delay for Seeing Race Condition
  }
}


void setup() {
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for the serial monitor to open

  Serial.println("Hello world from core " + String(xPortGetCoreID()));

  counterMutex = xSemaphoreCreateMutex();

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