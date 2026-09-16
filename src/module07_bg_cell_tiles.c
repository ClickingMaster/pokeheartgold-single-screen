#include "module07_bg_cell_tiles_private.h"

void sub_020199F4(Module07BgCellManager *manager, int index, u8 x, u8 y, u8 width, u8 height, u8 palette) {
    Module07BgCell *cell = &manager->cells[index];
    u16 *buffer = cell->buffer;
    int tilesW = cell->tilesW;
    u16 row, col;

    for (row = y; row < y + height; row++) {
        for (col = x; col < x + width; col++) {
            buffer[row * tilesW + col] = (buffer[row * tilesW + col] & 0xFFF) | (palette << 12);
        }
    }
}

void sub_02019A60(Module07BgCellManager *manager, int index, Module07CellWindow *window) {
    Module07BgCell *cell = &manager->cells[index];
    u16 *buffer = cell->buffer;
    u32 tilesW = cell->tilesW;
    u16 baseTile = window->baseTile;
    u16 palette = window->palette << 12;
    u16 row, col;

    for (row = 0; row < window->height; row++) {
        if (window->tilemapTop + row >= manager->cells[index].tilesH) {
            break;
        }
        for (col = 0; col < window->width; col++) {
            if (window->tilemapLeft + col >= manager->cells[index].tilesW) {
                break;
            }
            buffer[tilesW * (window->tilemapTop + row) + window->tilemapLeft + col] = palette + (baseTile + col);
        }
        baseTile += window->width;
    }
}
