#include "global.h"

#include "constants/sndseq.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"
#include "field/map_object_models.h"

#include "billboard_lists.h"
#include "field_effects.h"
#include "map_object.h"
#include "sound_playback.h"

// The "!" and "?" a map object shows over its head (field effect renderer 9; movement commands
// EmoteExclamationMark and EmoteQuestionMark, a trainer spotting the player, a bite while fishing, an item
// under a smashed rock): the mark pops up and falls back (EMOTE_MARK_POP_SPEED, slowing by
// EMOTE_MARK_POP_GRAVITY each frame), then the effect reports done after EMOTE_MARK_HOLD_FRAMES and stays
// until its owner destroys it. Each mark's graphics are loaded by the first running effect that needs
// them and freed when the last one ends.

#define EMOTE_MARK_EXCLAMATION 0
#define EMOTE_MARK_QUESTION    1

#define EMOTE_MARK_EXCLAMATION_RECORD   1
#define EMOTE_MARK_EXCLAMATION_RESOURCE 1
#define EMOTE_MARK_EXCLAMATION_SEQUENCE 1
#define EMOTE_MARK_EXCLAMATION_TEXTURE  1
#define EMOTE_MARK_QUESTION_RECORD      12
#define EMOTE_MARK_QUESTION_RESOURCE    10
#define EMOTE_MARK_QUESTION_SEQUENCE    10
#define EMOTE_MARK_QUESTION_TEXTURE     11

#define EMOTE_MARK_POP_SPEED   (6 * FX32_ONE)
#define EMOTE_MARK_POP_GRAVITY (2 * FX32_ONE)
#define EMOTE_MARK_HOLD_FRAMES 30

// Renderer data, from FieldEffectManager_GetRendererData(manager, 9).
typedef struct EmoteMark {
    int exclamationRefCount;
    int questionRefCount;
    BOOL exclamationGfxLoaded;
    BOOL questionGfxLoaded;
    FieldEffectManager *manager;
} EmoteMark;

typedef struct EmoteMarkEffectArgs {
    int kind;
    FieldEffectManager *manager;
    EmoteMark *emoteMark;
    LocalMapObject *mapObject;
} EmoteMarkEffectArgs;

typedef struct EmoteMarkEffect {
    int state;
    int timer;
    u32 objectId;
    u32 mapId;
    BOOL hidden;
    BOOL done;
    VecFx32 offset;
    u8 unk24[4];
    fx32 velocity;
    u8 unk2C[4];
    EmoteMarkEffectArgs args;
    void *billboard;
} EmoteMarkEffect;

EmoteMark *EmoteMark_New(FieldEffectManager *manager);
void EmoteMark_Free(EmoteMark *emoteMark);
void EmoteMark_RetainExclamation(EmoteMark *emoteMark);
void EmoteMark_ReleaseExclamation(EmoteMark *emoteMark);
void EmoteMark_RetainQuestion(EmoteMark *emoteMark);
void EmoteMark_ReleaseQuestion(EmoteMark *emoteMark);
void EmoteMark_Retain(EmoteMark *emoteMark, int kind);
void EmoteMark_Release(EmoteMark *emoteMark, int kind);
void EmoteMark_LoadExclamationGfx(EmoteMark *emoteMark);
void EmoteMark_LoadQuestionGfx(EmoteMark *emoteMark);
void EmoteMark_FreeExclamationGfx(EmoteMark *emoteMark);
void EmoteMark_FreeQuestionGfx(EmoteMark *emoteMark);
void EmoteMark_LoadExclamationGfxIfUnused(EmoteMark *emoteMark);
void EmoteMark_LoadQuestionGfxIfUnused(EmoteMark *emoteMark);
void EmoteMark_FreeExclamationGfxIfUnused(EmoteMark *emoteMark);
void EmoteMark_FreeQuestionGfxIfUnused(EmoteMark *emoteMark);
void EmoteMark_LoadGfxIfUnused(EmoteMark *emoteMark, int kind);
void EmoteMark_FreeGfxIfUnused(EmoteMark *emoteMark, int kind);
void *EmoteMark_CreateBillboard(FieldEffectManager *manager, int kind, VecFx32 *position);
BOOL EmoteMark_IsDone(FieldEffect *effect);
void EmoteMark_Hide(FieldEffect *effect);
BOOL EmoteMarkEffect_Init(FieldEffect *effect, EmoteMarkEffect *work);
void EmoteMarkEffect_Update(FieldEffect *effect, EmoteMarkEffect *work);
void EmoteMarkEffect_Destroy(FieldEffect *effect, EmoteMarkEffect *work);
void EmoteMarkEffect_Draw(FieldEffect *effect, EmoteMarkEffect *work);
FieldEffect *EmoteMark_Spawn(LocalMapObject *mapObject, int kind, BOOL playSound);
BOOL MapObjectEmoteMarkEffect_Init(FieldEffect *effect, EmoteMarkEffect *work);
void MapObjectEmoteMarkEffect_Update(FieldEffect *effect, EmoteMarkEffect *work);

