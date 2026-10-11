#include "bg_cell_manager.h"

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"

// A cell is a rectangular tilemap placed on a BG layer at (x, y) in tiles, clipped to [xMin, xMax) x [yMin, yMax);
// after drawing or erasing, the manager's transfer mode picks sBgCellTransferFuncs[] (none / commit / schedule).

typedef struct BgCell {
    u16 *tilemap;
    u16 width : 6;
    u16 height : 6;
    u16 hidden : 1;
    u16 moveState : 3;
    s8 x;
    s8 y;
    s8 dx;
    s8 dy;
    u8 bgId;
    u8 moveFrames;
    s8 xMin;
    s8 xMax;
    s8 yMin;
    s8 yMax;
} BgCell;

struct BgCellManager {
    BgConfig *bgConfig;
    BgCell *cells;
    u16 count;
    u16 transferMode : 15;
    u16 moving : 1;
    enum HeapID heapId;
};

extern void (*const sBgCellTransferFuncs[])(BgConfig *, u8);

void BgCellManager_LoadCellTilemap(BgCellManager *manager, int index, int narcId, int fileId, int compressed);
void BgCellManager_DrawCell(BgCellManager *manager, int index);
BOOL BgCellManager_UpdateCellMove(BgCellManager *manager, int index);
u8 BgCellManager_GetCellBgId(BgCellManager *manager, int index);
void BgCellManager_GetCellPosition(BgCellManager *manager, int index, s8 *x, s8 *y);
void BgCellManager_PutWindowTiles(BgCellManager *manager, int index, Window *window);
void BgCellManager_NoTransfer(BgConfig *bgConfig, u8 bgId);

BgCellManager *BgCellManager_New(BgConfig *bgConfig, int transferMode, int count, enum HeapID heapId) {
    BgCellManager *manager = Heap_Alloc(heapId, sizeof(BgCellManager));
    manager->bgConfig = bgConfig;
    manager->count = count;
    manager->heapId = heapId;
    manager->transferMode = transferMode;
    manager->moving = 0;
    manager->cells = Heap_Alloc(heapId, count * sizeof(BgCell));
    MI_CpuFill8(manager->cells, 0, count * sizeof(BgCell));
    return manager;
}

void BgCellManager_Delete(BgCellManager *manager) {
    u32 i;
    for (i = 0; i < manager->count; i++) {
        if (manager->cells[i].tilemap != NULL) {
            Heap_Free(manager->cells[i].tilemap);
        }
    }
    Heap_Free(manager->cells);
    Heap_Free(manager);
}

void BgCellManager_InitCell(BgCellManager *manager, int index, int bgId, int width, int height) {
    BgCell *cell = &manager->cells[index];
    cell->tilemap = Heap_Alloc(manager->heapId, width * height * 2);
    cell->width = width;
    cell->height = height;
    cell->bgId = bgId;
    cell->x = 0;
    cell->y = 0;
    cell->moveState = 0;
    cell->hidden = 1;
    cell->xMin = 0;
    cell->xMax = 32;
    cell->yMin = 0;
    cell->yMax = 24;
}

void BgCellManager_SetCellTilemap(BgCellManager *manager, int index, u16 *src) {
    BgCell *cell = &manager->cells[index];
    MIi_CpuCopy16(src, cell->tilemap, cell->width * cell->height * 2);
}

void BgCellManager_LoadCellTilemap(BgCellManager *manager, int index, int narcId, int fileId, int compressed) {
    NNSG2dScreenData *screen;
    void *file = GfGfxLoader_GetScrnData((NarcId)narcId, fileId, compressed, &screen, manager->heapId);
    BgCellManager_SetCellTilemap(manager, index, (u16 *)screen->rawData);
    Heap_Free(file);
}

void BgCellManager_LoadCellTilemapFromOpenNarc(BgCellManager *manager, int index, NARC *narc, int fileId, int compressed) {
    NNSG2dScreenData *screen;
    void *file = GfGfxLoader_GetScrnDataFromOpenNarc(narc, fileId, compressed, &screen, manager->heapId);
    BgCellManager_SetCellTilemap(manager, index, (u16 *)screen->rawData);
    Heap_Free(file);
}

#pragma force_active on

void BgCellManager_DrawCellAt(BgCellManager *manager, int index, int x, int y) {
    BgCell *cell = &manager->cells[index];
    u8 copyWidth;
    u8 destX, srcX;
    s16 width, xMax, height, yMax;
    s32 endX, endY, xMin;
    u8 copyHeight, srcY, destY;
    int yMin;

    cell->x = x;
    cell->y = y;
    xMax = cell->xMax;
    if (x >= xMax) {
        return;
    }
    yMax = cell->yMax;
    if (y >= yMax) {
        return;
    }
    width = cell->width;
    endX = x + width;
    xMin = cell->xMin;
    if (endX < xMin) {
        return;
    }
    height = cell->height;
    endY = y + height;
    yMin = cell->yMin;
    if (endY < yMin) {
        return;
    }
    destX = x;
    copyWidth = width;
    srcX = 0;
    if (x < xMin) {
        destX = xMin;
        copyWidth -= xMin - x;
        srcX = xMin - x;
    }
    if (endX >= xMax) {
        copyWidth -= endX - xMax;
    }
    destY = y;
    copyHeight = height;
    srcY = 0;
    if (y < yMin) {
        destY = yMin;
        copyHeight -= yMin - y;
        srcY = yMin - y;
    }
    if (endY >= yMax) {
        copyHeight -= endY - yMax;
    }
    CopyToBgTilemapRect(manager->bgConfig, cell->bgId, destX, destY, copyWidth, copyHeight, cell->tilemap, srcX, srcY, (u8)width, (u8)height);
    sBgCellTransferFuncs[manager->transferMode](manager->bgConfig, cell->bgId);
    cell->hidden = 0;
}

