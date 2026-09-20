#ifndef TO56_R2_W16_PRIVATE_H
#define TO56_R2_W16_PRIVATE_H

#include "global.h"

void ov07_0222FEB0(void *, void *, int, int);
int ov07_0222FF04(void *, void *);
int ov07_0222FFD0(void *);

typedef struct Ov07W16Record {
    u8 raw[0x24];
} Ov07W16Record;

typedef struct Ov07W16Work {
    u8 pad00[0x10];
    int argA;
    int argB;
    void *sprites[15];
    Ov07W16Record records[15];
    int timer;
    int count;
} Ov07W16Work;

#endif
