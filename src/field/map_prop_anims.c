#include "global.h"

#include "constants/sndseq.h"

#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"

#include "field_system.h"
#include "heap.h"
#include "overlay_02.h"
#include "sound_playback.h"
#include "task.h"
#include "field_terrain.h"

// Animations of map props started by script commands: the Pokemon Center healing machine (one Poke Ball prop per
// Pokemon, then the machine's and the balls' animations and the healing fanfare), the elevator, and the one-shot
// animations of a two-state prop.

#define MAP_PROP_HEALING_MACHINE   0x25
#define MAP_PROP_HEALING_POKE_BALL 0x6b
#define MAP_PROP_ELEVATOR          0xd0

// one-shot animation tags
#define HEALING_ANIM_TAG_BALLS   0x10
#define HEALING_ANIM_TAG_MACHINE 0x20
#define ELEVATOR_ANIM_TAG        1

#define HEALING_BALL_DELAY 12 // frames between two Poke Balls

typedef struct PokecenterAnim {
    VecFx32 machinePos;
    u8 ballCount;
    u8 ballIndex;
    u8 timer;
    u8 state;
    u8 ballProps[6];
} PokecenterAnim;

typedef struct ElevatorAnim {
    u8 length;
    u8 direction;
    u8 state;
    u8 unused;
} ElevatorAnim;

// Poke Ball positions on the healing machine, relative to it (x, y and z of one VecFx32 per ball; asm data)
extern const VecFx32 sHealingMachineBallOffsets[];
extern const VecFx32 sHealingMachineBallOffsetsY[];
extern const VecFx32 sHealingMachineBallOffsetsZ[];
// area_data.c; not declared in field/area_data.h while overlay 2's private headers declare it with other types
NNSG3dResFileHeader **AreaDataManager_GetMapPropModel(u32 mapPropId, AreaDataManager *areaDataManager);

typedef struct MapPropModelPair {
    int models[2];
} MapPropModelPair;

// the two prop models whose animations PCProp_LoadAnimations loads (asm data)
extern const MapPropModelPair sPCPropModels;

static inline NNSG3dResMdl *MapPropModel_Get(AreaDataManager *areaDataManager, int mapPropId) {
    return NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*AreaDataManager_GetMapPropModel(mapPropId, areaDataManager)), 0);
}

static inline NNSG3dResMdl *MapPropModelFile_GetModel(NNSG3dResFileHeader **modelFile) {
    return NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*modelFile), 0);
}

#pragma force_active on
BOOL PokecenterAnimRun(TaskManager *taskManager);

BOOL PokecenterAnimRun(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    PokecenterAnim *anim = TaskManager_GetEnvironment(taskManager);
    MapProp *machine;
    VecFx32 pos;
    NNSG3dResFileHeader **ballModelFile;
    NNSG3dResFileHeader **machineModelFile;
    NNSG3dResMdl *ballModel;
    NNSG3dResMdl *machineModel;
    NNSG3dRenderObj *renderObj;
    u8 i;

    switch (anim->state) {
    case 0:
        ballModelFile = AreaDataManager_GetMapPropModel(MAP_PROP_HEALING_POKE_BALL, fieldSystem->areaDataManager);
        machineModelFile = AreaDataManager_GetMapPropModel(MAP_PROP_HEALING_MACHINE, fieldSystem->areaDataManager);
        ballModel = MapPropModelFile_GetModel(ballModelFile);
        machineModel = MapPropModelFile_GetModel(machineModelFile);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_BALLS, MAP_PROP_HEALING_POKE_BALL, NULL, ballModel, AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager), 1, 1, FALSE);
        if (!FieldSystem_FindMapProp(fieldSystem, MAP_PROP_HEALING_MACHINE, &machine, NULL)) {
            GF_AssertFail();
        }
        renderObj = MapProp_GetRenderSurface(machine);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_MACHINE, MAP_PROP_HEALING_MACHINE, renderObj, machineModel, AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager), 1, 1, FALSE);
        anim->state++;
        break;
    case 1: {
        VecFx32 rotation = { 0 };
        pos.x = anim->machinePos.x + sHealingMachineBallOffsets[anim->ballIndex].x;
        pos.y = anim->machinePos.y + sHealingMachineBallOffsetsY[anim->ballIndex].x;
        pos.z = anim->machinePos.z + sHealingMachineBallOffsetsZ[anim->ballIndex].x;
        PlaySE(SEQ_SE_DP_BOWA);
        anim->ballProps[anim->ballIndex] = MapPropManager_LoadOne(fieldSystem->mapPropManager, MAP_PROP_HEALING_POKE_BALL, &pos, &rotation, fieldSystem->mapPropAnimationManager);
        anim->state++;
        break;
    }
    case 2:
        if (anim->timer < HEALING_BALL_DELAY) {
            anim->timer++;
        } else {
            anim->timer = 0;
            anim->ballIndex++;
            if (anim->ballIndex < anim->ballCount) {
                anim->state = 1;
            } else {
                MapPropOneShotAnimationManager_SetAnimationRenderObj(fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_BALLS, 0, MapProp_GetRenderSurface(MapPropManager_GetMapPropByIndex_Checked_RequireActive(fieldSystem->mapPropManager, anim->ballProps[0])));
                anim->state++;
            }
        }
        break;
    case 3:
        MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_BALLS, 0);
        MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_MACHINE, 0);
        PlayFanfare(SEQ_ME_ASA);
        anim->state++;
        break;
    case 4:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_BALLS)
            && MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_MACHINE)
            && !IsFanfarePlaying()) {
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_MACHINE);
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, HEALING_ANIM_TAG_BALLS);
            for (i = 0; i < anim->ballCount; i = (u8)(i + 1)) {
                MapPropManager_RemoveMapPropByIndex(anim->ballProps[i], fieldSystem->mapPropManager);
            }
            anim->state++;
        }
        break;
    case 5:
        Heap_Free(anim);
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

