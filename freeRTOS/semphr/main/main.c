#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

float temperatura;
SemaphoreHandle_t mutexI2C;

float acessa_i2c(int comando) {
  if (comando == 1) {
    ESP_LOGI("I2C", "Leitura do sensor de Temperatura");
    return 20.0 * ((float)rand() / (float)(RAND_MAX / 10));
  } else {
    ESP_LOGI("I2C", "Escrita do LCD");
    printf("Tela LCD - Temperatura = %f", temperatura);
  }

  return 0;
}

void le_sensor(void* params) {
  while (1) {
    if (xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS)) {
      temperatura = acessa_i2c(1);
      ESP_LOGI("LEITURA", "Temperatura: %f", temperatura);
      xSemaphoreGive(mutexI2C);
    } else {
      ESP_LOGE("LEITURA", "Não foi possivel ler o sensor");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void lcd_display(void* params) {
  while (1) {
    if (xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS)) {
      ESP_LOGI("DISPLAY", "Escreve no LCD");
      acessa_i2c(2);
      xSemaphoreGive(mutexI2C);
    } else {
      ESP_LOGE("DISPLAY", "Não foi possivel escrever no display");
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void app_main(void) {
  mutexI2C = xSemaphoreCreateMutex();
  xTaskCreate(&le_sensor, "Leitura Sensor 1", 2048, NULL, 2, NULL);
  xTaskCreate(&lcd_display, "Atualização do DISPLAY", 2048, NULL, 2, NULL);
}
