# include <Arduino.h>
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"

const BaseType_t app_cpu = 0; // Pin Core 0 for the application CPU

TaskHandle_t SamplerHandle = nullptr;

const unsigned long ProcDelayMs = 50U;

void SamplerTask(void *pvParameters)
{
    unsigned long prevStart = 0;
    unsigned long SampleNum = 0;
    unsigned long SampleCounter = 0;

    // Start the Sensor data Sampling
    for(;;)
    {
        unsigned long newStart = millis();  // Record sample start time
        while( (unsigned long) ( (millis() - newStart) > ProcDelayMs)){
            Serial.printf("Sample %lu | Start: %lu ms | Interval: %lu ms\n", SampleNum, newStart, newStart - prevStart);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
        SampleCounter++;
        prevStart = newStart;
    }
}


void setup()
{
    Serial.begin(115200);
    delay(1000);

    BaseType_t resultA = xTaskCreatePinnedToCore(
        SamplerTask,
        "Sampler",
        4096,
        NULL,
        1,
        &SamplerHandle,
        app_cpu
    );

    if (resultA != pdPASS){
        Serial.printf("Task Creation Failed! : Sampler\n");
    }
}

void loop()
{
    delay(1000); // We wont reach here Anyways!
}