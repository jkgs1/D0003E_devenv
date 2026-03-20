#include <stdint.h>
#include <avr/io.h>
#include "joystick.h"
#include "lcd.h"
#include "graphics.h"

void joystick_init(){
    // Joystick pins on AVR Butterfly:
    // UP: PINB6, DOWN: PINB7, LEFT: PINE2, RIGHT: PINE3, PUSH: PINB4
    // Configure as inputs with pull-ups
    PORTB |= (1 << 4) | (1 << 6) | (1 << 7);
    PORTE |= (1 << 2) | (1 << 3);

    // Enable pin change interrupts for PCINT1 (PORTB) and PCINT0 (PORTE)
    EIMSK |= (1 << PCIE1) | (1 << PCIE0);
    PCMSK1 |= (1 << PCINT12) | (1 << PCINT14) | (1 << PCINT15);
    PCMSK0 |= (1 << PCINT2)  | (1 << PCINT3);
}

void left_or_right(Joystick *self, int left_freq){
    if (!left_freq){
        self->pulsePointer = self->left;
    } else{
        self->pulsePointer = self->right;
    }
}

int get_freq_value() {
    return SYNC(&gui, read_direction, 0);
}

int joystick_pressed_PCINT1(Joystick *self, int arg){
    if(!(PINB & (1<<7))){ // DOWN

        ASYNC(self->pulsePointer, decrease, 1);
        ASYNC(&gui, update, 0);
        AFTER(MSEC(500), self, joystick_repeat, 7);

    }
    if(!(PINB & (1<<6))){ // UP

        ASYNC(self->pulsePointer, increase, 1);
        ASYNC(&gui, update, 0);
        AFTER(MSEC(500), self, joystick_repeat, 6);

    }
    if(!(PINB & (1<<4))){ // CENTER
        ASYNC(self->pulsePointer, save_or_load, 0);
        ASYNC(&gui, update, 0);
    }
    return 0;
}

int joystick_repeat(Joystick *self, int bit) {
    if (!(PINB & (1 << bit))) {
        if (bit == 7) ASYNC(self->pulsePointer, decrease, 1);
        if (bit == 6) ASYNC(self->pulsePointer, increase, 1);

        ASYNC(&gui, update, 0);

       AFTER(MSEC(200), self, joystick_repeat, bit);
    }
    return 0;
}

int joystick_pressed_PCINT0(Joystick *self, int arg){
    if(!(PINE & (1<<2))){ // LEFT
        if (get_freq_value() == 1){
            return 0;
        }
        left_or_right(self, get_freq_value());
        ASYNC(&gui, switch_freq, 0);
        switch_arrows(get_freq_value());
        ASYNC(&gui, update, 0);
    }
    if(!(PINE & (1<<3))){ // RIGHT
        if (get_freq_value() == 0){
            return 0;
        }
        left_or_right(self, get_freq_value());
        ASYNC(&gui, switch_freq, 0);
        switch_arrows(get_freq_value());
        ASYNC(&gui, update, 0);
    }
    return 0;
}

