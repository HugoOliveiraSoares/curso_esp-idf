#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

SemaphoreHandle_t semaforoBin;

void conexao_servidor(void* params) {
  while (1) {
    ESP_LOGI("Servidor", "Conectado ao servidor");
    xSemaphoreGive(semaforoBin);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void processa_dados(void* params) {
  while (1) {
    xSemaphoreTake(semaforoBin,
                   portMAX_DELAY);  // aguarda um tempo indeterminado;
    printf("Página carregada!\n");
  }
}

void app_main(void) {
  semaforoBin = xSemaphoreCreateBinary();
  xTaskCreate(&conexao_servidor, "Conexao com o servidor", 2048, NULL, 1, NULL);
  xTaskCreate(&processa_dados, "Processa dados", 2048, NULL, 1, NULL);
}
