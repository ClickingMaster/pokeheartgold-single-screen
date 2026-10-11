#include "global.h"

#include "overlay_15.h"
#include "system.h"

extern void *memset(void *, int, u32);

extern void HBlankInterruptDisable(void);
extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void *BgConfig_Alloc(u32);
extern u32 PlayerProfile_GetTrainerGender(void *);
extern void BeginNormalPaletteFade(u32, u32, u32, u32, u32, u32, u32);
extern BOOL IsPaletteFadeFinished(void);
extern void SetKeyRepeatTimers(int, int);
extern void TextFlags_SetCanTouchSpeedUpPrint(BOOL);
extern void ToggleBgLayer(u32, u32);
extern void Sound_SetSceneAndPlayBGM(int, int, int);
extern void SpriteSystem_DrawSprites(void *);
extern void GF_TouchpadResetSampling(void);
extern void GF_TouchpadStartSampling(int);
extern void sub_0203A964(void);
extern void sub_020880CC(u32, u32);

extern void BagApp_GetSaveStructPtrs(void *);
extern void BagApp_SetGraphicsBanks(void);
extern void BagApp_VBlankCallback(void *);
extern void BagApp_InitBgLayers(void *);
extern void BagApp_LoadBgGraphics(void *);
extern void BagApp_CreateMessageData(void *);
extern void BagApp_CountPockets(void *);
extern void BagApp_LoadCursorPositions(void *);
extern void BagApp_LoadPocketItemNames(void *);
extern void BagApp_AllocItemNameStrings(void *);
extern void BagApp_ClampPocketScroll(s16 *, u16 *, u32);
extern void ov15_021FA070(s16 *, u16 *, u32, u32);
extern u32 BagApp_GetPageItemCount(void *);
extern void BagApp_PrintCursorDescription(void *);
extern s32 BagApp_State_HandleInput(void *);
extern s32 BagApp_State_OpenItemMenu(void *);
extern s32 BagApp_State_ChangePage(void *, s32);
extern void ov15_021FA620(void *);
extern s32 BagApp_State_PocketSwitch(void *);
extern s32 BagApp_State_MoveItemInput(void *);
extern s32 BagApp_State_DropMovedItem(void *);
extern s32 BagApp_State_EndMoveItem(void *);
extern s32 BagApp_State_ItemMenuInput(void *);
extern s32 BagApp_State_CloseItemMenu(void *);
extern s32 BagApp_State_RunItemAction(void *);
extern s32 BagApp_State_WaitItemMessage(void *);
extern s32 BagApp_State_RunUseItemTask(void *);
extern s32 BagApp_State_TrashQuantityInput(void *);
extern s32 BagApp_State_ShowTrashConfirm(void *);
extern s32 BagApp_State_CancelTrash(void *);
extern s32 BagApp_State_WaitTrashConfirmMessage(void *);
extern s32 BagApp_State_TrashYesNo(void *);
extern s32 BagApp_State_WaitTrashResultMessage(void *);
extern s32 BagApp_State_FinishTrash(void *);
extern s32 BagApp_State_WaitRegisterFailMessage(void *);
extern s32 BagApp_State_HoldItemSelectInput(void *);
extern s32 BagApp_State_WaitCantHoldMessage(void *);
extern s32 BagApp_State_SellSelectInput(void *);
extern s32 BagApp_State_StartSell(void *);
extern s32 BagApp_State_SetupSellQuantity(void *);
extern s32 BagApp_State_SellQuantityInput(void *);
extern s32 BagApp_State_ShowSellPrice(void *);
extern s32 BagApp_State_CancelSellQuantity(void *);
extern s32 BagApp_State_WaitSellPriceMessage(void *);
extern s32 BagApp_State_SellYesNo(void *);
extern s32 BagApp_State_CompleteSale(void *);
extern s32 BagApp_State_WaitSaleMessage(void *);
extern s32 ov15_021FD3AC(void *);
extern void BagApp_LoadPocketSubBgPalette(void *, u32, u32);
extern void BagApp_LoadSubScreenLayout(void *, u32, u32, u32);
extern s32 BagApp_State_ButtonFeedback(void *);
extern void BagApp_Init3D(void *);
extern void BagApp_Draw3D(void *);
extern void BagApp_AddWindows(void *);
extern void ov15_021FE4C8(void *);
extern void ov15_021FE528(void *);
extern void ov15_021FE874(void *);
extern void BagApp_LoadItemMenuStrings(void *);
extern void ov15_021FF1E0(void *);
extern void BagApp_PrintCancelButton(void *, u32);
extern void BagApp_DrawItemList(void *, s32, s32, u32);
extern void BagApp_PrintPageNumber(void *, u32, s32, u32);
extern void BagApp_InitSprites(void *);
extern void BagApp_TickSprites(void *);
extern void BagApp_SetCursorPosition(void *, u32);
extern void BagApp_LoadPocketObjPalette(void *, u32);
extern void BagApp_DrawItemIcons(void *, void *, u32, u32);

