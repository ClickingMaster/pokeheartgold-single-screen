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

void FieldEffect_Destroy(int value);
FieldEffectUserData *FieldEffect_GetUserData(void *data);
void FieldEffect_GetPosition(void *data, VecFx32 *position);

#endif // POKEHEARTGOLD_FIELD_EFFECTS_H
