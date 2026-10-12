#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"
#include "field/map_object_models.h"

#include "field_effects.h"
#include "map_object.h"

// The billboard an apricorn tree shows when an apricorn grows back while the tree is on screen (field
// effect renderer 16; spawned by the apricorn tree object when its save state turns to 1): one sequence
// played five times, the middle two at double speed. Its graphics are loaded by the first running effect
// and freed when the last one ends.

#define APRICORN_REGROWTH_RECORD   13
#define APRICORN_REGROWTH_RESOURCE 11
#define APRICORN_REGROWTH_SEQUENCE 11
#define APRICORN_REGROWTH_TEXTURE  12

#define APRICORN_REGROWTH_PLAY_COUNT 5

// Renderer data, from FieldEffectManager_GetRendererData(manager, 16).
typedef struct ApricornRegrowth {
    int refCount;
    BOOL gfxLoaded;
    FieldEffectManager *manager;
} ApricornRegrowth;

typedef struct ApricornRegrowthEffectArgs {
    FieldEffectManager *manager;
    ApricornRegrowth *regrowth;
    u32 unk08;
} ApricornRegrowthEffectArgs;

typedef struct ApricornRegrowthEffect {
    u8 unk00[4];
    int playCount;
    BOOL sequenceDone;
    ApricornRegrowthEffectArgs args;
    void *billboard;
} ApricornRegrowthEffect;

typedef struct ApricornRegrowthSpeeds {
    fx32 speed[APRICORN_REGROWTH_PLAY_COUNT];
} ApricornRegrowthSpeeds;

ApricornRegrowth *ApricornRegrowth_New(FieldEffectManager *manager);
void ApricornRegrowth_Free(ApricornRegrowth *regrowth);
void ApricornRegrowth_Retain(ApricornRegrowth *regrowth);
void ApricornRegrowth_Release(ApricornRegrowth *regrowth);
void ApricornRegrowth_LoadGfx(ApricornRegrowth *regrowth);
void ApricornRegrowth_FreeGfx(ApricornRegrowth *regrowth);
void ApricornRegrowth_LoadGfxIfUnused(ApricornRegrowth *regrowth);
void ApricornRegrowth_FreeGfxIfUnused(ApricornRegrowth *regrowth);
void ApricornRegrowth_Spawn(LocalMapObject *mapObject);
BOOL ApricornRegrowthEffect_Init(FieldEffect *effect, ApricornRegrowthEffect *work);
void ApricornRegrowthEffect_Destroy(FieldEffect *effect, ApricornRegrowthEffect *work);
void ApricornRegrowthEffect_Update(FieldEffect *effect, ApricornRegrowthEffect *work);
void ApricornRegrowthEffect_Draw(FieldEffect *effect, ApricornRegrowthEffect *work);

// The effect template and the record parameters ({ 0, 9, 1, 0, 0, 2 }) stay in asm, linked around this file
// (asm/overlay_01_021FE590_residual_1.s before it, _residual_2.s after it): MWCC would not keep the update
// function's speed table between them.
extern const FieldEffectBasicTemplate ov01_02209084;
extern const u32 ov01_022090AC[6];

ApricornRegrowth *ApricornRegrowth_New(FieldEffectManager *manager) {
    ApricornRegrowth *regrowth = FieldEffectManager_HeapAllocAndFill(manager, sizeof(ApricornRegrowth), FALSE, 0);
    regrowth->manager = manager;
    return regrowth;
}

void ApricornRegrowth_Free(ApricornRegrowth *regrowth) {
    ApricornRegrowth_FreeGfx(regrowth);
    FieldEffectManager_HeapFree(regrowth);
}

void ApricornRegrowth_Retain(ApricornRegrowth *regrowth) {
    regrowth->refCount++;
}

void ApricornRegrowth_Release(ApricornRegrowth *regrowth) {
    regrowth->refCount--;
    if (regrowth->refCount < 0) {
        GF_AssertFail();
    }
}

