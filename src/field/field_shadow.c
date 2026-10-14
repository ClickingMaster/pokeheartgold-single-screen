#include "global.h"

#include "field/field_effect_manager.h"
#include "field/map_object_models.h"

#include "field_system.h"
#include "gf_rtc.h"
#include "map_object.h"
#include "render_animation.h"
#include "render_model.h"
#include "field_effects.h"

// Map object ground shadows: four shadow models whose alpha and scale follow the time of day.

// Not declared in any header yet.
void *FieldEffect_GetParam(void *effect);
void FieldEffect_SetPosition(void *effect, VecFx32 *position);
void NNS_G3dMdlSetMdlAlphaAll(NNSG3dResMdl *mdl, u32 alpha);
void RenderObj_Draw(void *renderObj, VecFx32 *pos, VecFx32 *scale, MtxFx33 *mtx);

// Renderer data of the field effect manager (renderer 0); 0x1C8 bytes.
typedef struct FieldShadow {
    int state;
    int timeOfDay;
    u32 unk_08;
    u32 unk_0C;
    fx32 alpha;
    VecFx32 scale;
    FieldEffectManager *manager;
    SysTask *task;
    RenderModelStorage models[4];
    NNSG3dRenderObj renderObjs[4];
} FieldShadow;

// The user data of the shadow effects.
typedef struct FieldShadowEffectArgs {
    FieldEffectManager *manager;
    FieldShadow *shadow;
    LocalMapObject *mapObject;
} FieldShadowEffectArgs;

// Field effect work of one shadow; 0x20 bytes (the first word of each template).
typedef struct FieldShadowEffect {
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    int hidden;
    void *parameter;
    FieldShadowEffectArgs args;
} FieldShadowEffect;

extern const u32 sShadowModelFiles[4];
extern const FieldEffectBasicTemplate sMapObjectShadowTemplate;
extern const fx32 sShadowAlphaByTimeOfDay[5];
extern const FieldEffectBasicTemplate sFollowMonShadowTemplate;
extern const FieldEffectBasicTemplate sFriendShadowTemplate;
extern const MtxFx33 sFollowMonShadowDrawRotation;
extern const MtxFx33 sShadowDrawRotation;
extern const VecFx32 sShadowScaleByTimeOfDay[5];

FieldShadow *FieldShadow_New(FieldEffectManager *manager);
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
void FieldShadow_CreateFollowMonShadow(LocalMapObject *object);
BOOL FieldShadowEffect_Init(FieldEffect *effect, FieldShadowEffect *work);
void FieldShadowEffect_Destroy(FieldEffect *effect, FieldShadowEffect *work);
void FieldShadowEffect_Update(FieldEffect *effect, FieldShadowEffect *work);
BOOL FieldShadowEffect_InitFollowMon(FieldEffect *effect, FieldShadowEffect *work);
void FieldShadowEffect_Draw(FieldEffect *effect, FieldShadowEffect *work);
void FieldShadowEffect_DrawFollowMon(FieldEffect *effect, FieldShadowEffect *work);
void FieldShadow_CreateFriendShadow(LocalMapObject *object, int index);
void FieldShadowEffect_UpdateFriendShadow(FieldEffect *effect, FieldShadowEffect *work);
void FieldShadowEffect_DrawFriendShadow(FieldEffect *effect, FieldShadowEffect *work);
void FieldShadow_OffsetByFacing(u32 direction, VecFx32 *pos);

FieldShadow *FieldShadow_New(FieldEffectManager *manager) {
    FieldShadow *shadow = FieldEffectManager_HeapAllocAndFill(manager, sizeof(FieldShadow), FALSE, 0);
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
    FieldSystem *fieldSystem = FieldEffectManager_GetFieldSystem(shadow->manager);
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
    shadow->alpha = sShadowAlphaByTimeOfDay[shadow->timeOfDay];
    shadow->scale = sShadowScaleByTimeOfDay[shadow->timeOfDay];
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
        alpha = sShadowAlphaByTimeOfDay[timeOfDay];
        scale = sShadowScaleByTimeOfDay[timeOfDay];
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
        FieldEffectManager_LoadRenderModel(shadow->manager, &shadow->models[i], 0, sShadowModelFiles[i], 0);
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
    NNSi_G3dModifyPolygonAttrMask(shadow->models[0].model, 1, 0x1F << 16);
    NNS_G3dMdlSetMdlAlphaAll(shadow->models[0].model, alpha);
}

void FieldShadow_CreateMapObjectShadow(LocalMapObject *object) {
    FieldShadowEffectArgs args;
    VecFx32 pos;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(object);
    args.manager = manager;
    args.shadow = FieldEffectManager_GetRendererData(manager, 0);
    args.mapObject = object;
    MapObject_CopyPositionVector(object, &pos);
    FieldEffectManager_CreateEffect(manager, &sMapObjectShadowTemplate, &pos, 0, &args, MapObject_GetPriorityPlusValue(object, 2));
}

void FieldShadow_CreateFollowMonShadow(LocalMapObject *object) {
    FieldShadowEffectArgs args;
    VecFx32 pos;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(object);
    args.manager = manager;
    args.shadow = FieldEffectManager_GetRendererData(manager, 0);
    args.mapObject = object;
    MapObject_CopyPositionVector(object, &pos);
    FieldEffectManager_CreateEffect(manager, &sFollowMonShadowTemplate, &pos, 3, &args, MapObject_GetPriorityPlusValue(object, 2));
}

