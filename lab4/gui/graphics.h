typedef struct {
    Object super;
    int enabled;
    Generator left;
    Generator right;
    bool left_freq;
} GUI;