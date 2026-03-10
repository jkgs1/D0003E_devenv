#include "graphics.h"
#include "lcd.h"
#include "joystick.h"

int switch_freq(GUI *self){
    self->left_freq = !self->left_freq;
    return 1;
}
int read_direction(GUI *self) {
    return self->left_freq;
}

int update(GUI *self){
    if (self->left_freq) {
        printAt(SYNC(self->left, read_frequency, 0), 0);
        ASYNC(self->left, waveGenerator, 0);
    } else {
        printAt(SYNC(self->right, read_frequency, 0), 4);
        ASYNC(self->right, waveGenerator, 0);
    }
    return 0;
}