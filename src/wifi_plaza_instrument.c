#include "wifi_plaza_instrument.h"

#include <nitro/fx/fx_trig.h>
#include <string.h>

#include "global.h"

#include "assert.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "model_animation.h"
#include "sound_playback.h"
#include "system.h"
#include "wifi_plaza_camera.h"
#include "wifi_plaza_light.h"
#include "wifi_plaza_map.h"
#include "wifi_plaza_obj.h"

// Entry of the type 12-14 sequences (12 steps each): show `model` at animation frame `frame`, 0 = hidden
typedef struct WiFiPlazaInstrumentStep {
    u16 model;
    u16 frame;
} WiFiPlazaInstrumentStep;

typedef void (*WiFiPlazaInstrumentBurstStartFunc)(WiFiPlazaInstrumentBurst *burst, WiFiPlazaObj *obj);
typedef BOOL (*WiFiPlazaInstrumentBurstUpdateFunc)(WiFiPlazaInstrumentBurst *burst);

// rodata of this overlay (asm/wifi_plaza_map_rodata.s)
extern const u8 sWiFiPlazaInstrumentBalloonHeights[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Bell01[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Cymbal02[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Drum01[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Bell02[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Cymbal03[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Cymbal01[];
extern const WiFiPlazaInstrumentBurstUpdateFunc sWiFiPlazaInstrumentBurstUpdateFuncs[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Drum02[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Bell03[];
extern const WiFiPlazaInstrumentBurstEvent sWiFiPlazaInstrumentBursts_Drum03[];
extern const WiFiPlazaInstrumentBurstStartFunc sWiFiPlazaInstrumentBurstStartFuncs[];
extern const VecFx32 sWiFiPlazaInstrumentSparkleStarts[];
extern const VecFx32 sWiFiPlazaInstrumentSparkleFalls[];
extern const WiFiPlazaInstrumentUpdateFunc sWiFiPlazaInstrumentUpdateFuncs[];
extern const WiFiPlazaInstrumentInitFunc sWiFiPlazaInstrumentInitFuncs[];
extern const u32 sWiFiPlazaInstrumentStartSEs[];
extern const WiFiPlazaInstrumentStep sWiFiPlazaInstrumentSignalSteps[][12];
extern const WiFiPlazaInstrumentStep ov49_0226A67C[][12];
extern const WiFiPlazaInstrumentModelDesc sWiFiPlazaInstrumentModelDescs[];
extern const WiFiPlazaInstrumentModelDesc sWiFiPlazaInstrumentPopModel1[];
extern const WiFiPlazaInstrumentModelDesc sWiFiPlazaInstrumentPopModel2[];
extern const WiFiPlazaInstrumentModelDesc sWiFiPlazaInstrumentPopModel3[];
extern const WiFiPlazaInstrumentModelDesc sWiFiPlazaInstrumentPopModel0[];
extern const WiFiPlazaInstrumentModelDesc sWiFiPlazaInstrumentBalloonModel0[];
extern const WiFiPlazaInstrumentModelDesc sWiFiPlazaInstrumentBalloonModel1[];
extern const WiFiPlazaInstrumentModelDesc sWiFiPlazaInstrumentBalloonModel2[];
extern const u8 sWiFiPlazaInstrumentBurstModels01[];
extern const u8 sWiFiPlazaInstrumentBurstModels02[];
extern const u8 sWiFiPlazaInstrumentBurstModels03[];
extern const u16 sWiFiPlazaLightColorSets[][5];
extern const u16 sWiFiPlazaLight1Colors[];

// library and runtime functions without a public prototype
s64 _ll_mul(s64 a, s64 b);
u32 _fflt(s32 value);
u32 _fadd(u32 a, u32 b);
s32 _ffix(u32 value);
BOOL PlaySEOnPlayer(u16 seqNo, int playerNo);
void NNS_G3dMdlSetMdlEmiAll(NNSG3dResMdl *model, u32 color);
void NNS_G3dMdlSetMdlAlphaAll(NNSG3dResMdl *model, u32 alpha);
void NNS_G3dMdlSetMdlPolygonIDAll(NNSG3dResMdl *model, u32 polygonID);
BOOL NNS_G3dForceBindMdlTex(NNSG3dResMdl *model, NNSG3dResTex *texture, u32 texToMatListIdx, u32 texIdx);
BOOL NNS_G3dForceBindMdlPltt(NNSG3dResMdl *model, NNSG3dResTex *texture, u32 plttToMatListIdx, u32 plttIdx);
void NNS_G3dReleaseMdlTex(NNSG3dResMdl *model);
void NNS_G3dReleaseMdlPltt(NNSG3dResMdl *model);

WiFiPlazaInstrumentManager *WiFiPlazaInstrument_Create(struct WiFiPlazaWork *lobby, WiFiPlazaMap *map, WiFiPlazaCamera *camera, WiFiPlazaObjManager *objManager, enum HeapID heapID, enum HeapID gfxHeapID) {
    NARC *narc;
    WiFiPlazaInstrumentManager *manager = Heap_Alloc(heapID, sizeof(WiFiPlazaInstrumentManager));

    memset(manager, 0, sizeof(WiFiPlazaInstrumentManager));
    manager->lobby = lobby;
    manager->objManager = objManager;
    manager->camera = camera;
    manager->map = map;
    narc = NARC_New(NARC_a_2_0_7, heapID);
    HeapExp_FndInitAllocator(&manager->allocator, gfxHeapID, 4);
    WiFiPlazaInstrument_LoadModels(manager, narc, gfxHeapID);
    WiFiPlazaInstrument_LoadTextures(manager, narc, gfxHeapID);
    WiFiPlazaInstrument_LoadAnimations(manager, narc, gfxHeapID);
    NARC_Delete(narc);
    return manager;
}

void WiFiPlazaInstrument_Delete(WiFiPlazaInstrumentManager *manager) {
    WiFiPlazaInstrument_FreeModels(manager);
    WiFiPlazaInstrument_FreeTextures(manager);
    WiFiPlazaInstrument_FreeAnimations(manager);
    Heap_Free(manager);
}

void WiFiPlazaInstrument_Update(WiFiPlazaInstrumentManager *manager) {
    int i;
    u8 *effect = (u8 *)manager;
    u32 stride = 0xD1;

    i = 0;
    effect += 0x10;
    stride <<= 4; // sizeof(WiFiPlazaInstrumentEffect)
    for (; i < WIFI_PLAZA_INSTRUMENT_PLAYERS; i++, effect += stride) {
        WiFiPlazaInstrument_UpdateEffect(manager, (WiFiPlazaInstrumentEffect *)effect);
    }
}

void WiFiPlazaInstrument_Draw(WiFiPlazaInstrumentManager *manager) {
    int i;
    u8 *effect;
    u32 stride;

    WiFiPlazaObj_GetOwnPlayer(manager->objManager);
    effect = (u8 *)manager;
    stride = 0xD1;
    i = 0;
    effect += 0x10;
    stride <<= 4; // sizeof(WiFiPlazaInstrumentEffect)
    for (; i < WIFI_PLAZA_INSTRUMENT_PLAYERS; i++, effect += stride) {
        WiFiPlazaInstrument_DrawEffect(manager, (WiFiPlazaInstrumentEffect *)effect);
    }
}

void WiFiPlazaInstrument_Start(WiFiPlazaInstrumentManager *manager, u32 index, u32 type) {
    WiFiPlazaObj *obj;

    if (index >= WIFI_PLAZA_INSTRUMENT_PLAYERS) {
        GF_AssertFail();
    }
    obj = WiFiPlazaObj_FindById(manager->objManager, index);
    if (obj != NULL) {
        WiFiPlazaInstrument_StartEffect(manager, &manager->effects[index], obj, type);
    }
}

BOOL WiFiPlazaInstrument_IsFinished(WiFiPlazaInstrumentManager *manager, u32 index) {
    if (WiFiPlazaInstrument_IsEffectActive(&manager->effects[index]) != TRUE) {
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaInstrumentTween_Init(WiFiPlazaInstrumentTween *tween, fx32 x0, fx32 x1, fx32 y0, fx32 y1, fx32 z0, fx32 z1, s32 duration) {
    tween->duration = duration;
    tween->x = x0;
    tween->deltaX = x1 - x0;
    tween->startX = x0;
    tween->y = y0;
    tween->deltaY = y1 - y0;
    tween->startY = y0;
    tween->z = z0;
    tween->deltaZ = z1 - z0;
    tween->startZ = z0;
}

// Returns TRUE once `frame` reached the duration (the position is then clamped to the end point).
BOOL WiFiPlazaInstrumentTween_Update(WiFiPlazaInstrumentTween *tween, s32 frame) {
    float durationX;
    float durationY;
    float durationZ;
    BOOL finished = FALSE;
    s32 delta;
    s32 scaled;

    if (frame > tween->duration) {
        frame = tween->duration;
        finished = TRUE;
    }

    delta = tween->deltaX;
    if (delta != 0) {
        if (tween->duration > 0) {
            durationX = (float)(tween->duration << 12) + 0.5f;
        } else {
            durationX = (float)(tween->duration << 12) - 0.5f;
        }
        scaled = (s32)((_ll_mul((s64)(s32)(frame > 0 ? (float)(frame << 12) + 0.5f : (float)(frame << 12) - 0.5f), (s64)delta) + 0x800) >> 12);
        tween->x = tween->startX + FX_Div(scaled, (s32)durationX);
    }

    delta = tween->deltaY;
    if (delta != 0) {
        if (tween->duration > 0) {
            durationY = (float)(tween->duration << 12) + 0.5f;
        } else {
            durationY = (float)(tween->duration << 12) - 0.5f;
        }
        scaled = (s32)((_ll_mul((s64)(s32)(frame > 0 ? (float)(frame << 12) + 0.5f : (float)(frame << 12) - 0.5f), (s64)delta) + 0x800) >> 12);
        tween->y = tween->startY + FX_Div(scaled, (s32)durationY);
    }

    delta = tween->deltaZ;
    if (delta != 0) {
        if (tween->duration > 0) {
            durationZ = (float)(tween->duration << 12) + 0.5f;
        } else {
            durationZ = (float)(tween->duration << 12) - 0.5f;
        }
        scaled = (s32)((_ll_mul((s64)(s32)(frame > 0 ? (float)(frame << 12) + 0.5f : (float)(frame << 12) - 0.5f), (s64)delta) + 0x800) >> 12);
        tween->z = tween->startZ + FX_Div(scaled, (s32)durationZ);
    }
    return finished;
}

void WiFiPlazaInstrumentTween_GetPosition(WiFiPlazaInstrumentTween *tween, fx32 *x, fx32 *y, fx32 *z) {
    *x = tween->x;
    *y = tween->y;
    *z = tween->z;
}

void WiFiPlazaInstrumentWave_Init(WiFiPlazaInstrumentWave *wave, int angle, int step, int amplitude) {
    s32 sin;

    wave->angle = angle;
    sin = FX_SinCosTable_[(angle >> 4) * 2];
    wave->step = step;
    wave->amplitude = amplitude;
    wave->value = (s32)(((s64)sin * wave->amplitude + 0x800) >> 12);
}

void WiFiPlazaInstrumentWave_Update(WiFiPlazaInstrumentWave *wave) {
    s32 sin;

    wave->angle += wave->step;
    sin = FX_SinCosTable_[(wave->angle >> 4) * 2];
    wave->value = (s32)(((s64)sin * wave->amplitude + 0x800) >> 12);
}

void WiFiPlazaInstrumentWave_GetValue(WiFiPlazaInstrumentWave *wave, fx32 *value) {
    *value = wave->value;
}

// The player's own effects are played on SE player 5; other players' only when their object is not hidden.
void WiFiPlazaInstrument_PlaySE(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 seqNo) {
    WiFiPlazaObj *obj = effect->obj;
    u32 seq = seqNo;
    WiFiPlazaObj *ownPlayer = WiFiPlazaObj_GetOwnPlayer(manager->objManager);

    if (obj == ownPlayer) {
        PlaySEOnPlayer((u16)seq, 5);
    } else if (WiFiPlazaObj_IsCulled(obj) == 0) {
        PlaySE((u16)seq);
    }
}

void WiFiPlazaInstrument_LoadModels(WiFiPlazaInstrumentManager *manager, NARC *narc, enum HeapID heapID) {
    int i;

    for (i = 0; i < 15; i++, manager = (WiFiPlazaInstrumentManager *)((u8 *)manager + sizeof(ModelResource))) {
        manager->models[0].file = GfGfxLoader_LoadFromOpenNarc(narc, i + 0x81, FALSE, heapID, FALSE);
        manager->models[0].set = NNS_G3dGetMdlSet(manager->models[0].file);
        manager->models[0].model = NNS_G3dGetMdlByIdx(manager->models[0].set, 0);
        manager->models[0].texture = NULL;
        NNS_G3dMdlSetMdlEmiAll(manager->models[0].model, 0x7FFF);
    }
}

void WiFiPlazaInstrument_FreeModels(WiFiPlazaInstrumentManager *manager) {
    int i;

    for (i = 0; i < 15; i++, manager = (WiFiPlazaInstrumentManager *)((u8 *)manager + sizeof(ModelResource))) {
        Heap_Free(manager->models[0].file);
    }
}

void WiFiPlazaInstrument_LoadTextures(WiFiPlazaInstrumentManager *manager, NARC *narc, enum HeapID heapID) {
    int i = 0;
    NNSG3dResFileHeader **file = manager->textures;

    for (; i < 39; i++, file++) {
        WiFiPlazaMap_LoadModelFile(file, narc, i + 0x90, heapID);
    }
}

void WiFiPlazaInstrument_FreeTextures(WiFiPlazaInstrumentManager *manager) {
    int i;

    for (i = 0; i < 39; i++, manager = (WiFiPlazaInstrumentManager *)((u8 *)manager + sizeof(NNSG3dResFileHeader *))) {
        NNSG3dResTex *texture = NNS_G3dGetTex(manager->textures[0]);
        NNSGfdTexKey key;
        NNSGfdTexKey key4x4;

        NNS_G3dTexReleaseTexKey(texture, &key, &key4x4);
        NNS_GfdDefaultFuncFreeTexVram(key);
        NNS_GfdDefaultFuncFreeTexVram(key4x4);
        NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(texture));
        Heap_Free(manager->textures[0]);
    }
}

#pragma force_active on

void WiFiPlazaInstrument_LoadAnimations(WiFiPlazaInstrumentManager *manager, NARC *narc, enum HeapID heapID) {
    int i, j;
    const WiFiPlazaInstrumentModelDesc *desc = sWiFiPlazaInstrumentModelDescs;

    for (i = 0; i < 39; i++) {
        for (j = 0; j < 2; j++) {
            if (desc->anims[j] == WIFI_PLAZA_INSTRUMENT_NO_ANIM) {
                continue;
            }
            if (manager->anims[desc->anims[j]].file != NULL) {
                continue;
            }
            manager->models[desc->model].texture = NNS_G3dGetTex(manager->textures[desc->texture]);
            ModelAnimation_LoadFromNarc(&manager->anims[desc->anims[j]], &manager->models[desc->model], narc, desc->anims[j] + 0xB7, heapID, &manager->allocator);
        }
        desc++;
    }
}

#pragma force_active reset

void WiFiPlazaInstrument_FreeAnimations(WiFiPlazaInstrumentManager *manager) {
    int i;
    ModelAnimation *anim = manager->anims;
    NNSFndAllocator *allocator = &manager->allocator;

    for (i = 0; i < 16; i++, manager = (WiFiPlazaInstrumentManager *)((u8 *)manager + sizeof(ModelAnimation)), anim++) {
        if (manager->anims[0].file != NULL) {
            ModelAnimation_Free(anim, allocator);
            manager->anims[0].file = NULL;
        }
    }
}

void WiFiPlazaInstrument_StartEffect(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, WiFiPlazaObj *obj, u32 type) {
    if (type >= WIFI_PLAZA_INSTRUMENT_EFFECT_TYPES) {
        GF_AssertFail();
    }
    WiFiPlazaInstrument_ClearEffect(manager, effect);
    effect->obj = obj;
    effect->type = type;
    effect->polygonID = WiFiPlazaObj_GetParam(obj, 5) + 0x28;
    sWiFiPlazaInstrumentInitFuncs[effect->type](manager, effect);
    WiFiPlazaInstrument_PlaySE(manager, effect, sWiFiPlazaInstrumentStartSEs[effect->type]);
}

void WiFiPlazaInstrument_UpdateEffect(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    if (WiFiPlazaInstrument_IsEffectActive(effect) != FALSE) {
        if (effect->type >= WIFI_PLAZA_INSTRUMENT_EFFECT_TYPES) {
            GF_AssertFail();
        }
        if (sWiFiPlazaInstrumentUpdateFuncs[effect->type](manager, effect) == TRUE) {
            WiFiPlazaInstrument_ClearEffect(manager, effect);
        }
    }
}

BOOL WiFiPlazaInstrument_DrawEffect(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    if (WiFiPlazaInstrument_IsEffectActive(effect) == FALSE) {
        return FALSE;
    }
    if (effect->type >= WIFI_PLAZA_INSTRUMENT_EFFECT_TYPES) {
        GF_AssertFail();
    }
    return WiFiPlazaInstrument_DrawEffectModels(manager, effect);
}

void WiFiPlazaInstrument_ClearEffect(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    memset(effect, 0, sizeof(WiFiPlazaInstrumentEffect));
}

BOOL WiFiPlazaInstrument_IsEffectActive(WiFiPlazaInstrumentEffect *effect) {
    return effect->obj != NULL;
}

BOOL WiFiPlazaInstrument_HasModel(WiFiPlazaInstrumentEffect *effect, u32 index) {
    return effect->descs[index] != NULL;
}

void WiFiPlazaInstrument_SetModel(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, const WiFiPlazaInstrumentModelDesc *desc) {
    u32 offset;
    const WiFiPlazaInstrumentModelDesc **descs;

    if (index >= WIFI_PLAZA_INSTRUMENT_EFFECT_MODELS) {
        GF_AssertFail();
    }
    descs = effect->descs;
    offset = index * 4;
    if (*(const WiFiPlazaInstrumentModelDesc **)((u8 *)descs + offset) != NULL) {
        GF_AssertFail();
    }
    *(const WiFiPlazaInstrumentModelDesc **)((u8 *)descs + offset) = desc;
    ModelInstance_Init((ModelInstance *)((u8 *)effect + 0xC + index * sizeof(ModelInstance)), (ModelResource *)((u8 *)manager + 0x10550 + desc->model * sizeof(ModelResource)));
}

#pragma force_active on

// Binds the slot's texture and animations to the shared model, draws it and unbinds them again.
void WiFiPlazaInstrument_DrawModel(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index) {
    ModelResource *model;
    int i;
    u8 anim;

    GF_ASSERT(effect->descs[index] != NULL);
    if (ModelInstance_IsVisible(&effect->models[index])) {
        model = &manager->models[effect->descs[index]->model];
        model->texture = NNS_G3dGetTex(manager->textures[effect->descs[index]->texture]);
        GF_ASSERT(NNS_G3dForceBindMdlTex(model->model, model->texture, 0, 0));
        GF_ASSERT(NNS_G3dForceBindMdlPltt(model->model, model->texture, 0, 0));
        for (i = 0; i < 2; i++) {
            anim = effect->descs[index]->anims[i];
            if (anim == WIFI_PLAZA_INSTRUMENT_NO_ANIM) {
                break;
            }
            ModelInstance_AddAnimation((ModelInstance *)((u8 *)effect + 0xC + index * sizeof(ModelInstance)), &manager->anims[anim]);
            ModelAnimation_SetFrame(&manager->anims[anim], effect->animFrames[index][i]);
        }
        NNS_G3dMdlSetMdlPolygonIDAll(model->model, effect->polygonID);
        ModelInstance_Draw((ModelInstance *)((u8 *)effect + 0xC + index * sizeof(ModelInstance)));
        for (i = 0; i < 2; i++) {
            anim = effect->descs[index]->anims[i];
            if (anim == WIFI_PLAZA_INSTRUMENT_NO_ANIM) {
                break;
            }
            ModelInstance_RemoveAnimation((ModelInstance *)((u8 *)effect + 0xC + index * sizeof(ModelInstance)), &manager->anims[anim]);
        }
        NNS_G3dReleaseMdlTex(model->model);
        NNS_G3dReleaseMdlPltt(model->model);
        model->texture = NULL;
    }
}

#pragma force_active reset

void WiFiPlazaInstrument_LoopAnim(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim) {
    WiFiPlazaInstrument_LoopAnimBy(manager, effect, index, anim, FX32_CONST(2));
}

BOOL WiFiPlazaInstrument_PlayAnim(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim) {
    return WiFiPlazaInstrument_PlayAnimBy(manager, effect, index, anim, FX32_CONST(2));
}

void WiFiPlazaInstrument_LoopAnimBy(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim, fx32 step) {
    const WiFiPlazaInstrumentModelDesc *desc = effect->descs[index];
    fx32 frameCount = ModelAnimation_GetFrameCount((ModelAnimation *)((u8 *)manager + 0x106DC + desc->anims[anim] * sizeof(ModelAnimation)));
    fx32 next = effect->animFrames[index][anim] + step;

    if (next < frameCount) {
        effect->animFrames[index][anim] = next;
    } else {
        effect->animFrames[index][anim] = (effect->animFrames[index][anim] + FX32_CONST(2)) % frameCount;
    }
}

#pragma force_active on

// Returns TRUE (and holds the last frame) once the animation reached its end.
BOOL WiFiPlazaInstrument_PlayAnimBy(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, int index, int anim, int step) {
    fx32 frameCount = ModelAnimation_GetFrameCount(&manager->anims[effect->descs[index]->anims[anim]]);
    fx32 next = effect->animFrames[index][anim] + step;

    if (next < frameCount) {
        effect->animFrames[index][anim] = next;
        return FALSE;
    }
    effect->animFrames[index][anim] = frameCount - FX32_CONST(0.5);
    return TRUE;
}

#pragma force_active reset

void WiFiPlazaInstrument_SetAnimFrame(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim, fx32 frame) {
    const WiFiPlazaInstrumentModelDesc *desc = effect->descs[index];
    fx32 frameCount = ModelAnimation_GetFrameCount((ModelAnimation *)((u8 *)manager + 0x106DC + desc->anims[anim] * sizeof(ModelAnimation)));

    if (frameCount < frame) {
        effect->animFrames[index][anim] = frameCount - FX32_CONST(0.5);
    } else {
        effect->animFrames[index][anim] = frame;
    }
}

fx32 WiFiPlazaInstrument_GetAnimFrame(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u32 anim) {
    const WiFiPlazaInstrumentModelDesc *desc = effect->descs[index];
    return ModelAnimation_GetFrame((ModelAnimation *)((u8 *)manager + 0x106DC + desc->anims[anim] * sizeof(ModelAnimation)));
}

void WiFiPlazaInstrument_InitRipple01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitRipple(manager, effect, 1);
}

void WiFiPlazaInstrument_InitRipple02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitRipple(manager, effect, 2);
}

void WiFiPlazaInstrument_InitRipple03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitRipple(manager, effect, 3);
}

void WiFiPlazaInstrument_InitWhirl01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitWhirl(manager, effect, 1);
}

void WiFiPlazaInstrument_InitWhirl02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitWhirl(manager, effect, 2);
}

void WiFiPlazaInstrument_InitWhirl03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitWhirl(manager, effect, 3);
}

void WiFiPlazaInstrument_InitSignal(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    int i = 0;
    ModelInstance *model = effect->models;

    for (; i < 3; i++, model++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[i + 3]);
        WiFiPlazaObj_GetPosition(effect->obj, &pos);
        pos.y += FX32_CONST(8);
        ModelInstance_SetPosition(model, pos.x, pos.y, pos.z);
        ModelInstance_SetVisible(model, FALSE);
    }
    effect->work.sequence.lastStep = 0xFF;
}

void WiFiPlazaInstrument_InitPop01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 a;
    VecFx32 b;

    WiFiPlazaInstrument_SetModel(manager, effect, 1, sWiFiPlazaInstrumentPopModel1);
    WiFiPlazaInstrument_SetModel(manager, effect, 0, sWiFiPlazaInstrumentPopModel0);
    ModelInstance_SetVisible(&effect->models[1], FALSE);
    WiFiPlazaObj_GetPosition(effect->obj, &a);
    a.y += FX32_CONST(16);
    a.x += FX32_CONST(8);
    b = a;
    b.y += FX32_CONST(16);
    effect->work.fade.fade[0] = 8;

    switch (WiFiPlazaObj_GetParam(effect->obj, 6)) {
    case 0:
        a.z -= 0x1C000;
        b.z -= 0xE000;
        ModelInstance_SetRotation(&effect->models[1], 0xBFFF, 1);
        break;
    case 1:
        a.z += 0x18000;
        b.z += 0xB000;
        ModelInstance_SetRotation(&effect->models[1], 0x3FFF, 1);
        break;
    case 2:
        a.x -= 0x17000;
        b.x -= 0x7000;
        b.z += 0x8000;
        ModelInstance_SetRotation(&effect->models[1], 0, 1);
        break;
    case 3:
        a.x += 0x17000;
        b.x += 0x7000;
        b.z += 0x8000;
        ModelInstance_SetRotation(&effect->models[1], 0x7FFF, 1);
        break;
    }
    ModelInstance_SetPosition(&effect->models[1], a.x, a.y, a.z);
    ModelInstance_SetPosition(&effect->models[0], b.x, b.y, b.z);
}

void WiFiPlazaInstrument_InitPop02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 a;
    VecFx32 b;

    WiFiPlazaInstrument_SetModel(manager, effect, 1, sWiFiPlazaInstrumentPopModel1);
    WiFiPlazaInstrument_SetModel(manager, effect, 0, sWiFiPlazaInstrumentPopModel0);
    WiFiPlazaInstrument_SetModel(manager, effect, 2, sWiFiPlazaInstrumentPopModel2);
    ModelInstance_SetVisible(&effect->models[1], FALSE);
    WiFiPlazaObj_GetPosition(effect->obj, &a);
    a.y += FX32_CONST(16);
    a.x += FX32_CONST(8);
    b = a;
    b.y += FX32_CONST(16);
    effect->work.fade.fade[0] = 8;
    effect->work.fade.fade[1] = 8;

    switch (WiFiPlazaObj_GetParam(effect->obj, 6)) {
    case 0:
        a.z -= 0x1C000;
        b.z -= 0xE000;
        ModelInstance_SetRotation(&effect->models[1], 0xA38D, 1);
        ModelInstance_SetRotation(&effect->models[2], 0xDC70, 1);
        ModelInstance_SetPosition(&effect->models[1], a.x + 0x8000, a.y, a.z);
        ModelInstance_SetPosition(&effect->models[2], a.x - 0x8000, a.y, a.z);
        break;
    case 1:
        a.z += 0x18000;
        b.z += 0xB000;
        ModelInstance_SetRotation(&effect->models[1], 0x238E, 1);
        ModelInstance_SetRotation(&effect->models[2], 0x5C71, 1);
        ModelInstance_SetPosition(&effect->models[1], a.x - 0x8000, a.y, a.z);
        ModelInstance_SetPosition(&effect->models[2], a.x + 0x8000, a.y, a.z);
        break;
    case 2:
        a.x -= 0x17000;
        b.x -= 0x7000;
        b.z += 0x8000;
        ModelInstance_SetRotation(&effect->models[1], 0xE38F, 1);
        ModelInstance_SetRotation(&effect->models[2], 0x1C71, 1);
        ModelInstance_SetPosition(&effect->models[1], a.x, a.y, a.z - 0x8000);
        ModelInstance_SetPosition(&effect->models[2], a.x, a.y, a.z + 0x8000);
        break;
    case 3:
        a.x += 0x17000;
        b.x += 0x7000;
        b.z += 0x8000;
        ModelInstance_SetRotation(&effect->models[1], 0x638D, 1);
        ModelInstance_SetRotation(&effect->models[2], 0x9C71, 1);
        ModelInstance_SetPosition(&effect->models[1], a.x, a.y, a.z + 0x8000);
        ModelInstance_SetPosition(&effect->models[2], a.x, a.y, a.z - 0x8000);
        break;
    }
    ModelInstance_SetPosition(&effect->models[0], b.x, b.y, b.z);
}

