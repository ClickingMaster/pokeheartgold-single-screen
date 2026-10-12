#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/scrcmd.h"
#include "constants/sprites.h"

#include "field/field_effect_helpers.h"
#include "field/field_follow_mon.h"
#include "field/resource_bank.h"

#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_022031C0.h"
#include "overlay_25.h"
#include "palette.h"
#include "party.h"
#include "player_avatar.h"
#include "poke_overlay.h"
#include "scrcmd.h"
#include "script.h"
#include "script_manager.h"
#include "script_pokemon_util.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_02054648.h"
#include "map_object_movement_commands.h"
#include "vram_transfer_manager.h"

// Overlay 01 helpers for the following Pokemon (map object obj_partner_poke, and the Day-Care Pokemon that use
// the same billboards): its map object params, keeping it next to the player, and the scripted tasks that put
// it into its Poke Ball (FollowMonBall_Spawn) or flash its palette. Also script command 808.

FS_EXTERN_OVERLAY(OVY_25);

// MapObject_GetParam(object, 1) of a following Pokemon: three nibbles of its tp_param entry.
#define FOLLOW_MON_PARAM_SHAPE 1
// MapObject_GetParam(object, 2) of a following Pokemon, bit 0.
#define FOLLOW_MON_PARAM_FLAGS 2

#define BILLBOARD_SEQUENCE_FOLLOW_MON     21
#define BILLBOARD_SEQUENCE_FOLLOW_MON_ALT 22

// map_object_follow_2.c / r40_unk_020659A8_run1.c / map_object_follow_3.c (no header).
u8 FollowPlayerMovement_GetHopFrame(LocalMapObject *object);
u32 FollowPlayerMovement_GetHopSpeed(LocalMapObject *object);
void sub_020659B8(LocalMapObject *object);
// unk_02062108.c: the movement for a direction (movement + direction).
u32 GetMovementForDirection(u32 direction, u32 movement);
// field object models (still in asm or crammed).
BOOL ov01_021F9744(MapObjectManager *manager, u32 spriteId, void *record);
u32 ov01_021FA44C(u32 direction);
void ov01_021F9048(LocalMapObject *object);
// The following Pokemon's billboard (field map object motion, still unnamed).
void *ov01_021F771C(MapObjectManager *manager);
int GetMoveModelNoBySpriteId(u32 spriteId);
void BillboardGfxSequence_Init(void *sequence, void *dest);
// billboard_lists.c
void BillboardObject_SetResource(void *billboard, void *record);
NNSG3dPlttKey BillboardObject_GetPlttKey(void *billboard);
NNSG3dTexKey BillboardObject_GetTexKey(void *billboard);
// texture helpers (render_animation / field object models)
void *Texture_GetPaletteData(NNSG3dResTex *texture, u32 index);
void *Texture_GetImageData(NNSG3dResTex *texture, u32 index);

// The render data of the map object manager (sub_0205F1A0); only the billboard sequence bank is used here.
typedef struct MapObjectRenderData {
    u8 unk00[0xFC];
    ResourceBank *sequenceBank;
} MapObjectRenderData;

// A billboard sequence from the sequence bank: count frames, then count u8s, then count u8 flags.
typedef struct BillboardSequenceData {
    int count;
    u16 frames[1];
} BillboardSequenceData;

typedef struct MovementTable {
    u32 movements[4];
} MovementTable;

typedef struct HopHeightTable {
    fx32 heights[3][16];
} HopHeightTable;

typedef struct HopOffsets {
    u8 offsets[8];
} HopOffsets;

typedef struct HopMovements {
    u32 movements[2];
} HopMovements;

typedef struct HopTable {
    HopOffsets yOffsets;
    HopMovements movements;
} HopTable;

typedef struct SepiaBillboardTransfer {
    void *file;
    void *texture;
    u16 palette[16];
} SepiaBillboardTransfer;

typedef struct UnkStruct_ov01_02205B14 {
    u8 state;
    u8 step;
    u8 timer;
    u8 movement;
    u8 unk04[64];
    BOOL toLeft;
} UnkStruct_ov01_02205B14;

