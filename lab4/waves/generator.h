#include "../TinyTimber.h"
#include "writer.h"
#ifndef generator_H
#define generator_H
typedef struct {
    Object super;
    int state;
    int frequency;
    int previous;
    int bit;
    PortWriter *pw;
    Msg msg;
} Generator;

int waveGenerator (Generator *self, int arg);
int increase      (Generator *self, int arg);
int decrease      (Generator *self, int arg);
int save          (Generator *self, int arg);
int load          (Generator *self, int arg);
int save_or_load  (Generator *self, int arg);
int read_frequency(Generator *self, int arg);

#define initGenerator(bit, pw) {initObject(), 0, 0, 0, bit, pw}

#endif