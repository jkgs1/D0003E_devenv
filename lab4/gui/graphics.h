#ifndef graphics_H
#define graphics_H
#include "../waves/generator.h"
#include <stdbool.h>

typedef struct {
    Object super;
    Generator *left;
    Generator *right;
    bool left_freq;
} GUI;

int switch_freq(GUI *self, int arg);
int read_direction(GUI *self, int arg);
int update(GUI *self, int arg);

#define initGRAPHICS(left, right){initObject(), left, right, true}

#endif