#ifndef TO50_OV96_R58_GAP3_PRIVATE_H
#define TO50_OV96_R58_GAP3_PRIVATE_H
#include "global.h"

void *ov96_021EB5B8(void *);
void Sprite_SetMatrix(void *, const s32 *);
void ov96_021F4DAC(u8, const s32 *, void *);
void ov96_021F4724(u8 *);
void GfGfx_EngineBTogglePlanes(int, BOOL);
void G2x_SetBlendAlpha_(u32, int, int, int, int);
void ov96_021F4B34(u8 *);
u32 LCRandom(void);
void ov96_021F4504(void *, int, int);
void ov96_021EB52C(void *, int, int);
extern const s32 ov96_0221C050[3];
extern const s32 ov96_0221C080[3];
void ov96_021F4CAC(u8 *);
void OS_WaitVBlankIntr(void);
void SetBgAffine(void *, int, const s32 *, int, int);
void ov96_021F4BB4(u8 *);

#endif
