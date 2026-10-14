#include "global.h"

#include "field_system.h"
#include "map_object.h"
#include "map_object_movement_utilities_internal.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "communication_command.h"
#include "comm_state.h"
#include "field_terrain.h"

// Defined elsewhere without a header yet.
void MapObject_ExecuteMovementCommand(LocalMapObject *object);
void MapObject_InitTypeBehavior(LocalMapObject *object);
BOOL MapObject_UpdateTypeBehavior(LocalMapObject *object);
const MapObjectEffectFlags *MapObject_GetEffectFlags(LocalMapObject *object);
u32 MapObject_GetAdjacentTileBehavior(LocalMapObject *object, u32 direction);
void Reflection_Spawn(LocalMapObject *object, int kind);
void Reflection_SpawnForFollower(LocalMapObject *object, int kind);
void TallGrass_Spawn(LocalMapObject *object, int mode);
void VeryTallGrass_Spawn(LocalMapObject *object, int mode);
void PuddleSplash_Spawn(LocalMapObject *object, int x, int y, int z);
void MudSplash_Spawn(LocalMapObject *object, int x, int y, int z);
void ShallowWaterRipple_Spawn(LocalMapObject *object, int value);
void Footprints_SpawnInSnow(LocalMapObject *object);
void Footprints_SpawnOnBehavior169(LocalMapObject *object);
void Footprints_SpawnTracksOnBehavior169(LocalMapObject *object);
void Footprints_SpawnInSand(LocalMapObject *object);
void Footprints_SpawnTracksInSand(LocalMapObject *object);
void FieldShadow_CreateMapObjectShadow(LocalMapObject *object);
void FieldShadow_CreateFollowMonShadow(LocalMapObject *object);
void GroundDust_Spawn(LocalMapObject *object);

void MapObject_CallMovementAndBehaviorInit(LocalMapObject *object) {
    MapObject_CallMovementInit(object);
    MapObject_InitTypeBehavior(object);
}

void MapObject_UpdateMovement(LocalMapObject *object) {
    if (MapObject_GetManagerFlagsBitsMask(object, MAPOBJECTFLAG_SINGLE_MOVEMENT) == 0) {
        MapObject_UpdateHeightIfFlagged(object);
        MapObject_RestartIfBehaviorsLoaded(object);
        MapObject_UpdateEffectsBeforeMovement(object);
        if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK4)) {
            MapObject_ExecuteMovementCommand(object);
        } else if (!MapObject_CheckMovementPaused(object) && MapObject_CanUpdateMovement(object) == 1 && !MapObject_UpdateTypeBehavior(object)) {
            MapObject_CallMovementUpdate(object);
        }
        MapObject_UpdateEffectsAfterMovement(object);
        MapObject_UpdateEffectsAtStepEnd(object);
    }
}

BOOL MapObject_CanUpdateMovement(LocalMapObject *object) {
    u32 flags;
    if (MapObject_CheckSingleMovement(object) == 1) {
        return TRUE;
    }
    if (!MapObject_GetFlagsBitsMask(object, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK11 | MAPOBJECTFLAG_UNK12))) {
        return TRUE;
    }
    if (MapObject_GetMovement(object) == 0x32) {
        return TRUE;
    }
    if (MapObject_GetMovement(object) == 0x30) {
        return TRUE;
    }
    flags = MapObject_GetFlags(object);
    if ((flags & (1 << 12)) && !(flags & (1 << 23))) {
        return FALSE;
    }
    if ((flags & (2 << 10)) && !MapObject_CheckIgnoreMetatileBehaviors(object)) {
        return FALSE;
    }
    return TRUE;
}

void MapObject_UpdateHeightIfFlagged(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK12)) {
        MapObject_UpdateHeight(object);
    }
}

void MapObject_RestartIfBehaviorsLoaded(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK11) && MapObject_UpdateMetatileBehaviors(object) == 1) {
        MapObject_SetStartMovement(object);
    }
}

void MapObject_UpdateEffectsBeforeMovement(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_START_MOVEMENT)) {
        MapObject_SpawnStartEffects(object);
    }
    MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_START_MOVEMENT | MAPOBJECTFLAG_UNK16));
}

