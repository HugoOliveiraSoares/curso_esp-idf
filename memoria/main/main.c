#include <stdio.h>
#include <string.h>

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"

void task1(void *params) {
  int stack_task = uxTaskGetStackHighWaterMark(NULL);
  ESP_LOGI("STACK TASK", "Tamanho livre na Stack %d", stack_task);
  char array1[8000];
  memset(array1, 1, sizeof(array1));

  stack_task = uxTaskGetStackHighWaterMark(NULL);
  ESP_LOGI("TASK1", "Tamanho livre na Stack %d", stack_task);

  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    char *array;
    array = (char *)malloc(15000 * sizeof(char));
    memset(array, 1, 15000);
    ESP_LOGI("TASK1", "Espaço livre na HEAP %d", xPortGetFreeHeapSize());
    free(array);
  }
}

void app_main(void) {
  int quantidade = 3;
  float valor = 15.0;

  ESP_LOGI("RAM", "Espaço livre na HEAP - Pré-alocação   %db",
           xPortGetFreeHeapSize());

  printf("Quantidade: %d | Valor: %f \n", quantidade, valor);

  // Alocação de memória HEAD
  ESP_LOGI("RAM", "Espaço livre na HEAP - Pré-alocação   %db",
           xPortGetFreeHeapSize());

  char *array;
  array = (char *)malloc(2048 * sizeof(char));
  if (array == NULL) {
    ESP_LOGE("RAM", "Falha na alocação de memória");
  } else {
    memset(array, 1, 2048);
    for (int i = 0; i < 2048; i++) {
      quantidade += array[i];
    }
  }

  printf("Quantidade %d\n", quantidade);
  ESP_LOGI("RAM", "Espaço livre na HEAP - Pós-alocação  %d b",
           xPortGetFreeHeapSize());
  free(array);

  ESP_LOGI("RAM", "Espaço livre na HEAP - Pós-desalocação  %d b",
           xPortGetFreeHeapSize());

  int DRAM = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  int IRAM = heap_caps_get_free_size(MALLOC_CAP_32BIT) - DRAM;

  ESP_LOGI("RAM", "Data RAM:  %d b", DRAM);
  ESP_LOGI("RAM", "Instruction RAM  %d b", IRAM);

  // stack
  int stack_main = uxTaskGetStackHighWaterMark(NULL);
  ESP_LOGI("STACK", "Tamanho livre na Stack %d", stack_main);

  xTaskCreate(&task1, "tarefa", 10000, NULL, 1, NULL);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  ESP_LOGI("RAM", "Espaço livre na HEAP - Após iniciar TASK %d b",
           xPortGetFreeHeapSize());
}
