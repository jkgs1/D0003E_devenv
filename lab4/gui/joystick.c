#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "joystick.h"
#include "lcd.h"
#include "graphics.h"

void joystick_init(Joystick *self){
    PORTB |= (1 << 7);
    EIMSK |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT15);
}
void left_or_right(Joystick *self, bool left_freq){
    if (left_freq){
        self->pulsePointer = self->left;
    } else{
        self->pulsePointer = self->right;
    }
    self->left_freq = left_freq;
}

void joystick_pressed_PCINT0(Joystick *self){
    if(!(PINB & (1<<7))){
        ASYNC(self->pulsePointer, decrease, 1);
        ASYNC(self->pulsePointer, update, self->left_freq);
    }
    if(!(PINB & (1<<6))){
        ASYNC(self->pulsePointer, increase, 1);
        ASYNC(self->pulsePointer, update, self->left_freq);
    }
    if(!(PINB & (1<<4))){
        ASYNC(self->pulsePointer, save_or_load, 0);
        ASYNC(self->pulsePointer, update, self->left_freq);
    }
}

void joystick_pressed_PCINT1(Joystick *self){
    if(!(PINE & (1<<2))){
        if (self->left_freq){
            return;
        }
        left_or_right(self, true);
        switch_arrows(self->left_freq);
        ASYNC(self->pulsePointer, update, self->left_freq);
    }
    if(!(PINE & (1<<3))){
        if (!self->left_freq){
            return;
        }
        left_or_right(self, false);
        switch_arrows(self->left_freq);
        ASYNC(self->pulsePointer, update, self->left_freq);
    }
}