typedef struct FollowMonRecall {
    int timer;
    u16 palette[32];
} FollowMonRecall;

typedef struct FollowMonFlash {
    s8 blend;
    u8 step;
    u8 delay;
    u8 period;
    u16 original[32];
    u16 blended[32];
} FollowMonFlash;

extern const MovementTable ov01_02209710;
extern const MovementTable ov01_02209720;
extern const MovementTable ov01_02209730;
extern const MovementTable ov01_02209740;
extern const HopHeightTable ov01_02209750;
extern const HopOffsets ov01_022096F0;
extern const VecFx32 ov01_022096F8;
extern const VecFx32 ov01_02209704;

const HopTable ov01_022096E0 = { { 1, 2, 2, 3, 3, 2, 2, 0 }, { 14, 12 } };

void ov01_0220596C(SysTask *task, void *data);
BOOL ov01_02205B14(TaskManager *taskMan);
u32 ov01_02205CF0(FieldSystem *fieldSystem, UnkStruct_ov01_02205B14 *env);
BOOL Task_FollowMonRecall(TaskManager *taskMan);
BOOL ov01_02205F00(TaskManager *taskMan);
BOOL Task_FollowMonFlash(TaskManager *taskMan);

BOOL ScrCmd_808(ScriptContext *ctx) {
    u16 trainerNum = FieldSystem_VarGet(ctx->fieldSystem, ScriptReadHalfword(ctx));
    FSOverlayID overlay = FS_OVERLAY_ID(OVY_25);
    HandleLoadOverlay(overlay, OVY_LOAD_ASYNC);
    TrainerHouse_StartBattle(ctx->fieldSystem, trainerNum);
    UnloadOverlayByID(overlay);
    return TRUE;
}

// On leaving a map: FollowMon_RecreateForMap re-creates or drops the following Pokemon on the next one.
void FollowMon_MarkForRecreate(FieldSystem *fieldSystem) {
    fieldSystem->followMon.unk15 = TRUE;
}

// The next faster movement in direction after movement (8-11, then steps 12-15, runs 16-19, 20-23).
u32 FollowMon_GetFasterMovement(u32 direction, u32 movement) {
    MovementTable slow = ov01_02209720;
    MovementTable step = ov01_02209730;
    MovementTable run = ov01_02209740;
    MovementTable fast = ov01_02209710;
    u8 i;
    for (i = 0; i < 4; i++) {
        if (movement == slow.movements[i]) {
            return step.movements[direction];
        }
    }
    for (i = 0; i < 4; i++) {
        if (movement == step.movements[i]) {
            return run.movements[direction];
        }
    }
    for (i = 0; i < 4; i++) {
        if (movement == run.movements[i]) {
            return fast.movements[direction];
        }
    }
    GF_ASSERT(FALSE);
    return 0;
}

fx32 ov01_022054E0(LocalMapObject *object) {
    HopHeightTable table = ov01_02209750;
    LocalMapObject *mapObject = object;
    u8 row = ov01_02205584(mapObject);
    s32 index;
    u8 column;
    if (row != 0) {
        return 0;
    }
    column = FollowPlayerMovement_GetHopSpeed(mapObject);
    if (column == 0) {
        return 0;
    }
    index = FollowPlayerMovement_GetHopFrame(mapObject);
    if (index >= 16) {
        GF_ASSERT(FALSE);
    }
    column--;
    return table.heights[column][index];
}

BOOL MapObject_IsShinyPokemon(LocalMapObject *object) {
    u32 id = MapObject_GetID(object);
    if (id != obj_partner_poke && id != obj_daycare_poke_1 && id != obj_daycare_poke_2) {
        return FALSE;
    }
    return MapObject_GetParam(object, FOLLOW_MON_PARAM_FLAGS) & 1;
}

BOOL MapObject_HasFollowMonSprite(LocalMapObject *object) {
    int spriteId = MapObject_GetSpriteID(object);
    return spriteId >= SPRITE_FOLLOWER_MON && spriteId <= SPRITE_FOLLOWER_MON_6;
}

