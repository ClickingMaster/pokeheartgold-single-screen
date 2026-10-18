#include "global.h"

#include "constants/sndseq.h"

#include "application/mailbox_app_internal.h"

#include "bag.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "grid_input_handler.h"
#include "heap.h"
#include "item.h"
#include "mail.h"
#include "mail_misc.h"
#include "overlay_55.h"
#include "party.h"
#include "party_menu.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"

FS_EXTERN_OVERLAY(OVY_55);

#define MAILBOX_STATE_FINISHED 25

typedef int (*MailboxAppStateFunc)(MailboxApp *app);

typedef struct MailboxAppSubApp {
    MailboxAppStateFunc start;
    MailboxAppStateFunc finish;
    int nextState;
} MailboxAppSubApp;

void BgCellManager_GetCellPosition(BgCellManager *manager, int index, s8 *x, s8 *y);

static int MailboxApp_StartFadeIn(MailboxApp *app, int nextState);
static int MailboxApp_StartFadeOut(MailboxApp *app, int nextState);
static int MailboxApp_WaitForText(MailboxApp *app, u8 waitForInput, int nextState);
static int MailboxApp_AskYesNo(MailboxApp *app, u32 question);
static int MailboxApp_StartCellFlash(MailboxApp *app, int cell, int nextState);
static int MailboxApp_StartSpriteFlash(MailboxApp *app, int sprite, int nextState);
static void MailboxAppData_SetListDimmed(MailboxAppData *data, BOOL enable);
static void MailboxApp_ShowMail(MailboxApp *app);
static void MailboxApp_HideMail(MailboxApp *app);
static void MailboxApp_PutMailInBag(MailboxApp *app);
static void MailboxApp_DeleteSelectedMail(MailboxApp *app);
static void MailboxApp_CreatePartyMenuArgs(MailboxApp *app, u8 context, u16 itemId, u8 partySlot);
static int MailboxApp_State_Setup(MailboxApp *app);
static int MailboxApp_State_Teardown(MailboxApp *app);
static int MailboxApp_State_WaitFade(MailboxApp *app);
static int MailboxApp_State_WaitText(MailboxApp *app);
static int MailboxApp_State_YesNo(MailboxApp *app);
static int MailboxApp_State_PaletteFlash(MailboxApp *app);
static int MailboxApp_State_StartSubApp(MailboxApp *app);
static int MailboxApp_State_RunSubApp(MailboxApp *app);
static int MailboxApp_State_FadeIn(MailboxApp *app);
static int MailboxApp_State_SelectMail(MailboxApp *app);
static int MailboxApp_State_Quit(MailboxApp *app);
static int MailboxApp_State_ChangePage(MailboxApp *app);
static int MailboxApp_State_ShowMailPrompt(MailboxApp *app);
static int MailboxApp_State_OpenMailMenu(MailboxApp *app);
static int MailboxApp_State_MailMenu(MailboxApp *app);
static int MailboxApp_State_ReadMail(MailboxApp *app);
static int MailboxApp_State_CloseMail(MailboxApp *app);
static int MailboxApp_State_ConfirmErase(MailboxApp *app);
static int MailboxApp_State_PrintHoldMailPrompt(MailboxApp *app);
static int MailboxApp_State_ConfirmHoldMail(MailboxApp *app);
static int MailboxApp_State_SelectMonForMailDone(MailboxApp *app);
static int MailboxApp_State_FadeInDimmedList(MailboxApp *app);
static int MailboxApp_State_ShowMailToBagResult(MailboxApp *app);
static int MailboxApp_State_UseMailDone(MailboxApp *app);
static int MailboxApp_ReturnToMailList(MailboxApp *app);
static int MailboxApp_StartPartyMenu(MailboxApp *app);
static int MailboxApp_FinishGiveMail(MailboxApp *app);
static int MailboxApp_FinishSelectMonForMail(MailboxApp *app);
static int MailboxApp_FinishPartyMenu(MailboxApp *app);
static int MailboxApp_StartUseMail(MailboxApp *app);
static int MailboxApp_FinishUseMail(MailboxApp *app);
static int MailboxApp_EraseMail(MailboxApp *app);
static int MailboxApp_SelectMonToHoldMail(MailboxApp *app);
static int MailboxApp_PrintMailToBagResult(MailboxApp *app);

