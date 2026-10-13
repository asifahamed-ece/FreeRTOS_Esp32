# include <Arduino.h>
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"

const BaseType_t app_cpu = 0; // Pin Core 0 for the application CPU

TaskHandle_t SamplerHandle = nullptr;

const unsigned long IdleTimeMs = 200U;
const unsigned long ProcDelayMs = 50U;
const unsigned long samplePeriodMs = IdleTimeMs+ProcDelayMs;

void SamplerTask(void *pvParameters)
{
    unsigned long prevStart = 0;
    unsigned long sampleNum = 0;

    const TickType_t period = pdMS_TO_TICKS(samplePeriodMs);
    TickType_t lastWake = xTaskGetTickCount();

    // Start the Sensor data Sampling
    for(;;)
    {
        unsigned long newStart = millis();  // Record sample start time
        
        unsigned long interval =  (sampleNum > 0) ? (newStart - prevStart) : (unsigned long)0;
        Serial.printf("Sample %lu | Start: %lu ms | Interval: %lu ms\n", sampleNum, newStart, interval);
        
        while( (millis() - newStart) < ProcDelayMs ){
        }

        sampleNum++;
        prevStart = newStart;

        vTaskDelayUntil( &lastWake, period);  // Sleep until next sample ready
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
    vTaskDelete(NULL); // We wont reach here Anyways!
}