#ifndef TO45_OV15_PREFIX_R3_PRIVATE_H
#define TO45_OV15_PREFIX_R3_PRIVATE_H

#include "global.h"

#define PTR(p, off) (*(void **)((u8 *)(p) + (off)))
#define U8(p, off)  (*(u8 *)((u8 *)(p) + (off)))

typedef struct ScreenModes {
    u32 words[4];
} ScreenModes;

typedef struct BagSlotEntry {
    u32 unk0;
    u16 *values;
    u32 unk8;
} BagSlotEntry;

typedef struct BagSlotTable {
    BagSlotEntry entries[8];
    u8 pad60[4];
    u8 selected;
} BagSlotTable;

extern const ScreenModes sBagGraphicsModes;
extern const u8 sBgTemplate_Main1[];
extern const u8 sBgTemplate_Main2[];
extern const u8 sBgTemplate_Main3[];
extern const u8 sBgTemplate_Sub0[];
extern const u8 sBgTemplate_Sub1[];
extern const u8 sBgTemplate_Sub2[];
extern const u8 sBgTemplate_Sub3[];

void SetBothScreensModesAndDisable(const ScreenModes *);
void InitBgFromTemplate(void *, int, const void *, int);
void BgClearTilemapBufferAndCommit(void *, int);
void BG_ClearCharDataRange(int, int, int, int);
void G2x_SetBlendAlpha_(u32, int, int, int, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void FreeBgTilemapBuffer(void *, int);
void Heap_FreeExplicit(int, void *);
void GfGfxLoader_LoadScrnData(int, int, void *, int, int, int, int, int);
void *NewMsgDataFromNarc(int, int, int, int);
void *MessagePrinter_New(int, int, int, int);
void *MessageFormat_New(int);
void *String_New(int, int);
void *NARC_New(int, int);
void GfGfxLoader_LoadCharData(int, int, void *, int, int, int, int, int);
void GfGfxLoader_GXLoadPal(int, int, int, int, int, int);
void LoadFontPal1(int, int, int);
void LoadUserFrameGfx1(void *, int, int, int, int, int);
void LoadUserFrameGfx2(void *, int, int, int, int, int);
int Options_GetFrame(void *);
void *GfGfxLoader_GetPlttData(int, int, void *, int);

void BagApp_InitBgLayers(void *);
void BagApp_FreeBgLayers(void *);
void BagApp_LoadBgGraphics(void *);
void BagApp_LoadInfoPanelTilemap(void *, int);
void BagApp_CreateMessageData(void *);
void BagApp_CountPockets(void *);
u16 BagApp_GetCurrentPocketSlotValue(void *, u16, BOOL);

#endif
