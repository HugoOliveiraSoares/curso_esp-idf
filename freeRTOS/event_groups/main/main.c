#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/portmacro.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

EventGroupHandle_t eventGroupConectividade;
const int conexaoWifi = BIT0;
const int conexaoMQTT = BIT1;

SemaphoreHandle_t ligarMQTT;

void conectaWifi(void* params) {
  while (1) {
    ESP_LOGI("WIFI", "Conectado ao Wifi");
    xEventGroupSetBits(eventGroupConectividade, conexaoWifi);
    xSemaphoreGive(ligarMQTT);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

void conectaMQTT(void* params) {
  while (true) {
    xSemaphoreTake(ligarMQTT, portMAX_DELAY);
    ESP_LOGI("MQTT", "Conectado ao MQTT");
    xEventGroupSetBits(eventGroupConectividade, conexaoMQTT);
  }
}

void processa_dados(void* params) {
  while (1) {
    xEventGroupWaitBits(eventGroupConectividade, conexaoWifi | conexaoMQTT,
                        true, true, portMAX_DELAY);
    printf("\n Procesando dados\n\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI("TASK", "Desconecta da internet\n");
  }
}

void app_main(void) {
  eventGroupConectividade = xEventGroupCreate();
  ligarMQTT = xSemaphoreCreateBinary();

  xTaskCreate(&conectaWifi, "Conecta ao Wifi", 2028, NULL, 1, NULL);
  xTaskCreate(&conectaMQTT, "Conecta ao servidor mqtt", 2028, NULL, 1, NULL);
  xTaskCreate(&processa_dados, "Processa dados", 2028, NULL, 1, NULL);
}
