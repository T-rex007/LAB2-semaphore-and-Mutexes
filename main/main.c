#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"
#include "semphr.h"
#include "driver/hw_timer.h"
SemaphoreHandle_t xSemaphore;
TaskHandle_t gpio_on_handle;
TaskHandle_t gpio_off_handle;
TaskHandle_t print_status_handle;
static const char *TAG = "Main";
static const char *TAG_PRINT = "Status";
static const char *TAG_OFF = "Off Task";
static const char *TAG_ON = "On Task";

#define GPIO_OUT_NUM0 2
#define GPIO_PIN_SEL 1ULL<<GPIO_OUT_NUM0


static void uGpioOnTask(void *arg){
    TickType_t xtime1;
    for (;;){
        if (xSemaphore != NULL){
            if(xSemaphoreTake(xSemaphore, 10) ==pdTRUE){
                xtime1 = xTaskGetTickCount();
                gpio_set_level(GPIO_OUT_NUM0, 1);
                while(pdMS_TO_TICKS(500)>(xTaskGetTickCount()- xtime1)){
                    continue;
                }
                xSemaphoreGive(xSemaphore);
                vTaskDelay(1000/portTICK_RATE_MS);
            }
            else{
                ESP_LOGI(TAG_ON, "Another task has the Mutex");
            }
        }
        else{
            ESP_LOGI(TAG_ON, "Mutex was not created");
        }
    }
}

static void uGpioOffTask(void *arg){
    TickType_t xtime1;
    for (;;){
        if (xSemaphore != NULL){
            if(xSemaphoreTake(xSemaphore, 10) ==pdTRUE){
                xtime1 = xTaskGetTickCount();
                gpio_set_level(GPIO_OUT_NUM0, 0);
                while(pdMS_TO_TICKS(500)>xTaskGetTickCount()- xtime1){
                    continue;
                }
                xSemaphoreGive(xSemaphore);
                vTaskDelay(1000/portTICK_RATE_MS);
            }
            else{
                ESP_LOGI(TAG_OFF, "Another task has the Mutex.");
            }
        }
        else{
            ESP_LOGI(TAG_OFF, "Mutex was not created.");
        }
    }
}

static void printStatus( void *arg){
    for (;;){
        if(gpio_get_level(GPIO_OUT_NUM0) == 1){
            ESP_LOGI(TAG_PRINT, "ON");
            vTaskDelay(1000/portTICK_RATE_MS);
        }
        else{
            ESP_LOGI(TAG_PRINT, "OFF");
            vTaskDelay(1000/portTICK_RATE_MS);
        }
    }
}


void app_main(void){
    gpio_config_t io_conf; 
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
    xSemaphore = xSemaphoreCreateMutex();
    // Priority Inheritance Manage mutex- combinations 
    // xTaskCreate(printStatus, "printStatus", 2048, NULL, 9, &print_status_handle    );
    // xTaskCreate(uGpioOnTask, "uGpioOnTask", 2048, NULL, 11, &gpio_on_handle);
    // xTaskCreate(uGpioOffTask, "uGpioOffTask", 2048, NULL, 10, &gpio_off_handle);


    // xTaskCreate(printStatus, "printStatus", 2048, NULL, 9, &print_status_handle    );
    // xTaskCreate(uGpioOnTask, "uGpioOnTask", 2048, NULL, 10, &gpio_on_handle);
    // xTaskCreate(uGpioOffTask, "uGpioOffTask", 2048, NULL,11, &gpio_off_handle);



    // xTaskCreate(printStatus, "printStatus", 2048, NULL, 10, &print_status_handle    );
    // xTaskCreate(uGpioOnTask, "uGpioOnTask", 2048, NULL, 9, &gpio_on_handle);
    // xTaskCreate(uGpioOffTask, "uGpioOffTask", 2048, NULL, 11, &gpio_off_handle);


    // xTaskCreate(printStatus, "printStatus", 2048, NULL, 11, &print_status_handle    );
    // xTaskCreate(uGpioOnTask, "uGpioOnTask", 2048, NULL, 9, &gpio_on_handle);
    // xTaskCreate(uGpioOffTask, "uGpioOffTask", 2048, NULL, 10, &gpio_off_handle);

    // xTaskCreate(printStatus, "printStatus", 2048, NULL, 11, &print_status_handle    );
    // xTaskCreate(uGpioOnTask, "uGpioOnTask", 2048, NULL, 10, &gpio_on_handle);
    // xTaskCreate(uGpioOffTask, "uGpioOffTask", 2048, NULL, 9, &gpio_off_handle);
    
    xTaskCreate(printStatus, "printStatus", 2048, NULL, 10, &print_status_handle    );
    xTaskCreate(uGpioOnTask, "uGpioOnTask", 2048, NULL, 11, &gpio_on_handle);
    xTaskCreate(uGpioOffTask, "uGpioOffTask", 2048, NULL, 9, &gpio_off_handle);

 

    ESP_LOGI(TAG, "Setup is finished");
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(10));
    };
}