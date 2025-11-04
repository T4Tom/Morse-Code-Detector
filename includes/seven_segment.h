/* Seven-segment display (5161BS) */
// http://www.xlitx.com/datasheet/5161BS.pdf

// (Raspberry Pi Pico pinout: https://datasheets.raspberrypi.com/pico/Pico-R3-A4-Pinout.pdf)

#pragma once




/** Top-center horizontal bar. */
#define SEGMENT_A            7  // Pin 10 (GPIO 07) // provided by skeleton

/** Top-right vertical bar */
#define SEGMENT_B            6  // Pin 09 (GPIO 06) // provided by skeleton

/** Bottom-right vertical bar */
#define SEGMENT_C           20  // Pin 26 (GPIO 20) // provided by skeleton

/** Bottom-center horizontal bar */
#define SEGMENT_D           19  // Pin 25 (GPIO 19) // provided by skeleton

/** Bottom-left vertical bar */
#define SEGMENT_E           18  // Pin 24 (GPIO 18) // provided by skeleton

/** Top-left vertical bar */
#define SEGMENT_F            8  // Pin 11 (GPIO 08) // provided by skeleton

/** Center horizontal bar */
#define SEGMENT_G            9  // Pin 12 (GPIO 09) // provided by skeleton

/** An array containing the pins for all segments. */
static const unsigned int ALL_SEGMENTS[] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G}; // provided by skeleton

#define ALL_SEGMENTS_COUNT  7 // provided by skeleton

uint8_t values[] = { // this is a sorted list of the values for each letter of the alphabet for usage in the seven segment display
    0b11101110,  // A index 0
    0b00111110,  // B index 1
    0b10011100,  // C index 2
    0b01111010,  // D index 3
    0b10011110,  // E index 4
    0b10001110,  // F index 5
    0b11110110,  // G index 6
    0b01101110,  // H index 7
    0b00001100,  // I index 8
    0b01111000,  // J index 9
    0b01101110,  // K index 10 (same as H)
    0b00011100,  // L index 11
    0b10101000,  // M index 12
    0b00101010,  // N index 13
    0b11111100,  // O index 14
    0b11001110,  // P index 15
    0b11100110,  // Q index 16
    0b00001010,  // R index 17
    0b10110110,  // S index 18
    0b00011110,  // T index 19
    0b01111100,  // U index 20
    0b00111000,  // V index 21
    0b01010100,  // W index 22
    0b01101110,  // X index 23 (same as H)
    0b01110110,  // Y index 24
    0b11011010,  // Z index 25
};




/* Initialise the GPIO pins for the seven-segment display. */
void seven_segment_init() { // provided by skeleton
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_init(ALL_SEGMENTS[i]);
        gpio_set_dir(ALL_SEGMENTS[i], GPIO_OUT);
    }    
}

/* Turn off the seven-segment display. */
void seven_segment_off() { // provided by skeleton
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_put(ALL_SEGMENTS[i], true);
    }
}
// Used in init
void seven_segment_on(){ // provided by skeleton
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_put(ALL_SEGMENTS[i], false);
    }
}

void seven_segment_show(unsigned int number) { // provided by skeleton
    for (unsigned int i = 0; i < 8; i++) {
        unsigned int segmentBit = 1 << (7 - i);
        bool illuminateSegment = (segmentBit & values[number]);
        gpio_put(
            ALL_SEGMENTS[i],
            !illuminateSegment
        );
    }
}