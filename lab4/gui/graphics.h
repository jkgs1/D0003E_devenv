#ifndef graphics_H
#define graphics_H
#include "../waves/generator.h"
#include <stdbool.h>

typedef struct {
    Object super;
    Generator left;
    Generator right;
    bool left_freq;
} GUI;

bool switch_freq(GUI *self, bool left_freq);
void update(Generator *self, bool left_freq);

#define initGRAPHICS(left, right){initObject(), left, right, true}

#endif