static const OverlayManagerTemplate sMailboxUseMailOverlayTemplate = {
    .init = UseMail_Init,
    .exec = UseMail_Main,
    .exit = UseMail_Exit,
    .ovy_id = FS_OVERLAY_ID(OVY_55),
};

// Yes / no handlers per question
static const MailboxAppStateFunc sMailboxYesNoHandlers[][2] = {
    { MailboxApp_EraseMail,           MailboxApp_ReturnToMailList     },
    { MailboxApp_SelectMonToHoldMail, MailboxApp_PrintMailToBagResult },
};

static const MailboxAppSubApp sMailboxSubApps[] = {
    { MailboxApp_StartPartyMenu, MailboxApp_FinishGiveMail,         MAILBOX_STATE_SETUP                    },
    { MailboxApp_StartPartyMenu, MailboxApp_FinishSelectMonForMail, MAILBOX_STATE_SELECT_MON_FOR_MAIL_DONE },
    { MailboxApp_StartUseMail,   MailboxApp_FinishUseMail,          MAILBOX_STATE_USE_MAIL_DONE            },
    { MailboxApp_StartPartyMenu, MailboxApp_FinishPartyMenu,        MAILBOX_STATE_SETUP                    },
};

static const MailboxAppStateFunc sMailboxStateFuncs[] = {
    [MAILBOX_STATE_SETUP] = MailboxApp_State_Setup,
    [MAILBOX_STATE_TEARDOWN] = MailboxApp_State_Teardown,
    [MAILBOX_STATE_WAIT_FADE] = MailboxApp_State_WaitFade,
    [MAILBOX_STATE_WAIT_TEXT] = MailboxApp_State_WaitText,
    [MAILBOX_STATE_YES_NO] = MailboxApp_State_YesNo,
    [MAILBOX_STATE_PALETTE_FLASH] = MailboxApp_State_PaletteFlash,
    [MAILBOX_STATE_START_SUB_APP] = MailboxApp_State_StartSubApp,
    [MAILBOX_STATE_RUN_SUB_APP] = MailboxApp_State_RunSubApp,
    [MAILBOX_STATE_FADE_IN] = MailboxApp_State_FadeIn,
    [MAILBOX_STATE_SELECT_MAIL] = MailboxApp_State_SelectMail,
    [MAILBOX_STATE_QUIT] = MailboxApp_State_Quit,
    [MAILBOX_STATE_CHANGE_PAGE] = MailboxApp_State_ChangePage,
    [MAILBOX_STATE_SHOW_MAIL_PROMPT] = MailboxApp_State_ShowMailPrompt,
    [MAILBOX_STATE_OPEN_MAIL_MENU] = MailboxApp_State_OpenMailMenu,
    [MAILBOX_STATE_MAIL_MENU] = MailboxApp_State_MailMenu,
    [MAILBOX_STATE_READ_MAIL] = MailboxApp_State_ReadMail,
    [MAILBOX_STATE_CLOSE_MAIL] = MailboxApp_State_CloseMail,
    [MAILBOX_STATE_CONFIRM_ERASE] = MailboxApp_State_ConfirmErase,
    [MAILBOX_STATE_PRINT_HOLD_MAIL_PROMPT] = MailboxApp_State_PrintHoldMailPrompt,
    [MAILBOX_STATE_CONFIRM_HOLD_MAIL] = MailboxApp_State_ConfirmHoldMail,
    [MAILBOX_STATE_SELECT_MON_FOR_MAIL_DONE] = MailboxApp_State_SelectMonForMailDone,
    [MAILBOX_STATE_FADE_IN_DIMMED_LIST] = MailboxApp_State_FadeInDimmedList,
    [MAILBOX_STATE_SHOW_MAIL_TO_BAG_RESULT] = MailboxApp_State_ShowMailToBagResult,
    [MAILBOX_STATE_USE_MAIL_DONE] = MailboxApp_State_UseMailDone,
    [MAILBOX_STATE_RETURN_TO_MAIL_LIST] = MailboxApp_ReturnToMailList,
};

