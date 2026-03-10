#include "generator.h"
#include "../TinyTimber.h"

int waveGenerator(Generator *self){
    if (self->frequency > 0) {
        self->state = !self->state;
        ASYNC(self->pw, portWriter, self->bit);
        Time offset = MSEC(500 / self->frequency);
        if (self->msg) {
            ABORT(self->msg);
            self->msg = NULL;
        }
        self->msg =AFTER(offset, self, waveGenerator, 0);
    } else {
        self->state = 0;
        ASYNC(self->pw, portWriter_clear, self->bit);
    }
    return 0;
}

int increase(Generator *self) {
    if (self->frequency < 99) {
        self->frequency++;
    }
    return self->frequency;
}

int decrease(Generator *self) {
    if (self->frequency > 0) {
        self->frequency--;
    }
    return self->frequency;
}
int save(Generator *self){
    self->previous = self->frequency;
    return 0;
}
int load(Generator *self){
    return self->previous;
}
int save_or_load(Generator *self){
    if(self->frequency == 0){
        return self->frequency = load(self);
    }
    return self->frequency = load(self);
}
int read_frequency(Generator *self) {
    return self->frequency;
}