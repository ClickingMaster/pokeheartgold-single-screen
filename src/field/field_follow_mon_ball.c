#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"
#include "field/field_follow_mon.h"
#include "field/overlay_01_021FB878.h"

#include "field_effects.h"
#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "vram_transfer_manager.h"

// The following Pokemon's Poke Ball (field effect renderer 17): a ball model and a burst model with one
// animation, both kept loaded. FollowMonBall_Spawn kinds:
//   0: the ball bursts open and the Pokemon appears (it is made visible when the burst starts);
//   1: the Pokemon's billboard turns white and shrinks into the ball (a large Pokemon is also pulled towards
//      the tile it faces), the ball stays shown;
//   2: the burst alone;
//   3: as 1 without the white palette; the Pokemon is hidden once it has shrunk.

#define FOLLOW_MON_BALL_SHRINK_FRAMES 4

// Puts position->y on the ground of the tile at position->x/z; FALSE while that tile has no ground yet.
// (unk_0205FD20.h declares it returning void.)
BOOL FieldSystem_SnapToTerrainHeight(FieldSystem *fieldSystem, VecFx32 *position);
// The following Pokemon's billboard (field map object motion, still unnamed).
void *MapObjectManager_GetFollowMonBillboard(MapObjectManager *manager);
// billboard_lists.c
void BillboardObject_SetScale(void *billboard, VecFx32 *scale);
NNSG3dPlttKey BillboardObject_GetPlttKey(void *billboard);

// Renderer data, from FieldEffectManager_GetRendererData(manager, 17).
typedef struct FollowMonBall {
    FieldEffectManager *manager;
    NNSFndAllocator allocator;
    void *ballFile;
    void *burstFile;
    Field3dModel ballModel;
    Field3dModel burstModel;
    Field3dObject ball;
    Field3dObject burst;
    Field3DModelAnimation burstAnimation;
} FollowMonBall;

typedef struct FollowMonBallEffectArgs {
    VecFx32 position;
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    FollowMonBall *ball;
    LocalMapObject *mapObject;
    u16 unk1C;
    u16 unk1E;
    s8 unk20;
} FollowMonBallEffectArgs;

typedef struct FollowMonBallEffect {
    int state;
    u32 spriteId;
    u32 objectId;
    u32 mapId;
    int timer;
    BOOL onGround;
    FollowMonBallEffectArgs args;
    fx32 *coordinate; // the follower's x or z being pulled, NULL if it is not a large one
    fx32 originalCoordinate;
    int coordinateStep;
    VecFx32 position;
} FollowMonBallEffect;

FollowMonBall *FollowMonBall_New(FieldEffectManager *manager);
void FollowMonBall_Free(FollowMonBall *ball);
void FollowMonBall_LoadModels(FollowMonBall *ball);
void FollowMonBall_FreeModels(FollowMonBall *ball);
FieldEffect *FollowMonBall_Spawn(LocalMapObject *mapObject, int kind);
BOOL FollowMonBallEffect_InitBurst(FieldEffect *effect, FollowMonBallEffect *work);
void FollowMonBallEffect_Destroy(void);
void FollowMonBallEffect_UpdateRelease(FieldEffect *effect, FollowMonBallEffect *work);
void FollowMonBallEffect_DrawRelease(FieldEffect *effect, FollowMonBallEffect *work);
void FollowMonBallEffect_UpdateBurst(FieldEffect *effect, FollowMonBallEffect *work);
void FollowMonBallEffect_DrawBurst(FieldEffect *effect, FollowMonBallEffect *work);
BOOL FollowMonBallEffect_InitRecallWhite(FieldEffect *effect, FollowMonBallEffect *work);
void FollowMonBallEffect_UpdateRecallWhite(FieldEffect *effect, FollowMonBallEffect *work);
void FollowMonBallEffect_DrawBall(FieldEffect *effect, FollowMonBallEffect *work);
BOOL FollowMonBallEffect_InitRecall(FieldEffect *effect, FollowMonBallEffect *work);
void FollowMonBallEffect_UpdateRecall(FieldEffect *effect, FollowMonBallEffect *work);
void FollowMonBall_GetPullCoordinate(u32 direction, VecFx32 *position, fx32 **coordinate);
int FollowMonBall_GetPullStep(u32 direction);

