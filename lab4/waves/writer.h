#include "../TinyTimber.h"
#ifndef writer_h
#define writer_h

typedef struct{
    Object self;
} PortWriter;

#define initPortWriter(){initObject()}

void port_init();
void portWriter(PortWriter *self, int bit);

#endif