u8 ov01_02205584(LocalMapObject *object) {
    u16 shape = MapObject_GetParam(object, FOLLOW_MON_PARAM_SHAPE);
    if (MapObject_GetID(object) != obj_partner_poke) {
        return 0;
    }
    return ((u8)shape >> 4) & 0xF;
}

u8 ov01_022055B0(LocalMapObject *object) {
    u16 shape = MapObject_GetParam(object, FOLLOW_MON_PARAM_SHAPE);
    if (MapObject_GetID(object) != obj_partner_poke) {
        return 0;
    }
    return (u8)shape & 0xF;
}

// Nonzero for a large following Pokemon (its own billboards, pulled while it shrinks into its ball).
u8 FollowMonObject_IsLarge(LocalMapObject *object) {
    u16 shape = MapObject_GetParam(object, FOLLOW_MON_PARAM_SHAPE);
    if (MapObject_GetID(object) != obj_partner_poke) {
        return 0;
    }
    return (shape >> 8) & 0xF;
}

// The tile behind the object.
void MapObject_GetCoordsBehind(LocalMapObject *object, int *x, int *z) {
    u8 direction = MapObject_GetFacingDirection(object);
    *x = MapObject_GetXCoord(object);
    *z = MapObject_GetZCoord(object);
    switch (direction) {
    case DIR_NORTH:
        (*z)++;
        break;
    case DIR_SOUTH:
        (*z)--;
        break;
    case DIR_WEST:
        (*x)++;
        break;
    case DIR_EAST:
        (*x)--;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

// The tile behind the object's previous position.
void MapObject_GetPreviousCoordsBehind(LocalMapObject *object, int *x, int *z) {
    u8 direction = MapObject_GetFacingDirection(object);
    *x = MapObject_GetPreviousXCoord(object);
    *z = MapObject_GetPreviousZCoord(object);
    switch (direction) {
    case DIR_NORTH:
        (*z)++;
        break;
    case DIR_SOUTH:
        (*z)--;
        break;
    case DIR_WEST:
        (*x)++;
        break;
    case DIR_EAST:
        (*x)--;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

// The metatile behavior of a tile next to the object: 0/1 the tile north/south of it, 2/3 east/west of it,
// 4/5 south-east/south-west of it.
u8 ov01_022056C4(LocalMapObject *object, u32 direction) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(object);
    int x = MapObject_GetXCoord(object);
    int z = MapObject_GetZCoord(object);
    switch (direction) {
    case 0:
        z--;
        break;
    case 1:
        z++;
        break;
    case 3:
        x--;
        break;
    case 2:
        x++;
        break;
    case 4:
        x++;
        z++;
        break;
    case 5:
        x--;
        z++;
        break;
    }
    return GetMetatileBehavior(fieldSystem, x, z);
}

// Puts object one tile from anchor in direction, facing facing.
void MapObject_SetPositionNextTo(LocalMapObject *anchor, LocalMapObject *object, int direction, int facing) {
    VecFx32 position;
    MapObject_CopyPositionVector(anchor, &position);
    switch (direction) {
    case DIR_NORTH:
        position.z -= FX32_ONE * 16;
        break;
    case DIR_SOUTH:
        position.z += FX32_ONE * 16;
        break;
    case DIR_EAST:
        position.x += FX32_ONE * 16;
        break;
    case DIR_WEST:
        position.x -= FX32_ONE * 16;
        break;
    }
    MapObject_SetPositionFromVectorAndDirection(object, &position, facing);
}

void FollowMon_SpawnReleaseBall(LocalMapObject *object) {
    FollowMonBall_Spawn(object, 0);
}

void FollowMon_MoveToPlayer(FieldSystem *fieldSystem, u8 direction) {
    VecFx32 position;
    LocalMapObject *followMon;
    if (FollowMon_IsActive(fieldSystem)) {
        followMon = FollowMon_GetMapObject(fieldSystem);
        MapObject_CopyPositionVector(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), &position);
        MapObject_SetPositionFromVectorAndDirection(followMon, &position, direction);
    }
}

BOOL FollowMon_CheckVisibleFlag(FieldSystem *fieldSystem) {
    return MapObject_CheckVisible(FollowMon_GetMapObject(fieldSystem));
}

void ov01_022057D0(FieldSystem *fieldSystem) {
    sub_020659B8(FollowMon_GetMapObject(fieldSystem));
}

#pragma force_active on
void ov01_022057DC(MapObjectManager *manager) {
    BillboardSequenceData *sequence = ResourceBank_Get(((MapObjectRenderData *)sub_0205F1A0(manager))->sequenceBank, BILLBOARD_SEQUENCE_FOLLOW_MON_ALT);
    int count = sequence->count, i;
    u8 *flags = (u8 *)(sequence->frames + count);
    flags += count;
    for (i = 0; i < count; i++) {
        flags[i] = 1;
    }
}
#pragma force_active reset

void ov01_02205808(BOOL alternate, LocalMapObject *object, void *billboard) {
    u8 record[0x28];
    MapObjectManager *manager = MapObject_GetManager(object);
    u32 spriteId = MapObject_GetSpriteID(object);
    ov01_021F9744(manager, spriteId, record);
    {
        u32 id = MapObject_GetID(object);
        if (id != obj_partner_poke && id != obj_daycare_poke_1 && id != obj_daycare_poke_2) {
            return;
        }
        ov01_02205870(alternate, object, billboard, record);
        BillboardObject_SetResource(billboard, record);
        BillboardObject_SetSequence(billboard, ov01_021FA44C(MapObject_GetFacingDirection(object)));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
}

void ov01_02205870(BOOL alternate, LocalMapObject *object, void *billboard, void *record) {
    MapObjectRenderData *renderData = sub_0205F1A0(MapObject_GetManager(object));
    void *sequence = ResourceBank_Get(renderData->sequenceBank, alternate ? BILLBOARD_SEQUENCE_FOLLOW_MON_ALT : BILLBOARD_SEQUENCE_FOLLOW_MON);
    BillboardGfxSequence_Init(sequence, (u8 *)record + 0xC);
}

#pragma force_active on
// Reloads the billboard's palette from the object's model file in sepia and re-uploads its texture.
void MapObject_SetBillboardSepia(void *unused, LocalMapObject *object, void *billboard) {
    SepiaBillboardTransfer *transfer = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(SepiaBillboardTransfer));
    void *texture, *palette;
    u32 texKey;
    int member;
    MI_CpuFill8(transfer, 0, 4);
    member = SpriteId_GetModelNo(((u32)MapObject_GetParam(object, FOLLOW_MON_PARAM_FLAGS) << 15) >> 16);
    transfer->file = NARC_AllocAndReadWholeMember(MapObjectManager_GetMapModelNarc(MapObject_GetManager(object)), member, HEAP_ID_FIELD2);
    texture = NNS_G3dGetTex(transfer->file);
    palette = Texture_GetPaletteData(texture, 0);
    transfer->texture = Texture_GetImageData(texture, 0);
    MIi_CpuCopy16(palette, transfer->palette, sizeof(transfer->palette));
    TintPalette_GrayScale(transfer->palette, 16);
    TintPalette_CustomTone(transfer->palette, 16, 256, 180, 0);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (u16)BillboardObject_GetPlttKey(billboard) * 8, transfer->palette, sizeof(transfer->palette));
    texKey = BillboardObject_GetTexKey(billboard);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, (u16)BillboardObject_GetTexKey(billboard) * 8, transfer->texture, ((texKey & 0x7FFF0000) >> 16) * 16);
    SysTask_CreateOnVWaitQueue(ov01_0220596C, transfer, 0);
}
#pragma force_active reset

