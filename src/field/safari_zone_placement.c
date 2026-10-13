#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/maps.h"

#include "overlay_2/overlay_02_02248728.h"

#include "field_metatile_query_internal.h"
#include "field_system.h"
#include "heap.h"
#include "map_object_movement_utilities.h"
#include "metatile_behavior.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "player_data.h"
#include "safari_zone.h"
#include "safari_zone_encounters.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "sys_flags.h"

// The Safari Zone map is a 3x2 grid of 32x32 areas, starting at (32, 32).
#define SAFARI_AREA_MIN_X 32
#define SAFARI_AREA_MAX_X 128
#define SAFARI_AREA_MIN_Z 32
#define SAFARI_AREA_MAX_Z 96

// Rounds the centre of grid cell n to fx32.
#define GRID_TO_FX32(n) ((fx32)(((n) * 16 + 8) > 0 ? (float)(((n) * 16 + 8) << 12) + 0.5f : (float)(((n) * 16 + 8) << 12) - 0.5f))

// Placement offsets: [0..1] across for 2x1 objects, [2..3] along for 1x2 objects, [4..5] for 2x2 objects.
extern const u8 ov02_02253A4C[];

BOOL MapId_IsSafariZone(u32 mapId);
int SafariZone_GetAreaIndexAt(s32 x, s32 z);
BOOL SafariZone_IsWaterObjectPlacementUnlocked(SaveData *saveData);
BOOL SafariZone_FindSpotFor1x2Object(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 onWater);
BOOL SafariZone_FindSpotFor2x1Object(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 onWater);
BOOL SafariZone_FindSpotFor2x2Object(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 onWater);
BOOL SafariZone_CanPlaceObjectOn(u8 behavior, BOOL blocked, u32 onWater);

BOOL MapId_IsSafariZone(u32 mapId) {
    return mapId == MAP_SAFARI_ZONE_ENTRANCE_EXTERIOR;
}

int SafariZone_GetAreaIndexAt(s32 x, s32 z) {
    int area = (x - SAFARI_AREA_MIN_X) / 32 + ((z - SAFARI_AREA_MIN_Z) / 32) * 3;
    if (area < 0 || area >= 6) {
        area = 0;
    }
    return area;
}

int FieldSystem_GetSafariAreaIndex(FieldSystem *fieldSystem) {
    return SafariZone_GetAreaIndexAt(PlayerAvatar_GetXCoord(fieldSystem->playerAvatar), PlayerAvatar_GetZCoord(fieldSystem->playerAvatar));
}

// The tile in front of the player is inside the Safari Zone areas and at the player's height.
BOOL FieldSystem_CanPlaceSafariObject(FieldSystem *fieldSystem) {
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    SafariZone *safariZone;
    int facing;
    int x;
    int z;
    int newX;
    int newZ;
    VecFx32 pos;
    u8 kind;
    fx32 height;
    fx32 newHeight;
    f32 fx;
    f32 fz;
    int vx;
    int vz;

    LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(fieldSystem->saveData));
    safariZone = Save_SafariZone_Get(fieldSystem->saveData);
    if (MapId_IsSafariZone(fieldSystem->location->mapId) == FALSE) {
        return FALSE;
    }
    if (Save_VarsFlags_CheckSafariSysFlag(varsFlags) == FALSE) {
        return FALSE;
    }
    if (SafariZone_GetActiveAreaSet(safariZone) != 0) {
        return FALSE;
    }
    if (SafariZone_GetObjectUnlockLevel(safariZone) == 0) {
        return FALSE;
    }
    facing = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    newX = x + GetDeltaXByFacingDirection(facing);
    newZ = z + GetDeltaYByFacingDirection(facing);
    if (newX < SAFARI_AREA_MIN_X || newX >= SAFARI_AREA_MAX_X || newZ < SAFARI_AREA_MIN_Z || newZ >= SAFARI_AREA_MAX_Z) {
        return FALSE;
    }
    if (newX < SAFARI_AREA_MIN_X || newX >= SAFARI_AREA_MAX_X || newZ < SAFARI_AREA_MIN_Z || newZ >= SAFARI_AREA_MAX_Z) {
        return FALSE;
    }
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &pos);
    height = Terrain_GetHeight_Bdhc(fieldSystem, pos.y, pos.x, pos.z, &kind);
    if (kind != 1) {
        return FALSE;
    }
    vz = (newZ << 4) + 8;
    if (vz > 0) {
        fz = (f32)(vz << 12) + 0.5f;
    } else {
        fz = (f32)(vz << 12) - 0.5f;
    }
    vx = (newX << 4) + 8;
    if (vx > 0) {
        fx = (f32)(vx << 12) + 0.5f;
    } else {
        fx = (f32)(vx << 12) - 0.5f;
    }
    newHeight = Terrain_GetHeight_Bdhc(fieldSystem, pos.y, (s32)fx, (s32)fz, &kind);
    if (kind != 1) {
        return FALSE;
    }
    return height == newHeight;
}