void MapObject_UpdateEffectsAfterMovement(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK16)) {
        MapObject_SpawnJumpStartEffects(object);
    } else if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_START_MOVEMENT)) {
        MapObject_SpawnStepStartEffects(object);
    }
    MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_START_MOVEMENT | MAPOBJECTFLAG_UNK16));
}

void MapObject_UpdateEffectsAtStepEnd(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK17)) {
        MapObject_SpawnJumpLandEffects(object);
    } else if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_END_MOVEMENT)) {
        MapObject_SpawnStepEndEffects(object);
    }
    MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_END_MOVEMENT | MAPOBJECTFLAG_UNK17));
}

const TileTestFunc gMetatileBlocksExitTests[4] = { MetatileBehavior_IsBlockedNorth, MetatileBehavior_IsBlockedSouth, MetatileBehavior_IsBlockedWest, MetatileBehavior_IsBlockedEast };
const TileTestFunc gMetatileBlocksEntryTests[4] = { MetatileBehavior_IsBlockedSouth, MetatileBehavior_IsBlockedNorth, MetatileBehavior_IsBlockedEast, MetatileBehavior_IsBlockedWest };
const s32 gDirectionDeltaX[4] = { 0, 0, -1, 1 };
const s32 gDirectionDeltaY[4] = { -1, 1, 0, 0 };
const s32 gOppositeDirections[4] = { 1, 0, 3, 2 };

void MapObject_SpawnStartEffects(LocalMapObject *object) {
    u8 previous;
    u8 current;
    const MapObjectEffectFlags *effect;
    MapObject_UpdateMetatileBehaviors(object);
    if (MapObject_IsDrawInitialized(object) == 1) {
        previous = MapObject_GetCurrentMetatileBehavior(object);
        current = MapObject_GetPreviousMetatileBehavior(object);
        effect = MapObject_GetEffectFlags(object);
        MapObject_UpdateOnBridgeFlag(object, previous, current, effect);
        MapObject_SpawnTallGrassStanding(object, previous, current, effect);
        MapObject_UpdateShallowWaterRipple(object, previous, current, effect);
        MapObject_CreateShadowWithSavedBehavior(object, previous, current, effect);
        MapObject_ResetTileDrawOffset(object, previous, current, effect);
        MapObject_SpawnVeryTallGrassStanding(object, previous, current, effect);
        MapObject_TileEffectDummy(object, previous, current, effect);
        MapObject_CreateReflection(object, previous, current, effect);
    }
}

void MapObject_SpawnStepStartEffects(LocalMapObject *object) {
    u8 previous;
    u8 current;
    const MapObjectEffectFlags *effect;
    MapObject_UpdateMetatileBehaviors(object);
    if (MapObject_IsDrawInitialized(object) == 1) {
        previous = MapObject_GetCurrentMetatileBehavior(object);
        current = MapObject_GetPreviousMetatileBehavior(object);
        effect = MapObject_GetEffectFlags(object);
        MapObject_UpdateOnBridgeFlag(object, previous, current, effect);
        MapObject_SpawnTallGrassOnStep(object, previous, current, effect);
        MapObject_SpawnFootprints(object, previous, current, effect);
        MapObject_UpdateShallowWaterRipple(object, previous, current, effect);
        MapObject_CreateShadow(object, previous, current, effect);
        MapObject_SpawnVeryTallGrassOnStep(object, previous, current, effect);
        MapObject_TileEffectDummy2(object, previous, current, effect);
        MapObject_SpawnPuddleSplashAtPrevious(object, previous, current, effect);
        MapObject_SpawnMudSplashAtPrevious(object, previous, current, effect);
        MapObject_CreateReflection(object, previous, current, effect);
        MapObject_TileEffectDummy3(object, previous, current, effect);
    }
}

void MapObject_SpawnJumpStartEffects(LocalMapObject *object) {
    MapObject_UpdateMetatileBehaviors(object);
    if (MapObject_IsDrawInitialized(object) == 1) {
        u8 previous = MapObject_GetCurrentMetatileBehavior(object);
        u8 current = MapObject_GetPreviousMetatileBehavior(object);
        const MapObjectEffectFlags *effect = MapObject_GetEffectFlags(object);
        MapObject_UpdateOnBridgeFlag(object, previous, current, effect);
        MapObject_CreateShadow(object, previous, current, effect);
        MapObject_CreateReflection(object, previous, current, effect);
        MapObject_ResetShallowWaterRipple(object, previous, current, effect);
        MapObject_TileEffectDummy3(object, previous, current, effect);
    }
}

