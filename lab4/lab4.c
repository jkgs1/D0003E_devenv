#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "TinyTimber.h"
#include "gui/graphics.h"
#include "gui/joystick.h"
#include "gui/lcd.h"
#include "waves/generator.h"
#include "waves/writer.h"

PortWriter pw     = initPortWriter();
Generator left    = initGenerator(4, pw);
Generator right   = initGenerator(6, pw);
Joystick joystick = initJoystick(&left, &right);
GUI gui           = initGRAPHICS(&left, &right);

int main(void){
    LCD_Init();
    joystick_init(&joystick);
    //port_init();
    INSTALL(&joystick, joystick_pressed_PCINT0, IRQ_PCINT0);
    INSTALL(&joystick, joystick_pressed_PCINT1, IRQ_PCINT1);
    return TINYTIMBER(&gui, update, 0);
}