# com1031cw

## How to setup

See fritzing diagram and original_diagram.png for the wiring diagram

Effectively, combine the wiring of the two images

## How to Run

Same method as in labs

build using `pico build` then run `pico debug`

## Adding

Commit any changes to the main branch, just make sure it works properly before commiting.


## Functionality 

The code does as follows:

initialisation for each of the pins etc 

Check if button is pressed:
    enter "decoder" function:
        check if the button is pressed:
            add 1 to the pressed counter
        else:
            add 1 to the break counter

        if the break counter is greater than the amount required to escape:
            assume end of letter
            decode the letter
            add the letter to the message

if the there is 4 letters which are valid: 
    ask if we should reset            

## Reading the code

Anything highlighted as being "provided by skeleton" has been code which was provided in the brief or extracted from the previous labs
since the code used in the labs is still applicable


### Highlights of each file

__src/buzzer.c:__

The majority of the functions are purely from the lab provided files
with the exception being in the functions at the bottom such as morseDash
and buzzer_goodbye to name but 2.

__src/potentiometer.c:__

No changes, directly downloaded from the lab for potentiometers

__src/morse_code.c:__

This is the main code file for everything. The code is commented as much
as possible to try and make it readable at a glance

__includes/buzzer.h:__

Left the same as provided except adding more function prototypes (e.g. `void morseBeep();`)
so the compiler is aware of the functions.

As well as adding some musical notes to the definitions

__includes/potentiometer.h:__

Left as provided

__includes/seven_segment.h:__

The addition of the values[] array is an alphabetical array.

Everything else is the same.

__fritzing/*:__

These are the files used for setting up the project when needed

First, follow the wiring of `original_diagram.png`, followed by `addons.png`

picosetup.fzz is for adding more to the diagrams
