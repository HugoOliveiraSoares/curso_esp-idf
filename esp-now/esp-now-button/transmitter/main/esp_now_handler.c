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
#define TAG "ESP-NOW-TRANSMITTER"

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

void recv_cb(const esp_now_recv_info_t *esp_now_info, uint8_t *data,
             int data_len) {
  ESP_LOGI(TAG, "Data received" MACSTR "%s", MAC2STR(esp_now_info->src_addr),
           data);
}

void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    ESP_LOGI(TAG, "ESP_NOW_SEND_SUCCESS");
  } else {
    ESP_LOGW(TAG, "ESP_NOW_SEND_FAIL");
  }
}

esp_err_t init_esp_now() {
  esp_now_init();
  esp_now_register_recv_cb(recv_cb);
  esp_now_register_send_cb(send_cb);

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
  ESP_LOGI(TAG, "Data send %s", data);
  esp_now_send(peer_addr, data, len);

  return ESP_OK;
}
