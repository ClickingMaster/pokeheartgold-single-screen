#include "global.h"

#include "constants/flags.h"
#include "constants/flypoints.h"
#include "constants/global_fieldmap.h"
#include "constants/maps.h"

#include "field_system.h"
#include "spawn_warp_lookup_internal.h"
#include "sys_flags.h"

#define NUM_SPAWN_POINTS 30

// Where the player reappears after a blackout (inside the Pokemon Center), lands when flying there, and the special
// spawn warp recorded for it; flypoint is the flypoint flag that Fly checks.
typedef struct SpawnPoint {
    u16 flypoint : 8;
    u16 isBlackoutSpawn : 1;
    u16 isFlyPoint : 1;
    u16 unused : 6;
    u16 deathMap;
    u16 deathX : 8;
    u16 deathY : 8;
    u16 flyMap;
    u16 flyX;
    u16 flyY;
    u16 specialMap;
    u16 specialX;
    u16 specialY;
} SpawnPoint;

static const SpawnPoint sSpawnPoints[NUM_SPAWN_POINTS] = {
    { FLYPOINT_NEW_BARK,     TRUE,  TRUE,  0, MAP_NEW_BARK_PLAYER_HOUSE_1F,               6, 8,  MAP_NEW_BARK,                        695,  397, MAP_NEW_BARK,         695,  397 },
    { FLYPOINT_CHERRYGROVE,  TRUE,  TRUE,  0, MAP_CHERRYGROVE_POKECENTER_1F,              8, 13, MAP_CHERRYGROVE,                     564,  392, MAP_CHERRYGROVE,      564,  392 },
    { FLYPOINT_VIOLET,       TRUE,  TRUE,  0, MAP_VIOLET_POKECENTER_1F,                   8, 13, MAP_VIOLET,                          497,  272, MAP_VIOLET,           497,  272 },
    { FLYPOINT_AZALEA,       TRUE,  TRUE,  0, MAP_AZALEA_POKECENTER_1F,                   8, 13, MAP_AZALEA,                          410,  461, MAP_AZALEA,           410,  461 },
    { FLYPOINT_GOLDENROD,    TRUE,  TRUE,  0, MAP_CIANWOOD_POKECENTER_1F,                 8, 13, MAP_CIANWOOD,                        187,  370, MAP_CIANWOOD,         187,  370 },
    { FLYPOINT_ECRUTEAK,     TRUE,  TRUE,  0, MAP_GOLDENROD_POKECENTER_1F,                8, 13, MAP_GOLDENROD,                       352,  369, MAP_GOLDENROD,        352,  369 },
    { FLYPOINT_OLIVINE,      TRUE,  TRUE,  0, MAP_OLIVINE_POKECENTER_1F,                  8, 13, MAP_OLIVINE,                         272,  258, MAP_OLIVINE,          272,  258 },
    { FLYPOINT_CIANWOOD,     TRUE,  TRUE,  0, MAP_ECRUTEAK_POKECENTER_1F,                 8, 13, MAP_ECRUTEAK,                        397,  184, MAP_ECRUTEAK,         397,  184 },
    { FLYPOINT_MAHOGANY,     TRUE,  TRUE,  0, MAP_MAHOGANY_POKECENTER_1F,                 8, 13, MAP_MAHOGANY,                        534,  184, MAP_MAHOGANY,         534,  184 },
    { FLYPOINT_LAKE_OF_RAGE, FALSE, TRUE,  0, MAP_LAKE_OF_RAGE,                           8, 13, MAP_LAKE_OF_RAGE,                    536,  90,  MAP_LAKE_OF_RAGE,     536,  90  },
    { FLYPOINT_BLACKTHORN,   TRUE,  TRUE,  0, MAP_BLACKTHORN_POKECENTER_1F,               8, 13, MAP_BLACKTHORN,                      674,  177, MAP_BLACKTHORN,       674,  177 },
    { FLYPOINT_MT_SILVER,    TRUE,  TRUE,  0, MAP_MOUNT_SILVER_POKECENTER_1F,             8, 13, MAP_MOUNT_SILVER,                    820,  266, MAP_MOUNT_SILVER,     820,  266 },
    { FLYPOINT_PALLET,       FALSE, TRUE,  0, MAP_PALLET,                                 8, 13, MAP_PALLET,                          1033, 364, MAP_PALLET,           1033, 364 },
    { FLYPOINT_VIRIDIAN,     TRUE,  TRUE,  0, MAP_VIRIDIAN_POKECENTER_1F,                 8, 13, MAP_VIRIDIAN,                        1032, 263, MAP_VIRIDIAN,         1032, 263 },
    { FLYPOINT_PEWTER,       TRUE,  TRUE,  0, MAP_PEWTER_POKECENTER_1F,                   8, 13, MAP_PEWTER,                          1048, 107, MAP_PEWTER,           1048, 107 },
    { FLYPOINT_CERULEAN,     TRUE,  TRUE,  0, MAP_CERULEAN_POKECENTER_1F,                 8, 13, MAP_CERULEAN,                        1309, 132, MAP_CERULEAN,         1309, 132 },
    { FLYPOINT_LAVENDER,     TRUE,  TRUE,  0, MAP_LAVENDER_POKECENTER_1F,                 8, 13, MAP_LAVENDER,                        1418, 235, MAP_LAVENDER,         1418, 235 },
    { FLYPOINT_VERMILION,    TRUE,  TRUE,  0, MAP_VERMILION_POKECENTER_1F,                8, 13, MAP_VERMILION,                       1297, 295, MAP_VERMILION,        1297, 295 },
    { FLYPOINT_CELADON,      TRUE,  TRUE,  0, MAP_CELADON_POKECENTER_1F,                  8, 13, MAP_CELADON,                         1231, 238, MAP_CELADON,          1231, 238 },
    { FLYPOINT_FUCHSIA,      TRUE,  TRUE,  0, MAP_FUCHSIA_POKECENTER_1F,                  8, 13, MAP_FUCHSIA,                         1209, 440, MAP_FUCHSIA,          1209, 440 },
    { FLYPOINT_CINNABAR,     FALSE, TRUE,  0, MAP_CINNABAR_ISLAND_POKECENTER_1F,          8, 13, MAP_CINNABAR_ISLAND,                 1039, 503, MAP_CINNABAR_ISLAND,  1039, 503 },
    { FLYPOINT_INDIGO,       TRUE,  TRUE,  0, MAP_POKEMON_LEAGUE_ENTRANCE,                6, 21, MAP_INDIGO_PLATEAU,                  912,  201, MAP_INDIGO_PLATEAU,   912,  201 },
    { FLYPOINT_SAFFRON,      TRUE,  TRUE,  0, MAP_SAFFRON_POKECENTER_1F,                  8, 13, MAP_SAFFRON,                         1294, 243, MAP_SAFFRON,          1294, 243 },
    { FLYPOINT_SAFARI,       TRUE,  TRUE,  0, MAP_SAFARI_ZONE_GATE_POKECENTER_1F,         8, 13, MAP_SAFARI_ZONE_GATE,                82,   303, MAP_SAFARI_ZONE_GATE, 82,   303 },
    { FLYPOINT_FRONTIER,     TRUE,  TRUE,  0, MAP_FRONTIER_ACCESS_POKECENTER_1F,          8, 13, MAP_BATTLE_FRONTIER_FRONTIER_ACCESS, 8,    15,  MAP_ROUTE_40,         237,  267 },
    { FLYPOINT_POKEATHLON,   FALSE, FALSE, 0, MAP_POKEATHLON_DOME,                        8, 13, MAP_POKEATHLON_DOME,                 42,   23,  MAP_ROUTE_35,         362,  267 },
    { FLYPOINT_VICTORY_ROAD, FALSE, FALSE, 0, MAP_ROUTE_22_POKEMON_LEAGUE_RECEPTION_GATE, 8, 13, MAP_ROUTE_26,                        909,  297, MAP_ROUTE_26,         909,  297 },
    { FLYPOINT_UNION_CAVE,   TRUE,  FALSE, 0, MAP_ROUTE_32_POKECENTER_1F,                 8, 13, MAP_ROUTE_32,                        468,  419, MAP_ROUTE_32,         468,  419 },
    { FLYPOINT_MT_MOON,      TRUE,  FALSE, 0, MAP_ROUTE_3_POKECENTER_1F,                  8, 13, MAP_ROUTE_3,                         1167, 107, MAP_ROUTE_3,          1167, 107 },
    { FLYPOINT_ROCK_TUNNEL,  TRUE,  FALSE, 0, MAP_ROUTE_10_POKECENTER_1F,                 8, 13, MAP_ROUTE_10,                        1426, 164, MAP_ROUTE_10,         1426, 164 },
};

