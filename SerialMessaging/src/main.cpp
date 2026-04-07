#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE 
  BaseType_t app_cpu = 0;
#else // CONFIG_FREERTOS_UNICORE
  BaseType_t app_cpu = 1;
#endif // CONFIG_FREERTOS_UNICORE

// Declaring Variables for Inter Task Communication
char msg[1000];


// Task Functions
void recieveMsg(void *Parameters){
  while(true){
    if(Serial.available() > 0){
      Serial.readUntil('\n', msg, sizeof(msg)); // Read the incoming message until a newline character is encountered
      Serial.print("Message Recieved: ");
      Serial.println(msg);
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay to prevent task from hogging the CPU
  }
}

void printMsg(void *Parameters){
  while(true){
    if(strlen(msg) > 0){ // Check if msg is not empty
      Serial.print("Message to Print: ");
      Serial.println(msg);
      memset(msg, 0, sizeof(msg)); // Clear the message after printing
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay to prevent task from hogging the CPU
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println("Serial Messaging Between Cores");

  // Creating Tasks
  xTaskCreatePinnedToCore(
    recieveMsg, // Task Function
    "Recieve Message Task", // Name of Task
    10000, // Stack Size
    NULL, // Parameters
    2,  // Priority Higher than the printMsg Task
    NULL, // Task Handle
    app_cpu  // Core 
  );

    xTaskCreatePinnedToCore(
    printMsg, // Task Function
    "Printing Message Task", // Name of Task
    10000, // Stack Size
    NULL, // Parameters
    1,  // Priority Low
    NULL, // Task Handle
    app_cpu  // Core 
  );
}

void loop(){
  // Nothing Here, Since we are on RTOS, Bruhhh!
}