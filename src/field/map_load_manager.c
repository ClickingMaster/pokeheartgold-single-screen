// field/map_prop_render.h (through field_system.h) declares AreaMapAnimation_IsLoaded void; its result is used here
#define AreaMapAnimation_IsLoaded AreaMapAnimation_IsLoaded_void
#include "field/map_load_manager.h"

#include <nnsys/gfd_internal.h>

#include "global.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/bdhc.h"
#include "field/field_control.h"
#include "field/map_load_buffers.h"
#include "field/map_prop.h"

#include "comm_state.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "map_matrix.h"
#include "player_data.h"
#include "safari_zone.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"

// The field keeps a 2x2 window of maps (map matrix cells of 32x32 tiles) loaded around the player:
//   maps[0] maps[1]
//   maps[2] maps[3]
// The quadrant of its map the player stands in decides which cell of the window that map is, so the window always
// holds the three neighbours the player is closest to. Walking into another quadrant scrolls the window: the two
// maps falling out are reset and the two new ones are loaded over several frames by a load slot (model and BDHC
// load tasks), while the tracked target position is followed tile by tile.

#define MAP_LOAD_NONE 0xFFFF
#define MAP_TILE_SIZE FX32_CONST(16)

// Scroll / walking directions
enum MapLoadDirection {
    MAP_LOAD_DIR_NONE,
    MAP_LOAD_DIR_EAST,
    MAP_LOAD_DIR_SOUTH,
    MAP_LOAD_DIR_WEST,
    MAP_LOAD_DIR_NORTH,
};

// MapLoadManager.state
enum MapLoadState {
    MAP_LOAD_STATE_START_CHUNK,
    MAP_LOAD_STATE_WAIT_CHUNK,
    MAP_LOAD_STATE_IDLE,
    MAP_LOAD_STATE_END,
};

// The four section sizes at the start of a land data file; the sections follow in the order permissions, props,
// model, BDHC.
typedef struct MapFileHeader {
    u32 permissionsSize;
    u32 modelSize;
    u32 bdhcSize;
    u32 propsSize;
} MapFileHeader;

// SoundplateStruct with its size word: the high half-word is the size of the soundplate data
typedef struct MapSoundplates {
    int header;
    u8 data[0x200];
} MapSoundplates;

// One cell of the loaded window
typedef struct LoadedMap {
    u16 permissions[0x400];
    NNSG3dRenderObj renderObj;
    NNSG3dResFileHeader *modelFile; // 0x854
    u8 *bdhcBuffer;
    Bdhc *bdhc;
    int matrixIndex;  // -1: empty
    BOOL modelLoaded; // 0x864
    MapPropManager *propManager;
    MapSoundplates soundplates;
    u8 *permissionBytes;        // 0xA70, safari zone mode only: the low bytes of permissions
} LoadedMap;

typedef struct MapLoadTasks {
    BOOL modelLoading;
    BOOL bdhcLoading;
} MapLoadTasks;

// Two maps loaded in the background after a scroll, one after the other
typedef struct MapLoadSlot {
    LoadedMap *maps[2];
    SysTask *modelTask;
    SysTask *bdhcTask;
    MapLoadTasks tasks; // 0x10
    int matrixIndices[2];
    u8 mapSlots[2]; // 0x20, cells of the window
    u8 numStarted;
    BOOL finished; // 0x24
    BOOL active;
    u8 direction; // 0x2C
} MapLoadSlot; // size: 0x30

// A scroll requested while both load slots were busy
typedef struct MapLoadQueued {
    LoadedMap *maps[2];
    BOOL queued; // 0x6C
    u8 direction;
    int matrixIndices[2];
    u8 mapSlots[2]; // 0x7C
} MapLoadQueued;

// The tracked position follows the target one tile at a time
typedef struct MapLoadTracking {
    VecFx32 pos;
    VecFx32 *target; // 0xDC
    fx32 *upper;
    fx32 *lower;
    BOOL moving;
    u8 direction; // 0xEC
    BOOL enabled;
} MapLoadTracking;

// Per land data mode: load the whole window now / load one map of a load slot in the background
typedef struct MapLoadFuncs {
    void (*initialLoad)(MapLoadManager *mapLoadManager, int x, int z, u32 stride);
    void (*loadMap)(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);
} MapLoadFuncs;

typedef void (*MapLoadedCallback)(void *arg, int matrixIndex, MapPropManager *propManager);

struct MapLoadManager {
    MapLoadBuffers *buffers;
    MapLoadSlot slots[2];
    MapLoadQueued queued;         // 0x64
    BOOL mapStale[4];             // 0x80, per cell: the map was reset, drop its pending load
    LoadedMap *maps[4];           // 0x90
    u8 numActiveSlots;            // 0xA0
    u8 nextSlot;                  // 0xA1
    u8 currentSlot;               // 0xA2
    u32 currentMatrixIndex;       // 0xA4
    u32 currentTile;              // 0xA8
    u8 currentMapSlot;            // 0xAC
    u8 quadrant;                  // 0xAD
    BOOL ended;                   // 0xB0
    int state;                    // 0xB4
    AreaDataManager *areaDataManager;
    Field3dRenderObjManager *renderObjManager; // 0xBC
    MapMatrix *mapMatrix;
    int width;                    // 0xC4, in maps
    int height;
    u32 stride;                   // 0xCC, width in tiles
    MapLoadTracking tracking;     // 0xD0
    MapPropAnimationManager *mapPropAnimationManager; // 0xF4
    AreaMapAnimation *areaMapAnimation;
    const MapLoadFuncs *funcs;    // 0xFC
    NARC *narc;
    SaveData *saveData;           // 0x104
    MapLoadedCallback mapLoadedCallback;
    void *mapLoadedCallbackArg;   // 0x10C
};

// The model load task
typedef struct MapModelLoad {
    NARC *narc;
    u32 modelNo;
    int size; // 0x08
    NNSG3dRenderObj *renderObj;
    NNSG3dResFileHeader **modelFile; // 0x10
    NNSG3dResTex *texture;
    u8 state; // 0x18
    BOOL *loaded;
    BOOL cancel; // 0x20
    BOOL *loading;
    AreaMapAnimation *areaMapAnimation; // 0x28
    int offset;
} MapModelLoad;

#undef AreaMapAnimation_IsLoaded
BOOL AreaMapAnimation_IsLoaded(AreaMapAnimation *unkCC);

BOOL sub_02054E20(u32 modelNo); // u16 in the field terrain headers; called here without narrowing
BOOL ov01_021FBA00(AreaDataManager *areaDataManager);

extern const MapLoadFuncs sMapLoadFuncs_Default;
extern const MapLoadFuncs sMapLoadFuncs_SafariZone;
extern const MapLoadFuncs sMapLoadFuncs_NoCollision;
extern const VecFx32 sMapModelScale;
extern const MtxFx33 sMapModelRotation;