static s32 SpawnIdToTableIndex(s32 spawnId) {
    if (spawnId <= 0 || (u32)spawnId > NUM_SPAWN_POINTS) {
        GF_ASSERT(FALSE);
        spawnId = 1;
    }
    return spawnId - 1;
}

u32 GetMomSpawnId(void) {
    return 1;
}

void GetFlyWarpData(s32 spawnId, Location *warp) {
    s32 index = SpawnIdToTableIndex(spawnId);
    warp->mapId = sSpawnPoints[index].flyMap;
    warp->warpId = -1;
    warp->x = sSpawnPoints[index].flyX;
    warp->y = sSpawnPoints[index].flyY;
    warp->direction = DIR_SOUTH;
}

void GetDeathWarpData(s32 spawnId, Location *warp) {
    s32 index = SpawnIdToTableIndex(spawnId);
    warp->mapId = sSpawnPoints[index].deathMap;
    warp->warpId = -1;
    warp->x = sSpawnPoints[index].deathX;
    warp->y = sSpawnPoints[index].deathY;
    warp->direction = DIR_NORTH;
}

void GetSpecialSpawnWarpData(s32 spawnId, Location *warp) {
    s32 index = SpawnIdToTableIndex(spawnId);
    warp->mapId = sSpawnPoints[index].specialMap;
    warp->warpId = -1;
    warp->x = sSpawnPoints[index].specialX;
    warp->y = sSpawnPoints[index].specialY;
    warp->direction = DIR_SOUTH;
}

u32 MapHeader_GetSpawnIdForDeathWarp(u32 mapId) {
    const SpawnPoint *spawnPoints = sSpawnPoints;
    u32 i;
    for (i = 0; i < NUM_SPAWN_POINTS; i++) {
        if (spawnPoints[i].deathMap == mapId && spawnPoints[i].isBlackoutSpawn) {
            return i + 1;
        }
    }
    return 0;
}

u32 GetSpawnIdForFlyMap(u32 mapId) {
    u32 i;
    for (i = 0; i < NUM_SPAWN_POINTS; i++) {
        if (sSpawnPoints[i].flyMap == mapId) {
            return i + 1;
        }
    }
    return 0;
}

// Entering a town marks it as a place to fly to.
void FlypointFlagAction(FieldSystem *fieldSystem, u32 mapId) {
    u32 i;
    for (i = 0; i < NUM_SPAWN_POINTS; i++) {
        if (sSpawnPoints[i].flyMap == mapId && sSpawnPoints[i].isFlyPoint) {
            SaveVarsFlags *varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
            Save_VarsFlags_FlypointFlagAction(varsFlags, FLAG_ACTION_SET, sSpawnPoints[i].flypoint);
            return;
        }
    }
}
