#include "global.h"

#include "constants/sndseq.h"

#include "nnsys/g3d/binres/res_struct_accessor.h"

#include "billboard_lists.h"
#include "field/field_effect_manager.h"
#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "task.h"
#include "field_effects.h"
#include "vram_transfer_manager.h"

// The emote bubble that pops up above a map object (field effect renderer 0x12): one billboard that
// rises into place, then plays one of 14 texture animations by streaming frames into its VRAM slot.

// Not declared in any header yet.
void *BillboardObject_Delete(void *billboard);
void BillboardObject_SetPosition(void *billboard, const VecFx32 *position);
void BillboardGfxSequence_Init(void *data, void *sequence);
void *FieldEffect_GetParam(void *effect);
void FieldEffect_SetPosition(void *effect, VecFx32 *position);
void *Texture_GetImageData(NNSG3dResTex *tex, int index);
void *Texture_GetPaletteData(NNSG3dResTex *tex, int index);
u32 Texture_GetImageVramOffsetByIndex(NNSG3dResTex *tex, int index);
u32 Texture_GetImageSizeByIndex(NNSG3dResTex *tex, int index);
void PlaySE(u16 seq);

#define EMOTE_BUBBLE_RECORD   0xE
#define EMOTE_BUBBLE_RESOURCE 0xC
#define EMOTE_BUBBLE_TEXTURE  0xD

// Renderer data, from FieldEffectManager_GetRendererData(manager, 0x12).
typedef struct EmoteBubble {
    FieldEffectManager *manager;
    BOOL active;
} EmoteBubble;

typedef struct EmoteBubbleEffectArgs {
    FieldEffectManager *manager;
    EmoteBubble *bubble;
    LocalMapObject *mapObject;
} EmoteBubbleEffectArgs;

typedef struct EmoteBubbleTaskEnv {
    int kind;
    LocalMapObject *mapObject;
} EmoteBubbleTaskEnv;

// Laid out like the sequence BillboardGfxSequence_Init fills in, plus the frame-streaming state.
typedef struct EmoteBubbleAnim {
    u16 *durations;
    u8 *frames;
    u32 unk8;
    u32 frameCount;
    NNSG3dResTex *tex;
    void *sequenceFile;
    void *texFile;
    u32 vramOffset;
    u32 imageSize;
    u16 timer;
    u16 frame;
} EmoteBubbleAnim;

typedef struct EmoteBubbleEffect {
    int state;
    int timer;
    u32 objectId;
    u32 mapId;
    int hidden;
    int done;
    VecFx32 offset;
    u8 unk24[4];
    fx32 velocity;
    u8 unk2C[4];
    EmoteBubbleEffectArgs args;
    void *billboard;
    EmoteBubbleAnim anim;
} EmoteBubbleEffect;

typedef struct EmoteBubbleFiles {
    u32 texFileId;
    u32 sequenceFileId;
} EmoteBubbleFiles;

EmoteBubble *EmoteBubble_New(FieldEffectManager *manager);
void EmoteBubble_Free(EmoteBubble *bubble);
BOOL EmoteBubble_Show(LocalMapObject *mapObject, int kind);
void FieldSystem_ShowEmoteBubble(FieldSystem *fieldSystem, LocalMapObject *mapObject, int kind);
BOOL EmoteBubble_ShowTask(TaskManager *taskManager);
void EmoteBubble_LoadGfx(EmoteBubble *bubble);
void EmoteBubble_FreeGfx(EmoteBubble *bubble);
void *EmoteBubble_CreateBillboard(FieldEffectManager *manager, const VecFx32 *position);
BOOL EmoteBubbleEffect_Init(FieldEffect *effect, EmoteBubbleEffect *work);
void EmoteBubbleEffect_Destroy(FieldEffect *effect, EmoteBubbleEffect *work);
void EmoteBubbleEffect_Update(FieldEffect *effect, EmoteBubbleEffect *work);
void EmoteBubbleEffect_Draw(FieldEffect *effect, EmoteBubbleEffect *work);
BOOL EmoteBubbleAnim_Advance(EmoteBubbleAnim *anim);

static const FieldEffectBasicTemplate sEmoteBubbleEffectTemplate = {
    sizeof(EmoteBubbleEffect),
    (FieldEffectInitFunc)EmoteBubbleEffect_Init,
    (FieldEffectFunc)EmoteBubbleEffect_Destroy,
    (FieldEffectFunc)EmoteBubbleEffect_Update,
    (FieldEffectFunc)EmoteBubbleEffect_Draw,
};

static const u32 sEmoteBubbleRecordParams[6] = { 0, 1, 0, 0, 0, 2 };

