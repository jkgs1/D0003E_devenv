#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "joystick.h"

void joystick_init(){
    PORTB = (1 << 7);
    EIMSK = (1 << PCIE1);
    PCMSK1 = (1 << PCINT15);
    enabled = 1;
}

void joystick_pressed_PCINT0(){
    if(!(PINB & (1<<7))){
        ASYNC(generator, decrease, 1);
    }
    if(!(PINB & (1<<6))){
        ASYNC(generator, increase, 1);
    }
    if(!(PINB & (1<<4))){
        ASYNC(generator, save_or_load, 0);
    }
}