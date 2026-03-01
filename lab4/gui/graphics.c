#include "lab4/gui/graphics.h"
#include "lcd.c"

int start_gui(GUI *self){
    lcd_init();
}

bool switch_freq(GUI *self, bool left_freq){
    return left_freq != left_freq;
}