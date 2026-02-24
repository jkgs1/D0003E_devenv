#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void updateReg(volatile uint8_t *reg, uint8_t high, uint8_t value);
int main(void){
    LCD_Init();
}