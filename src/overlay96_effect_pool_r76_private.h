#ifndef OVERLAY96_EFFECT_POOL_R76_PRIVATE_H
#define OVERLAY96_EFFECT_POOL_R76_PRIVATE_H

#include "global.h"

typedef struct Ov96EffectPoolEntry {
    u32 active;
    void *handle;
} Ov96EffectPoolEntry;

typedef struct Ov96EffectPoolRow {
    Ov96EffectPoolEntry entries[5];
    u32 next;
} Ov96EffectPoolRow;

extern void GF_AssertFail(void);
extern void PokeathlonCourseSpritePoolSprite_Show(void *handle, u32 a1, u32 a2);
extern void PokeathlonCourseSpritePoolSprite_SetAnim(void *handle, u32 value);
extern void PokeathlonCourseSpritePoolSprite_SetPosition(void *handle, const VecFx32 *position);
extern VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(void *handle);
extern u32 PokeathlonCourseSpritePoolSprite_IsAnimated(void *handle);

void ov96_021FC698(Ov96EffectPoolRow *rows, u32 row, const VecFx32 *position);
void ov96_021FC6EC(Ov96EffectPoolRow *rows, u32 row, u32 yOffset);
void ov96_021FC758(Ov96EffectPoolEntry *entry);

#endif
