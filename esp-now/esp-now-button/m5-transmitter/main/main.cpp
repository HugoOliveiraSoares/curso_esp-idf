#include <driver/gpio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "M5GFX.h"
#include "M5Unified.h"
#include "esp_log.h"
#include "esp_now_handler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "nvs_flash.h"

#define DISPLAY M5.Display

static uint8_t peer_mac[6] = {0x94, 0x3c, 0xc6, 0x28, 0x10, 0x00};

typedef struct data_pin {
  int value_1;
  int value_2;
} data_pin;

extern "C" void app_main(void) {
  M5.begin();
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(init_wifi());
  ESP_ERROR_CHECK(init_esp_now());
  ESP_ERROR_CHECK(register_peer(peer_mac));

  DISPLAY.setRotation(1);
  DISPLAY.setTextColor(GREEN);
  DISPLAY.setTextDatum(middle_center);
  DISPLAY.setTextFont(&fonts::Orbitron_Light_24);
  DISPLAY.setTextSize(1);
  DISPLAY.drawString("ESP NOW Test", DISPLAY.width() / 2, DISPLAY.height() / 2);

  data_pin data;

  while (1) {
    M5.update();
    data.value_1 = (int)M5.BtnA.isPressed();
    data.value_2 = (int)M5.BtnB.isPressed();

    ESP_LOGI("M5", "Data1 %d", data.value_1);
    ESP_LOGI("M5", "Data2 %d", data.value_2);

    ESP_ERROR_CHECK(
        esp_now_send_data(peer_mac, (uint8_t*)&data, sizeof(data_pin)));
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
