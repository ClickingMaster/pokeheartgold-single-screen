#include "global.h"

#include "constants/buildmodel.h"
#include "constants/global_fieldmap.h"
#include "constants/scrcmd.h"
#include "constants/sndseq.h"

#include "field/area_data.h"
#include "field/field_3d_object_task.h"
#include "field/field_move_effects.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_021FB878.h"

#include "camera.h"
#include "field_system.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "map_matrix.h"
#include "map_object.h"
#include "math_util.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "sound_playback.h"
#include "sys_task.h"
#include "task.h"

// Hall of Fame Poke Ball animation and the 3D effects of the field moves: Cut, Rock Smash, Headbutt (started by
// script, at the player or at the following Pokemon), Waterfall, Whirlpool and the rock debris of Rock Climb.
// The models and animations are in NARC_a_1_3_4; the task templates and tables are in the overlay's data block.

#define HALL_OF_FAME_MACHINE  BUILD_MODEL_YO_H02
#define HALL_OF_FAME_BALL     BUILD_MODEL_TN_GYMBASE
#define HALL_OF_FAME_ANIM_TAG 0x10

// A map tile is 16 units wide; objects stand at the tile center.
#define TILE_SIZE           FX32_CONST(16)
#define TILE_TO_POS(tile)   ((tile) * TILE_SIZE + TILE_SIZE / 2)

#define NUM_ROCK_CLIMB_DEBRIS_PIECES 16
#define NUM_ROCK_DEBRIS_PIECES       18

typedef struct HallOfFameBallAnim {
    VecFx32 basePos;
    u8 numBalls;
    u8 numPlaced;
    u8 timer;
    u8 state;
    u8 ballProps[8];
} HallOfFameBallAnim;

// A model with up to four animations loaded from NARC_a_1_3_4.
typedef struct FieldMoveEffectModel {
    Field3dObject object;
    Field3dModel model;
    Field3DModelAnimation animations[4];
    u32 numAnimations;
} FieldMoveEffectModel;

// Cut and Rock Smash: the animations play once, then the effect hides itself.
typedef struct FieldMoveEffect {
    FieldMoveEffectModel model;
    NNSFndAllocator allocator;
    int state;
} FieldMoveEffect;

typedef struct HeadbuttEffect {
    FieldMoveEffectModel model;
    NNSFndAllocator allocator;
    VecFx32 cameraPos;
    VecFx32 cameraTarget;
    VecFx32 shakeOffset;
    volatile s8 shakeTimer;
    u8 shaken;
    u8 numShakes;
    u8 state;
} HeadbuttEffect;

// Waterfall: the start splash plays once at the player, then the loop splash follows the player.
typedef struct WaterfallEffect {
    FieldMoveEffectModel models[2];
    NNSFndAllocator allocator;
    u16 numLoaded;
    u16 state;
} WaterfallEffect;

typedef struct WhirlpoolEffect {
    FieldMoveEffectModel model;
    NNSFndAllocator allocator;
    u16 state;
} WhirlpoolEffect;

// One rock fragment cloud; the pieces of a debris effect share one model and its animation data.
typedef struct RockDebrisPiece {
    Field3dObject object;
    Field3DModelAnimation animations[4];
    BOOL active;
} RockDebrisPiece;

typedef struct RockClimbDebris {
    Field3dModel model;
    RockDebrisPiece pieces[NUM_ROCK_CLIMB_DEBRIS_PIECES];
    void *animData[4];
    FieldSystem *fieldSystem;
    int spawnTimer;
    fx32 offsetY;
    fx32 offsetZ;
    VecFx32 lastPlayerPos;
    NNSFndAllocator allocator;
    BOOL paused;
} RockClimbDebris;

typedef struct RockDebris {
    Field3dModel model;
    RockDebrisPiece pieces[NUM_ROCK_DEBRIS_PIECES];
    void *animData[4];
    FieldSystem *fieldSystem;
    int spawnTimer;
    fx32 offsetY;
    fx32 offsetZ;
    NNSFndAllocator allocator;
    u16 paused;
    u16 withFollower;
} RockDebris;

typedef struct ScriptFieldMoveEffectTask {
    Field3dObjectTask *effect;
    u16 *done;
    u32 state;
    u32 kind;
} ScriptFieldMoveEffectTask;

typedef struct FileIdPair {
    u8 ids[2];
} FileIdPair;

typedef struct RideEffectFileIds {
    FileIdPair whirlpool;       // model, animation
    FileIdPair waterfallModels; // start, loop
    FileIdPair waterfallAnims;  // first animation of the start and the loop model
    u16 whirlpoolAngles[4];     // by facing direction
} RideEffectFileIds;

extern const VecFx32 sHallOfFameBallOffsets[];
extern RideEffectFileIds sRideEffectFileIds;
extern const Field3dObjectTaskTemplate sRockSmashEffectAtPlayerTemplate;
extern const Field3dObjectTaskTemplate sWhirlpoolEffectTemplate;
extern const Field3dObjectTaskTemplate sHeadbuttEffectAtFollowerTemplate;
extern const Field3dObjectTaskTemplate sWaterfallEffectTemplate;
extern const Field3dObjectTaskTemplate sRockClimbDebrisTemplate;
extern const Field3dObjectTaskTemplate sCutEffectAtPlayerTemplate;
extern const Field3dObjectTaskTemplate sRockSmashEffectAtFollowerTemplate;
extern const Field3dObjectTaskTemplate sHeadbuttEffectAtPlayerTemplate;
extern const Field3dObjectTaskTemplate sCutEffectAtFollowerTemplate;
extern const Field3dObjectTaskTemplate sRockDebrisTemplate;
extern const Field3dObjectTaskTemplate sRockDebrisWithFollowerTemplate;
extern void (*const sScriptFieldMoveEffectDeleteFuncs[])(Field3dObjectTask *task);
extern Field3dObjectTask *(*const sScriptFieldMoveEffectCreateFuncs[])(FieldSystem *fieldSystem);
extern BOOL (*const sScriptFieldMoveEffectIsFinishedFuncs[])(Field3dObjectTask *task);

