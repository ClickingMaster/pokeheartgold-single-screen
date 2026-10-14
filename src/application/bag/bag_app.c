#include "application/bag/bag_app.h"

#include <stddef.h>
#include <string.h>

#include "global.h"

#include "constants/items.h"
#include "constants/sndseq.h"

#include "bag.h"
#include "bag_cursor.h"
#include "bag_view.h"
#include "bg_window.h"
#include "camera.h"
#include "field_use_item.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "move.h"
#include "msgdata.h"
#include "overlay_15.h"
#include "render_text.h"
#include "render_window.h"
#include "roamer.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite_system.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "message_printer_number.h"
#include "swap_3d_buffers.h"
#include "wireless_status_icon.h"
#include "ui_util.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// Bag application, part 1: overlay entry points, input states, item actions (use, trash, register, give, sell),
// sub-screen layouts, 3D bag model and camera, windows.

// Data still in asm (asm/bag_app_data.s). The navigation tables are {up, down, left, right} per button;
// the *Down/*Left/*Right labels point at the later columns.
// Sub-screen tilemap edit: 1 copies a rect of the loaded tilemap, 2 clears one
typedef struct BagAppTilemapCommand {
    u8 type;
    u8 srcX;
    u8 srcY;
    u8 destX;
    u8 destY;
    u8 width;
    u8 height;
    u8 padding;
} BagAppTilemapCommand;

typedef struct BagAppTileRect {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} BagAppTileRect;

// Camera key per pocket (0: closed bag, 1-8: pockets), one set per trainer gender
typedef struct BagAppCameraKey {
    u16 angleX;
    u16 angleY;
    u8 unk_4[4];
    fx32 distance;
    fx32 modelY;
} BagAppCameraKey;

typedef struct BagAppWindowTemplate {
    u32 x;
    u32 y;
    u32 baseTile;
} BagAppWindowTemplate;

typedef struct BagAppWindowPos {
    u32 x;
    u32 y;
} BagAppWindowPos;

typedef struct BagAppItemMenuChoices {
    u8 actions[5]; // index into sItemActionFuncs, 0xFF for none
} BagAppItemMenuChoices;

extern const BagAppItemMenuChoices sDefaultItemMenuChoices;
extern const GraphicsModes sBagGraphicsModes;
extern const GraphicsBanks sBagGraphicsBanks;
extern const BgTemplate sBgTemplate_Main1;
extern const BgTemplate sBgTemplate_Main2;
extern const BgTemplate sBgTemplate_Main3;
extern const BgTemplate sBgTemplate_Sub0;
extern const BgTemplate sBgTemplate_Sub1;
extern const BgTemplate sBgTemplate_Sub2;
extern const BgTemplate sBgTemplate_Sub3;
extern const u8 sPocketCapacities[];
extern const u8 sButtonNavTable[];
extern const u8 sButtonNavTableDown[];
extern const u8 sButtonNavTableLeft[];
extern const u8 sButtonNavTableRight[];
extern const u8 sMoveNavTable[];
extern const u8 sMoveNavTableDown[];
extern const u8 sMoveNavTableLeft[];
extern const u8 sMoveNavTableRight[];
extern const u8 sItemMenuNavTable[];
extern const u8 sItemMenuNavTableDown[];
extern const u8 sItemMenuNavTableLeft[];
extern const u8 sItemMenuNavTableRight[];
extern u8 sItemMenuCursorButtons[]; // not const: BagApp_HandleItemMenuInput stores the cursor before reading it
extern const TouchscreenHitbox *const sTouchHitboxTables[];
extern int (*const sItemActionFuncs[])(BagApp *app);
extern const BagAppTilemapCommand sSubScreenTilemapCommands[][4];
extern const BagAppTileRect sUnusedSlotRects[][2];
extern const BagAppTileRect sItemSlotRects[];
extern const VecFx32 sCameraTarget;
extern const CameraParam sCameraSetup;
extern const VecFx32 sBagModelPosition;
extern const VecFx32 sBagModelScale;
extern const MtxFx33 sBagModelRotation;
extern const GXRgb sBagEdgeColorTable[8];
extern const BagAppCameraKey sCameraTweenKeys[2][9];
extern const BagAppWindowTemplate sItemListWindowTemplates[];
extern const BagAppWindowPos sItemMenuWindowPositions[];
extern const BagAppWindowPos sQuantityDigitWindowPositions[];
extern s32 sDebugSubScreenLayoutCount;
extern s32 sDebugSubScreenLayout;

void BagApp_GetSaveStructPtrs(BagApp *app);
RoamerSaveData *BagApp_GetSaveRoamers(BagApp *app);
void BagApp_SetRepelStepCount(BagApp *app, u8 steps);
void BagApp_SetFlute(BagApp *app, u8 flute);
void BagApp_VBlankCallback(void *arg);
void BagApp_SetGraphicsBanks(void);
void BagApp_InitBgLayers(BgConfig *bgConfig);
void BagApp_FreeBgLayers(BgConfig *bgConfig);
void BagApp_LoadBgGraphics(BagApp *app);
void BagApp_CreateMessageData(BagApp *app);
void BagApp_CountPockets(BagApp *app);
u16 BagApp_GetCurrentPocketSlotValue(BagApp *app, int index, BOOL quantity);
void BagApp_ReadItemName(MsgData *msgData, String *dest, u16 itemId, enum HeapID heapID);
void BagApp_ReadTMHMMoveName(MsgData *msgData, String *dest, u16 itemId, enum HeapID heapID);
void BagApp_LoadCursorPositions(BagApp *app);
void BagApp_SaveCursorPositions(BagApp *app);
void BagApp_LoadPocketItemNames(BagApp *app);
void BagApp_AllocItemNameStrings(BagApp *app);
void BagApp_FreeItemNameStrings(BagApp *app);
void BagApp_ClampPocketScroll(s16 *scroll, u16 *position, s32 itemCount);
void BagApp_PocketScrollDummy(s16 *scroll, u16 *position, u32 itemCount, enum HeapID heapID);
s32 BagApp_GetPageItemCount(BagApp *app);
int BagApp_GetMovingItemPageSlot(BagApp *app);
void ov15_021FA0D8(BagApp *app);
void BagApp_SetPocketPositionFromButton(BagApp *app, s32 button);
BOOL BagApp_ShouldMoveCursorToButton(BagApp *app, s32 button);
s32 BagApp_GetCursorItemId(BagApp *app);
void BagApp_PrintCursorDescription(BagApp *app);
int BagApp_State_HandleInput(BagApp *app);
int BagApp_State_OpenItemMenu(BagApp *app);
int BagApp_State_ChangePage(BagApp *app, int direction);
void BagApp_CalcPocketIconSpacing(BagApp *app);
int BagApp_DebugCycleSubScreenLayout(BagApp *app);
int BagApp_FindPocketIndex(BagApp *app, int pocketId);
int BagApp_GetAdjacentPocket(BagApp *app, int pocket, int direction);
void BagApp_RedrawPocketPage(BagApp *app, BagViewPocket *pocket);
int BagApp_PressButton(BagApp *app, int button, u8 *itemChosen, int unused3, int unused4, BOOL force);
int BagApp_State_PocketSwitch(BagApp *app);
BOOL BagApp_HandlePocketSwitchDpad(BagApp *app);
BOOL BagApp_RunPocketSwitch(BagApp *app);
int BagApp_GetTouchedButton(BagApp *app, u32 table);
int BagApp_GetTouchedPocket(BagApp *app);
void BagApp_StartMoveItem(BagApp *app);
int BagApp_GetMoveCursorFromDpad(int slot);
void BagApp_RedrawMovePage(BagApp *app, BagViewPocket *pocket);
int BagApp_GetMoveTargetResult(BagApp *app, int slot);
int BagApp_State_MoveItemInput(BagApp *app);
int BagApp_State_DropMovedItem(BagApp *app);
int BagApp_State_EndMoveItem(BagApp *app);
void BagApp_UpdateMoveMarker(BagApp *app);
void BagApp_SetupItemMenu(BagApp *app);
void BagApp_DrawItemMenu(BagApp *app, const u8 *actions);
int BagApp_HandleItemMenuInput(BagApp *app);
void BagApp_RedrawPocket(BagApp *app);
int BagApp_State_ItemMenuInput(BagApp *app);
int BagApp_State_CloseItemMenu(BagApp *app);
int BagApp_State_RunItemAction(BagApp *app);
int BagApp_ItemAction_Use(BagApp *app);
int BagApp_State_WaitItemMessage(BagApp *app);
int BagApp_UseSelectedItem(BagApp *app);
int BagApp_State_RunUseItemTask(BagApp *app);
int BagApp_UseTMHMTask(BagApp *app);
BOOL BagApp_TryUseItemInPlace(BagApp *app, u16 itemId);
String *BagApp_TryUseRepel(BagApp *app, u16 itemId);
String *BagApp_ToggleGBSounds(BagApp *app, u16 itemId);
void BagApp_TakeUsedItem(BagApp *app);
int BagApp_UseItemInPlaceTask(BagApp *app);
int ov15_021FBC6C(BagApp *app);
int BagApp_ItemAction_Confirm(BagApp *app);
int BagApp_ItemAction_Trash(BagApp *app);
s32 BagApp_StepQuantity(s32 quantity, s32 max, s32 step);
int BagApp_State_TrashQuantityInput(BagApp *app);
int BagApp_State_ShowTrashConfirm(BagApp *app);
int BagApp_State_CancelTrash(BagApp *app);
int BagApp_State_WaitTrashConfirmMessage(BagApp *app);
int BagApp_State_TrashYesNo(BagApp *app);
int BagApp_State_WaitTrashResultMessage(BagApp *app);
int BagApp_State_FinishTrash(BagApp *app);
int BagApp_ItemAction_Register(BagApp *app);
int BagApp_State_WaitRegisterFailMessage(BagApp *app);
int BagApp_ItemAction_Deselect(BagApp *app);
int BagApp_ItemAction_Move(BagApp *app);
int BagApp_ItemAction_Give(BagApp *app);
int BagApp_State_HoldItemSelectInput(BagApp *app);
int BagApp_State_WaitCantHoldMessage(BagApp *app);
int BagApp_State_SellSelectInput(BagApp *app);
int BagApp_State_StartSell(BagApp *app);
int BagApp_State_SetupSellQuantity(BagApp *app);
int BagApp_State_SellQuantityInput(BagApp *app);
int BagApp_State_ShowSellPrice(BagApp *app);
int BagApp_State_CancelSellQuantity(BagApp *app);
int BagApp_State_WaitSellPriceMessage(BagApp *app);
int BagApp_State_SellYesNo(BagApp *app);
int BagApp_State_CompleteSale(BagApp *app);
int BagApp_State_WaitSaleMessage(BagApp *app);
int BagApp_State_DebugSubScreenLayout(BagApp *app);
int BagApp_ItemAction_ChooseForBerryPatch(BagApp *app);
void BagApp_LoadPocketSubBgPalette(BagApp *app, int slot, int pocket);
void BagApp_ApplyTilemapCommands(BgConfig *bgConfig, int bgId, int layout);
void BagApp_DrawItemSlotFrames(BgConfig *bgConfig, int bgId, u32 count, u32 selection);
void BagApp_LoadSubScreenLayout(BagApp *app, u32 layout, int count, int selection);
void BagApp_SetMenuInputState(BagApp *app, MenuInputState state);
void BagApp_ShowCancelButton(BagApp *app, BOOL show);
int BagApp_StartButtonFlash(BagApp *app, u8 sprite, u8 flashPalette, u8 restorePalette, int nextState);
int BagApp_StartButtonAnim(BagApp *app, u8 sprite, u32 anim, int nextState);
int BagApp_State_ButtonFeedback(BagApp *app);
void BagApp_Init3D(BagApp *app);
void BagApp_InitCameraTween(BagAppScene *scene);
void BagApp_StartCameraTween(BagAppScene *scene, u32 to, u16 duration);
void BagApp_UpdateCameraTween(BagAppScene *scene, int trainerGender);
void BagApp_Free3D(BagApp *app);
void BagApp_Draw3D(BagApp *app);
void BagApp_AdvanceAnimLoop(NNSG3dAnmObj *anim);
void BagApp_LoadBagModel(BagApp *app);
void BagApp_FreeBagModel(BagApp *app);
void BagApp_SetBagModelPocketAnim(BagApp *app);
void BagApp_AddWindows(BagApp *app);
void BagApp_RemoveWindows(BagApp *app);
void BagApp_LoadUnusedStrings(BagApp *app);
void BagApp_FreeUnusedStrings(BagApp *app);
void ov15_021FE528(BagApp *app);
void BagApp_ClearMoveInfoWindow(BagApp *app);
void BagApp_LoadQuantityTemplates(BagApp *app);
void BagApp_FreeQuantityTemplates(BagApp *app);
void BagApp_MeasureQuantityText(BagApp *app, u16 number, u32 unused2, u32 unused3);
void BagApp_DrawHMMarker(BagApp *app, Window *window, u32 y);

BOOL Bag_Init(OverlayManager *man, int *state) {
    BagApp *app;

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
    app = OverlayManager_CreateAndGetData(man, sizeof(BagApp), HEAP_ID_6);
    memset(app, 0, sizeof(BagApp));
    app->bagView = OverlayManager_GetArgs(man);
    BagApp_GetSaveStructPtrs(app);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_6);
    app->trainerGender = PlayerProfile_GetTrainerGender(app->profile);
    BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, RGB_BLACK, 6, 1, HEAP_ID_6);
    SetKeyRepeatTimers(3, 8);
    BagApp_LoadCursorPositions(app);
    BagApp_CreateMessageData(app);
    BagApp_AllocItemNameStrings(app);
    BagApp_CountPockets(app);
    BagApp_CalcPocketIconSpacing(app);
    BagApp_SetGraphicsBanks();
    BagApp_InitBgLayers(app->bgConfig);
    BagApp_LoadBgGraphics(app);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    BagApp_AddWindows(app);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    BagApp_LoadUnusedStrings(app);
    ov15_021FE528(app);
    BagApp_LoadItemMenuStrings(app);
    BagApp_LoadQuantityTemplates(app);
    BagApp_LoadPocketItemNames(app);
    BagApp_PrintCancelButton(app, FALSE);
    BagApp_ClampPocketScroll(&app->bagView->pockets[app->bagView->pocketIndex].scroll, &app->bagView->pockets[app->bagView->pocketIndex].position, app->bagView->pockets[app->bagView->pocketIndex].itemCount);
    BagApp_PocketScrollDummy(&app->bagView->pockets[app->bagView->pocketIndex].scroll, &app->bagView->pockets[app->bagView->pocketIndex].position, app->bagView->pockets[app->bagView->pocketIndex].itemCount, HEAP_ID_6);
    BagApp_InitSprites(app);
    BagApp_LoadSubScreenLayout(app, 0, BagApp_GetPageItemCount(app), 0);
    BagApp_DrawItemList(app, app->bagView->pockets[app->bagView->pocketIndex].scroll, -1, 0);
    BagApp_LoadPocketObjPalette(app, app->bagView->pocketIndex);
    BagApp_LoadPocketSubBgPalette(app, 1, app->bagView->pocketIndex);
    BagApp_PrintPageNumber(app, app->bagView->pockets[app->bagView->pocketIndex].itemCount, app->bagView->pockets[app->bagView->pocketIndex].scroll, 0);
    BagApp_DrawItemIcons(app, &app->bagView->pockets[app->bagView->pocketIndex], BagApp_GetPageItemCount(app), 1);
    app->cursorButton = app->bagView->pockets[app->bagView->pocketIndex].position + 8;
    BagApp_SetCursorPosition(app, app->cursorButton);
    BagApp_PrintCursorDescription(app);
    if (app->bagView->mode == 4 || app->bagView->mode == 5) {
        ov15_021FF1E0(app);
    }
    BagApp_Init3D(app);
    Main_SetVBlankIntrCB(BagApp_VBlankCallback, app);
    Sound_SetSceneAndPlayBGM(0x33, 0, 0);
    WirelessStatusIcon_CreateIfConnected();
    reg_GX_POWCNT |= 0x8000;
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_ON);
    return TRUE;
}

