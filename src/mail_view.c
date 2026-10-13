#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail_message.h"
#include "menu_input_state.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_55.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pokemon_icon_idx.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "oam_manager.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// args->mailMessageIdx is the cursor row (0-2: message lines, 3: button row); the byte after it in
// MailViewArgs (not declared in overlay_55.h) holds the selected button.
#define MAIL_VIEW_ARGS_BUTTON(args) (((u8 *)(args))[3])

#define MAIL_VIEW_ROW_BUTTONS 3

// args->unk18[] holds the icon of each mon attached to the mail: char file id and palette.
typedef union MailViewIcon {
    u16 raw;

    struct {
        u16 fileId : 12;
        u16 palette : 4;
    };
} MailViewIcon;

// The args seen through a byte offset into unk18[] (iconOffset = 2 * mon).
typedef struct MailViewShiftedArgs {
    u8 filler_00[offsetof(MailViewArgs, unk18)];
    MailViewIcon icon;
} MailViewShiftedArgs;

#define MAIL_VIEW_ICON(args, offset) (&((volatile MailViewShiftedArgs *)((u8 *)(args) + (offset)))->icon)

enum MailViewScreen {
    MAIL_VIEW_SCREEN_READ,
    MAIL_VIEW_SCREEN_WRITE,
    MAIL_VIEW_SCREEN_EMPTY_WARNING,
    MAIL_VIEW_SCREEN_CONFIRM_QUIT,
};

typedef struct MailView {
    enum HeapID heapID;                        // 0x00
    u32 state;                                 // 0x04
    u16 subState;                              // 0x08
    u8 inputMode;                              // 0x0A
    u8 frame;                                  // 0x0B
    u8 screen;                                 // 0x0C
    u8 returnScreen;                           // 0x0D
    u8 unk_0E;                                 // 0x0E
    u8 textPrinterId;                          // 0x0F
    u8 textSpeed;                              // 0x10
    u8 cursorRow;                              // 0x11
    u8 cursorButton;                           // 0x12
    u8 cursorBlinkPaused;                      // 0x13
    u8 cursorBlinkLevel;                       // 0x14
    u8 cursorBlinkFalling;                     // 0x15
    u8 cursorPos;                              // 0x16
    u8 prevCursorPos;                          // 0x17
    BgConfig *bgConfig;                        // 0x18
    MailViewArgs *args;             // 0x1C
    MsgData *msgData;                          // 0x20
    u8 filler_24[0xC];                         // 0x24
    PaletteData *palette;                      // 0x30
    int (*onSwitchToKeys)(struct MailView *);  // 0x34
    int (*onSwitchToTouch)(struct MailView *); // 0x38
    void *bgScreenFile;                        // 0x3C
    void *buttonScreenFile;                    // 0x40
    NNSG2dScreenData *bgScreen;                // 0x44
    NNSG2dScreenData *buttonScreen;            // 0x48
    Window windows[6];                         // 0x4C
    YesNoPrompt *yesNoPrompt;                  // 0xAC
    SpriteSystem *spriteSystem;                // 0xB0
    SpriteManager *spriteManager;              // 0xB4
    ManagedSprite *icons[3];                   // 0xB8
} MailView;                                    // size: 0xC4

extern const GraphicsModes _021E6DD4;
extern int (*const sMailViewScreenFuncs[])(MailView *);
extern const OamCharTransferParam sMailViewOamCharTransferParam;
extern const SpriteResourceCountsListUnion sMailViewSpriteResourceCounts;
extern const TouchscreenHitbox sMailViewHitboxes[];
extern const OamManagerParam sMailViewOamManagerParam;
extern const GraphicsBanks sMailViewGraphicsBanks;

typedef struct MailViewBgTemplates {
    BgTemplate bgs[5];
} MailViewBgTemplates;

extern const MailViewBgTemplates sMailViewBgTemplates;

