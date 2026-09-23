#include "to58_r2_ov07_mid0_private.h"
extern int FX_Modf(int,int *);
extern int ov07_02222674(int,int,int);

void ov07_02222644(R2Motion *m, float *x, float *y);
void ov07_022226C4(void *pic, s16 base, s16 target, int progress, int reverse);
void ov07_022226FC(void *sprite, s16 base, s16 target, int progress, int reverse);

void ov07_02222644(R2Motion *m, float *x, float *y) {
    *x = (float)m->x / 256.0f;
    *y = (float)m->y / 256.0f;
}

int ov07_02222674(int base, int target, int progress) {
    int whole;
    int scaled = (0x50 - (target << 1)) << 12;
    int interpolation = (int)(((s64)scaled * progress + 0x800) >> 12);
    int fixed = scaled - FX_Div(interpolation, 0x100000);
    int frac = FX_Modf(fixed, &whole);
    if (frac != 0) frac += 0x800;
    int q = (whole + frac) >> 12;
    return (s16)((q + ((u32)q >> 31)) >> 1);
}