BOOL SafariZone_CanPlaceObjectOnLand(u8 behavior, BOOL blocked) {
    if (blocked) {
        return FALSE;
    }
    return sub_0205BAD0(behavior);
}

BOOL SafariZone_CanPlaceObjectOnWater(u8 behavior, BOOL blocked) {
    if (blocked) {
        return FALSE;
    }
    return MetatileBehavior_IsSurfableWater_thunk(behavior);
}

#pragma force_active on

SafariDecorationArgs *SafariDecoration_CreateArgs(FieldSystem *fieldSystem, enum HeapID heapID) {
    SafariDecorationArgs *args = Heap_AllocAtEnd(heapID, sizeof(SafariDecorationArgs));
    s16 x;
    s16 z;
    u8 onWater;
    u8 direction;
    s16 spotX;
    s16 spotZ;
    u8 kind;
    VecFx32 pos;

    MI_CpuFill8(args, 0, sizeof(SafariDecorationArgs));
    args->saveData = FieldSystem_GetSaveData(fieldSystem);
    args->unk04 = (BOOL *)&fieldSystem->menuInputState;
    args->unk20 = fieldSystem->unk_111;
    onWater = (u8)(PlayerAvatar_GetState(fieldSystem->playerAvatar) == PLAYER_STATE_SURFING ? TRUE : FALSE);
    args->unk18 = onWater;
    direction = (u8)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    x = (s16)PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    z = (s16)PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &pos);
    args->unk19 = (u8)SafariZone_GetAreaIndexAt(x, z);
    pos.y = Terrain_GetHeight_Bdhc(fieldSystem, pos.y, pos.x, pos.z, &kind);
    if (kind != 1) {
        return args;
    }
    args->unk1A = (u8)(pos.y >> FX32_SHIFT);
    args->deltaX = (u8)(x + GetDeltaXByFacingDirection(direction));
    args->deltaY = (u8)(z + GetDeltaYByFacingDirection(direction));
    args->unk08 = TRUE;
    if (SafariZone_FindSpotFor1x2Object(fieldSystem, x, z, direction, pos.y, &spotX, &spotZ, onWater)) {
        args->unk0D = (u8)spotX;
        args->unk0F = (u8)spotZ;
        args->unk0C = TRUE;
    }
    if (SafariZone_FindSpotFor2x1Object(fieldSystem, x, z, direction, pos.y, &spotX, &spotZ, onWater)) {
        args->unk11 = (u8)spotX;
        args->unk13 = (u8)spotZ;
        args->unk10 = TRUE;
    }
    if (SafariZone_FindSpotFor2x2Object(fieldSystem, x, z, direction, pos.y, &spotX, &spotZ, onWater)) {
        args->unk15 = (u8)spotX;
        args->unk17 = (u8)spotZ;
        args->unk14 = TRUE;
    }
    return args;
}

#pragma force_active reset

BOOL SafariZone_IsWaterObjectPlacementUnlocked(SaveData *saveData) {
    u32 trainerId = PlayerProfile_GetTrainerID(Save_PlayerData_GetProfile(saveData));
    int unlockLevel = SafariZone_GetObjectUnlockLevel(Save_SafariZone_Get(saveData));
    u8 digit = trainerId % 10;
    u8 value;

    if (digit < 6) {
        value = digit / 3;
    } else {
        value = (digit - 6) / 2 + 2;
    }
    value = 3 - value;
    return unlockLevel >= value + 1;
}

