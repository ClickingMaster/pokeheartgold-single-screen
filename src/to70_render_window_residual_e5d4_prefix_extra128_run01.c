#pragma force_active on
#ifndef TO50_RENDER_WINDOW_H
#define TO50_RENDER_WINDOW_H
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
void LoadUserFrameGfx1(BgConfig *, enum GFBgLayer, u32, u32, u8, enum HeapID);
void sub_0200E448(BgConfig *, u8, u8, u8, u8, u8, u8, u16);
void DrawFrameAndWindow1(Window *, BOOL, u16, u8);
void sub_0200E5D4(Window *, BOOL);
#endif

#pragma force_active on
void LoadUserFrameGfx1(BgConfig *bgConfig, enum GFBgLayer layer, u32 numTiles, u32 paletteNumber, u8 frameId, enum HeapID heapID) {
    int member;
    if (frameId != 0) member = 1; else member = 0;
    GfGfxLoader_LoadCharData((NarcId)0x26, member, bgConfig, layer, numTiles, 0, FALSE, heapID);
    if (frameId == 2) member = 0x2E; else member = 0x19;
    if ((u32)layer < GF_BG_LYR_SUB_FIRST) {
        GfGfxLoader_GXLoadPal((NarcId)0x26, member, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(paletteNumber << 5), 32, heapID);
        return;
    }
    GfGfxLoader_GXLoadPal((NarcId)0x26, member, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(paletteNumber << 5), 32, heapID);
}
void sub_0200E448(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 fillValue) {
    FillBgTilemapRect(bgConfig, bgId, fillValue, (u8)(x - 1), (u8)(y - 1), 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, (u16)(fillValue + 1), x, (u8)(y - 1), width, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, (u16)(fillValue + 2), (u8)(x + width), (u8)(y - 1), 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, (u16)(fillValue + 3), (u8)(x - 1), y, 1, height, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, (u16)(fillValue + 5), (u8)(x + width), y, 1, height, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, (u16)(fillValue + 6), (u8)(x - 1), (u8)(y + height), 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, (u16)(fillValue + 7), x, (u8)(y + height), width, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, (u16)(fillValue + 8), (u8)(x + width), (u8)(y + height), 1, 1, paletteNum);
}
void DrawFrameAndWindow1(Window *window, BOOL copyToVram, u16 fillValue, u8 paletteNum) {
    sub_0200E448(window->bgConfig, GetWindowBgId(window), GetWindowX(window), GetWindowY(window), GetWindowWidth(window), GetWindowHeight(window), paletteNum, fillValue);
    if (!copyToVram) CopyWindowToVram(window);
}
