#include "../TinyTimber.h"
#include "writer.h"
#ifndef generator_H
#define generator_H
typedef struct {
    Object self;
    int state;
    int frequency;
    int previous;
    int bit;
    PortWriter *pw;
    Msg msg;
} Generator;

int waveGenerator (Generator *self);
int increase      (Generator *self);
int decrease      (Generator *self);
int save          (Generator *self);
int load          (Generator *self);
int save_or_load  (Generator *self);
int read_frequency(Generator *self);

#define initGenerator(bit, pw) {initObject(), 0, 0, 0, bit, pw, NULL}

#endif