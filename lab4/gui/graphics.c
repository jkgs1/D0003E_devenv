#include "graphics.h"
#include "lcd.c"

bool switch_freq(GUI *self, bool left_freq){
    return left_freq != left_freq;
}

void update(Generator *self, bool left_freq){
    if (left_freq){
        printAt(self->frequency, 0);
    }else {
        printAt(self->frequency, 4);
    }
}