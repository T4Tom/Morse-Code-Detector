#include "../includes/buzzer.h"
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
//#include "pico/multicore.h"

void buzzer_init() { // provided by skeleton
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
}
// disable the buzzer pin to stop the sound
void buzzer_disable() { // provided by skeleton
    gpio_deinit(BUZZER_PIN);
}

void buzzer_enable(unsigned int frequency) { // provided by skeleton
    // Set the frequency and duty cycle for the specified pin to the specified frequency.
    pwm_set_freq_duty(
        pwm_gpio_to_slice_num(BUZZER_PIN),          // PWM Slice (comptued from pin)
        pwm_gpio_to_channel(BUZZER_PIN),            // PWM Channel (computed from pin)
        frequency,                                  // Frequency in Hz (as specified)
        0.1                                        // Duty cycle = 0.1 (max volume = 0.5)
    );

    // Mark the slice associated with the BUZZER_PIN enabled.
    pwm_set_enabled(pwm_gpio_to_slice_num(BUZZER_PIN), true);
}

// --------------------------------------------------------------------

unsigned int pwm_set_freq_duty( // provided by skeleton
    unsigned int slice,
    unsigned int channel,
    unsigned int frequency,
    double duty
) {

    // The Raspberry Pi Pico is clocked at 125Mhz.
    unsigned int clock = 125000000;
    
    // Compute the divider (multiplied by 16).
    // An 8-bit value is up to 256 (which is the integer limit for our divider), multiplied
    // again by 16 (256 * 16) gives us a multiple of 4096 for the frequency.
    unsigned int divider16 = ceil(clock / (frequency * (double) 4096));

    // Ensure the divider is no lower than 16.
    if (divider16 < 16) divider16 = 16;
    
    // The wrap (period) (number of cycles to count up to) can be computed as
    // clock (divided by divider) divided by the frequency (minus one because wrap
    // is the maximum cycle number, not the number of cycles).
    //
    // (This is computed as "clock * 16 / divider16" to avoid having to cast
    //  and divide the divider, but it is mathematically equivalent.)
    unsigned int wrap = (clock * 16 / divider16 / frequency) - 1;

    // Naturally, divider16 / 16 is the integer part of the divider, whilst
    // the argument after is the fractional part of the divider. It is a 4-bit (up to 16)
    // value, and 0xF, 15 in hexadecimal, represents the maximum possible 4-bit value.
    // Hence, bitwise-ANDing the divider with 0xF gives us the 4-bit fractional part of the
    // divider.
    pwm_set_clkdiv_int_frac(slice, divider16 / 16, divider16 & 0xF);

    // Set the wrap and level for the specified slice.
    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, channel, wrap * duty);

    // Return the wrap value for (optional) use by the calling program.
    return wrap;

}


void playNote(unsigned int frequency) { // provided by skeleton
	// Play the specified frequency for 1 second.
	buzzer_enable(frequency);
	sleep_ms(1000);
	
}


void morseBeep() { // the beep for the dot representation
    buzzer_init();
    buzzer_enable(250);
    
}

void morseDash() { // the beep for the dash representation
    buzzer_init();
    buzzer_enable(500);
}

void morseSilence(){ // wrapper for muting the buzzer
    buzzer_disable();
}

void buzzer_errorp1(){ // make a sound for the first part of the error
    buzzer_init();
    buzzer_enable(C4);
}

void buzzer_errorp2(){ // make a sound for the second part of the error
    buzzer_init();
    buzzer_enable(G3);
}

void buzzer_goodbye(){ // goodbye jingle
    buzzer_init();
    buzzer_enable(C4);
    sleep_ms(500);
    buzzer_enable(E3);
    sleep_ms(500);
    buzzer_enable(G3);
    sleep_ms(500);
    buzzer_enable(C4);
    sleep_ms(500);
    buzzer_disable();
}

void buzzer_success(){ // success jingle
    buzzer_init();
    buzzer_enable(C4);
    sleep_ms(200);
    buzzer_enable(E4);
    sleep_ms(200);
    buzzer_enable(G4);
    sleep_ms(200);
    buzzer_enable(E4);
    sleep_ms(200);
    buzzer_enable(C5);
    sleep_ms(300);
    buzzer_disable();
}

void buzzer_startup() { // boot up tune
    buzzer_init();
    buzzer_enable(C3);
    sleep_ms(400);
    buzzer_enable(E3);
    sleep_ms(400);
    buzzer_enable(G3);
    sleep_ms(400);
    buzzer_enable(E3);
    sleep_ms(400);
    buzzer_enable(C4);
    sleep_ms(400);
    buzzer_disable();
}
