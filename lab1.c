#include <avr/io.h>
#include <util/delay.h>
void LCD_Init(void) {
  /* Use 32 kHz crystal oscillator */
  /* 1/3 Bias and 1/3 duty, SEG21:SEG24 is used as port pins */
  LCDCRB = (1 << LCDCS) | (1 << LCDMUX1) | (1 << LCDPM2);
  /* Using 16 as prescaler selection and 7 as LCD Clock Divide */
  /* gives a frame rate of 49 Hz */
  LCDFRR = (1 << LCDCD2) | (1 << LCDCD1);
  /* Set segment drive time to 125 μs and output voltage to 3.3 V*/
  LCDCCR = (1 << LCDDC1) | (1 << LCDCC3) | (1 << LCDCC2) | (1 << LCDCC1);
  /* Enable LCD, default waveform and no interrupt enabled */
  LCDCRA = (1 << LCDEN);
}
void writeChar(int ch, int pos){
    /* Check pos value is within allowed interval */
    if (pos < 0 || pos > 5){
        return;
    }
    if (ch < 0 || ch > 9) {
        return;
    }
    lcdscc = [
        0x0EE7, // '0'
        0x0060, // '1'
        0x0B6D, // '2'
        0x0B79, // '3'
        0x0590, // '4'
        0x0B59, // '5'
        0x0F59, // '6'
        0x0071, // '7'
        0x0FF9, // '8'
        0x0B79  // '9'
    ]
    const temp = lcdscc[ch];
    switch (pos){
        case 0:

        case 1:

        case 2:

        case 3:

        case 4:

        case 5:

        default:
            return;
    }
}
int main(void){
    LCD_Init();
    LCD_update(1, 1);
}