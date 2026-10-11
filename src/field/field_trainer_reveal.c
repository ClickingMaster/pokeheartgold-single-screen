#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "field_effects.h"
#include "map_object.h"
#include "unk_0205FD20.h"

// The burst a hidden trainer makes when it jumps out of its hiding place (field effect renderer 15;
// movement command RevealTrainer): a billboard at the trainer's tile that plays its 7 frames once.

#define TRAINER_REVEAL_RECORD   11
#define TRAINER_REVEAL_RESOURCE 9
#define TRAINER_REVEAL_SEQUENCE 9
#define TRAINER_REVEAL_TEXTURE  10

// Renderer data, from FieldEffectManager_GetRendererData(manager, 15).
typedef struct TrainerReveal {
    FieldEffectManager *manager;
} TrainerReveal;

typedef struct TrainerRevealEffectArgs {
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    TrainerReveal *reveal;
    LocalMapObject *mapObject;
} TrainerRevealEffectArgs;

typedef struct TrainerRevealEffect {
    BOOL paused;
    u32 objectId;
    u32 mapId;
    u8 unk0C[4];
    TrainerRevealEffectArgs args;
    void *billboard;
} TrainerRevealEffect;

TrainerReveal *TrainerReveal_New(FieldEffectManager *manager);
void TrainerReveal_Free(TrainerReveal *reveal);
void TrainerReveal_LoadGfx(TrainerReveal *reveal);
void TrainerReveal_FreeGfx(TrainerReveal *reveal);
void TrainerReveal_Spawn(LocalMapObject *mapObject);
BOOL TrainerRevealEffect_Init(FieldEffect *effect, TrainerRevealEffect *work);
void TrainerRevealEffect_Destroy(FieldEffect *effect, TrainerRevealEffect *work);
void TrainerRevealEffect_Update(FieldEffect *effect, TrainerRevealEffect *work);
void TrainerRevealEffect_Draw(FieldEffect *effect, TrainerRevealEffect *work);

static const FieldEffectBasicTemplate sTrainerRevealEffectTemplate;
static const u32 sTrainerRevealRecordParams[6];

TrainerReveal *TrainerReveal_New(FieldEffectManager *manager) {
    TrainerReveal *reveal = FieldEffectManager_HeapAllocAndFill(manager, sizeof(TrainerReveal), FALSE, 0);
    reveal->manager = manager;
    TrainerReveal_LoadGfx(reveal);
    return reveal;
}

void TrainerReveal_Free(TrainerReveal *reveal) {
    TrainerReveal_FreeGfx(reveal);
    FieldEffectManager_HeapFree(reveal);
}

void TrainerReveal_LoadGfx(TrainerReveal *reveal) {
    FieldEffectManager_LoadBillboardResource(reveal->manager, TRAINER_REVEAL_RESOURCE, 0x7C);
    FieldEffectManager_LoadBillboardSequence(reveal->manager, TRAINER_REVEAL_SEQUENCE, 0x94);
    FieldEffectManager_LoadTexResource(reveal->manager, TRAINER_REVEAL_TEXTURE, 0x17, TRUE);
    FieldEffectManager_AddRecord(reveal->manager, TRAINER_REVEAL_RECORD, TRAINER_REVEAL_RESOURCE, TRAINER_REVEAL_SEQUENCE, TRAINER_REVEAL_TEXTURE, 0, sTrainerRevealRecordParams);
}

void TrainerReveal_FreeGfx(TrainerReveal *reveal) {
    FieldEffectManager_FreeBillboardResource(reveal->manager, TRAINER_REVEAL_RESOURCE);
    FieldEffectManager_FreeBillboardSequence(reveal->manager, TRAINER_REVEAL_SEQUENCE);
    FieldEffectManager_FreeTexResource(reveal->manager, TRAINER_REVEAL_TEXTURE);
    FieldEffectManager_ReleaseRecord(reveal->manager, TRAINER_REVEAL_RECORD);
}

void TrainerReveal_Spawn(LocalMapObject *mapObject) {
    TrainerRevealEffectArgs args;
    VecFx32 position;
    int x;
    int z;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);

    args.fieldSystem = MapObject_GetFieldSystem(mapObject);
    args.manager = manager;
    args.reveal = FieldEffectManager_GetRendererData(manager, 15);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    x = MapObject_GetXCoord(mapObject);
    z = MapObject_GetZCoord(mapObject);
    sub_020611C8(x, z, &position);
    FieldEffectManager_CreateEffect(manager, &sTrainerRevealEffectTemplate, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL TrainerRevealEffect_Init(FieldEffect *effect, TrainerRevealEffect *work) {
    VecFx32 position;
    work->args = *(TrainerRevealEffectArgs *)FieldEffect_GetUserData(effect);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    FieldEffect_GetPosition(effect, &position);
    work->billboard = FieldEffectManager_CreateBillboardFromRecord(work->args.manager, TRAINER_REVEAL_RECORD, &position);
    return TRUE;
}

void TrainerRevealEffect_Destroy(FieldEffect *effect, TrainerRevealEffect *work) {
    BillboardObject_Delete(work->billboard);
}

void TrainerRevealEffect_Update(FieldEffect *effect, TrainerRevealEffect *work) {
    if (work->paused == FALSE) {
        BillboardObject_AdvanceFrame(work->billboard, FX32_ONE);
        if (BillboardObject_GetSequenceFrame(work->billboard) / FX32_ONE >= 7) {
            FieldEffectManager_DestroyEffect((int)effect);
        }
    }
}

void TrainerRevealEffect_Draw(FieldEffect *effect, TrainerRevealEffect *work) {
    VecFx32 position;
    FieldEffect_GetPosition(effect, &position);
    position.z += 8 * FX32_ONE;
    BillboardObject_SetPosition(work->billboard, &position);
}

static const FieldEffectBasicTemplate sTrainerRevealEffectTemplate = {
    sizeof(TrainerRevealEffect),
    (FieldEffectInitFunc)TrainerRevealEffect_Init,
    (FieldEffectFunc)TrainerRevealEffect_Destroy,
    (FieldEffectFunc)TrainerRevealEffect_Update,
    (FieldEffectFunc)TrainerRevealEffect_Draw,
};

static const u32 sTrainerRevealRecordParams[6] = { 0, 7, 1, 0, 0, 2 };