BOOL Bag_Main(OverlayManager *man, int *state) {
    BagApp *app = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            switch (app->bagView->mode) {
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
        *state = BagApp_State_HandleInput(app);
        break;
    case 2:
        if (BagApp_State_PocketSwitch(app) == TRUE) {
            if (app->bagView->mode == 2) {
                *state = 16;
            } else if (app->bagView->mode == 1) {
                *state = 14;
            } else if (app->bagView->mode == 3) {
                *state = 26;
            } else {
                *state = 1;
            }
        }
        break;
    case 3:
        *state = BagApp_State_MoveItemInput(app);
        break;
    case 4:
        *state = BagApp_State_ItemMenuInput(app);
        break;
    case 5:
        *state = BagApp_State_TrashQuantityInput(app);
        break;
    case 6:
        *state = BagApp_State_ShowTrashConfirm(app);
        break;
    case 7:
        *state = BagApp_State_CancelTrash(app);
        break;
    case 8:
        *state = BagApp_State_WaitTrashConfirmMessage(app);
        break;
    case 9:
        *state = BagApp_State_TrashYesNo(app);
        break;
    case 10:
        *state = BagApp_State_WaitTrashResultMessage(app);
        break;
    case 11:
        *state = BagApp_State_FinishTrash(app);
        break;
    case 12:
        *state = BagApp_State_WaitItemMessage(app);
        break;
    case 13:
        *state = BagApp_State_RunUseItemTask(app);
        break;
    case 14:
        *state = BagApp_State_HoldItemSelectInput(app);
        break;
    case 15:
        *state = BagApp_State_WaitCantHoldMessage(app);
        break;
    case 16:
        *state = BagApp_State_SellSelectInput(app);
        break;
    case 17:
        *state = BagApp_State_SetupSellQuantity(app);
        break;
    case 18:
        *state = BagApp_State_SellQuantityInput(app);
        break;
    case 19:
        *state = BagApp_State_ShowSellPrice(app);
        break;
    case 20:
        *state = BagApp_State_CancelSellQuantity(app);
        break;
    case 21:
        *state = BagApp_State_WaitSellPriceMessage(app);
        break;
    case 22:
        *state = BagApp_State_SellYesNo(app);
        break;
    case 23:
        *state = BagApp_State_CompleteSale(app);
        break;
    case 24:
        *state = BagApp_State_WaitSaleMessage(app);
        break;
    case 26:
        *state = BagApp_State_DebugSubScreenLayout(app);
        break;
    case 25:
        *state = BagApp_State_WaitRegisterFailMessage(app);
        break;
    case 27:
        *state = BagApp_State_OpenItemMenu(app);
        break;
    case 28:
        *state = BagApp_State_CloseItemMenu(app);
        break;
    case 29:
        *state = BagApp_State_RunItemAction(app);
        break;
    case 30:
        *state = BagApp_State_ChangePage(app, 1);
        break;
    case 31:
        *state = BagApp_State_ChangePage(app, -1);
        break;
    case 32:
        *state = BagApp_State_EndMoveItem(app);
        break;
    case 33:
        *state = BagApp_State_DropMovedItem(app);
        break;
    case 34:
        *state = BagApp_State_StartSell(app);
        break;
    case 35:
        *state = BagApp_State_ButtonFeedback(app);
        break;
    case 36:
        BeginAppScreenFade(1, HEAP_ID_6);
        *state = 37;
        break;
    case 37:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    BagApp_TickSprites(app);
    SpriteSystem_DrawSprites(app->spriteManager);
    BagApp_Draw3D(app);
    return FALSE;
}

BOOL Bag_Exit(OverlayManager *man, int *state) {
    BagApp *app = OverlayManager_GetData(man);

    BagApp_Free3D(app);
    BagApp_FreeSprites(app);
    Heap_Free(app->bgPaletteBuffers[0]);
    Heap_Free(app->bgPaletteBuffers[1]);
    ov15_021FA0D8(app);
    BagApp_SaveCursorPositions(app);
    BagApp_RemoveWindows(app);
    BagApp_FreeBgLayers(app->bgConfig);
    GF_TouchpadStopSampling();
    GF_DestroyVramTransferManager();
    BagApp_FreeItemMenuStrings(app);
    BagApp_FreeUnusedStrings(app);
    BagApp_FreeQuantityTemplates(app);
    BagApp_FreeItemNameStrings(app);
    String_Delete(app->msgBuf);
    DestroyMsgData(app->moveNamesMsgData);
    DestroyMsgData(app->itemNamesMsgData);
    DestroyMsgData(app->msgData);
    MessagePrinter_Delete(app->msgPrinter);
    MessageFormat_Delete(app->msgFormat);
    NARC_Delete(app->narc);
    OverlayManager_FreeData(man);
    Sound_ClearSecondaryScene();
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_6);
    return TRUE;
}

void BagApp_GetSaveStructPtrs(BagApp *app) {
    app->bag = Save_Bag_Get(app->bagView->saveData);
    app->profile = Save_PlayerData_GetProfile(app->bagView->saveData);
    app->options = Save_PlayerData_GetOptionsAddr(app->bagView->saveData);
}

RoamerSaveData *BagApp_GetSaveRoamers(BagApp *app) {
    return Save_Roamers_Get(app->bagView->saveData);
}

void BagApp_SetRepelStepCount(BagApp *app, u8 steps) {
    *RoamerSave_GetRepelAddr(BagApp_GetSaveRoamers(app)) = steps;
}

void BagApp_SetFlute(BagApp *app, u8 flute) {
    RoamerSave_SetFlute(BagApp_GetSaveRoamers(app), flute);
}

void BagApp_VBlankCallback(void *arg) {
    BagApp *app = arg;
    vu32 *dtcm;

    DoScheduledBgGpuUpdates(app->bgConfig);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    dtcm = (vu32 *)0x027E0000;
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

void BagApp_SetGraphicsBanks(void) {
    GraphicsBanks banks = sBagGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void BagApp_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes = sBagGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &sBgTemplate_Main1, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &sBgTemplate_Main2, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &sBgTemplate_Main3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_6);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sBgTemplate_Sub0, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sBgTemplate_Sub1, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sBgTemplate_Sub2, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sBgTemplate_Sub3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_6);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 0x20, 0, HEAP_ID_6);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG3, 0, 0);
}

void BagApp_FreeBgLayers(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    Heap_FreeExplicit(HEAP_ID_6, bgConfig);
}

void BagApp_LoadBgGraphics(BagApp *app) {
    app->narc = NARC_New(NARC_a_0_1_5, HEAP_ID_6);
    GfGfxLoader_LoadCharData(NARC_a_0_1_5, 7, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_6);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_5, 0x36, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_6);
    if (app->trainerGender == 0) {
        GfGfxLoader_LoadScrnData(NARC_a_0_1_5, 0x5E, app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_6);
    } else {
        GfGfxLoader_LoadScrnData(NARC_a_0_1_5, 0x5D, app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_6);
    }
    GfGfxLoader_GXLoadPal(NARC_a_0_1_5, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_6);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_5, 0x11, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, 0x20, HEAP_ID_6);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, HEAP_ID_6);
    LoadUserFrameGfx1(app->bgConfig, GF_BG_LYR_MAIN_1, 0x3F7, 0xE, 0, HEAP_ID_6);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_1, 0x3D9, 0xC, Options_GetFrame(app->options), HEAP_ID_6);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_5, 0x26, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_6);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_11_OFFSET, HEAP_ID_6);
    GfGfxLoader_LoadCharData(NARC_a_0_1_5, 0x2E, app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_6);
    app->bgPaletteBuffers[0] = GfGfxLoader_GetPlttData(NARC_a_0_1_5, 0x28, &app->bgPalettes[0], HEAP_ID_6);
    app->bgPaletteBuffers[1] = GfGfxLoader_GetPlttData(NARC_a_0_1_5, 0x29, &app->bgPalettes[1], HEAP_ID_6);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_5, 8, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 0x80, HEAP_ID_6);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_SUB_0, 0x3E2, 0xC, Options_GetFrame(app->options), HEAP_ID_6);
}

void BagApp_LoadInfoPanelTilemap(BagApp *app, int mode) {
    if (mode == 1) {
        GfGfxLoader_LoadScrnData(NARC_a_0_1_5, 0x36, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_6);
    } else {
        GfGfxLoader_LoadScrnData(NARC_a_0_1_5, 9, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_6);
    }
}

void BagApp_CreateMessageData(BagApp *app) {
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 10, HEAP_ID_6);
    app->msgPrinter = MessagePrinter_New(1, 2, 0, HEAP_ID_6);
    app->msgFormat = MessageFormat_New(HEAP_ID_6);
    app->itemNamesMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 222, HEAP_ID_6);
    app->moveNamesMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 750, HEAP_ID_6);
    app->msgBuf = String_New(0x100, HEAP_ID_6);
}

void BagApp_CountPockets(BagApp *app) {
    u8 i;

    app->numPockets = 0;
    for (i = 0; i < 8; i++) {
        if (app->bagView->pockets[i].slots != NULL) {
            app->numPockets++;
        }
    }
}

#pragma force_active on

u16 BagApp_GetCurrentPocketSlotValue(BagApp *app, int index, BOOL quantity) {
    BagView *bagView = app->bagView;
    BagViewPocket *pocket = bagView->pockets;
    pocket += bagView->pocketIndex;
    if (quantity == FALSE) {
        return pocket->slots[index].id;
    }
    return pocket->slots[index].quantity;
}

#pragma force_active reset

void BagApp_ReadItemName(MsgData *msgData, String *dest, u16 itemId, enum HeapID heapID) {
    ReadMsgDataIntoString(msgData, itemId, dest);
}

void BagApp_ReadTMHMMoveName(MsgData *msgData, String *dest, u16 itemId, enum HeapID heapID) {
    ReadMsgDataIntoString(msgData, TMHMGetMove(itemId), dest);
}

void BagApp_LoadCursorPositions(BagApp *app) {
    BagViewPocket *pockets;
    u16 i;

    app->bagView->pocketIndex = 0;
    pockets = app->bagView->pockets;
    if (app->bagView->cursor == NULL) {
        for (i = 0; i < 8; i++) {
            BagViewPocket *pocket = &pockets[i];
            if (pocket->slots != NULL) {
                pocket->position = 0;
                pocket->scroll = 0;
            }
        }
        for (i = 0; i < 8; i++) {
            if (pockets[i].slots != NULL) {
                app->bagView->pocketIndex = i;
                return;
            }
        }
        return;
    }
    for (i = 0; i < 8; i++) {
        BagViewPocket *pocket = &pockets[i];
        if (pocket->slots != NULL) {
            u8 pos[2];
            BagCursor_Field_PocketGetPosition(app->bagView->cursor, pocket->pocketId, &pos[1], &pos[0]);
            pocket->position = pos[1];
            pocket->scroll = pos[0];
        }
    }
    {
        u32 pocketId = BagCursor_Field_GetPocket(app->bagView->cursor);
        if (pockets[pocketId].slots == NULL) {
            u16 j;
            for (j = 0; j < 8; j++) {
                if (pockets[j].slots != NULL) {
                    pocketId = j;
                    break;
                }
            }
        }
        for (i = 0; i < 8; i++) {
            BagViewPocket *pocket = &pockets[i];
            if (pocket->slots != NULL && pocketId == pocket->pocketId) {
                app->bagView->pocketIndex = i;
                return;
            }
        }
    }
}

void BagApp_SaveCursorPositions(BagApp *app) {
    BagView *bagView = app->bagView;
    BagViewPocket *pockets;
    u32 i;

    if (bagView->cursor == NULL) {
        return;
    }
    pockets = bagView->pockets;
    for (i = 0; i < 8; i++) {
        BagViewPocket *pocket = &pockets[i];
        if (pocket->slots != NULL) {
            BagCursor_Field_PocketSetPosition(app->bagView->cursor, pocket->pocketId, (u8)pocket->position, (u8)pocket->scroll);
        }
    }
    bagView = app->bagView;
    BagCursor_Field_SetPocket(bagView->cursor, pockets[bagView->pocketIndex].pocketId);
}

#pragma force_active on

void BagApp_LoadPocketItemNames(BagApp *app) {
    BagViewPocket *pocket = &app->bagView->pockets[app->bagView->pocketIndex];
    u32 i;
    s32 lastPage;

    if (pocket->pocketId == POCKET_TMHMS) {
        for (i = 0; i < sPocketCapacities[pocket->pocketId]; i++) {
            if (pocket->slots[i].id == ITEM_NONE || pocket->slots[i].quantity == 0) {
                break;
            }
            BagApp_ReadTMHMMoveName(app->moveNamesMsgData, app->itemNames[i], pocket->slots[i].id, HEAP_ID_6);
            app->itemIds[i] = pocket->slots[i].id;
        }
        pocket->itemCount = i;
    } else {
        for (i = 0; i < sPocketCapacities[pocket->pocketId]; i++) {
            if (pocket->slots[i].id == ITEM_NONE || pocket->slots[i].quantity == 0) {
                break;
            }
            BagApp_ReadItemName(app->itemNamesMsgData, app->itemNames[i], pocket->slots[i].id, HEAP_ID_6);
            app->itemIds[i] = pocket->slots[i].id;
        }
        pocket->itemCount = i;
    }
    if (pocket->itemCount == 0) {
        lastPage = 0;
    } else {
        lastPage = ((pocket->itemCount - 1) / 6) * 6;
    }
    if (pocket->scroll > lastPage) {
        pocket->scroll = lastPage;
    }
}

#pragma force_active reset

void BagApp_AllocItemNameStrings(BagApp *app) {
    u32 i;

    for (i = 0; i < NELEMS(app->itemNames); i++) {
        app->itemNames[i] = String_New(0x12, HEAP_ID_6);
    }
}

void BagApp_FreeItemNameStrings(BagApp *app) {
    u32 i;

    for (i = 0; i < NELEMS(app->itemNames); i++) {
        String_Delete(app->itemNames[i]);
    }
}

void BagApp_ClampPocketScroll(s16 *scroll, u16 *position, s32 itemCount) {
    if (*scroll + *position > itemCount) {
        *scroll = ((itemCount - 1) / 6) * 6;
    }
    if (*scroll < 0) {
        *scroll = 0;
    }
}

void BagApp_PocketScrollDummy(s16 *scroll, u16 *position, u32 itemCount, enum HeapID heapID) {
}

s32 BagApp_GetPageItemCount(BagApp *app) {
    BagView *bagView = app->bagView;
    u8 pocketIndex = bagView->pocketIndex;
    s32 count = bagView->pockets[pocketIndex].itemCount - bagView->pockets[pocketIndex].scroll;
    if (count > 6) {
        count = 6;
    }
    return count;
}

#pragma force_active on

int BagApp_GetMovingItemPageSlot(BagApp *app) {
    int source = app->moveSource;
    BagView *bagView = app->bagView;
    int page = source / 6;
    if (bagView->pockets[bagView->pocketIndex].scroll == page * 6) {
        return source % 6;
    }
    return -1;
}

#pragma force_active reset

void ov15_021FA0D8(BagApp *app) {
    app->unk_348 = 0;
}

void BagApp_SetPocketPositionFromButton(BagApp *app, s32 button) {
    BagView *bagView;
    u8 pocketIndex;

    if (button >= 8 && button < 14) {
        bagView = app->bagView;
        pocketIndex = bagView->pocketIndex;
        bagView->pockets[pocketIndex].position = button - 8;
    }
}

BOOL BagApp_ShouldMoveCursorToButton(BagApp *app, s32 button) {
    if (button == 14 || button == 15) {
        return FALSE;
    }
    if (button <= 7 && app->cursorButton >= 8 && app->cursorButton <= 13) {
        return FALSE;
    }
    return TRUE;
}

s32 BagApp_GetCursorItemId(BagApp *app) {
    BagView *bagView = app->bagView;
    BagViewPocket *pocket = &bagView->pockets[bagView->pocketIndex];
    u32 index = pocket->scroll + app->cursorButton - 8;
    if (sPocketCapacities[bagView->pocketIndex] <= index) {
        return ITEM_NONE;
    }
    return pocket->slots[index].id;
}

void BagApp_PrintCursorDescription(BagApp *app) {
    s32 button = app->cursorButton;
    if (button >= 8 && button <= 13) {
        u32 itemId = BagApp_GetCursorItemId(app);
        if (itemId != ITEM_NONE) {
            BagApp_ShowItemDescription(app, &app->windows[0], itemId);
        } else {
            BagApp_ClearDescriptionWindow(app, &app->windows[0]);
        }
    } else if (button >= 0 && button <= 7) {
        BagApp_ShowPocketDescription(app, &app->windows[0], button);
    } else {
        BagApp_ClearDescriptionWindow(app, &app->windows[0]);
    }
}

#pragma force_active on

