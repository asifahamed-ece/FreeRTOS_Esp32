#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

// Declare the Pin
static const int ledpin = 8;  // Blinking Built_In LED for ESP32-C3, GPIO 8
#define rate1 500
#define rate2 333


// Create Tasks

// Task 1
void ToggleLED1(void *parameters){
    while(1){
        Serial.println("Hello from Task 1");
        digitalWrite(ledpin, HIGH);
        vTaskDelay(rate1 / portTICK_PERIOD_MS);
        digitalWrite(ledpin, LOW);
        vTaskDelay(rate1 / portTICK_PERIOD_MS);
    }
}

// Task 2
void ToggleLED2(void *parameters){
    while(1){
        Serial.println("Hello from Task 2");
        digitalWrite(ledpin, HIGH);
        vTaskDelay(rate2 / portTICK_PERIOD_MS);
        digitalWrite(ledpin, LOW);
        vTaskDelay(rate2 / portTICK_PERIOD_MS);
    }
}

void setup(){
    Serial.begin(115200);
    pinMode(ledpin, OUTPUT);

    xTaskCreatePinnedToCore(
        ToggleLED1,
        "Toggle LED 1",
        1024,
        NULL,
        1,
        NULL,
        app_cpu
    );

    xTaskCreatePinnedToCore(
        ToggleLED2,
        "Toggle LED 2",
        1024,
        NULL,
        1,
        NULL,
        app_cpu
    );
}

void loop(){
    // We don't need to do anything in the loop() function, as our LED blinking is handled by the ToggleLED Task. 
}