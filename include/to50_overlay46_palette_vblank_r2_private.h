#ifndef OV46_R2_PRIVATE_H
#define OV46_R2_PRIVATE_H
#include "global.h"
extern void GX_LoadBGPltt(const void *, u32, u32);
extern void GXS_LoadBGPltt(const void *, u32, u32);
typedef struct {
    u8 pad00[4];
    void *enabled;
    u8 pad08[0x80];
    u8 palette[0x2A0];
    s16 paletteIndex;
    s8 direction;
    u8 tick;
    u32 mode;
} Ov46PaletteWork;
void ov46_02259210(void *, Ov46PaletteWork *);
#endif