int BagApp_State_HandleInput(BagApp *app) {
    int changed = 0, result, target;
    u8 pressed[5];

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        app->cursorButton = sButtonNavTable[app->cursorButton * 4];
        changed++;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        app->cursorButton = sButtonNavTableDown[app->cursorButton * 4];
        changed++;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        target = sButtonNavTableLeft[app->cursorButton * 4];
        if (target == 14) {
            return BagApp_PressButton(app, 14, &pressed[4], 1, 2, 0);
        }
        if (app->cursorButton != 16) {
            if (app->cursorButton >= 0 && app->cursorButton < 8) {
                result = BagApp_GetAdjacentPocket(app, app->cursorButton, -1);
                if (app->cursorButton != result) {
                    app->cursorButton = result;
                    changed++;
                }
            } else {
                app->cursorButton = target;
                changed++;
            }
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        target = sButtonNavTableRight[app->cursorButton * 4];
        if (target == 15) {
            return BagApp_PressButton(app, 15, &pressed[3], 1, 2, 0);
        }
        if (app->cursorButton != 16) {
            if (app->cursorButton >= 0 && app->cursorButton < 8) {
                result = BagApp_GetAdjacentPocket(app, app->cursorButton, 1);
                if (app->cursorButton != result) {
                    app->cursorButton = result;
                    changed++;
                }
            } else {
                app->cursorButton = target;
                changed++;
            }
        }
    } else if (gSystem.newKeys & PAD_BUTTON_L) {
        if (app->cursorButton >= 0 && app->cursorButton < 8) {
            app->cursorButton = BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, -1);
            BagApp_SetCursorPosition(app, app->cursorButton);
            return BagApp_PressButton(app, app->cursorButton, &pressed[2], 1, 2, changed);
        } else {
            return BagApp_PressButton(app, BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, -1), &pressed[2], 1, 2, 0);
        }
    } else if (gSystem.newKeys & PAD_BUTTON_R) {
        if (app->cursorButton >= 0 && app->cursorButton < 8) {
            app->cursorButton = BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, 1);
            BagApp_SetCursorPosition(app, app->cursorButton);
            return BagApp_PressButton(app, app->cursorButton, &pressed[1], 1, 2, changed);
        } else {
            return BagApp_PressButton(app, BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, 1), &pressed[1], 1, 2, 0);
        }
    }
    if (app->cursorButton == 17) {
        app->cursorButton = app->bagView->pocketIndex;
    }
    if (changed) {
        PlaySE(SEQ_SE_DP_SELECT);
        BagApp_SetCursorPosition(app, app->cursorButton);
        BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        BagApp_PrintCursorDescription(app);
    }
    pressed[0] = FALSE;
    // called without the hitbox table index (r1 is whatever the previous call left; the main table, 0, in practice)
    result = ((int (*)(BagApp *))BagApp_GetTouchedButton)(app);
    if (result != -1) {
        if (BagApp_ShouldMoveCursorToButton(app, result)) {
            if ((u32)result < 8) {
                if (BagApp_FindPocketIndex(app, result) != -1) {
                    app->cursorButton = result;
                    BagApp_SetCursorPosition(app, app->cursorButton);
                }
            } else {
                app->cursorButton = result;
                BagApp_SetCursorPosition(app, app->cursorButton);
                if (app->cursorButton >= 8 && app->cursorButton <= 13) {
                    BagApp_SetPocketPositionFromButton(app, app->cursorButton);
                }
            }
        }
        result = BagApp_PressButton(app, result, &pressed[0], 1, 2, 1);
        if (result != 1) {
            return result;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        result = BagApp_PressButton(app, app->cursorButton, &pressed[0], 1, 2, 0);
        if (app->cursorButton >= 8 && app->cursorButton <= 13) {
            BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        }
        if (result != 1) {
            return result;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        result = BagApp_PressButton(app, 16, &pressed[0], 1, 2, 0);
        BagApp_SetMenuInputState(app, MENU_INPUT_STATE_BUTTONS);
        if (app->cursorButton >= 8 && app->cursorButton <= 13) {
            BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        }
        if (result != 1) {
            return result;
        }
    }
    if (pressed[0] == TRUE) {
        return BagApp_StartButtonAnim(app, 20, 41, 27);
    }
    return 1;
}

int BagApp_State_OpenItemMenu(BagApp *app) {
    BagViewPocket *pockets = app->bagView->pockets;
    u32 offset = app->bagView->pocketIndex * sizeof(BagViewPocket);
    BagViewPocket *pocket = (BagViewPocket *)((u8 *)pockets + offset);

    BagApp_LoadSubScreenLayout(app, 2, 0, 0);
    BagApp_DrawSelectedItemEntry(app, pocket->scroll, app->cursorButton - 8);
    BagApp_ShowSelectedItemIcon(app, app->cursorButton - 8);
    BagApp_SetupItemMenu(app);
    // the slot is re-read through the pocket array base and the byte offset
    app->bagView->itemId = ((BagViewPocket *)((u8 *)pockets + offset))->slots[pocket->scroll + app->cursorButton - 8].id;
    app->maxQuantity = ((BagViewPocket *)((u8 *)pockets + offset))->slots[pocket->scroll + app->cursorButton - 8].quantity;
    return 4;
}

#pragma force_active reset

int BagApp_State_ChangePage(BagApp *app, int direction) {
    BagView *bagView = app->bagView;
    BagViewPocket *pocket = &bagView->pockets[bagView->pocketIndex];

    if (direction > 0) {
        s32 scroll = pocket->scroll + 6;
        if (scroll < pocket->itemCount) {
            pocket->scroll = scroll;
        } else {
            pocket->scroll = 0;
        }
    } else {
        s32 scroll = pocket->scroll - 6;
        if (scroll >= 0) {
            pocket->scroll = scroll;
        } else {
            pocket->scroll = ((pocket->itemCount - 1) / 6) * 6;
        }
    }
    if (app->isMovingItem == TRUE) {
        BagApp_RedrawMovePage(app, pocket);
        BagApp_SetMoveCursorPosition(app, app->menuCursor);
        return 3;
    }
    BagApp_RedrawPocketPage(app, pocket);
    BagApp_PrintCursorDescription(app);
    switch (app->bagView->mode) {
    case 2:
        return 16;
    case 1:
        return 14;
    case 3:
        return 26;
    default:
        return 1;
    }
}

void BagApp_CalcPocketIconSpacing(BagApp *app) {
    u8 numPockets = app->numPockets;
    app->unk_617 = (90 - numPockets * 10) / (numPockets + 1) + 6;
    app->unk_618 = app->unk_617 + 4;
}

int BagApp_DebugCycleSubScreenLayout(BagApp *app) {
    if (gSystem.newKeys & PAD_BUTTON_DEBUG) {
        s32 layout = ++sDebugSubScreenLayout;
        if (layout >= sDebugSubScreenLayoutCount) {
            sDebugSubScreenLayout = 0;
        }
        BagApp_LoadSubScreenLayout(app, 1, sDebugSubScreenLayoutCount, sDebugSubScreenLayout);
    }
    return 0;
}

#pragma force_active on

int BagApp_FindPocketIndex(BagApp *app, int pocketId) {
    int i;
    for (i = 0; i < 8; i++) {
        if (pocketId == app->bagView->pockets[i].pocketId && app->bagView->pockets[i].slots != NULL) {
            return i;
        }
    }
    return -1;
}

#pragma force_active reset

int BagApp_GetAdjacentPocket(BagApp *app, int pocket, int direction) {
    s32 i;
    for (i = 0; i < 8; i++) {
        pocket += direction;
        if (pocket < 0) {
            pocket = 7;
        } else if (pocket == 8) {
            pocket = 0;
        }
        if (BagApp_FindPocketIndex(app, pocket) != -1) {
            break;
        }
    }
    return pocket;
}

void BagApp_RedrawPocketPage(BagApp *app, BagViewPocket *pocket) {
    s32 count = BagApp_GetPageItemCount(app);
    BagApp_LoadSubScreenLayout(app, 0, count, 0);
    BagApp_DrawItemList(app, pocket->scroll, -1, 0);
    BagApp_PrintPageNumber(app, pocket->itemCount, pocket->scroll, 0);
    count = BagApp_GetPageItemCount(app);
    BagApp_DrawItemIcons(app, pocket, count, 1);
}

#pragma force_active on

int BagApp_PressButton(BagApp *app, int button, u8 *itemChosen, int unused3, int unused4, BOOL force) {
    int result = 1;
    BagViewPocket *pocket;
    int pocketIndex, index;

    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        pocketIndex = BagApp_FindPocketIndex(app, button);
        if (pocketIndex == -1) {
            return result;
        }
        if (pocketIndex == app->bagView->pocketIndex && !force) {
            return result;
        }
        app->bagView->pocketIndex = pocketIndex;
        BagApp_LoadPocketItemNames(app);
        pocket = &app->bagView->pockets[app->bagView->pocketIndex];
        BagApp_LoadSubScreenLayout(app, 0, BagApp_GetPageItemCount(app), 0);
        BagApp_DrawItemList(app, pocket->scroll, -1, 0);
        BagApp_LoadPocketObjPalette(app, app->bagView->pocketIndex);
        BagApp_PrintPageNumber(app, pocket->itemCount, pocket->scroll, 0);
        BagApp_DrawItemIcons(app, pocket, BagApp_GetPageItemCount(app), 1);
        BagApp_LoadPocketSubBgPalette(app, 1, app->bagView->pocketIndex);
        PlaySE(SEQ_SE_DP_SELECT);
        BagApp_PrintCursorDescription(app);
        if (app->cursorButton >= 8) {
            BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        }
        BagApp_SetBagModelPocketAnim(app);
        BagApp_StartCameraTween(&app->scene, app->bagView->pocketIndex + 1, 7);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        pocket = &app->bagView->pockets[app->bagView->pocketIndex];
        index = pocket->scroll + (button - 8);
        if (index < pocket->itemCount) {
            app->bagView->itemId = pocket->slots[index].id;
            *itemChosen = TRUE;
            PlaySE(SEQ_SE_DP_SELECT);
        }
        BagApp_PrintCursorDescription(app);
        break;
    case 15:
        if (app->bagView->pockets[app->bagView->pocketIndex].itemCount > 6) {
            PlaySE(SEQ_SE_DP_SELECT);
            result = BagApp_StartButtonFlash(app, 18, 9, 8, 30);
        }
        break;
    case 14:
        if (app->bagView->pockets[app->bagView->pocketIndex].itemCount > 6) {
            PlaySE(SEQ_SE_DP_SELECT);
            result = BagApp_StartButtonFlash(app, 17, 9, 8, 31);
        }
        break;
    case 16:
        app->bagView->itemId = ITEM_NONE;
        app->bagView->selectedAction = 5;
        BagApp_SetMenuInputState(app, (MenuInputState)force);
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        result = BagApp_StartButtonFlash(app, 19, 9, 8, 36);
        break;
    }
    return result;
}

#pragma force_active reset

// Pocket switch: BagAppPocketSwitch.flags bits 0-3 and 4-6 are two small states, bit 7 a flag.
int BagApp_State_PocketSwitch(BagApp *app) {
    BagAppPocketSwitch *pocketSwitch = &app->pocketSwitch;
    int pocket;

    if (!System_GetTouchHeld()) {
        pocketSwitch->flags = (pocketSwitch->flags & ~0x70) | 0x10;
    }
    pocket = BagApp_GetTouchedPocket(app);
    if (pocket == -1 || pocket != pocketSwitch->pocket) {
        pocketSwitch->flags = (pocketSwitch->flags & ~0xF) | 1;
    }
    if (!BagApp_HandlePocketSwitchDpad(app) && (((u32)pocketSwitch->flags << 25) >> 29) == 1 && pocket != -1) {
        u8 current;
        app->bagView->pocketIndex = pocketSwitch->pocket;
        current = pocketSwitch->pocket;
        if (current > (u8)pocket) {
            pocketSwitch->direction = 0;
            pocketSwitch->stage = 0;
        } else if (current < (u8)pocket) {
            pocketSwitch->direction = 1;
            pocketSwitch->stage = 0;
        } else {
            pocketSwitch->direction = 2;
        }
        pocketSwitch->pocket = pocket;
        pocketSwitch->flags &= ~0xF;
        pocketSwitch->flags &= ~0x70;
        pocketSwitch->unk_4 = 0;
        app->switchPocket = pocket;
        ov15_021FF950(app);
        BagApp_StartCameraTween(&app->scene, app->switchPocket + 1, 7);
    }
    BagApp_RunPocketSwitch(app);
    ov15_021FF964(app);
    return 0;
}

