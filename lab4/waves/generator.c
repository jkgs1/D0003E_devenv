#include "generator.h"
#include "../TinyTimber.h"
#include "writer.h"

int waveGenerator(Generator *self){
    self->state = !self->state;

    ASYNC(&(self->pw), portWriter, self->bit);

    Time offset = MSEC(500 / self->frequency);

    AFTER(offset, self, waveGenerator, 0);

    return 0;
}

int increase(Generator *self, int arg) {
    return self->frequency+arg;
}
int decrease(Generator *self, int arg) {
    return self->frequency-arg;
}
int save(Generator *self){
    self->previous = self->frequency;
    return self->frequency = 0;
}
int load(Generator *self){
    if(self->previous != 0){
        return self->frequency = self->previous;
    }
    return 0;
}
int save_or_load(Generator *self){
    if(self->frequency == 0){
        return self->frequency = SYNC(self, load, 0);        
    }
    return self->frequency = SYNC(self, save, 0);
}