#include <driver/gpio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "esp_now_handler.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "nvs_flash.h"

#define LED_V 22
#define LED_A 23

#define TAG "ESP-NOW-RECEIVER_MAIN"

typedef struct data_pin {
  int value_1;
  int value_2;
} data_pin;

static void recv_cb(const esp_now_recv_info_t *esp_now_info, uint8_t *data,
                    int data_len) {
  data_pin *pin_data = (data_pin *)data;

  ESP_LOGI(TAG, "Data received " MACSTR " %d %d",
           MAC2STR(esp_now_info->src_addr), pin_data->value_1,
           pin_data->value_2);

  gpio_set_level(LED_V, pin_data->value_1);
  gpio_set_level(LED_A, pin_data->value_2);
}

static void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    ESP_LOGI(TAG, "ESP_NOW_SEND_SUCCESS");
  } else {
    ESP_LOGW(TAG, "ESP_NOW_SEND_FAIL");
  }
}

void app_main(void) {
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Configuração dos pinos dos botoes
  gpio_reset_pin(LED_V);
  gpio_reset_pin(LED_A);

  gpio_set_direction(LED_V, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED_A, GPIO_MODE_OUTPUT);

  ESP_ERROR_CHECK(init_wifi());
  ESP_ERROR_CHECK(init_esp_now(recv_cb, send_cb));
}
