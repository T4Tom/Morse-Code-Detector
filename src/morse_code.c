#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "../includes/seven_segment.h"
#include "../includes/buzzer.h"
#include "../includes/potentiometer.h"
#include "../includes/rgb_led.h"

// #include "pico/multicore.h"


// All pins numbers are from https://pico.pinout.xyz/

// This is the pin in which the morse code will be inputted
// it is also the right button
#define BUTTON_PIN_MORSE 16 // Pin 21 (GPIO 16)

// used in determining if the user wants to continue or not
#define BUTTON_PIN_LEFT 17 // physical pin 22

#define TOO_LONG_TIME 700
#define ESCAPE_TIME 400
#define DOT_TIME 250
#define CHAR_OFFSET 65

const char *morseCode[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--.."}; // morse code for A-Z in order, therefore morseCode[0] = ".-", morseCode[1] = "-...", etc. where A = 0, B = 1 ...

uint32_t pressed;
char decoder();
char determineChar(char *morse);
int determineAllottedTime();

int main() {
    timer_hw->dbgpause = 0; // provided by skeleton
    stdio_init_all(); // provided by skeleton

    
    // buzzer logic

    buzzer_init(); // provided by skeleton
    
    // Initialise the RGB LED
    setup_rgb();

    // Initialise the seven segment display.
    // as per the mark scheme, the seven segment display should be turned on when the program starts, and do a "boot" style of startup with all segments on
    //Green light during boot
    
    seven_segment_init();
    seven_segment_on(); 
    show_green();
    sleep_ms(1000);
    turn_off();
    // reset the seven segment display to off after our "boot"
    seven_segment_off();
    buzzer_startup();


    // Initialise the button's GPIO pin.
    gpio_init(BUTTON_PIN_MORSE); // this is the right hand button
    gpio_set_dir(BUTTON_PIN_MORSE, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_MORSE); // Pull the button pin towards ground (with an internal pull-down resistor).

    gpio_init(BUTTON_PIN_LEFT); // this button is obviously the left hand button and is used for the "continue or terminate" logic after 4 valid letters
    gpio_set_dir(BUTTON_PIN_LEFT, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_LEFT);

    printf("Welcome to the Morse Code interpreter\n");
    printf("Press the button to enter Morse Code\n");



    
    // potentiometer logic

    potentiometer_init(); // provided by skeleton

    int allowed_time = determineAllottedTime(); // This uses the potentimeter to determine the time allowed for the entire morse code input
    printf("Allowed time: %dms\n", allowed_time); // print time in ms for the user to see
    
    char decoded_string[4]; // make an array to store the decoded string
    decoded_string[4] = '\0'; // null terminate the string

    for (int i = 0; i < 4;) { // for 4 runs of the loop, since we only want 4 letters inputted
        while (true) { // loop forever until we break out of it
            if (i == 4) { // if we have entered 4 letters successfully
                buzzer_success(); // function for making a success sound, there is also a small delay so that the user doesn't accidently exit the program
                sleep_ms(500);
                seven_segment_off();

                printf("Output: %s\n", decoded_string); // print the entire string

                printf("You have entered 4 letters, would you like to continue?\n");
                printf("Left to continue, right to terminate\n");
                while (true) { // kepe on looping until the user presses the left or right button
                    if (gpio_get(BUTTON_PIN_LEFT)) {
                        printf("Continuing\n");
                        show_green(); //show green for a second to indicate a continue
                        i = 0; // reset to initial state to keep going
                        sleep_ms(1000);
                        turn_off();
                        break; // break out of the while true
                    }
                    else if (gpio_get(BUTTON_PIN_MORSE)) {
                        printf("Terminating\n");
                        show_red(); //show red when exiting
                        buzzer_goodbye(); // goodbye buzzer noise
                        sleep_ms(1000);
                        turn_off();
                        
                        return 0; // kill the program, return no error code (0 is a success)
                    }
                }
            }

            // first time round need to light up the seven segment display

            // check for if the button is pressed

            if (gpio_get(BUTTON_PIN_MORSE)) { // this is run effectively all the time until the user presses the button, to start the morse code input process

                char letter; // make a letter variable to store the letter that is decoded at the end

                // decoder is initiated on the start for every single button press for a letter
                // however it will hold the button until the letter has been deciphered

                letter = decoder(allowed_time); // go to the decoder function to get the letter 
                seven_segment_off(); // make sure the display is reset, so that segments don't overlap by accident
                printf("%c\n", letter); // print the decoded letter to the console
                if (letter != '?') { // if the letter is not an error we can display it
                    // CHAR_OFFSET is used to convert the ASCII value of the letter to the index of the values array
                    // A = 65, therefore A - 65 = 0, B = 66 etc.
                    seven_segment_show(letter - CHAR_OFFSET);
                    show_green();
                    decoded_string[i++] = letter; // add the letter to the decoded string for the final output
                    sleep_ms(400);
                    turn_off();
                }
                else { // the char is an error, therefore make an error noise 
                    seven_segment_on(); // an error constists all segments being on
                    show_red();
                    buzzer_errorp1(); // the first tune of the sound
                    sleep_ms(500);
                    buzzer_errorp2(); // the second tune of the sound
                    sleep_ms(500);
                    seven_segment_off(); // turn off the display after the error sound
                    buzzer_disable(); // turn off the buzzer after finishing the error sound
                    turn_off();
                }
            }
        }
    }
    return 0; // this will likely never get run, however it is here for completeness

}

char decoder(int allowed_time) {
    static char morse[5]; // Buffer to store Morse code, assuming max length of 5
    int index = 0;        // Index for the Morse code buffer
    uint32_t press_duration = 0; // used for recording how long the button was pressed for (before release)
    uint32_t break_duration = 0; // used for determining if the button was released for enough time to be considered a break/end of a letter

    uint32_t start_of_operating = time_us_32(); // Just the hardware time for when the function was called (used for time limit)

    while (true) {
        if ( (time_us_32() - start_of_operating) > (allowed_time * 1000) ) { // As per the mark scheme, if the time exceeds the allowed time, it should return an error
            // the maths above in words is "if the current time in us - the start of the function is greater than the allowed time in milliseconds * 1000"
            // us is microseconds for reference
            
            printf("Error: Time exceeded\n");
            return '?'; // return our error char '?'
        }

        if (gpio_get(BUTTON_PIN_MORSE)) { 
            // Button is pressed actively
            press_duration += 1; // add "1ms" worth of time to the press duration
            break_duration = 0; // Reset break duration since the button has been pressed
            if (press_duration >= TOO_LONG_TIME) { 

                // See TOO_LONG_TIME for its value (700ms as of writing)
                // as per the mark scheme

                /*
                if the button is pressed longer than 700ms, the serial console
                displays this is an error input; the 7-segment displays ‘8’ for an
                error
                */
                printf("Error: Button pressed for too long\n");
                return '?'; // return our error char '?'
            }
        }
        else {
            // Button is not pressed 'right now'
            if (press_duration > 0) { // there is a undeciphered morse code dash/dot to be processed, since the button was just released 
                // Button was just released and therefore we must determine if it was a dot or a dash
                if (press_duration >= DOT_TIME) { // if the button has been pressed for longer than 250ms, it is a dash
                    morse[index] = '-';
                    index++;
                    // !!!!! should implment multicore stuff to stop delay !!!!!
                    morseDash();
                    sleep_ms(175);

                    morseSilence();
                }
                else { // the button has been released before the 250ms, therefore it is a dot
                    morse[index] = '.';
                    index++;
                    
                    morseBeep();

                    sleep_ms(50);

                    morseSilence();
                }

                press_duration = 0; // Reset press duration
            } 
            // else add "one ms" to break duration for the break count check
            break_duration += 1;
        }
        // Check if break duration exceeds escape time 
        if (break_duration >= ESCAPE_TIME) {
            morse[index] = '\0'; // Null-terminate the Morse code string
            break;
        }

        sleep_ms(1); // Sleep for 1ms since otherwise the loop would be too fast, so we work in 1ms intervals for the entire project
    }

    // Finally run our morse code entered through the determineChar function
    // See determineChar for more information on how it works
    char output = determineChar(morse);
    // Return the letter, or potentially a '?' if the morse code was invalid
    return output;
}

char determineChar(char *morse) { 
    
    // given the letter entered, this will loop through the morse code array
    // until it finds a match
    // if a match is found, we can use the index at which it was found to determine the letter


    for (int i = 0; i < 26; i++) {
        if (strcmp(morse, morseCode[i]) == 0) {
            return 'A' + i; // we can add numbers to char, since it works via ASCII
        }
    }
    printf("Invalid Morse code: %s\n", morse);
    return '?'; // error case
}

// group 80

int determineAllottedTime(){ // Just a wrapper function for readability

    return potentiometer_read(4000); // in ms 
}
