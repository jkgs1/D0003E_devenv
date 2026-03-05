#ifndef LCD_H
#define LCD_H

void LCD_Init();
void writeChar(int ch, int pos);
void updateReg(volatile uint8_t *reg, uint8_t high, uint8_t value);
void printAt(long num, int pos);

#endif