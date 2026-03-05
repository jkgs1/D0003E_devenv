#include "writer.h"
#include <avr/io.h>

void port_init(){
    DDRE |= (1 << 4) | (1 << 6);
}

void portWriter(PortWriter *self, int bit){
    PORTE ^= (1<<bit);   
}