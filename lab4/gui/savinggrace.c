#include "savinggrace.h"
#include "../TinyTimber.h"

// Magic functions that let us not disable interupts on the joystick handlers directly

int saveusgod(SavingGrace *self, int arg) {
    ASYNC(self->joy, joystick_pressed_PCINT1, 0);
    return 0;
}
int mosmeny(SavingGrace *self, int arg) {
    ASYNC(self->joy, joystick_pressed_PCINT0, 0);
    return 0;
}
