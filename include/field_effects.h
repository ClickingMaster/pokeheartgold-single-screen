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

void FieldEffect_Destroy(int a0);
UnkOv01_021FFFCC *FieldEffect_GetUserData(void *a0);
void FieldEffect_GetPosition(void *a0, VecFx32 *a1);

#endif // POKEHEARTGOLD_FIELD_EFFECTS_H
