/*
 * Borda de subida:   GPIO_INTR_POSEDGE
 * Borda de descida:  GPIO_INTR_NEGEDGE
 * Qualquer borda:    GPIO_INTR_ANYEDGE
 * Alto:              GPIO_INTR_HIGH_LEVEL
 * Baixo:             GPIO_INTR_LOW_LEVEL
 * Desabilita:        GPIO_INTR_DISABLE
 * */

#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

#define LED1 2
#define BOTAO1 15
#define BOTAO2 23

QueueHandle_t filaDeInterrupcao;

static void IRAM_ATTR isr_button(void* params) {
  int pino = (int)params;
  xQueueSendFromISR(filaDeInterrupcao, &pino, NULL);
}

void trata_interrupcao_botao(void* params) {
  int pino;
  int contador = 0;

  while (1) {
    if (xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY)) {
      // De-bouncing
      int estado = gpio_get_level(pino);
      if (estado == 1) {
        gpio_isr_handler_remove(pino);
        while (gpio_get_level(pino)) {
          vTaskDelay(50 / portTICK_PERIOD_MS);
        }

        contador++;
        printf("Os botões foram acionados %d vezes. Botão precionado: %d\n",
               contador, pino);
        // Habilitar novamentoe a interrupção
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_isr_handler_add(pino, isr_button, (void*)pino);
      }
    }
  }
}

void app_main(void) {
  gpio_reset_pin(LED1);

  gpio_set_direction(LED1, GPIO_MODE_OUTPUT);

  gpio_reset_pin(BOTAO1);
  gpio_reset_pin(BOTAO2);

  gpio_set_direction(BOTAO1, GPIO_MODE_INPUT);
  gpio_set_direction(BOTAO2, GPIO_MODE_INPUT);
  // Configura o resistor de pulldown para o botão (por padrão a esntrada estaá
  // em zero)
  gpio_pulldown_en(BOTAO1);
  gpio_pulldown_en(BOTAO2);
  // Desabilita o resistor de pull-up por segurança
  gpio_pullup_dis(BOTAO1);
  gpio_pullup_dis(BOTAO2);

  // Configura pino para interrupção
  gpio_set_intr_type(BOTAO1, GPIO_INTR_POSEDGE);
  gpio_set_intr_type(BOTAO2, GPIO_INTR_POSEDGE);

  filaDeInterrupcao = xQueueCreate(10, sizeof(int));
  xTaskCreate(trata_interrupcao_botao, "TrataBotao", 2048, NULL, 1, NULL);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BOTAO1, isr_button, (void*)BOTAO1);
  gpio_isr_handler_add(BOTAO2, isr_button, (void*)BOTAO2);
}