// Not declared in any header yet.
BOOL FieldSystem_FindMapProp(FieldSystem *fieldSystem, int buildModel, MapProp **outProp, int *outIdx);
void Terrain_GetChunkOrigin(int idx, int width, VecFx32 *pos);
NNSG3dResFileHeader **AreaDataManager_GetMapPropModel(u32 mapPropId, AreaDataManager *areaDataManager);
void Field3dObject_SetXRotation(Field3dObject *object, u16 angle, int axis);

BOOL Task_HallOfFameBallAnim(TaskManager *taskManager);
void Field3dObject_SetPosInFrontOfMapObject(MapObjectManager *manager, u32 id, Field3dObject *object);
void Field3dObject_SetPosInFrontOfPlayer(PlayerAvatar *playerAvatar, Field3dObject *object);
void Field3dObject_SetPosAtPlayer(PlayerAvatar *playerAvatar, Field3dObject *object);
void FieldMoveEffectModel_Load(FieldMoveEffectModel *model, int modelFileId, int firstAnimFileId, int numAnimations, NNSFndAllocator *allocator);
void FieldMoveEffectModel_Unload(FieldMoveEffectModel *model, NNSFndAllocator *allocator);
BOOL FieldMoveEffectModel_AnimateOnce(FieldMoveEffectModel *model);
void FieldMoveEffectModel_AnimateLoop(FieldMoveEffectModel *model);
void FieldMoveEffectModel_Draw(FieldMoveEffectModel *model);
void CutEffect_InitAtPlayer(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
void CutEffect_InitAtFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
void CutEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
void CutEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
void CutEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
Field3dObjectTask *CutEffect_CreateAtPlayer(FieldSystem *fieldSystem);
Field3dObjectTask *CutEffect_CreateAtFollower(FieldSystem *fieldSystem);
void CutEffect_Delete(Field3dObjectTask *task);
BOOL CutEffect_IsFinished(Field3dObjectTask *task);
void RockSmashEffect_InitAtPlayer(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
void RockSmashEffect_InitAtFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
void RockSmashEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
void RockSmashEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
void RockSmashEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect);
Field3dObjectTask *RockSmashEffect_CreateAtPlayer(FieldSystem *fieldSystem);
Field3dObjectTask *RockSmashEffect_CreateAtFollower(FieldSystem *fieldSystem);
void RockSmashEffect_Delete(Field3dObjectTask *task);
BOOL RockSmashEffect_IsFinished(Field3dObjectTask *task);
void WaterfallEffect_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, WaterfallEffect *effect);
void WaterfallEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, WaterfallEffect *effect);
void WaterfallEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, WaterfallEffect *effect);
void WaterfallEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, WaterfallEffect *effect);
void WhirlpoolEffect_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, WhirlpoolEffect *effect);
void WhirlpoolEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, WhirlpoolEffect *effect);
void WhirlpoolEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, WhirlpoolEffect *effect);
void WhirlpoolEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, WhirlpoolEffect *effect);
void RockClimbDebris_SpawnPiece(RockDebrisPiece *piece, PlayerAvatar *playerAvatar, fx32 offsetY, fx32 offsetZ);
void RockClimbDebris_UpdatePiece(RockDebrisPiece *piece);
void RockClimbDebris_InitPiece(RockDebrisPiece *piece, Field3dModel *model, NNSFndAllocator *allocator, void **animData);
void RockClimbDebris_FreePiece(RockDebrisPiece *piece, NNSFndAllocator *allocator);
void RockClimbDebris_UpdateOffsets(RockClimbDebris *debris);
void RockClimbDebris_Spawn(RockClimbDebris *debris);
void RockClimbDebris_UpdatePieces(RockClimbDebris *debris);
void RockClimbDebris_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, RockClimbDebris *debris);
void RockClimbDebris_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, RockClimbDebris *debris);
void RockClimbDebris_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, RockClimbDebris *debris);
void RockClimbDebris_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, RockClimbDebris *debris);
void HeadbuttEffect_InitAtPlayer(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect);
void HeadbuttEffect_InitAtFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect);
void HeadbuttEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect);
void HeadbuttEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect);
void HeadbuttEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect);
Field3dObjectTask *HeadbuttEffect_CreateAtPlayer(FieldSystem *fieldSystem);
Field3dObjectTask *HeadbuttEffect_CreateAtFollower(FieldSystem *fieldSystem);
void HeadbuttEffect_Delete(Field3dObjectTask *task);
BOOL HeadbuttEffect_IsFinished(Field3dObjectTask *task);
void RockDebris_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris);
void RockDebris_InitWithFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris);
void RockDebris_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris);
void RockDebris_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris);
void RockDebris_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris);
void RockDebris_SpawnPiece(RockDebrisPiece *piece, const VecFx32 *position, fx32 offsetY, fx32 offsetZ);
void RockDebris_UpdatePiece(RockDebrisPiece *piece);
void RockDebris_InitPiece(RockDebrisPiece *piece, Field3dModel *model, NNSFndAllocator *allocator, void **animData);
void RockDebris_FreePiece(RockDebrisPiece *piece, NNSFndAllocator *allocator);
void RockDebris_Spawn(RockDebris *debris);
void RockDebris_UpdatePieces(RockDebris *debris);
void Task_ScriptFieldMoveEffect(SysTask *task, void *data);