// Frees the sepia transfer once the VRAM transfers are done.
void ov01_0220596C(SysTask *task, void *data) {
    SepiaBillboardTransfer *transfer = data;
    if (GF_GetNumPendingVramTransferTasks() == 0) {
        SysTask_Destroy(task);
        Heap_Free(transfer->file);
        Heap_Free(transfer);
    }
}

// The player's step (movement from x, z) for the following Pokemon to repeat.
void FollowMon_QueuePlayerStep(int movement, int x, int z, FollowMon *followMon) {
    followMon->unk4 = movement;
    followMon->unk8 = x;
    followMon->unkC = z;
    if (followMon->unk1C == 3) {
        followMon->unk1C = 2;
    } else if (followMon->unk1C == 0) {
        followMon->unk1C = 1;
    }
}

// On entering a map: re-creates the following Pokemon next to the player, or removes it where it may not follow.
void FollowMon_RecreateForMap(FieldSystem *fieldSystem) {
    if (fieldSystem->followMon.unk15) {
        if (fieldSystem->followMon.active == FALSE) {
            int x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
            int z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
            u32 direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
            FollowMon_InitMapObject(fieldSystem->mapObjectManager, x, z, direction, fieldSystem->location->mapId);
            {
                u8 active = fieldSystem->followMon.active;
                LocalMapObject *mapObject = fieldSystem->followMon.mapObject;
                if (active == TRUE) {
                    ov01_021F9048(mapObject);
                }
            }
        } else {
            int species = FollowMon_GetSpecies(fieldSystem->followMon.mapObject);
            if (!FollowMon_GetPermissionBySpeciesAndMap(species, fieldSystem->location->mapId)) {
                MapObject_Remove(fieldSystem->followMon.mapObject);
                fieldSystem->followMon.active = FALSE;
            }
        }
    }
    fieldSystem->followMon.unk15 = FALSE;
}

