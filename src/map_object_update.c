#include "global.h"

#include "field_system.h"
#include "map_object.h"
#include "map_object_movement_utilities_internal.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "unk_02033AE0.h"
#include "unk_02037C94.h"
#include "unk_02054648.h"

// Defined elsewhere without a header yet.
void sub_02062400(LocalMapObject *object);
void sub_02063A14(LocalMapObject *object);
BOOL sub_02063A1C(LocalMapObject *object);
const MapObjectEffectFlags *ov01_021F9318(LocalMapObject *object);
u32 ov01_022056C4(LocalMapObject *object, u32 direction);
void Reflection_Spawn(LocalMapObject *object, int kind);
void Reflection_SpawnForFollower(LocalMapObject *object, int kind);
void TallGrass_Spawn(LocalMapObject *object, int mode);
void VeryTallGrass_Spawn(LocalMapObject *object, int mode);
void PuddleSplash_Spawn(LocalMapObject *object, int x, int y, int z);
void MudSplash_Spawn(LocalMapObject *object, int x, int y, int z);
void ShallowWaterRipple_Spawn(LocalMapObject *object, int value);
void ov01_021FE3DC(LocalMapObject *object);
void ov01_021FE3E8(LocalMapObject *object);
void ov01_021FE3F4(LocalMapObject *object);
void Footprints_SpawnInSand(LocalMapObject *object);
void Footprints_SpawnTracksInSand(LocalMapObject *object);
void FieldShadow_CreateMapObjectShadow(LocalMapObject *object);
void ov01_021FD684(LocalMapObject *object);
void GroundDust_Spawn(LocalMapObject *object);

void sub_0205FD20(LocalMapObject *object) {
    sub_0205F41C(object);
    sub_02063A14(object);
}

void sub_0205FD30(LocalMapObject *object) {
    if (sub_0205F5E8(object, MAPOBJECTFLAG_SINGLE_MOVEMENT) == 0) {
        sub_0205FE0C(object);
        sub_0205FE24(object);
        sub_0205FE48(object);
        if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK4)) {
            sub_02062400(object);
        } else if (!MapObject_CheckMovementPaused(object) && sub_0205FD98(object) == 1 && !sub_02063A1C(object)) {
            sub_0205F430(object);
        }
        sub_0205FE6C(object);
        sub_0205FEA4(object);
    }
}

BOOL sub_0205FD98(LocalMapObject *object) {
    u32 flags;
    if (MapObject_CheckSingleMovement(object) == 1) {
        return 1;
    }
    if (!MapObject_GetFlagsBitsMask(object, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK11 | MAPOBJECTFLAG_UNK12))) {
        return 1;
    }
    if (MapObject_GetMovement(object) == 0x32) {
        return 1;
    }
    if (MapObject_GetMovement(object) == 0x30) {
        return 1;
    }
    flags = MapObject_GetFlags(object);
    if ((flags & (1 << 12)) && !(flags & (1 << 23))) {
        return 0;
    }
    if ((flags & (2 << 10)) && !sub_0205F8D0(object)) {
        return 0;
    }
    return 1;
}

void sub_0205FE0C(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK12)) {
        MapObject_UpdateHeight(object);
    }
}

void sub_0205FE24(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK11) && MapObject_UpdateMetatileBehaviors(object) == 1) {
        MapObject_SetStartMovement(object);
    }
}

void sub_0205FE48(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_START_MOVEMENT)) {
        sub_0205FEDC(object);
    }
    MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_START_MOVEMENT | MAPOBJECTFLAG_UNK16));
}

void sub_0205FE6C(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK16)) {
        sub_02060020(object);
    } else if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_START_MOVEMENT)) {
        sub_0205FF6C(object);
    }
    MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_START_MOVEMENT | MAPOBJECTFLAG_UNK16));
}

void sub_0205FEA4(LocalMapObject *object) {
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK17)) {
        sub_02060114(object);
    } else if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_END_MOVEMENT)) {
        sub_0206008C(object);
    }
    MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_END_MOVEMENT | MAPOBJECTFLAG_UNK17));
}

