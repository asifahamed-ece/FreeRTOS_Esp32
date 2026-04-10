#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
 const BaseType_t app_cpu = 0;
#else
 const BaseType_t app_cpu = 1;
#endif

// perform some Mundane Tasks
void testTask(void *parameters){
  while(1){
    int a = 1;
    int b[100];

    // Do something with array so its not optimized by compiler
    for(int i = 0; i < 100; i++){
      b[i] = a + i;
    }
    Serial.println(b[0]);
  }
}

void setup(){
  Serial.begin(115200);

  // Delay to allow serial monitor to connect
  vTaskDelay(pdMS_TO_TICKS(2000));
  Serial.println();
  Serial.println("FreeRTOS Memory Demo");

  // Create a task that will run on the app_cpu core
  xTaskCreatePinnedToCore(
    testTask,          // Task function
    "Test Task",       // Name of the task
    1024,              // ** Knowingly Specified Small Stack Size to Demonstrate Memory Issues **
    NULL,              // Task input parameter
    1,                 // Priority of the task
    NULL,              // Task handle
    app_cpu            // Core where the task should run
  );
}

void loop(){
  // Main loop can perform other tasks or remain idle
}