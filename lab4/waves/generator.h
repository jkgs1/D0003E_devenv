typedef struct {
    Object self;
    int enabled;
    int frequency;
    int previous;
} Generator;

int waveGenerator(Generator *self, int arg);
int increase     (Generator *self, int arg);
int decrease     (Generator *self, int arg);
int save         (Generator *self, int arg);
int load         (Generator *self, int arg);
int save_or_load (Generator *self, int arg);