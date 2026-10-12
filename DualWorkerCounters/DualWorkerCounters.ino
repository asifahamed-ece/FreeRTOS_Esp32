# include <Arduino.h>
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"

struct workerConfig{
    const char *label;
    uint32_t delayMs;
};

static const workerConfig configA = {"A", 500};
static const workerConfig configB = {"B", 1000};

TaskHandle_t taskHandleA = nullptr;
TaskHandle_t taskHandleB = nullptr;

void WorkerTask (void *vParameters) {
    const workerConfig *config = static_cast<const workerConfig *> (vParameters);

    unsigned long counter = 0;

    for (;;)
    {
        Serial.printf("Worker %s : %lu\n", config->label, counter);
        
        counter++;

        vTaskDelay(pdMS_TO_TICKS(config->delayMs)); 
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    BaseType_t resultA = xTaskCreate(
        WorkerTask,
        "Worker A",
        4096,
        (void *) &configA,
        1,
        &taskHandleA
    );

    BaseType_t resultB = xTaskCreate(
        WorkerTask,
        "Worker B",
        4096,
        (void *) &configB,
        1,
        &taskHandleB
    );

    if(resultA != pdPASS ){
        Serial.println("Task A creation failed!");
    }
}

void loop()
{
    // Nothing in Loop
}