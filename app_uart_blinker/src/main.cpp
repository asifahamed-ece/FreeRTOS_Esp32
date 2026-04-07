// A FreeRTOS COde which Controls the Blinking of an LED on an ESP32 Board. The Blink Rate can be Updated Dynamically via Serial Input.
// The Code is Designed to Run on a Single Core of the ESP32, Demonstrating Task Management and Inter-task Communication in FreeRTOS.

#include <Arduino.h>
#include <stdlib.h>

// Setting up the CPU Core as UniCore from Dual Archi of Esp32.

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
  #endif

// Defining Pin and Rates for Blinking

const int ledpin = 2;
int rate = 1000;  // Default Blink Rate (1 sec).

void SerialTask(void *parameters){
  while(1){
    if(Serial.available() > 0){
      String input = Serial.readStringUntil('\n');
      input.trim(); //Remove any Leading or Trailing Whitespace
      int newrate  = input.toInt(); // Convert String to Integer
      rate = newrate > 0 ? newrate : rate; // Update Rate if Valid, else keep previous
      Serial.print("Blink Rate Updated To: ");
      Serial.print(rate);
      Serial.println(" ms");
    }
    vTaskDelay(100 / portTICK_PERIOD_MS); // Short Delay to Prevent Task Starvation
  }
}

void BlinkTask(void *parameters){
  while(1){
    digitalWrite(ledpin, HIGH);
    vTaskDelay(rate / portTICK_PERIOD_MS);
    digitalWrite(ledpin, LOW);
    vTaskDelay(rate / portTICK_PERIOD_MS);
  }
}

// Setup Function
void setup(){
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);

  Serial.print("Blinking at Defaul Rate: ");
  Serial.print(rate);
  Serial.println(" ms");

// Create Tasks
  xTaskCreatePinnedToCore(
    SerialTask,           // Function to implement the task
    "Serial Input Task", // Name of the task
    2048,                // Stack size in words
    NULL,               // Task input parameter
    2,                 // Priority of the task
    NULL,               // Task handle.
    app_cpu              // Core where the task should run
  );


  xTaskCreatePinnedToCore(
    BlinkTask,
    "Blink Task",
    1024,
    NULL,
    1, // Lower Priority than Serial Input Task.
    NULL,
    app_cpu
  );
}

void loop()
{
   // Empty Loop as Tasks are Running Independently
}