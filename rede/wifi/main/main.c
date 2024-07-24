#include <stdio.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/portmacro.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"

void app_main(void) {
  // Inicializar o NVS
  ESP_ERROR_CHECK(nvs_flash_init());

  // Inicializar a interface de Rede (Network Interface)
  ESP_ERROR_CHECK(esp_netif_init());

  // Inicializa a tarefa padrão de tratamento de eventos
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  // chama da biblioteca de exemplo do wifi
  example_connect();
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  example_disconnect();
}
