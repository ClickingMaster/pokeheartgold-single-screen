#ifndef POKEHEARTGOLD_BG_CELL_MANAGER_H
#define POKEHEARTGOLD_BG_CELL_MANAGER_H

#include "bg_window.h"
#include "filesystem.h"

typedef struct BgCellManager BgCellManager;

BgCellManager *BgCellManager_New(BgConfig *bgConfig, int transferMode, int count, enum HeapID heapId);
void BgCellManager_Delete(BgCellManager *manager);
void BgCellManager_InitCell(BgCellManager *manager, int index, int bgId, int width, int height);
void BgCellManager_SetCellTilemap(BgCellManager *manager, int index, u16 *src);
void BgCellManager_LoadCellTilemapFromOpenNarc(BgCellManager *manager, int index, NARC *narc, int fileId, int compressed);
void BgCellManager_DrawCellAt(BgCellManager *manager, int index, int x, int y);
void BgCellManager_EraseCell(BgCellManager *manager, int index);
void BgCellManager_StartCellMove(BgCellManager *manager, int index, int dx, int dy, u8 frames);
void BgCellManager_Update(BgCellManager *manager);
BOOL BgCellManager_IsCellMoving(BgCellManager *manager, int index);
void BgCellManager_SetCellPalette(BgCellManager *manager, int index, u8 x, u8 y, u8 width, u8 height, u8 palette);
u16 *BgCellManager_GetCellTilemap(BgCellManager *manager, int index);
void BgCellManager_GetCellSize(BgCellManager *manager, int index, u16 *width, u16 *height);
void BgCellManager_SetCellClipRect(BgCellManager *manager, int index, s8 xMin, s8 xMax, s8 yMin, s8 yMax);

#endif // POKEHEARTGOLD_BG_CELL_MANAGER_H