// Pops up one Poke Ball per party Pokemon on the Hall of Fame machine, then plays their animation.
void HallOfFame_StartBallAnim(FieldSystem *fieldSystem, u8 numBalls) {
    MapProp *machine;
    int machineIndex;
    VecFx32 tileCenter;
    VecFx32 machinePos;
    HallOfFameBallAnim *env;

    if (FieldSystem_FindMapProp(fieldSystem, HALL_OF_FAME_MACHINE, &machine, &machineIndex)) {
        env = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(HallOfFameBallAnim));
        env->numBalls = numBalls;
        env->numPlaced = 0;
        env->timer = 0;
        env->state = 0;
        Terrain_GetChunkOrigin(machineIndex, MapMatrix_GetWidth(fieldSystem->mapMatrix), &tileCenter);
        MapProp_GetTranslation(&machinePos, machine);
        env->basePos = machinePos;
        env->basePos.x += tileCenter.x;
        env->basePos.z += tileCenter.z;
        TaskManager_Call(fieldSystem->taskman, Task_HallOfFameBallAnim, env);
    } else {
        GF_ASSERT(FALSE);
    }
}

BOOL Task_HallOfFameBallAnim(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    HallOfFameBallAnim *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0: {
        NNSG3dResMdlSet *mdlSet = NNS_G3dGetMdlSet(*AreaDataManager_GetMapPropModel(HALL_OF_FAME_BALL, fieldSystem->areaDataManager));
        NNSG3dResMdl *mdl = NNS_G3dGetMdlByIdx(mdlSet, 0);
        NNSG3dResTex *tex = AreaDataManager_GetMapPropTexture(fieldSystem->areaDataManager);
        MapPropOneShotAnimationManager_LoadPropAnimations(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, HALL_OF_FAME_ANIM_TAG, HALL_OF_FAME_BALL, NULL, mdl, tex, 1, 1, FALSE);
        env->state++;
        break;
    }
    case 1: {
        VecFx32 pos;
        VecFx32 rotation;
        fx32 *coords = (fx32 *)&rotation;
        coords[0] = 0;
        coords[1] = 0;
        coords[2] = 0;
        pos.x = env->basePos.x + sHallOfFameBallOffsets[env->numPlaced].x;
        pos.y = env->basePos.y + sHallOfFameBallOffsets[env->numPlaced].y;
        pos.z = env->basePos.z + sHallOfFameBallOffsets[env->numPlaced].z;
        PlaySE(SEQ_SE_DP_BOWA);
        env->ballProps[env->numPlaced] = MapPropManager_LoadOne(fieldSystem->mapPropManager, HALL_OF_FAME_BALL, &pos, &rotation, fieldSystem->mapPropAnimationManager);
        env->state++;
        break;
    }
    case 2:
        if (env->timer < 15) {
            env->timer++;
        } else {
            env->timer = 0;
            env->numPlaced++;
            if (env->numPlaced < env->numBalls) {
                env->state = 1;
            } else {
                MapPropOneShotAnimationManager_SetAnimationRenderObj(fieldSystem->mapPropOneShotAnimationManager, HALL_OF_FAME_ANIM_TAG, 0, MapProp_GetRenderSurface(MapPropManager_GetMapPropByIndex_Checked_RequireActive(fieldSystem->mapPropManager, env->ballProps[0])));
                env->state++;
            }
        }
        break;
    case 3:
        MapPropOneShotAnimationManager_PlayAnimation(fieldSystem->mapPropOneShotAnimationManager, HALL_OF_FAME_ANIM_TAG, 0);
        env->state++;
        break;
    case 4:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, HALL_OF_FAME_ANIM_TAG)) {
            u8 i;
            MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, HALL_OF_FAME_ANIM_TAG);
            for (i = 0; i < env->numBalls; i++) {
                MapPropManager_RemoveMapPropByIndex(env->ballProps[i], fieldSystem->mapPropManager);
            }
            env->state++;
        }
        break;
    case 5:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

