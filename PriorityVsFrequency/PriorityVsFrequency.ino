# include <Arduino.h>
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"

struct workerConfig{
    const char *label;
    uint32_t delayMs;
};

BaseType_t app_cpu = xPortGetCoreID(); // Get the current CPU core ID

static const workerConfig configA = {"A", 500};
static const workerConfig configB = {"B", 1000};

TaskHandle_t taskHandleA = nullptr;
TaskHandle_t taskHandleB = nullptr;

void WorkerTask (void *vParameters) {
    const workerConfig *config = static_cast<const workerConfig *> (vParameters);

    unsigned long counter = 0;

    for (;;)
    {
        Serial.printf("[%lu ms] Worker %s | Core: %d | Counter: %lu\n", millis(), config->label, (int)xPortGetCoreID(), counter);
        
        counter++;

        vTaskDelay(pdMS_TO_TICKS(config->delayMs)); 
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.printf("Configured worker core: %d\n", (int)app_cpu);

    Serial.printf("setup() executing on core: %d\n",
                (int)xPortGetCoreID());

    BaseType_t resultA = xTaskCreatePinnedToCore(
        WorkerTask,
        "Worker A",
        4096,
        (void *) &configA,
        1,
        &taskHandleA,
        app_cpu
    );

    BaseType_t resultB = xTaskCreatePinnedToCore(
        WorkerTask,
        "Worker B",
        4096,
        (void *) &configB,
        3,                      // Higher priority than task A
        &taskHandleB,
        app_cpu
    );

    if(resultA != pdPASS ){
        Serial.println("Task A creation failed!");
    }
    else if(resultB != pdPASS ){
        Serial.println("Task B creation failed!");
    }
}

void loop()
{
    // Nothing in Loop
}