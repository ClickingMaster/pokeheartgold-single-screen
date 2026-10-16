#include "battle/battle_terrain.h"

#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_system.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "sprite_system.h"

// Sprite resource ids; the cell and animation of each side use its char id - 8
#define TERRAIN_RES_CHAR_PLAYER 0x4E2D
#define TERRAIN_RES_CHAR_ENEMY  0x4E2E
#define TERRAIN_RES_PALETTE     0x4E29
#define TERRAIN_RES_CELL_PLAYER 0x4E25
#define TERRAIN_RES_CELL_ENEMY  0x4E26

static void BattleTerrain_LoadResources(BattleTerrain *terrain);
static void BattleTerrain_CreateSprite(BattleTerrain *terrain);
static void BattleTerrain_DeleteSprite(BattleTerrain *terrain);
static void BattleTerrain_UnloadResources(BattleTerrain *terrain);
void BattleTerrain_Init(BattleTerrain *terrain, BattleSystem *battleSystem, s32 side, s32 terrainId);

static const u16 sEnemyTerrainCharFiles[] = {
    0x88,
    0x92,
    0x82,
    0x98,
    0x8C,
    0x96,
    0x8E,
    0x86,
    0x8A,
    0x90,
    0x94,
    0x94,
    0x9A,
    0x9C,
    0x9E,
    0xA0,
    0xA2,
    0xA4,
    0xA6,
    0xA8,
    0xAA,
    0xAC,
    0xAE,
    0xB0
};

static const u16 sPlayerTerrainCharFiles[] = {
    0x87,
    0x91,
    0x7F,
    0x97,
    0x8B,
    0x95,
    0x8D,
    0x85,
    0x89,
    0x8F,
    0x93,
    0x97,
    0x99,
    0x9B,
    0x9D,
    0x9F,
    0xA1,
    0xA3,
    0xA5,
    0xA7,
    0xA9,
    0xAB,
    0xAD,
    0xAF
};

static const ManagedSpriteTemplate sTerrainSpriteTemplates[] = {
    {
     .x = 0x150,
     .y = 0x88,
     .z = 0,
     .animation = 0,
     .drawPriority = 0x3E8,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { TERRAIN_RES_CHAR_PLAYER, TERRAIN_RES_PALETTE, TERRAIN_RES_CELL_PLAYER, TERRAIN_RES_CELL_PLAYER, -1, -1 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 0xFFB0,
     .y = 0x58,
     .z = 0,
     .animation = 0,
     .drawPriority = 0x3E8,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { TERRAIN_RES_CHAR_ENEMY, TERRAIN_RES_PALETTE, TERRAIN_RES_CELL_ENEMY, TERRAIN_RES_CELL_ENEMY, -1, -1 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
};

static const u16 sTerrainPaletteFiles[][3] __attribute__((aligned(4))) = {
    { 0x07, 0x08, 0x09 },
    { 0x16, 0x17, 0x18 },
    { 0x01, 0x02, 0x03 },
    { 0x1F, 0x20, 0x21 },
    { 0x0D, 0x0E, 0x0F },
    { 0x1C, 0x1D, 0x1E },
    { 0x10, 0x11, 0x12 },
    { 0x04, 0x05, 0x06 },
    { 0x0A, 0x0B, 0x0C },
    { 0x13, 0x14, 0x15 },
    { 0x19, 0x1A, 0x1B },
    { 0x19, 0x1A, 0x1B },
    { 0x22, 0x23, 0x24 },
    { 0x25, 0x26, 0x27 },
    { 0x28, 0x29, 0x2A },
    { 0x2B, 0x2C, 0x2D },
    { 0x2E, 0x2F, 0x30 },
    { 0x31, 0x32, 0x33 },
    { 0x34, 0x35, 0x36 },
    { 0x37, 0x38, 0x39 },
    { 0x3A, 0x3B, 0x3C },
    { 0x3D, 0x3E, 0x3F },
    { 0x40, 0x41, 0x42 },
    { 0x43, 0x44, 0x45 }
};

static void BattleTerrain_LoadResources(BattleTerrain *terrain) {
    u32 cellAnimResId;
    NARC *narc = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);

    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(terrain->battleSystem);
    SpriteManager *spriteManager = BattleSystem_GetSpriteManager(terrain->battleSystem);
    u32 subfile = BattleSystem_GetBgTimeOfDay(terrain->battleSystem);

    u16 charFile;
    u32 charResId;
    u32 cellFile;
    u32 animFile;
    if (terrain->side == 0) {
        charFile = sPlayerTerrainCharFiles[terrain->terrainId];
        charResId = TERRAIN_RES_CHAR_PLAYER;
        cellFile = 0x80;
        cellAnimResId = charResId - 8;
        animFile = 0x81;
    } else {
        charFile = sEnemyTerrainCharFiles[terrain->terrainId];
        charResId = TERRAIN_RES_CHAR_ENEMY;
        cellFile = 0x83;
        cellAnimResId = charResId - 8;
        animFile = 0x84;
    }
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, charFile, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, charResId);

    PaletteData *palette = BattleSystem_GetPaletteData(terrain->battleSystem);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(palette, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, sTerrainPaletteFiles[terrain->terrainId][subfile], FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, TERRAIN_RES_PALETTE);

    palette = BattleSystem_GetPaletteData(terrain->battleSystem);
    PaletteData_LoadNarc(palette, NARC_a_0_0_8, sTerrainPaletteFiles[terrain->terrainId][subfile], HEAP_ID_BATTLE, PLTTBUF_MAIN_BG, 0x20, 0x70);

    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, cellFile, TRUE, cellAnimResId);

    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, animFile, TRUE, cellAnimResId);

    NARC_Delete(narc);
}