// Places the object on the tile in front of the map object, at the map object's height.
void Field3dObject_SetPosInFrontOfMapObject(MapObjectManager *manager, u32 id, Field3dObject *object) {
    VecFx32 pos;
    LocalMapObject *mapObject = MapObjectManager_GetFirstActiveObjectByID(manager, id);
    u32 x;
    u32 z;

    MapObject_CopyPositionVector(mapObject, &pos);
    x = MapObject_GetXCoord(mapObject);
    z = MapObject_GetZCoord(mapObject);
    switch (MapObject_GetFacingDirection(mapObject)) {
    case DIR_NORTH:
        z--;
        break;
    case DIR_SOUTH:
        z++;
        break;
    case DIR_WEST:
        x--;
        break;
    case DIR_EAST:
        x++;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    Field3dObject_SetPosEx(object, TILE_TO_POS(x), pos.y, TILE_TO_POS(z));
}

void Field3dObject_SetPosInFrontOfPlayer(PlayerAvatar *playerAvatar, Field3dObject *object) {
    VecFx32 pos;
    u32 x;
    u32 z;

    PlayerAvatar_CopyPositionVector(playerAvatar, &pos);
    x = PlayerAvatar_GetXCoord(playerAvatar);
    z = PlayerAvatar_GetZCoord(playerAvatar);
    switch (PlayerAvatar_GetFacingDirection(playerAvatar)) {
    case DIR_NORTH:
        z--;
        break;
    case DIR_SOUTH:
        z++;
        break;
    case DIR_WEST:
        x--;
        break;
    case DIR_EAST:
        x++;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    Field3dObject_SetPosEx(object, TILE_TO_POS(x), pos.y, TILE_TO_POS(z));
}

void Field3dObject_SetPosAtPlayer(PlayerAvatar *playerAvatar, Field3dObject *object) {
    VecFx32 pos;
    PlayerAvatar_CopyPositionVector(playerAvatar, &pos);
    Field3dObject_SetPosEx(object, pos.x, pos.y, pos.z);
}

void FieldMoveEffectModel_Load(FieldMoveEffectModel *model, int modelFileId, int firstAnimFileId, int numAnimations, NNSFndAllocator *allocator) {
    int i;

    memset(model, 0, sizeof(FieldMoveEffectModel));
    Field3dModel_LoadFromFilesystem(&model->model, NARC_a_1_3_4, modelFileId, HEAP_ID_FIELD1);
    Field3dObject_InitFromModel(&model->object, &model->model);
    model->numAnimations = numAnimations;
    for (i = 0; i < model->numAnimations; i++) {
        Field3dModelAnimation_LoadFromFilesystem(&model->animations[i], &model->model, NARC_a_1_3_4, firstAnimFileId + i, HEAP_ID_FIELD1, allocator);
        Field3dObject_AddAnimation(&model->object, &model->animations[i]);
    }
}

void FieldMoveEffectModel_Unload(FieldMoveEffectModel *model, NNSFndAllocator *allocator) {
    u32 i;

    Field3dModel_Unload(&model->model);
    for (i = 0; i < model->numAnimations; i++) {
        Field3dModelAnimation_Unload(&model->animations[i], allocator);
    }
}

// Returns TRUE once every animation has reached its last frame.
BOOL FieldMoveEffectModel_AnimateOnce(FieldMoveEffectModel *model) {
    BOOL finished = TRUE;
    u32 i;

    for (i = 0; i < model->numAnimations; i++) {
        finished &= Field3dModelAnimation_FrameAdvanceAndCheck(&model->animations[i], FX32_ONE);
    }
    return finished;
}

void FieldMoveEffectModel_AnimateLoop(FieldMoveEffectModel *model) {
    u32 i;

    for (i = 0; i < model->numAnimations; i++) {
        Field3dModelAnimation_FrameAdvanceAndLoop(&model->animations[i], FX32_ONE);
    }
}

void FieldMoveEffectModel_Draw(FieldMoveEffectModel *model) {
    Field3dObject_Draw(&model->object);
}

void CutEffect_InitAtPlayer(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    memset(effect, 0, sizeof(FieldMoveEffect));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 32);
    FieldMoveEffectModel_Load(&effect->model, 3, 0, 3, &effect->allocator);
    Field3dObject_SetPosInFrontOfPlayer(fieldSystem->playerAvatar, &effect->model.object);
    PlaySE(SEQ_SE_DP_FW015);
    effect->state = 0;
}

void CutEffect_InitAtFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    memset(effect, 0, sizeof(FieldMoveEffect));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 32);
    FieldMoveEffectModel_Load(&effect->model, 3, 0, 3, &effect->allocator);
    Field3dObject_SetPosInFrontOfMapObject(fieldSystem->mapObjectManager, obj_partner_poke, &effect->model.object);
    PlaySE(SEQ_SE_DP_FW015);
    effect->state = 0;
}

void CutEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    FieldMoveEffectModel_Unload(&effect->model, &effect->allocator);
}

void CutEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    switch (effect->state) {
    case 0:
        if (FieldMoveEffectModel_AnimateOnce(&effect->model) == TRUE) {
            Field3dObject_SetActiveFlag(&effect->model.object, FALSE);
            effect->state++;
        }
        break;
    case 1:
        break;
    }
}

void CutEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    FieldMoveEffectModel_Draw(&effect->model);
}

Field3dObjectTask *CutEffect_CreateAtPlayer(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sCutEffectAtPlayerTemplate);
}

Field3dObjectTask *CutEffect_CreateAtFollower(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sCutEffectAtFollowerTemplate);
}

void CutEffect_Delete(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}

BOOL CutEffect_IsFinished(Field3dObjectTask *task) {
    FieldMoveEffect *effect = Field3dObjectTask_GetData(task);
    return effect->state == 1;
}

void RockSmashEffect_InitAtPlayer(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    memset(effect, 0, sizeof(FieldMoveEffect));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 32);
    FieldMoveEffectModel_Load(&effect->model, 8, 4, 4, &effect->allocator);
    Field3dObject_SetPosInFrontOfPlayer(fieldSystem->playerAvatar, &effect->model.object);
    PlaySE(SEQ_SE_DP_FW088);
    effect->state = 0;
}

void RockSmashEffect_InitAtFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    memset(effect, 0, sizeof(FieldMoveEffect));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 32);
    FieldMoveEffectModel_Load(&effect->model, 8, 4, 4, &effect->allocator);
    Field3dObject_SetPosInFrontOfMapObject(fieldSystem->mapObjectManager, obj_partner_poke, &effect->model.object);
    PlaySE(SEQ_SE_DP_FW088);
    effect->state = 0;
}

void RockSmashEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    FieldMoveEffectModel_Unload(&effect->model, &effect->allocator);
}

void RockSmashEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    switch (effect->state) {
    case 0:
        if (FieldMoveEffectModel_AnimateOnce(&effect->model) == TRUE) {
            Field3dObject_SetActiveFlag(&effect->model.object, FALSE);
            effect->state++;
        }
        break;
    case 1:
        break;
    }
}

void RockSmashEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, FieldMoveEffect *effect) {
    FieldMoveEffectModel_Draw(&effect->model);
}

Field3dObjectTask *RockSmashEffect_CreateAtPlayer(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sRockSmashEffectAtPlayerTemplate);
}

Field3dObjectTask *RockSmashEffect_CreateAtFollower(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sRockSmashEffectAtFollowerTemplate);
}

void RockSmashEffect_Delete(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}

