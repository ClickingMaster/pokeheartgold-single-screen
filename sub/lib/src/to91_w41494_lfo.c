#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 type, speed, depth, range;
    u16 delay, count, phase;
} Lfo09;

extern s8 SND_SinIdx(int);
int SND_GetLfoValue(Lfo09 *);

int SND_GetLfoValue(Lfo09 *raw) {
    Lfo09 *p = raw;
    const u8 *coeff = (const u8 *)raw + 2;
    int value;
    if (!p->depth) {
        return 0;
    }
    if (p->count < p->delay) {
        return 0;
    }
    value = SND_SinIdx((u32)p->phase >> 8);
    return (int)coeff[1] * ((int)coeff[0] * value);
}
