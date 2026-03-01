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
        update();
    }
    if(!(PINB & (1<<6))){
        ASYNC(generator, increase, 1);
        update();
    }
    if(!(PINB & (1<<4))){
        ASYNC(generator, save_or_load, 0);
        update();
    }
}

void joystick_pressed_PCINT1(){
    if(!(PINE & (1<<2))){
        if (left_freq){
            return;
        }
        switch_freq();
        switch_arrows();
        update();
    }
    if(!(PINE & (1<<3))){
        if (!left_freq){
            return;
        }
        switch_freq();
        switch_arrows();
        update();
    }
}

void update(){
    if (left_freq){
        printAt(frequency, 0);
    }else {
        printAt(frequency, 4);
    }
}
