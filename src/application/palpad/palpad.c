#include "global.h"

#include "application/palpad/palpad_internal.h"
#include "msgdata/msg.naix"

#include "brightness.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "menu_input_state.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "render_text.h"
#include "render_window.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "vram_transfer_manager.h"

FS_EXTERN_OVERLAY(OVY_42);

extern const GraphicsModes sPalPadGraphicsModes;
extern const ObjCharTransferTemplate sPalPadObjCharTransferTemplate;
extern const GraphicsBanks sPalPadGraphicsBanks;
extern const BgTemplate sPalPadBgTemplates[7];
extern const u8 sPalPadScreenFileIds[2];
extern NNSG2dViewRect sPalPadRenderSurfaceRect;
extern SpriteTemplate sPalPadArrowSpriteTemplates[2];
extern SpriteTemplate sPalPadCursorSpriteTemplate;
extern u8 ov43_0222F160[];

BOOL PalPad_Init(OverlayManager *man, int *state) {
    PalPadWork *work;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_42), OVY_LOAD_ASYNC);
    Heap_Create(HEAP_ID_3, HEAP_ID_51, 7 << 16);
    work = OverlayManager_CreateAndGetData(man, sizeof(PalPadWork), HEAP_ID_51);
    memset(work, 0, sizeof(PalPadWork));
    work->args = OverlayManager_GetArgs(man);
    PalPadShared_Init(&work->shared, work->args, HEAP_ID_51);
    PalPadGfx_Init(&work->gfx, &work->shared, HEAP_ID_51);
    PalPadMenu_Init(&work->menu, &work->shared, &work->gfx, HEAP_ID_51);
    PalPadRoster_Init(&work->roster, &work->shared, &work->gfx, HEAP_ID_51);
    PalPadNaming_Init(&work->naming, &work->shared, &work->gfx, HEAP_ID_51);
    PalPadFriendCode_Init(&work->friendCode, &work->shared, &work->gfx, HEAP_ID_51);
    PalPadDetails_Init(&work->details, &work->shared, &work->gfx, HEAP_ID_51);
    Main_SetVBlankIntrCB(PalPad_VBlankCallback, work);
    HBlankInterruptDisable();
    return TRUE;
}

BOOL PalPad_Main(OverlayManager *man, int *state) {
    PalPadWork *work = OverlayManager_GetData(man);
    int result;

    switch (*state) {
    case 0:
        switch (work->screen) {
        case PALPAD_SCREEN_MENU:
            result = PalPadMenu_Main(&work->menu, &work->shared, &work->gfx, HEAP_ID_51);
            break;
        case PALPAD_SCREEN_ROSTER:
            result = PalPadRoster_Main(&work->roster, &work->shared, &work->gfx, HEAP_ID_51);
            break;
        case PALPAD_SCREEN_NAMING:
            result = PalPadNaming_Main(&work->naming, work, &work->shared, &work->gfx, HEAP_ID_51);
            break;
        case PALPAD_SCREEN_FRIEND_CODE:
            result = PalPadFriendCode_Main(&work->friendCode, &work->shared, &work->gfx);
            break;
        case PALPAD_SCREEN_DETAILS:
            result = PalPadDetails_Main(&work->details, &work->shared, &work->gfx, HEAP_ID_51);
            break;
        case PALPAD_SCREEN_EXIT:
            return TRUE;
        }
        if (result == 1) {
            work->screen = work->shared.nextScreen;
            work->shared.subState = work->shared.nextSubState;
            work->screenChangeDelay = 4;
            *state = 1;
        }
        break;
    case 1:
        work->screenChangeDelay--;
        if (work->screenChangeDelay <= 0) {
            *state = 0;
        }
        break;
    }
    PalPadGfx_RenderSprites(&work->gfx);
    return FALSE;
}

BOOL PalPad_Exit(OverlayManager *man, int *state) {
    PalPadWork *work;
    OverlayManager *manager = man; // kept in its own register by the original

    work = OverlayManager_GetData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    PalPadMenu_Exit(&work->menu, &work->shared, &work->gfx);
    PalPadRoster_Exit(&work->roster, &work->shared, &work->gfx);
    PalPadNaming_Exit(&work->naming, &work->shared, &work->gfx);
    PalPadFriendCode_Exit(&work->friendCode, &work->shared, &work->gfx);
    PalPadDetails_Exit(&work->details, &work->shared, &work->gfx);
    PalPadGfx_Free(&work->gfx);
    PalPadShared_Free(&work->shared, work->args);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_51);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
    return TRUE;
}