BOOL BagApp_HandlePocketSwitchDpad(BagApp *app) {
    BagAppPocketSwitch *pocketSwitch = &app->pocketSwitch;

    if (gSystem.newKeys & PAD_KEY_LEFT) {
        if (app->numPockets == 1) {
            return TRUE;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        app->bagView->pocketIndex = pocketSwitch->pocket;
        if (pocketSwitch->pocket != 0) {
            pocketSwitch->pocket--;
            app->switchPocket--;
        } else {
            pocketSwitch->pocket = app->numPockets - 1;
            app->switchPocket = app->numPockets - 1;
        }
        pocketSwitch->stage = 0;
        pocketSwitch->direction = 0;
        pocketSwitch->flags = (pocketSwitch->flags & ~0x70) | 0x10;
        pocketSwitch->flags = (pocketSwitch->flags & ~0xF) | 1;
        pocketSwitch->flags &= ~0x80;
        if (pocketSwitch->unk_4 != 3) {
            pocketSwitch->unk_4 = 4;
        }
        ov15_021FF950(app);
        return TRUE;
    }
    if (gSystem.newKeys & PAD_KEY_RIGHT) {
        if (app->numPockets == 1) {
            return TRUE;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        app->bagView->pocketIndex = pocketSwitch->pocket;
        if (pocketSwitch->pocket + 1 < app->numPockets) {
            pocketSwitch->pocket++;
            app->switchPocket++;
        } else {
            pocketSwitch->pocket = 0;
            app->switchPocket = 0;
        }
        pocketSwitch->stage = 0;
        pocketSwitch->direction = 1;
        pocketSwitch->flags = (pocketSwitch->flags & ~0x70) | 0x10;
        pocketSwitch->flags = (pocketSwitch->flags & ~0xF) | 1;
        pocketSwitch->flags &= ~0x80;
        if (pocketSwitch->unk_4 != 3) {
            pocketSwitch->unk_4 = 4;
        }
        ov15_021FF950(app);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL BagApp_RunPocketSwitch(BagApp *app) {
    BagAppPocketSwitch *pocketSwitch = &app->pocketSwitch;

    switch (pocketSwitch->stage) {
    case 0:
        pocketSwitch->timer = 0;
        pocketSwitch->stage++;
        break;
    case 1:
        if (pocketSwitch->timer < 8) {
            pocketSwitch->timer++;
            break;
        }
        app->bagView->pocketIndex = pocketSwitch->pocket;
        BagApp_LoadPocketItemNames(app);
        BagApp_DrawItemList(app, app->bagView->pockets[app->bagView->pocketIndex].scroll, -1, 0);
        BagApp_LoadSubScreenLayout(app, 0, BagApp_GetPageItemCount(app), 0);
        BagApp_LoadPocketObjPalette(app, app->bagView->pocketIndex);
        BagApp_LoadPocketSubBgPalette(app, 1, app->bagView->pocketIndex);
        BagApp_ClampPocketScroll(&app->bagView->pockets[app->bagView->pocketIndex].scroll, &app->bagView->pockets[app->bagView->pocketIndex].position, app->bagView->pockets[app->bagView->pocketIndex].itemCount);
        BagApp_PocketScrollDummy(&app->bagView->pockets[app->bagView->pocketIndex].scroll, &app->bagView->pockets[app->bagView->pocketIndex].position, app->bagView->pockets[app->bagView->pocketIndex].itemCount, HEAP_ID_6);
        pocketSwitch->stage++;
        return TRUE;
    case 2:
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

int BagApp_GetTouchedButton(BagApp *app, u32 table) {
    return TouchscreenHitbox_FindRectAtTouchNew(sTouchHitboxTables[table]);
}

int BagApp_GetTouchedPocket(BagApp *app) {
    return -1;
}

#pragma force_active on

void BagApp_StartMoveItem(BagApp *app) {
    BagViewPocket *pocket = &app->bagView->pockets[app->bagView->pocketIndex];

    app->isMovingItem = TRUE;
    app->moveSource = pocket->scroll + app->cursorButton - 8;
    BagApp_PrintMoveItemPrompt(app);
    BagApp_UpdateMoveMarker(app);
    BagApp_HideItemMenuSprites(app);
    BagApp_ClearSelectedItemEntry(app);
    BagApp_ClearItemMenuLabels(&app->dynWindows[13]);
    BagApp_CloseItemActionWindows(app);
    pocket = &app->bagView->pockets[app->bagView->pocketIndex];
    BagApp_LoadSubScreenLayout(app, 1, BagApp_GetPageItemCount(app), app->cursorButton - 8);
    BagApp_DrawItemList(app, pocket->scroll, app->cursorButton - 8, 1);
    BagApp_PrintPageNumber(app, pocket->itemCount, pocket->scroll, 0);
    BagApp_DrawItemIcons(app, pocket, BagApp_GetPageItemCount(app), 0);
    BagApp_ShowMovingItemIcon(app, pocket, pocket->scroll + app->cursorButton - 8);
    BagApp_SetCursorPosition(app, app->cursorButton);
    app->menuCursor = app->cursorButton - 8;
}

#pragma force_active reset

int BagApp_GetMoveCursorFromDpad(int slot) {
    if (gSystem.newKeys & PAD_KEY_UP) {
        return sMoveNavTable[slot * 4] - 8;
    }
    if (gSystem.newKeys & PAD_KEY_DOWN) {
        return sMoveNavTableDown[slot * 4] - 8;
    }
    if (gSystem.newKeys & PAD_KEY_LEFT) {
        return sMoveNavTableLeft[slot * 4] - 8;
    }
    if (gSystem.newKeys & PAD_KEY_RIGHT) {
        return sMoveNavTableRight[slot * 4] - 8;
    }
    return slot;
}

#pragma force_active on

void BagApp_RedrawMovePage(BagApp *app, BagViewPocket *pocket) {
    BagApp_LoadSubScreenLayout(app, 1, BagApp_GetPageItemCount(app), BagApp_GetMovingItemPageSlot(app));
    BagApp_DrawItemList(app, pocket->scroll, BagApp_GetMovingItemPageSlot(app), 1);
    BagApp_ShowMovingItemIcon(app, pocket, app->moveSource);
    BagApp_PrintPageNumber(app, app->bagView->pockets[app->bagView->pocketIndex].itemCount, app->bagView->pockets[app->bagView->pocketIndex].scroll, 0);
}

#pragma force_active reset

int BagApp_GetMoveTargetResult(BagApp *app, int slot) {
    int result = -1;
    switch (slot) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (BagApp_GetPageItemCount(app) > slot) {
            result = app->menuCursor;
            PlaySE(SEQ_SE_DP_SELECT);
        } else {
            PlaySE(SEQ_SE_DP_BOX03);
        }
        break;
    case 6:
        result = 14;
        break;
    case 7:
        result = 15;
        break;
    case 8:
        result = -2;
        break;
    }
    return result;
}

#pragma force_active on

int BagApp_State_MoveItemInput(BagApp *app) {
    u32 result = -1, button;
    u16 next;
    BagViewPocket *pocket = &app->bagView->pockets[app->bagView->pocketIndex];

    next = BagApp_GetMoveCursorFromDpad(app->menuCursor);
    if (app->menuCursor != next) {
        if ((u16)(next + 0xFFFA) <= 1) { // next is 6 or 7 (page arrows)
            result = BagApp_GetMoveTargetResult(app, next);
        } else {
            app->menuCursor = next;
            BagApp_SetMoveCursorPosition(app, app->menuCursor);
            PlaySE(SEQ_SE_DP_SELECT);
        }
    }
    button = BagApp_GetTouchedButton(app, 2);
    if (button != -1) {
        if (button == 8) {
            result = BagApp_GetMoveTargetResult(app, button);
            BagApp_SetMoveCursorPosition(app, button);
        } else if (button - 6 <= 1) {
            result = BagApp_GetMoveTargetResult(app, button);
        } else {
            app->menuCursor = button;
            if (BagApp_GetPageItemCount(app) > button) {
                result = BagApp_GetMoveTargetResult(app, button);
            } else {
                PlaySE(SEQ_SE_DP_BOX03);
            }
            BagApp_SetMoveCursorPosition(app, app->menuCursor);
        }
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        result = BagApp_GetMoveTargetResult(app, app->menuCursor);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        result = -2;
    }
    switch (result) {
    case -1:
        break;
    case -2:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        pocket->scroll = (app->moveSource / 6) * 6;
        return BagApp_StartButtonFlash(app, 19, 9, 8, 32);
    case 14:
        if (pocket->itemCount > 6) {
            PlaySE(SEQ_SE_DP_SELECT);
            return BagApp_StartButtonFlash(app, 17, 9, 8, 31);
        }
        break;
    case 15:
        if (pocket->itemCount > 6) {
            PlaySE(SEQ_SE_DP_SELECT);
            return BagApp_StartButtonFlash(app, 18, 9, 8, 30);
        }
        break;
    default:
        PlaySE(SEQ_SE_DP_SELECT);
        if (app->moveSource == pocket->scroll + app->menuCursor) {
            return BagApp_StartButtonAnim(app, 20, 41, 33);
        }
        return BagApp_StartButtonAnim(app, 20, 42, 33);
    }
    return 3;
}

#pragma force_active reset

int BagApp_State_DropMovedItem(BagApp *app) {
    BagView *bagView = app->bagView;
    u8 pocketIndex = bagView->pocketIndex;
    BagViewPocket *pocket = &bagView->pockets[pocketIndex];
    s16 scroll = pocket->scroll;
    u16 target = scroll + app->menuCursor;

    if (app->moveSource != target) {
        MoveItemSlotInList(pocket->slots, app->moveSource, target, pocketIndex, HEAP_ID_6);
        BagApp_LoadPocketItemNames(app);
        app->cursorButton = app->menuCursor + 8;
        BagApp_SetPocketPositionFromButton(app, app->cursorButton);
    }
    return BagApp_State_EndMoveItem(app);
}

#pragma force_active on

int BagApp_State_EndMoveItem(BagApp *app) {
    BagViewPocket *pocket;

    app->unk_348 = 0;
    app->isMovingItem = FALSE;
    app->moveSource = 0;
    app->unk_674 = 0;
    BagApp_UpdateMoveMarker(app);
    BagApp_LoadPocketItemNames(app);
    pocket = &app->bagView->pockets[app->bagView->pocketIndex];
    BagApp_LoadSubScreenLayout(app, 0, BagApp_GetPageItemCount(app), 0);
    BagApp_DrawItemList(app, pocket->scroll, -1, 0);
    BagApp_LoadPocketObjPalette(app, app->bagView->pocketIndex);
    BagApp_LoadPocketSubBgPalette(app, 1, app->bagView->pocketIndex);
    BagApp_PrintPageNumber(app, pocket->itemCount, pocket->scroll, 0);
    BagApp_DrawItemIcons(app, pocket, BagApp_GetPageItemCount(app), 1);
    BagApp_CloseItemPromptWindow(app);
    BagApp_SetCursorPosition(app, app->cursorButton);
    return 1;
}

#pragma force_active reset

void BagApp_UpdateMoveMarker(BagApp *app) {
    BagView *bagView = app->bagView;
    BagViewPocket *pocket = &bagView->pockets[bagView->pocketIndex];
    if (app->isMovingItem != FALSE) {
        ManagedSprite_SetPositionXY(app->sprites[0], 177, (pocket->position - 1) * 16 + 16);
    }
}

#pragma force_active on

void BagApp_SetupItemMenu(BagApp *app) {
    BagAppItemMenuChoices choices = sDefaultItemMenuChoices;
    ItemData *itemData = LoadItemDataOrGfx(app->bagView->itemId, ITEMNARC_PARAM, HEAP_ID_6);
    u32 pocketId = app->bagView->pockets[app->bagView->pocketIndex].pocketId;
    int i;

    for (i = 0; i < 5; i++) {
        app->itemActions[i] = NULL;
    }
    if (app->bagView->mode == 0) {
        u16 mode = app->bagView->mapLoadType;
        if ((u16)(mode + 0xFFFE) <= 1) { // mode 2 or 3
            if (app->bagView->pockets[app->bagView->pocketIndex].pocketId == POCKET_MAIL) {
                choices.actions[0] = 2;
            }
        } else if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_FIELDUSEFUNC)) {
            if (app->bagView->itemId == ITEM_BICYCLE && app->bagView->isCycling == TRUE) {
                choices.actions[0] = 1;
            } else if (app->bagView->pockets[app->bagView->pocketIndex].pocketId == POCKET_MAIL) {
                choices.actions[0] = 2;
            } else if (app->bagView->itemId == ITEM_POFFIN_CASE) {
                choices.actions[0] = 4;
            } else if (app->bagView->pockets[app->bagView->pocketIndex].pocketId == POCKET_BERRIES && Leftover_CanPlantBerry(app->bagView->checkUseData) == TRUE) {
                choices.actions[0] = 3;
            } else if (app->bagView->itemId == ITEM_GB_SOUNDS && SoundSys_GetGBSoundsState() == TRUE) {
                choices.actions[0] = 15;
            } else {
                choices.actions[0] = 0;
            }
        }
        if (!GetItemAttr_PreloadedItemData(itemData, ITEMATTR_PREVENT_TOSS)) {
            if (ItemIdIsNotJohtoBall(app->bagView->itemId) == TRUE) {
                choices.actions[2] = 8;
            }
            if (pocketId != POCKET_TMHMS) {
                choices.actions[1] = 5;
            }
        }
        if (GetItemAttr_PreloadedItemData(itemData, ITEMATTR_SELECTABLE)) {
            if (app->bagView->itemId == Bag_GetRegisteredItem1(app->bag) || app->bagView->itemId == Bag_GetRegisteredItem2(app->bag)) {
                choices.actions[1] = 7;
            } else {
                choices.actions[1] = 6;
            }
        }
    } else if (app->bagView->mode == 6 && BagApp_IsBerryOrMulch(pocketId, app->bagView->itemId) == TRUE) {
        choices.actions[0] = 14;
    }
    if (app->bagView->mode != 6 && pocketId != POCKET_TMHMS && pocketId != POCKET_BERRIES) {
        choices.actions[3] = 12;
    }
    choices.actions[4] = 11;
    for (i = 0; i < 5; i++) {
        if (choices.actions[i] != 0xFF) {
            app->itemActions[i] = sItemActionFuncs[choices.actions[i]];
        }
    }
    BagApp_PrintItemSelectedMessage(app, choices.actions, 5);
    BagApp_DrawItemMenu(app, choices.actions);
    Heap_Free(itemData);
}

#pragma force_active reset

void BagApp_DrawItemMenu(BagApp *app, const u8 *actions) {
    app->menuCursor = 0;
    BagApp_SetCursorPosition(app, sItemMenuCursorButtons[app->menuCursor]);
    BagApp_ShowItemMenuSprites(app, actions);
    BagApp_PrintItemMenuLabel(&app->dynWindows[13], app->menuStrings, actions[0]);
    BagApp_PrintItemMenuLabel(&app->dynWindows[14], app->menuStrings, actions[1]);
    BagApp_PrintItemMenuLabel(&app->dynWindows[15], app->menuStrings, actions[2]);
    BagApp_PrintItemMenuLabel(&app->dynWindows[16], app->menuStrings, actions[3]);
}

#pragma force_active on

int BagApp_HandleItemMenuInput(BagApp *app) {
    u32 old = app->menuCursor, next = old;
    int selected;

    if (gSystem.newKeys & PAD_KEY_UP) {
        next = sItemMenuNavTable[old * 4];
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        next = sItemMenuNavTableDown[old * 4];
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        next = sItemMenuNavTableLeft[old * 4];
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        next = sItemMenuNavTableRight[old * 4];
    }
    if (next != old) {
        app->menuCursor = next;
        BagApp_SetCursorPosition(app, sItemMenuCursorButtons[next]);
        PlaySE(SEQ_SE_DP_SELECT);
        return -1;
    }
    selected = BagApp_GetTouchedButton(app, 1);
    if (selected != -1) {
        BagApp_SetMenuInputState(app, MENU_INPUT_STATE_TOUCH);
        app->menuCursor = selected;
        BagApp_SetCursorPosition(app, sItemMenuCursorButtons[selected]);
        if (selected == 4) {
            PlaySE(SEQ_SE_GS_GEARCANCEL);
            return -2;
        }
        if (app->itemActions[selected]) {
            PlaySE(SEQ_SE_DP_SELECT);
            return selected;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        if (app->itemActions[next]) {
            PlaySE(SEQ_SE_DP_SELECT);
            BagApp_SetMenuInputState(app, MENU_INPUT_STATE_BUTTONS);
            return next;
        }
        if (next == 4) {
            PlaySE(SEQ_SE_DP_SELECT);
            return -2;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return -2;
    }
    return -1;
}

#pragma force_active reset

void BagApp_RedrawPocket(BagApp *app) {
    BagView *bagView;
    BagViewPocket *pocket;

    BagApp_LoadPocketItemNames(app);
    bagView = app->bagView;
    pocket = &bagView->pockets[bagView->pocketIndex];
    BagApp_LoadSubScreenLayout(app, 0, BagApp_GetPageItemCount(app), 0);
    BagApp_DrawItemList(app, pocket->scroll, -1, 0);
    BagApp_LoadPocketObjPalette(app, app->bagView->pocketIndex);
    BagApp_LoadPocketSubBgPalette(app, 1, app->bagView->pocketIndex);
    BagApp_PrintPageNumber(app, pocket->itemCount, pocket->scroll, 0);
    BagApp_DrawItemIcons(app, pocket, BagApp_GetPageItemCount(app), 0);
    BagApp_SetCursorPosition(app, app->cursorButton);
    BagApp_LoadInfoPanelTilemap(app, 1);
}

#pragma force_active on

int BagApp_State_ItemMenuInput(BagApp *app) {
    int result = BagApp_HandleItemMenuInput(app);
    switch (result) {
    case -2:
        return BagApp_StartButtonFlash(app, 19, 9, 8, 28);
    default:
        app->chosenAction = result;
        return BagApp_StartButtonFlash(app, (u8)(result + 28), 9, 8, 29);
    case -1:
        return 4;
    }
}

#pragma force_active reset

int BagApp_State_CloseItemMenu(BagApp *app) {
    BagView *bagView;
    int count;

    BagApp_HideItemMenuSprites(app);
    BagApp_ClearSelectedItemEntry(app);
    BagApp_ClearItemMenuLabels(&app->dynWindows[13]);
    bagView = app->bagView;
    count = BagApp_GetPageItemCount(app);
    BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], count, 0);
    BagApp_ClearMoveInfoWindow(app);
    BagApp_CloseItemMenuWindows(app);
    BagApp_RedrawPocket(app);
    return 1;
}

int BagApp_State_RunItemAction(BagApp *app) {
    int (*action)(BagApp *);

    BagApp_HideItemMenuSprites(app);
    BagApp_ClearItemMenuLabels(&app->dynWindows[13]);
    action = app->itemActions[app->chosenAction];
    return action(app);
}

int BagApp_ItemAction_Use(BagApp *app) {
    void *useFunc;
    int result;

    BagApp_ShowCancelButton(app, FALSE);
    BagApp_HideCursor(app);
    useFunc = GetItemFieldUseFunc(2, GetItemAttr(app->bagView->itemId, ITEMATTR_FIELDUSEFUNC, HEAP_ID_6));
    if (useFunc != NULL && (result = ((int (*)(ItemCheckUseData *))useFunc)(app->bagView->checkUseData)) != 0) {
        GetItemUseErrorMessage(app->profile, app->msgBuf, app->bagView->itemId, (enum ItemUseError)result, HEAP_ID_6);
        app->printerId = BagApp_PrintMessage(app, FALSE);
        return 12;
    }
    return BagApp_UseSelectedItem(app);
}

int BagApp_State_WaitItemMessage(BagApp *app) {
    if (!TextPrinterCheckActive(app->printerId)) {
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || gSystem.touchNew) {
            if (app->bagView->mode != 3) {
                BagApp_CloseItemMenuWindows(app);
            }
            ClearFrameAndWindow2(&app->windows[3], TRUE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
            ScheduleWindowCopyToVram(&app->windows[0]);
            BagApp_ShowCancelButton(app, TRUE);
            BagApp_RedrawPocket(app);
            return app->bagView->mode == 3 ? 26 : 1;
        }
    }
    return 12;
}

int BagApp_UseSelectedItem(BagApp *app) {
    BagViewPocket *pocket = &app->bagView->pockets[app->bagView->pocketIndex];

    app->taskState = 0;
    if (pocket->pocketId == POCKET_TMHMS) {
        app->task = BagApp_UseTMHMTask;
        return 13;
    }
    if (TryFormatRegisteredKeyItemUseMessage(app->bagView->saveData, app->msgBuf, app->bagView->itemId, HEAP_ID_6) == TRUE) {
        app->printerId = BagApp_PrintMessage(app, FALSE);
        return 12;
    }
    if (BagApp_TryUseItemInPlace(app, app->bagView->itemId) == TRUE) {
        app->task = BagApp_UseItemInPlaceTask;
        return 13;
    }
    BeginAppScreenFade(1, HEAP_ID_6);
    app->bagView->selectedAction = 0;
    return 37;
}

int BagApp_State_RunUseItemTask(BagApp *app) {
    int (*task)(BagApp *) = app->task;
    return task(app);
}

#pragma force_active on

int BagApp_UseTMHMTask(BagApp *app) {
    switch (app->taskState) {
    case 0: {
        u16 move = TMHMGetMove(app->bagView->itemId);
        BufferMoveName(app->msgFormat, 0, move);
        if (MoveIsHM(move) == TRUE) {
            ReadMsgDataIntoString(app->msgData, 60, app->msgBuf);
        } else {
            ReadMsgDataIntoString(app->msgData, 59, app->msgBuf);
        }
        app->printerId = BagApp_PrintMessage(app, FALSE);
        app->taskState = 1;
    } break;
    case 1:
        if (!TextPrinterCheckActive(app->printerId)) {
            if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || gSystem.touchNew) {
                String *template = NewString_ReadMsgData(app->msgData, 61);
                FillWindowPixelBuffer(&app->windows[3], 15);
                StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
                String_Delete(template);
                app->printerId = BagApp_PrintMessage(app, FALSE);
                app->taskState = 2;
            }
        }
        break;
    case 2:
        if (!TextPrinterCheckActive(app->printerId)) {
            BagApp_CreateYesNoPrompt(app);
            app->taskState = 3;
        }
        break;
    case 3: {
        int answer = YesNoPrompt_HandleInput(app->yesNoPrompt);
        switch (answer) {
        case YESNORESPONSE_YES: {
            BagApp_DestroyYesNoPrompt(app);
            BeginAppScreenFade(1, HEAP_ID_6);
            app->bagView->selectedAction = 0;
            return 37;
        }
        case YESNORESPONSE_NO: {
            BagView *bagView;
            int count;
            BagApp_DestroyYesNoPrompt(app);
            BagApp_CloseItemMenuWindows(app);
            ClearFrameAndWindow2(&app->windows[3], TRUE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
            ScheduleWindowCopyToVram(&app->windows[0]);
            bagView = app->bagView;
            count = BagApp_GetPageItemCount(app);
            BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], count, 0);
            BagApp_ClearMoveInfoWindow(app);
            BagApp_HideItemMenuSprites(app);
            BagApp_RedrawPocket(app);
            BagApp_ShowCancelButton(app, TRUE);
            return 1;
        }
        default:
            break;
        }
    } break;
    }
    return 13;
}

#pragma force_active reset

BOOL BagApp_TryUseItemInPlace(BagApp *app, u16 itemId) {
    String *template;

    BufferPlayersName(app->msgFormat, 0, app->profile);
    BufferItemName(app->msgFormat, 1, itemId);
    if (itemId == ITEM_BLACK_FLUTE) {
        template = NewString_ReadMsgData(app->msgData, 65);
        BagApp_SetFlute(app, 1);
        app->quantity = 0;
    } else if (itemId == ITEM_WHITE_FLUTE) {
        template = NewString_ReadMsgData(app->msgData, 64);
        BagApp_SetFlute(app, 2);
        app->quantity = 0;
    } else if (itemId == ITEM_MAX_REPEL || itemId == ITEM_SUPER_REPEL || itemId == ITEM_REPEL) {
        template = BagApp_TryUseRepel(app, itemId);
    } else if (itemId == ITEM_GB_SOUNDS) {
        template = BagApp_ToggleGBSounds(app, itemId);
        app->quantity = 0;
    } else {
        return FALSE;
    }
    StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
    String_Delete(template);
    return TRUE;
}

String *BagApp_TryUseRepel(BagApp *app, u16 itemId) {
    if (RoamerSave_RepelNotInUse(BagApp_GetSaveRoamers(app)) == FALSE) {
        app->quantity = 0;
        return NewString_ReadMsgData(app->msgData, 63);
    }
    BagApp_SetRepelStepCount(app, GetItemAttr(itemId, ITEMATTR_HOLD_EFFECT_PARAM, HEAP_ID_6));
    app->quantity = 1;
    PlaySE(SEQ_SE_DP_CARD2);
    return NewString_ReadMsgData(app->msgData, 62);
}

String *BagApp_ToggleGBSounds(BagApp *app, u16 itemId) {
    if (SoundSys_GetGBSoundsState() == TRUE) {
        SoundSys_ToggleGBSounds();
        return NewString_ReadMsgData(app->msgData, 105);
    } else {
        SoundSys_ToggleGBSounds();
        return NewString_ReadMsgData(app->msgData, 104);
    }
}

#pragma force_active on

void BagApp_TakeUsedItem(BagApp *app) {
    Pocket_TakeItem(app->bagView->pockets[app->bagView->pocketIndex].slots, app->bagView->pockets[app->bagView->pocketIndex].itemCount, app->bagView->itemId, app->quantity, HEAP_ID_6);
    BagApp_LoadPocketItemNames(app);
    BagApp_ClampPocketScroll(&app->bagView->pockets[app->bagView->pocketIndex].scroll, &app->bagView->pockets[app->bagView->pocketIndex].position, app->bagView->pockets[app->bagView->pocketIndex].itemCount);
    BagApp_PocketScrollDummy(&app->bagView->pockets[app->bagView->pocketIndex].scroll, &app->bagView->pockets[app->bagView->pocketIndex].position, app->bagView->pockets[app->bagView->pocketIndex].itemCount, HEAP_ID_6);
}

#pragma force_active reset

int BagApp_UseItemInPlaceTask(BagApp *app) {
    BagView *bagView;

    switch (app->taskState) {
    case 0:
        app->printerId = BagApp_PrintMessage(app, FALSE);
        app->taskState = 1;
        break;
    case 1:
        if (!TextPrinterCheckActive(app->printerId) && ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || gSystem.touchNew)) {
            BagApp_CloseItemMenuWindows(app);
            ClearFrameAndWindow2(&app->windows[3], TRUE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
            BagApp_TakeUsedItem(app);
            BagApp_RedrawPocket(app);
            bagView = app->bagView;
            BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), 1);
            BagApp_PrintCursorDescription(app);
            ScheduleWindowCopyToVram(&app->windows[0]);
            BagApp_ShowCancelButton(app, TRUE);
            app->taskState = 0;
            return 1;
        }
        break;
    }
    return 13;
}