static void BattleTerrain_CreateSprite(BattleTerrain *terrain) {
    SpriteSystem *spriteSystem;
    ManagedSprite *managed;

    spriteSystem = BattleSystem_GetSpriteSystem(terrain->battleSystem);
    managed = SpriteSystem_NewSprite(spriteSystem, BattleSystem_GetSpriteManager(terrain->battleSystem), &sTerrainSpriteTemplates[terrain->side]);
    terrain->sprite = managed;
    Sprite_TickFrame(managed->sprite);
}

static void BattleTerrain_DeleteSprite(BattleTerrain *terrain) {
    if (terrain->sprite) {
        Sprite_DeleteAndFreeResources(terrain->sprite);
        terrain->sprite = NULL;
    }
}

static void BattleTerrain_UnloadResources(BattleTerrain *terrain) {
    SpriteManager *manager = BattleSystem_GetSpriteManager(terrain->battleSystem);
    s32 resId = terrain->side == 0 ? TERRAIN_RES_CHAR_PLAYER : TERRAIN_RES_CHAR_ENEMY;
    u32 cellAnimResId = resId - 8;
    SpriteManager_UnloadCharObjById(manager, resId);
    SpriteManager_UnloadPlttObjById(manager, TERRAIN_RES_PALETTE);
    SpriteManager_UnloadCellObjById(manager, cellAnimResId);
    SpriteManager_UnloadAnimObjById(manager, cellAnimResId);
}

void BattleTerrain_SetDrawFlag(BattleTerrain *terrain, s32 flag) {
    ManagedSprite *sprite = terrain->sprite;
    if (sprite) {
        ManagedSprite_SetDrawFlag(sprite, flag);
    }
}

void BattleTerrain_Init(BattleTerrain *terrain, BattleSystem *battleSystem, s32 side, s32 terrainId) {
    MIi_CpuClearFast(0, (void *)terrain, sizeof(BattleTerrain));
    terrain->battleSystem = battleSystem;
    terrain->side = side;
    terrain->terrainId = terrainId;
    if (terrainId >= 24) {
        GF_AssertFail();
        terrain->terrainId = 0;
    }
    BattleTerrain_LoadResources(terrain);
    BattleTerrain_CreateSprite(terrain);
}

void BattleTerrain_Free(BattleTerrain *terrain) {
    BattleTerrain_DeleteSprite(terrain);
    BattleTerrain_UnloadResources(terrain);
    MIi_CpuClearFast(0, (void *)terrain, sizeof(BattleTerrain));
}
