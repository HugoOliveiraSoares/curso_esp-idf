#include <driver/gpio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_now_handler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "nvs_flash.h"

#define BOTAO1 15
#define BOTAO2 23

#define LED1 16
#define LED2 17

static uint8_t peer_mac[6] = {0x94, 0x3c, 0xc6, 0x28, 0x10, 0x00};

typedef struct data_pin {
  int value_1;
  int value_2;
} data_pin;

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
  gpio_reset_pin(BOTAO1);
  gpio_reset_pin(BOTAO2);

  gpio_set_direction(BOTAO1, GPIO_MODE_INPUT);
  gpio_set_direction(BOTAO2, GPIO_MODE_INPUT);

  gpio_reset_pin(LED1);
  gpio_reset_pin(LED2);

  gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED2, GPIO_MODE_OUTPUT);

  gpio_pulldown_en(BOTAO1);
  gpio_pullup_dis(BOTAO1);

  gpio_pulldown_en(BOTAO2);
  gpio_pullup_dis(BOTAO2);

  ESP_ERROR_CHECK(init_wifi());
  ESP_ERROR_CHECK(init_esp_now());
  ESP_ERROR_CHECK(register_peer(peer_mac));

  data_pin data;

  while (1) {
    data.value_1 = gpio_get_level(BOTAO1);
    data.value_2 = gpio_get_level(BOTAO2);

    gpio_set_level(LED1, data.value_1);
    gpio_set_level(LED2, data.value_2);

    ESP_ERROR_CHECK(
        esp_now_send_data(peer_mac, (uint8_t*)&data, sizeof(data_pin)));
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
