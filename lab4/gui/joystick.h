#include "../TinyTimber.h"
#include "../waves/generator.h"
#ifndef joystick_H
#define joystick_H

typedef struct{
    Object self;
    Generator pulsePointer;
} Joystick;

void joystick_init();
void left_or_right(Joystick *self, bool left_freq);
void joystick_pressed_PCINT0();
void joystick_pressed_PCINT1();

#define initJoystick(pulsePointer){initObject(), pulsePointer}

#endif