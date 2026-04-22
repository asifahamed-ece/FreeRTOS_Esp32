#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE
const BaseType_t app_cpu = 0;
#else
const BaseType_t app_cpu = 1;
#endif

// Binary Semaphore Example for FreeRTOS on ESP32
static SemaphoreHandle_t binSem;

static SemaphoreHandle_t SerialMutex;

void WaitingTask(void *Parameters){
  while(1){
    Serial.println("[Waiting Task]: Attempting to Take Semaphore...");

    // Unblock when Semaphore is Aquired by the Task.
    if(xSemaphoreTake(binSem, portMAX_DELAY) == pdTRUE){

      if(xSemaphoreTake(SerialMutex, portMAX_DELAY) == pdTRUE){
        Serial.println("[Waiting Task]: Semaphore Aquired, Event Occured !\n");

        xSemaphoreGive(SerialMutex);
      }
      // Semaphore variable is 0 Now as its been taken.
    }
  }
}

void GivingTask(void *Paramters){
  while(1){
    vTaskDelay(pdMS_TO_TICKS(5000)); // Wait for 5 seconds then Give Semaphore

    if(xSemaphoreTake(SerialMutex, portMAX_DELAY) == pdTRUE){
        Serial.println("[Giving Task]: Giving Semaphore...");
        xSemaphoreGive(SerialMutex);
      }

    xSemaphoreGive(binSem); // Semaphore Set to 1, Can be taken by the Other Task Now.
  }
}

void setup(){
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(2000));  // Delay for Serial to Settle 

  Serial.print("\n\nBinary Semaphore Example\n\n");

  vTaskDelay(pdMS_TO_TICKS(1000));

  Serial.print("Program Started Execution on Core: ");
  Serial.println(xPortGetCoreID());

  binSem = xSemaphoreCreateBinary(); // Create a Bin Semaphore with Initial value '0'

  // Create a Serial Mutex for Protecting Serial Flooding.
  SerialMutex = xSemaphoreCreateMutex();

  if(SerialMutex == NULL){
    Serial.println("Serial Mutex Creation Failed!");
  }

  // Creating Tasks 
  xTaskCreatePinnedToCore(
    WaitingTask,
    "Waiting Task",
    1024,
    NULL,
    1, 
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    GivingTask,
    "Giving Task",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  Serial.println("Setup complete. WaitingTask will block until GivingTask gives...\n");
}

void loop(){

}