BOOL RockSmashEffect_IsFinished(Field3dObjectTask *task) {
    FieldMoveEffect *effect = Field3dObjectTask_GetData(task);
    return effect->state == 1;
}

void WaterfallEffect_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, WaterfallEffect *effect) {
    memset(effect, 0, sizeof(WaterfallEffect));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 32);
    effect->state = 0;
}

void WaterfallEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, WaterfallEffect *effect) {
    int i;
    for (i = 0; i < 2; i++) {
        FieldMoveEffectModel_Unload(&effect->models[i], &effect->allocator);
    }
}

#pragma force_active on
void WaterfallEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, WaterfallEffect *effect) {
    FileIdPair modelFileIds;
    FileIdPair animFileIds;
    u8 n;
    BOOL finished;

    switch (effect->state) {
    case 0: // one model per frame
        modelFileIds = sRideEffectFileIds.waterfallModels;
        animFileIds = sRideEffectFileIds.waterfallAnims;
        n = effect->numLoaded;
        FieldMoveEffectModel_Load(&effect->models[n], modelFileIds.ids[n], animFileIds.ids[n], 2, &effect->allocator);
        Field3dObject_SetPosAtPlayer(fieldSystem->playerAvatar, &effect->models[n].object);
        effect->numLoaded++;
        if (effect->numLoaded >= 2) {
            effect->state++;
        }
        break;
    case 1:
        Field3dObject_SetActiveFlag(&effect->models[1].object, FALSE);
        PlaySE(SEQ_SE_DP_FW463);
        effect->state++;
        // fallthrough
    case 2:
        finished = FieldMoveEffectModel_AnimateOnce(&effect->models[0]);
        Field3dObject_SetPosAtPlayer(fieldSystem->playerAvatar, &effect->models[0].object);
        if (finished == TRUE) {
            Field3dObject_SetActiveFlag(&effect->models[1].object, TRUE);
            Field3dObject_SetActiveFlag(&effect->models[0].object, FALSE);
            Field3dObject_SetPosAtPlayer(fieldSystem->playerAvatar, &effect->models[1].object);
            effect->state++;
        }
        break;
    case 3:
        FieldMoveEffectModel_AnimateLoop(&effect->models[1]);
        Field3dObject_SetPosAtPlayer(fieldSystem->playerAvatar, &effect->models[1].object);
        break;
    }
}
#pragma force_active reset

void WaterfallEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, WaterfallEffect *effect) {
    int i;
    for (i = 0; i < 2; i++) {
        FieldMoveEffectModel_Draw(&effect->models[i]);
    }
}

Field3dObjectTask *WaterfallEffect_Create(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sWaterfallEffectTemplate);
}

void WaterfallEffect_Delete(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}

#pragma force_active on
void WhirlpoolEffect_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, WhirlpoolEffect *effect) {
    struct {
        u8 animFileId;
        u8 modelFileId;
        u16 angles[4];
    } files;
    int direction;

    files.modelFileId = sRideEffectFileIds.whirlpool.ids[0];
    files.animFileId = sRideEffectFileIds.whirlpool.ids[1];
    files.angles[0] = sRideEffectFileIds.whirlpoolAngles[0];
    files.angles[1] = sRideEffectFileIds.whirlpoolAngles[1];
    files.angles[2] = sRideEffectFileIds.whirlpoolAngles[2];
    files.angles[3] = sRideEffectFileIds.whirlpoolAngles[3];
    memset(effect, 0, sizeof(WhirlpoolEffect));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 32);
    direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    FieldMoveEffectModel_Load(&effect->model, files.modelFileId, files.animFileId, 1, &effect->allocator);
    FieldMoveEffectModel_AnimateLoop(&effect->model);
    Field3dObject_SetPosAtPlayer(fieldSystem->playerAvatar, &effect->model.object);
    Field3dObject_SetXRotation(&effect->model.object, GF_DegreeToSinCosIdxNoWrap(files.angles[direction]), 1);
    PlaySE(SEQ_SE_DP_FW463);
    effect->state = 0;
}
#pragma force_active reset

void WhirlpoolEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, WhirlpoolEffect *effect) {
    FieldMoveEffectModel_Unload(&effect->model, &effect->allocator);
}

void WhirlpoolEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, WhirlpoolEffect *effect) {
    FieldMoveEffectModel_AnimateLoop(&effect->model);
    Field3dObject_SetPosAtPlayer(fieldSystem->playerAvatar, &effect->model.object);
}

void WhirlpoolEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, WhirlpoolEffect *effect) {
    FieldMoveEffectModel_Draw(&effect->model);
}

Field3dObjectTask *WhirlpoolEffect_Create(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sWhirlpoolEffectTemplate);
}

void WhirlpoolEffect_Delete(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}

void RockClimbDebris_SpawnPiece(RockDebrisPiece *piece, PlayerAvatar *playerAvatar, fx32 offsetY, fx32 offsetZ) {
    VecFx32 pos;
    int i;

    GF_ASSERT(!piece->active);
    PlayerAvatar_CopyPositionVector(playerAvatar, &pos);
    Field3dObject_SetPosEx(&piece->object, pos.x, pos.y + offsetY, pos.z + offsetZ);
    piece->active = TRUE;
    for (i = 0; i < 4; i++) {
        Field3dModelAnimation_FrameSet(&piece->animations[i], 0);
    }
    Field3dObject_SetActiveFlag(&piece->object, TRUE);
    PlaySE(SEQ_SE_DP_UG_023);
}

void RockClimbDebris_UpdatePiece(RockDebrisPiece *piece) {
    int i;
    BOOL finished;

    if (piece->active) {
        finished = TRUE;
        for (i = 0; i < 4; i++) {
            finished &= Field3dModelAnimation_FrameAdvanceAndCheck(&piece->animations[i], FX32_ONE);
        }
        if (finished == TRUE) {
            piece->active = FALSE;
            Field3dObject_SetActiveFlag(&piece->object, FALSE);
        }
    }
}

