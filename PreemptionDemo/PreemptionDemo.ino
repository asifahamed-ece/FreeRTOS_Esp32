# include <Arduino.h>
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"

TaskHandle_t taskHandleBg = nullptr;
TaskHandle_t taskHandleMon = nullptr;

const uint32_t Bg_delayMs = 400;
const uint32_t Mon_delayMs = 100;


void MonitorTask (void *vParameters) {

    unsigned long iteration = 0;

    for (;;)
    {
        Serial.printf("[ %lu ms] %s : %lu\n",millis(), "Monitor", iteration);
        
        iteration++;

        vTaskDelay(pdMS_TO_TICKS(Mon_delayMs)); 
    }
}

void BackgroundTask (void *vParameters) {

    unsigned long iteration = 0;

    for (;;)
    {
        Serial.printf("[ %lu ms] : BACKGROUND START : iteration : %lu\n", millis(), iteration);

        vTaskDelay(pdMS_TO_TICKS(Bg_delayMs)); 

        Serial.printf("[ %lu ms] : BACKGROUND END : iteration : %lu\n", millis(), iteration);
        iteration++;
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    BaseType_t resultA = xTaskCreate(
        BackgroundTask,
        "Background Task",
        4096,
        nullptr,
        1,
        &taskHandleBg
    );

    BaseType_t resultB = xTaskCreate(
        MonitorTask,
        "Monitor Task",
        4096,
        nullptr,
        3,
        &taskHandleMon
    );

    if(resultA != pdPASS ){
        Serial.println("Task A creation failed!");
    }
    else if (resultB != pdPASS ){
        Serial.println("Task B creation failed!");
    }
}

void loop()
{
    // Nothing in Loop
}