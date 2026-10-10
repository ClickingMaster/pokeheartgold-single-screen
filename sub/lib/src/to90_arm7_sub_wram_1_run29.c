#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 a, speed, depth, range;
    u16 delay, count;
    volatile u16 phase;
} Lfo35190;

void SND_InitLfoParam(Lfo35190 *p);

void SND_InitLfoParam(Lfo35190 *p) {
    p->a = 0;
    p->depth = 0;
    p->range = 1;
    p->speed = 16;
    p->delay = 0;
}
