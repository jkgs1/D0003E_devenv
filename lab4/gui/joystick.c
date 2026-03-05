#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "joystick.h"

void joystick_init(Joystick *self){
    PORTB = (1 << 7);
    EIMSK = (1 << PCIE1);
    PCMSK1 = (1 << PCINT15);
}
void left_or_right(Joystick *self, bool left_freq){
    if (left_freq){
        self->pulsePointer = pulsePointer->left;
    } else{
        self->pulsePointer = pulsePointer->right;
    }
}

void joystick_pressed_PCINT0(){
    left_or_right(left_freq);
    if(!(PINB & (1<<7))){
        ASYNC(&pulsePointer, decrease, 1);
        ASYNC(&pulsePointer, update, left_freq);
    }
    if(!(PINB & (1<<6))){
        ASYNC(&pulsePointer, increase, 1);
        ASYNC(&pulsePointer, update, left_freq);
    }
    if(!(PINB & (1<<4))){
        ASYNC(&pulsePointer, save_or_load, 0);
        ASYNC(&pulsePointer, update, left_freq);
    }
}

void joystick_pressed_PCINT1(){
    if(!(PINE & (1<<2))){
        if (left_freq){
            return;
        }
        switch_freq();
        switch_arrows(left_freq);
        ASYNC(&pulsePointer, update, left_freq);
    }
    if(!(PINE & (1<<3))){
        if (!left_freq){
            return;
        }
        switch_freq();
        switch_arrows(left_freq);
        ASYNC(&pulsePointer, update, left_freq);
    }
}

