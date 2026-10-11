#include "overlay_01_021FFECC.h"

#include "map_object.h"
#include "field/field_effect_manager.h"
#include "player_avatar.h"
#include "unk_0205FD20.h"
#include "unk_020658D4.h"
#include "field_effects.h"
#include "render_model.h"
#include "render_animation.h"

static void ov01_021FFEF8(UnkOv01_021FFECC *a0);
static void ov01_021FFF38(UnkOv01_021FFECC *a0);
static BOOL ov01_021FFFCC(void *a0, UnkOv01_021FFFCD *a1);
static BOOL ov01_021FFFFC(void *a0, UnkOv01_021FFFCD *a1);
static void ov01_0220001C(void *a0, UnkOv01_021FFFCD *a1);
static void ov01_02200020(void *a0, UnkOv01_021FFFCD *a1);

static const int ov01_02209270[4] = { 0x64, 0x65, 0x66, 0x67 };
static const UnkOv01_02209280 ov01_02209280 = {
    0x24, ov01_021FFFCC, ov01_021FFFFC, ov01_0220001C, ov01_02200020
};

UnkOv01_021FFECC *ov01_021FFECC(void *a0) {
    UnkOv01_021FFECC *ptr = FieldEffectManager_HeapAllocAndFill(a0, sizeof(UnkOv01_021FFECC), 0, 0);
    ptr->unk0 = a0;
    ov01_021FFEF8(ptr);
    return ptr;
}

void ov01_021FFEE8(UnkOv01_021FFECC *a0) {
    ov01_021FFF38(a0);
    FieldEffectManager_HeapFree(a0);
}

static void ov01_021FFEF8(UnkOv01_021FFECC *a0) {
    for (int i = 0; i < 4; i++) {
        ov01_021F19F4(a0->unk0, &a0->unk4[i], 0, ov01_02209270[i], 0);
        RenderObj_InitFromModel(&a0->unk54[i], &a0->unk4[i]);
    }
}

static void ov01_021FFF38(UnkOv01_021FFECC *a0) {
    for (int i = 0; i < 4; i++) {
        RenderModel_Free(&a0->unk4[i]);
    }
}

static NNSG3dRenderObj *ov01_021FFF50(UnkOv01_021FFECC *a0, int idx) {
    return &a0->unk54[idx];
}

void ov01_021FFF5C(LocalMapObject *mapObject, int a1) {
    VecFx32 temp;
    UnkOv01_021FFF5C temp2;

    FieldSystem *fieldSystem = FieldEffectManager_GetFromMapObject(mapObject);
    TaskManager *taskman = FieldEffectManager_GetFieldSystem(fieldSystem);

    int x = MapObject_GetXCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);

    sub_020611C8(x, z, &temp);
    sub_0206121C(taskman, &temp);

    temp2.unk0 = a1;
    temp2.unk4 = fieldSystem;

    UnkOv01_021FFECC *val1 = FieldEffectManager_GetRendererData(fieldSystem, 0xe);
    temp2.unk8 = val1;
    temp2.unkC = mapObject;
    temp2.unk10 = ov01_021FFF50(val1, a1);

    int val3 = MapObject_GetPriorityPlusValue(mapObject, 2);
    FieldEffectManager_CreateEffect(fieldSystem, &ov01_02209280, &temp, 0, &temp2, val3);
}

static BOOL ov01_021FFFCC(void *a0, UnkOv01_021FFFCD *a1) {
    UnkOv01_021FFFCC *ptr = FieldEffect_GetUserData(a0);
    LocalMapObject *mapObject = ptr->unk0.unkC;

    a1->unk10 = ptr->unk0;
    a1->localMapObjectId = MapObject_GetID(mapObject);
    a1->mapId = MapObject_GetMapID(mapObject);
    return TRUE;
}

static BOOL ov01_021FFFFC(void *a0, UnkOv01_021FFFCD *a1) {
    LocalMapObject *mapObject = a1->unk10.unkC;

    BOOL val = sub_0205F0A8(mapObject, a1->localMapObjectId, a1->mapId);
    if (val == TRUE) {
        return sub_02066420(mapObject, 0);
    }
    return val;
}

static void ov01_0220001C(void *a0, UnkOv01_021FFFCD *a1) {
}

static void ov01_02200020(void *a0, UnkOv01_021FFFCD *a1) {
    VecFx32 translation;

    if (a1->unk4 != TRUE) {
        FieldEffect_GetPosition(a0, &translation);
        RenderObj_DrawAt(a1->unk10.unk10, &translation);
    }
}
