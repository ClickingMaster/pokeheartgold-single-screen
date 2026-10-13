#include <nnsys/gfd_internal.h>

#include "global.h"

#include "application/dress_up/dress_up_internal.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0215.h"

#include "bg_window.h"
#include "brightness.h"
#include "fashion_case.h"
#include "font.h"
#include "game_stats.h"
#include "heap.h"
#include "launch_application_internal.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokepic.h"
#include "render_text.h"
#include "render_window.h"
#include "sound_system.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "unk_02009D48.h"
#include "unk_0200FA24.h"
#include "yes_no_prompt.h"

// Pokemon Dress-Up editor (ScrCmd_154): app Init/Main/Exit, the editor's graphics, canvas, tray, frame BG,
// buttons and HUD set-up, the DONE / return transitions, the two yes/no prompts and saving the design.

// fashion save entry accessors (src/save_fashion.c)
void FashionEntry_MarkSaved(SaveFashionDataSub *entry);
void FashionEntry_Clear(SaveFashionDataSub *entry);
void FashionEntry_SetMon(SaveFashionDataSub *entry, Pokemon *mon, DressUpMonPic *monPic);
void FashionEntry_PlaceItem(SaveFashionDataSub *entry, DressUpSlot *slot, int index);
void FashionEntry_SetBackdrop(SaveFashionDataSub *entry, int value);
void FashionEntry_SetMonSecondText(SaveFashionDataSub *entry, String *string, u32 gender); // u8 in save_fashion.c; the caller does not truncate

typedef struct DressUpTransition {
    DressUpWork *work;
    u32 *done;
    int counter;
    int state;
} DressUpTransition;

void DressUp_FinishTransitionTask(SysTask *task, DressUpTransition *env);
void DressUp_ReturnTransitionTask(SysTask *task, DressUpTransition *env);
BOOL DressUp_SlideCanvasStep(DressUpTransition *env, int dx, int dy, int frames);

void DressUpGfx_CreatePokepicManager(DressUpGfx *gfx, enum HeapID heapID, u32 texSize, u32 plttSize) {
    u32 key;

    gfx->pokepicManager = PokepicManager_Create(heapID);
    gfx->texKey = NNS_GfdDefaultFuncAllocTexVram(texSize, FALSE, 0);
    gfx->plttKey = NNS_GfdDefaultFuncAllocPlttVram(plttSize, FALSE, 1);

    key = gfx->texKey;
    PokepicManager_SetCharBaseAddrAndSize(gfx->pokepicManager, (key << 16) >> 13, ((key & 0x7FFF0000) >> 16) << 4);
    key = gfx->plttKey;
    PokepicManager_SetPlttBaseAddrAndSize(gfx->pokepicManager, (key << 16) >> 13, ((key & 0xFFFF0000) >> 16) << 3);
    gfx->pokepicActive = TRUE;
}

void DressUpGfx_DestroyPokepicManager(DressUpGfx *gfx) {
    PokepicManager_Delete(gfx->pokepicManager);
    NNS_GfdDefaultFuncFreeTexVram(gfx->texKey);
    NNS_GfdDefaultFuncFreePlttVram(gfx->plttKey);
    gfx->pokepicActive = FALSE;
}

void DressUpGfx_AllocRawResTables(DressUpGfxResources *resources, int imageCount, int paletteCount, enum HeapID heapID) {
    u32 size = imageCount * sizeof(GraphicsImageTemplate);
    resources->images = Heap_Alloc(heapID, size);
    memset(resources->images, 0, size);
    resources->charResMan = GF2dGfxRawResMan_Create(imageCount, heapID);
    resources->imageCount = imageCount;

    size = paletteCount * sizeof(GraphicsPaletteTemplate);
    resources->palettes = Heap_Alloc(heapID, size);
    memset(resources->palettes, 0, size);
    resources->plttResMan = GF2dGfxRawResMan_Create(paletteCount, heapID);
    resources->paletteCount = paletteCount;
}

