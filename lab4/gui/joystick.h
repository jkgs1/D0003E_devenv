#include "../TinyTimber.h"
#include "../waves/generator.h"
#ifndef joystick_H
#define joystick_H

#include "graphics.h"

typedef struct{
    Object super;
    Generator *pulsePointer;
    Generator *left;
    Generator *right;
    int state;
    GUI *gui;
    Msg msg;
} Joystick;

extern GUI gui;

void joystick_init();
void left_or_right(Joystick *self, int left_freq);
int get_freq_value();
int joystick_pressed_PCINT0(Joystick *self, int arg);
int joystick_pressed_PCINT1(Joystick *self, int arg);
int joystick_repeat(Joystick *self, int arg);

#define initJoystick(left, right, gui){initObject(), left, left, right, 0, gui, NULL}

#endif