
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

static const char *TAG = "main";

#define GPIO_OUT_NUM0 0
#define GPIO_PIN_SEL 1ULL<<GPIO_OUT_NUM0






void app_main(void){
    gpio_config_t io_conf; 
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_maks = GPIO_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(io_conf)
    while(1)

}