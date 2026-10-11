#ifndef POKEHEARTGOLD_FRONTIER_MAP_H
#define POKEHEARTGOLD_FRONTIER_MAP_H

#include "bg_window.h"
#include "gf_3d_vramman.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite_system.h"
#include "sys_task.h"

#define FRONTIER_MAP_SPRITE_MAX     8
#define FRONTIER_MAP_OBJECT_MAX     32
#define FRONTIER_MAP_OBJECT_GFX_MAX 24

#define FRONTIER_MAP_NO_ID 0xFFFF

// Object placement as the frontier scripts and the save copy store it (15 halfwords)
typedef struct FrontierMapObjectTemplate {
    u16 unk_00;
    u16 unk_02;
    u16 id;
    s16 x;
    s16 y;
    u8 unk_0A;
    u8 unk_0B;
    u8 unk_0C;
    u8 filler_0D[0x11];
} FrontierMapObjectTemplate;

// One of the 32 map objects kept in the frontier app's work (survives a map rebuild)
// Scripted movement of a map object (type 0: none)
typedef struct FrontierMapObjectMovement {
    u8 type;
    u8 state;
    s16 data[7];
} FrontierMapObjectMovement;

typedef struct FrontierMapObject {
    UnkStruct_ov42_02228110 *object;
    UnkStruct_ov42_0222903C *objectSprite;
    FrontierMapObjectTemplate template;
    FrontierMapObjectMovement movement;
    u8 unk_36[2];
    void *unk_38;
} FrontierMapObject;

// Object graphics loaded into the map's object graphics manager (24 slots)
typedef struct FrontierMapObjectGfx {
    u16 id;
    u8 unk_02;
    u8 unk_03;
} FrontierMapObjectGfx;

typedef struct FrontierMapSpriteRecord {
    s16 x;
    s16 y;
    u8 resourceId;
    u8 anim;
    u16 frame : 13;
    u16 animActive : 1;
    u16 draw : 1;
    u16 active : 1;
} FrontierMapSpriteRecord;

// Sprite state saved while the map is torn down for another application
typedef struct FrontierMapSpriteState {
    u16 resourceIds[FRONTIER_MAP_SPRITE_MAX];
    FrontierMapSpriteRecord sprites[FRONTIER_MAP_SPRITE_MAX];
} FrontierMapSpriteState;

// Extra sprites the scripts create on the map (resources from NARC a/1/8/4)
typedef struct FrontierMapSpriteSet {
    ManagedSprite *sprites[FRONTIER_MAP_SPRITE_MAX];
    u16 resourceIds[FRONTIER_MAP_SPRITE_MAX];
    u32 animActiveMask;
    u16 loadedResourceIds[FRONTIER_MAP_SPRITE_MAX];
} FrontierMapSpriteSet;

// Map shake (FrtCmd_076): the offsets are added to the scroll and flip sign every interval + 1 frames, count times.
typedef struct FrontierMapShake {
    SysTask *task;
    s16 x;
    s16 y;
    s8 frame;
    s8 interval;
    s8 count;
    u8 unk_0B;
} FrontierMapShake;

// Main screen window 0 set after a delay (FrtCmd_078).
typedef struct FrontierMapWindowTask {
    SysTask *task;
    u8 left;
    u8 top;
    u8 right;
    u8 bottom;
    s16 delay;
    s8 enable;
    u8 unk_0B;
} FrontierMapWindowTask;

typedef struct FrontierMap {
    BgConfig *bgConfig;
    PaletteData *paletteData;
    void *frontier; // 0x08, the frontier application's work
    GF3DVramMan *vramMan;
    u32 unk10; // 0x10, frontier resource manager (ov80_02239960), a handle as in frontier/overlay_80_02239960.h
    UnkStruct_ov42_022280A8 *objectManager;
    UnkStruct_ov42_02227F68 *unk18;
    UnkStruct_ov44_02232914 camera;
    UnkStruct_ov42_02228EDC *objectGfx;
    UnkStruct_ov42_022293B8 *bg3Layer;
    UnkStruct_ov42_022293B8 *bg2Layer;
    UnkStruct_ov42_02229A40 *unk2C;
    UnkStruct_ov42_022299C0 *unk30;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    FrontierMapSpriteSet spriteSet;
    ManagedSprite *unk80[4];
    void *mapEffect; // 0x90, set up by the map's init callback
    SysTask *objectTask;
    SysTask *eventTask;
    SysTask *updateTask;
    SysTask *vblankTask;
    FrontierMapShake shake;           // 0xA4
    FrontierMapWindowTask windowTask; // 0xB0
    u8 unk_BC[5];
    u8 mapId;
    u8 unk_C2[2];
} FrontierMap;

