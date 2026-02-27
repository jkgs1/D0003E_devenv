#ifndef LCD_H
#define LCD_H

void lcd_init();
void writeChar(char ch, int pos);
void updateReg(volatile uint8_t *reg, uint8_t high, uint8_t value);
void printAt(long num, int pos);

#endif