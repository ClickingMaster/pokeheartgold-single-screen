#include <string.h>

#include "global.h"

// LoadWindowFrameIconTiles only matches when BgGetCharPtr and BG_LoadCharTilesData get the background id as an
// int (the u8 prototypes in bg_window.h narrow it at both calls), so this file declares what it uses itself
// instead of including bg_window.h; the volatile locals keep the stack layout and store order.

typedef struct WindowView {
    void *bgConfig;
} WindowView; // leading member of Window

typedef struct CharDataView {
    u8 filler_00[0x14];
    void *pRawData;
} CharDataView; // NNSG2dCharacterData

int BgConfig_GetHeapId(void *bgConfig);
u8 GetWindowBgId(WindowView *window);
void *Heap_Alloc(int heapId, u32 size);
void *BgGetCharPtr(int bgId);
void *GfGfxLoader_GetCharData(int narcId, int member, int isCompressed, CharDataView **charData, int heapId);
void BlitPixelRect4Bit(void *src, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight, void *dest, u8 destWidth, u8 destHeight, u8 destX, u8 destY, u8 width, u8 height);
void BG_LoadCharTilesData(void *bgConfig, int bgId, void *data, u32 size, u32 tileStart);
void Heap_Free(void *ptr);
void LoadWindowFrameIconTiles(WindowView *window, int member, int baseTile, int count, u8 srcX, u8 srcY);

// Builds `count` copies of the 2x2 block of frame tiles 10/11, draws icon `member` of NARC a/0/3/8 over them
// (starting at srcX/srcY in the icon) and loads the result at baseTile + 18.
#pragma force_active on

void LoadWindowFrameIconTiles(WindowView *window, int member, int baseTile, int count, u8 srcX, u8 srcY) {
    struct {
        volatile int baseTile;
        volatile int count;
        u32 bgId;
        volatile u32 size;
        void *charFile;
        void *iconPixels;
        u32 tile10Offset;
        u32 tile11Offset;
        CharDataView *charData;
    } locals;

    int heapId;
    u8 *bgChars;
    u8 *buffer;
    u8 i;
    u8 width;
    u8 height;

    locals.baseTile = baseTile;
    locals.count = count;
    heapId = BgConfig_GetHeapId(window->bgConfig);
    locals.bgId = GetWindowBgId(window);
    locals.size = locals.count << 7;
    buffer = Heap_Alloc(heapId, locals.size);
    bgChars = BgGetCharPtr(locals.bgId);
    locals.charFile = GfGfxLoader_GetCharData(38, member, 0, &locals.charData, heapId);
    locals.iconPixels = locals.charData->pRawData;
    i = 0;
    if (locals.count > 0) {
        locals.tile10Offset = (locals.baseTile + 10) << 5;
        locals.tile11Offset = (locals.baseTile + 11) << 5;
        do {
            memcpy(buffer + (i << 7), bgChars + locals.tile10Offset, 32);
            memcpy(buffer + ((i << 7) + 32), bgChars + locals.tile11Offset, 32);
            memcpy(buffer + ((i << 7) + 64), bgChars + locals.tile10Offset, 32);
            memcpy(buffer + ((i << 7) + 96), bgChars + locals.tile11Offset, 32);
            i++;
        } while (i < locals.count);
    }
    height = (16 - srcY) * locals.count;
    width = 16 - srcX;
    BlitPixelRect4Bit(locals.iconPixels, srcX, srcY, width, height, buffer, width, height, 0, 0, width, height);
    {
        u32 tile = locals.baseTile;
        u32 size = locals.size;
        tile += 18;
        locals.baseTile = tile;
        BG_LoadCharTilesData(window->bgConfig, locals.bgId, buffer, size, tile);
    }
    Heap_Free(locals.charFile);
    Heap_Free(buffer);
}