BOOL ElevatorAnimRun(TaskManager *taskManager);

void ElevatorAnimCreate(FieldSystem *fieldSystem, u8 direction, u8 length) {
    if (FieldSystem_FindMapProp(fieldSystem, MAP_PROP_ELEVATOR, NULL, NULL)) {
        ElevatorAnim *anim = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(ElevatorAnim));
        anim->length = length;
        anim->direction = direction;
        anim->state = 0;
        TaskManager_Call(fieldSystem->taskman, ElevatorAnimRun, anim);
    } else {
        GF_AssertFail();
    }
}

BOOL ElevatorAnimRun(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    ElevatorAnim *anim = TaskManager_GetEnvironment(taskManager);

    switch (anim->state) {
    case 0: {
        MapProp *elevator;
        NNSG3dResMdl *model = MapPropModel_Get(fieldSystem->areaDataManager, MAP_PROP_ELEVATOR);
        NNSG3dRenderObj *renderObj;
        NNSG3dResTex *texture;
        if (FieldSystem_FindMapProp(fieldSystem, MAP_PROP_ELEVATOR, &elevator, NULL) == FALSE) {
            GF_AssertFail();
        }
        renderObj = MapProp_GetRenderSurface(elevator);
        texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, ELEVATOR_ANIM_TAG, MAP_PROP_ELEVATOR, renderObj, model, texture, 2, anim->length, FALSE);
        anim->state++;
        break;
    }
    case 1:
        GF_ASSERT(anim->direction == 0 || anim->direction == 1);
        MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, ELEVATOR_ANIM_TAG, anim->direction);
        PlaySE(SEQ_SE_DP_ELEBETA2);
        anim->state++;
        break;
    case 2:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, ELEVATOR_ANIM_TAG)) {
            StopSE(SEQ_SE_DP_ELEBETA2, 0);
            PlaySE(SEQ_SE_DP_PINPON);
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, ELEVATOR_ANIM_TAG);
            anim->state++;
        }
        break;
    case 3:
        if (!IsSEPlaying(SEQ_SE_DP_PINPON)) {
            anim->state++;
        }
        break;
    case 4:
        Heap_Free(anim);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
// Loads the two one-shot animations of the first of the props sPCPropModels found on the map under the tag.
void PCProp_LoadAnimations(FieldSystem *fieldSystem, u8 tag) {
    MapProp *prop;
    int model;
    MapPropModelPair pair = sPCPropModels;

    if (FieldSystem_FindMapPropOfModels(fieldSystem, pair.models, 2, &prop, &model)) {
        NNSG3dResTex *texture = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
        NNSG3dRenderObj *renderObj = MapProp_GetRenderSurface(prop);
        NNSG3dResMdl *resModel = MapProp_GetResModel(prop);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, tag, model, renderObj, resModel, texture, 2, 1, FALSE);
    } else {
        GF_AssertFail();
    }
}
#pragma force_active reset

void PCProp_PlayPowerOnAnimation(FieldSystem *fieldSystem, u8 tag) {
    MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, tag, 0);
}

void PCProp_PlayPowerOffAnimation(FieldSystem *fieldSystem, u8 tag) {
    MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, tag, 1);
}