void DressUpGfx_FreeRawResTables(DressUpGfxResources *resources) {
    if (resources->charResMan != NULL) {
        GF2dGfxRawResObj_Destroy(resources->charResMan);
        resources->charResMan = NULL;
    }
    if (resources->plttResMan != NULL) {
        GF2dGfxRawResObj_Destroy(resources->plttResMan);
        resources->plttResMan = NULL;
    }
    Heap_Free(resources->images);
    resources->images = NULL;
    Heap_Free(resources->palettes);
    resources->palettes = NULL;
}

BOOL DressUp_Init(OverlayManager *man, int *state) {
    DressUpWork *work;
    DressUpArgs *args;

    Heap_Create(HEAP_ID_3, HEAP_ID_13, 0x20000);
    Heap_Create(HEAP_ID_3, HEAP_ID_14, 0x40000);
    work = OverlayManager_CreateAndGetData(man, sizeof(DressUpWork), HEAP_ID_13);
    memset(work, 0, sizeof(DressUpWork));
    Main_SetVBlankIntrCB(DressUp_VBlankCB, work);
    HBlankInterruptDisable();
    args = OverlayManager_GetArgs(man);
    work->options = args->options;
    if (args->menuInputStatePtr != NULL) {
        work->menuInputState = MenuInputStateMgr_GetState(args->menuInputStatePtr);
    } else {
        work->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    DressUp_LoadFashionCaseTable(args->fashionCase, work->caseTable);
    DressUp_InitGraphics(work);
    DressUpNodePool_Init(&work->nodePool, 0x2CF, HEAP_ID_13);
    work->slotList = DressUpGfx_CreateSlotList(0x2BC, HEAP_ID_13);
    DressUp_InitCanvas(work, args->mon, 10, FALSE);
    DressUp_InitTray(work);
    DressUp_LoadFrameBg(work, 0);
    DressUp_InitButtons(work);
    DressUp_InitHud(work, args->options);
    DressUpDrag_Init(&work->touch, &work->canvas, &work->tray, &work->gfx, &work->hud, TRUE);
    work->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_13);
    work->msgWindow = AllocWindows(HEAP_ID_13, 1);
    work->uiState = DRESS_UP_UI_ACCESSORIES;
    Sound_SetSceneAndPlayBGM(0x35, 0, 0);
    return TRUE;
}

BOOL DressUp_Main(OverlayManager *man, int *state) {
    DressUpWork *work = OverlayManager_GetData(man);
    BOOL done = FALSE;
    DressUpArgs *args = OverlayManager_GetArgs(man);

    switch (*state) {
    case 0:
    case 1:
        BeginNormalPaletteFade(1, 5, 5, RGB_BLACK, 6, 1, HEAP_ID_13);
        *state = 2;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case 3:
        // tutorial
        if (args->showTutorial == 1) {
            TextFlags_SetCanTouchSpeedUpPrint(TRUE);
            work->printerId = DressUpHud_PrintMsg(&work->hud, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00047);
            (*state)++;
        } else {
            *state = 6;
        }
        break;
    case 4:
        if (!TextPrinterCheckActive(work->printerId)) {
            DressUpHud_FreeMsgString(&work->hud);
            (*state)++;
        }
        break;
    case 5:
        if (((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) | gSystem.touchNew) != 0) {
            DressUpHud_PrintMsgInstant(&work->hud, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00048);
            TextFlags_SetCanTouchSpeedUpPrint(FALSE);
            (*state)++;
        }
        break;
    case 6:
        // editing
        if (work->uiState == DRESS_UP_UI_DONE) {
            DressUp_StartFinishTransition(work, &work->transitionDone);
            *state = 7;
        }
        DressUpTouch_Update(&work->touch);
        DressUp_UpdateCountMeter(work);
        DressUpHud_UpdateCountdown(&work->hud);
        DressUp_UpdateButtons(work);
        break;
    case 7:
        if (work->transitionDone) {
            work->transitionDone = FALSE;
            *state = 8;
            work->uiState = DRESS_UP_UI_DRESSED_UP;
            DressUp_StartBackdropAnim(&work->backdropAnim, &work->canvas, HEAP_ID_14);
        }
        break;
    case 8:
        if (work->uiState == DRESS_UP_UI_DRESSED_NO) {
            DressUp_StartReturnTransition(work, &work->transitionDone);
            *state = 10;
        } else if (work->uiState == DRESS_UP_UI_DRESSED_YES) {
            *state = 9;
            work->uiState = DRESS_UP_UI_SAVE;
            DressUp_ShowSavePrompt(work);
        } else {
            work->uiState = DressUp_HandleDressedUpPrompt(work);
            DressUp_UpdateBackdropAnim(&work->backdropAnim);
        }
        break;
    case 9:
        if (work->uiState == DRESS_UP_UI_SAVE_YES) {
            work->saveRequested = TRUE;
            *state = 11;
        } else if (work->uiState == DRESS_UP_UI_SAVE_NO) {
            work->saveRequested = FALSE;
            *state = 11;
        } else {
            work->uiState = DressUp_HandleSavePrompt(work);
            DressUp_UpdateBackdropAnim(&work->backdropAnim);
        }
        break;
    case 10:
        if (work->transitionDone) {
            work->transitionDone = FALSE;
            *state = 6;
            work->uiState = DRESS_UP_UI_ACCESSORIES;
            DressUp_StopBackdropAnim(&work->backdropAnim);
        }
        break;
    case 11:
        BeginNormalPaletteFade(1, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_13);
        (*state)++;
        break;
    case 12:
        if (IsPaletteFadeFinished()) {
            *state = 0;
            work->uiState = DRESS_UP_UI_EXIT;
            done = TRUE;
            DressUp_StopBackdropAnim(&work->backdropAnim);
        }
        break;
    }
    DressUp_DrawFrame(work);
    return done;
}

