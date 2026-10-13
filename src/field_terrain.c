#include "field_terrain.h"

#include "global.h"

#include "field/map_load_manager.h"
#include "field/map_prop.h"

#include "field_system.h"
#include "gymmick_collision.h"
#include "heap.h"
#include "map_matrix.h"
#include "terrain_attributes.h"

// Field terrain queries (ARM9 static 0x02054648-0x02054E40). FieldSystem.terrainQueries points at one of two query tables,
// chosen by the map's useSimpleTerrainCollisions: the BDHC one reads heights from the loaded map chunks' BDHC data
// (and the dynamic height plates) and attributes from the loaded chunks, the simple one has no heights and reads
// attributes from the whole map's TerrainAttributes. Attribute bits: 0-7 metatile behavior, 15 collision.
// Also searches the loaded map chunks' props by build model.

typedef struct FieldTerrainQueries {
    int (*getHeight)(FieldSystem *fieldSystem, int height, fx32 x, fx32 z, u8 *outSource);
    BOOL (*getAttributes)(FieldSystem *fieldSystem, int x, int z, u16 *outAttributes);
} FieldTerrainQueries;

#define FS_TERRAIN_QUERIES(fieldSystem) ((const FieldTerrainQueries *)(fieldSystem)->terrainQueries)

extern const u16 _020FC5FC[];
extern const FieldTerrainQueries gTerrainQueries_Bdhc;
extern const u16 _020FC60C[];
extern const FieldTerrainQueries gTerrainQueries_Simple;

int Terrain_AbsDiff(int lhs, int rhs);
int FieldSystem_GetHeightStep(FieldSystem *fieldSystem, VecFx32 *playerPos, int x, int z, u8 *outKind);