BOOL MailboxApp_RunState(MailboxApp *app, int *state) {
    *state = sMailboxStateFuncs[*state](app);
    if (*state == MAILBOX_STATE_FINISHED) {
        return FALSE;
    }
    if (app->data != NULL) {
        BgCellManager_Update(app->data->cellManager);
        MailboxAppData_TickSprites(app->data);
        SpriteSystem_DrawSprites(app->data->spriteManager);
    }
    return TRUE;
}

static int MailboxApp_State_Setup(MailboxApp *app) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(GX_PLANEMASK_NONE);
    GfGfx_EngineBSetPlanes(GX_PLANEMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    Heap_Create(HEAP_ID_3, HEAP_ID_157, 0x60000);
    app->data = Heap_Alloc(HEAP_ID_157, sizeof(MailboxAppData));
    MI_CpuFill8(app->data, 0, sizeof(MailboxAppData));
    MailboxApp_LoadMail(app);
    MailboxApp_SetGfxBanks();
    MailboxApp_InitBgLayers(app);
    MailboxApp_LoadBgGraphics(app);
    MailboxApp_LoadFrameAndFontGfx(app);
    MailboxApp_AddWindows(app);
    MailboxApp_CreateNameplateCells(app);
    MailboxApp_CreateSprites(app);
    MailboxApp_CreateYesNoPrompt(app);
    MailboxApp_CreateGridInput(app);
    MailboxAppData_CreateStrings(app->data);
    MailboxApp_PrintTitleAndButtons(app);
    MailboxApp_PrintPageNumber(app);
    MailboxApp_DrawPage(app);
    MailboxApp_CreateMailMenuItems(app);
    Main_SetVBlankIntrCB(MailboxApp_VBlankCB, app);
    return app->nextState;
}

static int MailboxApp_State_Teardown(MailboxApp *app) {
    Main_SetVBlankIntrCB(NULL, NULL);
    MailboxApp_DeleteMailMenuItems(app);
    MailboxAppData_DeleteStrings(app->data);
    MailboxApp_FreeGridInput(app);
    MailboxApp_DestroyYesNoPrompt(app);
    MailboxApp_DeleteSprites(app);
    MailboxApp_DeleteNameplateCells(app);
    MailboxApp_RemoveWindows(app);
    MailboxApp_FreeBgLayers(app);
    MailboxApp_FreeMail(app);
    GF_TouchpadStopSampling();
    G2_BlendNone();
    G2S_BlendNone();
    GfGfx_EngineASetPlanes(GX_PLANEMASK_NONE);
    GfGfx_EngineBSetPlanes(GX_PLANEMASK_NONE);
    Heap_Free(app->data);
    app->data = NULL;
    Heap_Destroy(HEAP_ID_157);
    return app->nextState;
}

static int MailboxApp_State_WaitFade(MailboxApp *app) {
    if (IsPaletteFadeFinished() == TRUE) {
        return app->data->stateAfterFade;
    }
    return MAILBOX_STATE_WAIT_FADE;
}

static int MailboxApp_State_WaitText(MailboxApp *app) {
    if (TextPrinterCheckActive(app->data->printerId) == FALSE) {
        if (app->data->waitForInput == FALSE) {
            return app->nextState;
        }
        if (System_GetTouchNew() == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            return app->nextState;
        }
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            PlaySE(SEQ_SE_DP_SELECT);
            return app->nextState;
        }
    }
    return MAILBOX_STATE_WAIT_TEXT;
}