BOOL MailView_Init(OverlayManager *man, int *state);
BOOL MailView_Main(OverlayManager *man, int *state);
BOOL MailView_Exit(OverlayManager *man, int *state);
BOOL MailView_AreAllMessagesEmpty(MailView *mailView);
void MailView_ResetCursorHighlight(MailView *mailView);
int MailView_OnSwitchToTouch(MailView *mailView);
int MailView_OnSwitchToKeys(MailView *mailView);
BOOL MailView_CheckInputModeSwitch(MailView *mailView);
BOOL MailView_HandleReadInput(MailView *mailView);
BOOL MailView_HandleWriteKeys(MailView *mailView);
BOOL MailView_HandleWriteTouch(MailView *mailView);
BOOL MailView_HandleWriteInput(MailView *mailView);
BOOL MailView_ShowEmptyMailWarning(MailView *mailView);
void MailView_InitYesNoPrompt(MailView *mailView);
int MailView_HandleYesNoPrompt(MailView *mailView);
BOOL MailView_ConfirmQuit(MailView *mailView);
BOOL MailView_Run(MailView *mailView);
void MailView_VBlankCB(MailView *mailView);
void MailView_CursorBlinkTask(SysTask *task, MailView *mailView);
BOOL MailView_SetupGraphics(MailView *mailView);
BOOL MailView_FreeGraphics(MailView *mailView);
void MailView_SetGfxBanks(void);
void MailView_InitBgs(MailView *mailView);
void MailView_FreeBgs(MailView *mailView);
void MailView_LoadBgGraphics(void *data);
void MailView_FreeBgGraphics(MailView *mailView);
void MailView_CreateWindows(MailView *mailView);
void MailView_DeleteWindows(MailView *mailView);
void MailView_PrintTexts(MailView *mailView);
void MailView_CreateMonIcons(MailView *mailView);
void MailView_DeleteMonIcons(MailView *mailView);

BOOL MailView_Init(OverlayManager *man, int *state) {
    MailView *mailView;
    u8 cursorPos;

    Heap_Create(HEAP_ID_3, HEAP_ID_41, 0x20000);
    mailView = OverlayManager_CreateAndGetData(man, sizeof(MailView), HEAP_ID_41);
    memset(mailView, 0, sizeof(MailView));
    mailView->heapID = HEAP_ID_41;
    mailView->args = OverlayManager_GetArgs(man);
    mailView->screen = mailView->args->unk0;
    mailView->returnScreen = mailView->screen;
    mailView->cursorRow = mailView->args->mailMessageIdx;
    mailView->cursorButton = MAIL_VIEW_ARGS_BUTTON(mailView->args);
    cursorPos = mailView->cursorRow;
    mailView->cursorPos = cursorPos;
    mailView->prevCursorPos = cursorPos;
    mailView->args->unk0 = 0xFFFF;
    mailView->textSpeed = Options_GetTextFrameDelay(mailView->args->options);
    mailView->frame = Options_GetFrame(mailView->args->options);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    mailView->inputMode = MenuInputStateMgr_GetState(mailView->args->menuInputStateMgr);
    return TRUE;
}

BOOL MailView_Main(OverlayManager *man, int *state) {
    return MailView_Run(OverlayManager_GetData(man)) != FALSE;
}

BOOL MailView_Exit(OverlayManager *man, int *state) {
    MailView *mailView = OverlayManager_GetData(man);
    enum HeapID heapID;

    TextFlags_SetCanABSpeedUpPrint(FALSE);
    heapID = mailView->heapID;
    MenuInputStateMgr_SetState(mailView->args->menuInputStateMgr, (MenuInputState)mailView->inputMode);
    OverlayManager_FreeData(man);
    Heap_Destroy(heapID);
    return TRUE;
}

