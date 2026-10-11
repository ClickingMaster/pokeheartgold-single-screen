#include "to50_overlay49_menu_graphics_r19_2_private.h"

void ov49_0225A5EC(Ov49Graphics *work, void *saveData, u32 heapId) {
    s32 i;
    s32 j;
    u8 frame;

    *(u16 *)0x04000050 = 0;
    *(u16 *)0x04001050 = 0;
    GF_CreateVramTransferManager(0x20, heapId);
    GfGfx_SetBanks(ov49_022697CC);
    gSystem[0x69] = 0;
    GfGfx_SwapDisplay();
    BG_SetMaskColor(0, 0);
    SetBothScreensModesAndDisable(ov49_02269724);
    work->bgConfig = BgConfig_Alloc(heapId);

    for (i = 0; i < 4; i++) {
        InitBgFromTemplate(work->bgConfig, (u8)ov49_02269734[i], &ov49_0226981C[i], 0);
        BG_ClearCharDataRange((u8)ov49_02269734[i], 0x20, 0, heapId);
        BgClearTilemapBufferAndCommit(work->bgConfig, (u8)ov49_02269734[i]);
    }

    frame = (u8)Options_GetFrame(Save_PlayerData_GetOptionsAddr(saveData));
    LoadFontPal0(0, 0xA0, heapId);
    LoadFontPal1(0, 0x80, heapId);
    LoadUserFrameGfx1(work->bgConfig, 1, 0x55, 3, 0, heapId);
    LoadUserFrameGfx2(work->bgConfig, 1, 1, 1, frame, heapId);
    LoadMapSignpostFrameAndGraphic(work->bgConfig, 1, 0x1F, 2, 3, 0, heapId);
    GfGfxLoader_GXLoadPal(0xD1, 0x5A, 0, 0x40, 0x20, heapId);
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, heapId);
    ObjCharTransfer_InitEx(ov49_02269744, 0x10, 0x00100010);
    ObjPlttTransfer_Init(0x18, heapId);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(1, 0x10);
    G2dRenderer_SetPlttTransferReservedRegion(1);
    WirelessStatusIcon_Create();
    work->spriteList = G2dRenderer_Init(0x18, &work->padding08, heapId);
    G2dRenderer_SetSubSurfaceCoords(&work->padding08, 0, 1 << 20);
    for (j = 0; j < 4; j++) {
        work->resourceManagers[j] = Create2DGfxResObjMan(0x18, j, heapId);
    }
    work->cellTransfer = sub_02020654(0x18, heapId);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    work->vramManager = GF_3DVramMan_Create(heapId, 0, 2, 0, 4, ov49_0225A854);
}