int ov15_021FBC6C(BagApp *app) {
    BagApp_CloseItemMenuWindows(app);
    BeginAppScreenFade(1, HEAP_ID_6);
    app->bagView->selectedAction = 1;
    return 37;
}

int BagApp_ItemAction_Confirm(BagApp *app) {
    BagApp_CloseItemMenuWindows(app);
    BeginAppScreenFade(1, HEAP_ID_6);
    app->bagView->selectedAction = 3;
    return 37;
}

int BagApp_ItemAction_Trash(BagApp *app) {
    BagView *bagView;

    app->quantity = 1;
    BagApp_HideCursor(app);
    bagView = app->bagView;
    // called with a heap ID the function does not take
    if (((u16(*)(ItemSlot *, u32, u16, enum HeapID))Pocket_GetQuantity)(bagView->pockets[bagView->pocketIndex].slots, bagView->pockets[bagView->pocketIndex].itemCount, bagView->itemId, HEAP_ID_6) == 1) {
        BagApp_PrintTrashConfirmMessage(app);
        BagApp_ShowCancelButton(app, FALSE);
        return 8;
    }
    BagApp_LoadSubScreenLayout(app, 3, 0, 0);
    BagApp_ShowQuantityButtons(app, 3, app->maxQuantity);
    BagApp_PrintQuantityDigits(app, 3);
    BagApp_PrintTrashLabel(app);
    BagApp_PrintCancelButton(app, TRUE);
    return 5;
}

s32 BagApp_StepQuantity(s32 quantity, s32 max, s32 step) {
    if (step > 0) {
        if (quantity == max) {
            return 1;
        }
        quantity += step;
        if (quantity > max) {
            quantity = max;
        }
    } else {
        if (quantity == 1) {
            return max;
        }
        quantity += step;
        if (quantity <= 0) {
            quantity = 1;
        }
    }
    return quantity;
}

#pragma force_active on

int BagApp_State_TrashQuantityInput(BagApp *app) {
    int result = 0;
    int button = BagApp_GetTouchedButton(app, 3);

    if (button != -1) {
        switch (BagApp_GetUnusedDigitCount(app->maxQuantity)) {
        case 1:
            if (button == 0 || button == 3) {
                button = -1;
            }
            break;
        case 2:
            if (button == 0 || button == 1 || button == 3 || button == 4) {
                button = -1;
            }
            break;
        }
        switch (button) {
        case 0:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, 100);
            ManagedSprite_SetAnimationFrame(app->sprites[32], 0);
            ManagedSprite_SetAnim(app->sprites[32], 26);
            result = 1;
            break;
        case 1:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, 10);
            ManagedSprite_SetAnimationFrame(app->sprites[33], 0);
            ManagedSprite_SetAnim(app->sprites[33], 26);
            result = 1;
            break;
        case 2:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, 1);
            ManagedSprite_SetAnimationFrame(app->sprites[34], 0);
            ManagedSprite_SetAnim(app->sprites[34], 26);
            result = 1;
            break;
        case 3:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, -100);
            ManagedSprite_SetAnimationFrame(app->sprites[35], 0);
            ManagedSprite_SetAnim(app->sprites[35], 28);
            result = 2;
            break;
        case 4:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, -10);
            ManagedSprite_SetAnimationFrame(app->sprites[36], 0);
            ManagedSprite_SetAnim(app->sprites[36], 28);
            result = 2;
            break;
        case 5:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, -1);
            ManagedSprite_SetAnimationFrame(app->sprites[37], 0);
            ManagedSprite_SetAnim(app->sprites[37], 28);
            result = 2;
            break;
        case 6:
            result = 3;
            break;
        case 7:
            result = 4;
            break;
        }
    } else {
        result = HandleNumberSelectInput(&app->quantity, app->maxQuantity);
        if (!result) {
            if (gSystem.newKeys & PAD_BUTTON_A) {
                result = 3;
            } else if (gSystem.newKeys & PAD_BUTTON_B) {
                result = 4;
            }
        }
    }
    switch (result) {
    case 0:
        break;
    case 1:
    case 2:
        BagApp_PrintQuantityDigits(app, 3);
        PlaySE(SEQ_SE_DP_BAG_004);
        return 5;
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        return BagApp_StartButtonFlash(app, 38, 9, 8, 6);
    case 4:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return BagApp_StartButtonFlash(app, 19, 9, 8, 7);
    }
    return 5;
}

#pragma force_active reset

int BagApp_State_ShowTrashConfirm(BagApp *app) {
    BagApp_PrintTrashConfirmMessage(app);
    BagApp_HideQuantityButtons(app);
    BagApp_HideCursor(app);
    BagApp_ClearQuantityLabel(app);
    BagApp_ShowCancelButton(app, FALSE);
    return 8;
}

int BagApp_State_CancelTrash(BagApp *app) {
    BagApp_CloseItemMenuWindows(app);
    BagApp_RedrawPocket(app);
    BagApp_HideQuantityButtons(app);
    BagApp_PrintCancelButton(app, FALSE);
    ManagedSprite_SetAnimationFrame(app->sprites[19], 0);
    ManagedSprite_SetAnim(app->sprites[19], 16);
    return 1;
}

int BagApp_State_WaitTrashConfirmMessage(BagApp *app) {
    if (!TextPrinterCheckActive(app->printerId)) {
        BagApp_CreateYesNoPrompt(app);
        return 9;
    }
    return 8;
}

int BagApp_State_TrashYesNo(BagApp *app) {
    BagView *bagView;
    String *template;
    u32 input = YesNoPrompt_HandleInput(app->yesNoPrompt);

    switch (input) {
    case YESNORESPONSE_YES:
        BagApp_DestroyYesNoPrompt(app);
        template = NewString_ReadMsgData(app->msgData, 54);
        if (app->quantity == 1) {
            BufferItemName(app->msgFormat, 0, app->bagView->itemId);
        } else {
            BufferItemNamePlural(app->msgFormat, 0, app->bagView->itemId);
        }
        BufferIntegerAsString(app->msgFormat, 1, app->quantity, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
        String_Delete(template);
        FillWindowPixelBuffer(&app->windows[3], 15);
        app->printerId = BagApp_PrintMessage(app, FALSE);
        return 10;
    case YESNORESPONSE_NO:
        BagApp_DestroyYesNoPrompt(app);
        ClearFrameAndWindow2(&app->windows[3], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
        ScheduleWindowCopyToVram(&app->windows[0]);
        bagView = app->bagView;
        BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), 0);
        BagApp_ClearMoveInfoWindow(app);
        BagApp_CloseItemMenuWindows(app);
        BagApp_ShowCancelButton(app, TRUE);
        BagApp_RedrawPocket(app);
        return 1;
    case (u32)-1:
        break;
    }
    return 9;
}

int BagApp_State_WaitTrashResultMessage(BagApp *app) {
    if (TextPrinterCheckActive(app->printerId)) {
        return 10;
    }
    ScheduleWindowCopyToVram(&app->windows[3]);
    return 11;
}

int BagApp_State_FinishTrash(BagApp *app) {
    BagView *bagView;

    if (!TextPrinterCheckActive(app->printerId) && ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || gSystem.touchNew)) {
        ClearFrameAndWindow2(&app->windows[3], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
        ScheduleWindowCopyToVram(&app->windows[0]);
        bagView = app->bagView;
        Pocket_TakeItem(bagView->pockets[bagView->pocketIndex].slots, bagView->pockets[bagView->pocketIndex].itemCount, bagView->itemId, app->quantity, HEAP_ID_6);
        BagApp_LoadPocketItemNames(app);
        bagView = app->bagView;
        BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), TRUE);
        BagApp_ClearMoveInfoWindow(app);
        BagApp_CloseItemMenuWindows(app);
        BagApp_RedrawPocket(app);
        BagApp_PrintCursorDescription(app);
        BagApp_ShowCancelButton(app, TRUE);
        return 1;
    }
    return 11;
}

int BagApp_ItemAction_Register(BagApp *app) {
    BagView *bagView = app->bagView;
    String *template;

    if (Bag_TryRegisterItem(app->bag, bagView->itemId) == REG_ITEM_FAIL) {
        template = NewString_ReadMsgData(app->msgData, 103);
        FillWindowPixelBuffer(&app->windows[3], 15);
        StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
        String_Delete(template);
        BagApp_PrintMessage(app, FALSE);
        BagApp_HideItemMenuSprites(app);
        BagApp_HideCursor(app);
        BagApp_ShowCancelButton(app, FALSE);
        return 25;
    }
    BagApp_HideItemMenuSprites(app);
    BagApp_ClearSelectedItemEntry(app);
    BagApp_ClearItemMenuLabels(&app->dynWindows[13]);
    bagView = app->bagView;
    BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), FALSE);
    BagApp_ClearMoveInfoWindow(app);
    BagApp_CloseItemMenuWindows(app);
    BagApp_RedrawPocket(app);
    return 1;
}

int BagApp_State_WaitRegisterFailMessage(BagApp *app) {
    BagView *bagView;

    if (!TextPrinterCheckActive(app->printerId) && ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || gSystem.touchNew)) {
        ClearFrameAndWindow2(&app->windows[3], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
        BagApp_HideItemMenuSprites(app);
        BagApp_ClearSelectedItemEntry(app);
        BagApp_ClearItemMenuLabels(&app->dynWindows[13]);
        bagView = app->bagView;
        BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), FALSE);
        BagApp_ClearMoveInfoWindow(app);
        BagApp_CloseItemMenuWindows(app);
        BagApp_RedrawPocket(app);
        BagApp_ShowCancelButton(app, TRUE);
        return 1;
    }
    return 25;
}

int BagApp_ItemAction_Deselect(BagApp *app) {
    BagView *bagView;

    Bag_UnregisterItem(app->bag, app->bagView->itemId);
    BagApp_HideItemMenuSprites(app);
    BagApp_ClearSelectedItemEntry(app);
    BagApp_ClearItemMenuLabels(&app->dynWindows[13]);
    bagView = app->bagView;
    BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), FALSE);
    BagApp_ClearMoveInfoWindow(app);
    BagApp_CloseItemMenuWindows(app);
    BagApp_RedrawPocket(app);
    return 1;
}

int BagApp_ItemAction_Move(BagApp *app) {
    BagApp_StartMoveItem(app);
    return 3;
}

int BagApp_ItemAction_Give(BagApp *app) {
    BagApp_CloseItemMenuWindows(app);
    BagApp_ShowCancelButton(app, FALSE);
    BagApp_HideCursor(app);
    BeginAppScreenFade(1, HEAP_ID_6);
    app->bagView->selectedAction = 2;
    return 37;
}

#pragma force_active on