#define ARGS (*(u8 **)(work + 0x234))
#define SEL  (ARGS[0x64])



BOOL Bag_Init(OverlayManager *man, int *state) {
    u8 *work;

#pragma unused(state)

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    Heap_Create(HEAP_ID_3, HEAP_ID_6, 0x42000);
    work = OverlayManager_CreateAndGetData(man, 0x94C, HEAP_ID_6);
    memset(work, 0, 0x94C);
    *(void **)(work + 0x234) = OverlayManager_GetArgs(man);
    BagApp_GetSaveStructPtrs(work);
    *(void **)work = BgConfig_Alloc(HEAP_ID_6);
    work[0x615] = PlayerProfile_GetTrainerGender(*(void **)(work + 0x23C));
    BeginNormalPaletteFade(2, 3, 3, 0, 6, 1, HEAP_ID_6);
    SetKeyRepeatTimers(3, 8);
    BagApp_LoadCursorPositions(work);
    BagApp_CreateMessageData(work);
    BagApp_AllocItemNameStrings(work);
    BagApp_CountPockets(work);
    ov15_021FA620(work);
    BagApp_SetGraphicsBanks();
    BagApp_InitBgLayers(*(void **)work);
    BagApp_LoadBgGraphics(work);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    BagApp_AddWindows(work);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    ov15_021FE4C8(work);
    ov15_021FE528(work);
    BagApp_LoadItemMenuStrings(work);
    ov15_021FE874(work);
    BagApp_LoadPocketItemNames(work);
    BagApp_PrintCancelButton(work, 0);
    BagApp_ClampPocketScroll((s16 *)(ARGS + 10 + 12 * SEL), (u16 *)(ARGS + 8 + 12 * SEL), *(ARGS + 12 * SEL + 13));
    ov15_021FA070((s16 *)(ARGS + 10 + 12 * SEL), (u16 *)(ARGS + 8 + 12 * SEL), *(ARGS + 12 * SEL + 13), HEAP_ID_6);
    BagApp_InitSprites(work);
    BagApp_LoadSubScreenLayout(work, 0, BagApp_GetPageItemCount(work), 0);
    BagApp_DrawItemList(work, *(s16 *)(ARGS + 12 * SEL + 10), -1, 0);
    BagApp_LoadPocketObjPalette(work, ARGS[0x64]);
    BagApp_LoadPocketSubBgPalette(work, 1, ARGS[0x64]);
    BagApp_PrintPageNumber(work, *(ARGS + 12 * SEL + 13), *(s16 *)(ARGS + 12 * SEL + 10), 0);
    BagApp_DrawItemIcons(work, ARGS + 4 + 12 * SEL, BagApp_GetPageItemCount(work), 1);
    *(u32 *)(work + 0x644) = *(u16 *)(ARGS + 12 * SEL + 8) + 8;
    BagApp_SetCursorPosition(work, *(u32 *)(work + 0x644));
    BagApp_PrintCursorDescription(work);
    if (ARGS[0x65] == 4 || ARGS[0x65] == 5) {
        ov15_021FF1E0(work);
    }
    BagApp_Init3D(work);
    Main_SetVBlankIntrCB(BagApp_VBlankCallback, work);
    Sound_SetSceneAndPlayBGM(0x33, 0, 0);
    sub_0203A964();
    reg_GX_POWCNT |= 0x8000;
    ToggleBgLayer(4, 1);
    return TRUE;
}