BOOL MapMatrixIndex_IsSameColumn(int index1, int index2, int width);
BOOL MapMatrixIndex_IsSameRow(int index1, int index2, int width);
void MapLoadManager_CancelSlotTasks(MapLoadManager *mapLoadManager, int slot);
void MapLoadSlot_CancelTasks(MapLoadSlot *slot);
void CalcMapIndicesAroundPosition(int x, int z, int width, int height, u32 stride, int *indices);
void MapLoadManager_AllocMaps(MapLoadManager *mapLoadManager, BOOL withPermissionBytes);
void MapLoadManager_AllocMapsNoCollision(MapLoadManager *mapLoadManager);
void ReadMapFileHeader(NARC *narc, int modelNo, MapFileHeader *header);
int MapLoadManager_ReadMapFileHeader(MapFileHeader *header, MapLoadManager *mapLoadManager, MapMatrix *mapMatrix, LoadedMap *map, int matrixIndex, int numMaps, BOOL clearModel);
void MapLoadManager_ReadSoundplates(MapLoadManager *mapLoadManager, LoadedMap *map);
void MapLoadManager_ReadPermissions(MapLoadManager *mapLoadManager, LoadedMap *map, u32 size);
void MapLoadManager_LoadSafariZoneProps(MapLoadManager *mapLoadManager, LoadedMap *map, MapFileHeader *header, int matrixIndex);
void MapLoadManager_StartLoadTasks(MapLoadManager *mapLoadManager, LoadedMap *map, MapFileHeader *header, AreaDataManager *areaDataManager, MapLoadSlot *slot, int modelNo, BOOL withBdhc);
void MapLoadManager_LoadModelAndBdhc(MapLoadManager *mapLoadManager, LoadedMap *map, MapFileHeader *header, AreaDataManager *areaDataManager, int modelNo, int matrixIndex, BOOL applyLight, BOOL withBdhc);
void LoadSlotMap_Default(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);
void LoadSlotMap_NoCollision(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);
void LoadSlotMap_SafariZone(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot);
void LoadMapNow_Default(int matrixIndex, u8 mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, u32 width, u32 height, BOOL applyLight, MapLoadManager *mapLoadManager);
void LoadMapNow_NoCollision(int matrixIndex, u8 mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, u32 width, u32 height, BOOL applyLight, MapLoadManager *mapLoadManager);
void LoadMapNow_SafariZone(int matrixIndex, u8 mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, u32 width, u32 height, BOOL applyLight, MapLoadManager *mapLoadManager);
BOOL MapLoadTasks_AreDone(MapLoadTasks *tasks);
BOOL MapLoadManager_StartSlot(MapLoadManager *mapLoadManager, int matrixIndex1, int matrixIndex2, u8 mapSlot1, u8 mapSlot2, u8 slot, u8 direction);
void MapLoadManager_RequestLoad(int matrixIndex1, int matrixIndex2, u8 mapSlot1, u8 mapSlot2, u8 direction, MapLoadManager *mapLoadManager);
void MapLoadManager_ResetMap(int mapSlot, MapLoadManager *mapLoadManager);
void MapLoadManager_SwapMaps(int mapSlot1, int mapSlot2, int mapSlot3, int mapSlot4, MapLoadManager *mapLoadManager);
void MapLoadManager_Scroll(int matrixIndex, u8 direction, MapLoadManager *mapLoadManager);
u32 TileIndex_ToMatrixIndex(u32 tile, u32 width, u32 stride);
u32 TileCoords_ToIndex(u32 x, u32 z, u32 stride);
u32 ov01_021F5A30(u32 tile, u32 width, u32 stride);
int Quadrant_ToMapSlot(int quadrant);
int TileIndex_Step(int direction, int height, int stride, int tile);
u32 MapSlot_Step(u32 direction, int mapSlot);
void ov01_021F5B24(int direction, int target, u8 *mapSlot);
int Direction_GetOpposite(u32 direction);
BOOL MapLoadManager_FindOppositeSlot(u8 direction, MapLoadManager *mapLoadManager, u8 *slot);
BOOL MapLoadManager_StartQueuedSlot(MapLoadManager *mapLoadManager);
void MapLoadManager_StartQueuedLoad(MapLoadManager *mapLoadManager);
BOOL MapLoadManager_IsCurrentSlotFinished(MapLoadManager *mapLoadManager);
void MapLoadManager_ClearMapStale(MapLoadManager *mapLoadManager);
void MapLoadManager_MarkMapsStale(u32 direction, MapLoadManager *mapLoadManager);
void InitialLoad_Default(MapLoadManager *mapLoadManager, int x, int z, u32 stride);
void InitialLoad_NoCollision(MapLoadManager *mapLoadManager, int x, int z, u32 stride);
void InitialLoad_SafariZone(MapLoadManager *mapLoadManager, int x, int z, u32 stride);
void MapLoadManager_ResetSlots(MapLoadManager *mapLoadManager);
void MapLoadManager_SetPosition(int x, int z, MapLoadManager *mapLoadManager);
void CalcMapPosition(int matrixIndex, int width, MapMatrix *mapMatrix, VecFx32 *pos);
void MapLoadManager_OnTileStep(u8 direction, MapLoadManager *mapLoadManager);
void PositionToTile(fx32 x, fx32 z, int *tileX, int *tileZ);
void MapLoadManager_RenderLoadedMap(int mapSlot, MapLoadManager *mapLoadManager, ModelAttributes *modelAttributes);
int TileIndex_GetQuadrant(int tile, int stride);
u8 MapLoadManager_GetMapSlotOfMatrixIndex(int matrixIndex, int unused, MapLoadManager *mapLoadManager);
int MapLoadManager_GetMapMatrixIndex(MapLoadManager *mapLoadManager, u8 mapSlot);
BOOL MapLoadManager_GetMapSlotAt(MapLoadManager *mapLoadManager, int x, int z, u8 *mapSlot);
Bdhc *MapLoadManager_GetBdhc(MapLoadManager *mapLoadManager, u8 mapSlot);
SoundplateStruct *MapLoadManager_GetSoundplates(MapLoadManager *mapLoadManager, u8 mapSlot);
void MapModelLoad_Cancel(SysTask *task);
void Task_LoadMapModel(SysTask *task, MapModelLoad *load);
SysTask *StartMapModelLoadTask(NARC *narc, u32 modelNo, int size, NNSG3dRenderObj *renderObj, NNSG3dResFileHeader **modelFile, NNSG3dResTex *texture, AreaMapAnimation *unkCC, BOOL *loaded, BOOL *loading);
NNSG3dResMdl *LoadMapModel(NARC *narc, u32 size, NNSG3dRenderObj *renderObj, NNSG3dResFileHeader **modelFile, NNSG3dResTex *texture);

BOOL MapMatrixIndex_IsSameColumn(int index1, int index2, int width) {
    if (index1 % width == index2 % width) {
        return TRUE;
    }
    return FALSE;
}

BOOL MapMatrixIndex_IsSameRow(int index1, int index2, int width) {
    if (index1 / width == index2 / width) {
        return TRUE;
    }
    return FALSE;
}

void MapLoadManager_CancelSlotTasks(MapLoadManager *mapLoadManager, int slot) {
    if (mapLoadManager->slots[slot].tasks.modelLoading != FALSE) {
        MapModelLoad_Cancel(mapLoadManager->slots[slot].modelTask);
    }
    if (mapLoadManager->slots[slot].tasks.bdhcLoading != FALSE) {
        Bdhc_CancelLoadTask(mapLoadManager->slots[slot].bdhcTask);
    }
    mapLoadManager->slots[slot].finished = TRUE;
}

void MapLoadSlot_CancelTasks(MapLoadSlot *slot) {
    if (slot->tasks.modelLoading != FALSE) {
        MapModelLoad_Cancel(slot->modelTask);
    }
    if (slot->tasks.bdhcLoading != FALSE) {
        Bdhc_CancelLoadTask(slot->bdhcTask);
    }
    slot->tasks.modelLoading = FALSE;
    slot->tasks.bdhcLoading = FALSE;
}

#pragma force_active on
// The matrix indices of the window's four maps (-1 outside the matrix) for a tile position
void CalcMapIndicesAroundPosition(int x, int z, int width, int height, u32 stride, int *indices) {
    int mapX = x / 32, mapZ = z / 32;
    u32 quadrant = TileIndex_GetQuadrant(TileCoords_ToIndex(x, z, stride), stride);
    int index = mapX + mapZ * width;

    switch (quadrant) {
    case 0:
        indices[3] = index;
        indices[0] = index - width - 1;
        indices[1] = index - width;
        indices[2] = index - 1;
        if (mapX - 1 < 0) {
            indices[0] = -1;
            indices[2] = -1;
        }
        if (mapZ - 1 < 0) {
            indices[1] = -1;
        }
        break;
    case 1:
        indices[2] = index;
        indices[0] = index - width;
        indices[1] = index - width + 1;
        indices[3] = index + 1;
        if (mapX + 1 >= width) {
            indices[1] = -1;
            indices[3] = -1;
        }
        if (mapZ - 1 < 0) {
            indices[0] = -1;
        }
        break;
    case 2:
        indices[1] = index;
        indices[0] = index - 1;
        indices[2] = index + width - 1;
        indices[3] = index + width;
        if (mapX - 1 < 0) {
            indices[0] = -1;
            indices[2] = -1;
        }
        if (mapZ + 1 > height) {
            indices[3] = -1;
        }
        break;
    case 3:
        indices[0] = index;
        indices[1] = index + 1;
        indices[2] = index + width;
        indices[3] = index + width + 1;
        if (mapX + 1 >= width) {
            indices[1] = -1;
            indices[3] = -1;
        }
        if (mapZ + 1 > height) {
            indices[0] = -1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void MapLoadManager_AllocMaps(MapLoadManager *mapLoadManager, BOOL withPermissionBytes) {
    u32 i;
    LoadedMap *map;

    for (i = 0; i < 4; i = (u8)(i + 1)) {
        map = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LoadedMap));
        MI_CpuFill8(map, 0, sizeof(LoadedMap));
        mapLoadManager->maps[i] = map;
        mapLoadManager->maps[i]->modelLoaded = FALSE;
        MapLoadBuffers_GetPrimary(i, mapLoadManager->buffers, (void **)&mapLoadManager->maps[i]->modelFile);
        MapLoadBuffers_GetSecondary(i, mapLoadManager->buffers, (void **)&mapLoadManager->maps[i]->bdhcBuffer);
        mapLoadManager->maps[i]->propManager = MapPropManager_New(HEAP_ID_FIELD1, mapLoadManager->renderObjManager);
        mapLoadManager->maps[i]->matrixIndex = -1;
        MI_CpuFillFast(mapLoadManager->maps[i], -1, sizeof(mapLoadManager->maps[i]->permissions));
        if (withPermissionBytes != FALSE) {
            mapLoadManager->maps[i]->permissionBytes = Heap_Alloc(HEAP_ID_FIELD1, 0x400);
            MI_CpuFill8(mapLoadManager->maps[i]->permissionBytes, 0xFF, 0x400);
        }
        MI_CpuClear32(&mapLoadManager->maps[i]->soundplates, sizeof(mapLoadManager->maps[i]->soundplates));
    }
}
#pragma force_active reset

void MapLoadManager_AllocMapsNoCollision(MapLoadManager *mapLoadManager) {
    u8 i;
    LoadedMap *map;

    for (i = 0; i < 4; i++) {
        map = Heap_Alloc(HEAP_ID_FIELD1, sizeof(LoadedMap));
        MI_CpuFill8(map, 0, sizeof(LoadedMap));
        mapLoadManager->maps[i] = map;
        mapLoadManager->maps[i]->modelLoaded = FALSE;
        MapLoadBuffers_GetPrimary(i, mapLoadManager->buffers, (void **)&mapLoadManager->maps[i]->modelFile);
        mapLoadManager->maps[i]->propManager = MapPropManager_New(HEAP_ID_FIELD1, mapLoadManager->renderObjManager);
        mapLoadManager->maps[i]->matrixIndex = -1;
    }
}

void ReadMapFileHeader(NARC *narc, int modelNo, MapFileHeader *header) {
    u32 *buf;

    if (narc != NULL) {
        buf = Heap_AllocAtEnd(HEAP_ID_FIELD1, 16);
        NARC_ReadFromMember(narc, modelNo, 0, 16, buf);
    } else {
        GF_AssertFail();
        buf = AllocAtEndAndReadFromNarcMemberByIdPair(NARC_fielddata_landdata_land_data, modelNo, HEAP_ID_FIELD1, 0, 16);
    }
    header->permissionsSize = buf[0];
    header->propsSize = buf[1];
    header->modelSize = buf[2];
    header->bdhcSize = buf[3];
    Heap_Free(buf);
}

// Leaves the NARC at the start of the map's land data file; returns its model number (MAP_LOAD_NONE: no map)
int MapLoadManager_ReadMapFileHeader(MapFileHeader *header, MapLoadManager *mapLoadManager, MapMatrix *mapMatrix, LoadedMap *map, int matrixIndex, int numMaps, BOOL clearModel) {
    int modelNo;

    if (clearModel) {
        if (map->renderObj.anmMat != NULL) {
            GF_AssertFail();
        }
        map->renderObj.anmMat = NULL;
    }
    if (matrixIndex < 0 || matrixIndex >= numMaps) {
        return MAP_LOAD_NONE;
    }
    modelNo = GetMapModelNo(matrixIndex, mapMatrix);
    if (modelNo != MAP_LOAD_NONE) {
        ReadMapFileHeader(mapLoadManager->narc, modelNo, header);
    }
    return modelNo;
}

void MapLoadManager_ReadSoundplates(MapLoadManager *mapLoadManager, LoadedMap *map) {
    u16 size;

    NARC_ReadFile(mapLoadManager->narc, 4, &map->soundplates);
    size = map->soundplates.header >> 16;
    if (size != 0) {
        NARC_ReadFile(mapLoadManager->narc, size, map->soundplates.data);
    }
}

void MapLoadManager_ReadPermissions(MapLoadManager *mapLoadManager, LoadedMap *map, u32 size) {
    int i;

    NARC_ReadFile(mapLoadManager->narc, size, map);
    if (map->permissionBytes != NULL) {
        for (i = 0; i < 0x400; i++) {
            map->permissionBytes[i] = ((u16 *)map)[i];
        }
    }
}

// The safari zone's 3x2 area grid (matrix cells 1-3, 1-2) takes its props from the save
void MapLoadManager_LoadSafariZoneProps(MapLoadManager *mapLoadManager, LoadedMap *map, MapFileHeader *header, int matrixIndex) {
    u8 *areaSet;
    int width = mapLoadManager->width;
    u16 x = matrixIndex % width;
    u16 z = matrixIndex / width;

    if (x < 1 || x > 3 || z < 1 || z > 2) {
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header->propsSize, map->propManager, (int)mapLoadManager->mapPropAnimationManager);
    } else {
        SafariZone *safariZone = Save_SafariZone_Get(mapLoadManager->saveData);
        int linkLeader = SafariZone_GetActiveAreaSet(safariZone);
        areaSet = (u8 *)SafariZone_GetAreaSet(safariZone, linkLeader);
        int gender;
        if (linkLeader == 0) {
            gender = (u8)PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(mapLoadManager->saveData));
        } else {
            gender = SafariZone_GetLinkLeaderGender(safariZone);
        }
        u16 areaX = x - 1;
        u16 areaZ = z - 1;
        MapPropManager_LoadFromSafariZone(mapLoadManager->narc, map->propManager, (int)mapLoadManager->mapPropAnimationManager, (const SAFARIZONE_AREA *)(areaSet + sizeof(SAFARIZONE_AREA) * (areaX + 3 * areaZ)), (u16 *)map, gender);
    }
}

