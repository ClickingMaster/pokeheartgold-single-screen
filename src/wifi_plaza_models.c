#include "wifi_plaza_models.h"

#include <string.h>

#include "global.h"

#include "constants/sndseq.h"

#include "assert.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "model_animation.h"
#include "sound_playback.h"
#include "wifi_plaza_map.h"

typedef struct WiFiPlazaModelsSpawn {
    u16 tag;  // map cell tag (WiFiPlazaMap_FindTileTag)
    u16 kind; // prop kind
} WiFiPlazaModelsSpawn;

typedef struct WiFiPlazaMoverSpawn {
    u16 model;
    u16 texture;
} WiFiPlazaMoverSpawn;

typedef struct WiFiPlazaModelsOffset {
    s16 value;
    s16 unused;
} WiFiPlazaModelsOffset;

// OVY_45: drops the texture data of a model file once it is in VRAM
void WiFiPlaza_DisableModelLighting(NNSG3dResFileHeader *file);
// NitroSystem g3d calls without a public declaration
void NNS_G3dMdlSetMdlLightEnableFlagAll(NNSG3dResMdl *model, u32 lightFlags);
void NNS_G3dMdlSetMdlAlphaAll(NNSG3dResMdl *model, u32 alpha);
void NNS_G3dAnmObjDisableID(NNSG3dAnmObj *anmObj, int id);
void NNS_G3dReleaseMdlSet(NNSG3dResMdlSet *mdlSet);

// rodata of this overlay (asm/wifi_plaza_map_rodata.s)
extern const s32 sWiFiPlazaPropTagShiftX[2];
extern const WiFiPlazaModelsOffset ov49_02269A7C[]; // x / z cell offsets in front of a mover,
extern const WiFiPlazaModelsOffset ov49_02269A7E[]; // the z column labelled separately
extern const WiFiPlazaMoverSpawn sWiFiPlazaMoverSpawns[9];
extern const WiFiPlazaModelsSpawn sWiFiPlazaPropSpawns[35];

static inline WiFiPlazaModelsPos WiFiPlazaProp_UnpackPos(u32 packed) {
    WiFiPlazaModelsPos pos;
    pos.x = packed;
    pos.y = packed >> 16;
    return pos;
}

WiFiPlazaModelSet *WiFiPlazaModelSet_Create(int moverCount, int propCount, enum HeapID heapID, enum HeapID unused) {
    WiFiPlazaModelSet *set = Heap_Alloc(heapID, sizeof(WiFiPlazaModelSet));
    memset(set, 0, sizeof(WiFiPlazaModelSet));
    set->movers = Heap_Alloc(heapID, moverCount * sizeof(WiFiPlazaMover));
    set->props = Heap_Alloc(heapID, propCount * sizeof(WiFiPlazaProp));
    memset(set->movers, 0, moverCount * sizeof(WiFiPlazaMover));
    memset(set->props, 0, propCount * sizeof(WiFiPlazaProp));
    set->moverCount = moverCount;
    set->propCount = propCount;
    return set;
}

void WiFiPlazaModelSet_Delete(WiFiPlazaModelSet *set) {
    if (set->loaded) {
        WiFiPlazaModelSet_Unload(set);
    }
    Heap_Free(set->movers);
    Heap_Free(set->props);
    Heap_Free(set);
}

void WiFiPlazaModelSet_Update(WiFiPlazaModelSet *set) {
    int i;
    WiFiPlazaModelSet_UpdateBase(set, &set->baseRes);
    for (i = 0; i < set->moverCount; i++) {
        WiFiPlazaModelSet_UpdateMover(&set->movers[i], &set->moverRes);
    }
    for (i = 0; i < set->propCount; i++) {
        WiFiPlazaModelSet_UpdateProp(set, &set->props[i]);
    }
}

void WiFiPlazaModelSet_Draw(WiFiPlazaModelSet *set) {
    int i;
    GF_ASSERT(set != NULL);
    GF_ASSERT(set->props != NULL);
    GF_ASSERT(set->movers != NULL);
    WiFiPlazaModelSet_DrawBase(set, &set->baseRes);
    for (i = 0; i < set->propCount; i++) {
        WiFiPlazaModelSet_DrawProp(&set->propRes, &set->props[i]);
    }
    for (i = 0; i < set->moverCount; i++) {
        WiFiPlazaModelSet_DrawMover(&set->movers[i], &set->moverRes);
    }
}

void WiFiPlazaModelSet_VBlank(WiFiPlazaModelSet *set) {
}

void WiFiPlazaModelSet_Load(WiFiPlazaModelSet *set, u32 variant, u32 group, enum HeapID heapID, enum HeapID dataHeapID) {
    WiFiPlazaModelsLayout *layout;
    NARC *narc;

    set->layoutGroup = group;
    set->layoutVariant = variant;
    layout = WiFiPlazaModels_LoadLayout(variant, group, heapID);
    narc = NARC_New(NARC_a_2_0_1, heapID);
    HeapExp_FndInitAllocator(&set->allocator, dataHeapID, 4);
    WiFiPlazaModelSet_LoadBase(&set->baseRes, narc, layout, dataHeapID, &set->allocator);
    WiFiPlazaModelSet_LoadProps(&set->propRes, narc, &set->allocator, layout, dataHeapID);
    WiFiPlazaModelSet_LoadMovers(&set->moverRes, narc, &set->allocator, layout, dataHeapID);
    NARC_Delete(narc);
    WiFiPlazaModels_FreeLayout(layout);
    WiFiPlazaModelSet_InitBase(set, &set->baseRes);
    set->loaded = TRUE;
}

void WiFiPlazaModelSet_Unload(WiFiPlazaModelSet *set) {
    WiFiPlazaModelSet_ResetBase(set, &set->baseRes);
    WiFiPlazaModelSet_FreeBase(&set->baseRes, &set->allocator);
    WiFiPlazaModelSet_FreeProps(&set->propRes, &set->allocator);
    WiFiPlazaModelSet_FreeMovers(&set->moverRes, &set->allocator);
    set->loaded = FALSE;
}

void WiFiPlazaModelSet_StartBaseAnim4(WiFiPlazaModelSet *set) {
    if (set->baseRes.animLoaded[4] == TRUE && set->baseAnimActive[4] == FALSE) {
        set->baseAnimActive[4] = TRUE;
        ModelInstance_AddAnimation(&set->baseInstances[0], &set->baseRes.anims[4]);
    }
}

void WiFiPlazaModelSet_StopBaseAnim4(WiFiPlazaModelSet *set) {
    if (set->baseRes.animLoaded[4] == TRUE && set->baseAnimActive[4] == TRUE) {
        ModelInstance_RemoveAnimation(&set->baseInstances[0], &set->baseRes.anims[4]);
        set->baseAnimActive[4] = FALSE;
    }
}

WiFiPlazaMover *WiFiPlazaModelSet_AddMover(WiFiPlazaModelSet *set, s32 model, s32 texture, const VecFx32 *position) {
    WiFiPlazaMover *mover;

    if (model > 2) {
        GF_AssertFail();
    }
    if (texture > 3) {
        GF_AssertFail();
    }
    mover = WiFiPlazaModelSet_AllocMover(set);
    ModelInstance_Init(&mover->instance, &set->moverRes.models[model]);
    ModelInstance_SetVisible(&mover->instance, TRUE);
    WiFiPlazaMover_SetPosition(mover, position);
    {
        VecFx32 zero = { 0, 0, 0 };
        WiFiPlazaMover_SetOffset(mover, &zero);
    }
    mover->texture = texture;
    mover->model = model;
    mover->active = TRUE;
    mover->animPlaying[0] = TRUE;
    mover->animPlaying[2] = TRUE;
    return mover;
}

void WiFiPlazaModelSet_RemoveMover(WiFiPlazaMover *mover) {
    ModelInstance_SetVisible(&mover->instance, FALSE);
    mover->active = FALSE;
}

void WiFiPlazaMover_SetPosition(WiFiPlazaMover *mover, const VecFx32 *position) {
    mover->position = *position;
    ModelInstance_SetPosition(&mover->instance,
        mover->position.x + mover->offset.x,
        mover->position.y + mover->offset.y,
        mover->position.z + mover->offset.z);
}

void WiFiPlazaMover_SetOffset(WiFiPlazaMover *mover, const VecFx32 *offset) {
    mover->offset = *offset;
    ModelInstance_SetPosition(&mover->instance,
        mover->position.x + mover->offset.x,
        mover->position.y + mover->offset.y,
        mover->position.z + mover->offset.z);
}

void WiFiPlazaMover_GetPosition(WiFiPlazaMover *mover, VecFx32 *position) {
    ModelInstance_GetPosition(&mover->instance, &position->x, &position->y, &position->z);
}

