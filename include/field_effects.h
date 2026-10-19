#ifndef POKEHEARTGOLD_FIELD_EFFECTS_H
#define POKEHEARTGOLD_FIELD_EFFECTS_H

#include "field_types_def.h"

typedef struct {
    int unk0;
    u8 unk4[0x8];
    LocalMapObject *unkC;
    int unk10;
} FieldEffectUserDataHeader;

typedef struct {
    FieldEffectUserDataHeader unk0;
} FieldEffectUserData;

// The pool of running field effects. The FieldEffect and FieldEffectBasicTemplate types are in
// field/field_effect_manager.h, which includes this header.
typedef struct FieldEffectPool FieldEffectPool;

struct FieldEffect;
struct FieldEffectBasicTemplate;

FieldEffectPool *FieldEffectPool_New(enum HeapID, int);
void FieldEffectPool_Delete(FieldEffectPool *);
struct FieldEffect *FieldEffectPool_AddBasicEffect(FieldEffectPool *, const struct FieldEffectBasicTemplate *, const VecFx32 *, int, void *, u32);
void FieldEffect_Destroy(int value);
void FieldEffectPool_DrawAll(FieldEffectPool *);
FieldEffectUserData *FieldEffect_GetUserData(void *data);
void FieldEffect_GetPosition(void *data, VecFx32 *position);
void FieldEffect_DefaultDraw(struct FieldEffect *, void *);
void FieldEffect_DefaultDestroy(struct FieldEffect *, void *);

#endif // POKEHEARTGOLD_FIELD_EFFECTS_H
