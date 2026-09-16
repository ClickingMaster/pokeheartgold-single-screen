#ifndef POKEHEARTGOLD_MODULE07_BG_CELL_TILES_PRIVATE_H
#define POKEHEARTGOLD_MODULE07_BG_CELL_TILES_PRIVATE_H

#include "global.h"

typedef struct Module07BgCell {
    u16 *buffer;
    u16 tilesW : 6;
    u16 tilesH : 6;
    u16 active : 1;
    u16 state : 3;
    s8 x;
    s8 y;
    s8 dx;
    s8 dy;
    u8 bgId;
    u8 timer;
    s8 clipLeft;
    s8 clipRight;
    s8 clipTop;
    s8 clipBottom;
} Module07BgCell;

typedef struct Module07BgCellManager {
    void *bgConfig;
    Module07BgCell *cells;
    u16 count;
    u16 commitMode : 15;
    u16 dirty : 1;
    int heapId;
} Module07BgCellManager;

typedef struct Module07CellWindow {
    u8 filler0[5];
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
} Module07CellWindow;

void sub_020199F4(Module07BgCellManager *, int, u8, u8, u8, u8, u8);
void sub_02019A60(Module07BgCellManager *, int, Module07CellWindow *);

#endif
