#include "led_resource.h"

static struct led_resource board_A_led = { 
    .pin = GROUP_PIN(1,10),
};

struct led_resource * get_led_source(void)
{
    return &board_A_led;
}