void RockClimbDebris_InitPiece(RockDebrisPiece *piece, Field3dModel *model, NNSFndAllocator *allocator, void **animData) {
    int i;

    memset(piece, 0, sizeof(RockDebrisPiece));
    Field3dObject_InitFromModel(&piece->object, model);
    for (i = 0; i < 4; i++) {
        Field3dModelAnimation_LoadFromData(&piece->animations[i], model, animData[i], allocator);
        Field3dObject_AddAnimation(&piece->object, &piece->animations[i]);
    }
    Field3dObject_SetActiveFlag(&piece->object, FALSE);
}

void RockClimbDebris_FreePiece(RockDebrisPiece *piece, NNSFndAllocator *allocator) {
    int i;

    for (i = 0; i < 4; i++) {
        Field3dModelAnimation_Unload(&piece->animations[i], allocator);
    }
    memset(piece, 0, sizeof(RockDebrisPiece));
}

// The debris appears below the player while climbing up, behind the player otherwise.
void RockClimbDebris_UpdateOffsets(RockClimbDebris *debris) {
    VecFx32 pos;

    PlayerAvatar_CopyPositionVector(debris->fieldSystem->playerAvatar, &pos);
    if (pos.x - debris->lastPlayerPos.x == 0 && pos.y < debris->lastPlayerPos.y && pos.z > debris->lastPlayerPos.z) {
        debris->offsetY = 0;
        debris->offsetZ = 2 * TILE_SIZE;
    } else {
        debris->offsetY = 2 * TILE_SIZE;
        debris->offsetZ = TILE_SIZE;
    }
    debris->lastPlayerPos = pos;
}

void RockClimbDebris_Spawn(RockClimbDebris *debris) {
    int i;

    RockClimbDebris_UpdateOffsets(debris);
    for (i = 0; i < NUM_ROCK_CLIMB_DEBRIS_PIECES; i++) {
        if (!debris->pieces[i].active) {
            RockClimbDebris_SpawnPiece(&debris->pieces[i], debris->fieldSystem->playerAvatar, debris->offsetY, debris->offsetZ);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

void RockClimbDebris_UpdatePieces(RockClimbDebris *debris) {
    s32 i;
    for (i = 0; i < NUM_ROCK_CLIMB_DEBRIS_PIECES; i++) {
        RockClimbDebris_UpdatePiece(&debris->pieces[i]);
    }
}

void RockClimbDebris_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, RockClimbDebris *debris) {
    int i;

    memset(debris, 0, sizeof(RockClimbDebris));
    HeapExp_FndInitAllocator(&debris->allocator, HEAP_ID_FIELD1, 32);
    Field3dModel_LoadFromFilesystem(&debris->model, NARC_a_1_3_4, 8, HEAP_ID_FIELD1);
    for (i = 0; i < 4; i++) {
        debris->animData[i] = GfGfxLoader_LoadFromNarc(NARC_a_1_3_4, i + 4, FALSE, HEAP_ID_FIELD1, FALSE);
    }
    for (i = 0; i < NUM_ROCK_CLIMB_DEBRIS_PIECES; i++) {
        RockClimbDebris_InitPiece(&debris->pieces[i], &debris->model, &debris->allocator, debris->animData);
    }
    debris->fieldSystem = fieldSystem;
    PlayerAvatar_CopyPositionVector(debris->fieldSystem->playerAvatar, &debris->lastPlayerPos);
    debris->paused = FALSE;
}

void RockClimbDebris_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, RockClimbDebris *debris) {
    int i;

    for (i = 0; i < NUM_ROCK_CLIMB_DEBRIS_PIECES; i++) {
        RockClimbDebris_FreePiece(&debris->pieces[i], &debris->allocator);
    }
    Field3dModel_Unload(&debris->model);
    for (i = 0; i < 4; i++) {
        Heap_Free(debris->animData[i]);
    }
}

// Spawns a debris cloud every fifth frame.
void RockClimbDebris_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, RockClimbDebris *debris) {
    if (!debris->paused) {
        debris->spawnTimer = debris->spawnTimer - 1;
        if (debris->spawnTimer < 0) {
            debris->spawnTimer = 4;
            RockClimbDebris_Spawn(debris);
        }
        RockClimbDebris_UpdatePieces(debris);
    }
}

void RockClimbDebris_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, RockClimbDebris *debris) {
    s32 i;
    for (i = 0; i < NUM_ROCK_CLIMB_DEBRIS_PIECES; i++) {
        Field3dObject_Draw(&debris->pieces[i].object);
    }
}

Field3dObjectTask *RockClimbDebris_Create(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sRockClimbDebrisTemplate);
}

void RockClimbDebris_Delete(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}

// Headbutt: the camera shakes twice towards the facing direction while the animation plays.
void HeadbuttEffect_InitAtPlayer(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect) {
    VecFx32 offset;
    fx32 *coords;

    memset(effect, 0, sizeof(HeadbuttEffect));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 32);
    FieldMoveEffectModel_Load(&effect->model, 19, 17, 2, &effect->allocator);
    Field3dObject_SetPosInFrontOfPlayer(fieldSystem->playerAvatar, &effect->model.object);
    effect->cameraTarget = Camera_GetLookAtCamTarget(fieldSystem->camera);
    effect->cameraPos = Camera_GetLookAtCamPos(fieldSystem->camera);
    coords = (fx32 *)&offset;
    coords[0] = 0;
    coords[1] = 0;
    coords[2] = 0;
    switch (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar)) {
    case DIR_NORTH:
        offset.z -= TILE_SIZE / 2;
        break;
    case DIR_SOUTH:
        offset.z += TILE_SIZE / 2;
        break;
    case DIR_WEST:
        offset.x -= TILE_SIZE / 2;
        break;
    case DIR_EAST:
        offset.x += TILE_SIZE / 2;
        break;
    }
    effect->shakeOffset = offset;
    PlaySE(SEQ_SE_GS_ZUTUKI);
    effect->state = 0;
}

