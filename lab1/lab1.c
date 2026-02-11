#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void updateReg(volatile uint8_t *reg, uint8_t high, uint8_t value);
bool is_prime(long i);
void writeLong(long i);
void partIV(void);

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
    if (ch < 0 || ch > 9) {
        return;
    }
     
    uint16_t seg = lcdscc[ch];
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

void primes(){
    for (long i=10;i<100000000000000;i++){
        if (is_prime(i)){
            writeLong(i);
        };
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
void writeLong(long i){
    uint8_t digit;

    for (int x = 5; x >= 0; x--) {
        digit = i % 10;
        writeChar(digit, x);
        i /= 10;

        if (i == 0) break;
    }
}
void blink(){
    bool state = false;
    // Prescaling factor 256
    TCCR1B = (1 << CS12);
    uint16_t increment = 31250/2;
    while(true){
        while(TCNT1 - increment < 31250);
        if (!state){
            LCDDR0 = (1 << 1) | (1 << 2);
            state = true;
        } else {
            LCDDR0 = 0;
            state = false;
        }
        increment += 31250/2;
         
    }
}

void button(){
    PORTB = (1 << 7);
    bool state = false;
    while(true){
        // Loop indefinality until input is detected
        while(PINB & (1 << 7));
        // Loop indefinality until button is released
        while(!(PINB & (1 << 7)));
        
        if(!state){
            LCDDR2 = 0;
            LCDDR0 = (1 << 5) | (1 << 6);
            state = true;
        } else {
            LCDDR0 = 0;
            LCDDR2 = (1 << 1) | (1 << 2);
            state = false;
        }
    }
}

void partIV(void){
    PORTB = (1 << 7);
    bool stateButton = false;
    bool stateBlink = false;
    // Prescaling factor 256
    TCCR1B = (1 << CS12);
    uint16_t increment = 31250/2;
    long i = 2;
    while(true){
        // Blink priority 1
        while(TCNT1 - increment < 31250);
        if (!stateBlink){
            LCDDR0 = (1 << 1) | (1 << 2);
            stateBlink = true;
        } else {
            LCDDR0 = 0;
            stateBlink = false;
        }
        increment += 31250/2;
        
        if(!(PINB & (1<<7))){
            while(!(PINB & (1 << 7))){
                while(TCNT1 - increment < 31250);
                if (!stateBlink){
                    LCDDR0 = (1 << 1) | (1 << 2);
                    stateBlink = true;
                } else {
                    LCDDR0 = 0;
                    stateBlink = false;
                }
                increment += 31250/2;
                if (is_prime(i) && i < 10000){
                    writeLong(i);
                }   
            }
                if(!stateButton){
                    LCDDR3 = 0;
                    LCDDR8 = (1 << 0);
                    stateButton = true;
                } else{
                    LCDDR8 = 0;
                    LCDDR3 = (1 << 0);
                    stateButton = false;
                }
        }
        
        if (is_prime(i) && i < 10000){
            writeLong(i);
        }
        i+=1;
    }
}

int main(void){
    LCD_Init();
    //writeChar(8,2);
    //primes();
    //blink();
    //partIV();
    button();
}