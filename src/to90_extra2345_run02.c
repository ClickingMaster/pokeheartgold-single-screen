#pragma force_active on
#include "global.h"
#pragma force_active on

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x20];
    s16 sin, cos;
    u8 pad24[8];
    u16 width, height;
} Material;

extern void FX_DivAsync(s32, s32);
extern s32 FX_GetDivResult(void);
void texmtxCalc_flagTS___xsi(s32 *, const Material *);

void texmtxCalc_flagTS___xsi(s32 *matrix, const Material *m) {
    s32 width = m->width << 12, height = m->height << 12;
    FX_DivAsync(height, width);
    matrix[0] = m->cos;
    matrix[5] = m->cos;
    matrix[1] = (s32)((u32)m->sin * (u32)FX_GetDivResult()) >> 12;
    FX_DivAsync(width, height);
    matrix[12] = (s32)(((u32)m->width * (u32)m->sin) << 4);
    matrix[13] = (s32)(((-(u32)m->height) * (u32)(m->cos - 4096)) << 4);
    matrix[4] = (s32)((u32)(-m->sin) * (u32)FX_GetDivResult()) >> 12;
}