// The effect templates and the record parameters ({ 0, 1, 0, 0, 0, 2 }) stay in asm
// (asm/overlay_01_022001E4.s, linked after this file). sEmoteMarkTemplate is a mark at a fixed position that
// nothing creates; sMapObjectEmoteMarkTemplate follows a map object.
extern const FieldEffectBasicTemplate sMapObjectEmoteMarkTemplate;
extern const u32 sEmoteMarkRecordParams[6];

EmoteMark *EmoteMark_New(FieldEffectManager *manager) {
    EmoteMark *emoteMark = FieldEffectManager_HeapAllocAndFill(manager, sizeof(EmoteMark), FALSE, 0);
    emoteMark->manager = manager;
    return emoteMark;
}

void EmoteMark_Free(EmoteMark *emoteMark) {
    EmoteMark_FreeExclamationGfx(emoteMark);
    FieldEffectManager_HeapFree(emoteMark);
}

void EmoteMark_RetainExclamation(EmoteMark *emoteMark) {
    emoteMark->exclamationRefCount++;
}

void EmoteMark_ReleaseExclamation(EmoteMark *emoteMark) {
    if (--emoteMark->exclamationRefCount < 0) {
        GF_AssertFail();
    }
}

void EmoteMark_RetainQuestion(EmoteMark *emoteMark) {
    emoteMark->questionRefCount++;
}

void EmoteMark_ReleaseQuestion(EmoteMark *emoteMark) {
    if (--emoteMark->questionRefCount < 0) {
        GF_AssertFail();
    }
}

void EmoteMark_Retain(EmoteMark *emoteMark, int kind) {
    if (kind == EMOTE_MARK_EXCLAMATION) {
        EmoteMark_RetainExclamation(emoteMark);
    } else {
        EmoteMark_RetainQuestion(emoteMark);
    }
}

void EmoteMark_Release(EmoteMark *emoteMark, int kind) {
    if (kind == EMOTE_MARK_EXCLAMATION) {
        EmoteMark_ReleaseExclamation(emoteMark);
    } else {
        EmoteMark_ReleaseQuestion(emoteMark);
    }
}

void EmoteMark_LoadExclamationGfx(EmoteMark *emoteMark) {
    if (emoteMark->exclamationGfxLoaded == FALSE) {
        emoteMark->exclamationGfxLoaded = TRUE;
        FieldEffectManager_LoadBillboardResource(emoteMark->manager, EMOTE_MARK_EXCLAMATION_RESOURCE, 0x76);
        FieldEffectManager_LoadBillboardSequence(emoteMark->manager, EMOTE_MARK_EXCLAMATION_SEQUENCE, 0x8C);
        FieldEffectManager_LoadTexResource(emoteMark->manager, EMOTE_MARK_EXCLAMATION_TEXTURE, 0x11, TRUE);
        FieldEffectManager_AddRecord(emoteMark->manager, EMOTE_MARK_EXCLAMATION_RECORD, EMOTE_MARK_EXCLAMATION_RESOURCE, EMOTE_MARK_EXCLAMATION_SEQUENCE, EMOTE_MARK_EXCLAMATION_TEXTURE, 0, sEmoteMarkRecordParams);
    }
}

void EmoteMark_LoadQuestionGfx(EmoteMark *emoteMark) {
    if (emoteMark->questionGfxLoaded == FALSE) {
        emoteMark->questionGfxLoaded = TRUE;
        FieldEffectManager_LoadBillboardResource(emoteMark->manager, EMOTE_MARK_QUESTION_RESOURCE, 0x7D);
        FieldEffectManager_LoadBillboardSequence(emoteMark->manager, EMOTE_MARK_QUESTION_SEQUENCE, 0x8C);
        FieldEffectManager_LoadTexResource(emoteMark->manager, EMOTE_MARK_QUESTION_TEXTURE, 0x18, TRUE);
        FieldEffectManager_AddRecord(emoteMark->manager, EMOTE_MARK_QUESTION_RECORD, EMOTE_MARK_QUESTION_RESOURCE, EMOTE_MARK_QUESTION_SEQUENCE, EMOTE_MARK_QUESTION_TEXTURE, 0, sEmoteMarkRecordParams);
    }
}

