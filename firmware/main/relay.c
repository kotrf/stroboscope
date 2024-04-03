#include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "relay.h"

#define RELAY_IO  48

void relay_init(void)
{
    gpio_reset_pin(RELAY_IO);
    gpio_set_direction(RELAY_IO, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY_IO, false);
}

void relay_set(bool en)
{
    // printf("Relay: %s\n", en ? "ON" : "OFF");
    gpio_set_level(RELAY_IO, en);
}