int BagApp_State_HoldItemSelectInput(BagApp *app) {
    int changed = 0;
    u8 pressedLeft, pressedRight, pressedL, pressedR, itemChosen;
    int result;

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        ++changed;
        app->cursorButton = sButtonNavTable[app->cursorButton * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        ++changed;
        app->cursorButton = sButtonNavTableDown[app->cursorButton * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        int current = app->cursorButton, next = sButtonNavTableLeft[current * 4];
        if (next == 14) {
            result = BagApp_PressButton(app, 14, &pressedLeft, 0, 2, 0);
            if (result == 1) {
                return 14;
            } else {
                return result;
            }
        } else if (current != 16) {
            if (current >= 0 && current < 8) {
                int pocket = BagApp_GetAdjacentPocket(app, current, -1);
                if (app->cursorButton != pocket) {
                    app->cursorButton = pocket;
                    ++changed;
                }
            } else {
                ++changed;
                app->cursorButton = next;
            }
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        int current = app->cursorButton, next = sButtonNavTableRight[current * 4];
        if (next == 15) {
            result = BagApp_PressButton(app, 15, &pressedRight, 0, 2, 0);
            if (result == 1) {
                return 14;
            } else {
                return result;
            }
        } else if (current != 16) {
            if (current >= 0 && current < 8) {
                int pocket = BagApp_GetAdjacentPocket(app, current, 1);
                if (app->cursorButton != pocket) {
                    app->cursorButton = pocket;
                    ++changed;
                }
            } else {
                ++changed;
                app->cursorButton = next;
            }
        }
    } else if (gSystem.newKeys & PAD_BUTTON_L) {
        int pocket = BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, -1);
        BagApp_PressButton(app, pocket, &pressedL, 1, 2, 0);
        return 14;
    } else if (gSystem.newKeys & PAD_BUTTON_R) {
        int pocket = BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, 1);
        BagApp_PressButton(app, pocket, &pressedR, 1, 2, 0);
        return 14;
    }
    if (app->cursorButton == 17) {
        app->cursorButton = app->bagView->pocketIndex;
    }
    if (changed) {
        PlaySE(SEQ_SE_DP_SELECT);
        BagApp_SetCursorPosition(app, app->cursorButton);
        BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        BagApp_PrintCursorDescription(app);
    }
    itemChosen = FALSE;
    // called without the hitbox table index, see BagApp_State_HandleInput
    result = ((int (*)(BagApp *))BagApp_GetTouchedButton)(app);
    if (result != -1) {
        if (BagApp_ShouldMoveCursorToButton(app, result)) {
            if ((u32)result < 8) {
                if (BagApp_FindPocketIndex(app, result) != -1) {
                    app->cursorButton = result;
                    BagApp_SetCursorPosition(app, app->cursorButton);
                }
            } else {
                app->cursorButton = result;
                BagApp_SetCursorPosition(app, app->cursorButton);
                if (app->cursorButton >= 8 && app->cursorButton <= 13) {
                    BagApp_SetPocketPositionFromButton(app, app->cursorButton);
                }
            }
        }
        result = BagApp_PressButton(app, result, &itemChosen, 0, 2, 1);
        if (result != 1) {
            app->bagView->selectedAction = 4;
            return result;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        result = BagApp_PressButton(app, app->cursorButton, &itemChosen, 0, 2, 0);
        if (app->cursorButton >= 8 && app->cursorButton <= 13) {
            BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        }
        if (result != 1) {
            app->bagView->selectedAction = 4;
            return result;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        result = BagApp_PressButton(app, 16, &itemChosen, 0, 2, 0);
        app->bagView->selectedAction = 4;
        if (app->cursorButton >= 8 && app->cursorButton <= 13) {
            BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        }
        return result;
    }
    if (itemChosen == TRUE) {
        if (GetItemAttr(app->bagView->itemId, ITEMATTR_PREVENT_TOSS, HEAP_ID_6) || !ItemIdIsNotJohtoBall(app->bagView->itemId)) {
            String *template;
            BufferItemName(app->msgFormat, 0, app->bagView->itemId);
            template = NewString_ReadMsgData(app->msgData, 47);
            StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
            String_Delete(template);
            app->printerId = BagApp_PrintMessage(app, FALSE);
            app->bagView->selectedAction = 5;
            return 15;
        }
        app->bagView->selectedAction = 4;
        return BagApp_StartButtonAnim(app, 20, 41, 36);
    }
    return 14;
}

#pragma force_active reset

int BagApp_State_WaitCantHoldMessage(BagApp *app) {
    if (!TextPrinterCheckActive(app->printerId)) {
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || gSystem.touchNew) {
            BagView *bagView;
            ClearFrameAndWindow2(&app->windows[3], TRUE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
            bagView = app->bagView;
            BagApp_DrawItemList(app, bagView->pockets[bagView->pocketIndex].scroll, -1, 0);
            return 14;
        }
    }
    return 15;
}

#pragma force_active on

int BagApp_State_SellSelectInput(BagApp *app) {
    int changed = 0;
    u8 pressedLeft, pressedRight, pressedL, pressedR, itemChosen;
    int result;

    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        ++changed;
        app->cursorButton = sButtonNavTable[app->cursorButton * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        ++changed;
        app->cursorButton = sButtonNavTableDown[app->cursorButton * 4];
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        int current = app->cursorButton, next = sButtonNavTableLeft[current * 4];
        if (next == 14) {
            result = BagApp_PressButton(app, 14, &pressedLeft, 1, 4, 0);
            if (result == 1) {
                return 16;
            } else {
                return result;
            }
        } else if (current != 16) {
            if (current >= 0 && current < 8) {
                int pocket = BagApp_GetAdjacentPocket(app, current, -1);
                if (app->cursorButton != pocket) {
                    app->cursorButton = pocket;
                    ++changed;
                }
            } else {
                ++changed;
                app->cursorButton = next;
            }
        }
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        int current = app->cursorButton, next = sButtonNavTableRight[current * 4];
        if (next == 15) {
            result = BagApp_PressButton(app, 15, &pressedRight, 1, 4, 0);
            if (result == 1) {
                return 16;
            } else {
                return result;
            }
        } else if (current != 16) {
            if (current >= 0 && current < 8) {
                int pocket = BagApp_GetAdjacentPocket(app, current, 1);
                if (app->cursorButton != pocket) {
                    app->cursorButton = pocket;
                    ++changed;
                }
            } else {
                ++changed;
                app->cursorButton = next;
            }
        }
    } else if (gSystem.newKeys & PAD_BUTTON_L) {
        if (app->cursorButton >= 0 && app->cursorButton < 8) {
            app->cursorButton = BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, -1);
            BagApp_SetCursorPosition(app, app->cursorButton);
            BagApp_PressButton(app, app->cursorButton, &pressedL, 1, 4, 0);
        } else {
            int pocket = BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, -1);
            BagApp_PressButton(app, pocket, &pressedL, 1, 4, 0);
        }
        return 16;
    } else if (gSystem.newKeys & PAD_BUTTON_R) {
        if (app->cursorButton >= 0 && app->cursorButton < 8) {
            app->cursorButton = BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, 1);
            BagApp_SetCursorPosition(app, app->cursorButton);
            BagApp_PressButton(app, app->cursorButton, &pressedR, 1, 4, 0);
        } else {
            int pocket = BagApp_GetAdjacentPocket(app, app->bagView->pocketIndex, 1);
            BagApp_PressButton(app, pocket, &pressedR, 1, 4, 0);
        }
        return 16;
    }
    if (app->cursorButton == 17) {
        app->cursorButton = app->bagView->pocketIndex;
    }
    if (changed) {
        PlaySE(SEQ_SE_DP_SELECT);
        BagApp_SetCursorPosition(app, app->cursorButton);
        BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        BagApp_PrintCursorDescription(app);
    }
    itemChosen = FALSE;
    // called without the hitbox table index, see BagApp_State_HandleInput
    result = ((int (*)(BagApp *))BagApp_GetTouchedButton)(app);
    if (result != -1) {
        if (BagApp_ShouldMoveCursorToButton(app, result)) {
            if ((u32)result < 8) {
                if (BagApp_FindPocketIndex(app, result) != -1) {
                    app->cursorButton = result;
                    BagApp_SetCursorPosition(app, app->cursorButton);
                }
            } else {
                app->cursorButton = result;
                BagApp_SetCursorPosition(app, app->cursorButton);
                if (app->cursorButton >= 8 && app->cursorButton <= 13) {
                    BagApp_SetPocketPositionFromButton(app, app->cursorButton);
                }
            }
        }
        result = BagApp_PressButton(app, result, &itemChosen, 1, 4, 1);
        if (result != 1) {
            app->bagView->selectedAction = 5;
            return result;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        result = BagApp_PressButton(app, app->cursorButton, &itemChosen, 1, 4, 0);
        if (app->cursorButton >= 8 && app->cursorButton <= 13) {
            BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        }
        if (result != 1) {
            app->bagView->selectedAction = 5;
            return result;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        app->bagView->itemId = ITEM_NONE;
        app->bagView->selectedAction = 5;
        BagApp_SetMenuInputState(app, MENU_INPUT_STATE_BUTTONS);
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        if (app->cursorButton >= 8 && app->cursorButton <= 13) {
            BagApp_SetPocketPositionFromButton(app, app->cursorButton);
        }
        return BagApp_StartButtonFlash(app, 19, 9, 8, 36);
    }
    if (itemChosen == TRUE) {
        return BagApp_StartButtonAnim(app, 20, 41, 34);
    }
    return 16;
}

#pragma force_active reset

int BagApp_State_StartSell(BagApp *app) {
    BagViewPocket *pocket = &app->bagView->pockets[app->bagView->pocketIndex];
    String *template;
    s32 price;
    u32 fieldIdx;

    BagApp_LoadSubScreenLayout(app, 4, 0, 0);
    app->quantity = 1;
    price = GetItemAttr(app->bagView->itemId, ITEMATTR_PRICE, HEAP_ID_6);
    fieldIdx = 0;
    app->sellPrice = price >> 1;
    BagApp_ApplyTilemapCommands(app->bgConfig, GF_BG_LYR_SUB_1, 0);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_1);
    ManagedSprite_SetDrawFlag(app->sprites[17], FALSE);
    ManagedSprite_SetDrawFlag(app->sprites[18], FALSE);
    BagApp_ShowPocketTabSprites(app, FALSE);
    BagApp_ShowCancelButton(app, FALSE);
    BagApp_DrawSelectedItemEntry(app, pocket->scroll, app->cursorButton - 8);
    BagApp_ShowSelectedItemIcon(app, app->cursorButton - 8);
    BagApp_ShowItemDescription(app, &app->windows[0], app->bagView->itemId);
    BagApp_HideCursor(app);

    if (GetItemAttr(app->bagView->itemId, ITEMATTR_PREVENT_TOSS, HEAP_ID_6) != 0 || app->sellPrice == 0) {
        template = NewString_ReadMsgData(app->msgData, 76);
        BufferItemName(app->msgFormat, fieldIdx, app->bagView->itemId);
        StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
        String_Delete(template);
        app->printerId = BagApp_PrintMessage(app, FALSE);
        return 24;
    }

    BagApp_PrintMoney(app, FALSE);
    BagApp_PrintSellPrice(app);
    BagApp_PrintQuantityDigits(app, 2);
    // called with a heap ID the function does not take
    if (((u16(*)(ItemSlot *, u32, u16, enum HeapID))Pocket_GetQuantity)(app->bagView->pockets[app->bagView->pocketIndex].slots, app->bagView->pockets[app->bagView->pocketIndex].itemCount, app->bagView->itemId, HEAP_ID_6) == 1) {
        template = NewString_ReadMsgData(app->msgData, 78);
        BufferIntegerAsString(app->msgFormat, 0, app->quantity * app->sellPrice, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
        String_Delete(template);
        app->printerId = BagApp_PrintMessage(app, TRUE);
        return 21;
    }
    app->maxQuantity = pocket->slots[pocket->scroll + app->cursorButton - 8].quantity;
    template = NewString_ReadMsgData(app->msgData, 77);
    BufferItemName(app->msgFormat, fieldIdx, app->bagView->itemId);
    StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
    String_Delete(template);
    app->printerId = BagApp_PrintMessage(app, TRUE);
    return 17;
}

int BagApp_State_SetupSellQuantity(BagApp *app) {
    if (!TextPrinterCheckActive(app->printerId)) {
        if (app->maxQuantity > 99) {
            app->maxQuantity = 99;
        }
        BagApp_LoadSubScreenLayout(app, 4, 0, 0);
        BagApp_ShowQuantityButtons(app, 2, app->maxQuantity);
        BagApp_PrintSellLabel(app);
        BagApp_PrintCancelButton(app, TRUE);
        BagApp_SetCancelSpriteVisible(app, TRUE);
        BagApp_SetQuantityCursorPosition(app, 0);
        return 18;
    }
    return 17;
}

#pragma force_active on

int BagApp_State_SellQuantityInput(BagApp *app) {
    int result = 0;
    int button = BagApp_GetTouchedButton(app, 4);

    if (button != -1) {
        if (BagApp_GetUnusedDigitCount(app->maxQuantity) == 2) {
            if (button == 0 || button == 2) {
                button = -1;
            }
        }
        switch (button) {
        case 0:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, 10);
            ManagedSprite_SetAnimationFrame(app->sprites[32], 0);
            ManagedSprite_SetAnim(app->sprites[32], 26);
            result = 1;
            break;
        case 1:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, 1);
            ManagedSprite_SetAnimationFrame(app->sprites[33], 0);
            ManagedSprite_SetAnim(app->sprites[33], 26);
            result = 1;
            break;
        case 2:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, -10);
            ManagedSprite_SetAnimationFrame(app->sprites[35], 0);
            ManagedSprite_SetAnim(app->sprites[35], 28);
            result = 2;
            break;
        case 3:
            app->quantity = BagApp_StepQuantity(app->quantity, app->maxQuantity, -1);
            ManagedSprite_SetAnimationFrame(app->sprites[36], 0);
            ManagedSprite_SetAnim(app->sprites[36], 28);
            result = 2;
            break;
        case 4:
            result = 3;
            break;
        case 5:
            result = 4;
            break;
        }
    } else {
        result = HandleNumberSelectInput(&app->quantity, app->maxQuantity);
        if (!result) {
            if (gSystem.newKeys & PAD_BUTTON_A) {
                result = 3;
            } else if (gSystem.newKeys & PAD_BUTTON_B) {
                result = 4;
            }
        }
    }
    switch (result) {
    case 1:
        BagApp_PrintSellPrice(app);
        BagApp_PrintQuantityDigits(app, 2);
        PlaySE(SEQ_SE_DP_BAG_004);
        return 18;
    case 2:
        BagApp_PrintSellPrice(app);
        BagApp_PrintQuantityDigits(app, 2);
        PlaySE(SEQ_SE_DP_BAG_004);
        return 18;
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        return BagApp_StartButtonFlash(app, 38, 9, 8, 19);
    case 4:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return BagApp_StartButtonFlash(app, 19, 9, 8, 20);
    }
    return 18;
}

#pragma force_active reset

int BagApp_State_ShowSellPrice(BagApp *app) {
    String *template;
    s32 total;

    EraseFrameAndWindow1(&app->windows[4], TRUE);
    BagApp_ClearQuantityLabel(app);
    FillWindowPixelBuffer(&app->windows[3], 15);
    template = NewString_ReadMsgData(app->msgData, 78);
    total = app->quantity * (u32)app->sellPrice;
    BufferIntegerAsString(app->msgFormat, 0, total, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
    String_Delete(template);
    app->printerId = BagApp_PrintMessage(app, TRUE);
    BagApp_HideQuantityButtons(app);
    BagApp_HideCursor(app);
    BagApp_ShowCancelButton(app, FALSE);
    return 21;
}

int BagApp_State_CancelSellQuantity(BagApp *app) {
    BagView *bagView;

    app->sellPrice = 0;
    EraseFrameAndWindow1(&app->dynWindows[22], TRUE);
    EraseFrameAndWindow1(&app->windows[4], TRUE);
    ClearFrameAndWindow2(&app->windows[3], TRUE);
    ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
    ScheduleWindowCopyToVram(&app->windows[0]);
    BagApp_HideQuantityButtons(app);
    BagApp_HideCursor(app);
    bagView = app->bagView;
    BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), 0);
    BagApp_ClearMoveInfoWindow(app);
    BagApp_CloseItemMenuWindows(app);
    BagApp_RedrawPocket(app);
    BagApp_ShowPocketTabSprites(app, TRUE);
    BagApp_ShowCancelButton(app, TRUE);
    return 16;
}

int BagApp_State_WaitSellPriceMessage(BagApp *app) {
    if (!TextPrinterCheckActive(app->printerId)) {
        BagApp_CreateYesNoPrompt(app);
        return 22;
    }
    return 21;
}

int BagApp_State_SellYesNo(BagApp *app) {
    BagView *bagView;
    String *template;
    u32 input = YesNoPrompt_HandleInput(app->yesNoPrompt);

    switch (input) {
    case YESNORESPONSE_YES:
        BagApp_DestroyYesNoPrompt(app);
        template = NewString_ReadMsgData(app->msgData, 79);
        if (app->quantity > 1) {
            BufferItemNamePlural(app->msgFormat, 0, app->bagView->itemId);
        } else {
            BufferItemName(app->msgFormat, 0, app->bagView->itemId);
        }
        BufferIntegerAsString(app->msgFormat, 1, app->quantity * (u32)app->sellPrice, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(app->msgFormat, app->msgBuf, template);
        String_Delete(template);
        app->printerId = BagApp_PrintMessage(app, FALSE);
        return 23;
    case YESNORESPONSE_NO:
        BagApp_DestroyYesNoPrompt(app);
        app->sellPrice = 0;
        EraseFrameAndWindow1(&app->dynWindows[22], TRUE);
        ClearFrameAndWindow2(&app->windows[3], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
        ScheduleWindowCopyToVram(&app->windows[0]);
        bagView = app->bagView;
        BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), FALSE);
        BagApp_ClearMoveInfoWindow(app);
        BagApp_CloseItemMenuWindows(app);
        BagApp_RedrawPocket(app);
        BagApp_ShowPocketTabSprites(app, TRUE);
        BagApp_ShowCancelButton(app, TRUE);
        return 16;
    case (u32)-1:
        break;
    }
    return 22;
}