const TileTestFunc gMetatileBlocksExitTests[4] = { sub_0205B8F4, sub_0205B918, sub_0205B93C, sub_0205B960 };
const TileTestFunc gMetatileBlocksEntryTests[4] = { sub_0205B918, sub_0205B8F4, sub_0205B960, sub_0205B93C };
const s32 gDirectionDeltaX[4] = { 0, 0, -1, 1 };
const s32 gDirectionDeltaY[4] = { -1, 1, 0, 0 };
const s32 gOppositeDirections[4] = { 1, 0, 3, 2 };

void sub_0205FEDC(LocalMapObject *object) {
    u8 previous;
    u8 current;
    const MapObjectEffectFlags *effect;
    MapObject_UpdateMetatileBehaviors(object);
    if (sub_0205F73C(object) == 1) {
        previous = sub_0205F504(object);
        current = sub_0205F514(object);
        effect = ov01_021F9318(object);
        sub_02060AB8(object, previous, current, effect);
        sub_020601BC(object, previous, current, effect);
        MapObject_UpdateShallowWaterRipple(object, previous, current, effect);
        sub_020603DC(object, previous, current, effect);
        sub_020601A4(object, previous, current, effect);
        sub_02060698(object, previous, current, effect);
        sub_02060700(object, previous, current, effect);
        MapObject_CreateReflection(object, previous, current, effect);
    }
}

void sub_0205FF6C(LocalMapObject *object) {
    u8 previous;
    u8 current;
    const MapObjectEffectFlags *effect;
    MapObject_UpdateMetatileBehaviors(object);
    if (sub_0205F73C(object) == 1) {
        previous = sub_0205F504(object);
        current = sub_0205F514(object);
        effect = ov01_021F9318(object);
        sub_02060AB8(object, previous, current, effect);
        sub_02060274(object, previous, current, effect);
        MapObject_SpawnFootprints(object, previous, current, effect);
        MapObject_UpdateShallowWaterRipple(object, previous, current, effect);
        sub_020603F8(object, previous, current, effect);
        sub_020606CC(object, previous, current, effect);
        sub_02060704(object, previous, current, effect);
        MapObject_SpawnPuddleSplashAtPrevious(object, previous, current, effect);
        MapObject_SpawnMudSplashAtPrevious(object, previous, current, effect);
        MapObject_CreateReflection(object, previous, current, effect);
        sub_02060AF0(object, previous, current, effect);
    }
}

void sub_02060020(LocalMapObject *object) {
    MapObject_UpdateMetatileBehaviors(object);
    if (sub_0205F73C(object) == 1) {
        u8 previous = sub_0205F504(object);
        u8 current = sub_0205F514(object);
        const MapObjectEffectFlags *effect = ov01_021F9318(object);
        sub_02060AB8(object, previous, current, effect);
        sub_020603F8(object, previous, current, effect);
        MapObject_CreateReflection(object, previous, current, effect);
        MapObject_ResetShallowWaterRipple(object, previous, current, effect);
        sub_02060AF0(object, previous, current, effect);
    }
}

void sub_0206008C(LocalMapObject *object) {
    u8 value = sub_0205F514(object);
    sub_0205F51C(object, value);
    MapObject_UpdateMetatileBehaviors(object);
    if (sub_0205F73C(object) == TRUE) {
        u8 a = sub_0205F504(object);
        u8 b = sub_0205F514(object);
        const MapObjectEffectFlags *effect = ov01_021F9318(object);
        sub_020601A4(object, a, b, effect);
        MapObject_SpawnPuddleSplash(object, a, b, effect);
        MapObject_SpawnMudSplash(object, a, b, effect);
        MapObject_UpdateShallowWaterRipple(object, a, b, effect);
        MapObject_UpdateReflection(object, a, b, effect);
        sub_02060530(object, a, b, effect);
    }
}