static int MailboxApp_State_YesNo(MailboxApp *app) {
    switch (YesNoPrompt_HandleInput(app->data->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        YesNoPrompt_Reset(app->data->yesNoPrompt);
        return sMailboxYesNoHandlers[app->data->yesNoQuestion][0](app);
    case YESNORESPONSE_NO:
        YesNoPrompt_Reset(app->data->yesNoPrompt);
        return sMailboxYesNoHandlers[app->data->yesNoQuestion][1](app);
    default:
        return MAILBOX_STATE_YES_NO;
    }
}

static int MailboxApp_State_PaletteFlash(MailboxApp *app) {
    if (MailboxApp_RunPaletteFlash(app) == FALSE) {
        return app->nextState;
    }
    return MAILBOX_STATE_PALETTE_FLASH;
}

static int MailboxApp_State_StartSubApp(MailboxApp *app) {
    sMailboxSubApps[app->subAppId].start(app);
    return MAILBOX_STATE_RUN_SUB_APP;
}

static int MailboxApp_State_RunSubApp(MailboxApp *app) {
    if (OverlayManager_Run(app->subApp) == FALSE) {
        return MAILBOX_STATE_RUN_SUB_APP;
    }
    OverlayManager_Delete(app->subApp);
    app->nextState = sMailboxSubApps[app->subAppId].finish(app);
    return sMailboxSubApps[app->subAppId].nextState;
}

static int MailboxApp_State_FadeIn(MailboxApp *app) {
    return MailboxApp_StartFadeIn(app, MAILBOX_STATE_SELECT_MAIL);
}

static int MailboxApp_State_SelectMail(MailboxApp *app) {
    u32 input;

    switch (MailboxApp_GetTouchedPageArrow(app)) {
    case 0:
        if (app->page != 0) {
            app->page--;
            PlaySE(SEQ_SE_DP_SELECT);
            return MailboxApp_StartSpriteFlash(app, 0, MAILBOX_STATE_CHANGE_PAGE);
        } else {
            return MAILBOX_STATE_SELECT_MAIL;
        }
    case 1:
        if (app->page != app->data->lastPage) {
            app->page++;
            PlaySE(SEQ_SE_DP_SELECT);
            return MailboxApp_StartSpriteFlash(app, 1, MAILBOX_STATE_CHANGE_PAGE);
        } else {
            return MAILBOX_STATE_SELECT_MAIL;
        }
    }
    input = GridInputHandler_HandleInput_NoHold(app->data->gridInput);
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if ((app->data->mailOrder + input)[app->page * MAILBOX_MAIL_PER_PAGE] != MAILBOX_NO_MAIL) {
            app->cursorPos = input;
            app->selectedMail = (app->data->mailOrder + input)[app->page * MAILBOX_MAIL_PER_PAGE];
            PlaySE(SEQ_SE_DP_DECIDE);
            return MailboxApp_StartCellFlash(app, input, MAILBOX_STATE_SHOW_MAIL_PROMPT);
        }
        break;
    case 10:
    case -2:
        PlaySE(SEQ_SE_DP_DECIDE);
        return MailboxApp_StartSpriteFlash(app, 3, MAILBOX_STATE_QUIT);
    case -1:
        input = GridInputHandler_GetNextInput(app->data->gridInput);
        if ((gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) && (input == 1 || input == 3 || input == 5 || input == 7 || input == 9) && app->page != app->data->lastPage) {
            app->page++;
            PlaySE(SEQ_SE_DP_SELECT);
            return MailboxApp_StartSpriteFlash(app, 1, MAILBOX_STATE_CHANGE_PAGE);
        }
        if ((gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) && (input == 0 || input == 2 || input == 4 || input == 6 || input == 8) && app->page != 0) {
            app->page--;
            PlaySE(SEQ_SE_DP_SELECT);
            return MailboxApp_StartSpriteFlash(app, 0, MAILBOX_STATE_CHANGE_PAGE);
        }
        break;
    case -3:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case -4:
        break;
    }
    return MAILBOX_STATE_SELECT_MAIL;
}

