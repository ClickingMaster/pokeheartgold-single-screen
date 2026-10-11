#include "global.h"

#include "field/field_effect_manager.h"

#include "field_effect_renderer_list_internal.h"
#include "field_system.h"
#include "gf_rtc.h"
#include "map_object.h"
#include "render_animation.h"
#include "render_model.h"
#include "unk_020689C8.h"

// Map object ground shadows: four shadow models whose alpha and scale follow the time of day.

// Not declared in any header yet.
void *sub_02068D90(void *effect);
void NNS_G3dMdlSetMdlAlphaAll(NNSG3dResMdl *mdl, u32 alpha);
void RenderObj_Draw(void *renderObj, VecFx32 *pos, VecFx32 *scale, MtxFx33 *mtx);
BOOL ov01_021F8F88(LocalMapObject *object);
void ov01_021F8FA0(LocalMapObject *object, VecFx32 *offset);

// Renderer data of the field effect manager (renderer 0); 0x1C8 bytes.
typedef struct FieldShadow {
    int state;
    int timeOfDay;
    u32 unk_08;
    u32 unk_0C;
    fx32 alpha;
    VecFx32 scale;
    FieldSystem *manager; // the field effect manager, typed as overlay_01_021F1348.h does
    SysTask *task;
    UnkOv01_021FFECC_sub models[4];
    NNSG3dRenderObj renderObjs[4];
} FieldShadow;

// Field effect work of one shadow; 0x20 bytes (the first word of each template).
typedef struct FieldShadowEffect {
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    int hidden;
    void *parameter;
    u32 unk_14;
    FieldShadow *shadow;
    LocalMapObject *mapObject;
} FieldShadowEffect;

extern const u32 ov01_02208E90[4];
extern const UnkOv01_02209280 ov01_02208EA0;
extern const fx32 ov01_02208EB4[5];
extern const UnkOv01_02209280 ov01_02208EC8;
extern const UnkOv01_02209280 ov01_02208EDC;
extern const MtxFx33 ov01_02208EF0;
extern const MtxFx33 ov01_02208F14;
extern const VecFx32 ov01_02208F38[5];

FieldShadow *FieldShadow_New(FieldSystem *manager);
void FieldShadow_Free(FieldShadow *shadow);
void FieldShadow_StartTask(FieldShadow *shadow);
void FieldShadow_StopTask(FieldShadow *shadow);
void FieldShadow_GetScale(FieldShadow *shadow, VecFx32 *dest);
int FieldShadow_FxToAlpha(fx32 value);
void FieldShadow_SetTimeOfDayParams(FieldShadow *shadow, int timeOfDay);
void FieldShadow_StepTowards(fx32 *value, fx32 target, fx32 step);
void FieldShadow_UpdateTask(SysTask *task, FieldShadow *shadow);
void FieldShadow_LoadModels(FieldShadow *shadow);
void FieldShadow_FreeModels(FieldShadow *shadow);
void FieldShadow_SetAlpha(FieldShadow *shadow, u32 alpha);
void FieldShadow_CreateMapObjectShadow(LocalMapObject *object);
void ov01_021FD684(LocalMapObject *object);
BOOL FieldShadowEffect_Init(void *effect, FieldShadowEffect *work);
void FieldShadowEffect_Destroy(void *effect, FieldShadowEffect *work);
void FieldShadowEffect_Update(void *effect, FieldShadowEffect *work);
BOOL ov01_021FD784(void *effect, FieldShadowEffect *work);
void FieldShadowEffect_Draw(void *effect, FieldShadowEffect *work);
void ov01_021FD838(void *effect, FieldShadowEffect *work);
void ov01_021FD8E8(LocalMapObject *object, int index);
void ov01_021FD92C(void *effect, FieldShadowEffect *work);
void ov01_021FD980(void *effect, FieldShadowEffect *work);
void FieldShadow_OffsetByFacing(u32 direction, VecFx32 *pos);

