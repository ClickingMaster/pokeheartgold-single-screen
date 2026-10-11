#include "field/field_flash_darkness.h"

#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "camera.h"
#include "field_effects.h"
#include "field_system.h"
#include "gf_3d_render.h"
#include "map_object.h"

// The darkness of a cave that needs Flash (field effect renderer 19, used by the WEATHER_FLASH and
// WEATHER_FLASH_2 weather tasks): a model with a lit hole in the middle, kept a third of the way from the
// player to the camera. Using Flash scales it up from 1x to 4x, widening the hole; after Flash it is
// created at 4x.

#define FLASH_DARKNESS_MODEL_FILE 0x6A

// Defined in lib/NitroSDK/src/fx/fx_vec_mult_add.c; nitro/fx/fx_vec.h does not declare it.
fx32 VEC_Distance(const VecFx32 *a, const VecFx32 *b);

// Renderer data, from FieldEffectManager_GetRendererData(manager, 19).
typedef struct FlashDarkness {
    FieldEffectManager *manager;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *model;
    NNSG3dResFileHeader *resource;
    FieldEffect *effect;
    float scale;
} FlashDarkness;

typedef struct FlashDarknessEffectArgs {
    FieldEffectManager *manager;
    FlashDarkness *darkness;
    LocalMapObject *mapObject;
    fx32 distance;
    VecFx32 direction;
} FlashDarknessEffectArgs;

typedef struct FlashDarknessEffect {
    int param;
    FlashDarknessEffectArgs args;
} FlashDarknessEffect;

typedef struct FlashDarknessRotation {
    fx32 m[9];
} FlashDarknessRotation;

FlashDarkness *FlashDarkness_New(FieldEffectManager *manager);
void FlashDarkness_Free(FlashDarkness *darkness);
void FlashDarkness_LoadModel(FlashDarkness *darkness);
void FlashDarkness_FreeModel(FlashDarkness *darkness);
BOOL FlashDarknessEffect_Init(FieldEffect *effect, FlashDarknessEffect *work);
void FlashDarknessEffect_Destroy(void);
void FlashDarknessEffect_Update(FieldEffect *effect, FlashDarknessEffect *work);
void FlashDarknessEffect_Draw(FieldEffect *effect, FlashDarknessEffect *work);

// The effect template stays in asm (asm/overlay_01_02203E40_residual_1.s, linked ahead of this file).
extern const FieldEffectBasicTemplate ov01_022095B4;

FlashDarkness *FlashDarkness_New(FieldEffectManager *manager) {
    FlashDarkness *darkness = FieldEffectManager_HeapAllocAndFill(manager, sizeof(FlashDarkness), FALSE, 0);
    darkness->manager = manager;
    darkness->effect = NULL;
    darkness->scale = 1.0f;
    FlashDarkness_LoadModel(darkness);
    return darkness;
}

void FlashDarkness_Free(FlashDarkness *darkness) {
    FlashDarkness_FreeModel(darkness);
    FieldEffectManager_HeapFree(darkness);
}

void FlashDarkness_LoadModel(FlashDarkness *darkness) {
    FieldEffectManager_LoadModel(darkness->manager, FLASH_DARKNESS_MODEL_FILE, FALSE, &darkness->renderObj, &darkness->model, &darkness->resource);
}

void FlashDarkness_FreeModel(FlashDarkness *darkness) {
    FieldEffectManager_HeapFree(darkness->resource);
}

// Creates the darkness around the map object (the player), or returns the one already shown.
FieldEffect *FlashDarkness_Show(LocalMapObject *mapObject) {
    FlashDarknessEffectArgs args;
    VecFx32 position;
    VecFx32 objectPos;
    VecFx32 cameraPos;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);
    FieldSystem *fieldSystem = FieldEffectManager_GetFieldSystem(manager);

    args.manager = manager;
    args.darkness = FieldEffectManager_GetRendererData(manager, 19);
    if (args.darkness->effect != NULL) {
        return args.darkness->effect;
    }
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &objectPos);
    cameraPos = Camera_GetLookAtCamPos(fieldSystem->camera);
    VEC_Subtract(&cameraPos, &objectPos, &args.direction);
    args.distance = VEC_Distance(&cameraPos, &objectPos);
    args.distance /= 3;
    VEC_Normalize(&args.direction, &args.direction);
    MapObject_CopyPositionVector(mapObject, &position);
    args.darkness->effect = FieldEffectManager_CreateEffect(manager, &ov01_022095B4, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
    return args.darkness->effect;
}

void FlashDarkness_SetScale(FieldEffect *effect, float scale) {
    FlashDarknessEffect *work = FieldEffect_GetState(effect);
    work->args.darkness->scale = scale;
}

BOOL FlashDarknessEffect_Init(FieldEffect *effect, FlashDarknessEffect *work) {
    work->args = *(FlashDarknessEffectArgs *)FieldEffect_GetUserData(effect);
    work->param = FieldEffect_GetParam(effect);
    return TRUE;
}

void FlashDarknessEffect_Destroy(void) {
}

void FlashDarknessEffect_Update(FieldEffect *effect, FlashDarknessEffect *work) {
    VecFx32 objectPos;
    VecFx32 position;
    MapObject_CopyPositionVector(work->args.mapObject, &objectPos);
    VEC_MultAdd(work->args.distance, &work->args.direction, &objectPos, &position);
    FieldEffect_SetPosition(effect, &position);
}

void FlashDarknessEffect_Draw(FieldEffect *effect, FlashDarknessEffect *work) {
    static const FlashDarknessRotation sIdentity = {
        { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE }
    };
    VecFx32 position;
    VecFx32 scale;
    FlashDarknessRotation rotation;

    rotation = sIdentity;
    scale.x = (fx32)(FX32_ONE * work->args.darkness->scale);
    scale.y = (fx32)(FX32_ONE * work->args.darkness->scale);
    scale.z = (fx32)(FX32_ONE * work->args.darkness->scale);
    FieldEffect_GetPosition(effect, &position);
    GF3dRender_DrawModel(&work->args.darkness->renderObj, &position, (MtxFx33 *)&rotation, &scale);
}
