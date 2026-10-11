#ifndef POKEHEARTGOLD_BG_CELL_MANAGER_H
#define POKEHEARTGOLD_BG_CELL_MANAGER_H

#include "bg_window.h"
#include "filesystem.h"

typedef struct BgCellManager BgCellManager;

BgCellManager *BgCellManager_New(BgConfig *bgConfig, int a1, int a2, enum HeapID a3);
void BgCellManager_Delete(BgCellManager *a0);
void BgCellManager_InitCell(BgCellManager *a0, int a1, int a2, int a3, int a4);
void BgCellManager_SetCellTilemap(BgCellManager *a0, int a1, u16 *a2);
void BgCellManager_LoadCellTilemapFromOpenNarc(BgCellManager *a0, int a1, NARC *narc, int fileId, int a4);
void BgCellManager_DrawCellAt(BgCellManager *a0, int a1, int a2, int a3);
void BgCellManager_EraseCell(BgCellManager *a0, int a1);
void BgCellManager_StartCellMove(BgCellManager *a0, int a1, int a2, int a3, u8 a4);
void BgCellManager_Update(BgCellManager *a0);
BOOL BgCellManager_IsCellMoving(BgCellManager *a0, int a1);
void BgCellManager_SetCellPalette(BgCellManager *a0, int a1, u8 x, u8 y, u8 width, u8 height, u8 palette);
u16 *BgCellManager_GetCellTilemap(BgCellManager *a0, int a1);
void BgCellManager_GetCellSize(BgCellManager *a0, int a1, u16 *a2, u16 *a3);
void BgCellManager_SetCellClipRect(BgCellManager *a0, int a1, s8 a2, s8 a3, s8 a4, s8 a5);

#endif // POKEHEARTGOLD_BG_CELL_MANAGER_H