// 0: the object can be placed, 1: the area is full, 2: not unlocked, 3: no room.
u16 FieldSystem_CheckSafariObjectPlacement(FieldSystem *fieldSystem) {
    u8 direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    s16 x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    s16 z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    VecFx32 position;
    s16 spotX;
    s16 spotZ;
    SafariZoneAreaSet *areaSet;
    u8 areaIndex;

    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &position);
    areaSet = SafariZone_GetAreaSet(Save_SafariZone_Get(fieldSystem->saveData), 0);
    areaIndex = (x - SAFARI_AREA_MIN_X) / 32 + (z - SAFARI_AREA_MIN_Z) / 32 * 3;
    if (areaSet->areas[areaIndex].active_object_count >= 30) {
        return 1;
    }
    if (PlayerAvatar_GetState(fieldSystem->playerAvatar) != PLAYER_STATE_SURFING) {
        return 0;
    }
    if (!SafariZone_IsWaterObjectPlacementUnlocked(fieldSystem->saveData)) {
        return 2;
    }
    if (SafariZone_FindSpotFor2x2Object(fieldSystem, x, z, direction, position.y, &spotX, &spotZ, TRUE)) {
        return 0;
    }
    return 3;
}

// Returns the id of the placed object the player is facing (0xFF if none) and its index in its area.
u16 FieldSystem_GetFacingSafariObject(FieldSystem *fieldSystem, u16 *objectIndex) {
    int direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    int x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar) + GetDeltaXByFacingDirection(direction);
    int z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar) + GetDeltaYByFacingDirection(direction);
    u8 gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData));
    int areaIndex = SafariZone_GetAreaIndexAt(x, z);
    SAFARIZONE_AREA *area;
    int i;

    x %= 32;
    z %= 32;
    area = &SafariZone_GetAreaSet(Save_SafariZone_Get(fieldSystem->saveData), 0)->areas[areaIndex];
    for (i = 0; i < area->active_object_count; i++) {
        SafariObjectConfig config;
        SAFARIZONE_OBJECT *object = &area->objects[i];

        GetSafariObjectConfig(&config, object->id, gender);
        if (x >= object->x && z <= object->z && x < object->x + config.width && z > object->z - config.height) {
            if (objectIndex != NULL) {
                *objectIndex = i;
            }
            return object->id;
        }
    }
    *objectIndex = 0;
    return 0xFF;
}

#pragma force_active on

BOOL SafariZone_FindSpotFor1x2Object(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 onWater) {
    u8 query[3];
    u8 behavior;
    BOOL blocked;
    unsigned int i;
    u8 count;
    u8 idx;
    s8 offset;
    int height;
    s8 *delta;
    s8 *offsets;

    offsets = (s8 *)query;
    offsets[1] = ov02_02253A4C[2];
    offsets[2] = ov02_02253A4C[3];
    *outX = x;
    *outZ = z;
    switch (direction) {
    case DIR_NORTH:
    case DIR_SOUTH:
        count = 0;
        if (direction != DIR_NORTH) {
            idx = 1;
        } else {
            idx = 0;
        }
        offset = ((s8 *)&query[1])[idx];
        for (i = 0; (int)i < 2; i++) {
            s16 testZ = z + offset * (i + 1);
            behavior = GetMetatileBehavior(fieldSystem, x, testZ);
            blocked = FieldSystem_CheckTileCollision(fieldSystem, x, testZ);
            if (SafariZone_CanPlaceObjectOn(behavior, blocked, onWater)) {
                height = Terrain_GetHeight_Bdhc(fieldSystem, y, GRID_TO_FX32(x), GRID_TO_FX32(testZ), query);
                if (query[0] == 1 && height == y) {
                    count++;
                }
            }
        }
        if (count == 2) {
            *outX = x;
            if (direction == DIR_NORTH) {
                *outZ = z - 1;
            } else {
                *outZ = z + 2;
            }
            return TRUE;
        }
        break;
    case DIR_WEST:
    case DIR_EAST:
        x = (s16)(x + offsets[direction == DIR_WEST ? 1 : 2]);
        behavior = GetMetatileBehavior(fieldSystem, x, z);
        blocked = FieldSystem_CheckTileCollision(fieldSystem, x, z);
        height = Terrain_GetHeight_Bdhc(fieldSystem, y, GRID_TO_FX32(x), GRID_TO_FX32(z), query);
        if (!SafariZone_CanPlaceObjectOn(behavior, blocked, onWater) || query[0] != 1 || height != y) {
            return FALSE;
        }
        for (delta = (s8 *)&query[1], i = 0; (int)i < 2; delta++, i++) {
            s16 testZ = z + *delta;
            behavior = GetMetatileBehavior(fieldSystem, x, testZ);
            blocked = FieldSystem_CheckTileCollision(fieldSystem, x, testZ);
            height = Terrain_GetHeight_Bdhc(fieldSystem, y, GRID_TO_FX32(x), GRID_TO_FX32(testZ), query);
            if (SafariZone_CanPlaceObjectOn(behavior, blocked, onWater) && query[0] == 1 && height == y) {
                *outX = x;
                *outZ = z + i;
                return TRUE;
            }
        }
        break;
    }
    return FALSE;
}

