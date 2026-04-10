#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
 const BaseType_t app_cpu = 0;
#else
 const BaseType_t app_cpu = 1;
#endif

// Led Pin
const int led = 2;

// perform some Mundane Tasks
void testTask(void *parameters){
  while(1){
    int a = 1;
    int b[100];

    // Do something with array so its not optimized by compiler
    for(int i = 0; i < 100; i++){
      b[0] = a + i;
    }
    Serial.println(b[0]);

    // Print the high water mark of the stack to see how much stack is being used
    Serial.print("High Water Mark of Stack: ");;
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Print the free heap size to see how much heap is being used
    Serial.print("Free Heap Size: ");
    Serial.println(xPortGetFreeHeapSize());

    // Allocate a Dynamic Memory in Heap to see how much heap is being used
    int *new_heap = (int *)pvPortMalloc(sizeof(int) * 1024);

    // Do something with the allocated memory so its not optimized by compiler
    for(int i=0; i < 1024; i++){
      new_heap[i] = 5 + i;
    }

    if(new_heap != NULL){
      Serial.println("Memory allocated successfully");

    // Heap Size Occupied after Mem Allocation
      Serial.print("Free Heap Size after pvPortMalloc: ");
      Serial.println(xPortGetFreeHeapSize());

    // Free the Allocated Heap
    vPortFree(new_heap);            // ** Try Removing this line and then Try adding it back to see the difference in heap size **

    vTaskDelay(pdMS_TO_TICKS(100)); // Delay to allow time for observation
    }
    else {
      Serial.println("Heap Memory is Full, Allocation Failed");
    }
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  // Led Blink that Code Started 
  digitalWrite(led, HIGH);
  vTaskDelay(pdMS_TO_TICKS(500));
  digitalWrite(led, LOW);

  // Delay to allow serial monitor to connect
  vTaskDelay(pdMS_TO_TICKS(2000));
  Serial.println();
  Serial.println("FreeRTOS Memory Demo");

  // Create a task that will run on the app_cpu core
  xTaskCreatePinnedToCore(
    testTask,          // Task function
    "Test Task",       // Name of the task
    1500,              // Stack Size in words (not bytes)
    NULL,              // Task input parameter
    1,                 // Priority of the task
    NULL,              // Task handle
    app_cpu            // Core where the task should run
  );
}

void loop(){
  // Main loop can perform other tasks or remain idle
}