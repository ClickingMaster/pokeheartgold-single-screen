#ifndef TO52_R7_W09_PRIVATE_H
#define TO52_R7_W09_PRIVATE_H

#include "global.h"
#include <nitro/fx/fx_cp.h>

int FontID_String_GetWidth(void *string, int font, int spacing);
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *sprite, s16 x, s16 y, s32 offset);
void GF_AssertFail(void);
typedef struct Ov89WorkR7 {
    u8 pad0000[0x924];
    void *sprites[6];
    u8 pad093C[0x19D4 - 0x93C];
    s32 target;
    s32 current;
    s32 step;
} Ov89WorkR7;
void ov89_0225A368(int font, void *string, int *widthOut, int *tilesOut);
void ov89_0225A398(Ov89WorkR7 *work);
BOOL ov89_0225A41C(Ov89WorkR7 *work, s32 amount);

#endif
