#include <Arduino.h>  // Since I am using Arduino Framework inside PlatformIO to write this code, I need to include the Arduino.h header file to access the Arduino functions and definitions.

#if CONFIG_FREERTOS_UNICORE  // If the ESP32 is configured to run FreeRTOS on a single core, we need to specify which core to run our task on. In this case, we will run it on core 0.
  static const BaseType_t app_cpu = 0;
#else  // If the ESP32's Core 0 is used for WiFi and other system tasks, it's better to run our application on Core 1 to avoid conflicts and ensure better performance.
  static const BaseType_t app_cpu = 1;
#endif

//Pins
static const int ledpin = LED_BUILTIN;  // Blinking Built_In LED for ESP32, GPIO 2

// Our Task : Blink an LED
void ToggleLED(void *parameters)
{
  while(1)
  {
    digitalWrite(ledpin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);  // By default portTICK_PERIOD_MS for Esp32 is 1ms
    digitalWrite(ledpin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup(){
  
  // Configure Pin
  pinMode(ledpin, OUTPUT);

  //Create a Task for Blinking our LED
  xTaskCreatePinnedToCore(
    ToggleLED,  // Function that implements the task
    "Toggle LED", // Name of the Task
    1024, // Stack size in words (Minimum 768 for ESP32, but we are using 1024 to be safe)
    NULL, // Paramters to pass to the function (Not used in this case)
    1, //Task Priority (Higher Number means Higehr Priority, we are using 1 for Now )
    NULL, // Task handle (Used to reference the task to other Tasks, not needed in this case)
    app_cpu // Core where we want to run this Task (0 or 1)
  );

  // If this was Vanilla RTOS, we would need to start the scheduler here 
  // using vTaskStartScheduler(), but in Arduino framework, 
  // the scheduler is already started for us, so we don't need to do anything else.
}


// Note that the setup() and loop() functions are Automatically cofiguredto run as Priority 1 Tasks on the Core 0, 
// If we set our Tasks to run on the same core with the same priority, they will be scheduled in a round-robin fashion, meaning they will take turns running.


// Incase we set our Task on a Lower Priority than the setup() and loop() Tasks,
//  it will only run when the setup() and loop() Tasks are not running, which is not ideal for our blinking LED Task, as it needs to run continuously.



void loop(){
  // We don't need to do anything in the loop() function, as our LED blinking is handled by the ToggleLED Task.
}