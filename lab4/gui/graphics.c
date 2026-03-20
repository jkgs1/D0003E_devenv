#include "graphics.h"
#include "lcd.h"
#include "joystick.h"
#include "../waves/generator.h"

int switch_freq(GUI *self, int arg){
    self->left_freq = !self->left_freq;
    return 1;
}
int read_direction(GUI *self, int arg) {
    return self->left_freq;
}

int update(GUI *self, int arg){
    if (self->left_freq) {
        printAt(SYNC(self->left, read_frequency, 0), 0);
    } else {
        printAt(SYNC(self->right, read_frequency, 0), 4);
    }
    return 0;
}