void sub_02060114(LocalMapObject *object) {
    MapObject_UpdateMetatileBehaviors(object);
    if (sub_0205F73C(object) == TRUE) {
        u8 a = sub_0205F504(object);
        u8 b = sub_0205F514(object);
        const MapObjectEffectFlags *effect = ov01_021F9318(object);
        sub_020601A4(object, a, b, effect);
        MapObject_SpawnPuddleSplash(object, a, b, effect);
        MapObject_SpawnMudSplash(object, a, b, effect);
        MapObject_UpdateShallowWaterRipple(object, a, b, effect);
        MapObject_UpdateReflection(object, a, b, effect);
        sub_02060530(object, a, b, effect);
        sub_02060274(object, a, b, effect);
        MapObject_SpawnGroundDust(object, a, b, effect);
    }
}

void sub_020601A4(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    VecFx32 position = { 0, 0, 0 };
    sub_0205F9C0(object, &position);
}

void sub_020601BC(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
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

void sub_02060274(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
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
    if (sub_0205BA94(prevBehavior) == TRUE) {
        kind = flags->footprints;
        if (kind == 1) {
            ov01_021FE3E8(object);
        } else if (kind == 2) {
            ov01_021FE3F4(object);
        }
    }
    if (sub_02060EA4(object, prevBehavior) == TRUE) {
        kind = flags->footprints;
        if (kind == 1) {
            Footprints_SpawnInSand(object);
            return;
        } else if (kind == 2) {
            Footprints_SpawnTracksInSand(object);
            return;
        }
    } else if (sub_02060ED4(object, prevBehavior) == TRUE) {
        ov01_021FE3DC(object);
    }
}

void MapObject_UpdateShallowWaterRipple(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (MetatileBehavior_IsShallowWater(behavior) == 1) {
        if (MapObject_CheckFlag26(object) == 0) {
            ShallowWaterRipple_Spawn(object, 1);
            MapObject_SetFlag26(object, 1);
        }
    } else {
        MapObject_SetFlag26(object, 0);
    }
}

void MapObject_ResetShallowWaterRipple(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    MapObject_SetFlag26(object, 0);
}

void sub_020603DC(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u8 value = sub_0205F524(object);
    sub_020603F8(object, behavior, value, flags);
}

#define IS_GROUND_EFFECT_TILE(object, behavior)                                                           \
    (MetatileBehavior_IsTallGrass(behavior) == TRUE || MetatileBehavior_IsVeryTallGrass(behavior) == TRUE \
        || sub_02060E54(object, behavior) == TRUE || MetatileBehavior_IsPuddle(behavior) == TRUE          \
        || MetatileBehavior_IsShallowWater(behavior) == TRUE || sub_02060EBC(object, behavior) == TRUE    \
        || MetatileBehavior_IsMud(behavior) == TRUE || MetatileBehavior_IsReflective(behavior) != FALSE)

void sub_020603F8(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
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
            || sub_02060E54(object, behavior) == TRUE
            || MetatileBehavior_IsPuddle(behavior) == TRUE
            || MetatileBehavior_IsShallowWater(behavior) == TRUE
            || sub_02060EBC(object, behavior) == TRUE
            || MetatileBehavior_IsMud(behavior) == TRUE
            || MetatileBehavior_IsReflective(behavior) != FALSE) {
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
            return;
        }
        if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK15) == 0) {
            ov01_021FD684(object);
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

void sub_02060530(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    u16 previous;
    if (!MapObjectManager_NotEndMovement(MapObject_GetManager(object))) {
        return;
    }
    if (flags->shadow == 0) {
        return;
    }
    if (flags->shadow == 2) {
        // called as returning a full word: the matched code narrows the result here
        previous = (u16)((u32(*)(LocalMapObject *))sub_0205F524)(object);
        if (MetatileBehavior_IsTallGrass(behavior) == 1 || MetatileBehavior_IsVeryTallGrass(behavior) == 1 || MetatileBehavior_IsTallGrass((u8)previous) == 1 || MetatileBehavior_IsVeryTallGrass((u8)previous) == 1 || sub_02060E54(object, behavior) == 1 || MetatileBehavior_IsPuddle(behavior) == 1 || MetatileBehavior_IsShallowWater(behavior) == 1 || sub_02060EBC(object, behavior) == 1 || MetatileBehavior_IsMud(behavior) == 1 || MetatileBehavior_IsReflective(behavior)) {
            MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
        } else {
            MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK20);
        }
        return;
    }
    if (MetatileBehavior_IsTallGrass(behavior) == 1 || MetatileBehavior_IsVeryTallGrass(behavior) == 1 || sub_02060E54(object, behavior) == 1 || MetatileBehavior_IsPuddle(behavior) == 1 || MetatileBehavior_IsShallowWater(behavior) == 1 || sub_02060EBC(object, behavior) == 1 || MetatileBehavior_IsMud(behavior) == 1 || MetatileBehavior_IsReflective(behavior)) {
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK20);
    } else {
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK20);
    }
}

