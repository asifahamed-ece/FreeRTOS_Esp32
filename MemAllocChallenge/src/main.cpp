# include <Arduino.h>

// Task Handles
TaskHandle_t xTaskProducerHandle = NULL;
TaskHandle_t xTaskConsumerHandle = NULL;

// We will use a Direct Task Notification to Pass the POINTER.
// A Ptr on Esp32 is 32bit and Also the Notification Size is also 32bits.
// So we can Send the Ptr using the Notification Directly...

// Task 1 : Producer (Read UART for Messages)

void vTaskProducer(void *pvParameters){

  char rxbuff[256]; //Temp mem for Holding from Serial input (Static Mem)
    
  uint32_t idx = 0;
  char c;

  while(1){
    
    if(Serial.available() > 0){
      c = Serial.read();

      // Checking if its a NewLine char '\n"
      if(c == '\n'){
        // Null-terminate the buffer data
        rxbuff[idx]  = '\0';

        // Allocate the Dynamic heap memory
        char* pMessage = (char *)pvPortMalloc(idx + 1);  // (idx + 1) for Null char at End.

        // Check if Allocation Succeeded ?
        if(pMessage != NULL){
          Serial.println("Memory Allocated!");

          // Copy the rxBuffer to the new Heap Allocated
          strcpy(pMessage, rxbuff);

            // Send Allocated Message as a Notifcation to Printing Task 2
            xTaskNotify(xTaskConsumerHandle, (uint32_t)pMessage, eSetValueWithOverwrite);
            //  1. TypeCasted the Pointer into the Message, 2. Used the 3rd Arg API to Overwrite the mem with latest message sent.

            Serial.println("[Producer] Allocated and Sent Message.");
        }
        else{
          Serial.println("[Producer] ERROR : pvPortMalloc() failed!");
        }
        //  Reset the idx after Completing One Message.
        idx = 0;
      }
      else if(c != '\r'){
        // Store the Data until getting a "\n" char Yet...

        // Store the serial input in rxbuff if there's room
        if(idx < sizeof(rxbuff) - 1){
          rxbuff[idx] = c;
          idx++ ;
        }
        else{
          // Discards Entire Buffer data, No Overflow handle added.\
          idx = 0;
        }
      }
    }
    // Small Delay to Yeild CPU, Preventing WatchDog Timer if Serial is Slow.
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void vTaskConsumer(void *pvParameters){
  uint32_t ulNotificationValue; // Variable for Storing the Task Notification => String Ptr
  char *pRecMessage; // Ptr for the Address of rx msg from task 1

  while(1){
    // Wait Indefinitely Until this Task gets a Notification from producer (Which makes Consumer Unblocked )
    // The Notification will have the Memory address of the Msg.
    if( xTaskNotifyWait(0x00,
                        ULONG_MAX,
                        &ulNotificationValue,
                        portMAX_DELAY) == pdTRUE ){
    // Cast the Int value back to char ptr
    pRecMessage = (char *)ulNotificationValue;

    // Use the Data
    Serial.print("[Consumer] Recieved : ");
    Serial.println(pRecMessage);

    // Clean-Up  - Freeing Up the Memory
    vPortFree(pRecMessage);
    pRecMessage = NULL;

    Serial.println("[Consumer] Freed Memory.");
    }
  }
}

// Setup
void setup(){
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(2000));
  Serial.println("\n\n--- FreeRTOS Dynamic Memory Challenge ---");
  Serial.println("Type a Message and Press Enter.");

  xTaskCreate(
    vTaskProducer,
    "Producer Task (UART)",
    2048, // Allocated Stack in words
    NULL,
    1,  // Lower Priority
    &xTaskProducerHandle  // Task handler
  );

  xTaskCreate(
    vTaskConsumer,
    "Consumer Task (Print and Free)",
    2048, // Stack in words
    NULL,
    2, // Higher Priority
    &xTaskConsumerHandle  // Task Handler
  );
}

void loop(){
  vTaskDelete(NULL);
}