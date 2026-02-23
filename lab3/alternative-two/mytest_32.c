#include "tinythreads.h"
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void updateReg(volatile uint8_t *reg, uint8_t high, uint8_t value);
bool is_prime(long i);
uint16_t count_return();
uint16_t count_increase();


void LCD_Init(void) {
    CLKPR = 0x80;
    CLKPR = 0x00;
    /* Use 32 kHz crystal oscillator */
    /* 1/3 Bias and 1/4 duty. MUX0 & MUX1 Enabled */
    /* SEG0:SEG24 is used as port pins. Enable all segments, LCDPM */
    LCDCRB = (1 << LCDCS) | (1 << LCDMUX1) | (1 << LCDMUX0) | (1 << LCDPM2 | (1 << LCDPM1) | (1 << LCDPM0));
    /* Using 16 as prescaler selection and 8 as LCD Clock Divide */
    /* gives a frame rate of 32 Hz */
    LCDFRR = (1 << LCDCD2) | (1 << LCDCD1) | (1 << LCDCD0);
    /* Output voltage to 3.35 V (all enabled) */
    /* LCDDC[0:2] set to 0 for segment drive time 300μs*/
    LCDCCR = (1 << LCDCC3) | (1 << LCDCC2) | (1 << LCDCC1  | (1 << LCDCC0));
    /* Enable LCD, low power waveform and no interrupt enabled */
    LCDCRA = (1 << LCDEN )| (1 << LCDAB);
}

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
mutex m = MUTEX_INIT;  // printAt
mutex blink_mutex = MUTEX_INIT;  // blink
mutex button_mutex = MUTEX_INIT; // button

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
void blink(int arg){
    (void)arg;
    bool state;
    while(1){
        lock(&blink_mutex);

        if (!state){
            LCDDR18 = 0x1;
            state = true;
        } else {
            LCDDR18 = 0x0;
            state = false;
        }
    }
}
#define LCDDR2_MASK ((1 << 1) | (1 << 2))
void button(int arg){
    (void)arg;
    PORTB = (1 << 7);
    bool state = false;
    while(true){
        lock(&button_mutex);
        count_increase();
        printAt(count_return(), 4);
        if(!state){
            LCDDR2 = (LCDDR2 & ~((1 << 1) | (1 << 2))) | (1 << 1);
            state = true;
        } else {
            LCDDR2 = (LCDDR2 & ~((1 << 1) | (1 << 2))) | (1 << 2);
            state = false;
        }
    }
}
ISR(TIMER1_COMPA_vect){
    unlock(&blink_mutex);
}

ISR(PCINT1_vect){
    if(!(PINB & (1<<7))){
        unlock(&button_mutex);
    }
}

int main() {
    lock(&button_mutex);
    lock(&blink_mutex);
    LCD_Init();
    spawn(blink, 0);
    spawn(button, 0);
    computePrimes(0);
}
