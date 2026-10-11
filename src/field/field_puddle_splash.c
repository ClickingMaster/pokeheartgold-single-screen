#include "global.h"

#include "field/field_effect_manager.h"
#include "field/field_effect_helpers.h"

#include "map_object.h"
#include "render_animation.h"
#include "render_model.h"
#include "unk_0205FD20.h"
#include "field_effects.h"

// The splash a map object kicks up when it steps into a puddle (field effect renderer 6): one animated
// model played once at the tile, then the effect ends.

// Renderer data, from FieldEffectManager_GetRendererData(manager, 6).
typedef struct PuddleSplash {
    FieldEffectManager *manager;
    UnkOv01_021FFECC_sub model;
    RenderAnimation animation;
} PuddleSplash;

typedef struct PuddleSplashEffectArgs {
    FieldEffectManager *manager;
    PuddleSplash *splash;
} PuddleSplashEffectArgs;

typedef struct PuddleSplashEffect {
    RenderAnimation animation;
    NNSG3dRenderObj renderObj;
    PuddleSplashEffectArgs args;
} PuddleSplashEffect;

PuddleSplash *PuddleSplash_New(FieldEffectManager *manager);
void PuddleSplash_Free(PuddleSplash *splash);
void PuddleSplash_LoadGfx(PuddleSplash *splash);
void PuddleSplash_FreeGfx(PuddleSplash *splash);
void PuddleSplash_Spawn(LocalMapObject *mapObject, int x, int y, int z);
BOOL PuddleSplashEffect_Init(FieldEffect *effect, PuddleSplashEffect *work);
void PuddleSplashEffect_Destroy(FieldEffect *effect, PuddleSplashEffect *work);
void PuddleSplashEffect_Update(FieldEffect *effect, PuddleSplashEffect *work);
void PuddleSplashEffect_Draw(FieldEffect *effect, PuddleSplashEffect *work);

static const FieldEffectBasicTemplate sPuddleSplashEffectTemplate = {
    sizeof(PuddleSplashEffect),
    (FieldEffectInitFunc)PuddleSplashEffect_Init,
    (FieldEffectFunc)PuddleSplashEffect_Destroy,
    (FieldEffectFunc)PuddleSplashEffect_Update,
    (FieldEffectFunc)PuddleSplashEffect_Draw,
};

PuddleSplash *PuddleSplash_New(FieldEffectManager *manager) {
    PuddleSplash *splash = FieldEffectManager_HeapAllocAndFill(manager, sizeof(PuddleSplash), FALSE, 0);
    splash->manager = manager;
    PuddleSplash_LoadGfx(splash);
    return splash;
}

void PuddleSplash_Free(PuddleSplash *splash) {
    PuddleSplash_FreeGfx(splash);
    FieldEffectManager_HeapFree(splash);
}

void PuddleSplash_LoadGfx(PuddleSplash *splash) {
    FieldEffectManager_LoadRenderModel(splash->manager, &splash->model, 0, 0x1D, FALSE);
    FieldEffectManager_LoadRenderAnimation(splash->manager, &splash->animation, 0, 0x86, FALSE);
}

void PuddleSplash_FreeGfx(PuddleSplash *splash) {
    RenderModel_Free(&splash->model);
    RenderAnimation_Free(&splash->animation);
}

void PuddleSplash_Spawn(LocalMapObject *mapObject, int x, int y, int z) {
    PuddleSplashEffectArgs args;
    VecFx32 position;

    if (MapObject_TestFlagsBits(mapObject, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        return;
    }
    args.manager = FieldEffectManager_GetFromMapObject(mapObject);
    args.splash = FieldEffectManager_GetRendererData(args.manager, 6);
    sub_020611C8(x, z, &position);
    position.z += y * 8 + 0x1000;
    position.y = (MapObject_GetPreviousYCoord(mapObject) << 15) - 0x8000;
    FieldEffectManager_CreateEffect(args.manager, &sPuddleSplashEffectTemplate, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL PuddleSplashEffect_Init(FieldEffect *effect, PuddleSplashEffect *work) {
    PuddleSplashEffectArgs *args = (PuddleSplashEffectArgs *)FieldEffect_GetUserData(effect);
    work->args = *args;
    FieldEffectManager_InitRenderAnimation(work->args.manager, &work->animation, &work->args.splash->model, &work->args.splash->animation, 0);
    RenderObj_InitWithAnimation(&work->renderObj, &work->args.splash->model, &work->animation);
    return TRUE;
}

void PuddleSplashEffect_Destroy(FieldEffect *effect, PuddleSplashEffect *work) {
    RenderAnimation_Free(&work->animation);
}

void PuddleSplashEffect_Update(FieldEffect *effect, PuddleSplashEffect *work) {
    if (RenderAnimation_IsDone(&work->animation) == TRUE) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    RenderAnimation_Advance(&work->animation, FX32_ONE, FALSE);
}

void PuddleSplashEffect_Draw(FieldEffect *effect, PuddleSplashEffect *work) {
    VecFx32 position;
    FieldEffect_GetPosition(effect, &position);
    RenderObj_DrawAt((int)&work->renderObj, &position);
}