void MapObject_SpawnStepEndEffects(LocalMapObject *object) {
    u8 value = MapObject_GetPreviousMetatileBehavior(object);
    MapObject_SetSavedPreviousBehavior(object, value);
    MapObject_UpdateMetatileBehaviors(object);
    if (MapObject_IsDrawInitialized(object) == TRUE) {
        u8 a = MapObject_GetCurrentMetatileBehavior(object);
        u8 b = MapObject_GetPreviousMetatileBehavior(object);
        const MapObjectEffectFlags *effect = MapObject_GetEffectFlags(object);
        MapObject_ResetTileDrawOffset(object, a, b, effect);
        MapObject_SpawnPuddleSplash(object, a, b, effect);
        MapObject_SpawnMudSplash(object, a, b, effect);
        MapObject_UpdateShallowWaterRipple(object, a, b, effect);
        MapObject_UpdateReflection(object, a, b, effect);
        MapObject_UpdateShadowVisibility(object, a, b, effect);
    }
}

void MapObject_SpawnJumpLandEffects(LocalMapObject *object) {
    MapObject_UpdateMetatileBehaviors(object);
    if (MapObject_IsDrawInitialized(object) == TRUE) {
        u8 a = MapObject_GetCurrentMetatileBehavior(object);
        u8 b = MapObject_GetPreviousMetatileBehavior(object);
        const MapObjectEffectFlags *effect = MapObject_GetEffectFlags(object);
        MapObject_ResetTileDrawOffset(object, a, b, effect);
        MapObject_SpawnPuddleSplash(object, a, b, effect);
        MapObject_SpawnMudSplash(object, a, b, effect);
        MapObject_UpdateShallowWaterRipple(object, a, b, effect);
        MapObject_UpdateReflection(object, a, b, effect);
        MapObject_UpdateShadowVisibility(object, a, b, effect);
        MapObject_SpawnTallGrassOnStep(object, a, b, effect);
        MapObject_SpawnGroundDust(object, a, b, effect);
    }
}

void MapObject_ResetTileDrawOffset(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    VecFx32 position = { 0, 0, 0 };
    MapObject_SetTileDrawOffset(object, &position);
}

void MapObject_SpawnTallGrassStanding(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u32 id = MapObject_GetID(object);
    int x;
    int z;
    u8 facing;
    FieldSystem *fieldSystem;
    u32 nextBehavior;

    if (MetatileBehavior_IsTallGrass(behavior) == TRUE) {
        if (id != 0xFD || MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != TRUE) {
            TallGrass_Spawn(object, 0);
        } else {
            return;
        }
    }
    if (MapObject_GetID(object) != 0xFD || FollowMonObject_IsLarge(object) == FALSE) {
        return;
    }
    facing = MapObject_GetFacingDirection(object);
    fieldSystem = MapObject_GetFieldSystem(object);
    if ((u8)(facing + 0xFE) > 1) {
        return;
    }
    MapObject_GetCoordsBehind(object, &x, &z);
    nextBehavior = GetMetatileBehavior(fieldSystem, x, z);
    if (MetatileBehavior_IsTallGrass((u8)nextBehavior) == TRUE) {
        TallGrass_SpawnAt(object, 0, x, z, 1);
        return;
    }
    if (MetatileBehavior_IsVeryTallGrass((u8)nextBehavior) == TRUE) {
        VeryTallGrass_SpawnAt(object, 0, x, z, 1);
    }
}

