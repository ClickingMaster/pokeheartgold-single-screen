#include "field_effect_map_marker_internal.h"

UnkOv01_021FFECC *ov01_021FF854(void *a0);
void ov01_021FF870(UnkOv01_021FFECC *a0);
void ov01_021FF880(UnkOv01_021FFECC *a0);
void ov01_021FF8C8(UnkOv01_021FFECC *a0);
void ov01_021FF8F0(LocalMapObject *mapObject, int a1);
void ov01_021FF964(LocalMapObject *mapObject, int a1, int a2, int a3, int a4);
BOOL ov01_021FF9F4(void *a0, UnkOv01_02209218_Work *work);
void ov01_021FFAB0(void *a0, UnkOv01_02209218_Work *work);
void ov01_021FFABC(void *a0, UnkOv01_02209218_Work *work);
void ov01_021FFBD4(void *a0, UnkOv01_02209218_Work *work);

extern BOOL ov01_021FFCA8(void *a0, UnkOv01_02209218_Work *work);
extern void ov01_021FFD64(void *a0, UnkOv01_02209218_Work *work);
extern void ov01_021FFD70(void *a0, UnkOv01_02209218_Work *work);
extern void ov01_021FFE98(void *a0, UnkOv01_02209218_Work *work);

extern const UnkOv01_02209280 ov01_02209218;
extern const u32 ov01_0220922C[6];

UnkOv01_021FFECC *ov01_021FF854(void *a0) {
    UnkOv01_021FFECC *ptr = FieldEffectManager_HeapAllocAndFill(a0, 4, 0, 0);
    ptr->unk0 = a0;
    ov01_021FF880(ptr);
    return ptr;
}

void ov01_021FF870(UnkOv01_021FFECC *a0) {
    ov01_021FF8C8(a0);
    FieldEffectManager_HeapFree(a0);
}

void ov01_021FF880(UnkOv01_021FFECC *a0) {
    ov01_021F18D4(a0->unk0, 7, 0x7a);
    ov01_021F1908(a0->unk0, 7, 0x92);
    FieldEffectManager_LoadTexResource(a0->unk0, 8, 0x15, 1);
    FieldEffectManager_AddRecord(a0->unk0, 9, 7, 7, 8, 0, ov01_0220922C);
}

void ov01_021FF8C8(UnkOv01_021FFECC *a0) {
    ov01_021F18FC(a0->unk0, 7);
    ov01_021F1924(a0->unk0, 7);
    FieldEffectManager_FreeTexResource(a0->unk0, 8);
    FieldEffectManager_ReleaseRecord(a0->unk0, 9);
}