FieldShadow *FieldShadow_New(FieldSystem *manager) {
    FieldShadow *shadow = (FieldShadow *)FieldEffectManager_HeapAllocAndFill(manager, sizeof(FieldShadow), 0, 0);
    shadow->manager = manager;
    FieldShadow_LoadModels(shadow);
    FieldShadow_StartTask(shadow);
    return shadow;
}

void FieldShadow_Free(FieldShadow *shadow) {
    FieldShadow_StopTask(shadow);
    FieldShadow_FreeModels(shadow);
    FieldEffectManager_HeapFree(shadow);
}

void FieldShadow_StartTask(FieldShadow *shadow) {
    FieldSystem *fieldSystem = (FieldSystem *)FieldEffectManager_GetFieldSystem(shadow->manager);
    u32 priority = MapObjectManager_GetPriority(fieldSystem->mapObjectManager) - 1;
    shadow->task = SysTask_CreateOnMainQueue((SysTaskFunc)FieldShadow_UpdateTask, shadow, priority);
}

void FieldShadow_StopTask(FieldShadow *shadow) {
    SysTask_Destroy(shadow->task);
}

void FieldShadow_GetScale(FieldShadow *shadow, VecFx32 *dest) {
    *dest = shadow->scale;
}

int FieldShadow_FxToAlpha(fx32 value) {
    return value / 0x1000;
}

void FieldShadow_SetTimeOfDayParams(FieldShadow *shadow, int timeOfDay) {
    shadow->alpha = ov01_02208EB4[shadow->timeOfDay];
    shadow->scale = ov01_02208F38[shadow->timeOfDay];
}

void FieldShadow_StepTowards(fx32 *value, fx32 target, fx32 step) {
    fx32 cur = *value;
    if (cur < target) {
        cur += step;
        *value = cur;
        if (cur > target) {
            *value = target;
        }
    } else if (cur > target) {
        cur -= step;
        *value = cur;
        if (cur < target) {
            *value = target;
        }
    }
}

// Fades alpha and scale to the values of the current time of day.
void FieldShadow_UpdateTask(SysTask *task, FieldShadow *shadow) {
    fx32 y;
    VecFx32 scale;
    fx32 alpha;
    fx32 z;
    int state;
    TIMEOFDAY timeOfDay = GF_RTC_GetTimeOfDay();

    state = shadow->state;
    switch (state) {
    case 0:
        shadow->timeOfDay = timeOfDay;
        FieldShadow_SetTimeOfDayParams(shadow, timeOfDay);
        FieldShadow_SetAlpha(shadow, FieldShadow_FxToAlpha(shadow->alpha));
        shadow->state++;
        break;
    case 1:
        if (shadow->timeOfDay == timeOfDay) {
            break;
        }
        shadow->state = state + 1;
        // fallthrough
    case 2:
        alpha = ov01_02208EB4[timeOfDay];
        scale = ov01_02208F38[timeOfDay];
        FieldShadow_StepTowards(&shadow->scale.x, scale.x, 0x10);
        y = scale.y;
        FieldShadow_StepTowards(&shadow->scale.y, y, 0x10);
        z = scale.z;
        FieldShadow_StepTowards(&shadow->scale.z, z, 0x10);
        FieldShadow_StepTowards(&shadow->alpha, alpha, 0x200);
        FieldShadow_SetAlpha(shadow, FieldShadow_FxToAlpha(shadow->alpha));
        if (scale.x == shadow->scale.x && y == shadow->scale.y && z == shadow->scale.z && alpha == shadow->alpha) {
            shadow->timeOfDay = timeOfDay;
            shadow->state = 1;
        }
        break;
    }
}

void FieldShadow_LoadModels(FieldShadow *shadow) {
    int i;
    for (i = 0; i < 4; i++) {
        ov01_021F19F4(shadow->manager, &shadow->models[i], 0, ov01_02208E90[i], 0);
        RenderObj_InitFromModel(&shadow->renderObjs[i], &shadow->models[i]);
    }
}