BOOL DressUp_Exit(OverlayManager *man, int *state) {
    DressUpWork *work = OverlayManager_GetData(man);
    DressUpArgs *args = OverlayManager_GetArgs(man);

    if (work->saveRequested == TRUE) {
        GameStats_AddScore(args->gameStats, SCORE_EVENT_POKEMON_DRESSED);
        DressUp_SaveDesign(args->fashionEntry, &work->canvas, args->profile);
    }
    if (args->savedResult != NULL) {
        if (work->saveRequested == TRUE) {
            *args->savedResult = TRUE;
        } else {
            *args->savedResult = FALSE;
        }
    }
    if (args->menuInputStatePtr != NULL) {
        MenuInputStateMgr_SetState(args->menuInputStatePtr, work->menuInputState);
    }
    YesNoPrompt_Destroy(work->yesNoPrompt);
    WindowArray_Delete(work->msgWindow, 1);
    DressUp_FreeHud(work);
    DressUpDrag_Destroy(&work->touch);
    DressUp_FreeButtons(work);
    DressUp_FreeFrameBg(work);
    DressUp_FreeCanvas(work);
    DressUp_FreeTray(work);
    DressUpGfx_FreeSlotList(work->slotList);
    work->slotList = NULL;
    DressUpNodePool_Free(&work->nodePool);
    DressUp_FreeGraphics(work);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (GF_TouchpadStopSampling() != TRUE) {
        GF_AssertFail();
    }
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_13);
    Heap_Destroy(HEAP_ID_14);
    return TRUE;
}

void DressUp_InitGraphics(DressUpWork *work) {
    DressUpGfxResources resources;

    DressUpGfx_InitDisplay();
    DressUp_InitGfxCore(&work->gfx);
    DressUp_LoadRawResTables(&work->gfx, &resources);
    DressUpGfx_UploadRawResTables(&work->gfx, &resources);
    DressUpGfx_DestroyRawResTables(&resources);
}

void DressUp_DrawFrame(DressUpWork *work) {
    DressUp_DrawFrame3D(&work->gfx);
}

void DressUp_FreeGraphics(DressUpWork *work) {
    DressUpGfx_FreeAllTexAndPltt(&work->gfx);
    DressUp_FreeGfxCore(&work->gfx);
    DressUpGfx_ResetDisplay();
}