BOOL MailView_AreAllMessagesEmpty(MailView *mailView) {
    int i;

    for (i = 0; i < 3; i++) {
        if (MailMsg_IsInit(&mailView->args->mailMessages[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

void MailView_ResetCursorHighlight(MailView *mailView) {
    PaletteData_BlendPalette(mailView->palette, PLTTBUF_MAIN_BG, (u16)(mailView->prevCursorPos + 0x22), 1, 0, RGB_WHITE);
    mailView->cursorBlinkFalling = 0;
    mailView->cursorBlinkLevel = 0;
}

int MailView_OnSwitchToTouch(MailView *mailView) {
    MailView_ResetCursorHighlight(mailView);
    return 0;
}

int MailView_OnSwitchToKeys(MailView *mailView) {
    return 0;
}

BOOL MailView_CheckInputModeSwitch(MailView *mailView) {
    int (*callback)(MailView *);

    if (mailView->inputMode == MENU_INPUT_STATE_TOUCH) {
        if (System_GetTouchHeld()) {
            return FALSE;
        }
        if (gSystem.heldKeys) {
            callback = mailView->onSwitchToKeys;
            if (callback != NULL) {
                callback(mailView);
            }
            mailView->inputMode = MENU_INPUT_STATE_BUTTONS;
            return TRUE;
        }
    } else {
        if (gSystem.heldKeys) {
            return FALSE;
        }
        if (System_GetTouchHeld()) {
            callback = mailView->onSwitchToTouch;
            if (callback != NULL) {
                callback(mailView);
            }
            mailView->inputMode = MENU_INPUT_STATE_TOUCH;
            return FALSE;
        }
    }
    return FALSE;
}

BOOL MailView_HandleReadInput(MailView *mailView) {
    if (gSystem.newKeys) {
        mailView->inputMode = MENU_INPUT_STATE_BUTTONS;
    } else if (System_GetTouchHeld()) {
        mailView->inputMode = MENU_INPUT_STATE_TOUCH;
    } else {
        return FALSE;
    }
    PlaySE(SEQ_SE_DP_PIRORIRO2);
    return TRUE;
}

BOOL MailView_HandleWriteKeys(MailView *mailView) {
    u32 keys = gSystem.newKeys;
    BOOL moved = FALSE;

    if (keys & PAD_BUTTON_A) {
        if (mailView->cursorRow == MAIL_VIEW_ROW_BUTTONS) {
            if (mailView->cursorButton == 0) {
                if (MailView_AreAllMessagesEmpty(mailView)) {
                    PlaySE(SEQ_SE_DP_DECIDE);
                    mailView->screen = MAIL_VIEW_SCREEN_EMPTY_WARNING;
                    return FALSE;
                }
                mailView->args->unk0 = 3;
                PlaySE(SEQ_SE_DP_PIRORIRO2);
                mailView->args->mailMessageIdx = 0;
                MAIL_VIEW_ARGS_BUTTON(mailView->args) = 0;
                return TRUE;
            }
            PlaySE(SEQ_SE_DP_DECIDE);
            mailView->screen = MAIL_VIEW_SCREEN_CONFIRM_QUIT;
            return FALSE;
        }
        mailView->args->mailMessageIdx = mailView->cursorRow;
        mailView->args->unk0 = mailView->args->mailMessageIdx;
        MAIL_VIEW_ARGS_BUTTON(mailView->args) = mailView->cursorButton;
        PlaySE(SEQ_SE_DP_DECIDE);
        return TRUE;
    }
    if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_DECIDE);
        mailView->screen = MAIL_VIEW_SCREEN_CONFIRM_QUIT;
        return FALSE;
    }
    if (keys & PAD_BUTTON_START) {
        mailView->cursorRow = MAIL_VIEW_ROW_BUTTONS;
        mailView->cursorButton = 0;
        moved = TRUE;
    } else if (keys & PAD_KEY_DOWN) {
        mailView->cursorRow = (mailView->cursorRow + 1) % 4;
        moved = TRUE;
    } else if (keys & PAD_KEY_UP) {
        mailView->cursorRow = (mailView->cursorRow + 3) % 4;
        moved = TRUE;
    } else if (keys & (PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
        if (mailView->cursorRow == MAIL_VIEW_ROW_BUTTONS) {
            mailView->cursorButton ^= 1;
            moved = TRUE;
        }
    } else {
        return FALSE;
    }
    if (!moved) {
        return FALSE;
    }
    PlaySE(SEQ_SE_DP_SELECT);
    if (mailView->cursorRow == MAIL_VIEW_ROW_BUTTONS) {
        mailView->cursorPos = mailView->cursorRow + mailView->cursorButton;
    } else {
        mailView->cursorPos = mailView->cursorRow;
    }
    return FALSE;
}

BOOL MailView_HandleWriteTouch(MailView *mailView) {
    int hit;
    u16 transparent;

    hit = TouchscreenHitbox_FindRectAtTouchNew(sMailViewHitboxes);
    if (hit == TOUCH_MENU_NO_INPUT) {
        return FALSE;
    }
    transparent = 1;
    if (DoesPixelAtScreenXYMatchPtrVal(mailView->bgConfig, GF_BG_LYR_MAIN_2, gSystem.touchX, gSystem.touchY, &transparent) == TRUE) {
        return FALSE;
    }
    if (hit == 3) {
        PlaySE(SEQ_SE_DP_DECIDE);
        mailView->screen = MAIL_VIEW_SCREEN_CONFIRM_QUIT;
        return FALSE;
    }
    if (hit < 3) {
        mailView->args->mailMessageIdx = hit;
        mailView->args->unk0 = mailView->args->mailMessageIdx;
        MAIL_VIEW_ARGS_BUTTON(mailView->args) = mailView->cursorButton;
        PlaySE(SEQ_SE_DP_DECIDE);
        return TRUE;
    }
    if (MailView_AreAllMessagesEmpty(mailView)) {
        PlaySE(SEQ_SE_DP_DECIDE);
        mailView->screen = MAIL_VIEW_SCREEN_EMPTY_WARNING;
        return FALSE;
    }
    mailView->args->unk0 = 3;
    PlaySE(SEQ_SE_DP_PIRORIRO2);
    mailView->args->mailMessageIdx = 0;
    MAIL_VIEW_ARGS_BUTTON(mailView->args) = 0;
    return TRUE;
}

BOOL MailView_HandleWriteInput(MailView *mailView) {
    if (MailView_CheckInputModeSwitch(mailView)) {
        return FALSE;
    }
    if (mailView->inputMode == MENU_INPUT_STATE_BUTTONS) {
        return MailView_HandleWriteKeys(mailView);
    }
    return MailView_HandleWriteTouch(mailView);
}

BOOL MailView_ShowEmptyMailWarning(MailView *mailView) {
    BOOL done = FALSE;
    String *string = NULL;

    switch (mailView->subState) {
    case 0:
        DrawFrameAndWindow2(&mailView->windows[5], TRUE, 10, 6);
        FillWindowPixelBuffer(&mailView->windows[5], 0xFF);
        string = String_New(76, mailView->heapID);
        ReadMsgDataIntoString(mailView->msgData, 2, string);
        AddTextPrinterParameterizedWithColor(&mailView->windows[5], 1, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 15), NULL);
        String_Delete(string);
        break;
    case 1:
        if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            done = TRUE;
            mailView->inputMode = MENU_INPUT_STATE_BUTTONS;
        } else if (System_GetTouchNew()) {
            done = TRUE;
            mailView->inputMode = MENU_INPUT_STATE_TOUCH;
            MailView_ResetCursorHighlight(mailView);
        }
        if (!done) {
            return FALSE;
        }
        ClearFrameAndWindow2(&mailView->windows[5], TRUE);
        ClearWindowTilemapAndCopyToVram(&mailView->windows[5]);
        mailView->subState = 0;
        mailView->screen = mailView->returnScreen;
        return FALSE;
    }
    mailView->subState++;
    return FALSE;
}

void MailView_InitYesNoPrompt(MailView *mailView) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = mailView->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = 0x1BB;
    template.plttSlot = 3;
    template.x = 24;
    template.y = 10;
    template.ignoreTouchFlag = mailView->inputMode;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplateWithPalette(mailView->yesNoPrompt, &template, mailView->palette);
}

int MailView_HandleYesNoPrompt(MailView *mailView) {
    int result;
    int inputMode;

    result = YesNoPrompt_HandleInput(mailView->yesNoPrompt);
    switch (result) {
    case YESNORESPONSE_YES:
        result = TRUE;
        break;
    case YESNORESPONSE_NO:
        result = FALSE;
        break;
    default:
        return -1;
    }
    inputMode = YesNoPrompt_IsInTouchMode(mailView->yesNoPrompt);
    if (inputMode != mailView->inputMode) {
        // (the callbacks are used the other way round than in MailView_CheckInputModeSwitch)
        if (mailView->inputMode == MENU_INPUT_STATE_BUTTONS) {
            mailView->onSwitchToKeys(mailView);
        } else {
            mailView->onSwitchToTouch(mailView);
        }
        mailView->inputMode = inputMode;
    }
    YesNoPrompt_Reset(mailView->yesNoPrompt);
    return result;
}

BOOL MailView_ConfirmQuit(MailView *mailView) {
    String *string;
    int answer;

    switch (mailView->subState) {
    case 0:
        mailView->cursorBlinkPaused = TRUE;
        DrawFrameAndWindow2(&mailView->windows[5], TRUE, 10, 6);
        FillWindowPixelBuffer(&mailView->windows[5], 0xFF);
        string = String_New(76, mailView->heapID);
        ReadMsgDataIntoString(mailView->msgData, 3, string);
        mailView->textPrinterId = AddTextPrinterParameterizedWithColor(&mailView->windows[5], 1, string, 0, 0, mailView->textSpeed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
        String_Delete(string);
        MailView_ResetCursorHighlight(mailView);
        mailView->prevCursorPos = mailView->cursorPos;
        break;
    case 1:
        if (TextPrinterCheckActive(mailView->textPrinterId)) {
            return FALSE;
        }
        MailView_InitYesNoPrompt(mailView);
        break;
    case 2:
        answer = MailView_HandleYesNoPrompt(mailView);
        if (answer < 0) {
            return FALSE;
        }
        ClearFrameAndWindow2(&mailView->windows[5], TRUE);
        ClearWindowTilemapAndCopyToVram(&mailView->windows[5]);
        mailView->subState = 0;
        if (answer != FALSE) {
            mailView->args->unk0 = 0xFFFF;
            return TRUE;
        }
        mailView->screen = mailView->returnScreen;
        mailView->cursorBlinkPaused = FALSE;
        return FALSE;
    }
    mailView->subState++;
    return FALSE;
}

BOOL MailView_Run(MailView *mailView) {
    switch (mailView->state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
        ResetVisibleHardwareWindows(PM_LCD_TOP);
        ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
        break;
    case 1:
        if (!MailView_SetupGraphics(mailView)) {
            return FALSE;
        }
        Main_SetVBlankIntrCB((GFIntrCB)MailView_VBlankCB, mailView);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        SetMasterBrightnessNeutral(PM_LCD_TOP);
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3, 28, 4);
        PaletteData_BeginPaletteFade(mailView->palette, PLTTBUF_MAIN_BG_F | PLTTBUF_MAIN_OBJ_F, 0xFFFF, -1, 16, 0, RGB_BLACK);
        break;
    case 2:
        if (mailView->spriteManager != NULL) {
            SpriteSystem_DrawSprites(mailView->spriteManager);
        }
        if (PaletteData_GetSelectedBuffersBitmask(mailView->palette) != 0) {
            return FALSE;
        }
        break;
    case 3:
        if (mailView->spriteManager != NULL) {
            SpriteSystem_DrawSprites(mailView->spriteManager);
        }
        if (!sMailViewScreenFuncs[mailView->screen](mailView)) {
            return FALSE;
        }
        PaletteData_BeginPaletteFade(mailView->palette, PLTTBUF_MAIN_BG_F | PLTTBUF_MAIN_OBJ_F, 0xFFFF, -1, 0, 16, RGB_BLACK);
        break;
    case 4:
        if (PaletteData_GetSelectedBuffersBitmask(mailView->palette) != 0) {
            if (mailView->spriteManager != NULL) {
                SpriteSystem_DrawSprites(mailView->spriteManager);
            }
            return FALSE;
        }
        SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
        Main_SetVBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        break;
    case 5:
        if (MailView_FreeGraphics(mailView)) {
            return TRUE;
        }
        return FALSE;
    }
    mailView->state++;
    return FALSE;
}

void MailView_VBlankCB(MailView *mailView) {
    if (mailView->palette != NULL) {
        PaletteData_PushTransparentBuffers(mailView->palette);
    }
    if (mailView->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(mailView->bgConfig);
    *(u32 *)((u8 *)OS_IRQTable + 0x3FF8) |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

void MailView_CursorBlinkTask(SysTask *task, MailView *mailView) {
    u8 level;

    if (mailView->palette == NULL) {
        SysTask_Destroy(task);
        return;
    }
    if (mailView->cursorBlinkPaused) {
        return;
    }
    if (mailView->inputMode == MENU_INPUT_STATE_TOUCH) {
        return;
    }
    if (mailView->prevCursorPos != mailView->cursorPos) {
        MailView_ResetCursorHighlight(mailView);
        mailView->prevCursorPos = mailView->cursorPos;
    }
    PaletteData_BlendPalette(mailView->palette, PLTTBUF_MAIN_BG, (u16)(mailView->cursorPos + 0x22), 1, mailView->cursorBlinkLevel, RGB_WHITE);
    level = mailView->cursorBlinkLevel;
    if (mailView->cursorBlinkFalling) {
        mailView->cursorBlinkLevel = level - 1;
        if (level == 1) {
            mailView->cursorBlinkFalling ^= 1;
            return;
        }
    } else {
        mailView->cursorBlinkLevel = level + 1;
        if (level == 12) {
            mailView->cursorBlinkFalling ^= 1;
        }
    }
}

BOOL MailView_SetupGraphics(MailView *mailView) {
    switch (mailView->subState) {
    case 0:
        MailView_InitBgs(mailView);
        break;
    case 1:
        MailView_LoadBgGraphics(mailView);
        break;
    case 2:
        MailView_CreateWindows(mailView);
        MailView_PrintTexts(mailView);
        break;
    case 3:
        MailView_CreateMonIcons(mailView);
        mailView->subState = 0;
        return TRUE;
    }
    mailView->subState++;
    return FALSE;
}

BOOL MailView_FreeGraphics(MailView *mailView) {
    MailView_DeleteMonIcons(mailView);
    if (mailView->filler_24 != NULL) {
        DestroyMsgData(mailView->msgData);
    }
    MailView_DeleteWindows(mailView);
    MailView_FreeBgGraphics(mailView);
    MailView_FreeBgs(mailView);
    return TRUE;
}

void MailView_SetGfxBanks(void) {
    GraphicsBanks banks = sMailViewGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void MailView_InitBgs(MailView *mailView) {
    GraphicsModes modes;
    MailViewBgTemplates templates;

    MailView_SetGfxBanks();
    mailView->bgConfig = BgConfig_Alloc(mailView->heapID);
    modes = _021E6DD4;
    SetBothScreensModesAndDisable(&modes);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    templates = sMailViewBgTemplates;
    InitBgFromTemplate(mailView->bgConfig, GF_BG_LYR_MAIN_0, &templates.bgs[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(mailView->bgConfig, GF_BG_LYR_MAIN_1, &templates.bgs[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(mailView->bgConfig, GF_BG_LYR_MAIN_2, &templates.bgs[2], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(mailView->bgConfig, GF_BG_LYR_MAIN_3, &templates.bgs[3], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(mailView->bgConfig, GF_BG_LYR_SUB_0, &templates.bgs[4], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(mailView->bgConfig, GF_BG_LYR_MAIN_0);
    BgClearTilemapBufferAndCommit(mailView->bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(mailView->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(mailView->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(mailView->bgConfig, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, mailView->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, mailView->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, mailView->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, mailView->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, mailView->heapID);
}

void MailView_FreeBgs(MailView *mailView) {
    FreeBgTilemapBuffer(mailView->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(mailView->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(mailView->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(mailView->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(mailView->bgConfig, GF_BG_LYR_MAIN_0);
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    Heap_Free(mailView->bgConfig);
}

#pragma force_active on
#pragma opt_dead_assignments off

void MailView_LoadBgGraphics(void *data) {
    MailView *mailView = data;
    NARC *narc;
    void *buffer;
    int mailType;
    NNSG2dCharacterData *charData;
    NNSG2dPaletteData *plttData;

    mailType = mailView->args->mailType;
    // The file ids are u32 like the NARC_* parameters (an int id spills to swapped stack slots).
    u32 screenFile = mailType;
    screenFile = mailType + 24;
    narc = NARC_New(NARC_a_0_7_9, mailView->heapID);
    LoadUserFrameGfx1(mailView->bgConfig, GF_BG_LYR_MAIN_0, 1, 4, 0, mailView->heapID);
    LoadUserFrameGfx2(mailView->bgConfig, GF_BG_LYR_MAIN_0, 10, 6, mailView->frame, mailView->heapID);

    u32 charFile = mailType;
    charFile = mailType + 12;
    buffer = Heap_AllocAtEnd(mailView->heapID, NARC_GetMemberSize(narc, charFile));
    NARC_ReadWholeMember(narc, charFile, buffer);
    NNS_G2dGetUnpackedCharacterData(buffer, &charData);
    BG_LoadCharTilesData(mailView->bgConfig, GF_BG_LYR_MAIN_1, charData->pRawData, charData->szByte, 0);
    BG_LoadCharTilesData(mailView->bgConfig, GF_BG_LYR_SUB_0, charData->pRawData, charData->szByte, 0);
    Heap_Free(buffer);

    buffer = Heap_AllocAtEnd(mailView->heapID, NARC_GetMemberSize(narc, mailType));
    NARC_ReadWholeMember(narc, mailType, buffer);
    NNS_G2dGetUnpackedPaletteData(buffer, &plttData);
    BG_LoadPlttData(GF_PAL_LOCATION_SUB_BG, plttData->pRawData, (u16)plttData->szByte, 0);
    mailView->palette = PaletteData_Init(mailView->heapID);
    PaletteData_AllocBuffers(mailView->palette, PLTTBUF_MAIN_BG, 0xE0, mailView->heapID);
    PaletteData_AllocBuffers(mailView->palette, PLTTBUF_MAIN_OBJ, 0x60, mailView->heapID);
    PaletteData_LoadPalette(mailView->palette, plttData->pRawData, PLTTBUF_MAIN_BG, 0, 0x60);
    if (mailView->screen == MAIL_VIEW_SCREEN_WRITE) {
        PaletteData_LoadPalette(mailView->palette, (u16 *)((u8 *)plttData->pRawData + 0x60), PLTTBUF_MAIN_BG, 16, 0x20);
    }
    PaletteData_LoadNarc(mailView->palette, NARC_poketool_icongra_poke_icon, 0, mailView->heapID, PLTTBUF_MAIN_OBJ, 0x60, 0);
    PaletteData_LoadNarc(mailView->palette, NARC_graphic_font, 7, mailView->heapID, PLTTBUF_MAIN_BG, 0x20, 0x30);
    PaletteData_LoadNarc(mailView->palette, NARC_graphic_font, 8, mailView->heapID, PLTTBUF_MAIN_BG, 0x20, 0x50);
    PaletteData_LoadNarc(mailView->palette, NARC_a_0_3_8, 25, mailView->heapID, PLTTBUF_MAIN_BG, 0x20, 0x40);
    PaletteData_LoadNarc(mailView->palette, NARC_a_0_3_8, mailView->frame + 26, mailView->heapID, PLTTBUF_MAIN_BG, 0x20, 0x60);
    PaletteData_BlendPalette(mailView->palette, PLTTBUF_MAIN_BG, 0, 0x70, 16, RGB_BLACK);
    PaletteData_BlendPalette(mailView->palette, PLTTBUF_MAIN_OBJ, 0, 0x30, 16, RGB_BLACK);
    PaletteData_SetAutoTransparent(mailView->palette, TRUE);
    PaletteData_PushTransparentBuffers(mailView->palette);
    Heap_Free(buffer);

    mailView->bgScreenFile = Heap_Alloc(mailView->heapID, NARC_GetMemberSize(narc, screenFile));
    NARC_ReadWholeMember(narc, screenFile, mailView->bgScreenFile);
    NNS_G2dGetUnpackedScreenData(mailView->bgScreenFile, &mailView->bgScreen);
    mailView->buttonScreenFile = Heap_Alloc(mailView->heapID, NARC_GetMemberSize(narc, 36));
    NARC_ReadWholeMember(narc, 36, mailView->buttonScreenFile);
    NNS_G2dGetUnpackedScreenData(mailView->buttonScreenFile, &mailView->buttonScreen);
    NARC_Delete(narc);

    FillBgTilemapRect(mailView->bgConfig, GF_BG_LYR_SUB_0, 0x2001, 0, 0, 32, 32, 17);
    CopyToBgTilemapRect(mailView->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 32, 24, mailView->bgScreen->rawData, 0, 0, (u8)(mailView->bgScreen->screenWidth >> 3), (u8)(mailView->bgScreen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(mailView->bgConfig, GF_BG_LYR_MAIN_3);
    ScheduleBgTilemapBufferTransfer(mailView->bgConfig, GF_BG_LYR_SUB_0);
    if (mailView->screen != MAIL_VIEW_SCREEN_READ) {
        CopyToBgTilemapRect(mailView->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 24, mailView->buttonScreen->rawData, 0, 0, (u8)(mailView->buttonScreen->screenWidth >> 3), (u8)(mailView->buttonScreen->screenHeight >> 3));
        ScheduleBgTilemapBufferTransfer(mailView->bgConfig, GF_BG_LYR_MAIN_2);
        mailView->cursorBlinkLevel = 0;
        mailView->cursorBlinkFalling = 0;
        SysTask_CreateOnMainQueue((SysTaskFunc)MailView_CursorBlinkTask, mailView, 0);
        mailView->onSwitchToKeys = MailView_OnSwitchToKeys;
        mailView->onSwitchToTouch = MailView_OnSwitchToTouch;
    }
}

#pragma opt_dead_assignments reset
#pragma force_active reset

void MailView_FreeBgGraphics(MailView *mailView) {
    Heap_Free(mailView->buttonScreenFile);
    Heap_Free(mailView->bgScreenFile);
    PaletteData_FreeBuffers(mailView->palette, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(mailView->palette, PLTTBUF_MAIN_BG);
    PaletteData_Free(mailView->palette);
    mailView->palette = NULL;
    G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, 0, 0, 31, 0);
}

void MailView_CreateWindows(MailView *mailView) {
    int i;
    Window *window;

    AddWindowParameterized(mailView->bgConfig, &mailView->windows[0], GF_BG_LYR_MAIN_1, 3, 3, 26, 4, 1, 0x397);
    AddWindowParameterized(mailView->bgConfig, &mailView->windows[1], GF_BG_LYR_MAIN_1, 3, 8, 26, 4, 1, 0x32F);
    AddWindowParameterized(mailView->bgConfig, &mailView->windows[2], GF_BG_LYR_MAIN_1, 3, 13, 26, 4, 1, 0x2C7);
    AddWindowParameterized(mailView->bgConfig, &mailView->windows[3], GF_BG_LYR_MAIN_1, 3, 20, 8, 2, 1, 0x2B7);
    AddWindowParameterized(mailView->bgConfig, &mailView->windows[4], GF_BG_LYR_MAIN_1, 21, 20, 8, 2, 1, 0x2A7);
    AddWindowParameterized(mailView->bgConfig, &mailView->windows[5], GF_BG_LYR_MAIN_0, 2, 19, 27, 4, 5, 0x23B);
    i = 0;
    window = &mailView->windows[0];
    do {
        FillWindowPixelBuffer(window, 0);
        if (i < 5) {
            CopyWindowToVram(window);
        }
        i++;
        window++;
    } while (i < 6);
    mailView->yesNoPrompt = YesNoPrompt_Create(mailView->heapID);
}

void MailView_DeleteWindows(MailView *mailView) {
    int i;
    Window *window;

    YesNoPrompt_Destroy(mailView->yesNoPrompt);
    i = 0;
    window = &mailView->windows[0];
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        i++;
        window++;
    } while (i < 6);
}

void MailView_PrintTexts(MailView *mailView) {
    int i;
    int msgOffset;
    Window *window;
    String *string;

    for (i = 0, msgOffset = 0, window = &mailView->windows[0]; i < 3; i++, msgOffset += sizeof(MailMessage), window++) {
        if (MailMsg_IsInit((MailMessage *)((u8 *)mailView->args->mailMessages + msgOffset))) {
            string = MailMsg_GetExpandedString((MailMessage *)((u8 *)mailView->args->mailMessages + msgOffset), mailView->heapID);
            AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
            String_Delete(string);
            CopyWindowToVram(window);
        }
    }
    if (mailView->screen == MAIL_VIEW_SCREEN_WRITE) {
        int x;

        mailView->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0233_bin, mailView->heapID);
        string = String_New(16, mailView->heapID);
        i = 0;
        window = &mailView->windows[0];
        do {
            String_SetEmpty(string);
            ReadMsgDataIntoString(mailView->msgData, i, string);
            x = 64 - FontID_String_GetWidth(1, string, 0);
            AddTextPrinterParameterizedWithColor(&window[i + 3], 1, string, x / 2, 2, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
            CopyWindowToVram(&window[i + 3]);
            i++;
        } while (i < 2);
        String_Delete(string);
    } else {
        AddTextPrinterParameterizedWithColor(&mailView->windows[4], 1, mailView->args->mailAuthorName, 0, 1, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        CopyWindowToVram(&mailView->windows[4]);
    }
}

void MailView_CreateMonIcons(MailView *mailView) {
    ManagedSpriteTemplate template;
    OamManagerParam oamParam;
    OamCharTransferParam charTransferParam;
    SpriteResourceCountsListUnion resourceCounts;
    int i;
    int iconOffset;
    int xOffset;
    MailView *shifted;

    WirelessStatusIcon_CreateIfConnected();
    if (mailView->screen == MAIL_VIEW_SCREEN_WRITE) {
        return;
    }

    GF_CreateVramTransferManager(32, mailView->heapID);
    mailView->spriteSystem = SpriteSystem_Alloc(mailView->heapID);
    mailView->spriteManager = SpriteManager_New(mailView->spriteSystem);
    oamParam = sMailViewOamManagerParam;
    charTransferParam = sMailViewOamCharTransferParam;
    resourceCounts = sMailViewSpriteResourceCounts;
    SpriteSystem_Init(mailView->spriteSystem, &oamParam, &charTransferParam, 32);
    SpriteSystem_InitSprites(mailView->spriteSystem, mailView->spriteManager, 3);
    SpriteSystem_InitManagerWithCapacities(mailView->spriteSystem, mailView->spriteManager, &resourceCounts);
    thunk_ClearMainOAM(mailView->heapID);

    SpriteSystem_LoadPlttResObj(mailView->spriteSystem, mailView->spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObj(mailView->spriteSystem, mailView->spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconStaticCellNaix(), FALSE, 0);
    SpriteSystem_LoadAnimResObj(mailView->spriteSystem, mailView->spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconStaticAnimNaix(), FALSE, 0);

    i = 0;
    iconOffset = i;
    xOffset = i;
    // The work pointer shifted by i slots: icons[0] through it is icons[i].
    shifted = mailView;
    for (; i < 3; i++, iconOffset += sizeof(u16), shifted = (MailView *)((u8 *)shifted + sizeof(ManagedSprite *)), xOffset += 40) {
        if (MAIL_VIEW_ICON(mailView->args, iconOffset)->raw == 0xFFFF) {
            return;
        }
        SpriteSystem_LoadCharResObjWithHardwareMappingType(mailView->spriteSystem, mailView->spriteManager, NARC_poketool_icongra_poke_icon, MAIL_VIEW_ICON(mailView->args, iconOffset)->fileId, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, i);
        MI_CpuFill8(&template, 0, sizeof(template));
        template.x = 128 - xOffset;
        template.y = 160;
        template.z = 0;
        template.animation = 0;
        template.bgPriority = 2;
        template.pal = MAIL_VIEW_ICON(mailView->args, iconOffset)->palette;
        template.vramTransfer = FALSE;
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = i;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = 0;
        template.resIdList[GF_GFX_RES_TYPE_CELL] = 0;
        template.resIdList[GF_GFX_RES_TYPE_ANIM] = 0;
        template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
        template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
        shifted->icons[0] = SpriteSystem_NewSprite(mailView->spriteSystem, mailView->spriteManager, &template);
        if (MAIL_VIEW_ICON(mailView->args, iconOffset)->fileId == 7) {
            ManagedSprite_SetDrawFlag(shifted->icons[0], FALSE);
        }
    }
}

void MailView_DeleteMonIcons(MailView *mailView) {
    int i;
    MailView *shifted;

    if (mailView->screen != MAIL_VIEW_SCREEN_READ) {
        return;
    }
    i = 0;
    shifted = mailView;
    do {
        ManagedSprite *icon = shifted->icons[0];
        if (icon != NULL) {
            Sprite_DeleteAndFreeResources(icon);
        }
        i++;
        shifted = (MailView *)((u8 *)shifted + sizeof(ManagedSprite *));
    } while (i < 3);
    SpriteSystem_FreeResourcesAndManager(mailView->spriteSystem, mailView->spriteManager);
    SpriteSystem_Free(mailView->spriteSystem);
    GF_DestroyVramTransferManager();
}