void WiFiPlazaMover_SetVisible(WiFiPlazaMover *mover, BOOL visible) {
    ModelInstance_SetVisible(&mover->instance, visible);
}

BOOL WiFiPlazaMover_StartAnim1(WiFiPlazaMover *mover) {
    if (mover->animPlaying[1] == FALSE) {
        mover->animPlaying[1] = TRUE;
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaMover_StartAnim3(WiFiPlazaMover *mover) {
    if (mover->animPlaying[3] == FALSE) {
        mover->animPlaying[3] = TRUE;
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaMover_SetAngle(WiFiPlazaMover *mover, u16 angle) {
    ModelInstance_SetRotation(&mover->instance, angle, 0);
}

BOOL WiFiPlazaMover_IsAnim1Playing(WiFiPlazaMover *mover) {
    return mover->animPlaying[1];
}

BOOL WiFiPlazaMover_IsAnim3Playing(WiFiPlazaMover *mover) {
    return mover->animPlaying[3];
}

WiFiPlazaProp *WiFiPlazaModelSet_AddProp(WiFiPlazaModelSet *set, u32 kind, u32 x, u32 y) {
    register WiFiPlazaModelSet *base = set;
    WiFiPlazaProp *prop;
    s32 i;
    WiFiPlazaModelsPos pos;

    prop = WiFiPlazaModelSet_AllocProp(base);
    ModelInstance_Init(&prop->instance, &base->propRes.models[kind]);
    for (i = 0; i < WIFI_PLAZA_PROP_ANIM_COUNT; i++) {
        void *file = base->propRes.animFiles[kind][i];
        if (file != NULL) {
            ModelAnimation_InitFromFile(&prop->anims[i], (ModelResource *)((u8 *)base->propRes.models + (kind << 4)), file, &base->allocator);
        }
    }
    ModelInstance_SetVisible(&prop->instance, TRUE);
    pos.x = x << 4;
    pos.y = y << 4;
    WiFiPlazaProp_SetPos(prop, pos);
    prop->active = TRUE;
    prop->kind = kind;
    prop->randomDelay = 20;
    prop->animSpeed = FX32_ONE;
    prop->alphaOverride = FALSE;
    prop->alpha = 31;
    prop->restoreAlpha = 31;
    return prop;
}

// Note: this initialises the animations again instead of freeing them.
void WiFiPlazaModelSet_RemoveProp(WiFiPlazaModelSet *set, WiFiPlazaProp *prop) {
    s32 i;

    ModelInstance_SetVisible(&prop->instance, FALSE);
    for (i = 0; i < WIFI_PLAZA_PROP_ANIM_COUNT; i++) {
        u32 kind = prop->kind;
        void *file = set->propRes.animFiles[kind][i];
        if (file != NULL) {
            ModelAnimation_InitFromFile(&prop->anims[i], &set->propRes.models[kind], file, &set->allocator);
        }
    }
    prop->active = FALSE;
}

u16 WiFiPlazaProp_GetKind(WiFiPlazaProp *prop) {
    return prop->kind;
}

void WiFiPlazaProp_SetPos(WiFiPlazaProp *prop, WiFiPlazaModelsPos pos) {
    VecFx32 vec;
    WiFiPlazaMap_PosToVecFx32(&pos.x, &vec.x);
    ModelInstance_SetPosition(&prop->instance, vec.x, vec.y, vec.z);
}

// Cell position packed as x | y << 16.
u32 WiFiPlazaProp_GetPos(WiFiPlazaProp *prop) {
    VecFx32 vec;
    u16 pos[2];
    u32 result;
    ModelInstance_GetPosition(&prop->instance, &vec.x, &vec.y, &vec.z);
    WiFiPlazaMap_VecFx32ToPos(&vec.x, (s16 *)pos);
    result = pos[1];
    result <<= 16;
    result |= pos[0];
    return result;
}

void WiFiPlazaModelSet_StartPropAnim(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot, s32 mode) {
    WiFiPlazaModelSet_StartPropAnimWithCallback(set, prop, slot, mode, NULL);
}

void WiFiPlazaModelSet_StartPropAnimWithCallback(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot, s32 mode, WiFiPlazaPropCallback callback) {
    if (slot >= WIFI_PLAZA_PROP_ANIM_COUNT) {
        GF_AssertFail();
    }
    if (mode >= 7) {
        GF_AssertFail();
    }
    if (prop->kind >= WIFI_PLAZA_PROP_KIND_COUNT) {
        GF_AssertFail();
    }
    if (set->propRes.animFiles[prop->kind][slot] != NULL) {
        if (prop->animActive[slot] == FALSE) {
            ModelInstance_AddAnimation(&prop->instance, &prop->anims[slot]);
        }
        prop->animActive[slot] = TRUE;
        prop->animModes[slot] = mode;
        prop->animCallbacks[slot] = callback;
        prop->animDelays[slot] = 0;
        switch (mode) {
        case WIFI_PLAZA_PROP_ANIM_LOOP:
        case WIFI_PLAZA_PROP_ANIM_ONCE:
        case WIFI_PLAZA_PROP_ANIM_MANUAL:
            prop->animFrames[slot] = 0;
            break;
        case WIFI_PLAZA_PROP_ANIM_LOOP_BACKWARD:
        case WIFI_PLAZA_PROP_ANIM_ONCE_BACKWARD:
            prop->animFrames[slot] = ModelAnimation_GetFrameCount(&prop->anims[slot]);
            break;
        case WIFI_PLAZA_PROP_ANIM_ONCE_REPEATED:
            prop->animFrames[slot] = 0;
            prop->animDelays[slot] = MTRandom() % prop->randomDelay;
            break;
        case WIFI_PLAZA_PROP_ANIM_LOOP_DELAYED:
            prop->animFrames[slot] = 0;
            prop->animDelays[slot] = MTRandom() % prop->randomDelay;
            break;
        }
        ModelAnimation_SetFrame(&prop->anims[slot], prop->animFrames[slot]);
    }
}

void WiFiPlazaModelSet_StopPropAnim(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot) {
    u8 *active;
    u32 offset;
    u32 zero;
    if (slot >= WIFI_PLAZA_PROP_ANIM_COUNT) {
        GF_AssertFail();
    }
    if (prop->kind >= WIFI_PLAZA_PROP_KIND_COUNT) {
        GF_AssertFail();
    }
    if (set->propRes.animFiles[prop->kind][slot] != NULL) {
        active = prop->animActive;
        if (active[slot] == TRUE) {
            ModelInstance_RemoveAnimation(&prop->instance, &prop->anims[slot]);
            u8 *indexed;
            offset = slot << 2;
            zero = 0;
            active[slot] = zero;
            indexed = (u8 *)prop + offset;
            *(u32 *)(indexed + offsetof(WiFiPlazaProp, animFrames)) = zero;
            prop->animModes[slot] = zero;
            prop->animDelays[slot] = zero;
            *(u32 *)(indexed + offsetof(WiFiPlazaProp, animCallbacks)) = zero;
        }
    }
}

void WiFiPlazaModelSet_StopPropAnims(WiFiPlazaModelSet *set, WiFiPlazaProp *prop) {
    int i;
    for (i = 0; i < WIFI_PLAZA_PROP_ANIM_COUNT; i++) {
        if (WiFiPlazaProp_IsAnimActive(prop, i) == TRUE) {
            WiFiPlazaModelSet_StopPropAnim(set, prop, i);
        }
    }
}

BOOL WiFiPlazaModelSet_IsPropAnimActive(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, s32 slot) {
    if (slot >= WIFI_PLAZA_PROP_ANIM_COUNT) {
        GF_AssertFail();
    }
    if (prop->kind >= WIFI_PLAZA_PROP_KIND_COUNT) {
        GF_AssertFail();
    }
    if (set->propRes.animFiles[prop->kind][slot] != NULL) {
        return prop->animActive[slot];
    }
    return FALSE;
}

#pragma force_active on

void WiFiPlazaModelSet_SetPropAnimFrame(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, int slot, fx32 frame) {
    if (slot >= WIFI_PLAZA_PROP_ANIM_COUNT) {
        GF_AssertFail();
    }
    if (prop->kind >= WIFI_PLAZA_PROP_KIND_COUNT) {
        GF_AssertFail();
    }
    if (set->propRes.animFiles[prop->kind][slot] == NULL) {
        GF_AssertFail();
    }
    if (prop->animModes[slot] == WIFI_PLAZA_PROP_ANIM_MANUAL) {
        prop->animFrames[slot] = frame;
        ModelAnimation_SetFrame(&prop->anims[slot], prop->animFrames[slot]);
    }
}

#pragma force_active reset

BOOL WiFiPlazaProp_IsAnimActive(WiFiPlazaProp *prop, s32 slot) {
    if (slot >= WIFI_PLAZA_PROP_ANIM_COUNT) {
        GF_AssertFail();
    }
    if (prop->kind >= WIFI_PLAZA_PROP_KIND_COUNT) {
        GF_AssertFail();
    }
    return prop->animActive[slot];
}

fx32 WiFiPlazaProp_GetAnimFrame(WiFiPlazaProp *prop, s32 slot) {
    if (slot >= WIFI_PLAZA_PROP_ANIM_COUNT) {
        GF_AssertFail();
    }
    if (prop->kind >= WIFI_PLAZA_PROP_KIND_COUNT) {
        GF_AssertFail();
    }
    return prop->animFrames[slot];
}

void WiFiPlazaProp_SetVisible(WiFiPlazaProp *prop, BOOL visible) {
    ModelInstance_SetVisible(&prop->instance, visible);
}

void WiFiPlazaModelSet_SetPropLights(WiFiPlazaModelSet *set, WiFiPlazaProp *prop, u32 lightFlags) {
    if (prop->kind >= WIFI_PLAZA_PROP_KIND_COUNT) {
        GF_AssertFail();
    }
    NNS_G3dMdlSetMdlLightEnableFlagAll(set->propRes.models[prop->kind].model, lightFlags);
}

void WiFiPlazaProp_SetAnimSpeed(WiFiPlazaProp *prop, fx32 speed) {
    prop->animSpeed = speed;
}

void WiFiPlazaProp_SetAlpha(WiFiPlazaProp *prop, u8 alpha, u8 restoreAlpha) {
    prop->alphaOverride = TRUE;
    prop->alpha = alpha;
    prop->restoreAlpha = restoreAlpha;
}

void WiFiPlazaProp_ClearAlpha(WiFiPlazaProp *prop) {
    prop->alphaOverride = FALSE;
}

void WiFiPlazaProp_SetScale(WiFiPlazaProp *prop, fx32 x, fx32 y, fx32 z) {
    ModelInstance_SetScale(&prop->instance, x, y, z);
}

WiFiPlazaModelsLayout *WiFiPlazaModels_LoadLayout(u32 variant, u32 group, enum HeapID heapID) {
    u32 memberNo = variant + group * 5;

    GF_ASSERT(memberNo < 25);
    return GfGfxLoader_LoadFromNarc(NARC_a_2_0_0, memberNo + 1, FALSE, heapID, TRUE);
}

void WiFiPlazaModels_FreeLayout(WiFiPlazaModelsLayout *layout) {
    Heap_Free(layout);
}

#pragma force_active on

void WiFiPlazaModels_LoadModel(ModelResource *model, NARC *narc, int fileId, enum HeapID heapID) {
    WiFiPlazaMap_LoadModelFile(&model->file, narc, fileId, heapID);
    model->set = NNS_G3dGetMdlSet(model->file);
    model->model = NNS_G3dGetMdlByIdx(model->set, 0);
    model->texture = NNS_G3dGetTex(model->file);
    GF3dRender_BindModelSet(model->file, model->texture);
}

#pragma force_active reset

void WiFiPlazaModels_FreeModel(ModelResource *model) {
    ModelResource_Free(model);
}

void WiFiPlazaModels_AdvanceFrameLooped(fx32 *frame, ModelAnimation *anim, fx32 step) {
    fx32 count = ModelAnimation_GetFrameCount(anim);
    fx32 next = *frame + step;

    if (next < count) {
        *frame = next;
    } else {
        *frame = next % count;
    }
}

BOOL WiFiPlazaModels_AdvanceFrame(fx32 *frame, ModelAnimation *anim, fx32 step) {
    fx32 count = ModelAnimation_GetFrameCount(anim);
    fx32 next = *frame + step;

    if (next < count) {
        *frame = next;
        return FALSE;
    }
    *frame = count - FX32_HALF;
    return TRUE;
}

void WiFiPlazaModels_RewindFrameLooped(fx32 *frame, ModelAnimation *anim, fx32 step) {
    fx32 count = ModelAnimation_GetFrameCount(anim);
    fx32 next = *frame - step;

    if (next >= 0) {
        *frame = next;
    } else {
        *frame = count + next;
    }
}

BOOL WiFiPlazaModels_RewindFrame(fx32 *frame, ModelAnimation *anim, fx32 step) {
    fx32 next = *frame - step;
    if (next > 0) {
        *frame = next;
        return FALSE;
    }
    *frame = 0;
    return TRUE;
}

void WiFiPlazaModelSet_LoadBase(WiFiPlazaBaseResources *res, NARC *narc, WiFiPlazaModelsLayout *layout, enum HeapID heapID, NNSFndAllocator *allocator) {
    int i;
    for (i = 0; i < 2; i++) {
        WiFiPlazaModels_LoadModel(&res->models[i], narc, layout->baseModels[i], heapID);
        WiFiPlaza_DisableModelLighting(res->models[i].file);
    }
    for (i = 0; i < WIFI_PLAZA_BASE_ANIM_COUNT; i++) {
        if (layout->baseModels[0] == layout->baseAnims[i]) {
            res->animLoaded[i] = FALSE;
        } else {
            res->animLoaded[i] = TRUE;
            if (i != 3) {
                ModelAnimation_LoadFromNarc(&res->anims[i], &res->models[0], narc, layout->baseAnims[i], heapID, allocator);
            } else {
                ModelAnimation_LoadFromNarc(&res->anims[i], &res->models[1], narc, layout->baseAnims[i], heapID, allocator);
            }
        }
    }
}

void WiFiPlazaModelSet_FreeBase(WiFiPlazaBaseResources *res, NNSFndAllocator *allocator) {
    int i;
    for (i = 0; i < WIFI_PLAZA_BASE_ANIM_COUNT; i++) {
        if (res->animLoaded[i]) {
            ModelAnimation_Free(&res->anims[i], allocator);
            res->animLoaded[i] = FALSE;
        }
    }
    for (i = 0; i < 2; i++) {
        WiFiPlazaModels_FreeModel(&res->models[i]);
    }
}

// Animation 4 is started separately (WiFiPlazaModelSet_StartBaseAnim4).
void WiFiPlazaModelSet_InitBase(WiFiPlazaModelSet *set, WiFiPlazaBaseResources *res) {
    int i;
    set->baseVisible = TRUE;
    for (i = 0; i < 2; i++) {
        ModelInstance_Init(&set->baseInstances[i], &res->models[i]);
        ModelInstance_SetVisible(&set->baseInstances[i], TRUE);
    }
    for (i = 0; i < 4; i++) {
        if (res->animLoaded[i] == TRUE) {
            set->baseAnimActive[i] = TRUE;
            if (i != 3) {
                ModelInstance_AddAnimation(&set->baseInstances[0], &res->anims[i]);
            } else {
                ModelInstance_AddAnimation(&set->baseInstances[1], &res->anims[i]);
            }
        }
    }
}

void WiFiPlazaModelSet_ResetBase(WiFiPlazaModelSet *set, WiFiPlazaBaseResources *res) {
    int i;
    for (i = 0; i < 4; i++) {
        if (set->baseAnimActive[i] == TRUE) {
            set->baseAnimActive[i] = FALSE;
            if (i != 3) {
                ModelInstance_RemoveAnimation(&set->baseInstances[0], &res->anims[i]);
            } else {
                ModelInstance_RemoveAnimation(&set->baseInstances[1], &res->anims[i]);
            }
        }
    }
    set->baseVisible = FALSE;
}

void WiFiPlazaModelSet_UpdateBase(WiFiPlazaModelSet *set, WiFiPlazaBaseResources *res) {
    WiFiPlazaModelSet *cursor; // advanced by one animation slot (4 bytes) per step
    ModelAnimation *anim;
    fx32 *frame;
    ModelAnimation *setAnim;
    volatile int i;
    int next;
    cursor = set;
    i = 0;
    anim = res->anims;
    frame = set->baseAnimFrames;
    setAnim = anim;
    do {
        if (cursor->baseAnimActive[0]) {
            WiFiPlazaModels_AdvanceFrameLooped(frame, anim, FX32_ONE);
            ModelAnimation_SetFrame(setAnim, cursor->baseAnimFrames[0]);
        }
        next = i + 1;
        cursor = (WiFiPlazaModelSet *)((u8 *)cursor + 4);
        anim++;
        frame++;
        setAnim++;
        i = next;
    } while (next < WIFI_PLAZA_BASE_ANIM_COUNT);
}

void WiFiPlazaModelSet_DrawBase(WiFiPlazaModelSet *set, WiFiPlazaBaseResources *unused) {
    int i;
    ModelInstance *instance;
    if (set->baseVisible) {
        for (i = 0, instance = set->baseInstances; i < 2; i++, instance++) {
            ModelInstance_Draw(instance);
        }
    }
}

WiFiPlazaMover *WiFiPlazaModelSet_AllocMover(WiFiPlazaModelSet *set) {
    int i;
    for (i = 0; i < set->moverCount; i++) {
        if (set->movers[i].active == FALSE) {
            return &set->movers[i];
        }
    }
    GF_AssertFail();
    return NULL;
}

#pragma force_active on

void WiFiPlazaModelSet_LoadMovers(WiFiPlazaMoverResources *res, NARC *narc, NNSFndAllocator *allocator, WiFiPlazaModelsLayout *layout, enum HeapID heapID) {
    int i;
    int j;
    u32 k;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            WiFiPlazaMap_LoadModelFile(&res->textures[i][j], narc, layout->moverTextures[i][j], heapID);
        }
    }
    for (i = 0; i < 2; i++) {
        res->models[i].file = GfGfxLoader_LoadFromOpenNarc(narc, layout->moverModels[i], FALSE, heapID, FALSE);
        res->models[i].set = NNS_G3dGetMdlSet(res->models[i].file);
        res->models[i].model = NNS_G3dGetMdlByIdx(res->models[i].set, 0);
        res->models[i].texture = NNS_G3dGetTex(res->textures[i][0]);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < WIFI_PLAZA_MOVER_ANIM_COUNT; j++) {
            if (layout->moverAnims[i][j] != layout->moverModels[0]) {
                ModelAnimation_LoadFromNarc(&res->anims[i][j], &res->models[i], narc, layout->moverAnims[i][j], heapID, allocator);
                if (j >= 1) {
                    for (k = 0; k < layout->moverNodeCounts[i]; k++) {
                        if (k != layout->moverAnimNodes[i][j - 1]) {
                            NNS_G3dAnmObjDisableID(res->anims[i][j].object, k);
                        }
                    }
                }
            }
        }
    }
}

#pragma force_active reset

void WiFiPlazaModelSet_FreeMovers(WiFiPlazaMoverResources *res, NNSFndAllocator *allocator) {
    int i;
    int j;
    u8 *outer;
    u8 *cursor;
    u8 *freeCursor;
    int k;
    NNSG3dResTex *texture;
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;

    i = 0;
    outer = (u8 *)res + offsetof(WiFiPlazaMoverResources, anims);
    do {
        j = 0;
        cursor = outer;
        do {
            ModelAnimation_Free((ModelAnimation *)cursor, allocator);
            j++;
            cursor += sizeof(ModelAnimation);
        } while (j < WIFI_PLAZA_MOVER_ANIM_COUNT);
        outer += sizeof(res->anims[0]);
        i++;
    } while (i < 2);
    freeCursor = (u8 *)res;
    k = 0;
    do {
        Heap_Free(((ModelResource *)freeCursor)->file);
        k++;
        freeCursor += sizeof(ModelResource);
    } while (k < 2);
    i = 0;
    do {
        freeCursor = (u8 *)res;
        k = 0;
        do {
            texture = NNS_G3dGetTex(*(NNSG3dResFileHeader **)(freeCursor + offsetof(WiFiPlazaMoverResources, textures)));
            NNS_G3dTexReleaseTexKey(texture, &texKey, &tex4x4Key);
            NNS_GfdFreeTexVram(texKey);
            NNS_GfdFreeTexVram(tex4x4Key);
            NNS_GfdFreePlttVram(NNS_G3dPlttReleasePlttKey(texture));
            Heap_Free(*(NNSG3dResFileHeader **)(freeCursor + offsetof(WiFiPlazaMoverResources, textures)));
            k++;
            freeCursor += sizeof(NNSG3dResFileHeader *);
        } while (k < 3);
        res = (WiFiPlazaMoverResources *)((u8 *)res + sizeof(res->textures[0]));
        i++;
    } while (i < 2);
}

void WiFiPlazaModelSet_UpdateMover(WiFiPlazaMover *mover, WiFiPlazaMoverResources *res) {
    int i;
    int offset;
    u8 *slot;
    u8 *frame;
    ModelAnimation *anim;

    slot = (u8 *)mover;
    frame = (u8 *)mover;
    offset = (i = 0);
    frame += offsetof(WiFiPlazaMover, animFrames);
    for (; i < WIFI_PLAZA_MOVER_ANIM_COUNT; i++) {
        if (*(BOOL *)(slot + offsetof(WiFiPlazaMover, animPlaying)) != FALSE) {
            switch (i) {
            case 0:
            case 2:
                anim = (ModelAnimation *)((u8 *)res + offsetof(WiFiPlazaMoverResources, anims) + mover->model * sizeof(res->anims[0]) + offset);
                WiFiPlazaModels_AdvanceFrameLooped((fx32 *)frame, anim, FX32_ONE);
                break;
            case 1:
            case 3:
                anim = (ModelAnimation *)((u8 *)res + offsetof(WiFiPlazaMoverResources, anims) + mover->model * sizeof(res->anims[0]) + offset);
                if (WiFiPlazaModels_AdvanceFrame((fx32 *)frame, anim, FX32_ONE) == TRUE) {
                    *(BOOL *)(slot + offsetof(WiFiPlazaMover, animPlaying)) = FALSE;
                    *(fx32 *)(slot + offsetof(WiFiPlazaMover, animFrames)) = 0;
                }
                break;
            default:
                GF_AssertFail();
                break;
            }
        }
        slot += sizeof(BOOL);
        offset += sizeof(ModelAnimation);
        frame += sizeof(fx32);
    }
}

#pragma force_active on

void WiFiPlazaModelSet_DrawMover(WiFiPlazaMover *mover, WiFiPlazaMoverResources *res) {
    int i;
    if (mover->active && WiFiPlazaMap_IsModelInstanceInView(&res->models[mover->model], &mover->instance)) {
        if (mover->texture >= 3) {
            GF_AssertFail();
        }
        if (mover->model >= 2) {
            GF_AssertFail();
        }
        res->models[mover->model].texture = NNS_G3dGetTex(res->textures[mover->model][mover->texture]);
        if (!NNS_G3dBindMdlSet(res->models[mover->model].set, res->models[mover->model].texture)) {
            GF_AssertFail();
        }
        for (i = 0; i < WIFI_PLAZA_MOVER_ANIM_COUNT; i++) {
            if (mover->animPlaying[i]) {
                ModelInstance_AddAnimation(&mover->instance, &res->anims[mover->model][i]);
                ModelAnimation_SetFrame(&res->anims[mover->model][i], mover->animFrames[i]);
            }
        }
        ModelInstance_Draw(&mover->instance);
        for (i = 0; i < WIFI_PLAZA_MOVER_ANIM_COUNT; i++) {
            if (mover->animPlaying[i]) {
                ModelInstance_RemoveAnimation(&mover->instance, &res->anims[mover->model][i]);
            }
        }
        NNS_G3dReleaseMdlSet(res->models[mover->model].set);
    }
}

#pragma force_active reset

WiFiPlazaProp *WiFiPlazaModelSet_AllocProp(WiFiPlazaModelSet *set) {
    int i;
    for (i = 0; i < set->propCount; i++) {
        if (set->props[i].active == FALSE) {
            return &set->props[i];
        }
    }
    GF_AssertFail();
    return NULL;
}

#pragma force_active on

void WiFiPlazaModelSet_LoadProps(WiFiPlazaPropResources *res, NARC *narc, NNSFndAllocator *allocator, WiFiPlazaModelsLayout *layout, volatile enum HeapID heapID) {
    struct {
        u8 *model;
        NARC *narc;
        u8 *files;
        u32 *animIds;
        int i;
    } v;

    int j;
    u32 *animId;
    u8 *file;
    u32 *modelId = (u32 *)layout;

    v.model = (u8 *)res;
    heapID = heapID;
    v.i = 0;
    v.narc = narc;
    v.animIds = modelId;
    v.files = v.model;
    for (; v.i < WIFI_PLAZA_PROP_KIND_COUNT; v.i++) {
        WiFiPlazaModels_LoadModel((ModelResource *)v.model, v.narc, *modelId, heapID);
        if (v.i != 11 && v.i != 12) {
            WiFiPlaza_DisableModelLighting(*(NNSG3dResFileHeader **)v.model);
        }
        animId = v.animIds;
        file = v.files;
        for (j = 0; j < WIFI_PLAZA_PROP_ANIM_COUNT; j++, animId++, file += sizeof(void *)) {
            if (*modelId != animId[WIFI_PLAZA_PROP_KIND_COUNT]) {
                *(void **)((u32)file + offsetof(WiFiPlazaPropResources, animFiles)) = GfGfxLoader_LoadFromOpenNarc(v.narc, animId[WIFI_PLAZA_PROP_KIND_COUNT], FALSE, heapID, FALSE);
            } else {
                *(void **)((u32)file + offsetof(WiFiPlazaPropResources, animFiles)) = NULL;
            }
        }
        v.model += sizeof(ModelResource);
        modelId++;
        v.animIds += WIFI_PLAZA_PROP_ANIM_COUNT;
        v.files += sizeof(res->animFiles[0]);
    }
}

#pragma force_active reset

void WiFiPlazaModelSet_FreeProps(WiFiPlazaPropResources *res, NNSFndAllocator *allocator) {
    int i;
    int j;
    u8 *files;
    u8 *model;
    u8 *file;
    for (i = 0, files = (u8 *)res, model = (u8 *)res; i < WIFI_PLAZA_PROP_KIND_COUNT; files += sizeof(res->animFiles[0]), model += sizeof(ModelResource), i++) {
        for (j = 0, file = files; j < WIFI_PLAZA_PROP_ANIM_COUNT; j++, file += sizeof(void *)) {
            if (*(void **)(file + offsetof(WiFiPlazaPropResources, animFiles))) {
                Heap_Free(*(void **)(file + offsetof(WiFiPlazaPropResources, animFiles)));
                *(void **)(file + offsetof(WiFiPlazaPropResources, animFiles)) = NULL;
            }
        }
        WiFiPlazaModels_FreeModel((ModelResource *)model);
    }
}

void WiFiPlazaModelSet_DrawProp(WiFiPlazaPropResources *res, WiFiPlazaProp *prop) {
    if (prop->active) {
        if (WiFiPlazaMap_IsModelInstanceInView(&res->models[prop->kind], &prop->instance)) {
            if (prop->alphaOverride == TRUE) {
                NNS_G3dMdlSetMdlAlphaAll(res->models[prop->kind].model, prop->alpha);
            }
            ModelInstance_Draw(&prop->instance);
            if (prop->alphaOverride == TRUE) {
                NNS_G3dMdlSetMdlAlphaAll(res->models[prop->kind].model, prop->restoreAlpha);
            }
        }
    }
}

#pragma force_active on

void WiFiPlazaModelSet_UpdateProp(WiFiPlazaModelSet *set, WiFiPlazaProp *prop) {
    int i = 0;
    WiFiPlazaPropCallback callback1;
    ModelAnimation *anim = prop->anims;
    fx32 *frame = prop->animFrames;
    WiFiPlazaProp *cursor = prop;
    ModelAnimation *setAnim = prop->anims;
    int done1, done4;
    WiFiPlazaPropCallback callback4;

    for (; i < WIFI_PLAZA_PROP_ANIM_COUNT; i++, anim++, setAnim++, frame++, cursor = (WiFiPlazaProp *)((u8 *)cursor + 4)) {
        if (prop->animActive[i]) {
            switch (prop->animModes[i]) {
            case WIFI_PLAZA_PROP_ANIM_LOOP:
                WiFiPlazaModels_AdvanceFrameLooped(frame, anim, prop->animSpeed);
                ModelAnimation_SetFrame(setAnim, cursor->animFrames[0]);
                break;
            case WIFI_PLAZA_PROP_ANIM_ONCE:
                done1 = WiFiPlazaModels_AdvanceFrame(frame, anim, prop->animSpeed);
                ModelAnimation_SetFrame(setAnim, cursor->animFrames[0]);
                if (done1 == TRUE) {
                    callback1 = cursor->animCallbacks[0];
                    WiFiPlazaModelSet_StopPropAnim(set, prop, i);
                    if (callback1) {
                        callback1(set, prop);
                    }
                }
                break;
            case WIFI_PLAZA_PROP_ANIM_LOOP_BACKWARD:
                WiFiPlazaModels_RewindFrameLooped(frame, anim, prop->animSpeed);
                ModelAnimation_SetFrame(setAnim, cursor->animFrames[0]);
                break;
            case WIFI_PLAZA_PROP_ANIM_ONCE_BACKWARD:
                done4 = WiFiPlazaModels_RewindFrame(frame, anim, prop->animSpeed);
                ModelAnimation_SetFrame(setAnim, cursor->animFrames[0]);
                if (done4 == TRUE) {
                    callback4 = cursor->animCallbacks[0];
                    WiFiPlazaModelSet_StopPropAnim(set, prop, i);
                    if (callback4) {
                        callback4(set, prop);
                    }
                }
                break;
            case WIFI_PLAZA_PROP_ANIM_ONCE_REPEATED:
                if (prop->animDelays[i]) {
                    prop->animDelays[i]--;
                } else {
                    if (WiFiPlazaModels_AdvanceFrame(frame, anim, prop->animSpeed) == TRUE) {
                        prop->animDelays[i] = MTRandom() % prop->randomDelay;
                        cursor->animFrames[0] = 0;
                    }
                    ModelAnimation_SetFrame(setAnim, cursor->animFrames[0]);
                }
                break;
            case WIFI_PLAZA_PROP_ANIM_LOOP_DELAYED:
                if (prop->animDelays[i]) {
                    prop->animDelays[i]--;
                } else {
                    WiFiPlazaModels_AdvanceFrameLooped(frame, anim, prop->animSpeed);
                    ModelAnimation_SetFrame(setAnim, cursor->animFrames[0]);
                }
                break;
            }
        }
    }
}

#pragma force_active reset

static inline void WiFiPlazaModels_CreateMovers(WiFiPlazaModels *models, VecFx32 *position) {
    int i;

    position->x = 0;
    position->z = 40 * FX32_ONE;
    position->y = 0;
    for (i = 0; i < 9; i++) {
        models->movers[i] = WiFiPlazaModelSet_AddMover(models->set, sWiFiPlazaMoverSpawns[i].model, sWiFiPlazaMoverSpawns[i].texture, position);
        WiFiPlazaMover_SetVisible(models->movers[i], FALSE);
    }
}

#pragma force_active on

// Places a prop on every map cell carrying one of the spawn tags.
WiFiPlazaModels *WiFiPlazaModels_Create(int group, int variant, WiFiPlazaMap *map, enum HeapID heapID, enum HeapID dataHeapID) {
    WiFiPlazaModels *models = Heap_Alloc(heapID, sizeof(WiFiPlazaModels));
    const WiFiPlazaModelsSpawn *spawn;

    struct {
        struct {
            u16 y, x;

            struct {
                u16 x, y;
            } original;

            WiFiPlazaModelsPos moved;
        } cell;

        VecFx32 position;
    } scratch;

    WiFiPlazaPropShow *stageBursts, *stageShows, *effectShows;
    u32 i;
    int ordinal;

    memset(models, 0, sizeof(WiFiPlazaModels));
    models->map = map;
    models->layoutGroup = group;
    models->layoutVariant = variant;
    models->set = WiFiPlazaModelSet_Create(9, 128, heapID, dataHeapID);
    WiFiPlazaModelSet_Load(models->set, variant, group, heapID, dataHeapID);
    for (i = 0, spawn = sWiFiPlazaPropSpawns; i < 35; i++, spawn++) {
        ordinal = 0;
        if (WiFiPlazaMap_FindTileTag(models->map, spawn->tag, &scratch.cell.x, &scratch.cell.y, 0) == TRUE) {
            effectShows = models->effectShows;
            stageShows = models->stageShows;
            stageBursts = models->stageBursts;
            do {
                models->props[models->propCount] = WiFiPlazaModelSet_AddProp(models->set, spawn->kind, scratch.cell.x, scratch.cell.y);
                switch (spawn->kind) {
                case 6:
                case 7:
                case 8:
                    WiFiPlazaModelSet_StartPropAnim(models->set, models->props[models->propCount], 0, WIFI_PLAZA_PROP_ANIM_LOOP);
                    models->matchBooths[spawn->kind - 6] = models->props[models->propCount];
                    break;
                case 0:
                    WiFiPlazaModelSet_StartPropAnim(models->set, models->props[models->propCount], 0, WIFI_PLAZA_PROP_ANIM_LOOP);
                    WiFiPlazaModelSet_StartPropAnim(models->set, models->props[models->propCount], 1, WIFI_PLAZA_PROP_ANIM_LOOP);
                    WiFiPlazaModelSet_StartPropAnim(models->set, models->props[models->propCount], 2, WIFI_PLAZA_PROP_ANIM_LOOP);
                    break;
                case 10:
                    WiFiPlazaModelSet_StartPropAnim(models->set, models->props[models->propCount], 0, WIFI_PLAZA_PROP_ANIM_MANUAL);
                    models->matchSigns[spawn->tag - 0x49] = models->props[models->propCount];
                    break;
                case 11:
                case 12:
                    WiFiPlazaModelSet_StartPropAnim(models->set, models->props[models->propCount], 0, WIFI_PLAZA_PROP_ANIM_MANUAL);
                    break;
                case 13:
                    WiFiPlazaPropShow_Init(&effectShows[models->effectShowCount], models->props[models->propCount]);
                    models->effectShowCount++;
                    GF_ASSERT(models->effectShowCount <= 24);
                    WiFiPlazaProp_SetVisible(models->props[models->propCount], FALSE);
                    if ((u16)(spawn->tag + 0xFFA4) <= 1) { // tag 0x5C or 0x5D
                        WiFiPlazaProp_ApplyTagShift(models->props[models->propCount], spawn->tag);
                    }
                    break;
                case 14:
                    WiFiPlazaPropShow_Init(&stageShows[models->stageShowCount], models->props[models->propCount]);
                    models->stageShowCount++;
                    GF_ASSERT(models->effectShowCount <= 24);
                    WiFiPlazaProp_SetVisible(models->props[models->propCount], FALSE);
                    if ((u16)(spawn->tag + 0xFFA4) <= 1) { // tag 0x5C or 0x5D
                        WiFiPlazaProp_ApplyTagShift(models->props[models->propCount], spawn->tag);
                    }
                    break;
                case 15:
                    models->effectLoops[models->effectLoopCount] = models->props[models->propCount];
                    models->effectLoopCount++;
                    GF_ASSERT(models->effectLoopCount <= 24);
                    WiFiPlazaProp_SetVisible(models->props[models->propCount], FALSE);
                    break;
                case 16:
                    WiFiPlazaPropShow_Init(&stageBursts[models->stageBurstCount], models->props[models->propCount]);
                    models->stageBurstCount++;
                    GF_ASSERT(models->stageBurstCount <= 24);
                    WiFiPlazaProp_SetVisible(models->props[models->propCount], FALSE);
                    {
                        u32 packed = WiFiPlazaProp_GetPos(models->props[models->propCount]);
                        scratch.cell.original.x = (s16)packed;
                        scratch.cell.original.y = (s16)(packed >> 16);
                    }
                    scratch.cell.moved.x = scratch.cell.original.x;
                    scratch.cell.moved.y = scratch.cell.original.y;
                    scratch.cell.moved.x += 8;
                    scratch.cell.moved.y += 20;
                    WiFiPlazaProp_SetPos(models->props[models->propCount], scratch.cell.moved);
                    break;
                default:
                    break;
                }
                models->propCount++;
                ordinal++;
            } while (WiFiPlazaMap_FindTileTag(models->map, spawn->tag, &scratch.cell.x, &scratch.cell.y, ordinal) == TRUE);
        }
    }
    WiFiPlazaModels_CreateMovers(models, &scratch.position);
    models->moverCount = 9;
    return models;
}

#pragma force_active reset

void WiFiPlazaModels_Delete(WiFiPlazaModels *models) {
    int i;
    for (i = 0; i < models->moverCount; i++) {
        WiFiPlazaModelSet_RemoveMover(models->movers[i]);
    }
    for (i = 0; i < models->propCount; i++) {
        WiFiPlazaModelSet_RemoveProp(models->set, models->props[i]);
    }
    WiFiPlazaModelSet_Unload(models->set);
    WiFiPlazaModelSet_Delete(models->set);
    Heap_Free(models);
}

void WiFiPlazaModels_Update(WiFiPlazaModels *models) {
    int i;
    for (i = 0; i < models->effectShowCount; i++) {
        WiFiPlazaPropShow_UpdateEffect(&models->effectShows[i], models->set);
    }
    for (i = 0; i < models->stageShowCount; i++) {
        WiFiPlazaPropShow_UpdateStage(&models->stageShows[i], models->set);
    }
    for (i = 0; i < models->stageBurstCount; i++) {
        WiFiPlazaPropShow_UpdateBurst(&models->stageBursts[i], models->set);
    }
    WiFiPlazaModelSet_Update(models->set);
}

void WiFiPlazaModels_Draw(WiFiPlazaModels *models) {
    WiFiPlazaModelSet_Draw(models->set);
}

void WiFiPlazaModels_VBlank(WiFiPlazaModels *models) {
    WiFiPlazaModelSet_VBlank(models->set);
}

#define WIFI_PLAZA_ASSERT_MOVER(models, index)      \
    do {                                            \
        if ((models)->moverCount <= (u32)(index)) { \
            GF_AssertFail();                        \
        }                                           \
    } while (0)

void WiFiPlazaModels_SetMoverX(WiFiPlazaModels *models, int index, fx32 x) {
    VecFx32 position;
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    position.z = 40 * FX32_ONE;
    position.y = 0;
    position.x = x;
    WiFiPlazaMover_SetPosition(models->movers[index], &position);
}

void WiFiPlazaModels_SetMoverOffset(WiFiPlazaModels *models, int index, const VecFx32 *offset) {
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    WiFiPlazaMover_SetOffset(models->movers[index], offset);
}

void WiFiPlazaModels_GetMoverFrontPos(WiFiPlazaModels *models, int index, int side, VecFx32 *position) {
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    if ((u32)side >= 3) {
        GF_AssertFail();
    }
    WiFiPlazaMover_GetPosition(models->movers[index], position);
    position->x += ov49_02269A7C[side].value << FX32_SHIFT;
    position->z += ov49_02269A7E[side].value << FX32_SHIFT;
    position->y = 40 * FX32_ONE;
}

BOOL WiFiPlazaModels_StartMoverAnim1(WiFiPlazaModels *models, int index) {
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    return WiFiPlazaMover_StartAnim1(models->movers[index]);
}

BOOL WiFiPlazaModels_StartMoverAnim3(WiFiPlazaModels *models, int index) {
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    return WiFiPlazaMover_StartAnim3(models->movers[index]);
}

void WiFiPlazaModels_SetMoverVisible(WiFiPlazaModels *models, int index, BOOL visible) {
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    WiFiPlazaMover_SetVisible(models->movers[index], visible);
}

void WiFiPlazaModels_SetMoverAngle(WiFiPlazaModels *models, int index, u16 angle) {
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    WiFiPlazaMover_SetAngle(models->movers[index], angle);
}

BOOL WiFiPlazaModels_IsMoverAnim1Playing(WiFiPlazaModels *models, int index) {
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    return WiFiPlazaMover_IsAnim1Playing(models->movers[index]);
}

BOOL WiFiPlazaModels_IsMoverAnim3Playing(WiFiPlazaModels *models, int index) {
    WIFI_PLAZA_ASSERT_MOVER(models, index);
    return WiFiPlazaMover_IsAnim3Playing(models->movers[index]);
}

void WiFiPlazaModels_StartBaseAnim4(WiFiPlazaModels *models) {
    WiFiPlazaModelSet_StartBaseAnim4(models->set);
}

void WiFiPlazaModels_StopBaseAnim4(WiFiPlazaModels *models) {
    WiFiPlazaModelSet_StopBaseAnim4(models->set);
}

// The player stepped onto cell (x, y): props of kinds 1-4 on the two cells above it react.
void WiFiPlazaModels_TouchPropsAt(WiFiPlazaModels *models, u8 x, u8 y) {
    int i;
    for (i = 0; i < models->propCount; i++) {
        BOOL left = WiFiPlazaProp_IsAt(models->props[i], x - 1, y - 1);
        BOOL right = WiFiPlazaProp_IsAt(models->props[i], x, y - 1);
        if (left == TRUE || right == TRUE) {
            switch ((u32)WiFiPlazaProp_GetKind(models->props[i])) {
            case 1:
            case 2:
            case 3:
            case 4:
                if (models->layoutVariant == 4) {
                    WiFiPlazaModels_TouchPropQuiet(models, models->props[i]);
                } else {
                    WiFiPlazaModels_TouchProp(models, models->props[i]);
                }
                break;
            }
        }
    }
}

void WiFiPlazaModels_SetEffectActive(WiFiPlazaModels *models, BOOL active) {
    int i;
    for (i = 0; i < models->propCount; i++) {
        switch ((u32)WiFiPlazaProp_GetKind(models->props[i])) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (active == TRUE) {
                WiFiPlazaModelSet_SetPropLights(models->set, models->props[i], 1);
            }
            break;
        case 15:
            if (active == TRUE) {
                WiFiPlazaProp_SetAnimSpeed(models->props[i], FX32_HALF);
                WiFiPlazaModelSet_StartPropAnim(models->set, models->props[i], 0, WIFI_PLAZA_PROP_ANIM_LOOP_DELAYED);
                WiFiPlazaModelSet_StartPropAnim(models->set, models->props[i], 1, WIFI_PLAZA_PROP_ANIM_LOOP_DELAYED);
                WiFiPlazaModelSet_StartPropAnim(models->set, models->props[i], 2, WIFI_PLAZA_PROP_ANIM_LOOP_DELAYED);
                WiFiPlazaProp_SetVisible(models->props[i], TRUE);
            } else {
                WiFiPlazaModelSet_StopPropAnim(models->set, models->props[i], 0);
                WiFiPlazaModelSet_StopPropAnim(models->set, models->props[i], 1);
                WiFiPlazaModelSet_StopPropAnim(models->set, models->props[i], 2);
                WiFiPlazaProp_SetVisible(models->props[i], FALSE);
            }
            break;
        }
    }
}