void WiFiPlazaInstrument_InitPop03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 a;
    VecFx32 b;

    WiFiPlazaInstrument_SetModel(manager, effect, 1, sWiFiPlazaInstrumentPopModel1);
    WiFiPlazaInstrument_SetModel(manager, effect, 0, sWiFiPlazaInstrumentPopModel0);
    WiFiPlazaInstrument_SetModel(manager, effect, 2, sWiFiPlazaInstrumentPopModel2);
    WiFiPlazaInstrument_SetModel(manager, effect, 3, sWiFiPlazaInstrumentPopModel3);
    ModelInstance_SetVisible(&effect->models[1], FALSE);
    effect->work.fade.fade[0] = 8;
    effect->work.fade.fade[1] = 8;
    effect->work.fade.fade[2] = 8;
    WiFiPlazaObj_GetPosition(effect->obj, &a);
    a.y += FX32_CONST(16);
    a.x += FX32_CONST(8);
    b = a;
    b.y += FX32_CONST(16);

    switch (WiFiPlazaObj_GetParam(effect->obj, 6)) {
    case 0:
        a.z -= 0x1C000;
        b.z -= 0xE000;
        ModelInstance_SetRotation(&effect->models[1], 0xA38D, 1);
        ModelInstance_SetRotation(&effect->models[2], 0xDC70, 1);
        ModelInstance_SetRotation(&effect->models[3], 0xBFFF, 1);
        ModelInstance_SetPosition(&effect->models[1], a.x + 0x8000, a.y, a.z);
        ModelInstance_SetPosition(&effect->models[2], a.x - 0x8000, a.y, a.z);
        ModelInstance_SetPosition(&effect->models[3], a.x, a.y, a.z);
        break;
    case 1:
        a.z += 0x18000;
        b.z += 0xB000;
        ModelInstance_SetRotation(&effect->models[1], 0x238E, 1);
        ModelInstance_SetRotation(&effect->models[2], 0x5C71, 1);
        ModelInstance_SetRotation(&effect->models[3], 0x3FFF, 1);
        ModelInstance_SetPosition(&effect->models[1], a.x - 0x8000, a.y, a.z);
        ModelInstance_SetPosition(&effect->models[2], a.x + 0x8000, a.y, a.z);
        ModelInstance_SetPosition(&effect->models[3], a.x, a.y, a.z);
        break;
    case 2:
        a.x -= 0x17000;
        b.x -= 0x7000;
        b.z += 0x8000;
        ModelInstance_SetRotation(&effect->models[1], 0xE38F, 1);
        ModelInstance_SetRotation(&effect->models[2], 0x1C71, 1);
        ModelInstance_SetRotation(&effect->models[3], 0, 1);
        ModelInstance_SetPosition(&effect->models[1], a.x, a.y, a.z - 0x8000);
        ModelInstance_SetPosition(&effect->models[2], a.x, a.y, a.z + 0x8000);
        ModelInstance_SetPosition(&effect->models[3], a.x, a.y, a.z);
        break;
    case 3:
        a.x += 0x17000;
        b.x += 0x7000;
        b.z += 0x8000;
        ModelInstance_SetRotation(&effect->models[1], 0x638D, 1);
        ModelInstance_SetRotation(&effect->models[2], 0x9C71, 1);
        ModelInstance_SetRotation(&effect->models[3], 0x7FFF, 1);
        ModelInstance_SetPosition(&effect->models[1], a.x, a.y, a.z + 0x8000);
        ModelInstance_SetPosition(&effect->models[2], a.x, a.y, a.z - 0x8000);
        ModelInstance_SetPosition(&effect->models[3], a.x, a.y, a.z);
        break;
    }
    ModelInstance_SetPosition(&effect->models[0], b.x, b.y, b.z);
}

