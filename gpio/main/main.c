#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

#define LED1 2
#define LED2 18
#define LED3 19
#define LED4 21
#define BOTAO1 15
#define BOTAO2 23

void app_main(void) {
  // Configuração dos pinos dos LEDs
  gpio_reset_pin(LED1);
  gpio_reset_pin(LED2);
  gpio_reset_pin(LED3);
  gpio_reset_pin(LED4);

  // Configura os pinos dos LEDs como output
  gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED3, GPIO_MODE_OUTPUT);
  gpio_set_direction(LED4, GPIO_MODE_OUTPUT);

  // Configuração dos pinos dos botoes
  gpio_reset_pin(BOTAO1);
  gpio_reset_pin(BOTAO2);

  // Configuração dos pinos dos botoes como input
  gpio_set_direction(BOTAO1, GPIO_MODE_INPUT);
  gpio_set_direction(BOTAO2, GPIO_MODE_INPUT);
  // Configura o resistor de pulldown para o botão (por padrão a esntrada estaá
  // em zero)
  gpio_pulldown_en(BOTAO1);
  // Desabilita o resistor de pull-up por segurança
  gpio_pullup_dis(BOTAO1);

  for (int i = 0; i <= 8; i++) {
    gpio_set_level(LED2, (i & 1) >> 0);
    gpio_set_level(LED3, (i & 2) >> 1);
    gpio_set_level(LED4, (i & 4) >> 2);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  while (1) {
    int estado_botao = gpio_get_level(BOTAO1);
    gpio_set_level(LED1, estado_botao);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