void MapObject_SpawnTallGrassOnStep(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u32 id = MapObject_GetID(object);
    int x;
    int z;
    u8 facing;
    FieldSystem *fieldSystem;
    u32 nextBehavior;

    if (MetatileBehavior_IsTallGrass(behavior) == TRUE) {
        if (id != 0xFD || MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != TRUE) {
            TallGrass_Spawn(object, 1);
        } else {
            return;
        }
    }
    if (MapObject_GetID(object) != 0xFD || FollowMonObject_IsLarge(object) == FALSE) {
        return;
    }
    facing = MapObject_GetFacingDirection(object);
    fieldSystem = MapObject_GetFieldSystem(object);
    if ((u8)(facing + 0xFE) > 1) {
        return;
    }
    MapObject_GetCoordsBehind(object, &x, &z);
    nextBehavior = GetMetatileBehavior(fieldSystem, x, z);
    if (MetatileBehavior_IsTallGrass((u8)nextBehavior) == TRUE) {
        TallGrass_SpawnAt(object, 1, x, z, 1);
        return;
    }
    if (MetatileBehavior_IsVeryTallGrass((u8)nextBehavior) == TRUE) {
        VeryTallGrass_SpawnAt(object, 1, x, z, 1);
    }
}

void MapObject_SpawnFootprints(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u32 kind = flags->footprints;

    if (kind == 0) {
        return;
    }
    if (MetatileBehavior_Is169(prevBehavior) == TRUE) {
        kind = flags->footprints;
        if (kind == 1) {
            Footprints_SpawnOnBehavior169(object);
        } else if (kind == 2) {
            Footprints_SpawnTracksOnBehavior169(object);
        }
    }
    if (MapObject_CheckSandBehavior(object, prevBehavior) == TRUE) {
        kind = flags->footprints;
        if (kind == 1) {
            Footprints_SpawnInSand(object);
            return;
        } else if (kind == 2) {
            Footprints_SpawnTracksInSand(object);
            return;
        }
    } else if (MapObject_CheckSnowFootprintBehavior(object, prevBehavior) == TRUE) {
        Footprints_SpawnInSnow(object);
    }
}

void MapObject_UpdateShallowWaterRipple(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (MetatileBehavior_IsShallowWater(behavior) == 1) {
        if (MapObject_CheckFlag26(object) == 0) {
            ShallowWaterRipple_Spawn(object, 1);
            MapObject_SetFlag26(object, TRUE);
        }
    } else {
        MapObject_SetFlag26(object, FALSE);
    }
}

void MapObject_ResetShallowWaterRipple(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    MapObject_SetFlag26(object, FALSE);
}

void MapObject_CreateShadowWithSavedBehavior(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u8 value = MapObject_GetSavedPreviousBehavior(object);
    MapObject_CreateShadow(object, behavior, value, flags);
}

#define IS_GROUND_EFFECT_TILE(object, behavior)                                                           \
    (MetatileBehavior_IsTallGrass(behavior) == TRUE || MetatileBehavior_IsVeryTallGrass(behavior) == TRUE \
        || MapObject_CheckWaterBehavior(object, behavior) == TRUE || MetatileBehavior_IsPuddle(behavior) == TRUE          \
        || MetatileBehavior_IsShallowWater(behavior) == TRUE || MapObject_CheckSnowBehavior(object, behavior) == TRUE    \
        || MetatileBehavior_IsMud(behavior) == TRUE || MetatileBehavior_IsReflective(behavior) != FALSE)

void MapObject_CreateShadow(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u32 state;

    if (MapObjectManager_NotEndMovement(MapObject_GetManager(object)) == FALSE) {
        return;
    }
    state = flags->shadow;
    if (state == 0) {
        return;
    }
    if (state == 2) {
        if (MetatileBehavior_IsTallGrass(behavior) == TRUE
            || MetatileBehavior_IsVeryTallGrass(behavior) == TRUE
            || MetatileBehavior_IsTallGrass(prevBehavior) == TRUE
            || MetatileBehavior_IsVeryTallGrass(prevBehavior) == TRUE
            || MapObject_CheckWaterBehavior(object, behavior) == TRUE
            || MetatileBehavior_IsPuddle(behavior) == TRUE
            || MetatileBehavior_IsShallowWater(behavior) == TRUE
            || MapObject_CheckSnowBehavior(object, behavior) == TRUE
            || MetatileBehavior_IsMud(behavior) == TRUE
            || MetatileBehavior_IsReflective(behavior) != FALSE) {
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
            return;
        }
        if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK15) == 0) {
            FieldShadow_CreateFollowMonShadow(object);
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK15);
        }
    } else {
        if (IS_GROUND_EFFECT_TILE(object, behavior)) {
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
            return;
        }
        if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK15) == 0) {
            FieldShadow_CreateMapObjectShadow(object);
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK15);
        }
    }
}

