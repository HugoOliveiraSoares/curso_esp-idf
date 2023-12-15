/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <driver/gpio.h>
#include <esp_log.h>
#include <stdio.h>

#include <>

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "sdkconfig.h"

#define LED 18

void task1(void* params) {
  while (true) {
    // printf("Leitura de sensores\n");
    ESP_LOGI("TASK1", "Leitura de sensores\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task2(void* params) {
  while (true) {
    // printf("Escrever dados no display\n");
    ESP_LOGI("TASK2", "Escrever dados no display\n");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void app_main(void) {
  printf("Hello world!!!\n");

  xTaskCreate(&task1, "leitura", 2048, "Task 1", 1, NULL);
  xTaskCreate(&task2, "display", 2048, "Task 2", 1, NULL);

  gpio_reset_pin(LED);

  gpio_set_direction(LED, GPIO_MODE_OUTPUT);

  int estado = 0;
  while (1) {
    gpio_set_level(LED, estado);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    estado = !estado;
  }
}
