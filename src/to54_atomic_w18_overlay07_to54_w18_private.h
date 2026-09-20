#ifndef TO54_W18_PRIVATE_H
#define TO54_W18_PRIVATE_H

#include "global.h"

typedef void *Handle;

void *ov07_022324D8(void *, u32);
void ov07_0221C410(void *, void (*)(void *, void *), void *);
void ov07_0221C448(void *, void *);
void *ov07_0221C468(void *);
void *ov07_0221C470(void *);
void *ov07_0221FA48(void *, void *);
void *ov07_0221FAA0(void *, void *);
void *ov07_02222004(void *, void *);
void ov07_022220B8(void *, void *, s16, s16);
BOOL ov07_02222240(void *, s16, s16, void *);
void ov07_02222268(void *, s16, s16, int, int, int);
BOOL ov07_022222B4(void *);
void ov07_02222508(void *, s16, int, s16, int);
BOOL ov07_02222748(void *, void *);
void ov07_02222A44(void *, int, int);
void ov07_02222AC4(void *, int, int, int, int, int);
BOOL ov07_02222AF4(void *);
void ov07_02231E08(void *, int, int);

void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ManagedSprite_SetAffineOverwriteMode(void *, int);
void ManagedSprite_SetAffineScale(void *, float, float);
void ManagedSprite_SetOamMode(void *, int);
void ManagedSprite_SetAnimationFrame(void *, int);
void ManagedSprite_SetAnim(void *, int);
void ManagedSprite_SetAnimateFlag(void *, BOOL);
void ManagedSprite_SetDrawFlag(void *, BOOL);
BOOL ManagedSprite_IsAnimated(void *);
int Pokepic_GetAttr(void *, int);
void Pokepic_SetAttr(void *, int, int);
void SpriteSystem_DrawSprites(void *);
void Sprite_DeleteAndFreeResources(void *);
void StartBrightnessTransition(int, int, int, int, int);
BOOL IsBrightnessTransitionActive(int);
void Heap_Free(void *);

#endif
