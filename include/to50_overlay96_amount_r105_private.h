#ifndef OV96_R105_PRIVATE_H
#define OV96_R105_PRIVATE_H
#include "global.h"
typedef struct {
    u8 pad00[0x68];
    void *sprites[6];
    u8 pad80[0x64];
    u8 paletteStage;
    u8 ramp;
    u16 amount;
} R105Work;
extern int ov96_021E5F24(void *);
extern void ov96_0221058C(R105Work *, u8);
extern void ManagedSprite_SetAnim(void *, int);
extern void ManagedSprite_SetPaletteOverride(void *, int);
extern void PlaySE_SetPitch(u32, int);
extern void GF_AssertFail(void);
u8 ov96_02210324(R105Work *, u32, u16);
#endif