#pragma force_active on

void MapObject_UpdateShadowVisibility(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u16 previous;
    if (!MapObjectManager_NotEndMovement(MapObject_GetManager(object))) {
        return;
    }
    if (flags->shadow == 0) {
        return;
    }
    if (flags->shadow == 2) {
        // called as returning a full word: the matched code narrows the result here
        previous = (u16)((u32(*)(LocalMapObject *))MapObject_GetSavedPreviousBehavior)(object);
        if (MetatileBehavior_IsTallGrass(behavior) == 1 || MetatileBehavior_IsVeryTallGrass(behavior) == 1 || MetatileBehavior_IsTallGrass((u8)previous) == 1 || MetatileBehavior_IsVeryTallGrass((u8)previous) == 1 || MapObject_CheckWaterBehavior(object, behavior) == 1 || MetatileBehavior_IsPuddle(behavior) == 1 || MetatileBehavior_IsShallowWater(behavior) == 1 || MapObject_CheckSnowBehavior(object, behavior) == 1 || MetatileBehavior_IsMud(behavior) == 1 || MetatileBehavior_IsReflective(behavior)) {
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
        } else {
            MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK20);
        }
        return;
    }
    if (MetatileBehavior_IsTallGrass(behavior) == 1 || MetatileBehavior_IsVeryTallGrass(behavior) == 1 || MapObject_CheckWaterBehavior(object, behavior) == 1 || MetatileBehavior_IsPuddle(behavior) == 1 || MetatileBehavior_IsShallowWater(behavior) == 1 || MapObject_CheckSnowBehavior(object, behavior) == 1 || MetatileBehavior_IsMud(behavior) == 1 || MetatileBehavior_IsReflective(behavior)) {
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
    } else {
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK20);
    }
}

#pragma force_active off

void MapObject_SpawnGroundDust(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (MapObject_CheckWaterBehavior(object, behavior) == TRUE
        || MetatileBehavior_IsShallowWater(behavior) == TRUE
        || MetatileBehavior_IsIce(behavior) == TRUE
        || MetatileBehavior_IsMud(behavior) == TRUE
        || MapObject_CheckSnowBehavior(object, behavior) == TRUE
        || MapObject_CheckVisible(object) != FALSE) {
        return;
    }
    GroundDust_Spawn(object);
}

void MapObject_SpawnVeryTallGrassStanding(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    int id = MapObject_GetID(object);
    if (MetatileBehavior_IsVeryTallGrass(behavior) == 1 && (id != 0xFD || MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != 1)) {
        VeryTallGrass_Spawn(object, 0);
    }
}

void MapObject_SpawnVeryTallGrassOnStep(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    int id = MapObject_GetID(object);
    if (MetatileBehavior_IsVeryTallGrass(behavior) == 1 && (id != 0xFD || MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != 1)) {
        VeryTallGrass_Spawn(object, 1);
    }
}

void MapObject_TileEffectDummy(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
}

void MapObject_TileEffectDummy2(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
}

void MapObject_SpawnPuddleSplashAtPrevious(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (MetatileBehavior_IsPuddle(prevBehavior) == 1) {
        int x = MapObject_GetPreviousXCoord(object);
        int y = MapObject_GetPreviousYCoord(object);
        int z = MapObject_GetPreviousZCoord(object);
        PuddleSplash_Spawn(object, x, y, z);
    }
}

void MapObject_SpawnPuddleSplash(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (MetatileBehavior_IsPuddle(behavior) == 1) {
        int x = MapObject_GetXCoord(object);
        int y = MapObject_GetYCoord(object);
        int z = MapObject_GetZCoord(object);
        PuddleSplash_Spawn(object, x, y, z);
    }
}

void MapObject_SpawnMudSplashAtPrevious(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (MetatileBehavior_IsMud(prevBehavior) == 1) {
        int x = MapObject_GetPreviousXCoord(object);
        int y = MapObject_GetPreviousYCoord(object);
        int z = MapObject_GetPreviousZCoord(object);
        MudSplash_Spawn(object, x, y, z);
    }
}

