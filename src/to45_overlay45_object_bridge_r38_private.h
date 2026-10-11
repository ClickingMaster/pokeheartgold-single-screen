#ifndef POKEHEARTGOLD_TO45_OVERLAY45_OBJECT_BRIDGE_R38_PRIVATE_H
#define POKEHEARTGOLD_TO45_OVERLAY45_OBJECT_BRIDGE_R38_PRIVATE_H

#include "global.h"

typedef struct Overlay45R38Vec {
    s32 x;
    s32 y;
    s32 z;
} Overlay45R38Vec;

typedef struct Overlay45R38Object {
    u8 mode : 4;
    u8 type : 2;
    u8 high : 2;
    u8 active : 1;
    u8 value : 7;
    u8 byte2;
    u8 byte3;
    u8 padding[4];
    void *handle;
    u8 model[0x84];
} Overlay45R38Object;

void WiFiPlazaMap_PosToVecFx32(const s16 *source, s32 *destination);
void BillboardObject_SetPosition(void *handle, const Overlay45R38Vec *position);
void ModelInstance_SetPosition(void *model, u32 x, u32 y, u32 z);
const Overlay45R38Vec *BillboardObject_GetPosition(void *handle);
void BillboardObject_SetSequence(void *handle, u32 value);
void BillboardObject_SetSequenceFrame(void *handle, s32 value);
u32 ov45_022309C4(u32 value);

u32 ov45_022308B8(Overlay45R38Object *object);
void ov45_022308C0(Overlay45R38Object *object, const s16 *source);
void ov45_022308E4(Overlay45R38Object *object, const Overlay45R38Vec *position);
void ov45_02230908(Overlay45R38Object *object, Overlay45R38Vec *position);
void ov45_02230920(Overlay45R38Object *object);
void ov45_0223093C(Overlay45R38Object *object, u32 value);

#endif