void DressUp_InitCanvas(DressUpWork *work, Pokemon *mon, int limit, BOOL altMonPic) {
    DressUpCanvasTemplate template;
    PokepicTemplate pokepicTemplate;

    template.graphicsManager = work->gfx.graphicsManager;
    template.images = work->gfx.images;
    template.palettes = work->gfx.palettes;
    template.paletteSlots = work->gfx.monPicAttrs;
    template.pokepicManager = work->gfx.pokepicManager;
    {
        // bgConfig is loaded before limit is stored
        BgConfig *bgConfig = work->gfx.bgConfig;
        template.limit = limit;
        template.bgConfig = bgConfig;
    }
    template.slotList = work->slotList;
    template.nodePool = &work->nodePool;
    DressUpCanvas_Init(&work->canvas, &template);
    if (altMonPic == FALSE) {
        DressUpCanvas_CreateMonPic(&work->canvas, mon, &pokepicTemplate, HEAP_ID_14);
    } else {
        DressUpCanvas_CreateMonPicEx(&work->canvas, mon, &pokepicTemplate, HEAP_ID_14);
    }
    DressUp_LoadMonHitMap(&work->gfx, &pokepicTemplate);
    DressUpCanvas_RefreshPriorities(&work->canvas);
    DressUpCanvas_SetBackdrop(&work->canvas, 0, HEAP_ID_14);
}

void DressUp_FreeCanvas(DressUpWork *work) {
    DressUpCanvas_FreeBackdrop(&work->canvas);
    DressUpCanvas_ClearObjects(&work->canvas);
    DressUpCanvas_Destroy(&work->canvas);
}

#pragma force_active on

// fills the tray with every owned accessory (once per item owned) and every owned backdrop
void DressUp_InitTray(DressUpWork *work) {
    DressUpTrayTemplate template;
    int i, j, n;

    template.graphicsManager = work->gfx.graphicsManager;
    template.images = work->gfx.images;
    template.palettes = work->gfx.palettes;
    template.paletteSlots = work->gfx.monPicAttrs;
    template.bgConfig = work->gfx.bgConfig;
    template.caseTable = work->caseTable;
    template.slotList = work->slotList;
    template.nodePool = &work->nodePool;
    template.pageCounts[0] = 14;
    template.pageCounts[1] = 2;
    template.pageCounts[2] = 1;
    DressUpTray_Init(&work->tray, &template);
    DressUpTray_SetPageImmediate(&work->tray, 0, 0);
    for (i = 0; i < DRESS_UP_ACCESSORY_NUM; i++) {
        n = DressUp_GetAccessoryCount(work->caseTable, i);
        for (j = 0; j < n; j++) {
            DressUpTray_AddItem(&work->tray, 0, i);
        }
    }
    for (i = 0; i < DRESS_UP_BACKDROP_NUM; i++) {
        n = DressUp_GetBackdropByOrder(work->caseTable, i);
        if (n < DRESS_UP_BACKDROP_NUM) {
            DressUpTray_AddItem(&work->tray, 1, n);
        }
    }
    DressUpTray_RefreshPriorities(&work->tray);
}

#pragma force_active reset

void DressUp_FreeTray(DressUpWork *work) {
    DressUpTray_FreeTabBg(&work->tray);
    DressUpTray_ClearObjects(&work->tray);
    DressUpTray_Destroy(&work->tray);
}

#pragma force_active on

void DressUp_RestoreEditBgLayout(DressUpWork *work) {
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(3);
    BgSetPosTextAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, -16);
    DressUpTray_LoadTabBg(&work->tray, 0);
    DressUpTray_SlideInTabBg(&work->tray, 0, 2, 0, NULL);
}

#pragma force_active reset

void DressUp_VBlankCB(void *arg) {
    DressUp_VBlank(arg);
}