static int MailboxApp_State_Quit(MailboxApp *app) {
    app->nextState = MAILBOX_STATE_FINISHED;
    return MailboxApp_StartFadeOut(app, MAILBOX_STATE_TEARDOWN);
}

static int MailboxApp_State_ChangePage(MailboxApp *app) {
    if (app->page == 0) {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, FALSE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, TRUE);
    } else {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, TRUE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, FALSE);
    }
    MailboxApp_PrintPageNumber(app);
    MailboxApp_DrawPage(app);
    return MAILBOX_STATE_SELECT_MAIL;
}

static int MailboxApp_State_ShowMailPrompt(MailboxApp *app) {
    MailboxAppData_SetSpriteDrawFlag(app->data, 2, FALSE);
    MailboxAppData_SetListDimmed(app->data, TRUE);
    MailboxApp_DrawSelectedNameplate(app);
    MailboxApp_DrawMessageBox(app);
    MailboxApp_PrintMailPrompt(app);
    return MailboxApp_WaitForText(app, FALSE, MAILBOX_STATE_OPEN_MAIL_MENU);
}

static int MailboxApp_State_OpenMailMenu(MailboxApp *app) {
    MailboxApp_OpenMailMenu(app);
    return MAILBOX_STATE_MAIL_MENU;
}

static int MailboxApp_State_MailMenu(MailboxApp *app) {
    u32 input = TouchscreenListMenu_HandleInput(app->data->listMenu);
    u16 itemId;

    switch (input) {
    case 0: // READ
        MailboxApp_CloseMailMenu(app);
        MailboxApp_ClearMessageBox(app);
        MailboxAppData_SetSpriteDrawFlag(app->data, 2, TRUE);
        MailboxAppData_ShowCloseButton(app->data);
        MailboxAppData_MoveCursorToCloseButton(app->data);
        MailboxApp_ShowMail(app);
        return MAILBOX_STATE_READ_MAIL;
    case 1: // ERASE
        MailboxApp_CloseMailMenu(app);
        MailboxApp_PrintMessage(app, 0);
        return MailboxApp_WaitForText(app, FALSE, MAILBOX_STATE_CONFIRM_ERASE);
    case 2: // GIVE
        MailboxApp_CloseMailMenu(app);
        itemId = MailToItemId(Mail_GetType(app->data->mail[app->selectedMail]));
        MailboxApp_CreatePartyMenuArgs(app, 14, itemId, 0);
        app->subAppId = 0;
        app->nextState = MAILBOX_STATE_START_SUB_APP;
        return MailboxApp_StartFadeOut(app, MAILBOX_STATE_TEARDOWN);
    case 3: // CANCEL
    case -2:
        MailboxApp_CloseMailMenu(app);
        MailboxApp_ClearMessageBox(app);
        BgCellManager_EraseCell(app->data->cellManager, MAILBOX_CELL_SELECTED);
        MailboxAppData_SetListDimmed(app->data, FALSE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 2, TRUE);
        return MAILBOX_STATE_SELECT_MAIL;
    case -1:
    default:
        return MAILBOX_STATE_MAIL_MENU;
    }
}

static int MailboxApp_State_ReadMail(MailboxApp *app) {
    if (MailboxApp_IsCloseMailInput() == TRUE) {
        PlaySE(SEQ_SE_DP_DECIDE);
        return MailboxApp_StartSpriteFlash(app, 3, MAILBOX_STATE_CLOSE_MAIL);
    }
    return MAILBOX_STATE_READ_MAIL;
}