void MapObject_SpawnMudSplash(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (MetatileBehavior_IsMud(behavior) == 1) {
        int x = MapObject_GetXCoord(object);
        int y = MapObject_GetYCoord(object);
        int z = MapObject_GetZCoord(object);
        MudSplash_Spawn(object, x, y, z);
    }
}

void MapObject_CreateReflection(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u8 selected;

    if (flags->reflection != 0) {
        if (MapObject_CheckFlag24(object) != TRUE) {
            selected = GetMetatileBehavior_None();

            if (MapObject_GetID(object) == 0xFD) {
                if (FollowMonObject_IsLarge(object) != 0) {
                    u8 foundBehavior;
                    u8 first;
                    u8 second;
                    u8 third;
                    u8 fourth;
                    u8 fifth;
                    int found;
                    int kind;

                    if (MapObject_CheckFlag24(object) != FALSE) {
                        return;
                    }
                    if (MetatileBehavior_HasReflectiveSurface(behavior) == TRUE) {
                        selected = behavior;
                    } else {
                        first = MapObject_GetMetatileBehaviorInDirection(object, DIR_SOUTH);
                        second = MapObject_GetMetatileBehaviorInDirection(object, DIR_EAST);
                        third = MapObject_GetMetatileBehaviorInDirection(object, DIR_WEST);
                        fourth = MapObject_GetAdjacentTileBehavior(object, 4);
                        fifth = MapObject_GetAdjacentTileBehavior(object, 5);
                        found = FALSE;
                        if (MetatileBehavior_HasReflectiveSurface(first) == TRUE) {
                            foundBehavior = first;
                            found = TRUE;
                        } else if (MetatileBehavior_HasReflectiveSurface(second) == TRUE) {
                            foundBehavior = second;
                            found = TRUE;
                        } else if (MetatileBehavior_HasReflectiveSurface(third) == TRUE) {
                            foundBehavior = third;
                            found = TRUE;
                        } else if (MetatileBehavior_HasReflectiveSurface(fourth) == TRUE) {
                            foundBehavior = fourth;
                            found = TRUE;
                        } else if (MetatileBehavior_HasReflectiveSurface(fifth) == TRUE) {
                            foundBehavior = fifth;
                            found = TRUE;
                        }
                        if (found == TRUE) {
                            selected = foundBehavior;
                        }
                    }
                    if (selected == GetMetatileBehavior_None()) {
                        return;
                    }
                    MapObject_SetFlag24(object, TRUE);
                    if (MetatileBehavior_IsReflective(selected) == TRUE) {
                        kind = 5;
                    } else if (MetatileBehavior_IsPuddle(selected) == TRUE) {
                        kind = 3;
                    } else {
                        kind = 4;
                    }
                    Reflection_SpawnForFollower(object, kind);
                } else {
                    int kind;
                    if (MapObject_CheckFlag24(object) != FALSE) {
                        return;
                    }
                    if (MetatileBehavior_HasReflectiveSurface(behavior) == TRUE) {
                        selected = behavior;
                    } else {
                        u8 adjacent = MapObject_GetMetatileBehaviorInDirection(object, DIR_SOUTH);
                        if (MetatileBehavior_HasReflectiveSurface(adjacent) == TRUE) {
                            selected = adjacent;
                        }
                    }
                    if (selected == GetMetatileBehavior_None()) {
                        return;
                    }
                    MapObject_SetFlag24(object, TRUE);
                    if (MetatileBehavior_IsReflective(selected) == TRUE) {
                        kind = 2;
                    } else if (MetatileBehavior_IsPuddle(selected) == TRUE) {
                        kind = 0;
                    } else {
                        kind = 1;
                    }
                    Reflection_SpawnForFollower(object, kind);
                }
            } else {
                int kind;
                if (MapObject_CheckFlag24(object) != FALSE) {
                    return;
                }
                if (MetatileBehavior_HasReflectiveSurface(behavior) == TRUE) {
                    selected = behavior;
                } else {
                    u8 adjacent = MapObject_GetMetatileBehaviorInDirection(object, DIR_SOUTH);
                    if (MetatileBehavior_HasReflectiveSurface(adjacent) == TRUE) {
                        selected = adjacent;
                    }
                }
                if (selected == GetMetatileBehavior_None()) {
                    return;
                }
                MapObject_SetFlag24(object, TRUE);
                if (MetatileBehavior_IsReflective(selected) == TRUE) {
                    kind = 2;
                } else if (MetatileBehavior_IsPuddle(selected) == TRUE) {
                    kind = 0;
                } else {
                    kind = 1;
                }
                Reflection_Spawn(object, kind);
            }
        }
    }
}