// frame BG: 0 while editing, 1 once DONE is pressed
void DressUp_LoadFrameBg(DressUpWork *work, int frame) {
    DressUpBgScreenTemplate template;

    template.bgConfig = work->gfx.bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = 2 * frame + 0x79;
    template.plttFileId = 0x85;
    template.screenFileId = 2 * frame + 0x7A;
    template.x = 0;
    template.y = 0;
    template.layer = GF_BG_LYR_MAIN_1;
    template.plttCount = 1;
    template.plttSlot = 2;
    template.charOffset = 0;
    template.heapID = HEAP_ID_14;
    DressUpBgScreen_LoadFromTemplate(&work->frameBg, &template);
}

void DressUp_FreeFrameBg(DressUpWork *work) {
    DressUpBgScreen_Unload(&work->frameBg);
}

void DressUp_InitButtons(DressUpWork *work) {
    DressUpButtonPanel_Create(&work->buttonPanel, &work->gfx, &work->menuInputState);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 0, DressUp_OnNextPageButton, work, 0);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 1, DressUp_OnPrevPageButton, work, 0);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 2, DressUp_OnAccessoryTabButton, work, 0);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 3, DressUp_OnBackdropTabButton, work, 0);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 4, DressUp_OnDoneButton, work, 0);
}

void DressUp_FreeButtons(DressUpWork *work) {
    DressUpButtonPanel_Destroy(&work->buttonPanel, &work->gfx);
}

void DressUp_UpdateButtons(DressUpWork *work) {
    DressUpButtonPanel_Update(&work->buttonPanel);
}

void DressUp_ResetButtons(DressUpWork *work) {
    DressUpButtonPanel_ResetButtons(&work->buttonPanel);
}

void DressUp_OnDoneButton(void *button, void *arg) {
    DressUpWork *work = arg;
    if (DressUpTray_IsIdle(&work->tray)) {
        work->uiState = DRESS_UP_UI_DONE;
    }
}

void DressUp_OnNextPageButton(void *button, void *arg) {
    DressUpWork *work = arg;
    int group = DressUpTray_GetGroup(&work->tray);
    DressUpTray_CyclePage(&work->tray, group, FALSE);
}

void DressUp_OnPrevPageButton(void *button, void *arg) {
    DressUpWork *work = arg;
    int group = DressUpTray_GetGroup(&work->tray);
    DressUpTray_CyclePage(&work->tray, group, TRUE);
}

void DressUp_OnAccessoryTabButton(void *button, void *arg) {
    DressUpWork *work = arg;
    int page;

    if (work->uiState != DRESS_UP_UI_ACCESSORIES) {
        page = DressUpTray_GetGroupPage(&work->tray, 0);
        DressUpTray_SetPageImmediate(&work->tray, 0, page);
        work->uiState = DRESS_UP_UI_ACCESSORIES;
    }
}

void DressUp_OnBackdropTabButton(void *button, void *arg) {
    DressUpWork *work = arg;
    int page;

    if (work->uiState != DRESS_UP_UI_BACKDROPS) {
        page = DressUpTray_GetGroupPage(&work->tray, 1);
        DressUpTray_SetPageImmediate(&work->tray, 1, page);
        work->uiState = DRESS_UP_UI_BACKDROPS;
    }
}

void DressUp_InitHud(DressUpWork *work, Options *options) {
    DressUpHudTemplate template = { 0 };

    template.bgConfig = work->gfx.bgConfig;
    template.spriteList = work->gfx.spriteList;
    template.resMans = &work->gfx.charResMan;
    template.options = options;
    template.countMax = 10;
    template.narc = work->gfx.narc;
    DressUpHud_Init(&work->hud, &template, 15);
}

void DressUp_FreeHud(DressUpWork *work) {
    DressUpHud_Free(&work->hud);
}

void DressUp_StartFinishTransition(DressUpWork *work, u32 *done) {
    DressUpTransition *env = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUp_FinishTransitionTask, sizeof(DressUpTransition), 10, HEAP_ID_13));
    env->work = work;
    env->done = done;
    env->counter = 0;
    env->state = 0;
}

