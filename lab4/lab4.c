#include <stdbool.h>
#include "TinyTimber.h"
#include "gui/graphics.h"
#include "gui/joystick.h"
#include "gui/lcd.h"

PortWriter pw     = initPortWriter();
Generator left    = initGenerator(4, &pw);
Generator right   = initGenerator(6, &pw);
GUI gui           = initGRAPHICS(&left, &right);
Joystick joystick = initJoystick(&left, &right, &gui);

int start_app(GUI *obj, int arg) {
    LCD_Init();
    joystick_init();
    port_init();
    
    // Set initial frequencies
    left.frequency = 0;
    right.frequency = 0;
    
    // Start display
    update(&gui);
    switch_arrows(false);
    
    // Start pulse generators
    ASYNC(&left, waveGenerator, 0);
    ASYNC(&right, waveGenerator, 0);
    
    return 0;
}

int main(void){
    INSTALL(&joystick, joystick_pressed_PCINT0, IRQ_PCINT0);
    INSTALL(&joystick, joystick_pressed_PCINT1, IRQ_PCINT1);
    return TINYTIMBER(&gui, start_app, 0);
}