void MapObject_UpdateReflection(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (flags->reflection == 0 || MapObject_CheckFlag24(object) == FALSE) {
        return;
    }
    if (MapObject_GetID(object) == 0xFD && FollowMonObject_IsLarge(object) != 0) {
        u8 first = MapObject_GetMetatileBehaviorInDirection(object, DIR_SOUTH);
        u8 second = MapObject_GetMetatileBehaviorInDirection(object, DIR_EAST);
        u8 third = MapObject_GetMetatileBehaviorInDirection(object, DIR_WEST);
        u8 fourth = MapObject_GetAdjacentTileBehavior(object, 4);
        u8 fifth = MapObject_GetAdjacentTileBehavior(object, 5);
        BOOL found = FALSE;

        if (MetatileBehavior_HasReflectiveSurface(first) == TRUE) {
            found = TRUE;
        } else if (MetatileBehavior_HasReflectiveSurface(second) == TRUE) {
            found = TRUE;
        } else if (MetatileBehavior_HasReflectiveSurface(third) == TRUE) {
            found = TRUE;
        } else if (MetatileBehavior_HasReflectiveSurface(fourth) == TRUE) {
            found = TRUE;
        } else if (MetatileBehavior_HasReflectiveSurface(fifth) == TRUE) {
            found = TRUE;
        }
        if (found == FALSE) {
            MapObject_SetFlag24(object, FALSE);
        }
    } else {
        u8 adjacent = MapObject_GetMetatileBehaviorInDirection(object, DIR_SOUTH);
        if (MetatileBehavior_HasReflectiveSurface(adjacent) == FALSE) {
            MapObject_SetFlag24(object, FALSE);
        }
    }
}

void MapObject_UpdateOnBridgeFlag(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (MetatileBehavior_IsBridgeStart(behavior) == 1) {
        MapObject_SetFlag28(object, TRUE);
    } else if (MapObject_CheckFlag28(object) == 1 && MetatileBehavior_IsBridge(behavior) == 0) {
        MapObject_SetFlag28(object, FALSE);
    }
}

void MapObject_TileEffectDummy3(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
}

u32 MapObject_CheckCollision(LocalMapObject *object, VecFx32 *position, int x, int y, int z, int direction) {
    u32 result = 0;
    s8 slope;

    if (MapObject_IsOutsideMovementRange(object, x, y, z) == TRUE) {
        result |= 1;
    }
    if (FieldSystem_CheckCollisionAndHeight(MapObject_GetFieldSystem(object), position, x, z, (int)&slope) == TRUE) {
        result |= 2;
        if (slope != 0) {
            result |= 8;
        }
    }
    if (MapObject_IsMoveBlockedByBehavior(object, x, z, direction) == TRUE) {
        result |= 2;
    }
    if (Comm_IsUnionRoomLinkMode(CommState_GetLinkMode())) {
        if (MapObject_IsTileOccupiedInLink(object, x, y, z) == TRUE) {
            result |= 4;
        }
    } else {
        if (MapObject_IsTileOccupied(object, x, y, z) == TRUE) {
            result |= 4;
        }
    }
    return result;
}

u32 MapObject_CheckCollisionAt(LocalMapObject *object, int x, int y, int z, int direction) {
    VecFx32 position;
    MapObject_CopyPositionVector(object, &position);
    return MapObject_CheckCollision(object, &position, x, y, z, direction);
}

u32 MapObject_CheckCollisionInDirection(LocalMapObject *object, int direction) {
    int x = MapObject_GetXCoord(object) + GetDeltaXByFacingDirection(direction);
    int y = MapObject_GetYCoord(object);
    int z = MapObject_GetZCoord(object) + GetDeltaYByFacingDirection(direction);
    return MapObject_CheckCollisionAt(object, x, y, z, direction);
}
