#include <stdio.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "pwm.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO (5) // Define the output GPIO
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_14_BIT // Set duty resolution to 14 bits
#define LEDC_DUTY (8191)                // Set duty to 50%. ((2 ** 14) - 1) * 50% = 4095
#define LEDC_FREQUENCY (1000)           // Frequency in Hertz. Set frequency at 5 kHz
#define DUTY_MAX (16383)                // Max duty value

// static const char *TAG = "PWM";
static bool mode_pwm = false;

void pwm_init(void)
{
    mode_pwm = false;
    gpio_reset_pin(LEDC_OUTPUT_IO);
}

void pwm_start(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY, // Set output frequency at LEDC_FREQUENCY kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // printf("PWM init\n");
}

void pwm_set_freq(uint16_t freq)
{
    // printf("PWM freq: %u\n", freq);

    if (freq)
    {
        if (!mode_pwm)
        {
            mode_pwm = true;
            gpio_reset_pin(LEDC_OUTPUT_IO);
            pwm_start();
        }

        ledc_timer_config_t ledc_timer = {
            .speed_mode = LEDC_MODE,
            .timer_num = LEDC_TIMER,
            .duty_resolution = LEDC_DUTY_RES,
            .freq_hz = freq, // Set output frequency at LEDC_FREQUENCY kHz
            .clk_cfg = LEDC_AUTO_CLK};
        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    }
    else
    {
        if (mode_pwm)
        {
            mode_pwm = false;
            ledc_stop(LEDC_MODE, LEDC_CHANNEL, 0);
        }
    }
}

void pwm_set_duty(uint16_t duty)
{
    // printf("PWM duty: %u mode_pwm: %d\n", duty, mode_pwm);

    if (mode_pwm)
    {
        if (duty > DUTY_MAX)
            duty = DUTY_MAX;
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
    }
    else
    {
        gpio_reset_pin(LEDC_OUTPUT_IO);
        gpio_set_direction(LEDC_OUTPUT_IO, GPIO_MODE_OUTPUT);
        gpio_set_level(LEDC_OUTPUT_IO, duty);
    }
}
