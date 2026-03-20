#include "generator.h"
#include "../TinyTimber.h"

int waveGenerator(Generator *self, int arg){
    if (self->frequency > 0) {
        self->state = !self->state;
        SYNC(self->pw, portWriter, self->bit);
        Time offset = MSEC(1000 / self->frequency);
        //AFTER(offset, self, waveGenerator, 0);
        if (self->msg) {
            ABORT(self->msg);
            self->msg = NULL;
        }
        self->msg =AFTER(offset, self, waveGenerator, 0);
    } else {
        self->state = 0;
        SYNC(self->pw, portWriter_clear, self->bit);
    }
    return 0;
}

int increase(Generator *self, int arg) {
    if (self->frequency < 99) {
        self->frequency++;
    }
    return self->frequency;
}

int decrease(Generator *self, int arg) {
    if (self->frequency > 0) {
        self->frequency--;
    }
    return self->frequency;
}
int save(Generator *self, int arg){
    self->previous = self->frequency;
    return 0;
}
int load(Generator *self, int arg){
    return self->previous;
}
int save_or_load(Generator *self, int arg){
    if(self->frequency == 0){
        return self->frequency = load(self, arg);
    }
    return self->frequency = save(self, arg);
}
int read_frequency(Generator *self, int arg) {
    return self->frequency;
}