BOOL Bag_Main(OverlayManager *man, int *state) {
    u8 *work;

    work = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            switch (ARGS[0x65]) {
            case 2:
                *state = 16;
                break;
            case 1:
                *state = 14;
                break;
            case 3:
                *state = 26;
                break;
            default:
                *state = 1;
                break;
            }
        }
        break;
    case 1:
        *state = BagApp_State_HandleInput(work);
        break;
    case 2:
        if (BagApp_State_PocketSwitch(work) == TRUE) {
            if (ARGS[0x65] == 2) {
                *state = 16;
            } else if (ARGS[0x65] == 1) {
                *state = 14;
            } else if (ARGS[0x65] == 3) {
                *state = 26;
            } else {
                *state = 1;
            }
        }
        break;
    case 3:
        *state = BagApp_State_MoveItemInput(work);
        break;
    case 4:
        *state = BagApp_State_ItemMenuInput(work);
        break;
    case 5:
        *state = BagApp_State_TrashQuantityInput(work);
        break;
    case 6:
        *state = BagApp_State_ShowTrashConfirm(work);
        break;
    case 7:
        *state = BagApp_State_CancelTrash(work);
        break;
    case 8:
        *state = BagApp_State_WaitTrashConfirmMessage(work);
        break;
    case 9:
        *state = BagApp_State_TrashYesNo(work);
        break;
    case 10:
        *state = BagApp_State_WaitTrashResultMessage(work);
        break;
    case 11:
        *state = BagApp_State_FinishTrash(work);
        break;
    case 12:
        *state = BagApp_State_WaitItemMessage(work);
        break;
    case 13:
        *state = BagApp_State_RunUseItemTask(work);
        break;
    case 14:
        *state = BagApp_State_HoldItemSelectInput(work);
        break;
    case 15:
        *state = BagApp_State_WaitCantHoldMessage(work);
        break;
    case 16:
        *state = BagApp_State_SellSelectInput(work);
        break;
    case 17:
        *state = BagApp_State_SetupSellQuantity(work);
        break;
    case 18:
        *state = BagApp_State_SellQuantityInput(work);
        break;
    case 19:
        *state = BagApp_State_ShowSellPrice(work);
        break;
    case 20:
        *state = BagApp_State_CancelSellQuantity(work);
        break;
    case 21:
        *state = BagApp_State_WaitSellPriceMessage(work);
        break;
    case 22:
        *state = BagApp_State_SellYesNo(work);
        break;
    case 23:
        *state = BagApp_State_CompleteSale(work);
        break;
    case 24:
        *state = BagApp_State_WaitSaleMessage(work);
        break;
    case 26:
        *state = ov15_021FD3AC(work);
        break;
    case 25:
        *state = BagApp_State_WaitRegisterFailMessage(work);
        break;
    case 27:
        *state = BagApp_State_OpenItemMenu(work);
        break;
    case 28:
        *state = BagApp_State_CloseItemMenu(work);
        break;
    case 29:
        *state = BagApp_State_RunItemAction(work);
        break;
    case 30:
        *state = BagApp_State_ChangePage(work, 1);
        break;
    case 31:
        *state = BagApp_State_ChangePage(work, -1);
        break;
    case 32:
        *state = BagApp_State_EndMoveItem(work);
        break;
    case 33:
        *state = BagApp_State_DropMovedItem(work);
        break;
    case 34:
        *state = BagApp_State_StartSell(work);
        break;
    case 35:
        *state = BagApp_State_ButtonFeedback(work);
        break;
    case 36:
        sub_020880CC(1, HEAP_ID_6);
        *state = 37;
        break;
    case 37:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    BagApp_TickSprites(work);
    SpriteSystem_DrawSprites(*(void **)(work + 0x24C));
    BagApp_Draw3D(work);
    return FALSE;
}