static int MailboxApp_State_CloseMail(MailboxApp *app) {
    MailboxApp_HideMail(app);
    MailboxApp_MoveCursor(app, GridInputHandler_GetNextInput(app->data->gridInput));
    MailboxAppData_ShowCancelButton(app->data);
    BgCellManager_EraseCell(app->data->cellManager, MAILBOX_CELL_SELECTED);
    MailboxAppData_SetListDimmed(app->data, FALSE);
    return MAILBOX_STATE_SELECT_MAIL;
}

static int MailboxApp_State_ConfirmErase(MailboxApp *app) {
    return MailboxApp_AskYesNo(app, 0);
}

static int MailboxApp_State_PrintHoldMailPrompt(MailboxApp *app) {
    MailboxApp_PrintMessage(app, 2);
    return MailboxApp_WaitForText(app, FALSE, MAILBOX_STATE_CONFIRM_HOLD_MAIL);
}

static int MailboxApp_State_ConfirmHoldMail(MailboxApp *app) {
    return MailboxApp_AskYesNo(app, 1);
}

static int MailboxApp_State_SelectMonForMailDone(MailboxApp *app) {
    if (app->partyMenuAction != 6) {
        app->nextState = MAILBOX_STATE_FADE_IN_DIMMED_LIST;
        return MAILBOX_STATE_SETUP;
    }
    app->subAppId = 2;
    return MAILBOX_STATE_START_SUB_APP;
}

static int MailboxApp_State_FadeInDimmedList(MailboxApp *app) {
    MailboxAppData_SetSpriteDrawFlag(app->data, 2, FALSE);
    MailboxAppData_SetListDimmed(app->data, TRUE);
    return MailboxApp_StartFadeIn(app, MAILBOX_STATE_SHOW_MAIL_TO_BAG_RESULT);
}

static int MailboxApp_State_ShowMailToBagResult(MailboxApp *app) {
    MailboxApp_DrawMessageBox(app);
    return MailboxApp_PrintMailToBagResult(app);
}

static int MailboxApp_State_UseMailDone(MailboxApp *app) {
    if (app->useMailResult == 0) {
        app->nextState = MAILBOX_STATE_FADE_IN_DIMMED_LIST;
        return MAILBOX_STATE_SETUP;
    }
    MailboxApp_CreatePartyMenuArgs(app, 11, app->itemId, app->partySlot);
    app->subAppId = 3;
    return MAILBOX_STATE_START_SUB_APP;
}

static int MailboxApp_ReturnToMailList(MailboxApp *app) {
    MailboxApp_ClearMessageBox(app);
    BgCellManager_EraseCell(app->data->cellManager, MAILBOX_CELL_SELECTED);
    MailboxApp_MoveCursor(app, GridInputHandler_GetNextInput(app->data->gridInput));
    MailboxAppData_SetSpriteDrawFlag(app->data, 2, TRUE);
    MailboxAppData_SetListDimmed(app->data, FALSE);
    return MAILBOX_STATE_SELECT_MAIL;
}

static int MailboxApp_StartFadeIn(MailboxApp *app, int nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_157);
    app->data->stateAfterFade = nextState;
    return MAILBOX_STATE_WAIT_FADE;
}

static int MailboxApp_StartFadeOut(MailboxApp *app, int nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_157);
    app->data->stateAfterFade = nextState;
    return MAILBOX_STATE_WAIT_FADE;
}

static int MailboxApp_WaitForText(MailboxApp *app, u8 waitForInput, int nextState) {
    app->data->waitForInput = waitForInput;
    app->nextState = nextState;
    return MAILBOX_STATE_WAIT_TEXT;
}

static int MailboxApp_AskYesNo(MailboxApp *app, u32 question) {
    app->data->yesNoQuestion = question;
    MailboxApp_ShowYesNoPrompt(app);
    return MAILBOX_STATE_YES_NO;
}