void MapLoadManager_StartLoadTasks(MapLoadManager *mapLoadManager, LoadedMap *map, MapFileHeader *header, AreaDataManager *areaDataManager, MapLoadSlot *slot, int modelNo, BOOL withBdhc) {
    NNSG3dResTex *texture;

    slot->tasks.modelLoading++;
    texture = AreaDataManager_GetMapTexture(areaDataManager);
    slot->modelTask = StartMapModelLoadTask(mapLoadManager->narc, modelNo, header->modelSize, &map->renderObj, &map->modelFile, texture, mapLoadManager->areaMapAnimation, &map->modelLoaded, &slot->tasks.modelLoading);
    if (withBdhc) {
        slot->tasks.bdhcLoading++;
        slot->bdhcTask = Bdhc_StartLoadTask(mapLoadManager->narc, header->bdhcSize, map->bdhc, (u32 *)&slot->tasks.bdhcLoading, &map->bdhcBuffer, (SysTask **)&slot->tasks.modelLoading);
    }
}

void MapLoadManager_LoadModelAndBdhc(MapLoadManager *mapLoadManager, LoadedMap *map, MapFileHeader *header, AreaDataManager *areaDataManager, int modelNo, int matrixIndex, BOOL applyLight, BOOL withBdhc) {
    NNSG3dResTex *texture = AreaDataManager_GetMapTexture(areaDataManager);
    NNSG3dResMdl *model = LoadMapModel(mapLoadManager->narc, header->modelSize, &map->renderObj, &map->modelFile, texture);
    if (applyLight) {
        ov01_021EA3B0(model);
    }
    map->modelLoaded = TRUE;
    map->matrixIndex = matrixIndex;
    if (withBdhc) {
        Bdhc_Load(mapLoadManager->narc, header->bdhcSize, map->bdhc, map->bdhcBuffer);
        if (mapLoadManager->mapLoadedCallback != NULL) {
            mapLoadManager->mapLoadedCallback(mapLoadManager->mapLoadedCallbackArg, matrixIndex, map->propManager);
        }
        if (AreaMapAnimation_IsLoaded(mapLoadManager->areaMapAnimation) && !sub_02054E20(modelNo)) {
            AreaMapAnimation_AddToRenderObj(mapLoadManager->areaMapAnimation, &map->renderObj);
        }
    }
}

void LoadSlotMap_Default(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot) {
    MapFileHeader header;
    int modelNo = MapLoadManager_ReadMapFileHeader(&header, mapLoadManager, mapMatrix, slot->maps[index], slot->matrixIndices[index], width * height, TRUE);
    if (modelNo != MAP_LOAD_NONE) {
        MapLoadManager_ReadSoundplates(mapLoadManager, slot->maps[index]);
        MapLoadManager_ReadPermissions(mapLoadManager, slot->maps[index], header.permissionsSize);
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header.propsSize, slot->maps[index]->propManager, (int)mapLoadManager->mapPropAnimationManager);
        MapLoadManager_StartLoadTasks(mapLoadManager, slot->maps[index], &header, areaDataManager, slot, modelNo, TRUE);
    }
}

void LoadSlotMap_NoCollision(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot) {
    u8 permissions[0x800];
    MapFileHeader header;
    int modelNo = MapLoadManager_ReadMapFileHeader(&header, mapLoadManager, mapMatrix, slot->maps[index], slot->matrixIndices[index], width * height, FALSE);
    if (modelNo != MAP_LOAD_NONE) {
        MapLoadManager_ReadSoundplates(mapLoadManager, slot->maps[index]);
        NARC_ReadFile(mapLoadManager->narc, sizeof(permissions), permissions);
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header.propsSize, slot->maps[index]->propManager, (int)mapLoadManager->mapPropAnimationManager);
        MapLoadManager_StartLoadTasks(mapLoadManager, slot->maps[index], &header, areaDataManager, slot, modelNo, FALSE);
    }
}

void LoadSlotMap_SafariZone(int index, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, int width, int height, MapLoadManager *mapLoadManager, MapLoadSlot *slot) {
    MapFileHeader header;
    int modelNo = MapLoadManager_ReadMapFileHeader(&header, mapLoadManager, mapMatrix, slot->maps[index], slot->matrixIndices[index], width * height, TRUE);
    if (modelNo != MAP_LOAD_NONE) {
        MapLoadManager_ReadSoundplates(mapLoadManager, slot->maps[index]);
        MapLoadManager_ReadPermissions(mapLoadManager, slot->maps[index], header.permissionsSize);
        MapLoadManager_LoadSafariZoneProps(mapLoadManager, slot->maps[index], &header, slot->matrixIndices[index]);
        MapLoadManager_StartLoadTasks(mapLoadManager, slot->maps[index], &header, areaDataManager, slot, modelNo, TRUE);
    }
}

void LoadMapNow_Default(int matrixIndex, u8 mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, u32 width, u32 height, BOOL applyLight, MapLoadManager *mapLoadManager) {
    MapFileHeader header;
    int modelNo;

    modelNo = MapLoadManager_ReadMapFileHeader(&header, mapLoadManager, mapMatrix, mapLoadManager->maps[mapSlot], matrixIndex, width * height, TRUE);
    if (modelNo != MAP_LOAD_NONE) {
        MapLoadManager_ReadSoundplates(mapLoadManager, mapLoadManager->maps[mapSlot]);
        MapLoadManager_ReadPermissions(mapLoadManager, mapLoadManager->maps[mapSlot], header.permissionsSize);
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header.propsSize, mapLoadManager->maps[mapSlot]->propManager, (int)mapLoadManager->mapPropAnimationManager);
        MapLoadManager_LoadModelAndBdhc(mapLoadManager, mapLoadManager->maps[mapSlot], &header, areaDataManager, modelNo, matrixIndex, applyLight, TRUE);
    }
}

void LoadMapNow_NoCollision(int matrixIndex, u8 mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, u32 width, u32 height, BOOL applyLight, MapLoadManager *mapLoadManager) {
    u8 permissions[0x800];
    MapFileHeader header;
    int modelNo;

    modelNo = MapLoadManager_ReadMapFileHeader(&header, mapLoadManager, mapMatrix, mapLoadManager->maps[mapSlot], matrixIndex, width * height, FALSE);
    if (modelNo != MAP_LOAD_NONE) {
        MapLoadManager_ReadSoundplates(mapLoadManager, mapLoadManager->maps[mapSlot]);
        NARC_ReadFile(mapLoadManager->narc, sizeof(permissions), permissions);
        MapPropManager_LoadFromNARC(mapLoadManager->narc, header.propsSize, mapLoadManager->maps[mapSlot]->propManager, (int)mapLoadManager->mapPropAnimationManager);
        MapLoadManager_LoadModelAndBdhc(mapLoadManager, mapLoadManager->maps[mapSlot], &header, areaDataManager, modelNo, matrixIndex, applyLight, FALSE);
    }
}