void FollowMon_SetHeight(FieldSystem *fieldSystem, fx32 y) {
    VecFx32 position;
    LocalMapObject *mapObject;
    if (FollowMon_IsActive(fieldSystem)) {
        mapObject = FollowMon_GetMapObject(fieldSystem);
        MapObject_CopyPositionVector(mapObject, &position);
        position.y = y;
        MapObject_SetPositionVector(mapObject, &position);
    }
}

// Lets the following Pokemon finish its current movement (pausing it again if it was paused).
BOOL Task_WaitFollowMonMovement(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    u32 *state = TaskManager_GetStatePtr(taskMan);
    LocalMapObject *mapObject;
    if (!FollowMon_IsActive(fieldSystem)) {
        return TRUE;
    }
    mapObject = fieldSystem->followMon.mapObject;
    switch (*state) {
    case 0:
        if (MapObject_CheckMovementPaused(mapObject)) {
            MapObject_UnpauseMovement(mapObject);
            *state = 1;
        } else {
            *state = 2;
        }
        break;
    case 1:
        if (MapObject_IsMovementPaused(mapObject) && MapObject_AreBitsSetForMovementScriptInit(mapObject)) {
            MapObject_PauseMovement(mapObject);
            return TRUE;
        }
        break;
    case 2:
        if (MapObject_IsMovementPaused(mapObject) && MapObject_AreBitsSetForMovementScriptInit(mapObject)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void ov01_02205AEC(FieldSystem *fieldSystem) {
    UnkStruct_ov01_02205B14 *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnkStruct_ov01_02205B14));
    env->state = 0;
    env->step = 0;
    env->timer = 0;
    env->movement = 0;
    TaskManager_Call(fieldSystem->taskman, ov01_02205B14, env);
}

#pragma force_active on
BOOL ov01_02205B14(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    UnkStruct_ov01_02205B14 *env = TaskManager_GetEnvironment(taskMan);
    LocalMapObject *mapObject;
    HopMovements movements;
    HopOffsets zOffsets, yOffsets;
    VecFx32 position, scale;
    int delta;
    switch (env->state) {
    case 0:
        if (!FollowMon_IsActive(fieldSystem)) {
            Heap_Free(env);
            return TRUE;
        }
        if (FollowMon_CheckVisibleFlag(fieldSystem)) {
            Heap_Free(env);
            return TRUE;
        }
        MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
        env->state++;
    case 1:
        if (MapObject_IsMovementPaused(FollowMon_GetMapObject(fieldSystem))) {
            env->state = ov01_02205CF0(fieldSystem, env);
        }
        break;
    case 2: {
        movements = ov01_022096E0.movements;
        mapObject = FollowMon_GetMapObject(fieldSystem);
        if (!MapObject_IsMovementPaused(mapObject)) {
            break;
        }
        MapObject_SetHeldMovement(mapObject, movements.movements[env->movement++]);
        if (env->movement >= 2) {
            env->state++;
        }
        break;
    }
    case 3:
        mapObject = FollowMon_GetMapObject(fieldSystem);
        if (!MapObject_IsMovementPaused(mapObject)) {
            break;
        }
        MapObject_SetHeldMovement(mapObject, 0);
        env->state++;
        break;
    case 4: {
        zOffsets = ov01_022096F0;
        yOffsets = ov01_022096E0.yOffsets;
        mapObject = FollowMon_GetMapObject(fieldSystem);
        delta = FX32_ONE * 2;
        if (!env->toLeft) {
            delta *= -1;
        }
        MapObject_CopyPositionVector(mapObject, &position);
        position.z -= ((s8)zOffsets.offsets[env->step]) * FX32_ONE;
        position.x += delta;
        position.y += ((s8)yOffsets.offsets[env->step]) * FX32_ONE;
        MapObject_SetPositionVector(mapObject, &position);
        env->step++;
        if (env->step >= 8) {
            env->state++;
        }
        break;
    }
    case 5:
        FollowMonBall_Spawn(FollowMon_GetMapObject(fieldSystem), 3);
        env->state++;
        break;
    case 6: {
        env->timer++;
        if (env->timer < 20) {
            break;
        }
        FollowMon_MoveToPlayer(fieldSystem, DIR_NORTH);
        scale = ov01_02209704;
        BillboardObject_SetScale(ov01_021F771C(fieldSystem->mapObjectManager), &scale);
        sub_02069E84(fieldSystem->followMon.mapObject, 1);
        env->state++;
        break;
    }
    case 7:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

// Which side of the player the following Pokemon is on: below it (state 2), or left/right of it (state 3).
u32 ov01_02205CF0(FieldSystem *fieldSystem, UnkStruct_ov01_02205B14 *env) {
    int playerX = MapObject_GetXCoord(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
    int playerZ = MapObject_GetZCoord(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar));
    int x = MapObject_GetXCoord(FollowMon_GetMapObject(fieldSystem));
    int z = MapObject_GetZCoord(FollowMon_GetMapObject(fieldSystem));
    if (x == playerX && z == playerZ + 1) {
        env->toLeft = TRUE;
        return 2;
    }
    if (x == playerX + 1 && z == playerZ) {
        env->toLeft = FALSE;
        return 3;
    }
    if (x + 1 == playerX && z == playerZ) {
        env->toLeft = TRUE;
        return 3;
    }
    GF_ASSERT(FALSE);
    return 2;
}

// Puts the following Pokemon into its Poke Ball (it turns white and shrinks); FALSE if there is nothing to do.
BOOL FollowMon_StartRecall(FieldSystem *fieldSystem) {
    FollowMonRecall *env;
    if (!FollowMon_IsActive(fieldSystem)) {
        return FALSE;
    }
    if (FollowMon_CheckVisibleFlag(fieldSystem)) {
        sub_0206A054(fieldSystem);
        FollowMon_MoveToPlayer(fieldSystem, DIR_NORTH);
        return FALSE;
    }
    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FollowMonRecall));
    env->timer = 0;
    TaskManager_Call(fieldSystem->taskman, Task_FollowMonRecall, env);
    return TRUE;
}

#pragma force_active on
BOOL Task_FollowMonRecall(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FollowMonRecall *env = TaskManager_GetEnvironment(taskMan);
    u32 *state = TaskManager_GetStatePtr(taskMan);
    switch (*state) {
    case 0:
        MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
        (*state)++;
    case 1:
        if (MapObject_IsMovementPaused(FollowMon_GetMapObject(fieldSystem))) {
            (*state)++;
        }
        break;
    case 2: {
        LocalMapObject *mapObject = FollowMon_GetMapObject(fieldSystem);
        int member = SpriteId_GetModelNo(MapObject_GetSpriteID(mapObject));
        void *file = NARC_AllocAndReadWholeMember(MapObjectManager_GetMapModelNarc(fieldSystem->mapObjectManager), member, HEAP_ID_FIELD2);
        u8 *texture = (u8 *)NNS_G3dGetTex(file);
        MIi_CpuCopy16((u16 *)(texture + *(u32 *)(texture + 0x38)), env->palette, sizeof(env->palette));
        Heap_Free(file);
        FollowMonBall_Spawn(mapObject, 1);
        (*state)++;
        break;
    }
    case 3:
        env->timer++;
        if (env->timer >= 20) {
            VecFx32 scale;
            void *billboard;
            u32 plttKey, plttAddr;
            FollowMon_MoveToPlayer(fieldSystem, DIR_NORTH);
            scale = ov01_022096F8;
            billboard = ov01_021F771C(fieldSystem->mapObjectManager);
            BillboardObject_SetScale(billboard, &scale);
            plttKey = BillboardObject_GetPlttKey(billboard);
            plttAddr = BillboardObject_GetPlttKey(billboard);
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (u16)plttAddr << 3, env->palette, ((plttKey & 0xFFFF0000) >> 16) << 3);
            sub_0206A054(fieldSystem);
            sub_02069E28(FollowMon_GetMapObject(fieldSystem), 0);
            (*state)++;
        }
        break;
    case 4:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

void ov01_02205EE0(TaskManager *taskMan) {
    int *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(int));
    *env = 0;
    TaskManager_Call(taskMan, ov01_02205F00, env);
}

#pragma force_active on
BOOL ov01_02205F00(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    int *timer = TaskManager_GetEnvironment(taskMan);
    u32 *state = TaskManager_GetStatePtr(taskMan);
    switch (*state) {
    case 0:
        FollowPlayerMovement_Reset(fieldSystem->followMon.mapObject);
        sub_0205F484(fieldSystem->followMon.mapObject);
        ++*state;
        break;
    case 1:
        if (MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject) == TRUE) {
            MapObject_StepTowards(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), fieldSystem->followMon.mapObject);
            ++*state;
        }
        break;
    case 2:
        if (MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject) == TRUE) {
            MapObject_SetFacingDirection(fieldSystem->followMon.mapObject, MapObject_GetFacingDirection(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar)));
            ++*state;
        }
        break;
    case 3:
        if (++*timer > 10) {
            ++*state;
        }
        break;
    case 4:
        if (MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject) == TRUE) {
            sub_0206A040(fieldSystem->followMon.mapObject, FALSE);
            MapObject_SetHeldMovement(fieldSystem->followMon.mapObject, GetMovementForDirection(MapObject_GetFacingDirection(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar)), 52));
            ++*state;
        }
        break;
    case 5:
        if (MapObject_AreBitsSetForMovementScriptInit(fieldSystem->followMon.mapObject) == TRUE) {
            FollowMonBall_Spawn(fieldSystem->followMon.mapObject, 2);
            sub_0206A054(fieldSystem);
            ++*state;
        }
        break;
    case 6:
        Heap_Free(timer);
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