void ov01_021FF8F0(LocalMapObject *mapObject, int a1) {
    VecFx32 position;
    UnkOv01_02209218_Args args;
    FieldSystem *fieldSystem = FieldEffectManager_GetFromMapObject(mapObject);

    args.unk00 = MapObject_GetXCoord(mapObject);
    args.unk04 = MapObject_GetYCoord(mapObject);
    args.unk08 = MapObject_GetZCoord(mapObject);
    args.unk1C = 0;
    args.unk1E = 0;
    args.unk20 = -1;
    args.unk10 = fieldSystem;
    args.unk0C = FieldEffectManager_GetFieldSystem(fieldSystem);
    args.unk14 = FieldEffectManager_GetRendererData(fieldSystem, 0xc);
    args.unk18 = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    FieldEffectManager_CreateEffect(fieldSystem, &ov01_02209218, &position, a1, (UnkOv01_021FFF5C *)&args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

void ov01_021FF964(LocalMapObject *mapObject, int a1, int a2, int a3, int a4) {
    VecFx32 position;
    UnkOv01_02209218_Args args;
    FieldSystem *fieldSystem = FieldEffectManager_GetFromMapObject(mapObject);

    args.unk00 = a2;
    args.unk04 = MapObject_GetYCoord(mapObject);
    args.unk08 = a3;
    args.unk1C = MapObject_GetXCoord(mapObject) - a2;
    args.unk1E = MapObject_GetZCoord(mapObject) - a3;
    if (a4) {
        args.unk20 = MapObject_GetFacingDirection(mapObject);
    } else {
        args.unk20 = -1;
    }
    args.unk10 = fieldSystem;
    args.unk0C = FieldEffectManager_GetFieldSystem(fieldSystem);
    args.unk14 = FieldEffectManager_GetRendererData(fieldSystem, 0xc);
    args.unk18 = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    FieldEffectManager_CreateEffect(fieldSystem, &ov01_02209218, &position, a1, (UnkOv01_021FFF5C *)&args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL ov01_021FF9F4(void *a0, UnkOv01_02209218_Work *work) {
    VecFx32 position;

    work->unk18 = *(UnkOv01_02209218_Args *)FieldEffect_GetUserData(a0);
    work->unk04 = MapObject_GetSpriteID(work->unk18.unk18);
    work->unk08 = MapObject_GetID(work->unk18.unk18);
    work->unk0C = MapObject_GetMapID(work->unk18.unk18);
    position.x = work->unk18.unk00 << 16;
    position.z = work->unk18.unk08 << 16;
    position.y = MapObject_GetPositionVectorYCoord(work->unk18.unk18);
    work->unk14 = sub_0206121C(work->unk18.unk0C, &position);
    position.x += 2 << 14;
    position.z += 0x12 << 12;
    FieldEffect_SetPosition(a0, &position);
    work->unk3C = FieldEffectManager_CreateBillboardFromRecord(work->unk18.unk10, 9, &position);
    if (MapObject_TestFlagsBits(work->unk18.unk18, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        BillboardObject_SetVisible(work->unk3C, 0);
    }
    if (FieldEffect_GetParam(a0) == NULL) {
        BillboardObject_SetFrame(work->unk3C, 3 << 14);
        BillboardObject_AdvanceFrame(work->unk3C, 0);
        work->unk00 = 2;
    }
    return TRUE;
}

void ov01_021FFAB0(void *a0, UnkOv01_02209218_Work *work) {
    BillboardObject_Delete(work->unk3C);
}

void ov01_021FFABC(void *a0, UnkOv01_02209218_Work *work) {
    VecFx32 sp0C;
    VecFx32 position;
    LocalMapObject *mapObject = work->unk18.unk18;

    if (!sub_0205F0F8(mapObject, work->unk04, work->unk08, work->unk0C)) {
        FieldEffectManager_DestroyEffect((int)a0);
        return;
    }
    if (MapObject_TestFlagsBits(mapObject, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        BillboardObject_SetVisible(work->unk3C, 0);
    } else {
        BillboardObject_SetVisible(work->unk3C, 1);
    }
    if (work->unk14 == 0) {
        FieldEffect_GetPosition(a0, &sp0C);
        position.x = work->unk18.unk00 << 16;
        position.z = work->unk18.unk08 << 16;
        position.y = sp0C.y;
        work->unk14 = sub_0206121C(work->unk18.unk0C, &position);
        if (work->unk14 == TRUE) {
            sp0C.y = position.y;
            FieldEffect_SetPosition(a0, &sp0C);
        }
    }
    switch (work->unk00) {
    case 0:
        BillboardObject_AdvanceFrame(work->unk3C, 1 << 12);
        if (BillboardObject_GetSequenceFrame(work->unk3C) / FX32_ONE < 12) {
            return;
        }
        work->unk00 = 1;
        return;
    case 1:
        BillboardObject_SetFrame(work->unk3C, 3 << 14);
        BillboardObject_AdvanceFrame(work->unk3C, 0);
        work->unk00 = 2;
        // fallthrough
    case 2: {
        int dx = MapObject_GetXCoord(mapObject) - work->unk18.unk1C;
        int dz = MapObject_GetZCoord(mapObject) - work->unk18.unk1E;
        if (work->unk18.unk00 != dx || work->unk18.unk08 != dz) {
            FieldEffectManager_DestroyEffect((int)a0);
            return;
        }
        if (work->unk18.unk20 != -1 && work->unk18.unk20 != MapObject_GetFacingDirection(mapObject)) {
            FieldEffectManager_DestroyEffect((int)a0);
        }
    }
        return;
    }
}

void ov01_021FFBD4(void *a0, UnkOv01_02209218_Work *work) {
    VecFx32 position;

    if (!sub_0205F0F8(work->unk18.unk18, work->unk04, work->unk08, work->unk0C)) {
        FieldEffectManager_DestroyEffect((int)a0);
        return;
    }
    FieldEffect_GetPosition(a0, &position);
    BillboardObject_SetPosition(work->unk3C, &position);
}
