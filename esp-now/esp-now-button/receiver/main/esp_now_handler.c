#include "esp_now_handler.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_interface.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "nvs_flash.h"

#define ESP_CHANNEL 1
#define TAG "ESP-NOW-RECEIVER"

esp_err_t init_wifi() {
  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "Wifi init completed");
  return ESP_OK;
}

esp_err_t init_esp_now(esp_now_recv_cb_t recv_cb_t,
                       esp_now_send_cb_t send_cb_t) {
  esp_now_init();
  esp_now_register_recv_cb(recv_cb_t);
  esp_now_register_send_cb(send_cb_t);

  ESP_LOGI(TAG, "Esp now init completed");
  return ESP_OK;
}

esp_err_t register_peer(uint8_t *peer_addr) {
  esp_now_peer_info_t esp_now_peer_info = {};
  memcpy(esp_now_peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
  esp_now_peer_info.channel = ESP_CHANNEL;
  esp_now_peer_info.ifidx = ESP_IF_WIFI_STA;

  esp_now_add_peer(&esp_now_peer_info);
  return ESP_OK;
}

esp_err_t esp_now_send_data(uint8_t *peer_addr, uint8_t *data, uint8_t len) {
  esp_now_send(peer_addr, data, len);

  return ESP_OK;
}
