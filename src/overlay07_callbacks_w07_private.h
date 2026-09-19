#ifndef TO53_SOL_R1_W07_PRIVATE_H
#define TO53_SOL_R1_W07_PRIVATE_H

#include "global.h"

void *ov07_022324D8(void *, u32);
void ov07_0221C410(void *, void *, void *);
void ov07_0221C448(void *, void *);
void ov07_0222DBD8(void *);
BOOL ov07_0222DC54(void *);
void Sprite_DeleteAndFreeResources(void *);
void SpriteSystem_DrawSprites(void *);
void Heap_Free(void *);
BOOL ov07_0222E3E8(void *);
void ov07_0222E3A4(void *, u32);
BOOL ov07_0222E5E8(void *);
void ov07_02222590(void *, s32, s32, s32, s32, s32, s32);
void ManagedSprite_SetDrawPriority(void *, u32);
void ManagedSprite_SetPriority(void *, u32);
void ManagedSprite_SetDrawFlag(void *, BOOL);
void ManagedSprite_SetAnimateFlag(void *, BOOL);
void ManagedSprite_SetOamMode(void *, int);
void ManagedSprite_SetAnimSpeed(void *, fx32);
void ManagedSprite_SetAnim(void *, u32);
void ManagedSprite_SetAnimationFrame(void *, u32);
void ManagedSprite_TickNFrames(void *, fx32);
BOOL ManagedSprite_IsAnimated(void *);
u32 ov07_02221FF0(void *);
void PaletteData_BlendPalettes(void *, u32, u32, u32, u32);
BOOL ov07_0222FFD0(void *);
BOOL ov07_0223049C(void *, void *);
BOOL ov07_0223061C(void *, void *);
BOOL ov07_022222F0(void *, void *);

void ov07_0222DCD8(void *, void *);
void ov07_0222E4F8(void *, void *);
void ov07_0222E6A8(void *, void *);
void ov07_0222EDF0(void *);
void ov07_0222EF0C(void *, u32 *, fx32);
BOOL ov07_0222EF58(void *, u32 *);
void ov07_0222F408(void *, void *);
void ov07_02230170(void *);
void ov07_02230600(void *);
void ov07_02230960(void *, void *);

#endif
