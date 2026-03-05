#include "../TinyTimber.h"
#include "../waves/generator.h"
#ifndef joystick_H
#define joystick_H

typedef struct{
    Object self;
    Generator *pulsePointer;
    Generator *left;
    Generator *right;
    bool left_freq;
} Joystick;

void joystick_init(Joystick *self);
void left_or_right(Joystick *self, bool left_freq);
void joystick_pressed_PCINT0(Joystick *self);
void joystick_pressed_PCINT1(Joystick *self);

#define initJoystick(left, right){initObject(), left, left, right, true}

#endif