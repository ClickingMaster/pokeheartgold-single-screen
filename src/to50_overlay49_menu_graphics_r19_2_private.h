#ifndef TO50_OVERLAY49_MENU_GRAPHICS_R19_2_PRIVATE_H
#define TO50_OVERLAY49_MENU_GRAPHICS_R19_2_PRIVATE_H

#include "global.h"

typedef struct Ov49Graphics {
    void *bgConfig;
    void *spriteList;
    u8 padding08[0x128];
    void *resourceManagers[4];
    void *cellTransfer;
    void *vramManager;
} Ov49Graphics;

extern u8 gSystem[];
extern const u32 ov49_02269724[];
extern const u32 ov49_02269734[];
extern const u32 ov49_02269744[];
extern const u32 ov49_022697CC[];
typedef struct Ov49BgTemplate {
    u8 raw[0x1C];
} Ov49BgTemplate;
extern const Ov49BgTemplate ov49_0226981C[];
extern void ov49_0225A854(void);
void ov49_0225A5EC(Ov49Graphics *work, void *saveData, u32 heapId);

void GF_CreateVramTransferManager(u32 count, u32 heapId);
void GfGfx_SetBanks(const void *banks);
void GfGfx_SwapDisplay(void);
void BG_SetMaskColor(u32 screen, u32 color);
void SetBothScreensModesAndDisable(const void *modes);
void *BgConfig_Alloc(u32 heapId);
void InitBgFromTemplate(void *bgConfig, u8 bgId, const void *template, u32 unused);
void BG_ClearCharDataRange(u8 bgId, u32 size, u32 offset, u32 heapId);
void BgClearTilemapBufferAndCommit(void *bgConfig, u8 bgId);
void *Save_PlayerData_GetOptionsAddr(void *saveData);
u32 Options_GetFrame(void *options);
void LoadFontPal0(u32 screen, u32 offset, u32 heapId);
void LoadFontPal1(u32 screen, u32 offset, u32 heapId);
void LoadUserFrameGfx1(void *bgConfig, u32 bgId, u32 tile, u32 palette, u32 unused, u32 heapId);
void LoadUserFrameGfx2(void *bgConfig, u32 bgId, u32 tile, u32 palette, u32 frame, u32 heapId);
void LoadMapSignpostFrameAndGraphic(void *bgConfig, u32 bgId, u32 tile, u32 palette, u32 type, u32 unused, u32 heapId);
void GfGfxLoader_GXLoadPal(u32 narc, u32 member, u32 screen, u32 offset, u32 size, u32 heapId);
void NNS_G2dInitOamManagerModule(void);
void OamManager_Create(u32, u32, u32, u32, u32, u32, u32, u32, u32);
void ObjCharTransfer_InitEx(const void *, u32, u32);
void ObjPlttTransfer_Init(u32, u32);
void ObjCharTransfer_ClearBuffers(void);
void ObjPlttTransfer_Reset(void);
void G2dRenderer_SetObjCharTransferReservedRegion(u32, u32);
void G2dRenderer_SetPlttTransferReservedRegion(u32);
void WirelessStatusIcon_Create(void);
void *G2dRenderer_Init(u32, void *, u32);
void G2dRenderer_SetSubSurfaceCoords(void *, u32, u32);
void *Create2DGfxResObjMan(u32, u32, u32);
void *sub_02020654(u32, u32);
void GfGfx_EngineATogglePlanes(u32, u32);
void GfGfx_EngineBTogglePlanes(u32, u32);
void *GF_3DVramMan_Create(u32, u32, u32, u32, u32, void (*)(void));

#endif
