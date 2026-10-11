#ifndef POKEHEARTGOLD_FIELD_EFFECT_MAP_MARKER_INTERNAL_H
#define POKEHEARTGOLD_FIELD_EFFECT_MAP_MARKER_INTERNAL_H

#include "global.h"

#include "map_object.h"
#include "field/field_effect_manager.h"
#include "billboard_lists.h"
#include "field_effects.h"

// The 0x24-byte argument block handed to FieldEffectManager_CreateEffect as a4 and recovered
// inside the renderer via FieldEffect_GetUserData.
typedef struct {
    int unk00;               // 0x00
    int unk04;               // 0x04
    int unk08;               // 0x08
    void *unk0C;             // 0x0c
    FieldEffectManager *manager; // 0x10
    void *unk14; // 0x14
    LocalMapObject *unk18;   // 0x18
    s16 unk1C;               // 0x1c
    s16 unk1E;               // 0x1e
    s8 unk20;                // 0x20
} UnkOv01_02209218_Args;     // sizeof == 0x24

// The 0x40-byte per-instance work block (template unk0 == 0x40).
typedef struct {
    int unk00;                   // 0x00 state
    u32 unk04;                   // 0x04 sprite id
    u32 unk08;                   // 0x08 local map object id
    u32 unk0C;                   // 0x0c map id
    u32 unk10;                   // 0x10
    BOOL unk14;                  // 0x14
    UnkOv01_02209218_Args unk18; // 0x18 .. 0x3c
    Sprite *unk3C;               // 0x3c
} UnkOv01_02209218_Work;         // sizeof == 0x40


// FieldEffect_GetUserData comes from the tracked include/unk_020689C8.h (returns UnkOv01_021FFFCC *)
void *FieldEffect_GetParam(void *a0);
void *BillboardObject_Delete(Sprite *a0);
void BillboardObject_SetFrame(Sprite *a0, fx32 a1);
fx32 BillboardObject_GetSequenceFrame(Sprite *a0);
void BillboardObject_SetPosition(Sprite *a0, VecFx32 *a1);
void BillboardObject_AdvanceFrame(Sprite *a0, fx32 a1);
void FieldEffect_SetPosition(void *a0, VecFx32 *a1);
BOOL sub_0206121C(void *a0, VecFx32 *a1);

typedef struct UnkOv01_021FF854 {
    FieldEffectManager *unk0;
} UnkOv01_021FF854;

#endif // POKEHEARTGOLD_FIELD_EFFECT_MAP_MARKER_INTERNAL_H