static const EmoteBubbleFiles sEmoteBubbleFiles[14] = {
    { 0x2, 0x96 },
    { 0x3, 0x97 },
    { 0x4, 0x98 },
    { 0x5, 0x99 },
    { 0x6, 0x9A },
    { 0x7, 0x9B },
    { 0x8, 0x9C },
    { 0x9, 0x9D },
    { 0xA, 0x9E },
    { 0xB, 0x9F },
    { 0xC, 0xA0 },
    { 0xD, 0xA1 },
    { 0xE, 0xA2 },
    { 0xF, 0xA3 },
};

EmoteBubble *EmoteBubble_New(FieldEffectManager *manager) {
    EmoteBubble *bubble = FieldEffectManager_HeapAllocAndFill(manager, sizeof(EmoteBubble), FALSE, 0);
    bubble->manager = manager;
    bubble->active = FALSE;
    EmoteBubble_LoadGfx(bubble);
    return bubble;
}

void EmoteBubble_Free(EmoteBubble *bubble) {
    EmoteBubble_FreeGfx(bubble);
    FieldEffectManager_HeapFree(bubble);
}

// Returns FALSE while another bubble is up; the success path falls off the end without a value.
BOOL EmoteBubble_Show(LocalMapObject *mapObject, int kind) {
    EmoteBubbleEffectArgs args;
    VecFx32 position;
    VecFx32 facing;

    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);
    args.manager = manager;
    EmoteBubble *bubble = FieldEffectManager_GetRendererData(manager, 0x12);
    args.bubble = bubble;
    args.mapObject = mapObject;
    if (bubble->active) {
        return FALSE;
    }
    bubble->active = TRUE;
    MapObject_CopyPositionVector(mapObject, &position);
    MapObject_CopyFacingVector(mapObject, &facing);
    facing.y = 0;
    VEC_Add(&position, &facing, &position);
    int priority = MapObject_GetPriority(mapObject) + 1;
    FieldEffectManager_CreateEffect(manager, &sEmoteBubbleEffectTemplate, &position, kind, &args, priority);
}

void FieldSystem_ShowEmoteBubble(FieldSystem *fieldSystem, LocalMapObject *mapObject, int kind) {
    EmoteBubbleTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(EmoteBubbleTaskEnv));
    env->kind = kind;
    env->mapObject = mapObject;
    TaskManager_Call(fieldSystem->taskman, EmoteBubble_ShowTask, env);
}

BOOL EmoteBubble_ShowTask(TaskManager *taskManager) {
    u32 *state = TaskManager_GetStatePtr(taskManager);
    EmoteBubbleTaskEnv *env = TaskManager_GetEnvironment(taskManager);
    switch (*state) {
    case 0:
        EmoteBubble_Show(env->mapObject, env->kind);
        (*state)++;
        break;
    case 1: {
        EmoteBubble *bubble = FieldEffectManager_GetRendererData(FieldEffectManager_GetFromMapObject(env->mapObject), 0x12);
        if (!bubble->active) {
            Heap_Free(env);
            return TRUE;
        }
        break;
    }
    }
    return FALSE;
}

void EmoteBubble_LoadGfx(EmoteBubble *bubble) {
    FieldEffectManager_LoadBillboardResource(bubble->manager, EMOTE_BUBBLE_RESOURCE, 0x82);
    FieldEffectManager_LoadBillboardSequence(bubble->manager, EMOTE_BUBBLE_RESOURCE, 0x8c);
    FieldEffectManager_LoadTexResource(bubble->manager, EMOTE_BUBBLE_TEXTURE, 0x1c, TRUE);
    FieldEffectManager_AddRecord(bubble->manager, EMOTE_BUBBLE_RECORD, EMOTE_BUBBLE_RESOURCE, EMOTE_BUBBLE_RESOURCE, EMOTE_BUBBLE_TEXTURE, 0, sEmoteBubbleRecordParams);
}

void EmoteBubble_FreeGfx(EmoteBubble *bubble) {
    FieldEffectManager_FreeBillboardResource(bubble->manager, EMOTE_BUBBLE_RESOURCE);
    FieldEffectManager_FreeBillboardSequence(bubble->manager, EMOTE_BUBBLE_RESOURCE);
    FieldEffectManager_FreeTexResource(bubble->manager, EMOTE_BUBBLE_TEXTURE);
    FieldEffectManager_ReleaseRecord(bubble->manager, EMOTE_BUBBLE_RECORD);
}

void *EmoteBubble_CreateBillboard(FieldEffectManager *manager, const VecFx32 *position) {
    void *billboard = FieldEffectManager_CreateBillboardFromRecord(manager, EMOTE_BUBBLE_RECORD, position);
    NNS_G3dMdlSetMdlFogEnableFlagAll(BillboardObject_GetModel(billboard), FALSE);
    return billboard;
}

