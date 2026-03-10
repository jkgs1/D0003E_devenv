#include "writer.h"
#include <avr/io.h>

void port_init(){
    DDRE |= (1 << 4) | (1 << 6);
}

void portWriter(int bit){
    PORTE ^= (1<<bit);   
}

void portWriter_clear(int bit){
    PORTE &= ~(1<<bit);
}