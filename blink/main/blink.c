#include <driver/gpio.h>
#include <stdio.h>

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "sdkconfig.h"

#define LED 18

void app_main(void) {
  printf("Hello world!!!\n");

  gpio_reset_pin(LED);

  gpio_set_direction(LED, GPIO_MODE_OUTPUT);

  while (1) {
    gpio_set_level(LED, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(LED, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