void PalPad_VBlankCallback(void *data) {
    PalPadWork *work = data;

    PalPadGfx_VBlank(&work->gfx);
}

// Rebuilds the screens after a nested application (naming screen) gave the graphics back.
void PalPad_SetupScreens(PalPadWork *work, enum HeapID heapId) {
    PalPadGfx_Init(&work->gfx, &work->shared, heapId);
    PalPadMenu_Setup(&work->menu, &work->shared, &work->gfx, heapId);
    PalPadRoster_Setup(&work->roster, &work->shared, &work->gfx, heapId);
    PalPadNaming_Setup(&work->naming, &work->shared, &work->gfx, heapId);
    PalPadFriendCode_Setup(&work->friendCode, &work->shared, &work->gfx, heapId);
    PalPadDetails_Setup(&work->details, &work->shared, &work->gfx, heapId);
    Main_SetVBlankIntrCB(PalPad_VBlankCallback, work);
    HBlankInterruptDisable();
}

void PalPad_TeardownScreens(PalPadWork *work) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    PalPadMenu_Teardown(&work->menu, &work->shared, &work->gfx);
    PalPadRoster_Teardown(&work->roster, &work->shared, &work->gfx);
    PalPadNaming_Teardown(&work->naming, &work->shared, &work->gfx);
    PalPadFriendCode_Teardown(&work->friendCode, &work->shared, &work->gfx);
    PalPadDetails_Teardown(&work->details, &work->shared, &work->gfx);
    PalPadGfx_Free(&work->gfx);
}

void PalPad_AddWindow(BgConfig *bgConfig, Window *window, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile, u8 fillValue) {
    InitWindow(window);
    AddWindowParameterized(bgConfig, window, bgId, x, y, width, height, paletteNum, baseTile);
    FillWindowPixelBuffer(window, fillValue);
}

void PalPadShared_Init(PalPadShared *shared, PalPadArgs *args, enum HeapID heapId) {
    shared->saveData = args->saveData;
    shared->name = String_New(0x80, heapId);
    shared->code = String_New(0x80, heapId);
    shared->inputState = MenuInputStateMgr_GetState(args->menuInputStatePtr);
    Options *options = Save_PlayerData_GetOptionsAddr(shared->saveData);
    shared->textSpeed = Options_GetTextFrameDelay(options);
    shared->frame = Options_GetFrame(options);
    shared->lrPaging = Options_GetButtonMode(options) == 1 ? FALSE : TRUE;
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
}

void PalPadShared_Free(PalPadShared *shared, PalPadArgs *args) {
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    MenuInputStateMgr_SetState(args->menuInputStatePtr, (MenuInputState)shared->inputState);
    String_Delete(shared->name);
    String_Delete(shared->code);
}

void PalPadShared_SetNextScreen(PalPadShared *shared, u8 screen, u8 subState) {
    shared->nextScreen = screen;
    shared->nextSubState = subState;
}

#pragma force_active on

void PalPadShared_ResetInputStateOnKey(u32 *inputState) {
    if (gSystem.newKeys & PALPAD_KEYS_ANY) {
        *inputState = 0;
    }
}

#pragma force_active reset

void PalPadShared_SetNameAndCode(PalPadShared *shared, const String *name, const String *code) {
    if (name) {
        String_Copy(shared->name, name);
    }
    if (code) {
        String_Copy(shared->code, code);
    }
}

#pragma force_active on

// Returns 0 when registered, 1 for an invalid code (or own code, or no free slot), 2 if the friend
// is already registered.
int PalPadShared_RegisterFriendCode(PalPadShared *shared, String *code, String *name) {
    int i, valid, slot;
    u64 ownKey;
    FriendRoster *wifiList;

    wifiList = Save_FriendRoster_Get(shared->saveData);
    ownKey = DWC_CreateFriendKey(FriendRoster_GetUserData(wifiList));
    for (i = 0; i < PALPAD_FRIEND_MAX; i++) {
        if (FriendRoster_IsFriendValid(wifiList, i) == 0) {
            u64 key = String_atoi(code, &valid);
            if (valid == TRUE && key != ownKey) {
                if (!DWC_CheckFriendKey(FriendRoster_GetUserData(wifiList), key)) {
                    return 1;
                }
                if (!Comm_FindFriendSlotByKey(shared->saveData, key, &slot)) {
                    return 2;
                }
                DWC_CreateFriendKeyToken(FriendRoster_GetDwcFriendData(wifiList, i), key);
                FriendRoster_SetPlayerName(wifiList, i, name);
                FriendRoster_SetEntryField(wifiList, i, 8, 2);
                return 0;
            }
            return 1;
        }
    }
    GF_AssertFail();
    return 0;
}

