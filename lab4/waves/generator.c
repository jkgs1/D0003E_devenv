#include "generator.h"

int waveGenerator(Generator *self, int arg){
    self->self = !self->self;

    SYNC(self->portWriter, writeBit, self->pin, self->state);

    Time offset = MSEC(500 / self->frequency);

    AFTER(offset, self, toggle, 0);

    return 0;
}

int increase(Generator *self, int arg) {
    if (self->enabled)
        self->frequency = self->frequency+arg;
    return self->frequency;
}
int decrease(Generator *self, int arg) {
    if (self->enabled)
        self->frequency = self->frequency-arg;
    return self->frequency;
}
int save(Generator *self, int arg){
    self->previous = self->frequency;
    return self->frequency = 0;
}
int load(Generator *self, int arg){
    if(self->previous != 0){
        return self->frequency = self->previous;
    }
    return 0;
}
int save_or_load(Generator *self, int arg){
    if(self->frequency == 0){
        return self->frequency = ASYNC(generator, load, arg);        
    }
    return self->frequency = ASYNC(generator, save, arg);
}