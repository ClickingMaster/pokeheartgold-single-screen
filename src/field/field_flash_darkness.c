#include "field/field_flash_darkness.h"

#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "camera.h"
#include "field_effects.h"
#include "field_system.h"
#include "gf_3d_render.h"
#include "map_object.h"

// The darkness of a cave that needs Flash: a model with a lit hole in the middle, kept a third of the way from the
// player to the camera. Using Flash scales it up from 1x to 4x, widening the hole.

#define FLASH_DARKNESS_MODEL_FILE 0x6A

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
static void FlashDarkness_LoadModel(FlashDarkness *darkness);
static void FlashDarkness_FreeModel(FlashDarkness *darkness);
static BOOL FlashDarknessEffect_Init(FieldEffect *effect, void *state);
static void FlashDarknessEffect_Destroy(FieldEffect *effect, void *state);
static void FlashDarknessEffect_Update(FieldEffect *effect, void *state);
static void FlashDarknessEffect_Draw(FieldEffect *effect, void *state);

static const FieldEffectBasicTemplate sFlashDarknessTemplate = {
    sizeof(FlashDarknessEffect),
    FlashDarknessEffect_Init,
    FlashDarknessEffect_Destroy,
    FlashDarknessEffect_Update,
    FlashDarknessEffect_Draw,
};

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

static void FlashDarkness_LoadModel(FlashDarkness *darkness) {
    FieldEffectManager_LoadModel(darkness->manager, FLASH_DARKNESS_MODEL_FILE, FALSE, &darkness->renderObj, &darkness->model, &darkness->resource);
}

static void FlashDarkness_FreeModel(FlashDarkness *darkness) {
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
    args.darkness->effect = FieldEffectManager_CreateEffect(manager, &sFlashDarknessTemplate, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
    return args.darkness->effect;
}

void FlashDarkness_SetScale(FieldEffect *effect, float scale) {
    FlashDarknessEffect *work = FieldEffect_GetState(effect);
    work->args.darkness->scale = scale;
}

static BOOL FlashDarknessEffect_Init(FieldEffect *effect, void *state) {
    FlashDarknessEffect *work = state;
    work->args = *(FlashDarknessEffectArgs *)FieldEffect_GetUserData(effect);
    work->param = FieldEffect_GetParam(effect);
    return TRUE;
}

static void FlashDarknessEffect_Destroy(FieldEffect *effect, void *state) {
}

static void FlashDarknessEffect_Update(FieldEffect *effect, void *state) {
    FlashDarknessEffect *work = state;
    VecFx32 objectPos;
    VecFx32 position;
    MapObject_CopyPositionVector(work->args.mapObject, &objectPos);
    VEC_MultAdd(work->args.distance, &work->args.direction, &objectPos, &position);
    FieldEffect_SetPosition(effect, &position);
}

static void FlashDarknessEffect_Draw(FieldEffect *effect, void *state) {
    FlashDarknessEffect *work = state;
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