// Data stays in asm (asm/field_follow_mon_ball_data.s, linked after this file): two unit scales, the four
// effect templates (kinds 3, 0, 1, 2) and the white palette (.data).
extern const VecFx32 sFollowMonBallRecallScale;
extern const VecFx32 sFollowMonBallRecallWhiteScale;
extern const FieldEffectBasicTemplate ov01_022094C8;
extern const FieldEffectBasicTemplate ov01_022094DC;
extern const FieldEffectBasicTemplate ov01_022094F0;
extern const FieldEffectBasicTemplate ov01_02209504;
extern u8 sFollowMonWhitePalette[];

FollowMonBall *FollowMonBall_New(FieldEffectManager *manager) {
    FollowMonBall *ball = FieldEffectManager_HeapAllocAndFill(manager, sizeof(FollowMonBall), FALSE, 0);
    ball->manager = manager;
    HeapExp_FndInitAllocator(&ball->allocator, HEAP_ID_FIELD1, 32);
    FollowMonBall_LoadModels(ball);
    return ball;
}

void FollowMonBall_Free(FollowMonBall *ball) {
    FollowMonBall_FreeModels(ball);
    FieldEffectManager_HeapFree(ball);
}

void FollowMonBall_LoadModels(FollowMonBall *ball) {
    ball->ballFile = FieldEffectManager_LoadNarcMember(ball->manager, 0x81, FALSE);
    Field3dModel_LoadFromData(&ball->ballModel, ball->ballFile);
    Field3dObject_InitFromModel(&ball->ball, &ball->ballModel);
    ball->burstFile = FieldEffectManager_LoadNarcMember(ball->manager, 0x68, FALSE);
    Field3dModel_LoadFromData(&ball->burstModel, ball->burstFile);
    Field3dObject_InitFromModel(&ball->burst, &ball->burstModel);
    Field3dModelAnimation_LoadFromFilesystem(&ball->burstAnimation, &ball->burstModel, NARC_a_1_0_3, 0xA4, HEAP_ID_FIELD1, &ball->allocator);
    Field3dObject_AddAnimation(&ball->burst, &ball->burstAnimation);
}

void FollowMonBall_FreeModels(FollowMonBall *ball) {
    Field3dModel_UnloadKeepData(&ball->ballModel);
    FieldEffectManager_HeapFree(ball->ballFile);
    Field3dModel_UnloadKeepData(&ball->burstModel);
    FieldEffectManager_HeapFree(ball->burstFile);
    Field3dModelAnimation_Unload(&ball->burstAnimation, &ball->allocator);
}

FieldEffect *FollowMonBall_Spawn(LocalMapObject *mapObject, int kind) {
    VecFx32 position;
    FollowMonBallEffectArgs args;
    u32 priority;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);

    MapObject_CopyPositionVector(mapObject, &args.position);
    args.position.z += 6 * FX32_ONE;
    args.unk1C = 0;
    args.unk1E = 0;
    args.unk20 = -1;
    args.manager = manager;
    args.fieldSystem = FieldEffectManager_GetFieldSystem(manager);
    args.ball = FieldEffectManager_GetRendererData(manager, 17);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    priority = MapObject_GetPriorityPlusValue(mapObject, 2);
    if (kind == 0) {
        return FieldEffectManager_CreateEffect(manager, &ov01_022094DC, &position, 1, &args, priority);
    }
    if (kind == 1) {
        return FieldEffectManager_CreateEffect(manager, &ov01_022094F0, &position, 1, &args, priority);
    }
    if (kind == 2) {
        return FieldEffectManager_CreateEffect(manager, &ov01_02209504, &position, 1, &args, priority);
    }
    return FieldEffectManager_CreateEffect(manager, &ov01_022094C8, &position, 1, &args, priority);
}