void HeadbuttEffect_InitAtFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect) {
    VecFx32 offset;
    fx32 *coords;

    memset(effect, 0, sizeof(HeadbuttEffect));
    HeapExp_FndInitAllocator(&effect->allocator, HEAP_ID_FIELD1, 32);
    FieldMoveEffectModel_Load(&effect->model, 19, 17, 2, &effect->allocator);
    Field3dObject_SetPosInFrontOfMapObject(fieldSystem->mapObjectManager, obj_partner_poke, &effect->model.object);
    effect->cameraTarget = Camera_GetLookAtCamTarget(fieldSystem->camera);
    effect->cameraPos = Camera_GetLookAtCamPos(fieldSystem->camera);
    coords = (fx32 *)&offset;
    coords[0] = 0;
    coords[1] = 0;
    coords[2] = 0;
    switch (PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar)) {
    case DIR_NORTH:
        offset.z -= TILE_SIZE / 2;
        break;
    case DIR_SOUTH:
        offset.z += TILE_SIZE / 2;
        break;
    case DIR_WEST:
        offset.x -= TILE_SIZE / 2;
        break;
    case DIR_EAST:
        offset.x += TILE_SIZE / 2;
        break;
    }
    effect->shakeOffset = offset;
    PlaySE(SEQ_SE_GS_ZUTUKI);
    effect->state = 0;
}

void HeadbuttEffect_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect) {
    FieldMoveEffectModel_Unload(&effect->model, &effect->allocator);
}

#pragma force_active on
void HeadbuttEffect_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect) {
    switch (effect->state) {
    case 0:
        if (effect->numShakes < 2) {
            if (effect->shakeTimer <= 0) {
                effect->shakeTimer = 1;
                effect->shaken = (effect->shaken + 1) % 2;
                if (effect->shaken) {
                    Camera_OffsetLookAtPosAndTarget(&effect->shakeOffset, fieldSystem->camera);
                } else {
                    Camera_SetLookAtCamTarget(&effect->cameraTarget, fieldSystem->camera);
                    Camera_SetLookAtCamPos(&effect->cameraPos, fieldSystem->camera);
                    effect->numShakes++;
                }
            } else {
                effect->shakeTimer--;
            }
        }
        if (FieldMoveEffectModel_AnimateOnce(&effect->model) == TRUE) {
            Field3dObject_SetActiveFlag(&effect->model.object, FALSE);
            effect->state++;
        }
        break;
    case 1:
        Camera_SetLookAtCamTarget(&effect->cameraTarget, fieldSystem->camera);
        Camera_SetLookAtCamPos(&effect->cameraPos, fieldSystem->camera);
        break;
    }
}
#pragma force_active reset

void HeadbuttEffect_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, HeadbuttEffect *effect) {
    FieldMoveEffectModel_Draw(&effect->model);
}

Field3dObjectTask *HeadbuttEffect_CreateAtPlayer(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sHeadbuttEffectAtPlayerTemplate);
}

Field3dObjectTask *HeadbuttEffect_CreateAtFollower(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sHeadbuttEffectAtFollowerTemplate);
}

void HeadbuttEffect_Delete(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}

BOOL HeadbuttEffect_IsFinished(Field3dObjectTask *task) {
    HeadbuttEffect *effect = Field3dObjectTask_GetData(task);
    return effect->state == 1;
}

void RockDebris_Init(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris) {
    int i;
    int j;

    memset(debris, 0, sizeof(RockDebris));
    HeapExp_FndInitAllocator(&debris->allocator, HEAP_ID_FIELD1, 32);
    Field3dModel_LoadFromFilesystem(&debris->model, NARC_a_1_3_4, 8, HEAP_ID_FIELD1);
    for (i = 0; i < 4; i++) {
        debris->animData[i] = GfGfxLoader_LoadFromNarc(NARC_a_1_3_4, i + 4, FALSE, HEAP_ID_FIELD1, FALSE);
    }
    for (j = 0; j < NUM_ROCK_DEBRIS_PIECES; j++) {
        RockDebris_InitPiece(&debris->pieces[j], &debris->model, &debris->allocator, debris->animData);
    }
    debris->fieldSystem = fieldSystem;
    debris->paused = FALSE;
}

// Also spawns debris at the following Pokemon.
void RockDebris_InitWithFollower(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris) {
    RockDebris_Init(task, fieldSystem, debris);
    debris->withFollower = TRUE;
}

void RockDebris_Destroy(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris) {
    int i;

    for (i = 0; i < NUM_ROCK_DEBRIS_PIECES; i++) {
        RockDebris_FreePiece(&debris->pieces[i], &debris->allocator);
    }
    Field3dModel_Unload(&debris->model);
    for (i = 0; i < 4; i++) {
        Heap_Free(debris->animData[i]);
    }
}

// Spawns a debris cloud every fifth frame.
void RockDebris_Update(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris) {
    if (!debris->paused) {
        debris->spawnTimer = debris->spawnTimer - 1;
        if (debris->spawnTimer < 0) {
            debris->spawnTimer = 4;
            RockDebris_Spawn(debris);
        }
        RockDebris_UpdatePieces(debris);
    }
}

void RockDebris_Render(Field3dObjectTask *task, FieldSystem *fieldSystem, RockDebris *debris) {
    int i;
    for (i = 0; i < NUM_ROCK_DEBRIS_PIECES; i++) {
        Field3dObject_Draw(&debris->pieces[i].object);
    }
}