void ApricornRegrowth_LoadGfx(ApricornRegrowth *regrowth) {
    if (regrowth->gfxLoaded == FALSE) {
        regrowth->gfxLoaded = TRUE;
        FieldEffectManager_LoadBillboardResource(regrowth->manager, APRICORN_REGROWTH_RESOURCE, 0x80);
        FieldEffectManager_LoadBillboardSequence(regrowth->manager, APRICORN_REGROWTH_SEQUENCE, 0x95);
        FieldEffectManager_LoadTexResource(regrowth->manager, APRICORN_REGROWTH_TEXTURE, 0x1A, TRUE);
        FieldEffectManager_AddRecord(regrowth->manager, APRICORN_REGROWTH_RECORD, APRICORN_REGROWTH_RESOURCE, APRICORN_REGROWTH_SEQUENCE, APRICORN_REGROWTH_TEXTURE, 0, ov01_022090AC);
    }
}

void ApricornRegrowth_FreeGfx(ApricornRegrowth *regrowth) {
    if (regrowth->gfxLoaded == TRUE) {
        regrowth->gfxLoaded = FALSE;
        FieldEffectManager_FreeBillboardResource(regrowth->manager, APRICORN_REGROWTH_RESOURCE);
        FieldEffectManager_FreeBillboardSequence(regrowth->manager, APRICORN_REGROWTH_SEQUENCE);
        FieldEffectManager_FreeTexResource(regrowth->manager, APRICORN_REGROWTH_TEXTURE);
        FieldEffectManager_ReleaseRecord(regrowth->manager, APRICORN_REGROWTH_RECORD);
    }
}

void ApricornRegrowth_LoadGfxIfUnused(ApricornRegrowth *regrowth) {
    if (regrowth->refCount == 0) {
        ApricornRegrowth_LoadGfx(regrowth);
    }
}

void ApricornRegrowth_FreeGfxIfUnused(ApricornRegrowth *regrowth) {
    if (regrowth->refCount == 0) {
        ApricornRegrowth_FreeGfx(regrowth);
    }
}

void ApricornRegrowth_Spawn(LocalMapObject *mapObject) {
    VecFx32 position;
    ApricornRegrowthEffectArgs args;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);

    MapObject_GetDrawPosition(mapObject, &position);
    position.z += 8 * FX32_ONE;
    args.manager = manager;
    args.regrowth = FieldEffectManager_GetRendererData(manager, 16);
    FieldEffectManager_CreateEffect(manager, &ov01_02209084, &position, 0, &args, 0xFF);
}

BOOL ApricornRegrowthEffect_Init(FieldEffect *effect, ApricornRegrowthEffect *work) {
    VecFx32 position;
    work->args = *(ApricornRegrowthEffectArgs *)FieldEffect_GetUserData(effect);
    ApricornRegrowth_LoadGfxIfUnused(work->args.regrowth);
    FieldEffect_GetPosition(effect, &position);
    work->billboard = FieldEffectManager_CreateBillboardFromRecord(work->args.manager, APRICORN_REGROWTH_RECORD, &position);
    ApricornRegrowth_Retain(work->args.regrowth);
    return TRUE;
}

void ApricornRegrowthEffect_Destroy(FieldEffect *effect, ApricornRegrowthEffect *work) {
    BillboardObject_Delete(work->billboard);
    ApricornRegrowth_Release(work->args.regrowth);
    ApricornRegrowth_FreeGfxIfUnused(work->args.regrowth);
}

void ApricornRegrowthEffect_Update(FieldEffect *effect, ApricornRegrowthEffect *work) {
    static const ApricornRegrowthSpeeds sSpeeds = {
        { FX32_ONE, FX32_ONE, 2 * FX32_ONE, 2 * FX32_ONE, FX32_ONE }
    };
    ApricornRegrowthSpeeds speeds = sSpeeds;

    if (work->sequenceDone == TRUE) {
        work->sequenceDone = FALSE;
        work->playCount++;
        if (work->playCount >= APRICORN_REGROWTH_PLAY_COUNT) {
            FieldEffectManager_DestroyEffect((int)effect);
            return;
        }
        BillboardObject_SetFrame(work->billboard, 0);
    }
    if (BillboardObject_AdvanceFrame(work->billboard, speeds.speed[work->playCount]) == TRUE) {
        work->sequenceDone = TRUE;
    }
}

void ApricornRegrowthEffect_Draw(FieldEffect *effect, ApricornRegrowthEffect *work) {
    VecFx32 position;
    FieldEffect_GetPosition(effect, &position);
    BillboardObject_SetPosition(work->billboard, &position);
}