BOOL FollowMonBallEffect_InitBurst(FieldEffect *effect, FollowMonBallEffect *work) {
    VecFx32 position;
    FollowMonBallEffectArgs *args = (FollowMonBallEffectArgs *)FieldEffect_GetUserData(effect);
    work->state = 0;
    work->args = *args;
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    position = work->args.position;
    work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &position);
    FieldEffect_SetPosition(effect, &position);
    Field3dObject_SetPos(&work->args.ball->ball, &position);
    Field3dObject_SetPos(&work->args.ball->burst, &position);
    Field3dObject_SetActiveFlag(&work->args.ball->burst, FALSE);
    return TRUE;
}

void FollowMonBallEffect_Destroy(void) {
}

void FollowMonBallEffect_UpdateRelease(FieldEffect *effect, FollowMonBallEffect *work) {
    LocalMapObject *mapObject = work->args.mapObject;
    VecFx32 position;
    VecFx32 ground;
    if (!MapObject_CheckSpriteIDAndMap(mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    if (work->onGround == FALSE) {
        FieldEffect_GetPosition(effect, &position);
        ground = work->args.position;
        work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &ground);
        if (work->onGround == TRUE) {
            position.y = ground.y;
            FieldEffect_SetPosition(effect, &position);
        }
    }
    switch (work->state) {
    case 0:
        if (++work->timer >= 2) {
            work->state = 1;
        }
        break;
    case 1:
        Field3dObject_SetActiveFlag(&work->args.ball->ball, FALSE);
        Field3dObject_SetActiveFlag(&work->args.ball->burst, TRUE);
        FollowMon_HideUntilStep(mapObject, FALSE);
        MapObject_CallDrawUpdate(mapObject);
        Field3dModelAnimation_FrameSet(&work->args.ball->burstAnimation, 0);
        work->state = 2;
    case 2:
        if (Field3dModelAnimation_FrameAdvanceAndCheck(&work->args.ball->burstAnimation, FX32_ONE)) {
            FieldEffectManager_DestroyEffect((int)effect);
        }
        break;
    }
}

void FollowMonBallEffect_DrawRelease(FieldEffect *effect, FollowMonBallEffect *work) {
    VecFx32 position;
    if (!MapObject_CheckSpriteIDAndMap(work->args.mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    FieldEffect_GetPosition(effect, &position);
    Field3dObject_Draw(&work->args.ball->ball);
    Field3dObject_Draw(&work->args.ball->burst);
}

void FollowMonBallEffect_UpdateBurst(FieldEffect *effect, FollowMonBallEffect *work) {
    VecFx32 position;
    VecFx32 ground;
    if (!MapObject_CheckSpriteIDAndMap(work->args.mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    if (work->onGround == FALSE) {
        FieldEffect_GetPosition(effect, &position);
        ground = work->args.position;
        work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &ground);
        if (work->onGround == TRUE) {
            position.y = ground.y;
            FieldEffect_SetPosition(effect, &position);
        }
    }
    switch (work->state) {
    case 0:
        Field3dObject_SetActiveFlag(&work->args.ball->ball, FALSE);
        Field3dObject_SetActiveFlag(&work->args.ball->burst, TRUE);
        Field3dModelAnimation_FrameSet(&work->args.ball->burstAnimation, 0);
        work->state = 1;
    case 1:
        if (Field3dModelAnimation_FrameAdvanceAndCheck(&work->args.ball->burstAnimation, FX32_ONE)) {
            FieldEffectManager_DestroyEffect((int)effect);
        }
        break;
    }
}

void FollowMonBallEffect_DrawBurst(FieldEffect *effect, FollowMonBallEffect *work) {
    VecFx32 position;
    if (!MapObject_CheckSpriteIDAndMap(work->args.mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    FieldEffect_GetPosition(effect, &position);
    Field3dObject_Draw(&work->args.ball->burst);
}

BOOL FollowMonBallEffect_InitRecallWhite(FieldEffect *effect, FollowMonBallEffect *work) {
    VecFx32 position;
    FollowMonBallEffectArgs *args = (FollowMonBallEffectArgs *)FieldEffect_GetUserData(effect);
    work->state = 0;
    work->args = *args;
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    position = work->args.position;
    work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &position);
    FieldEffect_SetPosition(effect, &position);
    Field3dObject_SetPos(&work->args.ball->ball, &position);
    Field3dObject_SetActiveFlag(&work->args.ball->ball, FALSE);
    return TRUE;
}

void FollowMonBallEffect_UpdateRecallWhite(FieldEffect *effect, FollowMonBallEffect *work) {
    LocalMapObject *owner = work->args.mapObject;
    VecFx32 position;
    VecFx32 ground;
    VecFx32 scale;
    LocalMapObject *mapObject;
    void *billboard;
    u32 plttKey;
    u32 plttKey2;
    u32 direction;

    if (!MapObject_CheckSpriteIDAndMap(owner, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    if (work->onGround == FALSE) {
        FieldEffect_GetPosition(effect, &position);
        ground = work->args.position;
        work->onGround = FieldSystem_SnapToTerrainHeight(work->args.fieldSystem, &ground);
        if (work->onGround == TRUE) {
            position.y = ground.y;
            FieldEffect_SetPosition(effect, &position);
        }
    }
    switch (work->state) {
    case 0:
        mapObject = FollowMon_GetMapObject(work->args.fieldSystem);
        billboard = MapObjectManager_GetFollowMonBillboard(work->args.fieldSystem->mapObjectManager);
        plttKey = BillboardObject_GetPlttKey(billboard);
        plttKey2 = BillboardObject_GetPlttKey(billboard);
        GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (u16)plttKey2 << 3, sFollowMonWhitePalette, ((plttKey & 0xFFFF0000) >> 16) << 3);
        if (FollowMonObject_IsLarge(mapObject)) {
            direction = (u8)MapObject_GetFacingDirection(mapObject);
            MapObject_CopyPositionVector(mapObject, &work->position);
            FollowMonBall_GetPullCoordinate(direction, &work->position, &work->coordinate);
            work->originalCoordinate = *work->coordinate;
            work->coordinateStep = FollowMonBall_GetPullStep(direction);
        } else {
            work->coordinate = NULL;
        }
        ++work->state;
    case 1:
        scale = sFollowMonBallRecallWhiteScale;
        ++work->timer;
        scale.x = FX32_ONE / work->timer;
        scale.y = FX32_ONE / work->timer;
        BillboardObject_SetScale(MapObjectManager_GetFollowMonBillboard(work->args.fieldSystem->mapObjectManager), &scale);
        if (work->coordinate) {
            mapObject = FollowMon_GetMapObject(work->args.fieldSystem);
            {
                s32 timer = work->timer;
                s32 step = work->coordinateStep;
                *work->coordinate = ((timer * (step << FX32_SHIFT)) / 4) + work->originalCoordinate;
            }
            MapObject_SetPositionVector(mapObject, &work->position);
        }
        if (work->timer >= FOLLOW_MON_BALL_SHRINK_FRAMES) {
            ++work->state;
            Field3dObject_SetActiveFlag(&work->args.ball->ball, TRUE);
            MapObject_SetVisible(owner, TRUE);
            work->timer = 0;
            return;
        }
        break;
    case 2:
        if (++work->timer >= FOLLOW_MON_BALL_SHRINK_FRAMES) {
            FieldEffectManager_DestroyEffect((int)effect);
        }
        break;
    }
}

void FollowMonBallEffect_DrawBall(FieldEffect *effect, FollowMonBallEffect *work) {
    VecFx32 position;
    if (!MapObject_CheckSpriteIDAndMap(work->args.mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    FieldEffect_GetPosition(effect, &position);
    Field3dObject_Draw(&work->args.ball->ball);
}

BOOL FollowMonBallEffect_InitRecall(FieldEffect *effect, FollowMonBallEffect *work) {
    VecFx32 position;
    FollowMonBallEffectArgs *args = (FollowMonBallEffectArgs *)FieldEffect_GetUserData(effect);
    work->state = 0;
    work->args = *args;
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    position = work->args.position;
    FieldEffect_SetPosition(effect, &position);
    Field3dObject_SetPos(&work->args.ball->ball, &position);
    Field3dObject_SetActiveFlag(&work->args.ball->ball, FALSE);
    return TRUE;
}

#pragma force_active on

void FollowMonBallEffect_UpdateRecall(FieldEffect *effect, FollowMonBallEffect *work) {
    LocalMapObject *mapObject = work->args.mapObject;
    if (!MapObject_CheckSpriteIDAndMap(mapObject, work->spriteId, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    switch (work->state) {
    case 0: {
        LocalMapObject *followMon = FollowMon_GetMapObject(work->args.fieldSystem);
        u8 direction;
        MapObjectManager_GetFollowMonBillboard(work->args.fieldSystem->mapObjectManager);
        if (FollowMonObject_IsLarge(followMon)) {
            direction = MapObject_GetFacingDirection(followMon);
            MapObject_CopyPositionVector(followMon, &work->position);
            FollowMonBall_GetPullCoordinate(direction, &work->position, &work->coordinate);
            work->originalCoordinate = *work->coordinate;
            work->coordinateStep = FollowMonBall_GetPullStep(direction);
        } else {
            work->coordinate = NULL;
        }
        work->state++;
    }
    case 1: {
        VecFx32 scale = sFollowMonBallRecallScale;
        work->timer++;
        scale.x = FX32_ONE / work->timer;
        scale.y = FX32_ONE / work->timer;
        BillboardObject_SetScale(MapObjectManager_GetFollowMonBillboard(work->args.fieldSystem->mapObjectManager), &scale);
        if (work->coordinate) {
            LocalMapObject *followMon = FollowMon_GetMapObject(work->args.fieldSystem);
            s32 delta = work->timer * (s32)((u32)work->coordinateStep << FX32_SHIFT);
            *work->coordinate = work->originalCoordinate + delta / 4;
            MapObject_SetPositionVector(followMon, &work->position);
        }
        if (work->timer >= FOLLOW_MON_BALL_SHRINK_FRAMES) {
            work->state++;
            Field3dObject_SetActiveFlag(&work->args.ball->ball, TRUE);
            FollowMon_HideUntilStep(mapObject, TRUE);
            work->timer = 0;
        }
    } break;
    case 2:
        work->timer++;
        if (work->timer >= 16) {
            FieldEffectManager_DestroyEffect((int)effect);
        }
        break;
    }
}

#pragma force_active reset

// The coordinate of position a large following Pokemon is pulled along: z when facing north or south, x
// when facing west or east.
void FollowMonBall_GetPullCoordinate(u32 direction, VecFx32 *position, fx32 **coordinate) {
    switch (direction) {
    case 0:
    case 1:
        *coordinate = &position->z;
        break;
    case 2:
    case 3:
        *coordinate = &position->x;
        break;
    }
}

int FollowMonBall_GetPullStep(u32 direction) {
    switch (direction) {
    case 0:
        return -1;
    case 1:
        return 1;
    case 2:
        return -10;
    case 3:
        return 10;
    default:
        GF_AssertFail();
        return 0;
    }
}
