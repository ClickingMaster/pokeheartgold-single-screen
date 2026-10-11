#include "global.h"

#include "field/field_effect_manager.h"
#include "field/field_effect_helpers.h"

#include "map_object.h"
#include "render_animation.h"
#include "render_model.h"
#include "map_object_movement_utilities.h"
#include "field_effects.h"

// The mud a map object splashes up when it steps into mud (field effect renderer 7): one animated model
// played once at the tile, then the effect ends.

// Renderer data, from FieldEffectManager_GetRendererData(manager, 7).
typedef struct MudSplash {
    FieldEffectManager *manager;
    UnkOv01_021FFECC_sub model;
    RenderAnimation animation;
} MudSplash;

typedef struct MudSplashEffectArgs {
    FieldEffectManager *manager;
    MudSplash *splash;
} MudSplashEffectArgs;

typedef struct MudSplashEffect {
    RenderAnimation animation;
    NNSG3dRenderObj renderObj;
    MudSplashEffectArgs args;
} MudSplashEffect;

MudSplash *MudSplash_New(FieldEffectManager *manager);
void MudSplash_Free(MudSplash *splash);
void MudSplash_LoadGfx(MudSplash *splash);
void MudSplash_FreeGfx(MudSplash *splash);
void MudSplash_Spawn(LocalMapObject *mapObject, int x, int y, int z);
BOOL MudSplashEffect_Init(FieldEffect *effect, MudSplashEffect *work);
void MudSplashEffect_Destroy(FieldEffect *effect, MudSplashEffect *work);
void MudSplashEffect_Update(FieldEffect *effect, MudSplashEffect *work);
void MudSplashEffect_Draw(FieldEffect *effect, MudSplashEffect *work);

static const FieldEffectBasicTemplate sMudSplashEffectTemplate = {
    sizeof(MudSplashEffect),
    (FieldEffectInitFunc)MudSplashEffect_Init,
    (FieldEffectFunc)MudSplashEffect_Destroy,
    (FieldEffectFunc)MudSplashEffect_Update,
    (FieldEffectFunc)MudSplashEffect_Draw,
};

MudSplash *MudSplash_New(FieldEffectManager *manager) {
    MudSplash *splash = FieldEffectManager_HeapAllocAndFill(manager, sizeof(MudSplash), FALSE, 0);
    splash->manager = manager;
    MudSplash_LoadGfx(splash);
    return splash;
}

void MudSplash_Free(MudSplash *splash) {
    MudSplash_FreeGfx(splash);
    FieldEffectManager_HeapFree(splash);
}

void MudSplash_LoadGfx(MudSplash *splash) {
    FieldEffectManager_LoadRenderModel(splash->manager, &splash->model, 0, 0x1E, FALSE);
    FieldEffectManager_LoadRenderAnimation(splash->manager, &splash->animation, 0, 0x87, FALSE);
}

void MudSplash_FreeGfx(MudSplash *splash) {
    RenderModel_Free(&splash->model);
    RenderAnimation_Free(&splash->animation);
}

void MudSplash_Spawn(LocalMapObject *mapObject, int x, int y, int z) {
    MudSplashEffectArgs args;
    VecFx32 position;
    args.manager = FieldEffectManager_GetFromMapObject(mapObject);
    args.splash = FieldEffectManager_GetRendererData(args.manager, 7);
    SetPositionToTileCenter(x, z, &position);
    position.z += y * 8;
    position.y = MapObject_GetPreviousYCoord(mapObject) * 0x8000 - 0x8000;
    FieldEffectManager_CreateEffect(args.manager, &sMudSplashEffectTemplate, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL MudSplashEffect_Init(FieldEffect *effect, MudSplashEffect *work) {
    MudSplashEffectArgs *args = (MudSplashEffectArgs *)FieldEffect_GetUserData(effect);
    work->args = *args;
    FieldEffectManager_InitRenderAnimation(work->args.manager, &work->animation, &work->args.splash->model, &work->args.splash->animation, 0);
    RenderObj_InitWithAnimation(&work->renderObj, &work->args.splash->model, &work->animation);
    return TRUE;
}

void MudSplashEffect_Destroy(FieldEffect *effect, MudSplashEffect *work) {
    RenderAnimation_Free(&work->animation);
}

void MudSplashEffect_Update(FieldEffect *effect, MudSplashEffect *work) {
    if (RenderAnimation_IsDone(&work->animation) == TRUE) {
        FieldEffectManager_DestroyEffect((int)effect);
    } else {
        RenderAnimation_Advance(&work->animation, FX32_ONE, FALSE);
    }
}

void MudSplashEffect_Draw(FieldEffect *effect, MudSplashEffect *work) {
    VecFx32 position;
    FieldEffect_GetPosition(effect, &position);
    RenderObj_DrawAt((int)&work->renderObj, &position);
}