void EmoteMark_FreeExclamationGfx(EmoteMark *emoteMark) {
    if (emoteMark->exclamationGfxLoaded == TRUE) {
        emoteMark->exclamationGfxLoaded = FALSE;
        FieldEffectManager_FreeBillboardResource(emoteMark->manager, EMOTE_MARK_EXCLAMATION_RESOURCE);
        FieldEffectManager_FreeBillboardSequence(emoteMark->manager, EMOTE_MARK_EXCLAMATION_SEQUENCE);
        FieldEffectManager_FreeTexResource(emoteMark->manager, EMOTE_MARK_EXCLAMATION_TEXTURE);
        FieldEffectManager_ReleaseRecord(emoteMark->manager, EMOTE_MARK_EXCLAMATION_RECORD);
    }
}

void EmoteMark_FreeQuestionGfx(EmoteMark *emoteMark) {
    if (emoteMark->questionGfxLoaded == TRUE) {
        emoteMark->questionGfxLoaded = FALSE;
        FieldEffectManager_FreeBillboardResource(emoteMark->manager, EMOTE_MARK_QUESTION_RESOURCE);
        FieldEffectManager_FreeBillboardSequence(emoteMark->manager, EMOTE_MARK_QUESTION_SEQUENCE);
        FieldEffectManager_FreeTexResource(emoteMark->manager, EMOTE_MARK_QUESTION_TEXTURE);
        FieldEffectManager_ReleaseRecord(emoteMark->manager, EMOTE_MARK_QUESTION_RECORD);
    }
}

void EmoteMark_LoadExclamationGfxIfUnused(EmoteMark *emoteMark) {
    if (emoteMark->exclamationRefCount == 0) {
        EmoteMark_LoadExclamationGfx(emoteMark);
    }
}

void EmoteMark_LoadQuestionGfxIfUnused(EmoteMark *emoteMark) {
    if (emoteMark->questionRefCount == 0) {
        EmoteMark_LoadQuestionGfx(emoteMark);
    }
}

void EmoteMark_FreeExclamationGfxIfUnused(EmoteMark *emoteMark) {
    if (emoteMark->exclamationRefCount == 0) {
        EmoteMark_FreeExclamationGfx(emoteMark);
    }
}

void EmoteMark_FreeQuestionGfxIfUnused(EmoteMark *emoteMark) {
    if (emoteMark->questionRefCount == 0) {
        EmoteMark_FreeQuestionGfx(emoteMark);
    }
}

void EmoteMark_LoadGfxIfUnused(EmoteMark *emoteMark, int kind) {
    if (kind == EMOTE_MARK_EXCLAMATION) {
        EmoteMark_LoadExclamationGfxIfUnused(emoteMark);
    } else {
        EmoteMark_LoadQuestionGfxIfUnused(emoteMark);
    }
}

void EmoteMark_FreeGfxIfUnused(EmoteMark *emoteMark, int kind) {
    if (kind == EMOTE_MARK_EXCLAMATION) {
        EmoteMark_FreeExclamationGfxIfUnused(emoteMark);
    } else {
        EmoteMark_FreeQuestionGfxIfUnused(emoteMark);
    }
}

void *EmoteMark_CreateBillboard(FieldEffectManager *manager, int kind, VecFx32 *position) {
    int records[2] = { EMOTE_MARK_EXCLAMATION_RECORD, EMOTE_MARK_QUESTION_RECORD };
    void *billboard = FieldEffectManager_CreateBillboardFromRecord(manager, records[kind], position);
    NNSG3dResMdl *model = BillboardObject_GetModel(billboard);
    NNS_G3dMdlSetMdlFogEnableFlagAll(model, FALSE);
    return billboard;
}

BOOL EmoteMark_IsDone(FieldEffect *effect) {
    EmoteMarkEffect *work = FieldEffect_GetState(effect);
    return work->done;
}

void EmoteMark_Hide(FieldEffect *effect) {
    EmoteMarkEffect *work = FieldEffect_GetState(effect);
    work->hidden = TRUE;
    if (work->billboard != NULL) {
        BillboardObject_SetVisible(work->billboard, FALSE);
    }
}

BOOL EmoteMarkEffect_Init(FieldEffect *effect, EmoteMarkEffect *work) {
    VecFx32 position;
    work->args = *(EmoteMarkEffectArgs *)FieldEffect_GetUserData(effect);
    work->velocity = EMOTE_MARK_POP_SPEED;
    EmoteMark_LoadGfxIfUnused(work->args.emoteMark, work->args.kind);
    FieldEffect_GetPosition(effect, &position);
    work->billboard = EmoteMark_CreateBillboard(work->args.manager, work->args.kind, &position);
    EmoteMark_Retain(work->args.emoteMark, work->args.kind);
    BillboardObject_SetVisible(work->billboard, FALSE);
    if (FieldEffect_GetParam(effect) == TRUE) {
        PlaySE(SEQ_SE_DP_DECIDE);
    }
    return TRUE;
}