static int MailboxApp_StartCellFlash(MailboxApp *app, int cell, int nextState) {
    u16 width;
    u16 height;
    s8 pos[2];
    MailboxAppPaletteFlash *flash;

    BgCellManager_GetCellPosition(app->data->cellManager, cell, &pos[1], &pos[0]);
    BgCellManager_GetCellSize(app->data->cellManager, cell, &width, &height);
    flash = &app->data->flash;
    flash->isBg = TRUE;
    flash->target = GF_BG_LYR_MAIN_2;
    flash->firstPalette = 2;
    flash->secondPalette = 1;
    flash->state = 0;
    flash->timer = 0;
    flash->x = pos[1];
    flash->y = pos[0];
    flash->width = (u8)width;
    flash->height = (u8)height;
    app->nextState = nextState;
    return MAILBOX_STATE_PALETTE_FLASH;
}

static int MailboxApp_StartSpriteFlash(MailboxApp *app, int sprite, int nextState) {
    MailboxAppPaletteFlash *flash = &app->data->flash;

    flash->isBg = FALSE;
    flash->target = (u8)sprite;
    flash->firstPalette = 1;
    flash->secondPalette = 0;
    flash->state = 0;
    flash->timer = 0;
    app->nextState = nextState;
    return MAILBOX_STATE_PALETTE_FLASH;
}

static void MailboxAppData_SetListDimmed(MailboxAppData *data, BOOL enable) {
    if (enable == TRUE) {
        MailboxAppData_SetSpriteTranslucent(data, 0, TRUE);
        MailboxAppData_SetSpriteTranslucent(data, 1, TRUE);
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3, 4, 12);
    } else {
        MailboxAppData_SetSpriteTranslucent(data, 0, FALSE);
        MailboxAppData_SetSpriteTranslucent(data, 1, FALSE);
        G2_BlendNone();
    }
}

static void MailboxApp_ShowMail(MailboxApp *app) {
    MailboxApp_LoadStationeryGfx(app);
    MailboxApp_PrintMail(app);
    MailboxApp_ShowMailMonIcons(app);
}

static void MailboxApp_HideMail(MailboxApp *app) {
    MailboxAppData_SetSpriteDrawFlag(app->data, 4, FALSE);
    MailboxAppData_SetSpriteDrawFlag(app->data, 5, FALSE);
    MailboxAppData_SetSpriteDrawFlag(app->data, 6, FALSE);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_1);
}

static void MailboxApp_PutMailInBag(MailboxApp *app) {
    Bag *bag = Save_Bag_Get(app->args->saveData);

    app->itemId = MailToItemId(Mail_GetType(app->data->mail[app->selectedMail]));
    if (Bag_AddItem(bag, app->itemId, 1, HEAP_ID_157) == TRUE) {
        app->bagFull = FALSE;
    } else {
        app->bagFull = TRUE;
    }
}

static void MailboxApp_DeleteSelectedMail(MailboxApp *app) {
    Mailbox_DeleteSlotI(app->mailbox->msgs, 0, app->selectedMail);
}

static void MailboxApp_CreatePartyMenuArgs(MailboxApp *app, u8 context, u16 itemId, u8 partySlot) {
    PartyMenuArgs *args = Heap_Alloc(HEAP_ID_156, sizeof(PartyMenuArgs));

    MI_CpuFill8(args, 0, sizeof(PartyMenuArgs));
    args->party = SaveArray_Party_Get(app->args->saveData);
    args->bag = Save_Bag_Get(app->args->saveData);
    args->options = app->options;
    args->mailbox = app->mailbox;
    args->panelLayout = 0;
    args->context = context;
    args->itemId = itemId;
    args->fieldSystem = app->args->fieldSystem;
    args->menuInputStatePtr = app->args->menuInputStatePtr;
    args->partySlot = partySlot;
    app->subAppArgs = args;
}

