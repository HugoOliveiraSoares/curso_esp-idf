#ifndef ESP_NOW_HANDLER_H
#define ESP_NOW_HANDLER_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_err.h"

esp_err_t init_wifi();
esp_err_t init_esp_now();
esp_err_t register_peer(uint8_t *peer_addr);

esp_err_t esp_now_send_data(uint8_t *peer_addr, uint8_t *data, uint8_t len);
#endif  // !ESP_NOW_HANDLER_H
