#include "overlay_01_021FFECC.h"

#include "map_object.h"
#include "field/field_effect_manager.h"
#include "player_avatar.h"
#include "unk_0205FD20.h"
#include "unk_020658D4.h"
#include "field_effects.h"
#include "render_model.h"
#include "render_animation.h"

struct UnkOv01_021FFECC {
    FieldEffectManager *manager;
    UnkOv01_021FFECC_sub unk4[4];
    NNSG3dRenderObj unk54[4];
};

typedef struct {
    int unk0;
    FieldEffectManager *manager;
    UnkOv01_021FFECC *unk8;
    LocalMapObject *mapObject;
    NNSG3dRenderObj *renderObj;
} UnkOv01_021FFF5C;

typedef struct {
    int unk0;
    BOOL unk4;
    u32 localMapObjectId;
    u32 mapId;
    UnkOv01_021FFF5C args;
} UnkOv01_021FFFCD;

static void ov01_021FFEF8(UnkOv01_021FFECC *a0);
static void ov01_021FFF38(UnkOv01_021FFECC *a0);
static BOOL ov01_021FFFCC(FieldEffect *effect, UnkOv01_021FFFCD *a1);
static BOOL ov01_021FFFFC(FieldEffect *effect, UnkOv01_021FFFCD *a1);
static void ov01_0220001C(FieldEffect *effect, UnkOv01_021FFFCD *a1);
static void ov01_02200020(FieldEffect *effect, UnkOv01_021FFFCD *a1);

static const int ov01_02209270[4] = { 0x64, 0x65, 0x66, 0x67 };
static const FieldEffectBasicTemplate ov01_02209280 = {
    sizeof(UnkOv01_021FFFCD),
    (FieldEffectInitFunc)ov01_021FFFCC,
    (FieldEffectFunc)ov01_021FFFFC,
    (FieldEffectFunc)ov01_0220001C,
    (FieldEffectFunc)ov01_02200020,
};

UnkOv01_021FFECC *ov01_021FFECC(FieldEffectManager *manager) {
    UnkOv01_021FFECC *ptr = FieldEffectManager_HeapAllocAndFill(manager, sizeof(UnkOv01_021FFECC), FALSE, 0);
    ptr->manager = manager;
    ov01_021FFEF8(ptr);
    return ptr;
}

void ov01_021FFEE8(UnkOv01_021FFECC *a0) {
    ov01_021FFF38(a0);
    FieldEffectManager_HeapFree(a0);
}

static void ov01_021FFEF8(UnkOv01_021FFECC *a0) {
    for (int i = 0; i < 4; i++) {
        FieldEffectManager_LoadRenderModel(a0->manager, &a0->unk4[i], 0, ov01_02209270[i], FALSE);
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

    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);
    FieldSystem *fieldSystem = FieldEffectManager_GetFieldSystem(manager);

    int x = MapObject_GetXCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);

    sub_020611C8(x, z, &temp);
    sub_0206121C((TaskManager *)fieldSystem, &temp);

    temp2.unk0 = a1;
    temp2.manager = manager;

    UnkOv01_021FFECC *val1 = FieldEffectManager_GetRendererData(manager, 0xe);
    temp2.unk8 = val1;
    temp2.mapObject = mapObject;
    temp2.renderObj = ov01_021FFF50(val1, a1);

    int val3 = MapObject_GetPriorityPlusValue(mapObject, 2);
    FieldEffectManager_CreateEffect(manager, &ov01_02209280, &temp, 0, &temp2, val3);
}

static BOOL ov01_021FFFCC(FieldEffect *effect, UnkOv01_021FFFCD *a1) {
    UnkOv01_021FFF5C *args = (UnkOv01_021FFF5C *)FieldEffect_GetUserData(effect);
    LocalMapObject *mapObject = args->mapObject;

    a1->args = *args;
    a1->localMapObjectId = MapObject_GetID(mapObject);
    a1->mapId = MapObject_GetMapID(mapObject);
    return TRUE;
}

static BOOL ov01_021FFFFC(FieldEffect *effect, UnkOv01_021FFFCD *a1) {
    LocalMapObject *mapObject = a1->args.mapObject;

    BOOL val = sub_0205F0A8(mapObject, a1->localMapObjectId, a1->mapId);
    if (val == TRUE) {
        return sub_02066420(mapObject, 0);
    }
    return val;
}

static void ov01_0220001C(FieldEffect *effect, UnkOv01_021FFFCD *a1) {
}

static void ov01_02200020(FieldEffect *effect, UnkOv01_021FFFCD *a1) {
    VecFx32 translation;

    if (a1->unk4 != TRUE) {
        FieldEffect_GetPosition(effect, &translation);
        RenderObj_DrawAt((int)a1->args.renderObj, &translation);
    }
}