extern int TileIndex_GetQuadrant(int idx, int stride);
extern int MapLoadManager_GetMapSlotOfMatrixIndex(int chunkIdx, int a1, MapLoadManager *mapLoadManager);
extern void *MapLoadManager_GetBdhc(MapLoadManager *mapLoadManager, u8 idx);
extern int Bdhc_GetHeight(int a0, int a1, fx32 x, fx32 z, void *a4, fx32 *outY);
int Terrain_CalcHeight(FieldSystem *fieldSystem, int objectHeightMode, int objectHeight, fx32 x, fx32 z, u8 *outKind);
int Terrain_GetHeight_Bdhc(FieldSystem *fieldSystem, int objectHeight, fx32 x, fx32 z, u8 *outKind);
int Terrain_CalcHeightWithMode(FieldSystem *fieldSystem, int objectHeightMode, int objectHeight, fx32 x, fx32 z, u8 *outKind);
int Terrain_GetHeight_Simple(FieldSystem *fieldSystem, int objectHeight, fx32 x, fx32 z, u8 *outKind);
BOOL Terrain_GetAttributes_Loaded(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
BOOL Terrain_GetAttributes_Simple(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
u8 sub_020548EC(FieldSystem *fieldSystem, int x, int z);
int FieldSystem_GetTerrainHeight(FieldSystem *fieldSystem, int objectHeight, fx32 x, fx32 z, u8 *outKind);

extern BOOL MapLoadManager_GetMapSlotAt(MapLoadManager *mapLoadManager, int x, int z, u8 *outIdx);
extern SoundplateStruct *MapLoadManager_GetSoundplates(MapLoadManager *mapLoadManager, u8 idx);
extern int MapLoadManager_GetMapMatrixIndex(MapLoadManager *mapLoadManager, u8 idx);
u32 FieldSystem_CheckCollisionHeightAndGimmick(FieldSystem *fieldSystem, VecFx32 *playerPos, u32 x, u32 y, u32 *outSlope);
void Terrain_MakeTileRect(int x, int z, int offsetX, int offsetZ, int width, int depth, fx32 *out);
BOOL MapProp_IsInRect(MapProp *mapProp, fx32 *rect, VecFx32 *offset);
BOOL FieldSystem_FindMapPropInRect(FieldSystem *fieldSystem, int buildModel, fx32 *rect, MapProp **outProp);
BOOL FieldSystem_FindMapPropOfModelsInRect(FieldSystem *fieldSystem, int *models, u32 count, fx32 *rect, MapProp **outProp, int *outModel);
BOOL FieldSystem_FindMapProp(FieldSystem *fieldSystem, int buildModel, MapProp **outProp, int *outIdx);
BOOL FieldSystem_FindMapPropOfModels(FieldSystem *fieldSystem, int *models, u32 count, MapProp **outProp, int *outModel);
int *FieldSystem_CollectMapPropModelsInRect(FieldSystem *fieldSystem, u32 heapId, int capacity, fx32 *rect, u32 fill);
void Terrain_GetChunkOrigin(int idx, int width, VecFx32 *pos);
BOOL sub_02054E00(u16 a0);
BOOL sub_02054E20(u16 a0);

int Terrain_AbsDiff(int lhs, int rhs) {
    if (lhs >= rhs) {
        return lhs - rhs;
    }
    return rhs - lhs;
}

#define MAP_TILES_COUNT_X    32
#define MAP_TILES_COUNT_Z    32
#define MAP_OBJECT_TILE_SIZE FX32_CONST(16)
#define QUADRANT_COUNT       4

enum CalculatedHeightSource {
    CALCULATED_HEIGHT_SOURCE_NONE = 0,
    CALCULATED_HEIGHT_SOURCE_BDHC,
    CALCULATED_HEIGHT_SOURCE_DYNAMIC
};

int Terrain_CalcHeight(FieldSystem *fieldSystem, int objectHeightMode, int objectHeight, fx32 objectX, fx32 objectZ, u8 *newObjectHeightSourceOut) {
    BOOL dynHeightPlateFound;
    u32 tileX, tileZ;
    u32 mapMatrixX, mapMatrixZ;
    VecFx32 objectPosition;

    int mapMatrixWidth;
    int mapMatrixWidthTiles;
    MapLoadManager *landDataMan;
    BOOL bdhcHeightValid;
    fx32 newObjectHeight = 0;

    objectPosition.x = objectX;
    objectPosition.z = objectZ;
    objectPosition.y = 0;

    landDataMan = fieldSystem->mapLoadManager;
    mapMatrixWidth = MapMatrix_GetWidth(fieldSystem->mapMatrix);
    mapMatrixWidthTiles = mapMatrixWidth * MAP_TILES_COUNT_X;

    tileX = objectX / 0x10000;
    tileZ = objectZ / 0x10000;

    u8 dynHeightPlateIndex;
    dynHeightPlateFound = DynamicTerrainHeightManager_FindPlate(tileX, tileZ, fieldSystem->dynamicTerrainHeightManager, &dynHeightPlateIndex);

    mapMatrixX = tileX / MAP_TILES_COUNT_X;
    mapMatrixZ = tileZ / MAP_TILES_COUNT_Z;
    u32 mapMatrixIndex = mapMatrixX + mapMatrixZ * mapMatrixWidth;

    fx32 currentMapOriginX = (mapMatrixX * MAP_TILES_COUNT_X + MAP_TILES_COUNT_X / 2) * MAP_OBJECT_TILE_SIZE;
    fx32 currentMapOriginZ = (mapMatrixZ * MAP_TILES_COUNT_Z + MAP_TILES_COUNT_Z / 2) * MAP_OBJECT_TILE_SIZE;

    objectPosition.x = objectX - currentMapOriginX;
    objectPosition.z = objectZ - currentMapOriginZ;

    u32 tileIndex = tileX + tileZ * mapMatrixWidthTiles;
    int tileMapQuadrant = TileIndex_GetQuadrant(tileIndex, mapMatrixWidthTiles);
    u32 loadedMapIndex = MapLoadManager_GetMapSlotOfMatrixIndex(mapMatrixIndex, tileMapQuadrant, landDataMan);

    if (loadedMapIndex > QUADRANT_COUNT - 1) {
        bdhcHeightValid = FALSE;
    } else {
        void *bdhc = MapLoadManager_GetBdhc(landDataMan, loadedMapIndex);
        bdhcHeightValid = Bdhc_GetHeight(objectHeightMode, objectHeight, objectPosition.x, objectPosition.z, bdhc, &objectPosition.y);
    }

    u8 newObjectHeightSource;

    if (dynHeightPlateFound) {
        fx32 dynamicHeight = DynamicTerrainHeightManager_GetPlateHeight(dynHeightPlateIndex, fieldSystem->dynamicTerrainHeightManager);

        if (bdhcHeightValid) {
            if (dynamicHeight <= objectPosition.y) {
                newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_BDHC;
                newObjectHeight = objectPosition.y;
            } else {
                fx32 bdhcHeightDiff = Terrain_AbsDiff(objectPosition.y, objectHeight);
                fx32 dynamicHeightDiff = Terrain_AbsDiff(dynamicHeight, objectHeight);

                if (bdhcHeightDiff <= dynamicHeightDiff) {
                    newObjectHeight = objectPosition.y;
                    newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_BDHC;
                } else {
                    newObjectHeight = dynamicHeight;
                    newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_DYNAMIC;
                }
            }
        } else {
            newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_DYNAMIC;
            newObjectHeight = dynamicHeight;
        }
    } else {
        if (bdhcHeightValid) {
            newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_BDHC;
            newObjectHeight = objectPosition.y;
        } else {
            newObjectHeightSource = CALCULATED_HEIGHT_SOURCE_NONE;
        }
    }

    if (newObjectHeightSourceOut != NULL) {
        *newObjectHeightSourceOut = newObjectHeightSource;
    }

    return newObjectHeight;
}

int Terrain_GetHeight_Bdhc(FieldSystem *fieldSystem, int objectHeight, fx32 x, fx32 z, u8 *outKind) {
    return Terrain_CalcHeight(fieldSystem, 0, objectHeight, x, z, outKind);
}

int Terrain_CalcHeightWithMode(FieldSystem *fieldSystem, int objectHeightMode, int objectHeight, fx32 x, fx32 z, u8 *outKind) {
    return Terrain_CalcHeight(fieldSystem, objectHeightMode, objectHeight, x, z, outKind);
}

int Terrain_GetHeight_Simple(FieldSystem *fieldSystem, int objectHeight, fx32 x, fx32 z, u8 *outKind) {
    BOOL ok = MapLoadManager_GetMapSlotAt(fieldSystem->mapLoadManager, x / 0x10000, z / 0x10000, NULL);
    if (outKind != NULL) {
        *outKind = ok ? 1 : 0;
    }
    return 0;
}

BOOL Terrain_GetAttributes_Loaded(FieldSystem *fieldSystem, int x, int z, u16 *outAttr) {
    MapLoadManager *mapLoadManager = fieldSystem->mapLoadManager;
    u8 idx;
    u16 *attrs;
    if (!MapLoadManager_GetMapSlotAt(mapLoadManager, x, z, &idx)) {
        *outAttr = 0xFF;
        return FALSE;
    }
    attrs = MapLoadManager_GetPermissions(mapLoadManager, idx);
    *outAttr = attrs[x % 32 + z % 32 * 32];
    return TRUE;
}

BOOL Terrain_GetAttributes_Simple(FieldSystem *fieldSystem, int x, int z, u16 *outAttr) {
    const u16 *attrs = TerrainAttributes_Get(x / 32 + z / 32 * MapMatrix_GetWidth(fieldSystem->mapMatrix), fieldSystem->terrainAttributes);
    *outAttr = attrs[x % 32 + z % 32 * 32];
    return TRUE;
}

SoundplateStruct *FieldSystem_GetSoundplateAt(FieldSystem *fieldSystem, int x, int z) {
    MapLoadManager *mapLoadManager = fieldSystem->mapLoadManager;
    u8 idx;
    if (MapLoadManager_GetMapSlotAt(mapLoadManager, x, z, &idx) == 0) {
        return NULL;
    }
    return MapLoadManager_GetSoundplates(mapLoadManager, idx);
}

void FieldSystem_SetTerrainQueries(void **queries, int mode) {
    if (mode == 0) {
        *queries = (void *)&gTerrainQueries_Bdhc;
    } else if (mode == 1) {
        *queries = (void *)&gTerrainQueries_Simple;
    } else {
        GF_AssertFail();
    }
}

BOOL FieldSystem_CheckTileCollision(FieldSystem *fieldSystem, int x, int z) {
    u16 attr;
    if (FS_TERRAIN_QUERIES(fieldSystem)->getAttributes(fieldSystem, x, z, &attr)) {
        if ((u8)((u8)(attr >> 15) & 1) == 1) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 sub_020548EC(FieldSystem *fieldSystem, int x, int z) {
    u16 attr;
    if (FS_TERRAIN_QUERIES(fieldSystem)->getAttributes(fieldSystem, x, z, &attr)) {
        return (u8)(attr >> 8) & 0x7F;
    }
    return 0;
}

u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z) {
    u16 attr;
    int behavior;
    if (FS_TERRAIN_QUERIES(fieldSystem)->getAttributes(fieldSystem, x, z, &attr)) {
        behavior = (u8)attr;
        return (u8)behavior;
    }
    return 0xFF;
}

int FieldSystem_GetTerrainHeight(FieldSystem *fieldSystem, int objectHeight, fx32 x, fx32 z, u8 *outKind) {
    return FS_TERRAIN_QUERIES(fieldSystem)->getHeight(fieldSystem, objectHeight, x, z, outKind);
}

int FieldSystem_GetHeightStep(FieldSystem *fieldSystem, VecFx32 *playerPos, int x, int z, u8 *outKind) {
    int dir;
    fx32 hi, lo;
    fx32 h = FieldSystem_GetTerrainHeight(fieldSystem, playerPos->y, (x << 16) + 0x8000, (z << 16) + 0x8000, outKind);
    if (h < playerPos->y) {
        hi = playerPos->y;
        lo = h;
        dir = -1;
    } else if (h > playerPos->y) {
        hi = h;
        lo = playerPos->y;
        dir = 1;
    } else {
        return 0;
    }
    if (hi - lo >= 0x14000) {
        if (dir == 0) {
            GF_AssertFail();
        }
    } else {
        dir = 0;
    }
    return dir;
}

BOOL FieldSystem_CheckCollisionAndHeight(FieldSystem *fieldSystem, VecFx32 *playerPos, int xInFront, int yInFront, int outSlope) {
    u8 kind;
    BOOL ret;
    int slope = FieldSystem_GetHeightStep(fieldSystem, playerPos, xInFront, yInFront, &kind);
    if (outSlope != 0) {
        *(u8 *)outSlope = (u8)slope;
    }
    if (slope == 0) {
        ret = FieldSystem_CheckTileCollision(fieldSystem, xInFront, yInFront);
        if (!ret && kind == 2) {
            GetMetatileBehavior(fieldSystem, xInFront, yInFront);
        }
        return ret;
    }
    return TRUE;
}

u32 FieldSystem_CheckCollisionHeightAndGimmick(FieldSystem *fieldSystem, VecFx32 *playerPos, u32 x, u32 y, u32 *outSlope) {
    u8 kind;
    BOOL ret;
    int slope = FieldSystem_GetHeightStep(fieldSystem, playerPos, x, y, &kind);
    if (outSlope != NULL) {
        *(u8 *)outSlope = (u8)slope;
    }
    if (slope == 0) {
        if (Gymmick_CheckCollision(fieldSystem, x, y, playerPos->y, &ret) == 0) {
            ret = FieldSystem_CheckTileCollision(fieldSystem, x, y);
            if (!ret && kind == 2) {
                GetMetatileBehavior(fieldSystem, x, y);
            }
            return ret;
        }
        return ret;
    }
    return TRUE;
}

void Terrain_MakeTileRect(int x, int z, int offsetX, int offsetZ, int width, int depth, fx32 *out) {
    int x1 = x + offsetX;
    int z1 = z + offsetZ;
    int x2 = x1 + width;
    int z2 = z1 + depth;
    if (x1 < 0 || z1 < 0 || x2 < 0 || z2 < 0) {
        GF_AssertFail();
    }
    out[0] = x1 << 16;
    out[1] = z1 << 16;
    out[2] = x2 << 16;
    out[3] = z2 << 16;
}

BOOL MapProp_IsInRect(MapProp *mapProp, fx32 *rect, VecFx32 *offset) {
    VecFx32 pos;
    MapProp_GetTranslation(&pos, mapProp);
    pos.x = pos.x + offset->x;
    pos.z = pos.z + offset->z;
    if (rect[0] <= pos.x && pos.x <= rect[2] && rect[1] <= pos.z && pos.z <= rect[3]) {
        return TRUE;
    }
    return FALSE;
}

BOOL FieldSystem_FindMapPropInRect(FieldSystem *fieldSystem, int buildModel, fx32 *rect, MapProp **outProp) {
    u8 i;
    u8 j;
    MapPropManager *mgr;
    VecFx32 pos;
    MapProp *prop;
    int idx;
    int model;
    for (i = 0; i < 4; i++) {
        MapLoadManager_GetMapPropManager(i, fieldSystem->mapLoadManager, &mgr);
        if (mgr == NULL) {
            continue;
        }
        idx = MapLoadManager_GetMapMatrixIndex(fieldSystem->mapLoadManager, i);
        Terrain_GetChunkOrigin(idx, MapMatrix_GetWidth(fieldSystem->mapMatrix), &pos);
        for (j = 0; j < 32; j++) {
            prop = MapPropManager_GetMapPropByIndex(mgr, j);
            if (MapProp_IsInRect(prop, rect, &pos)) {
                model = MapProp_GetBuildModel(prop);
                if (model == buildModel) {
                    if (outProp != NULL) {
                        *outProp = prop;
                    }
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL FieldSystem_FindMapPropOfModelsInRect(FieldSystem *fieldSystem, int *models, u32 count, fx32 *rect, MapProp **outProp, int *outModel) {
    u8 i;
    u8 j;
    u8 k;
    MapPropManager *mgr;
    VecFx32 pos;
    MapProp *prop;
    int idx;
    int model;
    for (i = 0; i < 4; i++) {
        MapLoadManager_GetMapPropManager(i, fieldSystem->mapLoadManager, &mgr);
        if (mgr == NULL) {
            continue;
        }
        idx = MapLoadManager_GetMapMatrixIndex(fieldSystem->mapLoadManager, i);
        Terrain_GetChunkOrigin(idx, MapMatrix_GetWidth(fieldSystem->mapMatrix), &pos);
        for (j = 0; j < 32; j++) {
            prop = MapPropManager_GetMapPropByIndex(mgr, j);
            if (MapProp_IsInRect(prop, rect, &pos)) {
                model = MapProp_GetBuildModel(prop);
                for (k = 0; k < count; k++) {
                    if (model == models[k]) {
                        if (outProp != NULL) {
                            *outProp = prop;
                        }
                        if (outModel != NULL) {
                            *outModel = model;
                        }
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

BOOL FieldSystem_FindMapProp(FieldSystem *fieldSystem, int buildModel, MapProp **outProp, int *outIdx) {
    u8 i;
    u8 j;
    MapPropManager *mgr;
    MapProp *prop;
    int model;
    for (i = 0; i < 4; i++) {
        MapLoadManager_GetMapPropManager(i, fieldSystem->mapLoadManager, &mgr);
        if (mgr == NULL) {
            continue;
        }
        for (j = 0; j < 32; j++) {
            prop = MapPropManager_GetMapPropByIndex(mgr, j);
            model = MapProp_GetBuildModel(prop);
            if (model == buildModel) {
                if (outProp != NULL) {
                    *outProp = prop;
                }
                if (outIdx != NULL) {
                    *outIdx = MapLoadManager_GetMapMatrixIndex(fieldSystem->mapLoadManager, i);
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL FieldSystem_FindMapPropOfModels(FieldSystem *fieldSystem, int *models, u32 count, MapProp **outProp, int *outModel) {
    u8 i;
    u8 j;
    u8 k;
    MapPropManager *mgr;
    MapProp *prop;
    int model;
    for (i = 0; i < 4; i++) {
        MapLoadManager_GetMapPropManager(i, fieldSystem->mapLoadManager, &mgr);
        if (mgr == NULL) {
            continue;
        }
        for (j = 0; j < 32; j++) {
            prop = MapPropManager_GetMapPropByIndex(mgr, j);
            model = MapProp_GetBuildModel(prop);
            for (k = 0; k < count; k++) {
                if (model == models[k]) {
                    if (outProp != NULL) {
                        *outProp = prop;
                    }
                    if (outModel != NULL) {
                        *outModel = model;
                    }
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

int *FieldSystem_CollectMapPropModelsInRect(FieldSystem *fieldSystem, u32 heapId, int capacity, fx32 *rect, u32 fill) {
    u8 outer;
    int *array = Heap_AllocAtEnd((enum HeapID)heapId, capacity * sizeof(int));
    int i;
    u8 count;

    for (i = 0; i < capacity; i++) {
        array[i] = fill;
    }
    count = 0;
    for (outer = 0; outer < 4; outer++) {
        MapPropManager *manager;
        MapLoadManager_GetMapPropManager(outer, fieldSystem->mapLoadManager, &manager);
        if (manager != NULL) {
            int index = MapLoadManager_GetMapMatrixIndex(fieldSystem->mapLoadManager, outer);
            VecFx32 offset;
            Terrain_GetChunkOrigin(index, MapMatrix_GetWidth(fieldSystem->mapMatrix), &offset);
            u8 inner;
            for (inner = 0; inner < 32; inner++) {
                MapProp *prop = MapPropManager_GetMapPropByIndex(manager, inner);
                if (MapProp_IsInRect(prop, rect, &offset)) {
                    int model = MapProp_GetBuildModel(prop);
                    if (model != 0) {
                        if ((u32)count >= (u32)capacity) {
                            GF_AssertFail();
                            return array;
                        }
                        array[count++] = model;
                    }
                }
            }
        }
    }
    return array;
}

void Terrain_GetChunkOrigin(int idx, int width, VecFx32 *pos) {
    pos->x = 0x100000;
    pos->z = 0x100000;
    pos->x = pos->x + ((u16)(idx % width) << 21);
    pos->z = pos->z + ((u16)(idx / width) << 21);
}

BOOL sub_02054E00(u16 a0) {
    int i;
    const u16 *p = _020FC60C;
    for (i = 0; i < 4; i++) {
        if (a0 == *p) {
            return TRUE;
        }
        p++;
    }
    return FALSE;
}

BOOL sub_02054E20(u16 a0) {
    int i;
    const u16 *p;
    if (!sub_02054E00(a0)) {
        return FALSE;
    }
    p = _020FC5FC;
    for (i = 0; i < 3; i++) {
        if (a0 == *p) {
            return TRUE;
        }
        p++;
    }
    return FALSE;
}
