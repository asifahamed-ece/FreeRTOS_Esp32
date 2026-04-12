#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
  const BaseTupe_t app_cpu = 0;
#else 
  const BaseType_t app_cpu = 1;
#endif

// Declaring Handles for Required Queues 
QueueHandle_t xQueue1_Command = NULL;
QueueHandle_t xQueue2_Telemetry = NULL;

// Declaring the Task Functions
// Task A: Handles Serial Input and Queue Communication
void vTaskA (void *parameters){
  char rxBuffer[32];
  uint32_t idx = 0;
  rxBuffer[0] = '\0'; // Initialize the buffer with Null Character {Caused Double Printing Issue}

  while(1){
    if(Serial.available() > 0){
      char c = Serial.read();

      if(c == '\n' || c == '\r'){
        if(idx > 0){

        rxBuffer[idx] = '\0'; // Null Terminate the String

        // Echo the input to Terminal
        Serial.print("[ Echo ] : ");
        Serial.println(rxBuffer);

        // Check if delay entered and Send it to Queue 1.
        uint32_t delayValue;
        if(sscanf(rxBuffer, "delay %d", &delayValue) == 1){
          // Sending delayvalue to Queue 1
          xQueueSend(xQueue1_Command, &delayValue, 0); // Send the delayValue Immediately to Queue
        }
      }
        idx = 0;
      }
      else{
        if(idx < sizeof(rxBuffer) - 1){
          rxBuffer[idx] = c;
          idx++;
        }
      }
    }
    // Another Function of the TaskA, See for telemetry from Queue 2
    char telemetryMsg[32]; // Create a String container for Storing Telemetry msg recieved...

    if(xQueueReceive(xQueue2_Telemetry, &telemetryMsg, 0) == pdTRUE){
    Serial.print("[ Task A ] Recived : ");
    Serial.println(telemetryMsg);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); // Small Delay to prevent Task A from hogging CPU
  }
}

// Task B : Controls the LED Blinking based on delay values from Queue 1
void vTaskB(void * Parameters){
  
  uint32_t blinkDelayMs = 500; // Default Starting delayValue
  uint32_t blinkCount = 0; // Counter for Number of Blinks

  while(1){

    uint32_t newDelay;

    if(xQueueReceive(xQueue1_Command, &newDelay, 0) == pdTRUE){
      blinkDelayMs = newDelay;
      Serial.print("[Task B] Updated Delay : ");
      Serial.println(blinkDelayMs);
    }
    // --- Blink the LED (BLOCKING way) ---
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(blinkDelayMs));
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(blinkDelayMs));

    // After a full cycle (ON + OFF), increment count
    blinkCount++;

    // Every 100 full cycles, send telemetry
    if (blinkCount >= 100) {
      char msg[32];
      snprintf(msg, sizeof(msg), "Blinked %lu times", blinkCount);
      xQueueSend(xQueue2_Telemetry, msg, 0);
      Serial.print("[Task B] Sent telemetry: ");
      Serial.println(msg);
      blinkCount = 0; // Reset count

      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(2000)); // Give Serial Time so Settle

  // Queue1: Holds uint32_t delay values. Length 5.
  xQueue1_Command = xQueueCreate(5, sizeof(uint32_t));

  // Queue2: Holds fixed-size strings. Length 5, each string max 32 chars.
  xQueue2_Telemetry = xQueueCreate(5, sizeof(char[32]));

  // Check if Queue were Created Successfully
  if(xQueue1_Command == NULL || xQueue2_Telemetry == NULL){
    Serial.println("Failed to Create Queue(s)");
  }

  // Creation of Tasks 

  xTaskCreatePinnedToCore(
    vTaskA,
    "Serial Staff",
    1500, // Stack Size
    NULL, // Parameters
    2, // Priority
    NULL, // Task Handle
    app_cpu
  );

  xTaskCreatePinnedToCore(
    vTaskB,
    "Blink Controller Task",
    1024,
    NULL, // Parameters
    1, // Output Display only so lower than Task A
    NULL, // Handlers
    app_cpu 
  );

}

void loop(){
  // NOthing here...
}