void *FrontierMap_Init(void *frontier);
void FrontierMap_Free(FrontierMap *map);
void FrontierMap_SaveObjects(FrontierMap *map);
void FrontierMap_RestoreObjects(FrontierMap *map);
void FrontierMap_VBlank(FrontierMap *map);
void FrontierMap_VBlankTask(void);
void FrontierMap_ObjectTask(SysTask *task, FrontierMap *map);
void FrontierMap_EventTask(SysTask *task, FrontierMap *map);
void FrontierMap_Update(SysTask *task, FrontierMap *map);
void FrontierMap_Scroll(FrontierMap *map);
void FrontierMap_ScrollToCamera(FrontierMap *map);
void FrontierMap_ProcessEvents(FrontierMap *map);
void FrontierMap_SetVramBank(BgConfig *bgConfig, u32 mapId);
void FrontierMap_LoadPaletteData(FrontierMap *map);
void FrontierMap_LoadSubScreenGfx(FrontierMap *map);
void FrontierMap_InitMapObjects(FrontierMap *map, u32 mapId, void *profile);
void FrontierMap_FreeMapObjects(FrontierMap *map);
GF3DVramMan *FrontierMap_Create3DVramMan(enum HeapID heapID);
void FrontierMap_Init3D(void);
void FrontierMap_Delete3DVramMan(GF3DVramMan *vramMan);
void FrontierMap_InitSpriteSystem(FrontierMap *map);
void FrontierMap_FreeSpriteSystem(FrontierMap *map);
void FrontierMap_AddObjectGfx(FrontierMap *map, const FrontierMapObjectGfx *gfx);
void FrontierMap_RemoveObjectGfx(FrontierMap *map, u16 id);
UnkStruct_ov42_02228110 *FrontierMap_CreateObject(FrontierMap *map, const FrontierMapObjectTemplate *template, int index);
void FrontierMap_DeleteObject(FrontierMap *map, UnkStruct_ov42_02228110 *object);
void FrontierMap_GetObjectById(FrontierMap *map, u16 id, UnkStruct_ov42_02228110 **object, UnkStruct_ov42_0222903C **objectSprite);
void FrontierMap_AddLoadedResource(FrontierMap *map, u16 resourceId);
void FrontierMap_RemoveLoadedResource(FrontierMap *map, u16 resourceId);
void FrontierMap_DeleteSprite(FrontierMap *map, u32 index);
ManagedSprite *FrontierMap_GetSprite(FrontierMap *map, u32 index);
u32 FrontierMap_IsSpriteAnimActive(FrontierMap *map, u16 index);
void FrontierMap_SaveSprites(FrontierMap *map);
void FrontierMap_RestoreSprites(FrontierMap *map);
void FrontierMap_GetCameraPos(FrontierMap *map, u16 *x, u16 *y);
void FrontierMapObject_GetTemplate(FrontierMapObject *object, u16 *dest);
void FrontierMap_SetObject(void *frontier, u32 index, UnkStruct_ov42_02228110 *object, UnkStruct_ov42_0222903C *objectSprite, const FrontierMapObjectTemplate *template);
FrontierMapObject *FrontierMap_FindObject(void *frontier, u32 id);

typedef struct FrontierMapTileBlink FrontierMapTileBlink;
typedef struct FrontierMapBgAnim FrontierMapBgAnim;

FrontierMapTileBlink *FrontierMapTileBlink_Create(BgConfig *bgConfig);
void FrontierMapTileBlink_Free(FrontierMapTileBlink *blink);
u32 ov80_022385D8(u32 value);
u32 ov80_02238610(u32 value);

void FrontierMapSprite_LoadResources(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NARC *narc, PaletteData *paletteData, u16 kind);
void FrontierMapSprite_UnloadResources(SpriteManager *spriteManager, u32 kind);
ManagedSprite *FrontierMapSprite_Create(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u32 kind);
void FrontierMapSprite_Delete(ManagedSprite *sprite);

void FrontierMap_SetObjectMovement(void *frontier, u32 id, u32 type, const s16 *params, s32 count);
void FrontierMap_RunObjectMovement(void *frontier, FrontierMapObject *object);
void FrontierMap_RunObjectMovements(void *frontier);
BOOL FrontierMapMovement_LookAround(void *frontier, FrontierMapObject *object);

FrontierMapBgAnim *FrontierMapBgAnim_Create(BgConfig *bgConfig, PaletteData *paletteData);
void FrontierMapBgAnim_Free(FrontierMapBgAnim *anim);

#endif
