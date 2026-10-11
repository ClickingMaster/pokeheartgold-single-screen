#ifndef TO50_OVERLAY15_PREFIX_R18_PRIVATE_H
#define TO50_OVERLAY15_PREFIX_R18_PRIVATE_H

#include "global.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov15ScreenDataR18 {
    u16 width;
    u16 height;
    u16 colorMode;
    u16 format;
    u32 size;
    u32 raw[1];
} Ov15ScreenDataR18;

typedef struct Ov15TileCommandR18 {
    u8 type;
    u8 x;
    u8 y;
    u8 bgId;
    u8 destY;
    u8 width;
    u8 height;
    u8 padding;
} Ov15TileCommandR18;

typedef struct Ov15RectR18 {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} Ov15RectR18;

extern const Ov15TileCommandR18 sSubScreenTilemapCommands[][4];
extern const Ov15RectR18 sUnusedSlotRects[][2];
extern const Ov15RectR18 sItemSlotRects[];

extern void GXS_LoadBGPltt(const void *, u32, u32);
extern void *GetBgTilemapBuffer(void *, u8);
extern void LoadRectToBgTilemapRect(void *, u8, const void *, u8, u8, u8, u8);
extern void FillBgTilemapRect(void *, u8, u16, u8, u8, u8, u8, u8);
extern void *GfGfxLoader_GetScrnData(int, s32, BOOL, Ov15ScreenDataR18 **, int);
extern void BG_LoadScreenTilemapData(void *, u8, const void *, u32);
extern void ScheduleBgTilemapBufferTransfer(void *, u8);
extern void DC_FlushRange(const void *, u32);
extern void Heap_Free(void *);
extern void GF_AssertFail(void);
extern void BagApp_ShowMoveSlotSprites(void *, s32, s32, BOOL);
extern void BagApp_HideItemMenuSprites(void *);
extern s32 BagApp_GetMovingItemPageSlot(void *);

void BagApp_LoadPocketSubBgPalette(void *, s32, s32);
void BagApp_ApplyTilemapCommands(void *, s32, s32);
void BagApp_DrawItemSlotFrames(void *, s32, u32, u32);
void BagApp_LoadSubScreenLayout(void *, u32, s32, s32);

#endif
