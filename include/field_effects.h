#ifndef POKEHEARTGOLD_FIELD_EFFECTS_H
#define POKEHEARTGOLD_FIELD_EFFECTS_H

#include "field_types_def.h"

typedef struct {
    int unk0;
    u8 unk4[0x8];
    LocalMapObject *unkC;
    int unk10;
} UnkOv01_021FFFCC_common;

typedef struct {
    UnkOv01_021FFFCC_common unk0;
} UnkOv01_021FFFCC;

void FieldEffect_Destroy(int value);
UnkOv01_021FFFCC *FieldEffect_GetUserData(void *data);
void FieldEffect_GetPosition(void *data, VecFx32 *position);

#endif // POKEHEARTGOLD_FIELD_EFFECTS_H