void WiFiPlazaInstrument_InitBalloon01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitBalloon(manager, effect, 1);
}

void WiFiPlazaInstrument_InitBalloon02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitBalloon(manager, effect, 2);
}

void WiFiPlazaInstrument_InitBalloon03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitBalloon(manager, effect, 3);
}

void WiFiPlazaInstrument_InitSparkle01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitSparkle(manager, effect, 1);
}

void WiFiPlazaInstrument_InitSparkle02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitSparkle(manager, effect, 2);
}

void WiFiPlazaInstrument_InitSparkle03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    WiFiPlazaInstrument_InitSparkle(manager, effect, 4);
}

void WiFiPlazaInstrument_InitBell01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels01;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Bell01, 2, 0x21, 0, effect);
}

void WiFiPlazaInstrument_InitBell02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels02;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Bell02, 2, 0x21, 0, effect);
}

void WiFiPlazaInstrument_InitBell03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels03;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Bell03, 3, 0x21, 0, effect);
}

void WiFiPlazaInstrument_InitDrum01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels01;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Drum01, 2, 0x21, 1, effect);
}

void WiFiPlazaInstrument_InitDrum02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels02;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Drum02, 3, 0x29, 1, effect);
}

void WiFiPlazaInstrument_InitDrum03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels03;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Drum03, 3, 0x21, 1, effect);
}

