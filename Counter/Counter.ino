#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t counterHandle = nullptr;

void counterTask(void *argument)
{
    unsigned long counter = 0;

    for (;;)
    {
        // TODO 1: Print "Counter: " followed by counter.
        Serial.print("Counter: ");
        Serial.println(counter);

        // TODO 2: Increment counter.
        counter++;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    BaseType_t result = xTaskCreate(
        counterTask,       // Task function
        "Counter",         // Debug name
        4096,              // Stack size: bytes in ESP32's IDF-based port
        nullptr,           // No argument for this exercise
        1,                 // Priority
        &counterHandle     // Store the created task's handle
    );

    if (result != pdPASS)
    {
        Serial.println("Task creation failed");
    }
}

void loop()
{
    delay(1000);
}