// DONE: tray away, fade, slide the canvas into the middle, "Is your Pokemon all dressed up?"
void DressUp_FinishTransitionTask(SysTask *task, DressUpTransition *env) {
    switch (env->state) {
    case 0:
        if (!DressUpTray_AnimateToPage(&env->work->tray, 3, 0)) {
            GF_AssertFail();
        }
        env->state++;
        break;
    case 1:
        if (DressUpTray_IsIdle(&env->work->tray)) {
            env->state++;
        }
        break;
    case 2:
        StartBrightnessTransition(8, -16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 3:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 4:
        DressUpButtonPanel_MoveBy(&env->work->buttonPanel, 0, 8);
        env->counter++;
        if (env->counter >= 8) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 5:
        if (DressUp_SlideCanvasStep(env, -8, 5, 8)) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 6:
        DressUp_LoadFrameBg(env->work, 1);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        DressUp_ShowDressedUpPrompt(env->work);
        env->state++;
        break;
    case 7:
        StartBrightnessTransition(8, 0, -16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 8:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 9:
        *env->done = TRUE;
        DestroySysTaskAndEnvironment(task);
        break;
    }
}

void DressUp_StartReturnTransition(DressUpWork *work, u32 *done) {
    DressUpTransition *env = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUp_ReturnTransitionTask, sizeof(DressUpTransition), 10, HEAP_ID_13));
    env->work = work;
    env->done = done;
    env->counter = 0;
    env->state = 0;
}

// "no" to "all dressed up?": back to editing (reverse of the finish transition)
void DressUp_ReturnTransitionTask(SysTask *task, DressUpTransition *env) {
    switch (env->state) {
    case 0:
        StartBrightnessTransition(8, -16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 1:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 2:
        DressUp_HidePrompt(env->work);
        DressUp_RestoreEditBgLayout(env->work);
        DressUp_ResetButtons(env->work);
        DressUp_LoadFrameBg(env->work, 0);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, -40);
        env->state++;
        break;
    case 3:
        if (DressUp_SlideCanvasStep(env, 8, -5, 8)) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 4:
        DressUpButtonPanel_MoveBy(&env->work->buttonPanel, 0, -8);
        env->counter++;
        if (env->counter >= 8) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 5:
        StartBrightnessTransition(8, 0, -16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 6:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 7:
        if (!DressUpTray_AnimateToPage(&env->work->tray, 0, 0)) {
            GF_AssertFail();
        }
        env->state++;
        break;
    case 8:
        if (DressUpTray_IsIdle(&env->work->tray)) {
            env->state++;
        }
        break;
    case 9:
        *env->done = TRUE;
        DestroySysTaskAndEnvironment(task);
        break;
    }
}

// writes the mon, the trainer, the accessories (those on the mon first) and the backdrop to the fashion entry
void DressUp_SaveDesign(SaveFashionDataSub *entry, DressUpCanvas *canvas, PlayerProfile *profile) {
    DressUpNode *list;
    DressUpNode *node;
    int count;

    FashionEntry_Clear(entry);
    FashionEntry_SetMon(entry, canvas->monPic.mon, &canvas->monPic);
    if (profile != NULL) {
        String *name = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_13);
        FashionEntry_SetMonSecondText(entry, name, PlayerProfile_GetTrainerGender(profile));
        String_Delete(name);
    }
    count = 0;
    node = canvas->monList.next;
    list = &canvas->monList;
    for (; node != list; node = node->next) {
        if (node->mode == 0) {
            FashionEntry_PlaceItem(entry, node->object, count);
            count++;
        }
    }
    list = &canvas->looseList;
    for (node = canvas->looseList.next; node != list; node = node->next) {
        if (node->mode == 0) {
            FashionEntry_PlaceItem(entry, node->object, count);
            count++;
        }
    }
    FashionEntry_SetBackdrop(entry, (u8)canvas->backdropId);
    FashionEntry_MarkSaved(entry);
}

// moves the canvas objects and its two BGs by (dx, dy); TRUE after `frames` steps
BOOL DressUp_SlideCanvasStep(DressUpTransition *env, int dx, int dy, int frames) {
    if (env->counter < 8) {
        DressUpCanvas_MoveAll(&env->work->canvas, dx, dy);
    }
    if (env->counter >= 1) {
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SUB_X, dx);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SUB_X, dx);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SUB_Y, dy);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SUB_Y, dy);
    }
    env->counter++;
    if (env->counter > frames) {
        return TRUE;
    }
    return FALSE;
}

