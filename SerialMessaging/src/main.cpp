#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE 
  BaseType_t app_cpu = 0;
#else 
  BaseType_t app_cpu = 1;
#endif 

// 1. Declare the Queue Handle (NO global char arrays!)
QueueHandle_t msgQueue;

// --- Task 1: Receive Message ---
void receiveMsg(void *parameters) {
  char localBuffer[100]; // Temporary holding pen
  uint8_t index = 0;     // Keeps track of where we are in the buffer

  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();

      // If we hit 'Enter' (Newline)
      if (c == '\n' || c == '\r') {

        if(index > 0){ // Only process if we have something in the buffer
        localBuffer[index] = '\0'; // 1. Cap off the string with a Null Terminator

        // 2. Allocate exact memory in the heap (+1 for the '\0')
        char *heapPtr = (char *)pvPortMalloc(index + 1);

          if (heapPtr != NULL) {
            // 3. Copy our local string into the safe heap space
            strcpy(heapPtr, localBuffer);

            Serial.println("Task 1 Received: " + String(heapPtr)); // Just to show we got it

            // 4. Tap Task 2 on the shoulder and hand it the map coordinate (pointer)
            xQueueSend(msgQueue, &heapPtr, portMAX_DELAY);
          }

        index = 0; // Reset index for the next incoming message
        }
      }
      else {
        // Keep catching characters (with safety limit to prevent crashing)
        if (index < sizeof(localBuffer) - 1) {
          localBuffer[index] = c;
          index++;
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10)); // Tiny delay to feed the watchdog
  }
}

// --- Task 2: Print Message ---
void printMsg(void *parameters) {
  char *receivedPtr; // Empty pointer waiting for an address

  while (true) {
    // 5. This task goes into a deep coma UNTIL a pointer arrives in the Queue
    if (xQueueReceive(msgQueue, &receivedPtr, portMAX_DELAY) == pdPASS) {

      Serial.print("Task 2 Printed: ");
      Serial.println(receivedPtr); // Print the string from the heap

      // 6. Demolish the heap space so we don't run out of RAM!
      vPortFree(receivedPtr);
    }
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(1000)); // Give serial time to wake up
  Serial.println("Send a message ending with Enter/Newline!");

  // Initialize the Queue (Holds up to 10 pointers, each is the size of a char pointer)
  msgQueue = xQueueCreate(10, sizeof(char *));

  // Create Tasks
  xTaskCreatePinnedToCore(receiveMsg, "Receive Task", 2048, NULL, 2, NULL, app_cpu);
  xTaskCreatePinnedToCore(printMsg, "Print Task", 2048, NULL, 1, NULL, app_cpu);
}

void loop() {
  // FreeRTOS is driving now.
}