void FieldShadow_FreeModels(FieldShadow *shadow) {
    int i;
    for (i = 0; i < 4; i++) {
        RenderModel_Free(&shadow->models[i]);
    }
}

void FieldShadow_SetAlpha(FieldShadow *shadow, u32 alpha) {
    NNSi_G3dModifyPolygonAttrMask(shadow->models[0].unkC, 1, 0x1F << 16);
    NNS_G3dMdlSetMdlAlphaAll(shadow->models[0].unkC, alpha);
}

void FieldShadow_CreateMapObjectShadow(LocalMapObject *object) {
    FieldEffectRendererArgs args;
    VecFx32 pos;
    FieldSystem *manager = FieldEffectManager_GetFromMapObject(object);
    args.unk0 = (u32)manager;
    args.unk4 = (FieldSystem *)FieldEffectManager_GetRendererData(manager, 0);
    args.unk8 = object;
    MapObject_CopyPositionVector(object, &pos);
    FieldEffectManager_CreateEffect(manager, &ov01_02208EA0, &pos, 0, (UnkOv01_021FFF5C *)&args, MapObject_GetPriorityPlusValue(object, 2));
}

void ov01_021FD684(LocalMapObject *object) {
    FieldEffectRendererArgs args;
    VecFx32 pos;
    FieldSystem *manager = FieldEffectManager_GetFromMapObject(object);
    args.unk0 = (u32)manager;
    args.unk4 = (FieldSystem *)FieldEffectManager_GetRendererData(manager, 0);
    args.unk8 = object;
    MapObject_CopyPositionVector(object, &pos);
    FieldEffectManager_CreateEffect(manager, &ov01_02208EC8, &pos, 3, (UnkOv01_021FFF5C *)&args, MapObject_GetPriorityPlusValue(object, 2));
}

BOOL FieldShadowEffect_Init(void *effect, FieldShadowEffect *work) {
    *(FieldEffectRendererArgs *)&work->unk_14 = *(FieldEffectRendererArgs *)sub_02068D98(effect);
    work->parameter = sub_02068D90(effect);
    work->spriteId = MapObject_GetSpriteID(work->mapObject);
    work->objectId = MapObject_GetID(work->mapObject);
    if (MapObject_CheckFlag25(work->mapObject) == TRUE) {
        work->mapId = sub_0205F544(work->mapObject);
    } else {
        work->mapId = MapObject_GetMapID(work->mapObject);
    }
    return TRUE;
}

void FieldShadowEffect_Destroy(void *effect, FieldShadowEffect *work) {
}