void DressUp_ShowDressedUpPrompt(DressUpWork *work) {
    YesNoPromptTemplate template;

    BgClearTilemapBufferAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3);
    BgSetPosTextAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = work->gfx.bgConfig;
    template.bgId = GF_BG_LYR_MAIN_3;
    template.tileStart = 0;
    template.plttSlot = 5;
    template.x = 25;
    template.y = 4;
    template.ignoreTouchFlag = work->menuInputState;
    YesNoPrompt_InitFromTemplate(work->yesNoPrompt, &template);
    DressUp_CreatePromptMsgWindow(work, msg_0215_00001);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(1);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(0);
}

void DressUp_HidePrompt(DressUpWork *work) {
    YesNoPrompt_Reset(work->yesNoPrompt);
    DressUp_RemovePromptMsgWindow(work);
    BgClearTilemapBufferAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3);
}

int DressUp_HandleDressedUpPrompt(DressUpWork *work) {
    int result = YesNoPrompt_HandleInput(work->yesNoPrompt);
    switch (result) {
    case YESNORESPONSE_WAIT:
        return DRESS_UP_UI_DRESSED_UP;
    case YESNORESPONSE_YES:
        result = DRESS_UP_UI_DRESSED_YES;
        break;
    case YESNORESPONSE_NO:
        result = DRESS_UP_UI_DRESSED_NO;
        break;
    }
    work->menuInputState = YesNoPrompt_IsInTouchMode(work->yesNoPrompt);
    return result;
}

void DressUp_CreateMsgWindow(DressUpWork *work, int msgId, u8 x, u8 y, u8 width, u8 height) {
    MsgData *msgData;
    String *string;
    u32 frame = Options_GetFrame(work->options);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_7_OFFSET, HEAP_ID_14);
    AddWindowParameterized(work->gfx.bgConfig, work->msgWindow, GF_BG_LYR_MAIN_3, x, y, width, height, 7, 0x5A);
    FillWindowPixelBuffer(work->msgWindow, 15);
    LoadUserFrameGfx2(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, 0x3C, 8, frame, HEAP_ID_14);
    DrawFrameAndWindow2(work->msgWindow, FALSE, 0x3C, 8);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0215_bin, HEAP_ID_13);
    string = NewString_ReadMsgData(msgData, msgId);
    AddTextPrinterParameterizedWithColor(work->msgWindow, 1, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(work->msgWindow);
}

void DressUp_PrintMsgWindow(DressUpWork *work, int msgId) {
    MsgData *msgData;
    String *raw;
    String *expanded;
    FillWindowPixelBuffer(work->msgWindow, 15);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0215_bin, HEAP_ID_13);
    raw = NewString_ReadMsgData(msgData, msgId);
    expanded = String_New(0x100, HEAP_ID_13);
    StringExpandPlaceholders(work->msgFormat, expanded, raw);
    AddTextPrinterParameterizedWithColor(work->msgWindow, 1, expanded, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    String_Delete(expanded);
    String_Delete(raw);
    DestroyMsgData(msgData);
    CopyWindowToVram(work->msgWindow);
}

void DressUp_RemoveMsgWindow(DressUpWork *work) {
    ClearWindowTilemapAndCopyToVram(work->msgWindow);
    RemoveWindow(work->msgWindow);
}

void DressUp_CreatePromptMsgWindow(DressUpWork *work, int msgId) {
    DressUp_CreateMsgWindow(work, msgId, 2, 1, 27, 2);
}

void DressUp_PrintPromptMsgWindow(DressUpWork *work, int msgId) {
    DressUp_PrintMsgWindow(work, msgId);
}

void DressUp_RemovePromptMsgWindow(DressUpWork *work) {
    DressUp_RemoveMsgWindow(work);
}

void DressUp_UpdateCountMeter(DressUpWork *work) {
    int count = DressUpCanvas_GetCount(&work->canvas);
    DressUpHud_SetAccessoryCount(&work->hud, count);
}
