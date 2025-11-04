
// --------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------

#define BUZZER_PIN      15 
#define C3              130
#define E3              165
#define F3              175
#define G3              196     
#define A3              220    
#define B3              247    
#define C4              262    
#define C5             523
#define G4           392
#define E4         330


/**
 * Compute (and set) PWM (pulse-width modulation) frequency and duty cycle for the specified frequency.
 * The return value is the computed PWM wrap value (which needn't necessarily be used).
 */
unsigned int pwm_set_freq_duty( // provided by skeleton
    /** The PWM generator "slice" to output the requested frequency on. */
    unsigned int slice,

    /** The PWM generator channel to output the requested frequency on. */
    unsigned int channel,

    /** The desired frequency. */
    unsigned int frequency,

    /**
     * The duty cycle (expressed as a percentage - e.g., 0.5 for 50% duty cycle)
     * For a buzzer, the duty cycle affects only the volume (from 0%, silence, up to 50%, max volume).
     * (50% to 100% is the same as 0% to 50%).
     */
    double duty
);

// --------------------------------------------------------------------

/**
 * Called once at the start of the program to initialize all of the GPIO
 * pins responsible for the buzzer.
 */
void buzzer_init(); // provided by skeleton

/**
 * Disable the buzzer.
 */
void buzzer_disable(); // provided by skeleton

/**
 * Enable the buzzer at the specified frequency.
 */
void buzzer_enable(unsigned int frequency); // provided by skeleton

void playNote(unsigned int frequency); // provided by skeleton


void morseBeep();

void morseDash();

void buzzer_errorp1();

void buzzer_errorp2();

void morseSilence();

void buzzer_goodbye();

void buzzer_success();

void buzzer_startup();