#ifndef SAVINGGRACE_H
#define SAVINGGRACE_H
#include "joystick.h"

typedef struct {
    Object super;
    Joystick *joy;
} SavingGrace;

int saveusgod(SavingGrace *self, int arg);
int mosmeny(SavingGrace *self, int arg);

#define initSavingGrace(joy){initObject(), joy}
#endif