void FieldShadowEffect_Update(void *effect, FieldShadowEffect *work) {
    VecFx32 pos;
    LocalMapObject *object = work->mapObject;
    if (!sub_0205F0F8(object, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    if (sub_0205F5E8(object, (MapObjectFlagBits)8)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    work->hidden = 0;
    if (MapObject_TestFlagsBits(object, (MapObjectFlagBits)0x00100200) == TRUE) {
        work->hidden = 1;
        return;
    }
    MapObject_CopyPositionVector(object, &pos);
    sub_02068DA8(effect, &pos);
}

BOOL ov01_021FD784(void *effect, FieldShadowEffect *work) {
    *(FieldEffectRendererArgs *)&work->unk_14 = *(FieldEffectRendererArgs *)sub_02068D98(effect);
    work->parameter = sub_02068D90(effect);
    work->spriteId = MapObject_GetSpriteID(work->mapObject);
    work->objectId = MapObject_GetID(work->mapObject);
    if (MapObject_CheckFlag25(work->mapObject) == TRUE) {
        work->mapId = sub_0205F544(work->mapObject);
    } else {
        work->mapId = MapObject_GetMapID(work->mapObject);
    }
    work->hidden = TRUE;
    return TRUE;
}

void FieldShadowEffect_Draw(void *effect, FieldShadowEffect *work) {
    VecFx32 pos;
    VecFx32 scale;
    MtxFx33 mtx;
    FieldShadow *shadow;
    if (work->hidden != 0) {
        return;
    }
    mtx = ov01_02208F14;
    shadow = work->shadow;
    FieldShadow_GetScale(shadow, &scale);
    sub_02068DB8(effect, &pos);
    pos.x -= 0x800;
    pos.y -= 0x4000;
    pos.z += 0x1000;
    RenderObj_Draw(&shadow->renderObjs[0], &pos, &scale, &mtx);
}

void ov01_021FD838(void *effect, FieldShadowEffect *work) {
    VecFx32 pos;
    VecFx32 scale;
    MtxFx33 mtx;
    VecFx32 offset;
    LocalMapObject *object;
    FieldShadow *shadow;
    NNSG3dRenderObj *renderObj;

    if (work->hidden != 0) {
        return;
    }
    mtx = ov01_02208EF0;
    shadow = work->shadow;
    object = work->mapObject;
    renderObj = &shadow->renderObjs[0];
    FieldShadow_GetScale(shadow, &scale);
    if (ov01_021F8F88(object) != 0) {
        sub_02068DB8(effect, &pos);
        pos.x -= 0x800;
        pos.y -= 0x4000;
        pos.z += 0x1000;
        FieldShadow_OffsetByFacing(MapObject_GetFacingDirection(object), &pos);
    } else {
        MapObject_CopyPositionVector(object, &pos);
        ov01_021F8FA0(object, &offset);
        pos.x += offset.x;
        pos.z += offset.z;
        pos.x -= 0x800;
        pos.y -= 0x4000;
        pos.z += 0x1000;
    }
    RenderObj_Draw(renderObj, &pos, &scale, &mtx);
}

void ov01_021FD8E8(LocalMapObject *object, int index) {
    FieldEffectRendererArgs args;
    VecFx32 pos;
    FieldSystem *manager = FieldEffectManager_GetFromMapObject(object);
    args.unk0 = (u32)manager;
    args.unk4 = (FieldSystem *)FieldEffectManager_GetRendererData(manager, 0);
    args.unk8 = object;
    MapObject_CopyPositionVector(object, &pos);
    FieldEffectManager_CreateEffect(manager, &ov01_02208EDC, &pos, index, (UnkOv01_021FFF5C *)&args, MapObject_GetPriorityPlusValue(object, 2));
}

void ov01_021FD92C(void *effect, FieldShadowEffect *work) {
    VecFx32 pos;
    LocalMapObject *object = work->mapObject;
    if (!sub_0205F0F8(object, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    work->hidden = 0;
    if (MapObject_TestFlagsBits(object, (MapObjectFlagBits)0x00100200) == TRUE) {
        work->hidden = 1;
        return;
    }
    MapObject_CopyPositionVector(object, &pos);
    sub_02068DA8(effect, &pos);
}

void ov01_021FD980(void *effect, FieldShadowEffect *work) {
    VecFx32 pos;

    if (work->hidden == 1) {
        return;
    }
    {
        u32 index = (u32)sub_02068D90(effect);
        FieldShadow *shadow = work->shadow;
        NNSG3dRenderObj *renderObj = &shadow->renderObjs[index];
        sub_02068DB8(effect, &pos);
        pos.x -= 0x800;
        pos.y -= 0x4000;
        pos.z += 0x1000;
        RenderObj_DrawAt((int)renderObj, &pos);
    }
}

void FieldShadow_OffsetByFacing(u32 direction, VecFx32 *pos) {
    switch (direction) {
    case 0:
        pos->z += 0x2000;
        break;
    case 1:
        pos->z -= 0x2000;
        break;
    case 2:
        pos->x += 0x6000;
        break;
    case 3:
        pos->x -= 0x6000;
        break;
    }
}