#pragma force_active off

void BgCellManager_DrawCell(BgCellManager *manager, int index) {
    BgCellManager_DrawCellAt(manager, index, manager->cells[index].x, manager->cells[index].y);
}

#pragma force_active on

void BgCellManager_EraseCell(BgCellManager *manager, int index) {
    BgCell *cell = &manager->cells[index];
    s8 eraseWidth, eraseHeight;
    s8 yMax;
    int width;
    s8 destX;
    u16 height;
    int y0;
    s8 yMin;
    s8 x;
    int endX;
    s8 xMax;
    s8 xMin;
    int endY;
    s8 y;

    cell->hidden = 1;
    xMax = cell->xMax;
    x = cell->x;
    if (x >= xMax) {
        return;
    }
    yMax = cell->yMax;
    y = cell->y;
    if (y >= yMax) {
        return;
    }
    width = cell->width;
    endX = x + width;
    xMin = cell->xMin;
    if (endX < xMin) {
        return;
    }
    height = cell->height;
    y0 = y;
    endY = y + height;
    yMin = cell->yMin;
    if (endY < yMin) {
        return;
    }
    destX = x;
    eraseWidth = width;
    if (x < xMin) {
        destX = xMin;
        eraseWidth -= xMin - x;
    }
    if (endX >= xMax) {
        eraseWidth -= endX - xMax;
    }
    eraseHeight = height;
    if (y < yMin) {
        y = yMin;
        eraseHeight -= yMin - y0;
    }
    if (endY >= yMax) {
        eraseHeight -= endY - yMax;
    }
    FillBgTilemapRect(manager->bgConfig, cell->bgId, 0, destX, y, eraseWidth, eraseHeight, 16);
    sBgCellTransferFuncs[manager->transferMode](manager->bgConfig, cell->bgId);
}

#pragma force_active off

void BgCellManager_StartCellMove(BgCellManager *manager, int index, int dx, int dy, u8 frames) {
    BgCell *cell = &manager->cells[index];
    cell->dx = dx;
    cell->dy = dy;
    cell->moveFrames = frames;
    cell->moveState = 1;
    manager->moving = 1;
}

void BgCellManager_Update(BgCellManager *manager) {
    u32 i;
    if (manager->moving) {
        manager->moving = 0;
        for (i = 0; i < manager->count; i++) {
            if (BgCellManager_UpdateCellMove(manager, i) == TRUE) {
                manager->moving = 1;
            }
        }
    }
}

BOOL BgCellManager_UpdateCellMove(BgCellManager *manager, int index) {
    if (manager->cells[index].moveState == 0) {
        return FALSE;
    }
    BgCellManager_EraseCell(manager, index);
    manager->cells[index].x += manager->cells[index].dx;
    manager->cells[index].y += manager->cells[index].dy;
    BgCellManager_DrawCell(manager, index);
    manager->cells[index].moveFrames--;
    if (manager->cells[index].moveFrames == 0) {
        manager->cells[index].moveState = 0;
        return FALSE;
    }
    return TRUE;
}

BOOL BgCellManager_IsCellMoving(BgCellManager *manager, int index) {
    return manager->cells[index].moveState;
}

void BgCellManager_SetCellPalette(BgCellManager *manager, int index, u8 x, u8 y, u8 width, u8 height, u8 palette) {
    BgCell *cell = &manager->cells[index];
    u16 *tilemap = cell->tilemap;
    int cellWidth = cell->width;
    u16 row, col;

    for (row = y; row < y + height; row++) {
        for (col = x; col < x + width; col++) {
            tilemap[row * cellWidth + col] = (tilemap[row * cellWidth + col] & 0xFFF) | (palette << 12);
        }
    }
}

void BgCellManager_PutWindowTiles(BgCellManager *manager, int index, Window *window) {
    BgCell *cell = &manager->cells[index];
    u16 *tilemap = cell->tilemap;
    u32 cellWidth = cell->width;
    u16 tile = window->baseTile;
    u16 palette = window->paletteNum << 12;
    u16 row, col;

    for (row = 0; row < window->height; row++) {
        if (window->tilemapTop + row >= manager->cells[index].height) {
            break;
        }
        for (col = 0; col < window->width; col++) {
            if (window->tilemapLeft + col >= manager->cells[index].width) {
                break;
            }
            tilemap[cellWidth * (window->tilemapTop + row) + window->tilemapLeft + col] = palette + (tile + col);
        }
        tile += window->width;
    }
}

u16 *BgCellManager_GetCellTilemap(BgCellManager *manager, int index) {
    return manager->cells[index].tilemap;
}

u8 BgCellManager_GetCellBgId(BgCellManager *manager, int index) {
    return manager->cells[index].bgId;
}

void BgCellManager_GetCellPosition(BgCellManager *manager, int index, s8 *x, s8 *y) {
    if (x != NULL) {
        *x = manager->cells[index].x;
    }
    if (y != NULL) {
        *y = manager->cells[index].y;
    }
}

void BgCellManager_GetCellSize(BgCellManager *manager, int index, u16 *width, u16 *height) {
    if (width != NULL) {
        *width = manager->cells[index].width;
    }
    if (height != NULL) {
        *height = manager->cells[index].height;
    }
}

void BgCellManager_SetCellClipRect(BgCellManager *manager, int index, s8 xMin, s8 xMax, s8 yMin, s8 yMax) {
    manager->cells[index].xMin = xMin;
    manager->cells[index].xMax = xMax;
    manager->cells[index].yMin = yMin;
    manager->cells[index].yMax = yMax;
}

void BgCellManager_NoTransfer(BgConfig *bgConfig, u8 bgId) {
}