BOOL SafariZone_FindSpotFor2x1Object(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 onWater) {
    u8 query[3];
    u8 behavior;
    BOOL blocked;
    int i;
    u8 count;
    s16 start;
    int height;
    s8 *delta;
    s8 *offsets;

    offsets = (s8 *)query;
    offsets[1] = ov02_02253A4C[0];
    offsets[2] = ov02_02253A4C[1];
    *outX = x;
    *outZ = z;
    switch (direction) {
    case DIR_NORTH:
    case DIR_SOUTH:
        z = (s16)(z + offsets[direction == DIR_NORTH ? 1 : 2]);
        behavior = GetMetatileBehavior(fieldSystem, x, z);
        blocked = FieldSystem_CheckTileCollision(fieldSystem, x, z);
        height = Terrain_GetHeight_Bdhc(fieldSystem, y, GRID_TO_FX32(x), GRID_TO_FX32(z), query);
        if (!SafariZone_CanPlaceObjectOn(behavior, blocked, onWater) || query[0] != 1 || height != y) {
            return FALSE;
        }
        for (delta = (s8 *)&query[1], i = 0; i < 2; delta++, i++) {
            s16 testX = x - *delta;
            behavior = GetMetatileBehavior(fieldSystem, testX, z);
            blocked = FieldSystem_CheckTileCollision(fieldSystem, testX, z);
            height = Terrain_GetHeight_Bdhc(fieldSystem, y, GRID_TO_FX32(testX), GRID_TO_FX32(z), query);
            if (SafariZone_CanPlaceObjectOn(behavior, blocked, onWater) && query[0] == 1 && height == y) {
                *outX = x - i;
                *outZ = z;
                return TRUE;
            }
        }
        break;
    case DIR_WEST:
    case DIR_EAST:
        count = 0;
        if (direction == DIR_WEST) {
            start = x - 2;
        } else {
            start = x + 1;
        }
        for (i = 0; i < 2; i++) {
            behavior = GetMetatileBehavior(fieldSystem, (s16)(start + i), z);
            blocked = FieldSystem_CheckTileCollision(fieldSystem, (s16)(start + i), z);
            if (SafariZone_CanPlaceObjectOn(behavior, blocked, onWater)) {
                height = Terrain_GetHeight_Bdhc(fieldSystem, y, GRID_TO_FX32((s16)(start + i)), GRID_TO_FX32(z), query);
                if (query[0] == 1 && height == y) {
                    count++;
                }
            }
        }
        if (count == 2) {
            *outX = start;
            *outZ = z;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL SafariZone_FindSpotFor2x2Object(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 onWater) {
    s8 offsets[2];
    int i;

    offsets[0] = ov02_02253A4C[4];
    offsets[1] = ov02_02253A4C[5];
    *outX = x;
    *outZ = z;
    switch (direction) {
    case DIR_NORTH:
    case DIR_SOUTH:
        if (SafariZone_FindSpotFor1x2Object(fieldSystem, x, z, direction, y, outX, outZ, onWater)) {
            for (i = 0; i < 2; i++) {
                if (SafariZone_FindSpotFor1x2Object(fieldSystem, (s16)(x + offsets[i]), z, direction, y, outX, outZ, onWater)) {
                    *outX = x - i;
                    if (direction == DIR_NORTH) {
                        *outZ = z - 1;
                    } else {
                        *outZ = z + 2;
                    }
                    return TRUE;
                }
            }
        }
        break;
    case DIR_WEST:
    case DIR_EAST:
        if (SafariZone_FindSpotFor2x1Object(fieldSystem, x, z, direction, y, outX, outZ, onWater)) {
            for (i = 0; i < 2; i++) {
                if (SafariZone_FindSpotFor2x1Object(fieldSystem, x, (s16)(z - offsets[i]), direction, y, outX, outZ, onWater)) {
                    if (direction == DIR_WEST) {
                        *outX = x - 2;
                    } else {
                        *outX = x + 1;
                    }
                    *outZ = z + i;
                    return TRUE;
                }
            }
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL SafariZone_CanPlaceObjectOn(u8 behavior, BOOL blocked, u32 onWater) {
    if (onWater == FALSE) {
        return SafariZone_CanPlaceObjectOnLand(behavior, blocked);
    }
    return SafariZone_CanPlaceObjectOnWater(behavior, blocked);
}