void WiFiPlazaInstrument_InitCymbal01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels01;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Cymbal01, 2, 0x21, 2, effect);
    WiFiPlazaInstrument_InitCymbalFlash(manager, effect);
}

void WiFiPlazaInstrument_InitCymbal02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels02;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Cymbal02, 2, 0x21, 2, effect);
    WiFiPlazaInstrument_InitCymbalFlash(manager, effect);
}

void WiFiPlazaInstrument_InitCymbal03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    const u8 *desc = sWiFiPlazaInstrumentBurstModels03;

    for (i = 0; i < 16; i++, desc++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[*desc]);
    }
    WiFiPlazaInstrument_InitBursts(&effect->work.burst, sWiFiPlazaInstrumentBursts_Cymbal03, 2, 0x21, 2, effect);
    WiFiPlazaInstrument_InitCymbalFlash(manager, effect);
}

// Shows the models one after another (6 steps over 54 frames) and hides each when its animation ended.
BOOL WiFiPlazaInstrument_UpdateRipple(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int limit;
    int step;
    int i;
    int result;
    int next = effect->frame + 1;

    if (next <= 54) {
        effect->frame = next;
    }
    limit = effect->frame * 6 / 54;
    for (step = effect->work.rise.shown; (u32)step < (u32)limit; step++) {
        int index = step % 3;
        if ((u32)index < (u32)effect->work.rise.count) {
            ModelInstance_SetVisible(&effect->models[index], TRUE);
        }
    }
    effect->work.rise.shown = limit;
    result = TRUE;
    i = 0;
    if (effect->work.rise.count > 0) {
        ModelInstance *model = effect->models;
        ModelInstance *model2 = effect->models;
        do {
            if (ModelInstance_IsVisible(model) == TRUE) {
                result = WiFiPlazaInstrument_PlayAnim(manager, effect, i, 0);
                if (result != FALSE) {
                    ModelInstance_SetVisible(model2, FALSE);
                    WiFiPlazaInstrument_SetAnimFrame(manager, effect, i, 0, 0);
                }
            }
            i++;
            model++;
            model2++;
        } while (i < effect->work.rise.count);
    }
    if (effect->work.rise.shown >= 6 && result == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaInstrument_UpdateSignal01(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    return WiFiPlazaInstrument_UpdateSignalSequence(manager, effect, 0);
}

BOOL WiFiPlazaInstrument_UpdateSignal02(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    return WiFiPlazaInstrument_UpdateSignalSequence(manager, effect, 1);
}

BOOL WiFiPlazaInstrument_UpdateSignal03(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    return WiFiPlazaInstrument_UpdateSignalSequence(manager, effect, 2);
}

// Shows the models one at a time over 32 frames; done when the last one finished its animation.
BOOL WiFiPlazaInstrument_UpdateWhirl(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    int current;
    BOOL result;

    current = (effect->frame * 3) / 32;
    if (current != effect->work.stack.shown) {
        effect->work.stack.shown = current;
        if ((u32)effect->work.stack.count > (u32)current) {
            if (current != 0) {
                ModelInstance_SetVisible(&effect->models[current - 1], FALSE);
            }
            ModelInstance_SetVisible(&effect->models[current], TRUE);
        }
    }
    if (effect->frame < 32) {
        effect->frame++;
    }
    result = FALSE;
    for (i = 0; i < effect->work.stack.count; i++) {
        if (ModelInstance_IsVisible(&effect->models[i])) {
            if (i == effect->work.stack.count - 1) {
                result = WiFiPlazaInstrument_PlayAnim(manager, effect, i, 0);
            } else {
                WiFiPlazaInstrument_LoopAnim(manager, effect, i, 0);
            }
        }
    }
    return result;
}

#pragma force_active on

// Model 0 plays three times; models 1-3 follow, each fading out (8 steps) once its animation ended.
BOOL WiFiPlazaInstrument_UpdatePop(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    BOOL done;

    if (effect->descs[0]->anims[0] == WIFI_PLAZA_INSTRUMENT_NO_ANIM) {
        GF_AssertFail();
    }
    if (effect->work.fade.delay > 0) {
        effect->work.fade.delay--;
        ModelInstance_SetVisible(&effect->models[0], FALSE);
    } else if (effect->frame < 3 && WiFiPlazaInstrument_HasModel(effect, effect->frame + 1) == TRUE) {
        ModelInstance_SetVisible(&effect->models[0], TRUE);
        if (WiFiPlazaInstrument_PlayAnim(manager, effect, 0, 0) == TRUE) {
            effect->frame++;
            effect->work.fade.delay = 3;
            if (effect->frame < 3) {
                WiFiPlazaInstrument_SetAnimFrame(manager, effect, 0, 0, 0);
            } else {
                ModelInstance_SetVisible(&effect->models[0], FALSE);
            }
        }
    }
    int i = 1;
    done = TRUE;
    ModelInstance *model = &effect->models[0];
    model++;
    WiFiPlazaInstrumentEffect *descView = (WiFiPlazaInstrumentEffect *)((u8 *)effect + 4); // descView->descs[0] is descs[i]
    for (; i <= 3; i++, model++, descView = (WiFiPlazaInstrumentEffect *)((u8 *)descView + 4)) {
        if (WiFiPlazaInstrument_HasModel(effect, i) == TRUE) {
            if (effect->frame > i - 1) {
                ModelInstance_SetVisible(model, TRUE);
                if (WiFiPlazaInstrument_PlayAnimBy(manager, effect, i, 0, FX32_CONST(2.5)) == FALSE) {
                    done = FALSE;
                } else {
                    // fadeView->work.fade.fade[0] is work.fade.fade[i - 1]
                    volatile WiFiPlazaInstrumentEffect *fadeView = (WiFiPlazaInstrumentEffect *)((u8 *)effect + (i - 1) * 4);
                    if (fadeView->work.fade.fade[0] == 0) {
                        ModelInstance_SetVisible(model, FALSE);
                        NNS_G3dMdlSetMdlAlphaAll(manager->models[descView->descs[0]->model].model, 31);
                    } else {
                        fadeView->work.fade.fade[0]--;
                        NNS_G3dMdlSetMdlAlphaAll(manager->models[descView->descs[0]->model].model, (s32)(fadeView->work.fade.fade[0] * 20) / 8);
                        done = FALSE;
                    }
                }
            } else {
                done = FALSE;
            }
        }
    }
    return done;
}

#pragma force_active reset

BOOL WiFiPlazaInstrument_UpdateBalloon(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    u8 phase = effect->phase;

    switch (phase) {
    case 0:
        if (WiFiPlazaInstrument_UpdateBalloonUp(manager, effect) != FALSE) {
            effect->phase = 1;
            WiFiPlazaInstrument_StartBalloonDown(manager, effect);
        }
        break;
    case 1:
        if (WiFiPlazaInstrument_UpdateBalloonDown(manager, effect) != FALSE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

// Shows the models row by row (4 per row, every 8 frames); each falls along its tween.
BOOL WiFiPlazaInstrument_UpdateSparkle(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;
    BOOL result;

    if (effect->work.shower.rows < effect->work.shower.totalRows) {
        int target;
        u32 n;
        ModelInstance *model;

        effect->work.shower.tick++;
        if (effect->work.shower.tick >= 8) {
            effect->work.shower.tick = 0;
            effect->work.shower.rows++;
        }
        target = (effect->work.shower.tick * 4) / 8 + effect->work.shower.rows * 4;
        n = effect->work.shower.shown;
        if (n < (u32)target) {
            model = &effect->models[n];
            do {
                ModelInstance_SetVisible(model, TRUE);
                model++;
            } while (++n < (u32)target);
        }
        effect->work.shower.shown = target;
    }
    i = 0;
    if (i < effect->work.shower.shown) {
        ModelInstance *model = effect->models;
        ModelInstance *model2 = model;
        do {
            if (ModelInstance_IsVisible(model) == TRUE) {
                result = WiFiPlazaInstrument_UpdateSparkleModel(manager, effect, i);
                if (result == TRUE) {
                    ModelInstance_SetVisible(model2, FALSE);
                }
            }
            model++;
            model2++;
        } while (++i < effect->work.shower.shown);
    }
    if (effect->work.shower.rows == effect->work.shower.totalRows && result == TRUE) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiFiPlazaInstrument_UpdateBellDrum(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int result = WiFiPlazaInstrument_UpdateBursts(&effect->work.burst, effect->frame);

    effect->frame = effect->frame + 1;
    if (result == 2) {
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaInstrument_UpdateCymbal(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int result = WiFiPlazaInstrument_UpdateBursts(&effect->work.burst, effect->frame);

    effect->frame = effect->frame + 1;
    if (result == 1) {
        WiFiPlazaInstrument_StartCymbalFlash(manager, effect);
    }
    WiFiPlazaInstrument_UpdateCymbalFlash(manager, effect);
    if (result == 2) {
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaInstrument_DrawEffectModels(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;

    if (WiFiPlazaObj_IsCulled(effect->obj) == 1) {
        return FALSE;
    }
    for (i = 0; i < WIFI_PLAZA_INSTRUMENT_EFFECT_MODELS; i++) {
        if (WiFiPlazaInstrument_HasModel(effect, i) == TRUE) {
            WiFiPlazaInstrument_DrawModel(manager, effect, i);
        }
    }
    return TRUE;
}

#pragma force_active on

// The player's object rises with up to 3 models (types 24-26); on map attribute 42 it only lifts slightly.
void WiFiPlazaInstrument_InitBalloon(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, int count) {
    u32 packed;

    struct {
        s16 x, y;
    } copy, original;

    if (effect->obj == WiFiPlazaObj_GetOwnPlayer(manager->objManager)) {
        WiFiPlazaCamera_ClearFollowObject(manager->camera);
    }
    packed = ((u32(*)(WiFiPlazaObj *))WiFiPlazaObj_GetGridPos)(effect->obj);
    original.x = (s16)packed;
    original.y = (s16)(packed >> 16);
    copy = original;
    effect->work.hop.onTile42 = WiFiPlazaMap_GetTileAttr(manager->map, (u16)(copy.x / 16), (u16)(copy.y / 16)) == 42;
    switch (count) {
    case 1:
        WiFiPlazaInstrument_SetModel(manager, effect, 0, sWiFiPlazaInstrumentBalloonModel0);
        WiFiPlazaInstrument_SetBalloonAnim(manager, effect, 0, 0);
        break;
    case 2:
        WiFiPlazaInstrument_SetModel(manager, effect, 0, sWiFiPlazaInstrumentBalloonModel0);
        WiFiPlazaInstrument_SetModel(manager, effect, 1, sWiFiPlazaInstrumentBalloonModel1);
        WiFiPlazaInstrument_SetBalloonAnim(manager, effect, 0, 2);
        WiFiPlazaInstrument_SetBalloonAnim(manager, effect, 1, 1);
        break;
    case 3:
    default:
        WiFiPlazaInstrument_SetModel(manager, effect, 0, sWiFiPlazaInstrumentBalloonModel0);
        WiFiPlazaInstrument_SetModel(manager, effect, 1, sWiFiPlazaInstrumentBalloonModel1);
        WiFiPlazaInstrument_SetModel(manager, effect, 2, sWiFiPlazaInstrumentBalloonModel2);
        WiFiPlazaInstrument_SetBalloonAnim(manager, effect, 0, 0);
        WiFiPlazaInstrument_SetBalloonAnim(manager, effect, 1, 2);
        WiFiPlazaInstrument_SetBalloonAnim(manager, effect, 2, 1);
        break;
    }
    effect->work.hop.count = (u8)count;
    WiFiPlazaObj_SetAvatarMode(effect->obj, FALSE);
    if (!effect->work.hop.onTile42) {
        WiFiPlazaInstrument_StartBalloonRise(manager, effect);
    } else {
        WiFiPlazaInstrument_StartBalloonLift(manager, effect);
    }
}

#pragma force_active reset

void WiFiPlazaInstrument_StartBalloonRise(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    u8 height;
    float delta;

    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    height = sWiFiPlazaInstrumentBalloonHeights[effect->work.hop.count - 1];
    if (height != 0) {
        delta = (float)(height << 12) + 0.5f;
    } else {
        delta = (float)(height << 12) - 0.5f;
    }
    WiFiPlazaInstrumentTween_Init(&effect->work.hop.tweens[3], pos.x, pos.x, pos.y, pos.y + (fx32)delta, pos.z, pos.z, 0x6C);
    WiFiPlazaInstrumentWave_Init(&effect->work.hop.objWave, 0, 0x71C, FX32_CONST(8));
    WiFiPlazaInstrument_PlaceBalloons(effect);
    effect->work.hop.timer = 0;
}

void WiFiPlazaInstrument_StartBalloonLift(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;

    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    WiFiPlazaInstrumentTween_Init(&effect->work.hop.tweens[3], pos.x, pos.x, pos.y, pos.y + FX32_CONST(2), pos.z, pos.z, 2);
    WiFiPlazaInstrumentWave_Init(&effect->work.hop.objWave, 0, 0x71C, 0);
    WiFiPlazaInstrument_PlaceBalloonsLifted(effect);
    effect->work.hop.timer = 0;
}

BOOL WiFiPlazaInstrument_UpdateBalloonUp(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    if (effect->work.hop.onTile42 == FALSE) {
        return WiFiPlazaInstrument_UpdateBalloonRise(manager, effect);
    }
    return WiFiPlazaInstrument_UpdateBalloonLift(manager, effect);
}

// Rising: the object sways sideways; the own player may still turn and step with the D-pad.
BOOL WiFiPlazaInstrument_UpdateBalloonRise(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    fx32 sway;
    BOOL result;
    int i;

    result = WiFiPlazaInstrumentTween_Update(&effect->work.hop.tweens[3], effect->work.hop.timer);
    WiFiPlazaInstrumentWave_Update(&effect->work.hop.objWave);
    effect->work.hop.timer++;
    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    WiFiPlazaInstrumentTween_GetPosition(&effect->work.hop.tweens[3], &pos.x, &pos.y, &pos.z);
    WiFiPlazaInstrumentWave_GetValue(&effect->work.hop.objWave, &sway);
    if (result == FALSE) {
        pos.x += sway;
    }
    WiFiPlazaObj_SetPosition(effect->obj, &pos);
    WiFiPlazaInstrument_PlaceBalloons(effect);
    for (i = 0; i < effect->work.hop.count; i++) {
        WiFiPlazaInstrument_UpdateBalloonAnim(manager, effect, i);
    }
    if (effect->obj == WiFiPlazaObj_GetOwnPlayer(manager->objManager)) {
        u32 keys = gSystem.newKeys;
        u32 direction = 4;
        Field2DPoint position;
        u32 packed;

        if (keys & PAD_KEY_UP) {
            direction = 0;
        }
        if (keys & PAD_KEY_DOWN) {
            direction = 1;
        }
        if (keys & PAD_KEY_RIGHT) {
            direction = 3;
        }
        if (keys & PAD_KEY_LEFT) {
            direction = 2;
        }
        if (direction != 4) {
            WiFiPlazaObj_SetDirection(effect->obj, direction);
            packed = ((u32(*)(WiFiPlazaObj *))WiFiPlazaObj_GetGridPos)(effect->obj);
            position.unk0 = (s16)packed;
            position.unk2 = (s16)(packed >> 16);
            WiFiPlazaObj_SetGridPosAndDir(effect->obj, position, direction);
        }
    }
    return result;
}

BOOL WiFiPlazaInstrument_UpdateBalloonLift(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    BOOL result;
    int i;

    result = WiFiPlazaInstrumentTween_Update(&effect->work.hop.tweens[3], effect->work.hop.timer);
    WiFiPlazaInstrumentWave_Update(&effect->work.hop.objWave);
    effect->work.hop.timer++;
    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    WiFiPlazaInstrumentTween_GetPosition(&effect->work.hop.tweens[3], &pos.x, &pos.y, &pos.z);
    WiFiPlazaObj_SetPosition(effect->obj, &pos);
    WiFiPlazaInstrument_PlaceBalloonsLifted(effect);
    for (i = 0; i < effect->work.hop.count; i++) {
        WiFiPlazaInstrument_UpdateBalloonAnim(manager, effect, i);
    }
    return result;
}

void WiFiPlazaInstrument_StartBalloonDown(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    if (effect->work.hop.onTile42 == FALSE) {
        WiFiPlazaInstrument_StartBalloonFall(manager, effect);
        return;
    }
    WiFiPlazaInstrument_StartBalloonDrop(manager, effect);
}

#pragma force_active on

// Starts the way down: the object drops back, the models fly off (up and to the sides).
void WiFiPlazaInstrument_StartBalloonFall(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    fx32 x, y, z;
    int i;

    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    WiFiPlazaInstrumentTween_Init(&effect->work.hop.tweens[3], pos.x, pos.x, pos.y, pos.y - FX32_CONST(sWiFiPlazaInstrumentBalloonHeights[effect->work.hop.count - 1]), pos.z, pos.z, 10);
    effect->work.hop.timer = 0;
    effect->work.hop.stage = 0;
    for (i = 0; i < effect->work.hop.count; i++) {
        ModelInstance_GetPosition(&effect->models[i], &pos.x, &pos.y, &pos.z);
        switch (effect->work.hop.animKinds[i]) {
        case 0:
            x = pos.x;
            y = pos.y + FX32_CONST(64);
            z = pos.z - FX32_CONST(64);
            break;
        case 1:
            x = pos.x + FX32_CONST(64);
            y = pos.y + FX32_CONST(64);
            z = pos.z;
            break;
        case 2:
            x = pos.x - FX32_CONST(64);
            y = pos.y + FX32_CONST(64);
            z = pos.z;
            break;
        }
        WiFiPlazaInstrumentTween_Init(&effect->work.hop.tweens[i], pos.x, x, pos.y, y, pos.z, z, 26);
        WiFiPlazaInstrumentWave_Init(&effect->work.hop.waves[i], 0, 0xAAA, FX32_CONST(6));
        WiFiPlazaInstrument_SetBalloonAnim(manager, effect, i, 3);
    }
    effect->work.hop.tick = 0;
}

#pragma force_active reset

void WiFiPlazaInstrument_StartBalloonDrop(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;

    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    WiFiPlazaInstrumentTween_Init(&effect->work.hop.tweens[3], pos.x, pos.x, pos.y, pos.y - FX32_CONST(2), pos.z, pos.z, 1);
    effect->work.hop.timer = 0;
    effect->work.hop.stage = 0;
}

BOOL WiFiPlazaInstrument_UpdateBalloonDown(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    if (effect->work.hop.onTile42 == FALSE) {
        return WiFiPlazaInstrument_UpdateBalloonFall(manager, effect);
    }
    return WiFiPlazaInstrument_UpdateBalloonDrop(manager, effect);
}

#pragma force_active on

// The way down off map attribute 42: wait, flash the object, drop it and let it bounce once while the models fly off.
BOOL WiFiPlazaInstrument_UpdateBalloonFall(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 objPos;
    VecFx32 bouncePos;
    VecFx32 modelPos;
    fx32 sway;
    int i;
    BOOL done;
    WiFiPlazaInstrumentTween *tween;
    WiFiPlazaInstrumentWave *wave;
    ModelInstance *model;

    switch (effect->work.hop.stage) {
    case 0:
        effect->work.hop.timer++;
        if (effect->work.hop.timer >= 8) {
            effect->work.hop.stage = 1;
        }
        break;
    case 1:
        WiFiPlazaObj_SetPoseLoopEffect(effect->obj, TRUE);
        effect->work.hop.timer = 16;
        effect->work.hop.stage = 2;
        break;
    case 2:
        effect->work.hop.timer--;
        if (effect->work.hop.timer <= 0) {
            WiFiPlazaObj_SetPoseLoopEffect(effect->obj, FALSE);
            effect->work.hop.stage = 3;
            effect->work.hop.timer = 0;
            WiFiPlazaObj_SetDirection(effect->obj, WiFiPlazaObj_GetParam(effect->obj, 6));
        }
        break;
    case 3:
        done = WiFiPlazaInstrumentTween_Update(&effect->work.hop.tweens[3], effect->work.hop.timer);
        effect->work.hop.timer++;
        WiFiPlazaObj_GetPosition(effect->obj, &objPos);
        WiFiPlazaInstrumentTween_GetPosition(&effect->work.hop.tweens[3], &objPos.x, &objPos.y, &objPos.z);
        WiFiPlazaObj_SetPosition(effect->obj, &objPos);
        if (done == TRUE) {
            effect->work.hop.stage = 4;
            effect->work.hop.timer = 0;
            effect->work.hop.baseY = objPos.y;
            WiFiPlazaInstrument_PlaySE(manager, effect, SEQ_SE_PL_SUTYA);
        }
        break;
    case 4: {
        u16 angle = 0x7FFF * effect->work.hop.timer / 10;
        fx32 height = FX_Mul(FX_SinIdx(angle), FX32_CONST(8));
        int next;

        WiFiPlazaObj_GetPosition(effect->obj, &bouncePos);
        bouncePos.y = effect->work.hop.baseY + height;
        WiFiPlazaObj_SetPosition(effect->obj, &bouncePos);
        next = effect->work.hop.timer + 1;
        if (next > 10) {
            WiFiPlazaObj *obj;

            WiFiPlazaObj_SetAvatarMode(effect->obj, TRUE);
            obj = effect->obj;
            if (obj == WiFiPlazaObj_GetOwnPlayer(manager->objManager)) {
                WiFiPlazaCamera_SetFollowObject(manager->camera, obj);
            }
            return TRUE;
        }
        effect->work.hop.timer = next;
        break;
    }
    }
    effect->work.hop.tick++;
    i = 0;
    if (i < effect->work.hop.count) {
        tween = effect->work.hop.tweens;
        wave = effect->work.hop.waves;
        model = effect->models;
        do {
            if (WiFiPlazaInstrumentTween_Update(tween, effect->work.hop.tick) == FALSE) {
                WiFiPlazaInstrumentWave_Update(wave);
                ModelInstance_GetPosition(model, &modelPos.x, &modelPos.y, &modelPos.z);
                WiFiPlazaInstrumentTween_GetPosition(&effect->work.hop.tweens[i], &modelPos.x, &modelPos.y, &modelPos.z);
                WiFiPlazaInstrumentWave_GetValue(&effect->work.hop.waves[i], &sway);
                modelPos.x += sway;
                ModelInstance_SetPosition(&effect->models[i], modelPos.x, modelPos.y, modelPos.z);
            } else if (WiFiPlazaInstrument_SetBalloonAnim(manager, effect, i, 4) == TRUE) {
                WiFiPlazaInstrument_PlaySE(manager, effect, SEQ_SE_PL_HIROBA20);
            }
            WiFiPlazaInstrument_UpdateBalloonAnim(manager, effect, i);
            tween++;
            wave++;
            model++;
        } while (++i < effect->work.hop.count);
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiFiPlazaInstrument_UpdateBalloonDrop(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    int i;
    u32 result;

    for (i = 0; i < effect->work.hop.count; i++) {
        WiFiPlazaInstrument_UpdateBalloonAnim(manager, effect, i);
    }
    switch (effect->work.hop.stage) {
    case 0:
        WiFiPlazaInstrument_PlaySE(manager, effect, SEQ_SE_PL_HIROBA20);
        for (i = 0; i < effect->work.hop.count; i++) {
            WiFiPlazaInstrument_SetBalloonAnim(manager, effect, i, 4);
        }
        effect->work.hop.stage = 1;
        break;
    case 1:
        WiFiPlazaObj_SetPoseLoopEffect(effect->obj, TRUE);
        effect->work.hop.timer = 8;
        effect->work.hop.stage = 2;
        break;
    case 2:
        effect->work.hop.timer--;
        if (effect->work.hop.timer <= 0) {
            WiFiPlazaObj_SetPoseLoopEffect(effect->obj, FALSE);
            effect->work.hop.stage = 3;
            effect->work.hop.timer = 0;
            result = WiFiPlazaObj_GetParam(effect->obj, 6);
            WiFiPlazaObj_SetDirection(effect->obj, result);
        }
        break;
    case 3:
        result = WiFiPlazaInstrumentTween_Update(&effect->work.hop.tweens[3], effect->work.hop.timer);
        effect->work.hop.timer++;
        WiFiPlazaObj_GetPosition(effect->obj, &pos);
        WiFiPlazaInstrumentTween_GetPosition(&effect->work.hop.tweens[3], &pos.x, &pos.y, &pos.z);
        WiFiPlazaObj_SetPosition(effect->obj, &pos);
        if (result == TRUE) {
            effect->work.hop.timer = 0;
            effect->work.hop.baseY = pos.y;
            WiFiPlazaInstrument_PlaySE(manager, effect, SEQ_SE_PL_SUTYA);
            WiFiPlazaObj_SetAvatarMode(effect->obj, TRUE);
            if (effect->obj == WiFiPlazaObj_GetOwnPlayer(manager->objManager)) {
                WiFiPlazaCamera_SetFollowObject(manager->camera, effect->obj);
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// Places the models (up to 3) next to the object while it rises; the float conversions are kept as calls.
void WiFiPlazaInstrument_PlaceBalloons(WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    int count;
    u32 y;
    u32 x;

    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    count = effect->work.hop.count;
    switch (count) {
    case 1:
        y = _fadd(0x3F000000, _fflt(15 << 14));
        x = _fadd(0x3F000000, _fflt(2 << 14));
        ModelInstance_SetPosition(&effect->models[0], pos.x + _ffix(x), pos.y + _ffix(y), pos.z + FX32_ONE);
        break;
    case 2:
        y = _fadd(0x3F000000, _fflt(14 << 14));
        x = _fadd(0x3F000000, _fflt(2 << 12));
        ModelInstance_SetPosition(&effect->models[0], pos.x + _ffix(x), pos.y + _ffix(y), pos.z + FX32_ONE);
        y = _fadd(0x3F000000, _fflt(14 << 14));
        x = _fadd(0x3F000000, _fflt(14 << 12));
        ModelInstance_SetPosition(&effect->models[1], pos.x + _ffix(x), pos.y + _ffix(y), pos.z + FX32_ONE);
        break;
    case 3:
    default:
        y = _fadd(0x3F000000, _fflt(15 << 14));
        x = _fadd(0x3F000000, _fflt(2 << 14));
        ModelInstance_SetPosition(&effect->models[0], pos.x + _ffix(x), pos.y + _ffix(y), pos.z + FX32_ONE);
        y = _fadd(0x3F000000, _fflt(14 << 14));
        x = _fadd(0x3F000000, _fflt(2 << 12));
        ModelInstance_SetPosition(&effect->models[1], pos.x + _ffix(x), pos.y + _ffix(y), pos.z + FX32_ONE);
        y = _fadd(0x3F000000, _fflt(14 << 14));
        x = _fadd(0x3F000000, _fflt(14 << 12));
        ModelInstance_SetPosition(&effect->models[2], pos.x + _ffix(x), pos.y + _ffix(y), pos.z + FX32_ONE);
        break;
    }
}

// Same for the slight lift on map attribute 42.
void WiFiPlazaInstrument_PlaceBalloonsLifted(WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    int count;
    u32 y;
    u32 x;

    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    count = effect->work.hop.count;
    switch (count) {
    case 1:
        y = _fadd(0x3F000000, _fflt(3 << 16));
        x = _fadd(0x3F000000, _fflt(2 << 14));
        ModelInstance_SetPosition(&effect->models[0], pos.x + _ffix(x), pos.y + _ffix(y), pos.z - FX32_CONST(8));
        break;
    case 2:
        y = _fadd(0x3F000000, _fflt(11 << 14));
        x = _fadd(0x3F000000, _fflt(2 << 12));
        ModelInstance_SetPosition(&effect->models[0], pos.x + _ffix(x), pos.y + _ffix(y), pos.z - FX32_CONST(8));
        y = _fadd(0x3F000000, _fflt(11 << 14));
        x = _fadd(0x3F000000, _fflt(14 << 12));
        ModelInstance_SetPosition(&effect->models[1], pos.x + _ffix(x), pos.y + _ffix(y), pos.z - FX32_CONST(8));
        break;
    case 3:
    default:
        y = _fadd(0x3F000000, _fflt(3 << 16));
        x = _fadd(0x3F000000, _fflt(2 << 14));
        ModelInstance_SetPosition(&effect->models[0], pos.x + _ffix(x), pos.y + _ffix(y), pos.z - FX32_CONST(8));
        y = _fadd(0x3F000000, _fflt(11 << 14));
        x = _fadd(0x3F000000, _fflt(2 << 12));
        ModelInstance_SetPosition(&effect->models[1], pos.x + _ffix(x), pos.y + _ffix(y), pos.z - FX32_CONST(8));
        y = _fadd(0x3F000000, _fflt(11 << 14));
        x = _fadd(0x3F000000, _fflt(14 << 12));
        ModelInstance_SetPosition(&effect->models[2], pos.x + _ffix(x), pos.y + _ffix(y), pos.z - FX32_CONST(8));
        break;
    }
}

// Switches model `index` to animation step `kind` (frames kind * 2 .. kind * 2 + 1, 4 = hide when done).
BOOL WiFiPlazaInstrument_SetBalloonAnim(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index, u8 kind) {
    if (kind != effect->work.hop.animKinds[index]) {
        effect->work.hop.animKinds[index] = kind;
        effect->work.hop.animTimers[index] = 0;
        WiFiPlazaInstrument_SetAnimFrame(manager, effect, index, 0, kind << 13);
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaInstrument_UpdateBalloonAnim(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 index) {
    u8 kind = effect->work.hop.animKinds[index];
    u8 *timers = effect->work.hop.animTimers;
    u32 frame;

    if (timers[index] == 0) {
        frame = WiFiPlazaInstrument_GetAnimFrame(manager, effect, index, 0);
        if (frame == (kind << 13)) {
            frame = (kind << 13) + FX32_ONE;
        } else if (kind != 4) {
            frame = kind << 13;
        } else {
            ModelInstance_SetVisible(&effect->models[index], FALSE);
        }
        WiFiPlazaInstrument_SetAnimFrame(manager, effect, index, 0, frame);
        timers[index] = 8;
    }
    timers[index]--;
}

void WiFiPlazaInstrument_InitSparkle(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 rows) {
    VecFx32 base;
    VecFx32 start;
    VecFx32 end;
    u32 i;
    int j;

    WiFiPlazaObj_GetPosition(effect->obj, &base);
    for (i = 0; i < rows; i++) {
        u32 first = i * 4;
        u32 descIndex = i + 13;
        if (descIndex > 15) {
            descIndex = 15;
        }
        for (j = 0; j < 4; j++) {
            u32 index = j + first;
            WiFiPlazaInstrument_SetModel(manager, effect, index, &sWiFiPlazaInstrumentModelDescs[descIndex]);
            VEC_Add(&base, &sWiFiPlazaInstrumentSparkleStarts[j], &start);
            VEC_Add(&start, &sWiFiPlazaInstrumentSparkleFalls[j], &end);
            ModelInstance_SetPosition(&effect->models[index], start.x, start.y, start.z);
            ModelInstance_SetVisible(&effect->models[index], FALSE);
            effect->work.shower.timers[index] = 0;
            WiFiPlazaInstrumentTween_Init(&effect->work.shower.tweens[index], start.x, end.x, start.y, end.y, start.z, end.z, 14);
        }
    }
    effect->work.shower.totalRows = rows;
    effect->work.shower.rows = 0;
    effect->work.shower.tick = 0;
    effect->work.shower.shown = 0;
}

#pragma force_active on

BOOL WiFiPlazaInstrument_UpdateSparkleModel(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, int index) {
    VecFx32 pos;
    BOOL done;
    int offset;
    volatile s8 *timers = (volatile s8 *)effect->work.shower.timers;

    timers[index]++;
    offset = index;
    offset *= sizeof(WiFiPlazaInstrumentTween);
    done = WiFiPlazaInstrumentTween_Update((WiFiPlazaInstrumentTween *)((u8 *)effect + 0x968 + offset), timers[index]);
    WiFiPlazaInstrumentTween_GetPosition((WiFiPlazaInstrumentTween *)(0x968 + (u32)effect + offset), &pos.x, &pos.y, &pos.z);
    ModelInstance_SetPosition(&effect->models[index], pos.x, pos.y, pos.z);
    WiFiPlazaInstrument_LoopAnim(manager, effect, index, 0);
    return done;
}

#pragma force_active reset

void WiFiPlazaInstrument_InitRipple(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        VecFx32 pos;
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[i]);
        WiFiPlazaObj_GetPosition(effect->obj, &pos);
        pos.y += FX32_CONST(8);
        pos.z += FX32_CONST(6);
        ModelInstance_SetPosition(&effect->models[i], pos.x, pos.y, pos.z);
        ModelInstance_SetVisible(&effect->models[i], FALSE);
    }
    effect->work.rise.count = count;
}

#pragma force_active on

// Plays sequence `kind`: 12 steps over 48 frames, each showing one model at a fixed animation frame.
BOOL WiFiPlazaInstrument_UpdateSignalSequence(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, int kind) {
    WiFiPlazaInstrumentStep prev;
    WiFiPlazaInstrumentStep cur;
    int step = effect->frame * 12 / 48;

    if (step != effect->work.sequence.lastStep) {
        effect->work.sequence.lastStep = step;
        if (step != 0) {
            prev = sWiFiPlazaInstrumentSignalSteps[kind][step];
            ModelInstance_SetVisible(&effect->models[prev.model], FALSE);
        }
        cur = ov49_0226A67C[kind][step];
        if (cur.frame != 0) {
            WiFiPlazaInstrument_SetAnimFrame(manager, effect, cur.model, 0, FX32_CONST(cur.frame));
            WiFiPlazaInstrument_SetAnimFrame(manager, effect, cur.model, 1, FX32_CONST(cur.frame));
            ModelInstance_SetVisible(&effect->models[cur.model], TRUE);
        } else {
            ModelInstance_SetVisible(&effect->models[cur.model], FALSE);
        }
    }
    if (effect->frame < 48) {
        effect->frame++;
    }
    if (effect->frame >= 48) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void WiFiPlazaInstrument_InitWhirl(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        VecFx32 pos;
        WiFiPlazaInstrument_SetModel(manager, effect, i, &sWiFiPlazaInstrumentModelDescs[i + 6]);
        WiFiPlazaObj_GetPosition(effect->obj, &pos);
        pos.y += FX32_CONST(8);
        ModelInstance_SetPosition(&effect->models[i], pos.x, pos.y, pos.z);
        ModelInstance_SetVisible(&effect->models[i], FALSE);
    }
    effect->work.stack.shown = 0xFF;
    effect->work.stack.count = count;
}

void WiFiPlazaInstrument_InitCymbalFlash(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    VecFx32 pos;
    int i;

    WiFiPlazaObj_GetPosition(effect->obj, &pos);
    pos.z -= FX32_CONST(10);
    pos.x -= FX32_CONST(4);
    pos.y += FX32_CONST(16);
    for (i = 0; i < 2; i++) {
        WiFiPlazaInstrument_SetModel(manager, effect, i + 16, &sWiFiPlazaInstrumentModelDescs[i + 37]);
        ModelInstance_SetPosition(&effect->models[i + 16], pos.x + i * FX32_CONST(24), pos.y, pos.z);
    }
}

void WiFiPlazaInstrument_StartCymbalFlash(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;

    effect->work.burst.flashActive = TRUE;
    effect->work.burst.flashFrame = 0;
    for (i = 0; i < 2; i++) {
        ModelInstance_SetVisible(&effect->models[i + 16], TRUE);
    }
}

void WiFiPlazaInstrument_UpdateCymbalFlash(WiFiPlazaInstrumentManager *manager, WiFiPlazaInstrumentEffect *effect) {
    int i;

    if (effect->work.burst.flashActive != FALSE) {
        if (effect->work.burst.flashFrame < 13) {
            effect->work.burst.flashFrame++;
        } else {
            effect->work.burst.flashActive = FALSE;
        }
        for (i = 0; i < 2; i++) {
            if (effect->work.burst.flashFrame < 13) {
                WiFiPlazaInstrument_LoopAnim(manager, effect, i + 16, 0);
            } else {
                ModelInstance_SetVisible(&effect->models[i + 16], FALSE);
            }
        }
    }
}

#pragma force_active on

void WiFiPlazaInstrument_InitBursts(WiFiPlazaInstrumentBurstWork *work, const WiFiPlazaInstrumentBurstEvent *events, u16 eventCount, u16 duration, u32 kind, WiFiPlazaInstrumentEffect *effect) {
    int i;

    work->events = events;
    work->eventCount = eventCount;
    work->duration = duration;
    for (i = 0; i < 4; i++) {
        WiFiPlazaInstrumentBurst_Init(&work->bursts[i], &effect->models[i * 4], &effect->models[i * 4 + 1], &effect->models[i * 4 + 2], &effect->models[i * 4 + 3], effect->obj, kind);
    }
}

// Starts the bursts scheduled for `frame`; returns 1 when one started, 2 when all are done and the duration passed.
int WiFiPlazaInstrument_UpdateBursts(WiFiPlazaInstrumentBurstWork *work, u32 frame) {
    int i = 0;
    int result = 0;
    WiFiPlazaInstrumentBurstEvent event;
    u32 burst;
    u32 count;
    u8 *cur;

    for (i = 0; i < work->eventCount; i++) {
        event = work->events[i];
        if (event.frame == frame) {
            burst = event.burst;
            if (burst >= 4) {
                GF_AssertFail();
            }
            count = event.count;
            if (count > 4) {
                GF_AssertFail();
            }
            WiFiPlazaInstrumentBurst_Start((WiFiPlazaInstrumentBurst *)((u8 *)work + burst * sizeof(WiFiPlazaInstrumentBurst)), count);
            result = 1;
        }
    }
    burst = TRUE; // all finished
    for (i = 0, cur = (u8 *)work; i < 4; i++, cur += sizeof(WiFiPlazaInstrumentBurst)) {
        if (WiFiPlazaInstrumentBurst_Update((WiFiPlazaInstrumentBurst *)cur) == FALSE) {
            burst = FALSE;
        }
    }
    if (work->duration <= frame && burst == TRUE) {
        result = 2;
    }
    return result;
}

#pragma force_active reset

void WiFiPlazaInstrumentBurst_Init(WiFiPlazaInstrumentBurst *burst, ModelInstance *model0, ModelInstance *model1, ModelInstance *model2, ModelInstance *model3, WiFiPlazaObj *obj, u32 kind) {
    GF_ASSERT(kind < 3);
    burst->models[0] = model0;
    burst->models[1] = model1;
    burst->models[2] = model2;
    burst->models[3] = model3;
    burst->obj = obj;
    burst->kind = kind;
}

void WiFiPlazaInstrumentBurst_Start(WiFiPlazaInstrumentBurst *burst, u32 count) {
    int i;

    for (i = 0; i < count; i++) {
        ModelInstance_SetVisible(burst->models[i], TRUE);
    }
    burst->frame = 0;
    burst->active = TRUE;
    burst->count = count;
    sWiFiPlazaInstrumentBurstStartFuncs[burst->kind](burst, burst->obj);
}

BOOL WiFiPlazaInstrumentBurst_Update(WiFiPlazaInstrumentBurst *burst) {
    BOOL done;
    int i;

    if (burst->active == FALSE) {
        return TRUE;
    }
    done = sWiFiPlazaInstrumentBurstUpdateFuncs[burst->kind](burst);
    burst->frame++;
    if (done == TRUE) {
        for (i = 0; i < burst->count; i++) {
            ModelInstance_SetVisible(burst->models[i], FALSE);
        }
        burst->active = FALSE;
    }
    return done;
}

#pragma force_active on

void WiFiPlazaInstrumentBurst_StartBell(WiFiPlazaInstrumentBurst *burst, WiFiPlazaObj *obj) {
    VecFx32 pos;
    u16 angle;
    int i;
    fx32 x, y, z;

    WiFiPlazaObj_GetPosition(obj, &pos);
    pos.x += FX32_CONST(8);
    pos.y += FX32_CONST(16);
    pos.z -= FX32_CONST(8);
    for (i = 0; i < burst->count; i++) {
        switch (i) {
        case 0:
            x = pos.x - FX32_CONST(12);
            y = pos.y + FX32_CONST(16);
            z = pos.z;
            angle = 0;
            break;
        case 1:
            x = pos.x + FX32_CONST(12);
            y = pos.y + FX32_CONST(16);
            z = pos.z;
            angle = 0x7FFF;
            break;
        case 2:
            x = pos.x - FX32_CONST(8);
            y = pos.y + FX32_CONST(16);
            z = pos.z - FX32_CONST(6);
            angle = 0x7FFF;
            break;
        case 3:
            x = pos.x + FX32_CONST(8);
            y = pos.y + FX32_CONST(16);
            z = pos.z - FX32_CONST(6);
            angle = 0;
            break;
        }
        WiFiPlazaInstrumentTween_Init(&burst->tweens[i], pos.x, x, pos.y, y, pos.z, z, 18);
        WiFiPlazaInstrumentWave_Init(&burst->waves[i], angle, 0xCCC, FX32_CONST(6));
        ModelInstance_SetPosition(burst->models[i], pos.x, pos.y, pos.z);
    }
}

BOOL WiFiPlazaInstrumentBurst_UpdateBell(WiFiPlazaInstrumentBurst *burst) {
    int i;
    BOOL result;
    WiFiPlazaInstrumentWave *wave;
    WiFiPlazaInstrumentTween *tween;
    fx32 z;
    fx32 y;
    fx32 x;
    fx32 sway;

    i = 0;
    if (i < burst->count) {
        wave = burst->waves;
        tween = burst->tweens;
        do {
            WiFiPlazaInstrumentWave_Update(wave);
            result = WiFiPlazaInstrumentTween_Update(tween, burst->frame);
            WiFiPlazaInstrumentWave_GetValue(&burst->waves[i], &sway);
            WiFiPlazaInstrumentTween_GetPosition(&burst->tweens[i], &x, &y, &z);
            x += sway;
            ModelInstance_SetPosition(burst->models[i], x, y, z);
            wave++;
            tween++;
        } while (++i < burst->count);
    }
    return result;
}

#pragma force_active reset

void WiFiPlazaInstrumentBurst_StartDrum(WiFiPlazaInstrumentBurst *burst, WiFiPlazaObj *obj) {
    VecFx32 pos;
    fx32 x;
    fx32 y;
    fx32 z;
    int i;

    WiFiPlazaObj_GetPosition(obj, &pos);
    pos.x += FX32_CONST(8);
    pos.y += FX32_CONST(8);
    pos.z -= FX32_CONST(8);
    for (i = 0; i < burst->count; i++) {
        switch (i) {
        case 0:
            x = pos.x - FX32_CONST(16);
            y = pos.y;
            z = pos.z + FX32_CONST(16);
            break;
        case 1:
            x = pos.x + FX32_CONST(16);
            y = pos.y;
            z = pos.z + FX32_CONST(16);
            break;
        case 2:
            if (burst->count == 3) {
                x = pos.x;
                y = pos.y;
                z = pos.z - FX32_CONST(32);
            } else {
                x = pos.x - FX32_CONST(16);
                y = pos.y;
                z = pos.z - FX32_CONST(16);
            }
            break;
        case 3:
            x = pos.x + FX32_CONST(16);
            y = pos.y;
            z = pos.z - FX32_CONST(16);
            break;
        }
        WiFiPlazaInstrumentTween_Init(&burst->tweens[i], pos.x, x, pos.y, y, pos.z, z, 19);
        WiFiPlazaInstrumentWave_Init(&burst->waves[i], 0x1555, 0x71C, FX32_CONST(32));
    }
    WiFiPlazaInstrumentBurst_UpdateDrum(burst);
}

#pragma force_active on

BOOL WiFiPlazaInstrumentBurst_UpdateDrum(WiFiPlazaInstrumentBurst *burst) {
    int i;
    WiFiPlazaInstrumentWave *wave;
    WiFiPlazaInstrumentTween *tween;
    fx32 z;
    fx32 y;
    fx32 x;
    fx32 bob;

    i = 0;
    if (i < burst->count) {
        wave = burst->waves;
        tween = burst->tweens;
        do {
            if (burst->frame == 14) {
                WiFiPlazaInstrumentWave_Init(wave, 0, 0xE38, FX32_CONST(12));
            } else {
                WiFiPlazaInstrumentWave_Update(wave);
            }
            WiFiPlazaInstrumentTween_Update(tween, burst->frame);
            WiFiPlazaInstrumentWave_GetValue(&burst->waves[i], &bob);
            WiFiPlazaInstrumentTween_GetPosition(&burst->tweens[i], &x, &y, &z);
            y += bob;
            ModelInstance_SetPosition(burst->models[i], x, y, z);
            wave++;
            tween++;
        } while (++i < burst->count);
    }
    return burst->frame >= 22;
}

#pragma force_active reset

void WiFiPlazaInstrumentBurst_StartCymbal(WiFiPlazaInstrumentBurst *burst, WiFiPlazaObj *obj) {
    VecFx32 pos;
    int i;

    WiFiPlazaObj_GetPosition(obj, &pos);
    pos.x += FX32_CONST(8);
    pos.y += FX32_CONST(20);
    pos.z -= FX32_CONST(8);
    for (i = 0; i < burst->count; i++) {
        WiFiPlazaInstrumentBurst_StartHop(burst, i, &pos, 0xD000, 0x5000, 0x7000, 0xE000, 8);
    }
}

#pragma force_active on
#pragma opt_propagation off

// Three hops: each time the tweens end, the next one starts from where the model landed.
BOOL WiFiPlazaInstrumentBurst_UpdateCymbal(WiFiPlazaInstrumentBurst *burst) {
    struct {
        u32 done;
        VecFx32 pos;
    } out;

    int i = 0;
    int count = burst->count;

    if (count > 0) {
        WiFiPlazaInstrumentTween *tween = burst->tweens;
        u8 *tweenBytes = (u8 *)burst + 8;
        u8 *modelBytes = (u8 *)burst;
        do {
            out.done = WiFiPlazaInstrumentTween_Update(tween, burst->frame);
            WiFiPlazaInstrumentTween_GetPosition((WiFiPlazaInstrumentTween *)tweenBytes, &out.pos.x, &out.pos.y, &out.pos.z);
            ModelInstance_SetPosition(*(ModelInstance **)(modelBytes + 0xD8), out.pos.x, out.pos.y, out.pos.z);
            if (out.done == TRUE) {
                if (burst->phase == 0) {
                    WiFiPlazaInstrumentBurst_StartHop(burst, i, &out.pos, -0x3000, 0x4000, -0x3000, 0x3000, 3);
                } else {
                    WiFiPlazaInstrumentBurst_StartHop(burst, i, &out.pos, 0xA000, 0x5000, 0x6000, 0xA000, 4);
                }
            }
            i++;
            tween++;
            tweenBytes += sizeof(WiFiPlazaInstrumentTween);
            modelBytes += sizeof(ModelInstance *);
        } while (i < burst->count);
    }
    if (out.done == TRUE) {
        int phase = burst->phase + 1;
        if (phase < 3) {
            burst->phase = phase;
            burst->frame = 0;
        } else {
            return TRUE;
        }
    }
    return FALSE;
}

#pragma opt_propagation reset
#pragma force_active reset

void WiFiPlazaInstrumentBurst_StartHop(WiFiPlazaInstrumentBurst *burst, u32 index, VecFx32 *origin, fx32 dx01, fx32 dy01, fx32 dx23, fx32 dy23, s32 duration) {
    fx32 x;
    fx32 y;
    fx32 z;

    switch (index) {
    case 0:
        x = origin->x - dx01;
        y = origin->y + dy01;
        z = origin->z;
        break;
    case 1:
        x = origin->x + dx01;
        y = origin->y + dy01;
        z = origin->z;
        break;
    case 2:
        x = origin->x - dx23;
        y = origin->y + dy23;
        z = origin->z;
        break;
    case 3:
        x = origin->x + dx23;
        y = origin->y + dy23;
        z = origin->z;
        break;
    }
    WiFiPlazaInstrumentTween_Init(&burst->tweens[index], origin->x, x, origin->y, y, origin->z, z, duration);
    ModelInstance_SetPosition(burst->models[index], origin->x, origin->y, origin->z);
}

// First function of the light module (see src/wifi_plaza_event.c); it was linked into this file.
WiFiPlazaLight *WiFiPlazaLight_Create(enum HeapID heapID, u32 colorSet, u32 light1Set, u32 thirdSet) {
    WiFiPlazaLight *light = Heap_Alloc(heapID, sizeof(WiFiPlazaLight));
    int i;

    memset(light, 0, sizeof(WiFiPlazaLight));
    WiFiPlazaLight_UnpackColorSet((s8 *)light, sWiFiPlazaLightColorSets[colorSet]);
    WiFiPlazaLight_UnpackLight1(light, &sWiFiPlazaLight1Colors[light1Set]);
    for (i = 0; i < 2; i++) {
        light->lightTimers[i] = 60;
    }
    for (i = 0; i < 4; i++) {
        light->materialTimers[i] = 60;
    }
    WiFiPlazaLight_Apply(light);
    return light;
}
