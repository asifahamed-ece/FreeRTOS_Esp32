# include <Arduino.h>
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"

const BaseType_t app_cpu = 0; // Pin Core 0 for the application CPU

TaskHandle_t SamplerHandle = nullptr;

const unsigned long ProcDelayMs = 50U;

void SamplerTask(void *pvParameters)
{
    const unsigned long prevStart = 0;
    const unsigned long SampleNum = 0;
    const unsigned long SampleCounter = 0;

    const bool firstSample = true;

    // Start the Sensor data Sampling
    for(;;)
    {
        const unsigned long newStart = millis();  // Record sample start time
        while( (unsigned long) ( (millis() - newStart) > ProcDelayMs)){
            Serial.printf("Sample %lu | Start: %lu ms | Interval: %lu ms\n", SampleNum, newStart, (firstSample != true) ? (prevStart - newStart) : "0");
        }

        vTaskDelay(pdMS_TO_TICKS(200));
        SampleCounter++;
        prevStart = newStart;

        if(firstSample != false){
            firstSample = false;
        }
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