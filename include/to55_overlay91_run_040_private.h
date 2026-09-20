#ifndef TO55_SOL_R3_W05_PRIVATE_H
#define TO55_SOL_R3_W05_PRIVATE_H

#include "global.h"

#include "system.h"

typedef struct Ov91R40Work {
    u8 pad00[2];
    u16 counter;
    u8 pad04[0x1c];
    u32 active;
    u8 pad24[0x08];
    VecFx32 position;
    s16 timer;
    u16 value3A;
    u8 pad3C[4];
    fx32 magnitude;
    u8 pad44[4];
    u32 frames;
} Ov91R40Work;

void ov91_0225E728(Ov91R40Work *work, VecFx32 *out);
int ov91_0225DE8C(void *state, VecFx32 *position, const VecFx32 *a, VecFx32 *b);
void ov91_0225E3F4(void *outer, Ov91R40Work *work, int value, u16 kind);
void ov91_0225E9F4(Ov91R40Work *work, void *outer);
void ov91_0225EA54(Ov91R40Work *work, const VecFx32 *delta);
void ov91_0225EA7C(Ov91R40Work *work);
void ov91_0225EA98(Ov91R40Work *work);
void ov91_0225EC7C(Ov91R40Work *work);
BOOL ov91_0225ED6C(Ov91R40Work *work);
fx32 FX_Sqrt(fx32 value);

#endif
