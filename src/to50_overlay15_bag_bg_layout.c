#include "to50_overlay15_bag_bg_layout_private.h"

void BagApp_LoadPocketSubBgPalette(void *work, s32 slot, s32 palette) {
    const u8 *data = PTR(PTR(work, 0x694 + slot * 4), 0xC);

    if (palette <= 7) {
        data += palette * 0x20;
        GXS_LoadBGPltt(data, 0, 0x20);
        GXS_LoadBGPltt(data, 0x20, 0x40);
        GXS_LoadBGPltt(data, 0x60, 0x20);
    }
}

void BagApp_ApplyTilemapCommands(void *bgConfig, s32 bgId, s32 mode) {
    void *buffer = GetBgTilemapBuffer(bgConfig, (u8)bgId);
    const Ov15TileCommandR18 *command;
    s32 i;

    if (mode == 6) {
        return;
    }
    command = sSubScreenTilemapCommands[mode];
    for (i = 0; i < 4; i++, command++) {
        if (command->type == 1) {
            LoadRectToBgTilemapRect(bgConfig, (u8)bgId, (u8 *)buffer + (command->x + (command->y << 5)) * 2, command->bgId, command->destY, command->width, command->height);
        } else if (command->type == 2) {
            FillBgTilemapRect(bgConfig, (u8)bgId, 0, command->bgId, command->destY, command->width, command->height, 0);
        }
    }
}

void BagApp_DrawItemSlotFrames(void *bgConfig, s32 bgId, u32 mode, u32 selection) {
    s32 i;
    void *buffer = GetBgTilemapBuffer(bgConfig, (u8)bgId);
    const Ov15RectR18 *rect;

    i = 0;
    rect = sUnusedSlotRects[mode - 1];
    for (; i < 2; i++, rect++) {
        FillBgTilemapRect(bgConfig, (u8)bgId, 0, rect->x, rect->y, rect->width, rect->height, 0);
    }
    if (selection != (u32)-1) {
        rect = &sItemSlotRects[selection];
        if (selection <= 1) {
            LoadRectToBgTilemapRect(bgConfig, (u8)bgId, (u8 *)buffer + 0x600, rect->x, rect->y, rect->width, rect->height);
        } else {
            LoadRectToBgTilemapRect(bgConfig, (u8)bgId, (u8 *)buffer + 0x6C0, rect->x, rect->y, rect->width, rect->height);
        }
    }
}

void BagApp_LoadSubScreenLayout(void *work, u32 mode, s32 value, s32 selection) {
    Ov15ScreenDataR18 *screen;
    void *allocation;

    GF_ASSERT(mode <= 4);
    switch (mode) {
    case 0:
        allocation = GfGfxLoader_GetScrnData(0xF, 0x2B, FALSE, &screen, 6);
        BG_LoadScreenTilemapData(PTR(work, 0), 5, screen->raw, screen->size);
        DC_FlushRange(GetBgTilemapBuffer(PTR(work, 0), 5), screen->size);
        BagApp_ApplyTilemapCommands(PTR(work, 0), 5, value);
        ScheduleBgTilemapBufferTransfer(PTR(work, 0), 5);
        Heap_Free(allocation);

        allocation = GfGfxLoader_GetScrnData(0xF, 0x27, FALSE, &screen, 6);
        BG_LoadScreenTilemapData(PTR(work, 0), 6, screen->raw, screen->size);
        ScheduleBgTilemapBufferTransfer(PTR(work, 0), 6);
        Heap_Free(allocation);
        BagApp_ShowMoveSlotSprites(work, 0, 0, FALSE);
        break;
    case 1:
        allocation = GfGfxLoader_GetScrnData(0xF, 0x2C, FALSE, &screen, 6);
        BG_LoadScreenTilemapData(PTR(work, 0), 5, screen->raw, screen->size);
        DC_FlushRange(GetBgTilemapBuffer(PTR(work, 0), 5), screen->size);
        BagApp_DrawItemSlotFrames(PTR(work, 0), 5, value, selection);
        ScheduleBgTilemapBufferTransfer(PTR(work, 0), 5);
        Heap_Free(allocation);

        allocation = GfGfxLoader_GetScrnData(0xF, 0x2A, FALSE, &screen, 6);
        BG_LoadScreenTilemapData(PTR(work, 0), 6, screen->raw, screen->size);
        ScheduleBgTilemapBufferTransfer(PTR(work, 0), 6);
        Heap_Free(allocation);
        BagApp_HideItemMenuSprites(work);
        BagApp_ShowMoveSlotSprites(work, value, BagApp_GetMovingItemPageSlot(work), FALSE);
        break;
    case 2:
        allocation = GfGfxLoader_GetScrnData(0xF, 0x2D, FALSE, &screen, 6);
        BG_LoadScreenTilemapData(PTR(work, 0), 6, screen->raw, screen->size);
        ScheduleBgTilemapBufferTransfer(PTR(work, 0), 6);
        Heap_Free(allocation);
        BagApp_ApplyTilemapCommands(PTR(work, 0), 5, value);
        ScheduleBgTilemapBufferTransfer(PTR(work, 0), 5);
        break;
    case 3:
        allocation = GfGfxLoader_GetScrnData(0xF, 0x34, FALSE, &screen, 6);
        BG_LoadScreenTilemapData(PTR(work, 0), 6, screen->raw, screen->size);
        ScheduleBgTilemapBufferTransfer(PTR(work, 0), 6);
        Heap_Free(allocation);
        break;
    case 4:
        allocation = GfGfxLoader_GetScrnData(0xF, 0x35, FALSE, &screen, 6);
        BG_LoadScreenTilemapData(PTR(work, 0), 6, screen->raw, screen->size);
        ScheduleBgTilemapBufferTransfer(PTR(work, 0), 6);
        Heap_Free(allocation);
        break;
    }
}
