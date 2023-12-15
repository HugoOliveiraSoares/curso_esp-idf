#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"

int32_t le_valor_nvs() {
  int32_t valor = 0;

  ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

  nvs_handle particao_padrao_handle;
  esp_err_t res_nvs = nvs_open_from_partition(
      "DadosNVS", "armazenamento", NVS_READONLY, &particao_padrao_handle);
  // esp_err_t res_nvs =
  // nvs_open("armazenamento", NVS_READONLY, &particao_padrao_handle);

  if (res_nvs == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGE("LE_VALOR_NVS", "Namespace: armazenamento, não encontrado");
    return -1;
  }

  esp_err_t res = nvs_get_i32(particao_padrao_handle, "contador", &valor);

  if (res == ESP_ERR_NOT_FOUND) {
    ESP_LOGE("LE_VALOR_NVS", "Valor não encontrado");
    return -1;
  } else if (res != ESP_OK) {
    ESP_LOGE("LE_VALOR_NVS", "Erro ao acessar o NVS (%s)\n",
             esp_err_to_name(res));
    return -1;
  }

  ESP_LOGI("LE_VALOR_NVS", "Valor armazenado: %ld\n", valor);
  nvs_close(particao_padrao_handle);
  return valor;
}

void grava_valor_nvs(int32_t valor) {
  ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

  nvs_handle particao_padrao_handle;
  esp_err_t res_nvs = nvs_open_from_partition(
      "DadosNVS", "armazenamento", NVS_READWRITE, &particao_padrao_handle);
  // esp_err_t res_nvs =
  //     nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);

  if (res_nvs == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGE("GRAVA_VALOR_NVS", "Namespace: armazenamento, não encontrado");
  }

  esp_err_t res = nvs_set_i32(particao_padrao_handle, "contador", valor + 1);

  if (res != ESP_OK) {
    ESP_LOGE("GRAVA_VALOR_NVS", "Não foi possivel escrever no NVS (%s)",
             esp_err_to_name(res));
  }

  nvs_commit(particao_padrao_handle);
  nvs_close(particao_padrao_handle);
}

void app_main(void) {
  int32_t valor = 0;
  valor = le_valor_nvs();

  if (valor == -1) {
    valor = 0;
  }
  grava_valor_nvs(valor);
}