static int MailboxApp_StartPartyMenu(MailboxApp *app) {
    app->subApp = OverlayManager_New(&gOverlayTemplate_PartyMenu, app->subAppArgs, HEAP_ID_156);
    return 0;
}

static int MailboxApp_FinishGiveMail(MailboxApp *app) {
    PartyMenuArgs *args = app->subAppArgs;
    u32 partySlot = args->partySlot;

    if (partySlot <= 5) {
        Mailbox_MoveMessageToMon(app->mailbox->msgs, app->selectedMail, Party_GetMonByIndex(SaveArray_Party_Get(app->args->saveData), partySlot), HEAP_ID_156);
    }
    Heap_Free(app->subAppArgs);
    return MAILBOX_STATE_FADE_IN;
}

static int MailboxApp_FinishSelectMonForMail(MailboxApp *app) {
    PartyMenuArgs *args = app->subAppArgs;

    app->partySlot = args->partySlot;
    app->partyMenuAction = args->selectedAction;
    Heap_Free(app->subAppArgs);
    return MAILBOX_STATE_SETUP;
}

static int MailboxApp_FinishPartyMenu(MailboxApp *app) {
    Heap_Free(app->subAppArgs);
    return MAILBOX_STATE_FADE_IN;
}

static int MailboxApp_StartUseMail(MailboxApp *app) {
    UseMailArgs *args = UseMailArgs_NewCompose(app->args->saveData, 2, app->partySlot, ItemToMailId(app->itemId), HEAP_ID_156);

    args->menuInputStatePtr = app->args->menuInputStatePtr;
    app->subAppArgs = args;
    app->subApp = OverlayManager_New(&sMailboxUseMailOverlayTemplate, app->subAppArgs, HEAP_ID_156);
    return 0;
}

static int MailboxApp_FinishUseMail(MailboxApp *app) {
    app->useMailResult = UseMailArgs_WasMailWritten(app->subAppArgs);
    if (app->useMailResult == 1) {
        Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(app->args->saveData), app->partySlot);
        UseMailArgs_GiveMailToMon(app->subAppArgs, mon);
        Bag_TakeItem(Save_Bag_Get(app->args->saveData), app->itemId, 1, HEAP_ID_156);
    }
    UseMailArgs_Free(app->subAppArgs);
    return MAILBOX_STATE_SETUP;
}

static int MailboxApp_EraseMail(MailboxApp *app) {
    MailboxApp_PutMailInBag(app);
    MailboxApp_DeleteSelectedMail(app);
    MailboxApp_FreeMail(app);
    MailboxApp_LoadMail(app);
    if (app->data->mailCount <= MAILBOX_MAIL_PER_PAGE) {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, FALSE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, FALSE);
    }
    if (app->page > app->data->lastPage) {
        app->page = app->data->lastPage;
        app->cursorPos = 0;
    }
    MailboxApp_PrintPageNumber(app);
    MailboxApp_DrawPage(app);
    BgCellManager_EraseCell(app->data->cellManager, MAILBOX_CELL_SELECTED);
    MailboxApp_PrintMessage(app, 1);
    return MailboxApp_WaitForText(app, TRUE, MAILBOX_STATE_PRINT_HOLD_MAIL_PROMPT);
}

static int MailboxApp_SelectMonToHoldMail(MailboxApp *app) {
    MailboxApp_CreatePartyMenuArgs(app, 9, app->itemId, 0);
    app->subAppId = 1;
    app->nextState = MAILBOX_STATE_START_SUB_APP;
    return MailboxApp_StartFadeOut(app, MAILBOX_STATE_TEARDOWN);
}

static int MailboxApp_PrintMailToBagResult(MailboxApp *app) {
    if (app->bagFull == FALSE) {
        MailboxApp_PrintMessage(app, 3);
    } else {
        MailboxApp_PrintMessage(app, 4);
    }
    MailboxApp_WaitForText(app, TRUE, MAILBOX_STATE_RETURN_TO_MAIL_LIST);
}