#pragma force_active on

int BagApp_State_CompleteSale(BagApp *app) {
    if (TextPrinterCheckActive(app->printerId)) {
        return 23;
    }
    PlaySE(SEQ_SE_DP_REGI);
    PlayerProfile_AddMoney(app->profile, (u32)(s32)app->quantity * app->sellPrice);
    if (app->quantity == 1) {
        if (app->bagView->sellCount != 255) {
            app->bagView->sellCount++;
        }
    } else {
        app->bagView->sellCount = 2;
    }
    Pocket_TakeItem(app->bagView->pockets[app->bagView->pocketIndex].slots, app->bagView->pockets[app->bagView->pocketIndex].itemCount, app->bagView->itemId, (u16)app->quantity, HEAP_ID_6);
    BagApp_LoadPocketItemNames(app);
    ScheduleWindowCopyToVram(&app->windows[3]);
    return 24;
}

#pragma force_active reset

int BagApp_State_WaitSaleMessage(BagApp *app) {
    BagView *bagView;

    if (!TextPrinterCheckActive(app->printerId) && ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || gSystem.touchNew)) {
        app->sellPrice = 0;
        EraseFrameAndWindow1(&app->dynWindows[22], TRUE);
        ClearFrameAndWindow2(&app->windows[3], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[3]);
        ScheduleWindowCopyToVram(&app->windows[0]);
        bagView = app->bagView;
        BagApp_DrawItemIcons(app, &bagView->pockets[bagView->pocketIndex], BagApp_GetPageItemCount(app), TRUE);
        BagApp_ClearMoveInfoWindow(app);
        BagApp_CloseItemMenuWindows(app);
        BagApp_RedrawPocket(app);
        BagApp_PrintCursorDescription(app);
        BagApp_ShowPocketTabSprites(app, TRUE);
        BagApp_ShowCancelButton(app, TRUE);
        return 16;
    }
    return 24;
}

int BagApp_State_DebugSubScreenLayout(BagApp *app) {
    if (BagApp_DebugCycleSubScreenLayout(app) == 1) {
        return 2;
    }
    return 26;
}

int BagApp_ItemAction_ChooseForBerryPatch(BagApp *app) {
    BagApp_CloseItemMenuWindows(app);
    BagApp_ShowCancelButton(app, FALSE);
    BagApp_HideCursor(app);
    BeginAppScreenFade(1, HEAP_ID_6);
    app->bagView->selectedAction = 4;
    return 37;
}

BOOL BagApp_IsBerryOrMulch(u32 pocketId, u32 itemId) {
    if (pocketId == POCKET_BERRIES || (itemId >= ITEM_GROWTH_MULCH && itemId < ITEM_GOOEY_MULCH + 1)) {
        return TRUE;
    }
    return FALSE;
}

void BagApp_LoadPocketSubBgPalette(BagApp *app, int slot, int pocket) {
    const u8 *data = app->bgPalettes[slot]->pRawData;

    if (pocket <= 7) {
        data += pocket * 0x20;
        GXS_LoadBGPltt(data, 0, 0x20);
        GXS_LoadBGPltt(data, 0x20, 0x40);
        GXS_LoadBGPltt(data, 0x60, 0x20);
    }
}

void BagApp_ApplyTilemapCommands(BgConfig *bgConfig, int bgId, int layout) {
    void *buffer = GetBgTilemapBuffer(bgConfig, (u8)bgId);
    const BagAppTilemapCommand *command;
    s32 i;

    if (layout == 6) {
        return;
    }
    command = sSubScreenTilemapCommands[layout];
    for (i = 0; i < 4; i++, command++) {
        if (command->type == 1) {
            LoadRectToBgTilemapRect(bgConfig, (u8)bgId, (u8 *)buffer + (command->srcX + (command->srcY << 5)) * 2, command->destX, command->destY, command->width, command->height);
        } else if (command->type == 2) {
            FillBgTilemapRect(bgConfig, (u8)bgId, 0, command->destX, command->destY, command->width, command->height, 0);
        }
    }
}

void BagApp_DrawItemSlotFrames(BgConfig *bgConfig, int bgId, u32 count, u32 selection) {
    s32 i;
    void *buffer = GetBgTilemapBuffer(bgConfig, (u8)bgId);
    const BagAppTileRect *rect;

    i = 0;
    rect = sUnusedSlotRects[count - 1];
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

// Layouts: 0 item list, 1 moving an item, 2 item menu, 3 trash quantity, 4 sell quantity
void BagApp_LoadSubScreenLayout(BagApp *app, u32 layout, int count, int selection) {
    NNSG2dScreenData *screen;
    void *buffer;

    GF_ASSERT(layout <= 4);
    switch (layout) {
    case 0:
        buffer = GfGfxLoader_GetScrnData(NARC_a_0_1_5, 0x2B, FALSE, &screen, HEAP_ID_6);
        BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_1, screen->rawData, screen->szByte);
        DC_FlushRange(GetBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_1), screen->szByte);
        BagApp_ApplyTilemapCommands(app->bgConfig, GF_BG_LYR_SUB_1, count);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_1);
        Heap_Free(buffer);

        buffer = GfGfxLoader_GetScrnData(NARC_a_0_1_5, 0x27, FALSE, &screen, HEAP_ID_6);
        BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_2, screen->rawData, screen->szByte);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(buffer);
        BagApp_ShowMoveSlotSprites(app, 0, 0, FALSE);
        break;
    case 1:
        buffer = GfGfxLoader_GetScrnData(NARC_a_0_1_5, 0x2C, FALSE, &screen, HEAP_ID_6);
        BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_1, screen->rawData, screen->szByte);
        DC_FlushRange(GetBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_1), screen->szByte);
        BagApp_DrawItemSlotFrames(app->bgConfig, GF_BG_LYR_SUB_1, count, selection);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_1);
        Heap_Free(buffer);

        buffer = GfGfxLoader_GetScrnData(NARC_a_0_1_5, 0x2A, FALSE, &screen, HEAP_ID_6);
        BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_2, screen->rawData, screen->szByte);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(buffer);
        BagApp_HideItemMenuSprites(app);
        BagApp_ShowMoveSlotSprites(app, count, BagApp_GetMovingItemPageSlot(app), FALSE);
        break;
    case 2:
        buffer = GfGfxLoader_GetScrnData(NARC_a_0_1_5, 0x2D, FALSE, &screen, HEAP_ID_6);
        BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_2, screen->rawData, screen->szByte);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(buffer);
        BagApp_ApplyTilemapCommands(app->bgConfig, GF_BG_LYR_SUB_1, count);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_1);
        break;
    case 3:
        buffer = GfGfxLoader_GetScrnData(NARC_a_0_1_5, 0x34, FALSE, &screen, HEAP_ID_6);
        BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_2, screen->rawData, screen->szByte);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(buffer);
        break;
    case 4:
        buffer = GfGfxLoader_GetScrnData(NARC_a_0_1_5, 0x35, FALSE, &screen, HEAP_ID_6);
        BG_LoadScreenTilemapData(app->bgConfig, GF_BG_LYR_SUB_2, screen->rawData, screen->szByte);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_2);
        Heap_Free(buffer);
        break;
    }
}

void BagApp_SetMenuInputState(BagApp *app, MenuInputState state) {
    MenuInputStateMgr *menuInputStateMgr = app->bagView->menuInputStateMgr;
    if (menuInputStateMgr != NULL) {
        MenuInputStateMgr_SetState(menuInputStateMgr, state);
    }
}

void BagApp_ShowCancelButton(BagApp *app, BOOL show) {
    if (show == TRUE) {
        BagApp_SetCancelSpriteVisible(app, TRUE);
        BagApp_PrintCancelButton(app, FALSE);
        ScheduleWindowCopyToVram(&app->windows[7]);
        ManagedSprite_SetAnimationFrame(app->sprites[19], 0);
        ManagedSprite_SetAnim(app->sprites[19], 16);
    } else {
        BagApp_SetCancelSpriteVisible(app, FALSE);
        BagApp_ClearCancelButtonText(app);
    }
}

int BagApp_StartButtonFlash(BagApp *app, u8 sprite, u8 flashPalette, u8 restorePalette, int nextState) {
    BagAppButtonFeedback *feedback = &app->buttonFeedback;
    feedback->isAnim = FALSE;
    feedback->sprite = sprite;
    feedback->flashPalette = flashPalette;
    feedback->restorePalette = restorePalette;
    feedback->nextState = nextState;
    feedback->stage = 0;
    feedback->timer = 0;
    return 35;
}

int BagApp_StartButtonAnim(BagApp *app, u8 sprite, u32 anim, int nextState) {
    BagAppButtonFeedback *feedback = &app->buttonFeedback;
    feedback->isAnim = TRUE;
    feedback->sprite = sprite;
    feedback->nextState = nextState;
    feedback->stage = 0;
    feedback->timer = 0;
    ManagedSprite_SetAnimationFrame(app->sprites[sprite], 0);
    ManagedSprite_SetAnim(app->sprites[sprite], anim);
    return 35;
}

int BagApp_State_ButtonFeedback(BagApp *app) {
    BagAppButtonFeedback *feedback = &app->buttonFeedback;

    if (feedback->isAnim == TRUE) {
        if (ManagedSprite_IsAnimated(app->sprites[feedback->sprite]) == FALSE) {
            return feedback->nextState;
        }
    } else {
        switch (feedback->stage) {
        case 0:
            ManagedSprite_SetPaletteOverride(app->sprites[feedback->sprite], feedback->flashPalette);
            feedback->stage++;
            break;
        case 1:
            feedback->timer++;
            if (feedback->timer == 4) {
                ManagedSprite_SetPaletteOverride(app->sprites[feedback->sprite], feedback->restorePalette);
                feedback->timer = 0;
                feedback->stage++;
            }
            break;
        case 2:
            feedback->timer++;
            if (feedback->timer == 2) {
                return feedback->nextState;
            }
            break;
        }
    }
    return 35;
}

void BagApp_Init3D(BagApp *app) {
    u32 i;

    GF3dRender_InitSimpleManager(HEAP_ID_6);
    G3X_AntiAlias(TRUE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7FFF, 0, FALSE);
    NNS_G3dGlbMaterialColorDiffAmb(GX_RGB(15, 15, 15), GX_RGB(10, 10, 10), FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(GX_RGB(15, 15, 15), GX_RGB(15, 15, 15), FALSE);
    NNS_G3dGlbPolygonAttr(15, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sBagEdgeColorTable);
    SetBgPriority(GF_BG_LYR_MAIN_0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    app->scene.camera = Camera_New(HEAP_ID_6);
    app->scene.cameraTarget = sCameraTarget;
    app->scene.cameraParam = sCameraSetup;
    Camera_Init_FromTargetDistanceAndAngle(&app->scene.cameraTarget, app->scene.cameraParam.distance, &app->scene.cameraParam.angle, app->scene.cameraParam.perspective, app->scene.cameraParam.perspectiveType, TRUE, app->scene.camera);
    app->scene.modelPos = sBagModelPosition;
    BagApp_InitCameraTween(&app->scene);
    BagApp_StartCameraTween(&app->scene, app->bagView->pocketIndex + 1, 7);
    Camera_SetPerspectiveClippingPlane(FX32_CONST(123), FX32_CONST(1700), app->scene.camera);
    Camera_SetStaticPtr(app->scene.camera);
    for (i = 0; i < 4; i++) {
        NNS_G3dGlbLightVector((GXLightId)i, FX16_ONE, 0, 0);
        NNS_G3dGlbLightColor((GXLightId)i, GX_RGB(31, 31, 31));
    }
    BagApp_LoadBagModel(app);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(2);
}

void BagApp_InitCameraTween(BagAppScene *scene) {
    scene->tween.from = 0;
    scene->tween.to = 0;
    scene->tween.queued = -1;
    scene->tween.frame = 7;
    scene->tween.duration = 7;
}

void BagApp_StartCameraTween(BagAppScene *scene, u32 to, u16 duration) {
    if (scene->tween.frame == scene->tween.duration) {
        scene->tween.from = scene->tween.to;
        scene->tween.to = to;
        scene->tween.frame = 0;
        scene->tween.duration = duration;
    } else {
        scene->tween.queued = to;
    }
}

void BagApp_UpdateCameraTween(BagAppScene *scene, int trainerGender) {
    const u8 *keys = (const u8 *)sCameraTweenKeys[trainerGender];
    u32 fromAngle;
    u32 toAngle;
    s32 delta;
    s32 direction;
    s32 angleX;
    s32 angleY;
    s32 distance;
    s32 modelY;
    s32 fromOffset; // byte offsets of the two keys; the fields are read as base + (offset + field), as indexing the
                    // key array allocates the stack differently
    s32 toOffset;
    s32 duration;
    s32 frame;

#define KEY_FIELD(type, offset, field) (*(const type *)(keys + ((offset) + (int)offsetof(BagAppCameraKey, field))))
    if (scene->tween.frame != scene->tween.duration) {
        scene->tween.frame++;
        fromOffset = scene->tween.from << 4;
        fromAngle = KEY_FIELD(u16, fromOffset, angleX);
        toOffset = scene->tween.to << 4;
        toAngle = KEY_FIELD(u16, toOffset, angleX);
        delta = toAngle - fromAngle;
        if (delta < 0) {
            delta = -delta;
        }
        direction = toAngle < fromAngle ? -1 : 1;
        if (delta > 0x8000) {
            delta = 0x10000 - delta;
            direction *= -1;
        }
        if (direction > 0) {
            duration = scene->tween.duration;
            frame = scene->tween.frame;
            angleX = fromAngle + delta / duration * frame;
        } else {
            duration = scene->tween.duration;
            frame = scene->tween.frame;
            angleX = fromAngle - delta / duration * frame;
        }
        angleY = KEY_FIELD(u16, fromOffset, angleY) + (KEY_FIELD(u16, toOffset, angleY) - KEY_FIELD(u16, fromOffset, angleY)) / duration * frame;
        distance = KEY_FIELD(fx32, fromOffset, distance) + (KEY_FIELD(fx32, toOffset, distance) - KEY_FIELD(fx32, fromOffset, distance)) / duration * frame;
        modelY = KEY_FIELD(fx32, fromOffset, modelY) + (KEY_FIELD(fx32, toOffset, modelY) - KEY_FIELD(fx32, fromOffset, modelY)) / duration * frame;
        scene->cameraParam.angle.x = angleX;
        scene->cameraParam.angle.y = angleY;
        scene->cameraParam.distance = distance;
        scene->modelPos.y = modelY;
    }
#undef KEY_FIELD
    if (scene->tween.frame == scene->tween.duration && scene->tween.queued != -1) {
        scene->tween.from = scene->tween.to;
        scene->tween.to = scene->tween.queued;
        scene->tween.queued = -1;
        scene->tween.frame = 0;
    }
}

void BagApp_Free3D(BagApp *app) {
    BagApp_FreeBagModel(app);
    Camera_Delete(app->scene.camera);
    GF3dRender_DeleteSimpleManager();
}

void BagApp_Draw3D(BagApp *app) {
    MtxFx33 rotation = sBagModelRotation;
    VecFx32 scale = sBagModelScale;
    BagAppModel *model;

    BagApp_UpdateCameraTween(&app->scene, app->trainerGender);
    Camera_Init_FromTargetDistanceAndAngle(&app->scene.cameraTarget, app->scene.cameraParam.distance, &app->scene.cameraParam.angle, app->scene.cameraParam.perspective, app->scene.cameraParam.perspectiveType, TRUE, app->scene.camera);
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    model = &app->scene.model;
    BagApp_AdvanceAnimLoop(model->pocketAnims[model->pocket]);
    BagApp_AdvanceAnimLoop(model->pocketAnims2[model->pocket]);
    BagApp_AdvanceAnimLoop(model->extraAnim);
    GF3dRender_DrawModel(&model->renderObj, &app->scene.modelPos, &rotation, &scale);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

void BagApp_AdvanceAnimLoop(NNSG3dAnmObj *anim) {
    fx32 frame = anim->frame + FX32_ONE;
    if (frame < NNS_G3dAnmObjGetNumFrame(anim)) {
        anim->frame = frame;
    } else {
        anim->frame = 0;
    }
}

void BagApp_LoadBagModel(BagApp *app) {
    NARC *narc = NARC_New(NARC_a_0_1_5, HEAP_ID_6);
    BagAppModel *model;
    NNSG3dResTex *tex;
    void *anim;
    void *anim2;
    u32 animMember;
    u32 animMember2;
    u32 extraMember;
    u32 modelMember;
    u32 i;

    HeapExp_FndInitAllocator(&app->scene.allocator, HEAP_ID_6, 4);
    model = &app->scene.model;
    if (app->trainerGender == 0) {
        animMember = 0x39;
        animMember2 = 0x41;
        modelMember = 0x37;
        extraMember = 0x49;
    } else {
        animMember = 0x4C;
        animMember2 = 0x54;
        modelMember = 0x4A;
        extraMember = 0x5C;
    }
    model->modelData = NARC_AllocAndReadWholeMember(narc, modelMember, HEAP_ID_6);
    GF3dRender_InitObjFromHeader(&model->renderObj, &model->model, &model->modelData);
    tex = NNS_G3dGetTex(model->modelData);
    NNSi_G3dModifyMatFlag(model->model, TRUE, NNS_G3D_MATFLAG_DIFFUSE);
    NNSi_G3dModifyMatFlag(model->model, TRUE, NNS_G3D_MATFLAG_AMBIENT);
    NNSi_G3dModifyMatFlag(model->model, TRUE, NNS_G3D_MATFLAG_SPECULAR);
    NNSi_G3dModifyMatFlag(model->model, TRUE, NNS_G3D_MATFLAG_EMISSION);
    NNSi_G3dModifyPolygonAttrMask(model->model, TRUE, 0x3F000000);
    for (i = 0; i < 8; i++) {
        model->pocketAnimData[i] = NARC_AllocAndReadWholeMember(narc, animMember + i, HEAP_ID_6);
        anim = NNS_G3dGetAnmByIdx(model->pocketAnimData[i], 0);
        model->pocketAnims[i] = NNS_G3dAllocAnmObj(&app->scene.allocator, anim, model->model);
        NNS_G3dAnmObjInit(model->pocketAnims[i], anim, model->model, tex);
        model->pocketAnimData2[i] = NARC_AllocAndReadWholeMember(narc, animMember2 + i, HEAP_ID_6);
        anim2 = NNS_G3dGetAnmByIdx(model->pocketAnimData2[i], 0);
        model->pocketAnims2[i] = NNS_G3dAllocAnmObj(&app->scene.allocator, anim2, model->model);
        NNS_G3dAnmObjInit(model->pocketAnims2[i], anim2, model->model, tex);
    }
    model->extraAnimData = NARC_AllocAndReadWholeMember(narc, extraMember, HEAP_ID_6);
    anim = NNS_G3dGetAnmByIdx(model->extraAnimData, 0);
    model->extraAnim = NNS_G3dAllocAnmObj(&app->scene.allocator, anim, model->model);
    NNS_G3dAnmObjInit(model->extraAnim, anim, model->model, tex);
    model->pocket = app->bagView->pocketIndex;
    NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->pocketAnims[model->pocket]);
    NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->pocketAnims2[model->pocket]);
    NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->extraAnim);
    NARC_Delete(narc);
}