#pragma force_active reset

void PalPadShared_SetInitialName(PalPadShared *shared, const u16 *name) {
    shared->initialName = name;
    shared->useInitialName = TRUE;
}

void PalPadShared_SetSelectedFriend(PalPadShared *shared, int index) {
    shared->selectedFriend = index;
}

#pragma force_active on

void PalPadShared_ReadFriendList(PalPadShared *shared) {
    FriendRoster *wifiList;
    int i;

    memset(&shared->friends, 0, sizeof(shared->friends));
    wifiList = Save_FriendRoster_Get(shared->saveData);
    shared->friends.count = 0;
    for (i = 0; i < PALPAD_FRIEND_MAX; i++) {
        if (FriendRoster_IsFriendValid(wifiList, i) == 1) {
            shared->friends.slots[shared->friends.count] = i;
            if (FriendRoster_GetEntryField(wifiList, i, 8) == 2) {
                shared->friends.numbers[shared->friends.count] = 0xFF;
            } else {
                shared->friends.numbers[shared->friends.count] = shared->friends.numberedCount++;
            }
            shared->friends.count++;
        }
    }
}

void PalPadGfx_Init(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapId) {
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    gfx->narc = NARC_New(NARC_a_0_8_5, heapId);
    PalPadGfx_SetVramBanks();
    PalPadGfx_InitBgs(gfx, heapId);
    PalPadGfx_InitSpriteSystem(gfx, heapId);
    PalPadGfx_InitMsg(gfx, heapId);
    PalPadGfx_InitWindows(gfx, shared, heapId);
    PalPadGfx_CreateSprites(gfx, heapId);
    PalPadGfx_LoadScreens(gfx, heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
}

#pragma force_active reset

void PalPadGfx_RenderSprites(PalPadGfx *gfx) {
    SpriteList_RenderAndAnimateSprites(gfx->spriteList);
}

void PalPadGfx_VBlank(PalPadGfx *gfx) {
    DoScheduledBgGpuUpdates(gfx->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
}

void PalPadGfx_Free(PalPadGfx *gfx) {
    PalPadGfx_DeleteSprites(gfx);
    PalPadGfx_FreeScreens(gfx);
    PalPadGfx_FreeWindows(gfx);
    PalPadGfx_FreeMsg(gfx);
    PalPadGfx_FreeSpriteSystem(gfx);
    PalPadGfx_FreeBgs(gfx);
    NARC_Delete(gfx->narc);
}

void PalPadGfx_SetVramBanks(void) {
    GraphicsBanks banks = sPalPadGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void PalPadGfx_InitBgs(PalPadGfx *gfx, enum HeapID heapId) {
    GraphicsModes modes;
    int i;

    gfx->bgConfig = BgConfig_Alloc(heapId);
    modes = sPalPadGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(gfx->bgConfig, i, &sPalPadBgTemplates[i], 0);
        BG_ClearCharDataRange(i, 0x20, 0, heapId);
        BgClearTilemapBufferAndCommit(gfx->bgConfig, i);
    }
    GfGfxLoader_GXLoadPalFromOpenNarc(gfx->narc, 5, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x160, heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(gfx->narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0xA0, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(gfx->narc, 7, gfx->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(gfx->narc, 7, gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(gfx->narc, 6, gfx->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(gfx->narc, 10, gfx->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(gfx->narc, 8, gfx->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, heapId);
}

void PalPadGfx_FreeBgs(PalPadGfx *gfx) {
    int i;

    for (i = 0; i < 7; i++) {
        FreeBgTilemapBuffer(gfx->bgConfig, i);
    }
    Heap_Free(gfx->bgConfig);
    gfx->bgConfig = NULL;
}

void PalPadGfx_InitSpriteSystem(PalPadGfx *gfx, enum HeapID heapId) {
    ObjCharTransferTemplate template;
    int i;

    NNS_G2dInitOamManagerModule();
    GF_CreateVramTransferManager(0x10, heapId);
    OamManager_Create(0, 0x7E, 0, 0x1E, 0, 0x7E, 0, 0x1E, heapId);
    template = sPalPadObjCharTransferTemplate;
    template.heapID = heapId;
    ObjCharTransfer_InitEx(&template, (GXOBJVRamModeChar)0x10, (GXOBJVRamModeChar)0x10);
    ObjPlttTransfer_Init(0x40, heapId);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    gfx->spriteList = G2dRenderer_Init(0x40, &gfx->renderer, heapId);
    OamManager_InitRenderSurface(&gfx->renderSurface, &sPalPadRenderSurfaceRect, NNS_G2D_VRAM_TYPE_2DMAIN, &gfx->renderer.rendererInstance);
    for (i = 0; i < 4; i++) {
        gfx->resourceManagers[i] = Create2DGfxResObjMan(0x40, (GfGfxResType)i, heapId);
    }
    gfx->spriteResources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resourceManagers[GF_GFX_RES_TYPE_CHAR], gfx->narc, 1, TRUE, 100, NNS_G2D_VRAM_TYPE_2DMAIN, heapId);
    gfx->spriteResources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(gfx->resourceManagers[GF_GFX_RES_TYPE_PLTT], gfx->narc, 0, FALSE, 100, NNS_G2D_VRAM_TYPE_2DMAIN, 5, heapId);
    gfx->spriteResources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resourceManagers[GF_GFX_RES_TYPE_CELL], gfx->narc, 2, TRUE, 100, GF_GFX_RES_TYPE_CELL, heapId);
    gfx->spriteResources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resourceManagers[GF_GFX_RES_TYPE_ANIM], gfx->narc, 3, TRUE, 100, GF_GFX_RES_TYPE_ANIM, heapId);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(gfx->spriteResources[GF_GFX_RES_TYPE_CHAR]));
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(gfx->spriteResources[GF_GFX_RES_TYPE_PLTT]));
    GF2DGfxResObj_FreeData(gfx->spriteResources[GF_GFX_RES_TYPE_CHAR]);
    GF2DGfxResObj_FreeData(gfx->spriteResources[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&gfx->spriteHeader, 100, 100, 100, 100, -1, -1, 0, 0, gfx->resourceManagers[GF_GFX_RES_TYPE_CHAR], gfx->resourceManagers[GF_GFX_RES_TYPE_PLTT], gfx->resourceManagers[GF_GFX_RES_TYPE_CELL], gfx->resourceManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

void PalPadGfx_FreeSpriteSystem(PalPadGfx *gfx) {
    int i;

    SpriteTransfer_DeleteCharTransferTask(gfx->spriteResources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(gfx->spriteResources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(gfx->resourceManagers[i], gfx->spriteResources[i]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resourceManagers[i]);
    }
    SpriteList_Delete(gfx->spriteList);
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
    GF_DestroyVramTransferManager();
}

void PalPadGfx_InitMsg(PalPadGfx *gfx, enum HeapID heapId) {
    gfx->msgFormat = MessageFormat_New(heapId);
    gfx->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0779_bin, heapId);
    FontID_Alloc(4, heapId);
}

void PalPadGfx_FreeMsg(PalPadGfx *gfx) {
    FontID_Release(4);
    DestroyMsgData(gfx->msgData);
    MessageFormat_Delete(gfx->msgFormat);
}

#pragma force_active on

void PalPadGfx_InitWindows(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapId) {
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, heapId);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, heapId);
    LoadUserFrameGfx2(gfx->bgConfig, GF_BG_LYR_MAIN_1, 70, 12, shared->frame, heapId);
    InitWindow(&gfx->titleWindow);
    AddWindowParameterized(gfx->bgConfig, &gfx->titleWindow, GF_BG_LYR_MAIN_1, 1, 0, 24, 3, 11, 100);
    gfx->titleText = String_New(0x80, heapId);
    gfx->titleBuffer = String_New(0x80, heapId);
    gfx->pageText = String_New(0x80, heapId);
    gfx->pageBuffer = String_New(0x80, heapId);
    gfx->listMenuSpawner = TouchscreenListMenuSpawner_Create(heapId, 0);
    gfx->yesNoPrompt = YesNoPrompt_Create(heapId);
}

#pragma force_active reset

void PalPadGfx_FreeWindows(PalPadGfx *gfx) {
    YesNoPrompt_Destroy(gfx->yesNoPrompt);
    TouchscreenListMenuSpawner_Destroy(gfx->listMenuSpawner);
    String_Delete(gfx->pageText);
    String_Delete(gfx->pageBuffer);
    String_Delete(gfx->titleText);
    String_Delete(gfx->titleBuffer);
    RemoveWindow(&gfx->titleWindow);
}

#pragma force_active on

void PalPadGfx_LoadScreens(PalPadGfx *gfx, enum HeapID heapId) {
    int i;

    for (i = 0; i < 2; i++) {
        gfx->screens.files[i] = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, sPalPadScreenFileIds[i], TRUE, &gfx->screens.data[i], heapId);
    }
}

#pragma force_active reset

void PalPadGfx_FreeScreens(PalPadGfx *gfx) {
    int i;

    for (i = 0; i < 2; i++) {
        Heap_Free(gfx->screens.files[i]);
    }
}

#pragma force_active on

void PalPadGfx_PrintTitle(PalPadGfx *gfx, Window *window, int msgId) {
    if (TextPrinterCheckActive(gfx->titlePrinterId) == TRUE) {
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        RemoveTextPrinter(gfx->titlePrinterId);
    }
    FillWindowPixelBuffer(window, 0);
    ReadMsgDataIntoString(gfx->msgData, msgId, gfx->titleBuffer);
    StringExpandPlaceholders(gfx->msgFormat, gfx->titleText, gfx->titleBuffer);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    gfx->titlePrinterId = AddTextPrinterParameterizedWithColor(window, 1, gfx->titleText, 0, 4, 2, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    ScheduleWindowCopyToVram(window);
}

#pragma force_active reset

void PalPadGfx_ClearTitle(PalPadGfx *gfx) {
    if (TextPrinterCheckActive(gfx->titlePrinterId) == TRUE) {
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        RemoveTextPrinter(gfx->titlePrinterId);
    }
    ClearWindowTilemapAndScheduleTransfer(&gfx->titleWindow);
}

#pragma force_active on

// 12-digit friend code as three groups of four digits
void PalPadGfx_BufferFriendCode(PalPadGfx *gfx, u64 friendCode) {
    BufferIntegerAsString(gfx->msgFormat, 1, friendCode / 100000000, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(gfx->msgFormat, 2, friendCode / 10000 % 10000, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(gfx->msgFormat, 3, friendCode % 10000, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
}

#pragma force_active reset

void PalPadGfx_BufferFriendName(PalPadGfx *gfx, SaveData *saveData, int slot, enum HeapID heapId) {
    FriendRoster *wifiList = Save_FriendRoster_Get(saveData);
    PlayerProfile *profile = PlayerProfile_New(heapId);

    Save_Profile_PlayerName_Set(profile, FriendRoster_GetPlayerName(wifiList, slot));
    BufferPlayersName(gfx->msgFormat, 0, profile);
    Heap_Free(profile);
}

void ov43_0222AB5C(PalPadGfx *gfx, SaveData *saveData, int slot, enum HeapID heapId) {
    PlayerProfile *profile = PlayerProfile_New(heapId);
    FriendRoster *wifiList = Save_FriendRoster_Get(saveData);

    Save_Profile_PlayerName_Set(profile, FriendRoster_GetGroupName(wifiList, slot));
    BufferPlayersName(gfx->msgFormat, 0, profile);
    Heap_Free(profile);
}

#pragma force_active on

// Buffers the date of the last meeting; FALSE if the friend was never met.
BOOL PalPadGfx_BufferLastDate(PalPadGfx *gfx, SaveData *saveData, int slot) {
    FriendRoster *wifiList = Save_FriendRoster_Get(saveData);
    BOOL met = TRUE;
    int day = FriendRoster_GetEntryField(wifiList, slot, 6);

    if (!day) {
        met = FALSE;
    }
    BufferIntegerAsString(gfx->msgFormat, 2, day, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BufferIntegerAsString(gfx->msgFormat, 0, FriendRoster_GetEntryField(wifiList, slot, 4), 4, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BufferMonthNameAbbr(gfx->msgFormat, 1, FriendRoster_GetEntryField(wifiList, slot, 5));
    return met;
}

#pragma force_active reset

void PalPadGfx_BufferNumber(PalPadGfx *gfx, int value) {
    BufferIntegerAsString(gfx->msgFormat, 0, value, 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
}

void PalPadGfx_BufferActivityName(PalPadGfx *gfx, int activity) {
    BufferWiFiPlazaActivityName(gfx->msgFormat, 0, activity);
}

#pragma force_active on

void PalPadGfx_CreateSprites(PalPadGfx *gfx, enum HeapID heapId) {
    SpriteResourcesHeader *header;
    int i, animSeq;
    SpriteTemplate *template;
    SpriteTemplate *create;
    PalPadGfx *arrowSlot; // moves one Sprite pointer per arrow; written through ->arrows[0]

    i = 0;
    arrowSlot = gfx;
    template = sPalPadArrowSpriteTemplates;
    create = template;
    animSeq = 5;
    header = &gfx->spriteHeader;
    for (; i < 2; i++) {
        template->spriteList = gfx->spriteList;
        template->header = header;
        template->heapID = heapId;
        arrowSlot->arrows[0] = Sprite_CreateAffine(create);
        Sprite_SetAnimActiveFlag(arrowSlot->arrows[0], TRUE);
        Sprite_SetDrawFlag(arrowSlot->arrows[0], FALSE);
        Sprite_SetAnimCtrlSeq(arrowSlot->arrows[0], animSeq);
        template ++;
        create++;
        arrowSlot = (PalPadGfx *)((u8 *)arrowSlot + sizeof(Sprite *));
        animSeq += 2;
    }
    // the original addresses the cursor template from a label 0x14 bytes into the arrow templates
    ((SpriteTemplate *)(ov43_0222F160 + 0x4C))->spriteList = gfx->spriteList;
    ((SpriteTemplate *)(ov43_0222F160 + 0x4C))->header = &gfx->spriteHeader;
    ((SpriteTemplate *)(ov43_0222F160 + 0x4C))->heapID = heapId;
    gfx->cursor = Sprite_CreateAffine(&sPalPadCursorSpriteTemplate);
    Sprite_SetDrawFlag(gfx->cursor, FALSE);
    Sprite_SetAnimCtrlSeq(gfx->cursor, 1);
}

#pragma force_active reset

void PalPadGfx_DeleteSprites(PalPadGfx *gfx) {
    int i;

    Sprite_Delete(gfx->cursor);
    for (i = 0; i < 2; i++) {
        Sprite_Delete(gfx->arrows[i]);
    }
}

void PalPadGfx_SetArrowsVisible(PalPadGfx *gfx, BOOL visible) {
    int i;

    for (i = 0; i < 2; i++) {
        Sprite_SetDrawFlag(gfx->arrows[i], visible);
    }
}

void PalPadGfx_SetArrowsAnimActive(PalPadGfx *gfx, BOOL active) {
    int i;

    for (i = 0; i < 2; i++) {
        Sprite_SetAnimActiveFlag(gfx->arrows[i], active);
    }
}

// arrow 0/1: left/right (roster), 2/3: up/down (details; same sprites)
void PalPadGfx_AnimateArrow(PalPadGfx *gfx, u8 arrow, int pressed) {
    u8 animSeq = arrow * 2;

    if (arrow >= 2) {
        arrow -= 2;
    }
    Sprite_SetAnimActiveFlag(gfx->arrows[arrow], TRUE);
    Sprite_SetAnimCtrlSeq(gfx->arrows[arrow], animSeq + 5 + pressed);
    Sprite_ResetAnimCtrlState(gfx->arrows[arrow]);
}

void PalPadGfx_SetCursor(PalPadGfx *gfx, BOOL visible, int animSeq) {
    if (animSeq != 0xFF) {
        Sprite_SetAnimCtrlSeq(gfx->cursor, animSeq);
    }
    Sprite_SetDrawFlag(gfx->cursor, visible);
}

#pragma force_active on

void PalPadGfx_SetCursorPriority(PalPadGfx *gfx, u8 priority, int drawPriority) {
    Sprite_SetPriority(gfx->cursor, priority);
    Sprite_SetDrawPriority(gfx->cursor, drawPriority);
}

void PalPadGfx_StartYesNo(u32 *inputState, PalPadGfx *gfx, int initialCursorPos) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = gfx->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_1;
    template.tileStart = 10;
    template.plttSlot = 13;
    template.x = 25;
    template.y = 9;
    template.ignoreTouchFlag = *inputState;
    template.initialCursorPos = initialCursorPos;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(gfx->yesNoPrompt, &template);
    StartBrightnessTransition(4, 8, 0, (GXBlendPlaneMask)61, SCREEN_MASK_MAIN);
}

#pragma force_active reset

int PalPadGfx_HandleYesNo(u32 *inputState, PalPadGfx *gfx) {
    int result = YesNoPrompt_HandleInput(gfx->yesNoPrompt);

    if (result == YESNORESPONSE_YES || result == YESNORESPONSE_NO) {
        *inputState = YesNoPrompt_IsInTouchMode(gfx->yesNoPrompt);
        YesNoPrompt_Reset(gfx->yesNoPrompt);
        SetBlendBrightness(0, (GXBlendPlaneMask)61, SCREEN_MASK_MAIN);
    }
    return result;
}