void LoadMapNow_SafariZone(int matrixIndex, u8 mapSlot, AreaDataManager *areaDataManager, MapMatrix *mapMatrix, u32 width, u32 height, BOOL applyLight, MapLoadManager *mapLoadManager) {
    MapFileHeader header;
    int modelNo;

    modelNo = MapLoadManager_ReadMapFileHeader(&header, mapLoadManager, mapMatrix, mapLoadManager->maps[mapSlot], matrixIndex, width * height, TRUE);
    if (modelNo != MAP_LOAD_NONE) {
        MapLoadManager_ReadSoundplates(mapLoadManager, mapLoadManager->maps[mapSlot]);
        MapLoadManager_ReadPermissions(mapLoadManager, mapLoadManager->maps[mapSlot], header.permissionsSize);
        MapLoadManager_LoadSafariZoneProps(mapLoadManager, mapLoadManager->maps[mapSlot], &header, matrixIndex);
        MapLoadManager_LoadModelAndBdhc(mapLoadManager, mapLoadManager->maps[mapSlot], &header, areaDataManager, modelNo, matrixIndex, applyLight, TRUE);
    }
}

BOOL MapLoadTasks_AreDone(MapLoadTasks *tasks) {
    if (tasks->modelLoading == FALSE && tasks->bdhcLoading == FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL MapLoadManager_StartSlot(MapLoadManager *mapLoadManager, int matrixIndex1, int matrixIndex2, u8 mapSlot1, u8 mapSlot2, u8 slot, u8 direction) {
    if (mapLoadManager->slots[slot].active != FALSE) {
        GF_AssertFail();
    }
    mapLoadManager->slots[slot].active = TRUE;
    mapLoadManager->slots[slot].direction = direction;
    mapLoadManager->slots[slot].matrixIndices[0] = matrixIndex1;
    mapLoadManager->slots[slot].matrixIndices[1] = matrixIndex2;
    mapLoadManager->slots[slot].mapSlots[0] = mapSlot1;
    mapLoadManager->slots[slot].mapSlots[1] = mapSlot2;
    mapLoadManager->slots[slot].maps[0] = mapLoadManager->maps[mapSlot1];
    mapLoadManager->slots[slot].maps[1] = mapLoadManager->maps[mapSlot2];
    mapLoadManager->slots[slot].numStarted = 0;
    mapLoadManager->slots[slot].finished = FALSE;
    if (mapLoadManager->numActiveSlots == 0) {
        mapLoadManager->state = MAP_LOAD_STATE_START_CHUNK;
    }
    mapLoadManager->maps[mapSlot1]->matrixIndex = matrixIndex1;
    mapLoadManager->maps[mapSlot2]->matrixIndex = matrixIndex2;
    mapLoadManager->numActiveSlots++;
    mapLoadManager->nextSlot = (mapLoadManager->nextSlot + 1) % 2;
    return TRUE;
}

#pragma force_active on
void MapLoadManager_Tick(MapLoadManager *mapLoadManager) {
    MapLoadSlot *slot = &mapLoadManager->slots[mapLoadManager->currentSlot];
    MapLoadTracking *tracking = &mapLoadManager->tracking;
    int x, z;
    LoadedMap *map;
    NNSG3dResMdl *model;

    if (tracking->enabled == TRUE && tracking->target != NULL) {
        if (tracking->moving) {
            if (*tracking->lower > *tracking->upper) {
                GF_AssertFail();
            }
            if (*tracking->upper - *tracking->lower >= MAP_TILE_SIZE) {
                PositionToTile(tracking->target->x, tracking->target->z, &x, &z);
                MapLoadManager_SetPosition(x, z, mapLoadManager);
                {
                    VecFx32 *dst = &tracking->pos;
                    VecFx32 *src = tracking->target;
                    *dst = *src;
                }
                if (tracking->pos.z % (MAP_TILE_SIZE / 2) && CommState_SetError(1)) {
                    return;
                }
                if (tracking->pos.z % (MAP_TILE_SIZE / 2)) {
                    GF_AssertFail();
                }
                tracking->moving = FALSE;
                tracking->upper = NULL;
                tracking->lower = NULL;
            }
        } else {
            if (tracking->pos.x != tracking->target->x && tracking->pos.z != tracking->target->z) {
                GF_AssertFail();
            } else if (tracking->pos.x != tracking->target->x || tracking->pos.z != tracking->target->z) {
                tracking->moving = TRUE;
                if (tracking->pos.x != tracking->target->x) {
                    if (tracking->pos.x > tracking->target->x) {
                        tracking->upper = &tracking->pos.x;
                        tracking->lower = &tracking->target->x;
                        tracking->direction = MAP_LOAD_DIR_WEST;
                    } else {
                        tracking->upper = &tracking->target->x;
                        tracking->lower = &tracking->pos.x;
                        tracking->direction = MAP_LOAD_DIR_EAST;
                    }
                    if (tracking->pos.x % (MAP_TILE_SIZE / 2) && CommState_SetError(1)) {
                        return;
                    }
                    if (tracking->pos.x % (MAP_TILE_SIZE / 2)) {
                        GF_AssertFail();
                    }
                } else {
                    if (mapLoadManager->tracking.pos.z > mapLoadManager->tracking.target->z) {
                        tracking->upper = &tracking->pos.z;
                        tracking->lower = &tracking->target->z;
                        tracking->direction = MAP_LOAD_DIR_NORTH;
                    } else {
                        tracking->upper = &tracking->target->z;
                        tracking->lower = &tracking->pos.z;
                        tracking->direction = MAP_LOAD_DIR_SOUTH;
                    }
                    if (tracking->pos.z % (MAP_TILE_SIZE / 2) && CommState_SetError(1)) {
                        return;
                    }
                    if (tracking->pos.z % (MAP_TILE_SIZE / 2)) {
                        GF_AssertFail();
                    }
                }
                MapLoadManager_OnTileStep(tracking->direction, mapLoadManager);
            }
        }
    }

    switch (mapLoadManager->state) {
    case MAP_LOAD_STATE_START_CHUNK:
        if (mapLoadManager->mapStale[slot->mapSlots[slot->numStarted]] != TRUE) {
            mapLoadManager->funcs->loadMap(slot->numStarted, mapLoadManager->areaDataManager, mapLoadManager->mapMatrix, mapLoadManager->width, mapLoadManager->height, mapLoadManager, slot);
        }
        slot->numStarted++;
        mapLoadManager->state = MAP_LOAD_STATE_WAIT_CHUNK;
        break;
    case MAP_LOAD_STATE_WAIT_CHUNK:
        if (mapLoadManager->mapStale[slot->mapSlots[slot->numStarted - 1]] == TRUE) {
            MapLoadSlot_CancelTasks(slot);
            mapLoadManager->mapStale[slot->mapSlots[slot->numStarted - 1]] = FALSE;
        }
        if (slot->numStarted <= 2 && MapLoadTasks_AreDone(&slot->tasks) == TRUE) {
            if (slot->numStarted >= 2) {
                slot->finished = TRUE;
            } else {
                mapLoadManager->state = MAP_LOAD_STATE_START_CHUNK;
            }
            map = slot->maps[slot->numStarted - 1];
            if (map->modelLoaded == TRUE) {
                model = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(map->modelFile), 0);
                if (ov01_021FBA00(mapLoadManager->areaDataManager) == TRUE) {
                    ov01_021EA3B0(model);
                }
            }
            if (mapLoadManager->mapLoadedCallback != NULL) {
                map = slot->maps[slot->numStarted - 1];
                if (map->matrixIndex >= 0 && map->matrixIndex < mapLoadManager->width * mapLoadManager->height) {
                    mapLoadManager->mapLoadedCallback(mapLoadManager->mapLoadedCallbackArg, map->matrixIndex, map->propManager);
                }
            }
        }
        break;
    case MAP_LOAD_STATE_IDLE:
        break;
    case MAP_LOAD_STATE_END:
        if (MapLoadTasks_AreDone(&slot->tasks) == TRUE) {
            mapLoadManager->numActiveSlots = 0;
        }
        break;
    }

    if (mapLoadManager->numActiveSlots == 0) {
        mapLoadManager->currentSlot = 0;
        mapLoadManager->nextSlot = 0;
        mapLoadManager->state = MAP_LOAD_STATE_IDLE;
        return;
    }
    if (MapLoadManager_IsCurrentSlotFinished(mapLoadManager) == TRUE) {
        mapLoadManager->slots[mapLoadManager->currentSlot].active = FALSE;
        MapLoadManager_ClearMapStale(mapLoadManager);
        mapLoadManager->numActiveSlots--;
        mapLoadManager->currentSlot = (mapLoadManager->currentSlot + 1) % 2;
        if (mapLoadManager->numActiveSlots) {
            mapLoadManager->state = MAP_LOAD_STATE_START_CHUNK;
        } else {
            mapLoadManager->currentSlot = 0;
            mapLoadManager->nextSlot = 0;
            mapLoadManager->state = MAP_LOAD_STATE_IDLE;
        }
        if (mapLoadManager->queued.queued == TRUE) {
            mapLoadManager->queued.queued = FALSE;
            MapLoadManager_MarkMapsStale(mapLoadManager->queued.direction, mapLoadManager);
            MapLoadManager_StartQueuedLoad(mapLoadManager);
        }
    }
}
#pragma force_active reset

// Starts loading two maps in a free slot; with both slots busy, the slot loading the other way is cancelled and the
// request queued
void MapLoadManager_RequestLoad(int matrixIndex1, int matrixIndex2, u8 mapSlot1, u8 mapSlot2, u8 direction, MapLoadManager *mapLoadManager) {
    u8 slot;

    if (mapLoadManager->queued.queued == TRUE) {
        GF_AssertFail();
    }
    if (MapLoadManager_FindOppositeSlot(direction, mapLoadManager, &slot) == TRUE) {
        MapLoadManager_CancelSlotTasks(mapLoadManager, slot);
        if (mapLoadManager->numActiveSlots >= 2) {
            mapLoadManager->queued.queued = TRUE;
            mapLoadManager->queued.matrixIndices[0] = matrixIndex1;
            mapLoadManager->queued.matrixIndices[1] = matrixIndex2;
            mapLoadManager->queued.mapSlots[0] = mapSlot1;
            mapLoadManager->queued.mapSlots[1] = mapSlot2;
            mapLoadManager->queued.maps[0] = mapLoadManager->maps[mapSlot1];
            mapLoadManager->queued.maps[1] = mapLoadManager->maps[mapSlot2];
            mapLoadManager->queued.direction = direction;
            mapLoadManager->maps[mapSlot1]->matrixIndex = matrixIndex1;
            mapLoadManager->maps[mapSlot2]->matrixIndex = matrixIndex2;
            return;
        }
        MapLoadManager_StartSlot(mapLoadManager, matrixIndex1, matrixIndex2, mapSlot1, mapSlot2, mapLoadManager->nextSlot, direction);
    } else if (MapLoadManager_StartSlot(mapLoadManager, matrixIndex1, matrixIndex2, mapSlot1, mapSlot2, mapLoadManager->nextSlot, direction) != TRUE) {
        GF_AssertFail();
    }
}

void MapLoadManager_ResetMap(int mapSlot, MapLoadManager *mapLoadManager) {
    mapLoadManager->maps[mapSlot]->modelLoaded = FALSE;
    Bdhc_Reset(mapLoadManager->maps[mapSlot]->bdhc);
    MapPropManager_Reset(mapLoadManager->maps[mapSlot]->propManager);
    mapLoadManager->maps[mapSlot]->matrixIndex = -1;
    MIi_CpuClearFast(-1, (u32 *)mapLoadManager->maps[mapSlot], sizeof(mapLoadManager->maps[mapSlot]->permissions));
    MIi_CpuClear32(0, (u32 *)&mapLoadManager->maps[mapSlot]->soundplates, sizeof(mapLoadManager->maps[mapSlot]->soundplates));
    mapLoadManager->mapStale[mapSlot] = TRUE;
    if (AreaMapAnimation_IsLoaded(mapLoadManager->areaMapAnimation) != FALSE) {
        AreaMapAnimation_RemoveFromRenderObj(mapLoadManager->areaMapAnimation, &mapLoadManager->maps[mapSlot]->renderObj);
    }
    mapLoadManager->maps[mapSlot]->renderObj.anmMat = NULL;
}

// Resets the maps in mapSlot1/2 and swaps them with mapSlot3/4
void MapLoadManager_SwapMaps(int mapSlot1, int mapSlot2, int mapSlot3, int mapSlot4, MapLoadManager *mapLoadManager) {
    LoadedMap *map1;
    LoadedMap *map2;
    LoadedMap *map3;

    MapLoadManager_ResetMap(mapSlot1, mapLoadManager);
    MapLoadManager_ResetMap(mapSlot2, mapLoadManager);
    map1 = mapLoadManager->maps[mapSlot1];
    map2 = mapLoadManager->maps[mapSlot2];
    map3 = mapLoadManager->maps[mapSlot3];
    mapLoadManager->maps[mapSlot1] = map3;
    mapLoadManager->maps[mapSlot2] = mapLoadManager->maps[mapSlot4];
    mapLoadManager->maps[mapSlot3] = map1;
    mapLoadManager->maps[mapSlot4] = map2;
}

#pragma force_active on
// The player entered another quadrant of the map at matrixIndex: move the window and load the two new maps
void MapLoadManager_Scroll(int matrixIndex, u8 direction, MapLoadManager *mapLoadManager) {
    u32 mapSlot1;
    int matrixIndex1;
    int matrixIndex2;
    unsigned int mapSlot2;

    switch (direction) {
    case MAP_LOAD_DIR_NORTH:
        MapLoadManager_SwapMaps(2, 3, 0, 1, mapLoadManager);
        if (mapLoadManager->quadrant == 2) {
            matrixIndex1 = matrixIndex - mapLoadManager->width;
            mapSlot1 = 1;
            mapSlot2 = 0;
            matrixIndex2 = matrixIndex1 - 1;
            if (matrixIndex1 < 0) {
                matrixIndex1 = -1;
                matrixIndex2 = -1;
            } else if (matrixIndex2 < 0 || !MapMatrixIndex_IsSameRow(matrixIndex1, matrixIndex2, mapLoadManager->width)) {
                matrixIndex2 = -1;
            }
        } else if (mapLoadManager->quadrant == 3) {
            matrixIndex1 = matrixIndex - mapLoadManager->width;
            matrixIndex2 = matrixIndex1 + 1;
            mapSlot1 = 0;
            mapSlot2 = 1;
            if (matrixIndex1 < 0) {
                matrixIndex1 = -1;
                matrixIndex2 = -1;
            } else if (!MapMatrixIndex_IsSameRow(matrixIndex1, matrixIndex2, mapLoadManager->width)) {
                matrixIndex2 = -1;
            }
        } else {
            GF_AssertFail();
        }
        MapLoadManager_RequestLoad(matrixIndex1, matrixIndex2, mapSlot1, mapSlot2, direction, mapLoadManager);
        return;
    case MAP_LOAD_DIR_WEST:
        MapLoadManager_SwapMaps(1, 3, 0, 2, mapLoadManager);
        if (mapLoadManager->quadrant == 1) {
            matrixIndex1 = matrixIndex - 1;
            matrixIndex2 = matrixIndex - mapLoadManager->width - 1;
            mapSlot1 = 2;
            mapSlot2 = 0;
            if (matrixIndex1 < 0 || !MapMatrixIndex_IsSameRow(matrixIndex1, matrixIndex, mapLoadManager->width)) {
                matrixIndex1 = -1;
                matrixIndex2 = matrixIndex1;
            }
            if (matrixIndex2 < 0 || !MapMatrixIndex_IsSameColumn(matrixIndex1, matrixIndex2, mapLoadManager->width)) {
                matrixIndex2 = -1;
            }
        } else if (mapLoadManager->quadrant == 3) {
            matrixIndex1 = matrixIndex - 1;
            matrixIndex2 = matrixIndex + mapLoadManager->width - 1;
            mapSlot1 = 0;
            mapSlot2 = 2;
            if (matrixIndex1 < 0 || !MapMatrixIndex_IsSameRow(matrixIndex1, matrixIndex, mapLoadManager->width)) {
                matrixIndex1 = -1;
                matrixIndex2 = matrixIndex1;
            }
            if (mapLoadManager->width * mapLoadManager->height <= matrixIndex2 || !MapMatrixIndex_IsSameColumn(matrixIndex1, matrixIndex2, mapLoadManager->width)) {
                matrixIndex2 = -1;
            }
        } else {
            GF_AssertFail();
        }
        MapLoadManager_RequestLoad(matrixIndex1, matrixIndex2, mapSlot1, mapSlot2, direction, mapLoadManager);
        return;
    case MAP_LOAD_DIR_EAST:
        MapLoadManager_SwapMaps(0, 2, 1, 3, mapLoadManager);
        if (mapLoadManager->quadrant == 0) {
            matrixIndex1 = matrixIndex + 1;
            matrixIndex2 = matrixIndex - mapLoadManager->width + 1;
            mapSlot1 = 3;
            mapSlot2 = 1;
            if (mapLoadManager->width * mapLoadManager->height <= matrixIndex1 || !MapMatrixIndex_IsSameRow(matrixIndex1, matrixIndex, mapLoadManager->width)) {
                matrixIndex2 = matrixIndex1 = -1;
            }
            if (mapLoadManager->width * mapLoadManager->height <= matrixIndex2 || !MapMatrixIndex_IsSameColumn(matrixIndex1, matrixIndex2, mapLoadManager->width)) {
                matrixIndex2 = -1;
            }
        } else if (mapLoadManager->quadrant == 2) {
            matrixIndex1 = matrixIndex + 1;
            matrixIndex2 = matrixIndex + mapLoadManager->width + 1;
            mapSlot1 = 1;
            mapSlot2 = 3;
            if (mapLoadManager->width * mapLoadManager->height <= matrixIndex1 || !MapMatrixIndex_IsSameRow(matrixIndex1, matrixIndex, mapLoadManager->width)) {
                matrixIndex2 = matrixIndex1 = -1;
            }
            if (matrixIndex2 < 0 || !MapMatrixIndex_IsSameColumn(matrixIndex1, matrixIndex2, mapLoadManager->width)) {
                matrixIndex2 = -1;
            }
        }
        MapLoadManager_RequestLoad(matrixIndex1, matrixIndex2, mapSlot1, mapSlot2, direction, mapLoadManager);
        return;
    case MAP_LOAD_DIR_SOUTH:
        MapLoadManager_SwapMaps(0, 1, 2, 3, mapLoadManager);
        if (mapLoadManager->quadrant == 0) {
            matrixIndex1 = matrixIndex + mapLoadManager->width;
            matrixIndex2 = matrixIndex1 - 1;
            mapSlot1 = 3;
            mapSlot2 = 2;
            if (mapLoadManager->width * mapLoadManager->height <= matrixIndex1) {
                matrixIndex1 = -1;
                matrixIndex2 = -1;
            } else if (!MapMatrixIndex_IsSameRow(matrixIndex1, matrixIndex2, mapLoadManager->width)) {
                matrixIndex2 = -1;
            }
        } else if (mapLoadManager->quadrant == 1) {
            matrixIndex1 = matrixIndex + mapLoadManager->width;
            matrixIndex2 = matrixIndex1 + 1;
            mapSlot1 = 2;
            mapSlot2 = 3;
            if (mapLoadManager->width * mapLoadManager->height <= matrixIndex1) {
                matrixIndex2 = matrixIndex1 = -1;
            } else if (mapLoadManager->width * mapLoadManager->height <= matrixIndex2 || !MapMatrixIndex_IsSameRow(matrixIndex1, matrixIndex2, mapLoadManager->width)) {
                matrixIndex2 = -1;
            }
        } else {
            GF_AssertFail();
        }
        MapLoadManager_RequestLoad(matrixIndex1, matrixIndex2, mapSlot1, mapSlot2, direction, mapLoadManager);
        return;
    case MAP_LOAD_DIR_NONE:
    default:
        GF_AssertFail();
        return;
    }
}

void MapLoadManager_RenderLoadedMap(int mapSlot, MapLoadManager *mapLoadManager, ModelAttributes *modelAttributes) {
    VecFx32 pos = { 0, 0, 0 };
    VecFx32 scale;
    MtxFx33 rotation;
    LoadedMap *map;
    volatile void *volatile savedModelAttributes = modelAttributes;

    CalcMapPosition(mapLoadManager->maps[mapSlot]->matrixIndex, mapLoadManager->width, mapLoadManager->mapMatrix, &pos);
    if (mapLoadManager->maps[mapSlot]->modelLoaded == TRUE) {
        scale = sMapModelScale;
        rotation = sMapModelRotation;
        GF3dRender_DrawModel(&mapLoadManager->maps[mapSlot]->renderObj, &pos, &rotation, &scale);
    }
    map = mapLoadManager->maps[mapSlot];
    if (map->modelLoaded == TRUE) {
        BOOL lit = ov01_021FBA00(mapLoadManager->areaDataManager);
        ov01_021F3A3C(&pos, mapLoadManager->areaDataManager, lit, (ModelAttributes *)savedModelAttributes, map->propManager);
    }
}
#pragma force_active reset

u32 TileIndex_ToMatrixIndex(u32 tile, u32 width, u32 stride) {
    u32 x = tile % stride;
    u32 z = tile / stride;

    return z / 32 * width + x / 32;
}

u32 TileCoords_ToIndex(u32 x, u32 z, u32 stride) {
    return x + z * stride;
}

u32 ov01_021F5A30(u32 tile, u32 width, u32 stride) {
    u32 x = tile % stride;
    u32 z = tile / stride;

    return z / 32 * width + x / 32;
}

int Quadrant_ToMapSlot(int quadrant) {
    if (quadrant == 0) {
        return 3;
    }
    if (quadrant == 1) {
        return 2;
    }
    if (quadrant == 2) {
        return 1;
    }
    if (quadrant == 3) {
        return 0;
    }
    GF_AssertFail();
    return 0;
}

#pragma force_active on
// The tile next to tile in direction, or tile itself at the edge of the matrix
int TileIndex_Step(int direction, int height, int stride, int tile) {
    int next;

    switch (direction) {
    case MAP_LOAD_DIR_EAST:
        next = tile % stride + 1;
        if (next >= stride) {
            next = tile;
        } else {
            next = tile + 1;
        }
        break;
    case MAP_LOAD_DIR_SOUTH:
        next = tile + stride;
        if (next >= height * 32 * stride) {
            next = tile;
        }
        break;
    case MAP_LOAD_DIR_WEST:
        next = tile % stride - 1;
        if (next < 0) {
            next = tile;
        } else {
            next = tile - 1;
        }
        break;
    case MAP_LOAD_DIR_NORTH:
        next = tile - stride;
        if (next < 0) {
            next = tile;
        }
        break;
    default:
        GF_AssertFail();
        next = 0;
        break;
    }
    return next;
}
#pragma force_active reset

// The window cell next to mapSlot in direction
u32 MapSlot_Step(u32 direction, int mapSlot) {
    switch (direction) {
    case MAP_LOAD_DIR_NORTH:
        return (u8)(mapSlot - 2);
    case MAP_LOAD_DIR_SOUTH:
        return (u8)(mapSlot + 2);
    case MAP_LOAD_DIR_WEST:
        return (u8)(mapSlot - 1);
    case MAP_LOAD_DIR_EAST:
        return (u8)(mapSlot + 1);
    }
    return direction;
}

void ov01_021F5B24(int direction, int target, u8 *mapSlot) {
    int current = MapSlot_Step(direction, target);
    u8 result;

    if (target < current) {
        int distance = current - target;
        if (distance == 1) {
            result = *mapSlot - 1;
        } else if (distance == 2) {
            result = *mapSlot - 2;
        } else {
            GF_AssertFail();
        }
    } else if (target > current) {
        int distance = target - current;
        if (distance == 1) {
            result = *mapSlot + 1;
        } else if (distance == 2) {
            result = *mapSlot + 2;
        } else {
            GF_AssertFail();
        }
    } else {
        GF_AssertFail();
    }
    *mapSlot = result;
}

int Direction_GetOpposite(u32 direction) {
    switch (direction) {
    case MAP_LOAD_DIR_NORTH:
        return MAP_LOAD_DIR_SOUTH;
    case MAP_LOAD_DIR_SOUTH:
        return MAP_LOAD_DIR_NORTH;
    case MAP_LOAD_DIR_EAST:
        return MAP_LOAD_DIR_WEST;
    case MAP_LOAD_DIR_WEST:
        return MAP_LOAD_DIR_EAST;
    }
    GF_AssertFail();
    return MAP_LOAD_DIR_NONE;
}

#pragma force_active on
BOOL MapLoadManager_FindOppositeSlot(u8 direction, MapLoadManager *mapLoadManager, u8 *slot) {
    int i;

    for (i = 0; i < 2; i++) {
        if (mapLoadManager->slots[i].active == TRUE && mapLoadManager->slots[i].direction == Direction_GetOpposite(direction)) {
            *slot = i;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL MapLoadManager_StartQueuedSlot(MapLoadManager *mapLoadManager) {
    MapLoadQueued *queued = &mapLoadManager->queued;

    GF_ASSERT(mapLoadManager->slots[mapLoadManager->nextSlot].active == FALSE);
    mapLoadManager->slots[mapLoadManager->nextSlot].active = TRUE;
    mapLoadManager->slots[mapLoadManager->nextSlot].direction = queued->direction;
    mapLoadManager->slots[mapLoadManager->nextSlot].matrixIndices[0] = queued->matrixIndices[0];
    mapLoadManager->slots[mapLoadManager->nextSlot].matrixIndices[1] = queued->matrixIndices[1];
    mapLoadManager->slots[mapLoadManager->nextSlot].mapSlots[0] = queued->mapSlots[0];
    mapLoadManager->slots[mapLoadManager->nextSlot].mapSlots[1] = queued->mapSlots[1];
    mapLoadManager->slots[mapLoadManager->nextSlot].maps[0] = queued->maps[0];
    mapLoadManager->slots[mapLoadManager->nextSlot].maps[1] = queued->maps[1];
    mapLoadManager->slots[mapLoadManager->nextSlot].numStarted = 0;
    mapLoadManager->slots[mapLoadManager->nextSlot].finished = FALSE;
    return TRUE;
}
#pragma force_active reset

void MapLoadManager_StartQueuedLoad(MapLoadManager *mapLoadManager) {
    MapLoadQueued *queued = &mapLoadManager->queued;

    if (mapLoadManager->numActiveSlots >= 2) {
        GF_AssertFail();
    }
    if (MapLoadManager_StartQueuedSlot(mapLoadManager) != TRUE) {
        GF_AssertFail();
    }
    queued->maps[0]->matrixIndex = queued->matrixIndices[0];
    queued->maps[1]->matrixIndex = queued->matrixIndices[1];
    mapLoadManager->numActiveSlots++;
    mapLoadManager->nextSlot = (mapLoadManager->nextSlot + 1) % 2;
}

BOOL MapLoadManager_IsCurrentSlotFinished(MapLoadManager *mapLoadManager) {
    return mapLoadManager->slots[mapLoadManager->currentSlot].finished;
}

void MapLoadManager_ClearMapStale(MapLoadManager *mapLoadManager) {
    u8 i;

    for (i = 0; i < 4; i++) {
        mapLoadManager->mapStale[i] = FALSE;
    }
}

// The two cells a scroll in direction replaces
void MapLoadManager_MarkMapsStale(u32 direction, MapLoadManager *mapLoadManager) {
    switch (direction) {
    case MAP_LOAD_DIR_NORTH:
        mapLoadManager->mapStale[2] = TRUE;
        mapLoadManager->mapStale[3] = TRUE;
        break;
    case MAP_LOAD_DIR_EAST:
        mapLoadManager->mapStale[0] = TRUE;
        mapLoadManager->mapStale[2] = TRUE;
        break;
    case MAP_LOAD_DIR_SOUTH:
        mapLoadManager->mapStale[0] = TRUE;
        mapLoadManager->mapStale[1] = TRUE;
        break;
    case MAP_LOAD_DIR_WEST:
        mapLoadManager->mapStale[1] = TRUE;
        mapLoadManager->mapStale[3] = TRUE;
        break;
    }
}

void InitialLoad_Default(MapLoadManager *mapLoadManager, int x, int z, u32 stride) {
    u8 i;
    int matrixIndices[4];

    MapLoadManager_AllocMaps(mapLoadManager, FALSE);
    CalcMapIndicesAroundPosition(x, z, mapLoadManager->width, mapLoadManager->height, stride, matrixIndices);
    for (i = 0; i < 4; i++) {
        mapLoadManager->maps[i]->bdhc = Bdhc_New();
        Bdhc_SetUnloaded(mapLoadManager->maps[i]->bdhc);
        LoadMapNow_Default(matrixIndices[i], i, mapLoadManager->areaDataManager, mapLoadManager->mapMatrix, mapLoadManager->width, mapLoadManager->height, ov01_021FBA00(mapLoadManager->areaDataManager), mapLoadManager);
    }
}

void InitialLoad_NoCollision(MapLoadManager *mapLoadManager, int x, int z, u32 stride) {
    u8 i;
    int matrixIndices[4];

    MapLoadManager_AllocMapsNoCollision(mapLoadManager);
    CalcMapIndicesAroundPosition(x, z, mapLoadManager->width, mapLoadManager->height, stride, matrixIndices);
    for (i = 0; i < 4; i++) {
        mapLoadManager->maps[i]->bdhc = NULL;
        LoadMapNow_NoCollision(matrixIndices[i], i, mapLoadManager->areaDataManager, mapLoadManager->mapMatrix, mapLoadManager->width, mapLoadManager->height, ov01_021FBA00(mapLoadManager->areaDataManager), mapLoadManager);
    }
}

void InitialLoad_SafariZone(MapLoadManager *mapLoadManager, int x, int z, u32 stride) {
    u8 i;
    int matrixIndices[4];

    MapLoadManager_AllocMaps(mapLoadManager, TRUE);
    CalcMapIndicesAroundPosition(x, z, mapLoadManager->width, mapLoadManager->height, stride, matrixIndices);
    for (i = 0; i < 4; i++) {
        mapLoadManager->maps[i]->bdhc = Bdhc_New();
        Bdhc_SetUnloaded(mapLoadManager->maps[i]->bdhc);
        LoadMapNow_SafariZone(matrixIndices[i], i, mapLoadManager->areaDataManager, mapLoadManager->mapMatrix, mapLoadManager->width, mapLoadManager->height, ov01_021FBA00(mapLoadManager->areaDataManager), mapLoadManager);
    }
}

void MapLoadManager_ResetSlots(MapLoadManager *mapLoadManager) {
    int i;

    mapLoadManager->currentSlot = 0;
    mapLoadManager->nextSlot = 0;
    mapLoadManager->numActiveSlots = 0;
    for (i = 0; i < 2; i++) {
        mapLoadManager->slots[i].active = FALSE;
        mapLoadManager->slots[i].finished = TRUE;
    }
    MapLoadManager_ClearMapStale(mapLoadManager);
}

void MapLoadManager_SetPosition(int x, int z, MapLoadManager *mapLoadManager) {
    mapLoadManager->currentTile = TileCoords_ToIndex(x, z, mapLoadManager->stride);
    mapLoadManager->quadrant = TileIndex_GetQuadrant(mapLoadManager->currentTile, mapLoadManager->stride);
    mapLoadManager->currentMapSlot = Quadrant_ToMapSlot(mapLoadManager->quadrant);
    mapLoadManager->currentMatrixIndex = ov01_021F5A30(mapLoadManager->currentTile, mapLoadManager->width, mapLoadManager->stride);
}

// The world position of a map's origin (its altitude from the map matrix)
void CalcMapPosition(int matrixIndex, int width, MapMatrix *mapMatrix, VecFx32 *pos) {
    pos->x = FX32_CONST(256);
    pos->z = FX32_CONST(256);
    if (matrixIndex != -1) {
        u16 x = matrixIndex % width;
        u16 z = matrixIndex / width;
        int matrixWidth = MapMatrix_GetWidth(mapMatrix);
        u8 matrixId = MapMatrix_GetMatrixId(mapMatrix);
        pos->y = MapMatrix_GetMapAltitude(mapMatrix, matrixId, x, z, matrixWidth) << 15;
        pos->x += x << 21;
        pos->z += z << 21;
    }
}

MapLoadManager *MapLoadManager_New(MapMatrix *mapMatrix, void *areaDataManager, void *renderObjManager, MapPropAnimationManager *mapPropAnimationManager, void *unkCC, int mode, SaveData *saveData) {
    MapLoadManager *mapLoadManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(MapLoadManager));
    BOOL withSecondary;

    MI_CpuFill8(mapLoadManager, 0, sizeof(MapLoadManager));
    if (mode == 0) {
        mapLoadManager->funcs = &sMapLoadFuncs_Default;
        withSecondary = TRUE;
    } else if (mode == 1) {
        mapLoadManager->funcs = &sMapLoadFuncs_NoCollision;
        withSecondary = FALSE;
    } else if (mode == 2) {
        mapLoadManager->funcs = &sMapLoadFuncs_SafariZone;
        withSecondary = TRUE;
    } else {
        GF_AssertFail();
    }
    mapLoadManager->saveData = saveData;
    mapLoadManager->buffers = MapLoadBuffers_New(withSecondary);
    mapLoadManager->areaDataManager = areaDataManager;
    mapLoadManager->mapMatrix = mapMatrix;
    mapLoadManager->renderObjManager = renderObjManager;
    mapLoadManager->width = MapMatrix_GetWidth(mapMatrix);
    mapLoadManager->height = MapMatrix_GetHeight(mapMatrix);
    mapLoadManager->stride = mapLoadManager->width << 5;
    mapLoadManager->mapPropAnimationManager = mapPropAnimationManager;
    mapLoadManager->areaMapAnimation = unkCC;
    mapLoadManager->tracking.enabled = TRUE;
    MapLoadManager_ResetSlots(mapLoadManager);
    mapLoadManager->ended = FALSE;
    mapLoadManager->state = MAP_LOAD_STATE_IDLE;
    mapLoadManager->narc = NARC_New(NARC_fielddata_landdata_land_data, HEAP_ID_FIELD1);
    mapLoadManager->mapLoadedCallback = NULL;
    return mapLoadManager;
}

void MapLoadManager_InitialLoad(MapLoadManager *mapLoadManager, int x, int z) {
    mapLoadManager->funcs->initialLoad(mapLoadManager, x, z, mapLoadManager->stride);
    MapLoadManager_SetPosition(x, z, mapLoadManager);
}

// Called for each tile the tracked position moves; scrolls the window when the player enters another quadrant of
// the current map
void MapLoadManager_OnTileStep(u8 direction, MapLoadManager *mapLoadManager) {
    int tile = TileIndex_Step(direction, mapLoadManager->height, mapLoadManager->stride, mapLoadManager->currentTile);
    int matrixIndex = TileIndex_ToMatrixIndex(tile, mapLoadManager->width, mapLoadManager->stride);
    int quadrant = TileIndex_GetQuadrant(tile, mapLoadManager->stride);
    if (mapLoadManager->currentMatrixIndex == matrixIndex && mapLoadManager->quadrant != quadrant) {
        MapLoadManager_Scroll(matrixIndex, direction, mapLoadManager);
        ov01_021F5B24(direction, mapLoadManager->quadrant, &mapLoadManager->currentMapSlot);
    }
}

void MapLoadManager_RenderLoadedMaps(MapLoadManager *mapLoadManager, ModelAttributes *modelAttributes) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (mapLoadManager->maps[i]->matrixIndex != -1) {
            MapLoadManager_RenderLoadedMap(i, mapLoadManager, modelAttributes);
        }
    }
}

void PositionToTile(fx32 x, fx32 z, int *tileX, int *tileZ) {
    *tileX = x / MAP_TILE_SIZE;
    *tileZ = z / MAP_TILE_SIZE;
}

#pragma force_active on
void MapLoadManager_End(MapLoadManager *mapLoadManager) {
    u8 i;

    FreeFileCache();
    NNS_GfdResetLnkTexVramState();
    NNS_GfdResetLnkPlttVramState();
    for (i = 0; i < 2; i++) {
        MapLoadSlot_CancelTasks(&mapLoadManager->slots[i]);
    }
    for (i = 0; i < 4; i++) {
        mapLoadManager->maps[i]->modelLoaded = FALSE;
        Bdhc_Free(mapLoadManager->maps[i]->bdhc);
        MapPropManager_Free(mapLoadManager->maps[i]->propManager);
        if (mapLoadManager->maps[i]->permissionBytes != NULL) {
            Heap_Free(mapLoadManager->maps[i]->permissionBytes);
            mapLoadManager->maps[i]->permissionBytes = NULL;
        }
        Heap_Free(mapLoadManager->maps[i]);
        mapLoadManager->maps[i] = NULL;
    }
    mapLoadManager->state = MAP_LOAD_STATE_END;
    mapLoadManager->ended = TRUE;
    mapLoadManager->tracking.enabled = FALSE;
}
#pragma force_active reset

void MapLoadManager_FreeNARCAndLoadedMapBuffers(MapLoadManager *mapLoadManager) {
    NARC_Delete(mapLoadManager->narc);
    MapLoadBuffers_Free(mapLoadManager->buffers);
    Heap_Free(mapLoadManager);
}

BOOL MapLoadManager_HasEnded(MapLoadManager *mapLoadManager) {
    if (mapLoadManager->ended == TRUE && mapLoadManager->numActiveSlots == 0) {
        return TRUE;
    }
    return FALSE;
}

void MapLoadManager_TrackTarget(VecFx32 *vec, MapLoadManager *mapLoadManager) {
    mapLoadManager->tracking.pos = *vec;
    mapLoadManager->tracking.target = vec;
}

void MapLoadManager_ForgetTrackedTarget(MapLoadManager *mapLoadManager) {
    mapLoadManager->tracking.target = NULL;
}

void MapLoadManager_GetMapPropManager(int mapSlot, MapLoadManager *mapLoadManager, MapPropManager **mapPropManager) {
    *mapPropManager = mapLoadManager->maps[mapSlot]->propManager;
}

u8 MapLoadManager_GetCurrentMapSlot(MapLoadManager *mapLoadManager) {
    return mapLoadManager->currentMapSlot;
}

// 0-3: top left, top right, bottom left, bottom right quarter of its map
int TileIndex_GetQuadrant(int tile, int stride) {
    int z = (int)((u32)tile / (u32)stride) % 32;
    int x = tile & 0x1F;

    if (x < 16) {
        if (z < 16) {
            return 0;
        }
        return 2;
    }
    if (z < 16) {
        return 1;
    }
    return 3;
}

#pragma force_active on
// The window cell holding the map at matrixIndex, 4 if it is not loaded
u8 MapLoadManager_GetMapSlotOfMatrixIndex(int matrixIndex, int unused, MapLoadManager *mapLoadManager) {
    if (mapLoadManager->currentMatrixIndex == matrixIndex) {
        return mapLoadManager->currentMapSlot;
    }
    GF_ASSERT(matrixIndex != -1);
    switch (mapLoadManager->currentMapSlot) {
    case 0:
        if (mapLoadManager->width == matrixIndex - mapLoadManager->currentMatrixIndex) {
            return mapLoadManager->currentMapSlot + 2;
        }
        if (matrixIndex - mapLoadManager->currentMatrixIndex == 1 && MapMatrixIndex_IsSameRow(mapLoadManager->currentMatrixIndex, matrixIndex, mapLoadManager->width)) {
            return mapLoadManager->currentMapSlot + 1;
        }
        if (mapLoadManager->width + 1 == matrixIndex - mapLoadManager->currentMatrixIndex && !MapMatrixIndex_IsSameRow(mapLoadManager->currentMatrixIndex, matrixIndex, mapLoadManager->width)) {
            return mapLoadManager->currentMapSlot + 3;
        }
        return 4;
    case 1:
        if (mapLoadManager->width == matrixIndex - mapLoadManager->currentMatrixIndex) {
            return mapLoadManager->currentMapSlot + 2;
        }
        if (mapLoadManager->currentMatrixIndex - matrixIndex == 1 && MapMatrixIndex_IsSameRow(mapLoadManager->currentMatrixIndex, matrixIndex, mapLoadManager->width)) {
            return mapLoadManager->currentMapSlot - 1;
        }
        if (mapLoadManager->width - 1 == matrixIndex - mapLoadManager->currentMatrixIndex && !MapMatrixIndex_IsSameRow(mapLoadManager->currentMatrixIndex, matrixIndex, mapLoadManager->width)) {
            return mapLoadManager->currentMapSlot + 1;
        }
        return 4;
    case 2:
        if (mapLoadManager->width == mapLoadManager->currentMatrixIndex - matrixIndex) {
            return mapLoadManager->currentMapSlot - 2;
        }
        if (mapLoadManager->width - 1 == mapLoadManager->currentMatrixIndex - matrixIndex && !MapMatrixIndex_IsSameRow(mapLoadManager->currentMatrixIndex, matrixIndex, mapLoadManager->width)) {
            return mapLoadManager->currentMapSlot - 1;
        }
        if (matrixIndex - mapLoadManager->currentMatrixIndex == 1 && MapMatrixIndex_IsSameRow(mapLoadManager->currentMatrixIndex, matrixIndex, mapLoadManager->width)) {
            return mapLoadManager->currentMapSlot + 1;
        }
        return 4;
    case 3:
        if (mapLoadManager->width == mapLoadManager->currentMatrixIndex - matrixIndex) {
            return mapLoadManager->currentMapSlot - 2;
        }
        if (mapLoadManager->width + 1 == mapLoadManager->currentMatrixIndex - matrixIndex && !MapMatrixIndex_IsSameRow(mapLoadManager->currentMatrixIndex, matrixIndex, mapLoadManager->width)) {
            return mapLoadManager->currentMapSlot - 3;
        }
        if (mapLoadManager->currentMatrixIndex - matrixIndex == 1 && MapMatrixIndex_IsSameRow(mapLoadManager->currentMatrixIndex, matrixIndex, mapLoadManager->width)) {
            return mapLoadManager->currentMapSlot - 1;
        }
        return 4;
    }
    return 4;
}
#pragma force_active reset

int MapLoadManager_GetMapMatrixIndex(MapLoadManager *mapLoadManager, u8 mapSlot) {
    if (mapSlot >= 4) {
        GF_AssertFail();
    }
    return mapLoadManager->maps[mapSlot]->matrixIndex;
}

#pragma force_active on
#pragma opt_lifetimes off
// The window cell holding tile (x, z); FALSE if its map is not loaded
BOOL MapLoadManager_GetMapSlotAt(MapLoadManager *mapLoadManager, int x, int z, u8 *mapSlot) {
    u32 matrixIndex = (z / 32) * mapLoadManager->width + x / 32;
    u32 slot;
    int stride, tileRow;

    if (matrixIndex >= mapLoadManager->width * mapLoadManager->height && CommState_SetError(1) != 0) {
        return FALSE;
    }
    if (matrixIndex >= mapLoadManager->width * mapLoadManager->height) {
        GF_AssertFail();
    }
    stride = mapLoadManager->stride;
    tileRow = z;
    tileRow = tileRow * stride;
    slot = MapLoadManager_GetMapSlotOfMatrixIndex(matrixIndex, TileIndex_GetQuadrant(x + tileRow, stride), mapLoadManager);
    if (mapSlot != NULL) {
        *mapSlot = slot;
    }
    if (slot > 3) {
        return FALSE;
    }
    return TRUE;
}
#pragma opt_lifetimes reset
#pragma force_active reset

Bdhc *MapLoadManager_GetBdhc(MapLoadManager *mapLoadManager, u8 mapSlot) {
    return mapLoadManager->maps[mapSlot]->bdhc;
}

u16 *MapLoadManager_GetPermissions(MapLoadManager *mapLoadManager, u8 mapSlot) {
    return (u16 *)mapLoadManager->maps[mapSlot];
}

u8 *MapLoadManager_GetPermissionBytes(MapLoadManager *mapLoadManager, u8 mapSlot) {
    return mapLoadManager->maps[mapSlot]->permissionBytes;
}

SoundplateStruct *MapLoadManager_GetSoundplates(MapLoadManager *mapLoadManager, u8 mapSlot) {
    return (SoundplateStruct *)&mapLoadManager->maps[mapSlot]->soundplates;
}

void MapModelLoad_Cancel(SysTask *task) {
    MapModelLoad *load = SysTask_GetData(task);
    load->cancel = TRUE;
}

#pragma force_active on
// Reads the model in chunks of 0xE000 bytes per frame, binds the area texture and sets up the render object
void Task_LoadMapModel(SysTask *task, MapModelLoad *load) {
    if (load->cancel == TRUE) {
        load->state = 5;
    }
    switch (load->state) {
    case 0: {
        int size;
        load->offset = 0;
        size = load->size;
        if (size <= 0xE000) {
            load->state = 2;
        } else {
            size = 0xE000;
            load->state = 1;
        }
        NARC_ReadFile(load->narc, size, (u8 *)*load->modelFile + load->offset);
        load->offset += size;
    } break;
    case 1: {
        int last, size = load->size - load->offset;
        if (size > 0xE000) {
            size = 0xE000;
            last = FALSE;
        } else {
            last = TRUE;
        }
        NARC_ReadFile(load->narc, size, (u8 *)*load->modelFile + load->offset);
        if (last) {
            load->state = 2;
        } else {
            load->offset += size;
        }
    } break;
    case 2:
        if (load->texture && GF3dRender_ResTexIsLoaded(load->texture) == TRUE) {
            GF3dRender_BindModelSet(*load->modelFile, load->texture);
        }
        load->state = 3;
        break;
    case 3: {
        NNSG3dResMdlSet *modelSet = NNS_G3dGetMdlSet(*load->modelFile);
        NNSG3dResMdl *model;
        GF_ASSERT(modelSet->dict.numEntry == 1);
        model = NNS_G3dGetMdlByIdx(modelSet, 0);
        GF_ASSERT(model);
        NNS_G3dRenderObjInit(load->renderObj, model);
        GF_ASSERT(load->renderObj->anmMat == NULL);
        load->renderObj->anmMat = NULL;
        if (AreaMapAnimation_IsLoaded(load->areaMapAnimation) && !sub_02054E20(load->modelNo)) {
            AreaMapAnimation_AddToRenderObj(load->areaMapAnimation, load->renderObj);
        }
        *load->loaded = TRUE;
        load->state = 5;
    } break;
    case 5:
        *load->loading = FALSE;
        Heap_Free(load);
        SysTask_Destroy(task);
        break;
    }
}
#pragma force_active reset

SysTask *StartMapModelLoadTask(NARC *narc, u32 modelNo, int size, NNSG3dRenderObj *renderObj, NNSG3dResFileHeader **modelFile, NNSG3dResTex *texture, AreaMapAnimation *unkCC, BOOL *loaded, BOOL *loading) {
    MapModelLoad *load = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(MapModelLoad));

    load->narc = narc;
    load->modelNo = modelNo;
    load->size = size;
    load->renderObj = renderObj;
    load->modelFile = modelFile;
    load->texture = texture;
    load->areaMapAnimation = unkCC;
    load->state = 0;
    load->loaded = loaded;
    *loaded = FALSE;
    load->loading = loading;
    load->cancel = FALSE;
    return SysTask_CreateOnMainQueue((SysTaskFunc)Task_LoadMapModel, load, 1);
}

#pragma force_active on
NNSG3dResMdl *LoadMapModel(NARC *narc, u32 size, NNSG3dRenderObj *renderObj, NNSG3dResFileHeader **modelFile, NNSG3dResTex *texture) {
    NNSG3dResMdlSet *modelSet;
    NNSG3dResMdl *model;

    NARC_ReadFile(narc, size, *modelFile);
    if (texture && GF3dRender_ResTexIsLoaded(texture) == TRUE) {
        if (!GF3dRender_BindModelSet(*modelFile, texture)) {
            GF_AssertFail();
        }
    }
    modelSet = NNS_G3dGetMdlSet(*modelFile);
    if (modelSet->dict.numEntry != 1) {
        GF_AssertFail();
    }
    modelSet = NNS_G3dGetMdlSet(*modelFile);
    model = NNS_G3dGetMdlByIdx(modelSet, 0);
    if (!model) {
        GF_AssertFail();
    }
    NNS_G3dRenderObjInit(renderObj, model);
    return model;
}
#pragma force_active reset
