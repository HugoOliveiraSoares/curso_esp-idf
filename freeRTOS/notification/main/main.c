#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

static TaskHandle_t receptorHandle = NULL;
static TaskHandle_t emissorHandle = NULL;

void emissor(void* params) {
  while (1) {
    xTaskNotifyGive(receptorHandle);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void receptor(void* params) {
  while (1) {
    int quantidade = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    printf("Notificações recebidas %d\n", quantidade);
  }
}

void app_main(void) {
  xTaskCreate(&receptor, "Receptor", 2048, NULL, 2, &receptorHandle);
  xTaskCreate(&emissor, "Emissor", 2048, NULL, 2, &emissorHandle);
  xTaskCreate(&emissor, "Emissor2", 2048, NULL, 2, &emissorHandle);
}