BOOL FieldShadowEffect_Init(FieldEffect *effect, FieldShadowEffect *work) {
    work->args = *(FieldShadowEffectArgs *)FieldEffect_GetUserData(effect);
    work->parameter = FieldEffect_GetParam(effect);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    if (MapObject_CheckFlag25(work->args.mapObject) == TRUE) {
        work->mapId = MapObject_GetFlag25MapID(work->args.mapObject);
    } else {
        work->mapId = MapObject_GetMapID(work->args.mapObject);
    }
    return TRUE;
}

void FieldShadowEffect_Destroy(FieldEffect *effect, FieldShadowEffect *work) {
}

void FieldShadowEffect_Update(FieldEffect *effect, FieldShadowEffect *work) {
    VecFx32 pos;
    LocalMapObject *object = work->args.mapObject;
    if (!MapObject_CheckSpriteIDAndMap(object, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    if (MapObject_GetManagerFlagsBitsMask(object, (MapObjectFlagBits)8)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    work->hidden = 0;
    if (MapObject_TestFlagsBits(object, (MapObjectFlagBits)0x00100200) == TRUE) {
        work->hidden = 1;
        return;
    }
    MapObject_CopyPositionVector(object, &pos);
    FieldEffect_SetPosition(effect, &pos);
}

BOOL FieldShadowEffect_InitFollowMon(FieldEffect *effect, FieldShadowEffect *work) {
    work->args = *(FieldShadowEffectArgs *)FieldEffect_GetUserData(effect);
    work->parameter = FieldEffect_GetParam(effect);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    if (MapObject_CheckFlag25(work->args.mapObject) == TRUE) {
        work->mapId = MapObject_GetFlag25MapID(work->args.mapObject);
    } else {
        work->mapId = MapObject_GetMapID(work->args.mapObject);
    }
    work->hidden = TRUE;
    return TRUE;
}

void FieldShadowEffect_Draw(FieldEffect *effect, FieldShadowEffect *work) {
    VecFx32 pos;
    VecFx32 scale;
    MtxFx33 mtx;
    FieldShadow *shadow;
    if (work->hidden != 0) {
        return;
    }
    mtx = sShadowDrawRotation;
    shadow = work->args.shadow;
    FieldShadow_GetScale(shadow, &scale);
    FieldEffect_GetPosition(effect, &pos);
    pos.x -= 0x800;
    pos.y -= 0x4000;
    pos.z += 0x1000;
    RenderObj_Draw(&shadow->renderObjs[0], &pos, &scale, &mtx);
}

void FieldShadowEffect_DrawFollowMon(FieldEffect *effect, FieldShadowEffect *work) {
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
    mtx = sFollowMonShadowDrawRotation;
    shadow = work->args.shadow;
    object = work->args.mapObject;
    renderObj = &shadow->renderObjs[0];
    FieldShadow_GetScale(shadow, &scale);
    if (FollowMonBillboard_IsShadowFollowing(object) != 0) {
        FieldEffect_GetPosition(effect, &pos);
        pos.x -= 0x800;
        pos.y -= 0x4000;
        pos.z += 0x1000;
        FieldShadow_OffsetByFacing(MapObject_GetFacingDirection(object), &pos);
    } else {
        MapObject_CopyPositionVector(object, &pos);
        FollowMonBillboard_GetShadowOffset(object, &offset);
        pos.x += offset.x;
        pos.z += offset.z;
        pos.x -= 0x800;
        pos.y -= 0x4000;
        pos.z += 0x1000;
    }
    RenderObj_Draw(renderObj, &pos, &scale, &mtx);
}

void FieldShadow_CreateFriendShadow(LocalMapObject *object, int index) {
    FieldShadowEffectArgs args;
    VecFx32 pos;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(object);
    args.manager = manager;
    args.shadow = FieldEffectManager_GetRendererData(manager, 0);
    args.mapObject = object;
    MapObject_CopyPositionVector(object, &pos);
    FieldEffectManager_CreateEffect(manager, &sFriendShadowTemplate, &pos, index, &args, MapObject_GetPriorityPlusValue(object, 2));
}

void FieldShadowEffect_UpdateFriendShadow(FieldEffect *effect, FieldShadowEffect *work) {
    VecFx32 pos;
    LocalMapObject *object = work->args.mapObject;
    if (!MapObject_CheckSpriteIDAndMap(object, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    work->hidden = 0;
    if (MapObject_TestFlagsBits(object, (MapObjectFlagBits)0x00100200) == TRUE) {
        work->hidden = 1;
        return;
    }
    MapObject_CopyPositionVector(object, &pos);
    FieldEffect_SetPosition(effect, &pos);
}

void FieldShadowEffect_DrawFriendShadow(FieldEffect *effect, FieldShadowEffect *work) {
    VecFx32 pos;

    if (work->hidden == 1) {
        return;
    }
    {
        u32 index = (u32)FieldEffect_GetParam(effect);
        FieldShadow *shadow = work->args.shadow;
        NNSG3dRenderObj *renderObj = &shadow->renderObjs[index];
        FieldEffect_GetPosition(effect, &pos);
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
