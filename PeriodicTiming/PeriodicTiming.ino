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

    bool firstSample = true;

    // Start the Sensor data Sampling
    for(;;)
    {
        unsigned long newStart = millis();  // Record sample start time
        Serial.printf("Sample %lu | Start: %lu ms | Interval: %lu ms\n", SampleNum, newStart, (firstSample != true) ? (newStart - prevStart) : (unsigned long)0);
        
        while( (unsigned long)(millis() - newStart) < ProcDelayMs){
        }

        vTaskDelay(pdMS_TO_TICKS(200));
        SampleNum++;
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