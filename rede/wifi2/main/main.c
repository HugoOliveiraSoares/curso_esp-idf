#include <stdio.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "wifi.h"

void app_main(void) {
  // Inicializa o NVS

  // retorno da inicialização
  esp_err_t ret_nvs = nvs_flash_init();

  if (ret_nvs == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret_nvs == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret_nvs = nvs_flash_init();
  }

  ESP_ERROR_CHECK(ret_nvs);

  wifi_start();
}
