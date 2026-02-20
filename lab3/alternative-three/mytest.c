#include "tinythreads.h"
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void updateReg(volatile uint8_t *reg, uint8_t high, uint8_t value);
bool is_prime(long i);
uint16_t count_return();
uint16_t count_increase();
uint8_t state_toggle_for_blink();
uint8_t state_toggle_for_button();

// Store the lcdscc of every valid input in a sorted array,
// Makes it possible to easily retrive ch by index and later store it in var seg
static const uint16_t lcdscc[10]= {
    0x1551, // '0'
    0x0110, // '1'
    0x1E11, // '2'
    0x1B11, // '3'
    0x0B50, // '4'
    0x1B41, // '5'
    0x1F41, // '6'
    0x0111, // '7'
    0x1F51, // '8'
    0x1B51, // '9'
};
void writeChar(int ch, int pos){
    /* Check pos and ch value is within allowed interval */
    if (pos < 0 || pos > 5){
        return;
    }
    if (ch < ('0' + 0) || ch > ('0' + 9)) {
        return;
    }
     
    uint16_t seg = lcdscc[ch - '0'];
    volatile uint8_t *base_reg;

    // Depending on what position we want to write on,
    // a switch case on pos is used to determine the right registry
    switch (pos) {
        case 0: base_reg = &LCDDR0; break;
        case 1: base_reg = &LCDDR0; break;
        case 2: base_reg = &LCDDR1; break;
        case 3: base_reg = &LCDDR1; break;
        case 4: base_reg = &LCDDR2; break;
        case 5: base_reg = &LCDDR2; break;
    }
    for (int i = 0; i < 4; i++) {
        uint8_t nibble = seg & 0x0F;                      // Get the lowest 4 bits
        updateReg(base_reg + (i * 5), (pos % 2), nibble); // Jump by 5 each time
        seg = seg >> 4;                                   // Shift to the next nibble
    }
}

void updateReg(volatile uint8_t *reg, uint8_t high, uint8_t value) {
    if (high) {
        *reg &= 0x0F;          // clear upper nibble
        *reg |= value << 4;    // write digit
    } else {
        *reg &= 0xF0;          // clear lower nibble
        *reg |= value;
    }
}

// Return true if prime, else false
bool is_prime(long i){
    for(long n=2; n<i; n++){
        if(i % n == 0){
            return false;
        }
    }
    return true;
}

int pp=0;
mutex m = MUTEX_INIT;

void printAt(long num, int pos) {
    lock(&m);
    pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    volatile int i=0;
    while(i<1000){
        i++;
    }
    pp++;
    writeChar( num % 10 + '0', pp);
    unlock(&m);
}

void computePrimes(int pos) {
    long n;
    for(n = 1; ; n++) {
        if (is_prime(n)) {
            printAt(n, pos);
        }
    }
}
void blink(int state){
    if (state == 0){
        LCDDR18 = 0x1;
    } else {
        LCDDR18 = 0x0;
    }
}
#define LCDDR2_MASK ((1 << 1) | (1 << 2))
void button(int state){
    PORTB = (1 << 7);
    count_increase();
    printAt(count_return(), 4);
    if(state == 0){
        LCDDR2 = (LCDDR2 & ~((1 << 1) | (1 << 2))) | (1 << 1);
    } else {
        LCDDR2 = (LCDDR2 & ~((1 << 1) | (1 << 2))) | (1 << 2);
    }
}
ISR(TIMER1_COMPA_vect){
    spawn(blink, state_toggle_for_blink());
}

ISR(PCINT1_vect){
    if(!(PINB & (1<<7))){
        spawn(button, state_toggle_for_button());
    }
}

int main() {
    LCD_Init();
    spawn(blink, 0);
    spawn(button, 0);
    computePrimes(0);
}