// Starts object walking one tile towards target.
void MapObject_StepTowards(LocalMapObject *target, LocalMapObject *object) {
    int targetX = MapObject_GetXCoord(target);
    int targetZ = MapObject_GetZCoord(target);
    int x = MapObject_GetXCoord(object);
    int z = MapObject_GetZCoord(object);
    int dx = targetX - x;
    int dz = targetZ - z;
    MapObject_GetFacingDirection(target);
    if (dx < 0) {
        MapObject_SetHeldMovement(object, 10);
    } else if (dx > 0) {
        MapObject_SetHeldMovement(object, 11);
    } else if (dz < 0) {
        MapObject_SetHeldMovement(object, 8);
    } else if (dz > 0) {
        MapObject_SetHeldMovement(object, 9);
    }
}

int SpriteId_GetModelNo(u32 spriteId) {
    int modelNo = GetMoveModelNoBySpriteId(spriteId);
    if (modelNo < 0) {
        GF_ASSERT(FALSE);
        modelNo = 0;
    }
    return modelNo;
}

void FollowMon_SetFacingDirection(FieldSystem *fieldSystem, u32 direction) {
    if (FollowMon_IsActive(fieldSystem)) {
        MapObject_SetFacingDirection(fieldSystem->followMon.mapObject, direction);
    }
}