void EmoteMarkEffect_Update(FieldEffect *effect, EmoteMarkEffect *work) {
    VecFx32 position;
    FieldEffect_GetPosition(effect, &position);
    switch (work->state) {
    case 0:
        BillboardObject_SetVisible(work->billboard, TRUE);
        work->offset.y += work->velocity;
        if (work->offset.y != 0) {
            work->velocity -= EMOTE_MARK_POP_GRAVITY;
        } else {
            work->velocity = 0;
            work->state++;
        }
        break;
    case 1:
        work->timer++;
        if (work->timer >= EMOTE_MARK_HOLD_FRAMES) {
            work->state++;
            work->timer = 0;
            work->done = TRUE;
        }
        break;
    case 2:
        break;
    }
    FieldEffect_SetPosition(effect, &position);
}

void EmoteMarkEffect_Destroy(FieldEffect *effect, EmoteMarkEffect *work) {
    BillboardObject_Delete(work->billboard);
    EmoteMark_Release(work->args.emoteMark, work->args.kind);
    EmoteMark_FreeGfxIfUnused(work->args.emoteMark, work->args.kind);
}

void EmoteMarkEffect_Draw(FieldEffect *effect, EmoteMarkEffect *work) {
    VecFx32 position;
    if (work->hidden != TRUE) {
        FieldEffect_GetPosition(effect, &position);
        position.x += work->offset.x;
        position.y += work->offset.y;
        position.z += work->offset.z;
        BillboardObject_SetPosition(work->billboard, &position);
    }
}

// Shows the mark over mapObject; playSound plays SEQ_SE_DP_DECIDE as it appears.
FieldEffect *EmoteMark_Spawn(LocalMapObject *mapObject, int kind, BOOL playSound) {
    u32 priority;
    EmoteMarkEffectArgs args;
    VecFx32 position;
    VecFx32 facing;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);

    args.kind = kind;
    args.manager = manager;
    args.emoteMark = FieldEffectManager_GetRendererData(manager, 9);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    MapObject_CopyFacingVector(mapObject, &facing);
    VEC_Add(&position, &facing, &position);
    priority = MapObject_GetPriority(mapObject) + 1;
    return FieldEffectManager_CreateEffect(manager, &sMapObjectEmoteMarkTemplate, &position, playSound, &args, priority);
}

BOOL MapObjectEmoteMarkEffect_Init(FieldEffect *effect, EmoteMarkEffect *work) {
    VecFx32 position;
    work->args = *(EmoteMarkEffectArgs *)FieldEffect_GetUserData(effect);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    work->velocity = EMOTE_MARK_POP_SPEED;
    EmoteMark_LoadGfxIfUnused(work->args.emoteMark, work->args.kind);
    FieldEffect_GetPosition(effect, &position);
    work->billboard = EmoteMark_CreateBillboard(work->args.manager, work->args.kind, &position);
    EmoteMark_Retain(work->args.emoteMark, work->args.kind);
    BillboardObject_SetVisible(work->billboard, FALSE);
    if (FieldEffect_GetParam(effect) == TRUE) {
        PlaySE(SEQ_SE_DP_DECIDE);
    }
    return TRUE;
}

void MapObjectEmoteMarkEffect_Update(FieldEffect *effect, EmoteMarkEffect *work) {
    VecFx32 position;
    LocalMapObject *mapObject = work->args.mapObject;
    if (MapObject_CheckIDAndMap(mapObject, work->objectId, work->mapId) == FALSE) {
        GF_AssertFail();
    }
    MapObject_GetDrawPosition(mapObject, &position);
    position.y += 32 * FX32_ONE;
    position.z += FX32_ONE;
    switch (work->state) {
    case 0:
        BillboardObject_SetVisible(work->billboard, TRUE);
        work->offset.y += work->velocity;
        if (work->offset.y != 0) {
            work->velocity -= EMOTE_MARK_POP_GRAVITY;
        } else {
            work->velocity = 0;
            work->state++;
        }
        break;
    case 1:
        work->timer++;
        if (work->timer >= EMOTE_MARK_HOLD_FRAMES) {
            work->state++;
            work->timer = 0;
            work->done = TRUE;
        }
        break;
    case 2:
        break;
    }
    FieldEffect_SetPosition(effect, &position);
}
