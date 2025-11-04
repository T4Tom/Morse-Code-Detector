#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"

//Define pin numbers
#define R 13 
#define G 12 
#define B 11 

#define BRIGHTNESS 5
#define LOOP_SLEEP 10
#define MAX_COLOUR_VALUE 255
#define MAX_PWM_LEVEL 65535

void setup_rgb()
{
    // Tell the LED pins that the PWM is in charge of its value.
    gpio_set_function(R, GPIO_FUNC_PWM);
    gpio_set_function(G, GPIO_FUNC_PWM);
    gpio_set_function(B, GPIO_FUNC_PWM);

    // Figure out which slice we just connected to the LED pin, this is done for the other colors below
    uint slice_num = pwm_gpio_to_slice_num(R);
    // Get the defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(G);
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(B);
    pwm_init(slice_num, &config, true);
}

void turn_off() {
    pwm_set_gpio_level(R, ~(MAX_PWM_LEVEL * 0 / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
    pwm_set_gpio_level(G, ~(MAX_PWM_LEVEL * 0 / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
    pwm_set_gpio_level(B, ~(MAX_PWM_LEVEL * 0 / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
}

void show_green() {
    turn_off();
    pwm_set_gpio_level(G, ~(MAX_PWM_LEVEL * MAX_COLOUR_VALUE / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
}

void show_red() {
    turn_off();
    pwm_set_gpio_level(R, ~(MAX_PWM_LEVEL * MAX_COLOUR_VALUE / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
}

