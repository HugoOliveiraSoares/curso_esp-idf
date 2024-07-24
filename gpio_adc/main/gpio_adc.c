#include <stdio.h>

#include "driver/adc.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define JOYSTICK_X ADC1_CHANNEL_3
#define JOYSTICK_Y ADC1_CHANNEL_6
#define JOYSTICK_BOTAO 32

// os pinos 34 a 39 não tem resistores internos de pullup ou pulldown

void app_main() {
  gpio_reset_pin(JOYSTICK_BOTAO);
  gpio_set_direction(JOYSTICK_BOTAO, GPIO_MODE_INPUT);
  gpio_pulldown_en(JOYSTICK_BOTAO);
  gpio_pulldown_dis(JOYSTICK_BOTAO);
  gpio_pullup_en(JOYSTICK_BOTAO);

  adc1_config_width(ADC_WIDTH_BIT_10);  // Configura o conversor AD
  adc1_config_channel_atten(JOYSTICK_X, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(JOYSTICK_X, ADC_ATTEN_DB_11);

  int posicao_x = 0;
  int posicao_y = 0;
  int botao = 1;
  while (true) {
    posicao_x = adc1_get_raw(JOYSTICK_X);
    posicao_y = adc1_get_raw(JOYSTICK_Y);
    botao = gpio_get_level(JOYSTICK_BOTAO);

    posicao_x = posicao_x - 512;
    posicao_y = posicao_y - 512;
    printf("Possição X: %.3d \t Posição Y: %.3d | Botão: %d\n", posicao_x,
           posicao_y, botao);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