BOOL EmoteBubbleEffect_Init(FieldEffect *effect, EmoteBubbleEffect *work) {
    VecFx32 position;

    work->args = *(EmoteBubbleEffectArgs *)FieldEffect_GetUserData(effect);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    work->velocity = FX32_CONST(6);
    u32 kind = (u32)FieldEffect_GetParam(effect);
    work->anim.texFile = FieldEffectManager_LoadNarcMember(work->args.manager, sEmoteBubbleFiles[kind].texFileId, TRUE);
    work->anim.sequenceFile = FieldEffectManager_LoadNarcMember(work->args.manager, sEmoteBubbleFiles[kind].sequenceFileId, TRUE);
    BillboardGfxSequence_Init(work->anim.sequenceFile, &work->anim);
    work->anim.tex = NNS_G3dGetTex((const NNSG3dResFileHeader *)work->anim.texFile);
    work->anim.timer = 0;
    work->anim.frame = 0;
    NNSG3dResTex *tex = FieldEffectManager_GetTexture(work->args.manager, EMOTE_BUBBLE_TEXTURE);
    work->anim.imageSize = Texture_GetImageSizeByIndex(tex, 0);
    work->anim.vramOffset = Texture_GetImageVramOffsetByIndex(tex, 0);
    GF_CreateNewVramTransferTask((NNS_GFD_DST_TYPE)0, work->anim.vramOffset, Texture_GetImageData(work->anim.tex, 0), work->anim.imageSize);
    u32 plttAddr = NNS_GfdGetTexKeyAddr(tex->plttInfo.vramKey);
    GF_CreateNewVramTransferTask((NNS_GFD_DST_TYPE)1, plttAddr, Texture_GetPaletteData(work->anim.tex, 0), 0x20);
    FieldEffect_GetPosition(effect, &position);
    work->billboard = EmoteBubble_CreateBillboard(work->args.manager, &position);
    BillboardObject_SetVisible(work->billboard, 0);
    PlaySE(SEQ_SE_DP_DECIDE);
    return TRUE;
}

void EmoteBubbleEffect_Destroy(FieldEffect *effect, EmoteBubbleEffect *work) {
    BillboardObject_Delete(work->billboard);
    Heap_Free(work->anim.sequenceFile);
    Heap_Free(work->anim.texFile);
}

void EmoteBubbleEffect_Update(FieldEffect *effect, EmoteBubbleEffect *work) {
    VecFx32 result;
    VecFx32 position;
    VecFx32 facing;
    VecFx32 v1;
    VecFx32 v0;

    LocalMapObject *mapObject = work->args.mapObject;
    if (!MapObject_CheckIDAndMap(mapObject, work->objectId, work->mapId)) {
        GF_AssertFail();
    }
    MapObject_CopyPositionVector(mapObject, &position);
    MapObject_CopyFacingVector(mapObject, &facing);
    MapObject_CopyDrawOffset(mapObject, &v1);
    sub_0205F9B0(mapObject, &v0);
    result.x = v0.x + (v1.x + (position.x + facing.x));
    result.y = v0.y + (position.y + v1.y);
    result.z = v0.z + (v1.z + (position.z + facing.z));
    result.y += 0x20000;
    result.z += FX32_ONE;
    switch (work->state) {
    case 0: // rise into place
        BillboardObject_SetVisible(work->billboard, 1);
        work->offset.y += work->velocity;
        if (work->offset.y > 0) {
            work->velocity -= FX32_CONST(2);
        } else {
            work->offset.y = 0;
            work->velocity = 0;
            work->state++;
        }
        break;
    case 1:
        if (EmoteBubbleAnim_Advance(&work->anim)) {
            work->state++;
        }
        break;
    case 2:
        if (++work->timer >= 2) {
            work->state++;
            work->timer = 0;
            work->done = 1;
        }
        break;
    case 3:
        work->args.bubble->active = FALSE;
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    FieldEffect_SetPosition(effect, &result);
}

void EmoteBubbleEffect_Draw(FieldEffect *effect, EmoteBubbleEffect *work) {
    VecFx32 position;

    if (work->hidden != 1) {
        FieldEffect_GetPosition(effect, &position);
        position.x += work->offset.x;
        position.y += work->offset.y;
        position.z += work->offset.z;
        BillboardObject_SetPosition(work->billboard, &position);
    }
}

BOOL EmoteBubbleAnim_Advance(EmoteBubbleAnim *anim) {
    anim->timer++;
    if (anim->timer >= anim->durations[anim->frame]) {
        anim->frame++;
        anim->timer = 0;
        if (anim->frame >= anim->frameCount) {
            return TRUE;
        }
        GF_CreateNewVramTransferTask((NNS_GFD_DST_TYPE)0, anim->vramOffset, Texture_GetImageData(anim->tex, anim->frames[anim->frame]), anim->imageSize);
    }
    return FALSE;
}