// Fades the following Pokemon's palette to white and back, step per change every period + 1 frames.
BOOL FollowMon_StartFlash(FieldSystem *fieldSystem, u8 step, u8 period) {
    FollowMonFlash *env;
    if (!FollowMon_IsActive(fieldSystem)) {
        return FALSE;
    }
    if (FollowMon_CheckVisibleFlag(fieldSystem)) {
        return FALSE;
    }
    if (step == 0) {
        return FALSE;
    }
    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FollowMonFlash));
    MI_CpuFill8(env, 0, sizeof(FollowMonFlash));
    env->step = step;
    env->period = period;
    TaskManager_Call(fieldSystem->taskman, Task_FollowMonFlash, env);
    return TRUE;
}

#pragma force_active on
BOOL Task_FollowMonFlash(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FollowMonFlash *env = TaskManager_GetEnvironment(taskMan);
    u32 *state = TaskManager_GetStatePtr(taskMan);
    switch (*state) {
    case 0: {
        int member = SpriteId_GetModelNo(MapObject_GetSpriteID(FollowMon_GetMapObject(fieldSystem)));
        void *file = NARC_AllocAndReadWholeMember(MapObjectManager_GetMapModelNarc(fieldSystem->mapObjectManager), member, HEAP_ID_FIELD2);
        u8 *texture = (u8 *)NNS_G3dGetTex(file);
        MIi_CpuCopy16((u16 *)(texture + *(u32 *)(texture + 0x38)), env->original, sizeof(env->original));
        Heap_Free(file);
        (*state)++;
        break;
    }
    case 1:
        if (env->delay) {
            env->delay--;
        } else {
            void *billboard;
            u32 plttKey, plttAddr;
            env->blend += env->step;
            if (env->blend >= 16) {
                env->blend = 16;
                (*state)++;
            }
            BlendPalette(env->original, env->blended, 32, (u8)env->blend, 0xFFFF);
            billboard = ov01_021F771C(fieldSystem->mapObjectManager);
            plttKey = BillboardObject_GetPlttKey(billboard);
            plttAddr = BillboardObject_GetPlttKey(billboard);
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (u16)plttAddr * 8, env->blended, ((plttKey & 0xFFFF0000) >> 16) * 8);
            env->delay = env->period;
        }
        break;
    case 2:
        if (env->delay) {
            env->delay--;
        } else {
            void *billboard;
            u32 plttKey, plttAddr;
            env->blend -= env->step;
            if (env->blend <= 0) {
                (*state)++;
            }
            BlendPalette(env->original, env->blended, 32, (u8)env->blend, 0xFFFF);
            billboard = ov01_021F771C(fieldSystem->mapObjectManager);
            plttKey = BillboardObject_GetPlttKey(billboard);
            plttAddr = BillboardObject_GetPlttKey(billboard);
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (u16)plttAddr * 8, env->blended, ((plttKey & 0xFFFF0000) >> 16) * 8);
            env->delay = env->period;
        }
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

BOOL FollowMon_IsNextToPlayer(FieldSystem *fieldSystem) {
    LocalMapObject *player, *followMon;
    int x, z, followX, followZ;
    if (!FollowMon_IsVisible(fieldSystem)) {
        return FALSE;
    }
    player = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    followMon = fieldSystem->followMon.mapObject;
    x = MapObject_GetXCoord(player);
    z = MapObject_GetZCoord(player);
    followX = MapObject_GetXCoord(followMon);
    followZ = MapObject_GetZCoord(followMon);
    if (x == followX) {
        if (z + 1 == followZ || z - 1 == followZ) {
            return TRUE;
        }
    } else if (z == followZ) {
        if (x + 1 == followX || x - 1 == followX) {
            return TRUE;
        }
    }
    return FALSE;
}
#pragma force_active reset

u8 FieldSystem_GetLeadMonIndex(FieldSystem *fieldSystem) {
    return GetIdxOfFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
}