void BagApp_FreeBagModel(BagApp *app) {
    BagAppModel *model = &app->scene.model;
    u32 i;

    for (i = 0; i < 8; i++) {
        NNS_G3dFreeAnmObj(&app->scene.allocator, model->pocketAnims[i]);
        NNS_G3dFreeAnmObj(&app->scene.allocator, model->pocketAnims2[i]);
        Heap_Free(model->pocketAnimData[i]);
        Heap_Free(model->pocketAnimData2[i]);
    }
    NNS_G3dFreeAnmObj(&app->scene.allocator, model->extraAnim);
    Heap_Free(model->extraAnimData);
    Heap_Free(model->modelData);
}

void BagApp_SetBagModelPocketAnim(BagApp *app) {
    BagAppModel *model = &app->scene.model;

    NNS_G3dRenderObjRemoveAnmObj(&model->renderObj, model->pocketAnims2[model->pocket]);
    NNS_G3dRenderObjRemoveAnmObj(&model->renderObj, model->pocketAnims[model->pocket]);
    model->pocket = app->bagView->pocketIndex;
    model->pocketAnims[model->pocket]->frame = 0;
    model->pocketAnims2[model->pocket]->frame = 0;
    model->extraAnim->frame = 0;
    NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->pocketAnims[model->pocket]);
    NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->pocketAnims2[model->pocket]);
}

void BagApp_AddWindows(BagApp *app) {
    int i;

    AddWindowParameterized(app->bgConfig, &app->windows[0], GF_BG_LYR_MAIN_1, 0, 0x12, 0x20, 6, 4, 1);
    AddWindowParameterized(app->bgConfig, &app->windows[1], GF_BG_LYR_MAIN_1, 0, 0xD, 0x20, 4, 4, 0xC1);
    AddWindowParameterized(app->bgConfig, &app->windows[2], GF_BG_LYR_SUB_0, 2, 1, 0x1B, 2, 0xB, 1);
    AddWindowParameterized(app->bgConfig, &app->windows[3], GF_BG_LYR_SUB_0, 2, 1, 0x1B, 4, 0xB, 0x53);
    AddWindowParameterized(app->bgConfig, &app->windows[4], GF_BG_LYR_MAIN_1, 0x13, 0xD, 0xC, 4, 4, 0xDB);
    AddWindowParameterized(app->bgConfig, &app->windows[5], GF_BG_LYR_MAIN_1, 1, 0xC, 0xB, 4, 4, 0x12B);
    AddWindowParameterized(app->bgConfig, &app->windows[6], GF_BG_LYR_SUB_0, 0xA, 0x15, 7, 2, 0xB, 0x37);
    FillWindowPixelBuffer(&app->windows[6], 0);
    AddWindowParameterized(app->bgConfig, &app->windows[7], GF_BG_LYR_SUB_0, 0x18, 0x15, 7, 2, 0xB, 0x45);
    FillWindowPixelBuffer(&app->windows[7], 0);

    for (i = 0; i < 24; i++) {
        app->dynWindows[i].bgConfig = NULL;
    }
}

void BagApp_RemoveWindows(BagApp *app) {
    u16 i;
    for (i = 0; i < 8; i++) {
        RemoveWindow(&app->windows[i]);
    }
    BagApp_RemoveItemActionWindows(app);
    BagApp_RemoveItemListWindows(app);
}

void BagApp_AddItemListWindows(BagApp *app) {
    int i;

    if (app->dynWindows[0].bgConfig != NULL) {
        return;
    }
    for (i = 0; i < 12; i++) {
        AddWindowParameterized(app->bgConfig, &app->dynWindows[i], GF_BG_LYR_SUB_0, sItemListWindowTemplates[i].x, sItemListWindowTemplates[i].y, 11, 4, 11, sItemListWindowTemplates[i].baseTile);
    }
}

void BagApp_RemoveItemListWindows(BagApp *app) {
    int i;

    if (app->dynWindows[0].bgConfig == NULL) {
        return;
    }
    for (i = 0; i < 12; i++) {
        ClearWindowTilemapAndScheduleTransfer(&app->dynWindows[i]);
        RemoveWindow(&app->dynWindows[i]);
        app->dynWindows[i].bgConfig = NULL;
    }
}

void BagApp_AddItemActionWindows(BagApp *app) {
    int i;
    int baseTile;
    const BagAppWindowPos *pos;

    if (app->dynWindows[12].bgConfig != NULL) {
        return;
    }
    AddWindowParameterized(app->bgConfig, &app->dynWindows[12], GF_BG_LYR_SUB_0, 12, 7, 11, 4, 11, 0x2CF);
    FillWindowPixelBuffer(&app->dynWindows[12], 0);

    baseTile = 0x31B;
    pos = sItemMenuWindowPositions;
    for (i = 0; i < 4; i++) {
        AddWindowParameterized(app->bgConfig, &app->dynWindows[i + 13], GF_BG_LYR_SUB_0, pos->x, pos->y, 10, 2, 11, baseTile);
        FillWindowPixelBuffer(&app->dynWindows[i + 13], 0);
        baseTile += 20;
        pos++;
    }

    baseTile = 0x2FB;
    pos = sQuantityDigitWindowPositions;
    for (i = 0; i < 3; i++) {
        AddWindowParameterized(app->bgConfig, &app->dynWindows[i + 17], GF_BG_LYR_SUB_0, pos->x, pos->y, 2, 3, 11, baseTile);
        FillWindowPixelBuffer(&app->dynWindows[i + 17], 0);
        baseTile += 6;
        pos++;
    }

    AddWindowParameterized(app->bgConfig, &app->dynWindows[20], GF_BG_LYR_SUB_0, 14, 21, 7, 2, 11, 0x30D);
    FillWindowPixelBuffer(&app->dynWindows[20], 0);
    AddWindowParameterized(app->bgConfig, &app->dynWindows[21], GF_BG_LYR_SUB_0, 11, 1, 18, 4, 11, 0x31B);
    FillWindowPixelBuffer(&app->dynWindows[21], 0);
    AddWindowParameterized(app->bgConfig, &app->dynWindows[22], GF_BG_LYR_SUB_0, 0, 0, 9, 4, 11, 0x363);
    FillWindowPixelBuffer(&app->dynWindows[22], 0);
    AddWindowParameterized(app->bgConfig, &app->dynWindows[23], GF_BG_LYR_SUB_0, 24, 14, 8, 3, 11, 0x387);
    FillWindowPixelBuffer(&app->dynWindows[23], 0);
}

void BagApp_RemoveItemActionWindows(BagApp *app) {
    int i;

    if (app->dynWindows[12].bgConfig == NULL) {
        return;
    }
    for (i = 0; i < 3; i++) {
        ClearWindowTilemapAndScheduleTransfer(&app->dynWindows[i + 17]);
        RemoveWindow(&app->dynWindows[i + 17]);
        app->dynWindows[i + 17].bgConfig = NULL;
    }
    ClearWindowTilemapAndScheduleTransfer(&app->dynWindows[23]);
    RemoveWindow(&app->dynWindows[23]);
    app->dynWindows[23].bgConfig = NULL;
    RemoveWindow(&app->dynWindows[22]);
    app->dynWindows[22].bgConfig = NULL;
    RemoveWindow(&app->dynWindows[21]);
    app->dynWindows[21].bgConfig = NULL;
    ClearWindowTilemapAndScheduleTransfer(&app->dynWindows[20]);
    RemoveWindow(&app->dynWindows[20]);
    app->dynWindows[20].bgConfig = NULL;
    for (i = 0; i < 4; i++) {
        ClearWindowTilemapAndScheduleTransfer(&app->dynWindows[i + 13]);
        RemoveWindow(&app->dynWindows[i + 13]);
        app->dynWindows[i + 13].bgConfig = NULL;
    }
    ClearWindowTilemapAndScheduleTransfer(&app->dynWindows[12]);
    RemoveWindow(&app->dynWindows[12]);
    app->dynWindows[12].bgConfig = NULL;
}

void BagApp_LoadUnusedStrings(BagApp *app) {
    MsgData *msgData;
    u16 i;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 225, HEAP_ID_6);
    for (i = 0; i < 8; i++) {
        app->unusedStrings[i] = NewString_ReadMsgData(msgData, i);
    }
    DestroyMsgData(msgData);
}

void BagApp_FreeUnusedStrings(BagApp *app) {
    u16 i;
    for (i = 0; i < 8; i++) {
        String_Delete(app->unusedStrings[i]);
    }
}

void ov15_021FE528(BagApp *app) {
    u16 i;
    for (i = 0; i < 12; i++) {
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_3, i + 0xCD, i, 13, 1, 1, 4);
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_3, i + 0xF1, i, 14, 1, 1, 4);
    }
}

void BagApp_BufferSlotItemName(BagApp *app, u32 slot, u32 fieldIdx) {
    u16 itemId = BagApp_GetCurrentPocketSlotValue(app, (u16)slot, FALSE);
    BufferItemName(app->msgFormat, fieldIdx, itemId);
}

void BagApp_BufferSlotItemNamePlural(BagApp *app, u32 slot, u32 fieldIdx) {
    u16 itemId = BagApp_GetCurrentPocketSlotValue(app, (u16)slot, FALSE);
    BufferItemNamePlural(app->msgFormat, fieldIdx, itemId);
}

void BagApp_PrintItemDescription(BagApp *app, u16 itemId) {
    String *string;

    if (itemId != 0xFFFF) {
        string = String_New(0x82, HEAP_ID_6);
        GetItemDescIntoString(string, itemId, HEAP_ID_6);
    } else {
        string = NewString_ReadMsgData(app->msgData, 97);
    }
    AddTextPrinterParameterizedWithColor(&app->windows[0], 0, string, 20, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
}

void BagApp_PrintTMHMMoveInfo(BagApp *app, u16 itemId) {
    Window *window = &app->windows[1];
    u16 move = TMHMGetMove(itemId);
    String *string;
    u32 maxPP;
    u16 value;

    string = NewString_ReadMsgData(app->msgData, 101);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(app->msgData, 89);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(app->msgData, 92);
    AddTextPrinterParameterizedWithColor(window, 0, string, 72, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(app->msgData, 90);
    AddTextPrinterParameterizedWithColor(window, 0, string, 168, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(app->msgData, 91);
    AddTextPrinterParameterizedWithColor(window, 0, string, 168, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);

    maxPP = GetMoveMaxPP(move, 0);
    string = NewString_ReadMsgData(app->msgData, 93);
    BufferIntegerAsString(app->msgFormat, 0, maxPP, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, app->msgBuf, string);
    String_Delete(string);
    AddTextPrinterParameterizedWithColor(window, 0, app->msgBuf, 48, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);

    value = GetMoveAttr(move, MOVEATTR_POWER);
    if (value <= 1) {
        string = NewString_ReadMsgData(app->msgData, 25);
    } else {
        string = NewString_ReadMsgData(app->msgData, 94);
    }
    BufferIntegerAsString(app->msgFormat, 0, value, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, app->msgBuf, string);
    String_Delete(string);
    AddTextPrinterParameterizedWithColor(window, 0, app->msgBuf, 232, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);

    value = GetMoveAttr(move, MOVEATTR_ACCURACY);
    if (value == 0) {
        string = NewString_ReadMsgData(app->msgData, 25);
    } else {
        string = NewString_ReadMsgData(app->msgData, 94);
    }
    BufferIntegerAsString(app->msgFormat, 0, value, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, app->msgBuf, string);
    String_Delete(string);
    AddTextPrinterParameterizedWithColor(window, 0, app->msgBuf, 232, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    ScheduleWindowCopyToVram(window);
}

void BagApp_ClearMoveInfoWindow(BagApp *app) {
    ClearWindowTilemapAndScheduleTransfer(&app->windows[1]);
}

void BagApp_LoadQuantityTemplates(BagApp *app) {
    app->unk_5E8 = NewString_ReadMsgData(app->msgData, 39);
    app->quantityTemplate = NewString_ReadMsgData(app->msgData, 38);
}

void BagApp_FreeQuantityTemplates(BagApp *app) {
    String_Delete(app->unk_5E8);
    String_Delete(app->quantityTemplate);
}

void BagApp_MeasureQuantityText(BagApp *app, u16 number, u32 unused2, u32 unused3) {
    String *string = String_New(10, HEAP_ID_6);
    BufferIntegerAsString(app->msgFormat, 0, number, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(app->msgFormat, string, app->quantityTemplate);
    FontID_String_GetWidth(0, string, 0);
    String_Delete(string);
}

#pragma force_active on
#pragma opt_propagation off

void BagApp_PrintTMHMNumber(BagApp *app, Window *window, ItemSlot *slot, int y) {
    u16 itemId = slot->id;
    if (itemId < ITEM_HM01) {
        u16 number = itemId - ITEM_TM01 + 1;
        MessagePrinter_PrintGlyphAndUInt(app->msgPrinter, 2, number, 2, PRINTING_MODE_LEADING_ZEROS, window, 0, y + 5);
        BagApp_MeasureQuantityText(app, slot->quantity, (u16)y, 0x10200);
    } else {
        u16 number = itemId - ITEM_HM01 + 1;
        PrintUIntOnWindow(app->msgPrinter, number, 2, PRINTING_MODE_RIGHT_ALIGN, window, 16, y + 5);
        BagApp_DrawHMMarker(app, window, 16);
    }
}

#pragma opt_propagation reset
#pragma force_active reset

void *BagApp_LoadListIconChars(BagApp *app, NNSG2dCharacterData **charData) {
    void *buffer = NARC_AllocAndReadWholeMember(app->narc, 0x25, HEAP_ID_6);
    NNS_G2dGetUnpackedBGCharacterData(buffer, charData);
    return buffer;
}

void BagApp_DrawHMMarker(BagApp *app, Window *window, u32 y) {
    NNSG2dCharacterData *charData;
    void *buffer = BagApp_LoadListIconChars(app, &charData);

    BlitBitmapRectToWindow(window, charData->pRawData, 0, 0, 104, 16, 0, y, 24, 16);
    Heap_FreeExplicit(HEAP_ID_6, buffer);
}