#pragma force_active off

void MapObject_SpawnGroundDust(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (sub_02060E54(object, behavior) == TRUE
        || MetatileBehavior_IsShallowWater(behavior) == TRUE
        || MetatileBehavior_IsIce(behavior) == TRUE
        || MetatileBehavior_IsMud(behavior) == TRUE
        || sub_02060EBC(object, behavior) == TRUE
        || MapObject_CheckVisible(object) != FALSE) {
        return;
    }
    GroundDust_Spawn(object);
}

void sub_02060698(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    int id = MapObject_GetID(object);
    if (MetatileBehavior_IsVeryTallGrass(behavior) == 1 && (id != 0xFD || MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != 1)) {
        VeryTallGrass_Spawn(object, 0);
    }
}

void sub_020606CC(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    int id = MapObject_GetID(object);
    if (MetatileBehavior_IsVeryTallGrass(behavior) == 1 && (id != 0xFD || MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) != 1)) {
        VeryTallGrass_Spawn(object, 1);
    }
}

void sub_02060700(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
}

void sub_02060704(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
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
                        first = MapObject_GetMetatileBehaviorInDirection(object, 1);
                        second = MapObject_GetMetatileBehaviorInDirection(object, 3);
                        third = MapObject_GetMetatileBehaviorInDirection(object, 2);
                        fourth = ov01_022056C4(object, 4);
                        fifth = ov01_022056C4(object, 5);
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
                        u8 adjacent = MapObject_GetMetatileBehaviorInDirection(object, 1);
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
                    u8 adjacent = MapObject_GetMetatileBehaviorInDirection(object, 1);
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
        u8 first = MapObject_GetMetatileBehaviorInDirection(object, 1);
        u8 second = MapObject_GetMetatileBehaviorInDirection(object, 3);
        u8 third = MapObject_GetMetatileBehaviorInDirection(object, 2);
        u8 fourth = ov01_022056C4(object, 4);
        u8 fifth = ov01_022056C4(object, 5);
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
        u8 adjacent = MapObject_GetMetatileBehaviorInDirection(object, 1);
        if (MetatileBehavior_HasReflectiveSurface(adjacent) == FALSE) {
            MapObject_SetFlag24(object, FALSE);
        }
    }
}

void sub_02060AB8(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
    if (sub_0205BA24(behavior) == 1) {
        MapObject_SetFlag28(object, 1);
    } else if (MapObject_CheckFlag28(object) == 1 && sub_0205BA30(behavior) == 0) {
        MapObject_SetFlag28(object, 0);
    }
}

void sub_02060AF0(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags) {
}

u32 MapObject_CheckCollision(LocalMapObject *object, VecFx32 *position, int x, int y, int z, int direction) {
    u32 result = 0;
    s8 slope;

    if (MapObject_IsOutsideMovementRange(object, x, y, z) == TRUE) {
        result |= 1;
    }
    if (sub_020549A8(MapObject_GetFieldSystem(object), position, x, z, (int)&slope) == TRUE) {
        result |= 2;
        if (slope != 0) {
            result |= 8;
        }
    }
    if (MapObject_IsMoveBlockedByBehavior(object, x, z, direction) == TRUE) {
        result |= 2;
    }
    if (sub_0203401C(sub_0203993C())) {
        if (sub_02060BFC(object, x, y, z) == TRUE) {
            result |= 4;
        }
    } else {
        if (sub_02060CA8(object, x, y, z) == TRUE) {
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