void WiFiPlazaModels_TriggerStageBursts(WiFiPlazaModels *models, fx32 animSpeed) {
    int i = 0;
    if (i < models->stageBurstCount) {
        WiFiPlazaPropShow *show = models->stageBursts;
        do {
            WiFiPlazaPropShow_Trigger(show, animSpeed);
            i++;
            show++;
        } while (i < models->stageBurstCount);
    }
}

void WiFiPlazaModels_EnableKind1To4Lights(WiFiPlazaModels *models) {
    s32 i;
    for (i = 0; i < models->propCount; i++) {
        switch (WiFiPlazaProp_GetKind(models->props[i])) {
        case 1:
        case 2:
        case 3:
        case 4:
            WiFiPlazaModelSet_SetPropLights(models->set, models->props[i], 1);
            break;
        }
    }
}

// Kind 11 and 12 props: 0 rest, 1 play forward (then loop the second animation),
// 2 play backward back to rest, 3 loop the second animation.
void WiFiPlazaModels_SetPairAnim(WiFiPlazaModels *models, u32 mode) {
    s32 i;
    for (i = 0; i < models->propCount; i++) {
        u32 kind = WiFiPlazaProp_GetKind(models->props[i]);
        switch (kind) {
        case 11:
        case 12:
            switch (mode) {
            case 0:
                WiFiPlazaModelSet_StartPropAnim(models->set, models->props[i], 0, WIFI_PLAZA_PROP_ANIM_MANUAL);
                models->pairAnimState = FALSE;
                break;
            case 1:
                WiFiPlazaModelSet_StartPropAnimWithCallback(models->set, models->props[i], 0, WIFI_PLAZA_PROP_ANIM_ONCE, WiFiPlazaModels_OnPairAnimOpened);
                models->pairAnimState = TRUE;
                break;
            case 2:
                WiFiPlazaModelSet_StartPropAnimWithCallback(models->set, models->props[i], 0, WIFI_PLAZA_PROP_ANIM_ONCE_BACKWARD, WiFiPlazaModels_OnPairAnimClosed);
                WiFiPlazaModelSet_StopPropAnim(models->set, models->props[i], 1);
                models->pairAnimState = FALSE;
                break;
            case 3:
                WiFiPlazaModelSet_StartPropAnim(models->set, models->props[i], 1, WIFI_PLAZA_PROP_ANIM_LOOP);
                WiFiPlazaModelSet_StopPropAnim(models->set, models->props[i], 0);
                models->pairAnimState = TRUE;
                break;
            }
            break;
        }
    }
}