Field3dObjectTask *RockDebris_Create(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sRockDebrisTemplate);
}

Field3dObjectTask *RockDebris_CreateWithFollower(FieldSystem *fieldSystem) {
    return Field3dObjectTaskManager_CreateTask(fieldSystem->unk4->field3dObjectTaskManager, &sRockDebrisWithFollowerTemplate);
}

void RockDebris_Delete(Field3dObjectTask *task) {
    Field3dObjectTask_Delete(task);
}

void RockDebris_SpawnPiece(RockDebrisPiece *piece, const VecFx32 *position, fx32 offsetY, fx32 offsetZ) {
    int i;

    GF_ASSERT(!piece->active);
    Field3dObject_SetPosEx(&piece->object, position->x, position->y + offsetY, position->z + offsetZ);
    piece->active = TRUE;
    for (i = 0; i < 4; i++) {
        Field3dModelAnimation_FrameSet(&piece->animations[i], 0);
    }
    Field3dObject_SetActiveFlag(&piece->object, TRUE);
    PlaySE(SEQ_SE_DP_UG_023);
}

void RockDebris_UpdatePiece(RockDebrisPiece *piece) {
    int i;
    BOOL finished;

    if (piece->active) {
        finished = TRUE;
        for (i = 0; i < 4; i++) {
            finished &= Field3dModelAnimation_FrameAdvanceAndCheck(&piece->animations[i], FX32_ONE);
        }
        if (finished == TRUE) {
            piece->active = FALSE;
            Field3dObject_SetActiveFlag(&piece->object, FALSE);
        }
    }
}

void RockDebris_InitPiece(RockDebrisPiece *piece, Field3dModel *model, NNSFndAllocator *allocator, void **animData) {
    int i;

    memset(piece, 0, sizeof(RockDebrisPiece));
    Field3dObject_InitFromModel(&piece->object, model);
    for (i = 0; i < 4; i++) {
        Field3dModelAnimation_LoadFromData(&piece->animations[i], model, animData[i], allocator);
        Field3dObject_AddAnimation(&piece->object, &piece->animations[i]);
    }
    Field3dObject_SetActiveFlag(&piece->object, FALSE);
}

void RockDebris_FreePiece(RockDebrisPiece *piece, NNSFndAllocator *allocator) {
    int i;

    for (i = 0; i < 4; i++) {
        Field3dModelAnimation_Unload(&piece->animations[i], allocator);
    }
    memset(piece, 0, sizeof(RockDebrisPiece));
}

void RockDebris_Spawn(RockDebris *debris) {
    VecFx32 playerPos;
    VecFx32 followerPos;
    int i;

    debris->offsetY = FX32_ONE;
    debris->offsetZ = 3 * FX32_ONE;
    PlayerAvatar_CopyPositionVector(debris->fieldSystem->playerAvatar, &playerPos);
    if (debris->withFollower) {
        MapObject_CopyPositionVector(FollowMon_GetMapObject(debris->fieldSystem), &followerPos);
        for (i = 0; i < NUM_ROCK_DEBRIS_PIECES; i++) {
            if (!debris->pieces[i].active) {
                RockDebris_SpawnPiece(&debris->pieces[i], &playerPos, debris->offsetY, debris->offsetZ);
                break;
            }
        }
        if (i == NUM_ROCK_DEBRIS_PIECES) {
            GF_ASSERT(FALSE);
        }
        for (i = 0; i < NUM_ROCK_DEBRIS_PIECES; i++) {
            if (!debris->pieces[i].active) {
                RockDebris_SpawnPiece(&debris->pieces[i], &followerPos, debris->offsetY, debris->offsetZ);
                return;
            }
        }
    } else {
        for (i = 0; i < NUM_ROCK_DEBRIS_PIECES; i++) {
            if (!debris->pieces[i].active) {
                RockDebris_SpawnPiece(&debris->pieces[i], &playerPos, debris->offsetY, debris->offsetZ);
                return;
            }
        }
    }
    GF_ASSERT(FALSE);
}

void RockDebris_UpdatePieces(RockDebris *debris) {
    int i;
    for (i = 0; i < NUM_ROCK_DEBRIS_PIECES; i++) {
        RockDebris_UpdatePiece(&debris->pieces[i]);
    }
}

// Script field move effects, by kind: 0 Cut at the player, 1 Rock Smash at the player, 2 Rock Smash at the
// following Pokemon, 3 Cut at the following Pokemon, 4 Headbutt at the player, 5 Headbutt at the following Pokemon.
void Task_ScriptFieldMoveEffect(SysTask *task, void *data) {
    ScriptFieldMoveEffectTask *env = data;

    switch (env->state) {
    case 0:
        if (sScriptFieldMoveEffectIsFinishedFuncs[env->kind](env->effect) == TRUE) {
            env->state++;
        }
        break;
    case 1:
        sScriptFieldMoveEffectDeleteFuncs[env->kind](env->effect);
        *env->done = TRUE;
        Heap_Free(env);
        SysTask_Destroy(task);
        break;
    }
}

void ScriptFieldMoveEffect_Start(FieldSystem *fieldSystem, u16 *done, int kind, enum HeapID heapID) {
    ScriptFieldMoveEffectTask *env = Heap_Alloc(heapID, sizeof(ScriptFieldMoveEffectTask));
    memset(env, 0, sizeof(ScriptFieldMoveEffectTask));
    env->effect = sScriptFieldMoveEffectCreateFuncs[kind](fieldSystem);
    env->done = done;
    env->kind = kind;
    *done = FALSE;
    SysTask_CreateOnMainQueue(Task_ScriptFieldMoveEffect, env, 0);
}
