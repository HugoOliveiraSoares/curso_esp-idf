#include <stdio.h>

#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"

#define LED1 2
#define LED2 18

void app_main(void) {
  // configuração do timer

  ledc_timer_config_t timer_config = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_8_BIT,
      .timer_num = LEDC_TIMER_0,
      .freq_hz = 1000,
      .clk_cfg = LEDC_AUTO_CLK,
  };

  // inicializar o timer
  ledc_timer_config(&timer_config);

  // Configuração do canal
  ledc_channel_config_t channel_config = {
      .gpio_num = LED1,
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .channel = LEDC_CHANNEL_0,
      .timer_sel = LEDC_TIMER_0,
      .duty = 0,
      .hpoint = 0,
  };

  ledc_channel_config(&channel_config);

  ledc_channel_config_t channel_config1 = {
      .gpio_num = LED2,
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .channel = LEDC_CHANNEL_1,
      .timer_sel = LEDC_TIMER_0,
      .duty = 0,
      .hpoint = 0,
  };

  ledc_channel_config(&channel_config1);

  // Utilizando o pwm
  ledc_fade_func_install(0);
  while (1) {
    for (int i = 0; i < 255; i++) {
      ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
      ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    for (int i = 255; i > 0; i--) {
      ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
      ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0, 1000,
                                 LEDC_FADE_WAIT_DONE);

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 255, 1000,
                                 LEDC_FADE_WAIT_DONE);
  }
}