BOOL WiFiPlazaModels_GetPairAnimState(WiFiPlazaModels *models) {
    return models->pairAnimState;
}

void WiFiPlazaModels_ShowEffectProps(WiFiPlazaModels *models, BOOL show, fx32 animSpeed) {
    s32 i;
    for (i = 0; i < models->effectShowCount; i++) {
        WiFiPlazaModels_ShowEffectProp(models, i, show, animSpeed);
    }
}

BOOL WiFiPlazaModels_ShowEffectProp(WiFiPlazaModels *models, u32 index, BOOL show, fx32 animSpeed) {
    u8 *base;
    u32 offset;

    if (index < models->effectShowCount) {
        base = (u8 *)models + offsetof(WiFiPlazaModels, effectShows);
        offset = index * sizeof(WiFiPlazaPropShow);
        if (show) {
            WiFiPlazaPropShow_RequestShow((WiFiPlazaPropShow *)(base + offset), models->set, animSpeed);
        } else {
            WiFiPlazaPropShow_RequestHide((WiFiPlazaPropShow *)(base + offset));
        }
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaModels_TriggerStageShows(WiFiPlazaModels *models, fx32 animSpeed) {
    s32 i;

    for (i = 0; i < models->stageShowCount; i++) {
        WiFiPlazaPropShow_Trigger(&models->stageShows[i], animSpeed);
    }
}

// Rounds the digit frame `n` to fx32 the way the original float conversion did.
#define WIFI_PLAZA_SIGN_FRAME(n) ((s32)(((n) << 2) ? (float)((n) << 14) + 0.5f : (float)((n) << 14) - 0.5f))

#pragma force_active on

// Sign of matchmaking session `match`: shows its player count (frames 4 + count while it recruits),
// blinking every 14 frames while the session runs.
void WiFiPlazaModels_UpdateMatchSign(WiFiPlazaModels *models, int match, u32 playerCount, BOOL exists, BOOL recruiting, BOOL stop) {
    WiFiPlazaProp *sign, *booth;
    booth = models->matchBooths[match];
    sign = models->matchSigns[match];
    if (recruiting == TRUE || exists == TRUE) {
        if (!WiFiPlazaProp_IsAnimActive(booth, 1)) {
            WiFiPlazaModelSet_StartPropAnim(models->set, booth, 1, WIFI_PLAZA_PROP_ANIM_LOOP);
        }
        if (recruiting == TRUE) {
            WiFiPlazaModelSet_SetPropAnimFrame(models->set, sign, 0, WIFI_PLAZA_SIGN_FRAME(playerCount + 4));
            return;
        }
        models->blinkTimer = (models->blinkTimer + 1) % 28;
        if (models->blinkTimer < 14) {
            WiFiPlazaModelSet_SetPropAnimFrame(models->set, sign, 0, WIFI_PLAZA_SIGN_FRAME(playerCount));
        } else {
            WiFiPlazaModelSet_SetPropAnimFrame(models->set, sign, 0, 0);
        }
    } else if (stop) {
        WiFiPlazaModelSet_StopPropAnims(models->set, booth);
        WiFiPlazaModelSet_StopPropAnims(models->set, sign);
        WiFiPlazaModelSet_SetPropLights(models->set, booth, 1);
    } else {
        WiFiPlazaModelSet_StopPropAnim(models->set, booth, 1);
        WiFiPlazaModelSet_SetPropAnimFrame(models->set, sign, 0, 0);
    }
}

#pragma force_active reset

BOOL WiFiPlazaProp_IsAt(WiFiPlazaProp *prop, u8 x, u8 y) {
    s16 cellY = (s16)(y * 16);
    WiFiPlazaModelsPos pos = WiFiPlazaProp_UnpackPos(WiFiPlazaProp_GetPos(prop));

    return pos.x == (s16)(x * 16) && pos.y == cellY;
}

void WiFiPlazaModels_OnPairAnimOpened(WiFiPlazaModelSet *set, WiFiPlazaProp *prop) {
    u32 kind = WiFiPlazaProp_GetKind(prop);
    if (kind != 11 && kind != 12) {
        GF_AssertFail();
    }
    if (kind - 11 <= 1) {
        WiFiPlazaModelSet_StartPropAnim(set, prop, 1, WIFI_PLAZA_PROP_ANIM_LOOP);
    }
}

void WiFiPlazaModels_OnPairAnimClosed(WiFiPlazaModelSet *set, WiFiPlazaProp *prop) {
    u32 kind = WiFiPlazaProp_GetKind(prop);
    if (kind != 11 && kind != 12) {
        GF_AssertFail();
    }
    if (kind - 11 <= 1) {
        WiFiPlazaModelSet_StartPropAnim(set, prop, 0, WIFI_PLAZA_PROP_ANIM_MANUAL);
    }
}

void WiFiPlazaModels_TouchProp(WiFiPlazaModels *models, WiFiPlazaProp *prop) {
    u32 start = FALSE;
    if (!WiFiPlazaModelSet_IsPropAnimActive(models->set, prop, 0)) {
        start = TRUE;
    } else if (WiFiPlazaProp_GetAnimFrame(prop, 0) > 2 * FX32_ONE) {
        start = TRUE;
    }
    if (start == TRUE) {
        WiFiPlazaModels_PlayPropAnimsOnce(models, prop);
        PlaySE(SEQ_SE_PL_EFF02);
    }
}

void WiFiPlazaModels_TouchPropQuiet(WiFiPlazaModels *models, WiFiPlazaProp *prop) {
    u32 start = FALSE;
    if (!WiFiPlazaModelSet_IsPropAnimActive(models->set, prop, 0)) {
        start = TRUE;
    }
    if (start == TRUE) {
        WiFiPlazaModels_PlayPropAnimsOnce(models, prop);
        PlaySE(SEQ_SE_PL_EFF04);
    }
}

void WiFiPlazaModels_PlayPropAnimsOnce(WiFiPlazaModels *models, WiFiPlazaProp *prop) {
    s32 i;
    for (i = 0; i < WIFI_PLAZA_PROP_ANIM_COUNT; i++) {
        WiFiPlazaModelSet_StartPropAnim(models->set, prop, i, WIFI_PLAZA_PROP_ANIM_ONCE);
    }
}

void WiFiPlazaPropShow_Init(WiFiPlazaPropShow *show, WiFiPlazaProp *prop) {
    show->prop = prop;
    show->state = 0;
}

void WiFiPlazaPropShow_UpdateEffect(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set) {
    switch (show->state) {
    case 0:
        break;
    case 1:
        WiFiPlazaPropShow_ShowEffect(show, set);
        show->state = 2;
        break;
    case 2:
        break;
    case 3:
        WiFiPlazaPropShow_StartFade(show);
        show->state = 4;
        break;
    case 4:
        if (WiFiPlazaPropShow_UpdateFade(show) == TRUE) {
            WiFiPlazaPropShow_HideEffect(show, set);
            show->state = 0;
        }
        break;
    }
}

void WiFiPlazaPropShow_RequestShow(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set, fx32 animSpeed) {
    if ((u16)(show->state + 0xFFFF) <= 1) {
        return;
    }
    if ((u16)(show->state + 0xFFFD) <= 1) {
        WiFiPlazaPropShow_HideEffect(show, set);
    }
    show->state = 1;
    show->animSpeed = animSpeed;
}

void WiFiPlazaPropShow_RequestHide(WiFiPlazaPropShow *show) {
    if ((u16)(show->state + 0xFFFD) <= 1) {
        return;
    }
    if (show->state <= 1) {
        show->state = 0;
    } else {
        show->state = 3;
    }
}

void WiFiPlazaPropShow_ShowEffect(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set) {
    WiFiPlazaProp_SetAnimSpeed(show->prop, show->animSpeed);
    WiFiPlazaModelSet_StartPropAnim(set, show->prop, 0, WIFI_PLAZA_PROP_ANIM_LOOP);
    WiFiPlazaModelSet_StartPropAnim(set, show->prop, 1, WIFI_PLAZA_PROP_ANIM_LOOP);
    WiFiPlazaModelSet_StartPropAnim(set, show->prop, 2, WIFI_PLAZA_PROP_ANIM_LOOP);
    WiFiPlazaProp_SetVisible(show->prop, TRUE);
}

void WiFiPlazaPropShow_HideEffect(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set) {
    WiFiPlazaModelSet_StopPropAnim(set, show->prop, 0);
    WiFiPlazaModelSet_StopPropAnim(set, show->prop, 1);
    WiFiPlazaModelSet_StopPropAnim(set, show->prop, 2);
    WiFiPlazaProp_SetVisible(show->prop, FALSE);
    WiFiPlazaProp_ClearAlpha(show->prop);
    WiFiPlazaProp_SetScale(show->prop, FX32_ONE, FX32_ONE, FX32_ONE);
}

void WiFiPlazaPropShow_StartFade(WiFiPlazaPropShow *show) {
    show->fadeTimer = 0;
}

#pragma force_active on

// 10 frames: the animations speed up to twice their speed, alpha falls to 0 and the scale grows by 0.18.
BOOL WiFiPlazaPropShow_UpdateFade(WiFiPlazaPropShow *show) {
    fx32 value;
    register s32 alpha;
    if (show->fadeTimer < 10) {
        show->fadeTimer++;
        value = FX_Div((s32)(((s64)(show->fadeTimer << FX32_SHIFT) * show->animSpeed + 2048) >> 12), 10 << FX32_SHIFT);
        WiFiPlazaProp_SetAnimSpeed(show->prop, value + show->animSpeed);
        alpha = show->fadeTimer * 31 / 10;
        alpha = 31 - alpha;
        WiFiPlazaProp_SetAlpha(show->prop, alpha, 31);
        value = FX_Div((s32)(((s64)(show->fadeTimer << FX32_SHIFT) * 737 + 2048) >> 12), 10 << FX32_SHIFT);
        WiFiPlazaProp_SetScale(show->prop, value + FX32_ONE, value + FX32_ONE, value + FX32_ONE);
        return FALSE;
    }
    return TRUE;
}

#pragma force_active reset

void WiFiPlazaPropShow_Trigger(WiFiPlazaPropShow *show, fx32 animSpeed) {
    u32 state = show->state;
    state += 0xFFFF;
    state <<= 16;
    state >>= 16;
    if (state > 1) {
        show->state = 1;
        show->animSpeed = animSpeed;
    }
}

// Shown once: faded out when the first animation has ended.
void WiFiPlazaPropShow_UpdateStage(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set) {
    switch (show->state) {
    case 0:
        break;
    case 1:
        WiFiPlazaPropShow_ShowStage(show, set);
        show->state = 2;
        break;
    case 2:
        if (WiFiPlazaModelSet_IsPropAnimActive(set, show->prop, 0) == FALSE) {
            show->state = 3;
        }
        break;
    case 3:
        WiFiPlazaPropShow_StartFade(show);
        show->state = 4;
        break;
    case 4:
        if (WiFiPlazaPropShow_UpdateFade(show) == TRUE) {
            WiFiPlazaProp_SetVisible(show->prop, FALSE);
            WiFiPlazaProp_ClearAlpha(show->prop);
            WiFiPlazaProp_SetScale(show->prop, FX32_ONE, FX32_ONE, FX32_ONE);
            show->state = 0;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

void WiFiPlazaPropShow_ShowStage(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set) {
    WiFiPlazaProp_SetAnimSpeed(show->prop, show->animSpeed);
    WiFiPlazaModelSet_StartPropAnim(set, show->prop, 0, WIFI_PLAZA_PROP_ANIM_ONCE);
    WiFiPlazaModelSet_StartPropAnim(set, show->prop, 1, WIFI_PLAZA_PROP_ANIM_ONCE);
    WiFiPlazaProp_SetVisible(show->prop, TRUE);
}

void WiFiPlazaPropShow_UpdateBurst(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set) {
    switch (show->state) {
    case 0:
        break;
    case 1:
        WiFiPlazaPropShow_ShowBurst(show, set);
        show->state = 2;
        break;
    case 2:
        if (WiFiPlazaModelSet_IsPropAnimActive(set, show->prop, 0) == FALSE) {
            show->state = 3;
        }
        break;
    case 3:
        WiFiPlazaPropShow_StartFade(show);
        show->state = 4;
        break;
    case 4:
        if (WiFiPlazaPropShow_UpdateFade(show) == TRUE) {
            WiFiPlazaProp_SetVisible(show->prop, FALSE);
            WiFiPlazaProp_ClearAlpha(show->prop);
            WiFiPlazaProp_SetScale(show->prop, FX32_ONE, FX32_ONE, FX32_ONE);
            show->state = 0;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

void WiFiPlazaPropShow_ShowBurst(WiFiPlazaPropShow *show, WiFiPlazaModelSet *set) {
    WiFiPlazaProp_SetAnimSpeed(show->prop, show->animSpeed);
    WiFiPlazaModelSet_StartPropAnim(set, show->prop, 0, WIFI_PLAZA_PROP_ANIM_ONCE);
    WiFiPlazaModelSet_StartPropAnim(set, show->prop, 1, WIFI_PLAZA_PROP_ANIM_ONCE);
    WiFiPlazaModelSet_StartPropAnim(set, show->prop, 2, WIFI_PLAZA_PROP_ANIM_ONCE);
    WiFiPlazaProp_SetVisible(show->prop, TRUE);
}

// Props on tags 0x5C / 0x5D are moved sideways and 10 pixels up.
void WiFiPlazaProp_ApplyTagShift(WiFiPlazaProp *prop, int tag) {
    WiFiPlazaModelsPos pos;

    GF_ASSERT(tag == 0x5C || tag == 0x5D);
    pos = WiFiPlazaProp_UnpackPos(WiFiPlazaProp_GetPos(prop));
    pos.x += sWiFiPlazaPropTagShiftX[tag - 0x5C];
    pos.y -= 10;
    WiFiPlazaProp_SetPos(prop, pos);
}
