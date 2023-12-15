#include <stdio.h>

#include "esp_log.h"
#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "SPIFSS"

void app_main(void) {
  esp_vfs_spiffs_conf_t spiffs_config = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 10,
      .format_if_mount_failed = true,
  };

  esp_vfs_spiffs_register(&spiffs_config);

  FILE *arquivo1 = fopen("/spiffs/arquivo1.txt", "r");

  if (arquivo1 == NULL) {
    ESP_LOGE(TAG, "Não foi possível abrir o arquivo");
  } else {
    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo1) != NULL) {
      printf("%s\n", linha);
    }
    fclose(arquivo1);
  }

  FILE *arquivo2 = fopen("/spiffs/pasta/arquivo2.txt", "r");

  if (arquivo2 == NULL) {
    ESP_LOGE(TAG, "Não foi possível abrir o arquivo");
  } else {
    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo1) != NULL) {
      printf("%s\n", linha);
    }
    fclose(arquivo1);
  }
}
