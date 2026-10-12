#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0441.h"

#include "frontier/battle_hall_data.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "save_special_ribbons.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "oam_manager.h"
#include "unk_0200FA24.h"
#include "unk_02035900.h"
#include "link_synchronization.h"
#include "wireless_status_icon.h"
#include "unk_02088288.h"
#include "unk_02096910.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

FS_EXTERN_OVERLAY(OVY_80);

#define TYPE_GRID_COLUMNS 4
#define TYPE_GRID_ROWS    5
#define TYPE_CELL_EMPTY   0xFE
#define TYPE_CELL_MATRON  19 // the "???" cell, used for the Hall Matron battle
#define NO_LINK_CHOICE    0xFF
#define NO_SAVED_CURSOR   117
#define MAX_TYPE_RANK     10

// Main loop states
#define HALL_STATE_FADE_IN      0
#define HALL_STATE_SUMMARY      1
#define HALL_STATE_SELECT       2
#define HALL_STATE_LINK_DECIDE  3
#define HALL_STATE_LINK_SYNC    4
#define HALL_STATE_FADE_OUT     5
#define HALL_STATE_CLOSE_PROMPT 6

typedef struct BattleHallSprite {
    u8 unk_00[0xC];
    const u8 *anim;
    Sprite *sprite;
} BattleHallSprite;

typedef struct BattleHallSpriteGfx {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObj[2][4];
} BattleHallSpriteGfx;

typedef struct BattleHallApp {
    OverlayManager *manager;         // 0x000
    OverlayManager *summaryApp;      // 0x004
    u8 subState;                     // 0x008
    u8 mode;                         // 0x009
    u8 printerId;                    // 0x00A
    u8 openSummary;                  // 0x00B
    u8 prevCursor;                   // 0x00C
    u8 cursor;                       // 0x00D
    u8 unk_00E;                      // 0x00E
    u8 yesNoActive;                  // 0x00F
    u16 summaryPending;              // 0x010
    u16 linkReady;                   // 0x012
    s16 brightness;                  // 0x014
    u8 recvCount;                    // 0x016
    u8 linkDecided;                  // 0x017
    u8 linkChoice;                   // 0x018
    u8 reselect;                     // 0x019
    u8 linkCursor;                   // 0x01A
    u8 timer;                        // 0x01B
    u16 battleNo;                    // 0x01C
    u8 matronMode;                   // 0x01E
    u8 savedCursor;                  // 0x01F
    MsgData *msgData;                // 0x020
    MessageFormat *msgFormat;        // 0x024
    String *expandedString;          // 0x028
    String *rawString;               // 0x02C
    String *strings[2];              // 0x030
    u8 unk_038[0x10];                // 0x038
    BgConfig *bgConfig;              // 0x048
    Window windows[4];               // 0x04C
    YesNoPrompt *yesNoPrompt;        // 0x08C
    u8 usedTouch;                    // 0x090
    u8 unk_091[3];                   // 0x091
    PaletteData *paletteData;        // 0x094
    MessagePrinter *messagePrinter;  // 0x098
    Options *options;                // 0x09C
    SaveData *saveData;              // 0x0A0
    PokemonSummaryArgs *summaryArgs; // 0x0A4
    BattleHallSpriteGfx spriteGfx;   // 0x0A8
    BattleHallSprite *cursorSprite;  // 0x204
    BattleHallSprite *monIcon;       // 0x208
    u32 summaryResult;               // 0x20C
    u16 *resultOut;                  // 0x210
    Party *party;                    // 0x214
    void *hallRecords;               // 0x218
    u16 *unk_21C;                    // 0x21C
    NARC *narc;                      // 0x220
    u16 linkCommand;                 // 0x224
    u16 linkArg;                     // 0x226
    u16 linkChoiceSent;              // 0x228
    u16 linkLevel;                   // 0x22A
    u8 unk_22C[0x50];                // 0x22C
    u8 partnerCursor;                // 0x27C
    u8 partnerAnswer;                // 0x27D
    u8 unk_27E[6];                   // 0x27E
} BattleHallApp;

extern const u8 _0223FE20[];
extern const GraphicsModes ov82_0223FE28;
extern const BgTemplate ov82_0223FE38;
extern const BgTemplate ov82_0223FE54;
extern const BgTemplate ov82_0223FE70;
extern const BgTemplate ov82_0223FE8C;
extern const BgTemplate ov82_0223FEA8;
extern const GraphicsBanks ov82_0223FEC4;
extern const u8 ov82_0223FEEC[4];
extern const ObjCharTransferTemplate ov82_0223FEF0;
extern const WindowTemplate ov82_0223FF00[4];

// Frontier overlay (80) helpers
u8 BattleHall_GetCellType(u8 index);
u32 BattleHall_IsMultiType(u8 mode);
void Frontier_BufferPartnerName(void *a0, u32 a1);

// Not declared by the owners' headers yet
u32 HallTypeRanks_Get(u8 type, void *hallRecords);
void GX_LoadBGPltt(const void *src, u32 offset, u32 size);

// text.h (and render_text.h, which includes it) are not included: text.h declares fontId as u8
// (FontID), but this module passes it unnarrowed
#define MAKE_TEXT_COLOR(fg, sh, bg) ((((fg) & 0xFF) << 16) | (((sh) & 0xFF) << 8) | (((bg) & 0xFF) << 0))
u8 AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
u8 TextPrinterCheckActive(u8 printerId);
void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);

BOOL BattleHallApp_Init(OverlayManager *man, int *state);
BOOL BattleHallApp_Main(OverlayManager *man, int *state);
BOOL BattleHallApp_Exit(OverlayManager *man);
BOOL BattleHallApp_FadeIn(BattleHallApp *app);
void BattleHallApp_PrintTypeGrid(BattleHallApp *app);
void BattleHallApp_PrintTitle(BattleHallApp *app);
void BattleHallApp_DrawFrame(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u16 tile, u16 palette);
BOOL BattleHallApp_RunSummary(BattleHallApp *app);
BOOL BattleHallApp_SelectType(BattleHallApp *app);
BOOL BattleHallApp_LinkDecide(BattleHallApp *app);
BOOL BattleHallApp_LinkSync(BattleHallApp *app);
BOOL BattleHallApp_FadeOut(BattleHallApp *app);
BOOL BattleHallApp_ClosePrompt(BattleHallApp *app);
void BattleHallApp_FreeGraphics(BattleHallApp *app);
void BattleHallApp_ResumeAfterSummary(BattleHallApp *app);
void BattleHallApp_DisableDisplay(void);
void BattleHallApp_InitGraphics(BattleHallApp *app);
void BattleHallApp_InitBgGraphics(BattleHallApp *app);
void BattleHallApp_InitSprites(BattleHallApp *app);
void BattleHallApp_FreeBgLayers(BgConfig *bgConfig);
void BattleHallApp_VBlankCallback(BattleHallApp *app);
void BattleHallApp_SetGraphicsBanks(void);
void BattleHallApp_InitBgLayers(BgConfig *bgConfig);
void BattleHallApp_InitLinkWindow(BattleHallApp *app);
void BattleHallApp_LoadTypeGridGraphics(BattleHallApp *app, u8 bgId);
void BattleHallApp_LoadBgPalette(void);
void BattleHallApp_LoadSubBgGraphics(BattleHallApp *app, u32 bgId);
void BattleHallApp_LoadTitleFrameGraphics(BattleHallApp *app, u32 bgId);
u8 BattleHallApp_PrintMessage(BattleHallApp *app, Window *window, u32 msgId, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId);
u8 BattleHallApp_ShowMessage(BattleHallApp *app, u32 msgId, u8 fontId);
void BattleHallApp_BufferNumber(BattleHallApp *app, u32 field, u32 value);
void BattleHallApp_PrintMonName(BattleHallApp *app, Window *window, int x, int y, u8 fg, u8 shadow, u8 bg, u8 fontId);
void BattleHallApp_PrintTypeNames(BattleHallApp *app, Window *window, int fg, int shadow, u8 bg, u8 fontId);
void BattleHallApp_PrintNumber(BattleHallApp *app, Window *window, u32 value, u32 x, u32 y);
void BattleHallApp_PrintTypeRanks(BattleHallApp *app, Window *window);
void BattleHallApp_PrintSelectPrompt(BattleHallApp *app, Window *window, u32 x, u32 y, u8 fg, u8 shadow, u8 bg, u8 fontId);
void BattleHallApp_CreateSummaryArgs(BattleHallApp *app);
void BattleHallApp_SetMainState(BattleHallApp *app, int *state, int nextState);
void BattleHallApp_HandleDpad(BattleHallApp *app, int unused);
BOOL BattleHallApp_HandleTouch(BattleHallApp *app);
BOOL BattleHallApp_HandleConfirm(BattleHallApp *app);
u16 BattleHallApp_GetCursorX(BattleHallApp *app);
u16 BattleHallApp_GetCursorY(BattleHallApp *app);
void BattleHallApp_GrayOutTypes(BattleHallApp *app, BgConfig *bgConfig);
void BattleHallApp_SetCellPalette(BgConfig *bgConfig, int index, int state);
u8 BattleHallApp_GetRecordIndex(u8 index);
BOOL BattleHallApp_IsMatronBattle(BattleHallApp *app);
u8 BattleHallApp_IsMatronMode(BattleHallApp *app);
BOOL BattleHallApp_SendLinkCommand(BattleHallApp *app, u16 command, u16 arg);
void BattleHallApp_PrepareLinkPing(BattleHallApp *app, u16 command);
void BattleHall_ReceivePing(u32 netId);
void BattleHallApp_PrepareLinkChoice(BattleHallApp *app, u16 command, u16 choice);
void BattleHall_ReceiveChoice(u32 netId, u32 unused, const u16 *msg, BattleHallApp *app);
void BattleHallApp_PrepareLinkAnswer(BattleHallApp *app, u16 command, u16 answer);
void BattleHall_ReceiveAnswer(u32 netId, u32 unused, const u16 *msg, BattleHallApp *app);
void BattleHallApp_ResetYesNo(BattleHallApp *app);
void BattleHallApp_ShowConfirmPrompt(BattleHallApp *app);
void BattleHallApp_CancelConfirm(BattleHallApp *app);
void BattleHallApp_ClearMessage(BattleHallApp *app);
void BattleHallApp_SetBrightness(s32 brightness);
void BattleHallSpriteGfx_Init(BattleHallSpriteGfx *gfx, Pokemon *mon);
Sprite *BattleHallSpriteGfx_CreateSprite(BattleHallSpriteGfx *gfx, int resourceId, int animSeq, int drawPriority, u8 screen);
void BattleHallSpriteGfx_Free(BattleHallSpriteGfx *gfx);
void BattleHallSpriteGfx_InitTransfers(void);
BattleHallSprite *BattleHallSprite_Create(BattleHallSpriteGfx *gfx, u32 resourceId, u32 animSeq, s32 x, u16 y, const u8 *anim);
u32 BattleHallSprite_Delete(BattleHallSprite *sprite);
void BattleHallSprite_SetVisible(BattleHallSprite *sprite, BOOL visible);
void BattleHallSprite_SetPosition(BattleHallSprite *sprite, s32 x, s32 y);
void BattleHallSprite_SetAnim(BattleHallSprite *sprite, u32 animSeq);
void BattleHallSprite_SetIconPalette(BattleHallSprite *sprite, Pokemon *mon);
void BattleHallApp_AddWindows(BgConfig *bgConfig, Window *windows);
void BattleHallApp_RemoveWindows(Window *windows);
void BattleHallApp_DrawMessageFrame(Window *window, u32 frame);
YesNoPrompt *BattleHallApp_CreateYesNo(enum HeapID heapId);
void BattleHallApp_DestroyYesNo(YesNoPrompt *prompt);
void BattleHallApp_InitYesNo(YesNoPrompt *prompt, BgConfig *bgConfig, u8 ignoreTouch);
u32 BattleHallApp_HandleYesNo(YesNoPrompt *prompt);

BOOL BattleHallApp_Init(OverlayManager *man, int *state) {
    BattleHallApp *app;
    BattleHallAppArgs *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    BattleHallApp_DisableDisplay();
    Heap_Create(HEAP_ID_3, HEAP_ID_105, 0x20000);
    app = OverlayManager_CreateAndGetData(man, sizeof(BattleHallApp), HEAP_ID_105);
    memset(app, 0, sizeof(BattleHallApp));
    app->bgConfig = BgConfig_Alloc(HEAP_ID_105);
    app->manager = man;
    args = OverlayManager_GetArgs(man);
    app->saveData = args->saveData;
    app->mode = args->mode;
    app->resultOut = &args->result;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->party = args->party;
    app->hallRecords = args->hallRecords;
    app->unk_21C = args->partnerLevel;
    app->battleNo = args->battleNo + 1;
    app->cursor = args->cursor;
    app->partnerCursor = NO_LINK_CHOICE;
    app->linkChoice = NO_LINK_CHOICE;
    app->savedCursor = NO_SAVED_CURSOR;
    BattleHallApp_InitGraphics(app);
    if (BattleHall_IsMultiType(app->mode) == TRUE) {
        sub_02096910(app);
    }
    *state = HALL_STATE_FADE_IN;
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    return TRUE;
}

BOOL BattleHallApp_Main(OverlayManager *man, int *state) {
    BattleHallApp *app = OverlayManager_GetData(man);

    if (app->linkChoice != NO_LINK_CHOICE && *state == HALL_STATE_SELECT && app->summaryPending == 0) {
        BattleHallApp_ResetYesNo(app);
        BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_DECIDE);
    }
    switch (*state) {
    case HALL_STATE_SUMMARY:
        if (BattleHallApp_RunSummary(app) == TRUE) {
            BattleHallApp_SetMainState(app, state, HALL_STATE_FADE_IN);
        } else {
            return FALSE;
        }
    case HALL_STATE_FADE_IN:
        if (BattleHallApp_FadeIn(app) == TRUE) {
            BattleHallApp_SetMainState(app, state, HALL_STATE_SELECT);
        }
        break;
    case HALL_STATE_SELECT:
        if (BattleHallApp_SelectType(app) == TRUE) {
            if (app->linkDecided == 1) {
                BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_DECIDE);
            } else if (app->openSummary == 1) {
                BattleHallApp_SetMainState(app, state, HALL_STATE_SUMMARY);
                return FALSE;
            } else if (BattleHall_IsMultiType(app->mode) == TRUE) {
                BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_SYNC);
            } else {
                BattleHallApp_SetMainState(app, state, HALL_STATE_FADE_OUT);
            }
        }
        break;
    case HALL_STATE_LINK_DECIDE:
        if (BattleHallApp_LinkDecide(app) == TRUE) {
            if (app->reselect == 1) {
                app->reselect = 0;
                BattleHallApp_SetMainState(app, state, HALL_STATE_SELECT);
            } else if (BattleHall_IsMultiType(app->mode) == TRUE) {
                BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_SYNC);
            } else {
                BattleHallApp_SetMainState(app, state, HALL_STATE_FADE_OUT);
            }
        }
        break;
    case HALL_STATE_LINK_SYNC:
        if (BattleHallApp_LinkSync(app) == TRUE) {
            BattleHallApp_SetMainState(app, state, HALL_STATE_FADE_OUT);
        }
        break;
    case HALL_STATE_FADE_OUT:
        if (BattleHallApp_FadeOut(app) == TRUE) {
            return TRUE;
        }
        break;
    case HALL_STATE_CLOSE_PROMPT:
        if (BattleHallApp_ClosePrompt(app) == TRUE) {
            BattleHallApp_SetMainState(app, state, HALL_STATE_LINK_SYNC);
        }
        break;
    }
    SpriteList_RenderAndAnimateSprites(app->spriteGfx.spriteList);
    return FALSE;
}

BOOL BattleHallApp_Exit(OverlayManager *man) {
    BattleHallApp *app = OverlayManager_GetData(man);

    *app->resultOut = app->cursor;
    BattleHallApp_FreeGraphics(app);
    OverlayManager_FreeData(man);
    reg_GX_POWCNT |= 0x8000;
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_105);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return TRUE;
}

BOOL BattleHallApp_FadeIn(BattleHallApp *app) {
    switch (app->subState) {
    case 0:
        if (app->linkReady == 0 && BattleHall_IsMultiType(app->mode) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(0x70);
        }
        app->subState++;
        break;
    case 1:
        if (app->linkReady == 0 && BattleHall_IsMultiType(app->mode) == TRUE) {
            if (LinkSync_IsSynced(0x70) == TRUE) {
                LinkSync_ClearPackets();
                app->linkReady = 1;
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 2:
        BattleHallApp_PrintTypeGrid(app);
        BattleHallApp_PrintTitle(app);
        BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 3, HEAP_ID_105);
        app->subState++;
        break;
    case 3:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void BattleHallApp_PrintTypeGrid(BattleHallApp *app) {
    BattleHallApp_PrintTypeNames(app, &app->windows[2], 1, 2, 0, 0);
    BattleHallApp_PrintTypeRanks(app, &app->windows[2]);
    BattleHallApp_PrintMonName(app, &app->windows[1], 0, 0, 1, 2, 0, 0);
}

void BattleHallApp_PrintTitle(BattleHallApp *app) {
    u8 x = GetWindowX(&app->windows[3]) - 1;
    u8 y = GetWindowY(&app->windows[3]) - 1;
    u8 width = GetWindowWidth(&app->windows[3]) + 2;
    u8 height = GetWindowHeight(&app->windows[3]) + 2;
    u8 bgId = GetWindowBgId(&app->windows[3]);

    BattleHallApp_PrintSelectPrompt(app, &app->windows[3], 0, 0, 1, 2, 15, 0);
    BattleHallApp_LoadTitleFrameGraphics(app, GF_BG_LYR_SUB_0);
    BattleHallApp_DrawFrame(app->bgConfig, bgId, x, y, width, height, 1, 1);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, bgId);
}

#pragma force_active on
// Frame tiles: corners at tile+0/2/6/8, edges at tile+1/3/5/7
void BattleHallApp_DrawFrame(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u16 tile, u16 palette) {
    FillBgTilemapRect(bgConfig, bgId, tile, x, y, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 2, x + width - 1, y, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 6, x, y + height - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 8, x + width - 1, y + height - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 1, x + 1, y, width - 2, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 7, x + 1, y + height - 1, width - 2, 1, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 3, x, y + 1, 1, height - 2, palette);
    FillBgTilemapRect(bgConfig, bgId, tile + 5, x + width - 1, y + 1, 1, height - 2, palette);
}
#pragma force_active reset

BOOL BattleHallApp_RunSummary(BattleHallApp *app) {
    if (app->subState == 0) {
        if (OverlayManager_Run(app->summaryApp) == TRUE) {
            app->summaryResult = app->summaryArgs->partySlot;
            Heap_Free(app->summaryArgs);
            Heap_Free(app->summaryApp);
            app->summaryApp = NULL;
            BattleHallApp_ResumeAfterSummary(app);
            app->summaryPending = 0;
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active on
BOOL BattleHallApp_SelectType(BattleHallApp *app) {
    switch (app->subState) {
    case 0:
        app->openSummary = 0;
        app->subState = 1;
        break;
    case 1:
        BattleHallApp_HandleDpad(app, gSystem.newKeys);
        if (!BattleHallApp_HandleConfirm(app) && !BattleHallApp_HandleTouch(app)) {
            break;
        }
        if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
            // The empty cell opens the Pokemon's summary
            PlaySE(SEQ_SE_DP_DECIDE);
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_105);
            app->summaryPending = 1;
            app->subState = 7;
            break;
        }
        if (BattleHallApp_IsMatronMode(app) == 1) {
            if (BattleHall_GetCellType(app->cursor) != TYPE_MYSTERY) {
                PlaySE(SEQ_SE_DP_BOX03);
                return FALSE;
            }
        } else {
            if (HallTypeRanks_Get(BattleHallApp_GetRecordIndex(app->cursor), app->hallRecords) >= MAX_TYPE_RANK) {
                PlaySE(SEQ_SE_DP_BOX03);
                return FALSE;
            }
            if (BattleHall_GetCellType(app->cursor) == TYPE_MYSTERY) {
                PlaySE(SEQ_SE_DP_BOX03);
                return FALSE;
            }
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        if (!BattleHall_IsMultiType(app->mode)) {
            BattleHallApp_SetCellPalette(app->bgConfig, app->cursor, 1);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
        }
        app->brightness = 0;
        app->subState = 2;
        break;
    case 2:
        app->brightness++;
        BattleHallApp_SetBrightness(-app->brightness);
        if (app->brightness == 8) {
            if (!BattleHall_IsMultiType(app->mode)) {
                BattleHallApp_ShowConfirmPrompt(app);
                app->subState = 3;
            } else {
                BattleHallApp_DrawMessageFrame(&app->windows[0], Options_GetFrame(app->options));
                app->printerId = BattleHallApp_ShowMessage(app, msg_0441_00000, 1);
                app->linkCursor = app->cursor;
                app->linkDecided = 1;
                return TRUE;
            }
        }
        break;
    case 3:
        switch (BattleHallApp_HandleYesNo(app->yesNoPrompt)) {
        case YESNORESPONSE_WAIT:
            break;
        case YESNORESPONSE_YES:
            BattleHallApp_ResetYesNo(app);
            if (BattleHallApp_IsMatronBattle(app) == 1 && BattleHallApp_IsMatronMode(app) == 0) {
                app->savedCursor = app->cursor;
                app->subState = 5;
                break;
            }
            StopSE(SEQ_SE_DP_SELECT, 0);
            PlaySE(SEQ_SE_DP_UG_020);
            if (app->savedCursor != NO_SAVED_CURSOR) {
                app->cursor = app->savedCursor;
            }
            return TRUE;
        case YESNORESPONSE_NO:
            BattleHallApp_ResetYesNo(app);
            app->subState = 4;
            break;
        }
        break;
    case 4:
        app->brightness--;
        BattleHallApp_SetBrightness(-app->brightness);
        if (app->brightness <= 0) {
            BattleHallApp_CancelConfirm(app);
            if (app->matronMode == 1) {
                app->subState = 0;
            } else if (app->matronMode == 0) {
                app->subState = 1;
            } else {
                GF_AssertFail();
            }
        }
        break;
    case 5:
        // The Hall Matron interrupts the selection
        BattleHallApp_DrawMessageFrame(&app->windows[0], Options_GetFrame(app->options));
        app->printerId = BattleHallApp_PrintMessage(app, &app->windows[0], msg_0441_00032, 1, 1, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(app->saveData)), 1, 2, 15, 1);
        CopyWindowToVram(&app->windows[0]);
        app->subState = 6;
        break;
    case 6:
        if (!TextPrinterCheckActive(app->printerId)) {
            int x, y;
            app->matronMode = 1;
            BattleHallApp_ClearMessage(app);
            BattleHallSprite_SetAnim(app->cursorSprite, 1);
            BattleHallApp_SetCellPalette(app->bgConfig, TYPE_CELL_MATRON, 0);
            BattleHallApp_GrayOutTypes(app, app->bgConfig);
            app->cursor = TYPE_CELL_MATRON;
            x = BattleHallApp_GetCursorX(app);
            y = BattleHallApp_GetCursorY(app);
            BattleHallSprite_SetPosition(app->cursorSprite, x, y);
            app->subState = 4;
        }
        break;
    case 7:
        if (IsPaletteFadeFinished() == TRUE) {
            BattleHallApp_CreateSummaryArgs(app);
            BattleHallApp_FreeGraphics(app);
            app->summaryApp = OverlayManager_New(&gOverlayTemplate_PokemonSummary, app->summaryArgs, HEAP_ID_105);
            app->openSummary = 1;
            return TRUE;
        }
        break;
    }
    return FALSE;
}
#pragma force_active reset

BOOL BattleHallApp_LinkDecide(BattleHallApp *app) {
    switch (app->subState) {
    case 0:
        if (BattleHallApp_SendLinkCommand(app, 5, BattleHallApp_GetRecordIndex(app->linkCursor)) == TRUE) {
            app->timer = 0;
            app->linkCursor = 0;
            app->linkDecided = 0;
            app->subState = 1;
        }
        break;
    case 1:
        if (app->timer != 0) {
            app->timer--;
        } else if (app->recvCount >= 2) {
            app->recvCount = 0;
            // The parent's choice wins unless the child chose first
            if (app->linkChoice < 20) {
                if (Comm_GetLocalNetId() == 0) {
                    app->subState = 2;
                } else {
                    app->subState = 3;
                }
            } else {
                if (Comm_GetLocalNetId() == 0) {
                    app->subState = 3;
                } else {
                    app->subState = 2;
                }
            }
        }
        break;
    case 3:
        BattleHallApp_SetBrightness(0);
        app->brightness = 0;
        BattleHallSprite_SetVisible(app->monIcon, FALSE);
        BattleHallApp_DrawMessageFrame(&app->windows[0], Options_GetFrame(app->options));
        Frontier_BufferPartnerName(app->msgFormat, 0);
        app->printerId = BattleHallApp_ShowMessage(app, msg_0441_00011, 1);
        app->timer = 10;
        BattleHallApp_SetCellPalette(app->bgConfig, app->partnerCursor, 2);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
        app->subState = 4;
        break;
    case 4:
        if (app->partnerAnswer != 0) {
            if (app->partnerAnswer == 1) {
                app->cursor = app->partnerCursor;
                return TRUE;
            }
            BattleHallApp_ClearMessage(app);
            BattleHallApp_SetCellPalette(app->bgConfig, app->partnerCursor, 0);
            ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
            app->partnerCursor = NO_LINK_CHOICE;
            app->partnerAnswer = 0;
            app->linkChoice = NO_LINK_CHOICE;
            app->reselect = 1;
            return TRUE;
        }
        break;
    case 2:
        BattleHallApp_SetCellPalette(app->bgConfig, app->cursor, 1);
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
        BattleHallApp_ShowConfirmPrompt(app);
        app->subState = 5;
        break;
    case 5:
        switch (BattleHallApp_HandleYesNo(app->yesNoPrompt)) {
        case YESNORESPONSE_WAIT:
            break;
        case YESNORESPONSE_YES:
            StopSE(SEQ_SE_DP_SELECT, 0);
            PlaySE(SEQ_SE_DP_UG_020);
            BattleHallApp_ResetYesNo(app);
            app->subState = 6;
            break;
        case YESNORESPONSE_NO:
            BattleHallApp_ResetYesNo(app);
            app->subState = 7;
            break;
        }
        break;
    case 6:
        if (BattleHallApp_SendLinkCommand(app, 6, 1) == TRUE) {
            return TRUE;
        }
        break;
    case 7:
        if (app->brightness > 0) {
            app->brightness--;
            BattleHallApp_SetBrightness(-app->brightness);
        } else if (BattleHallApp_SendLinkCommand(app, 6, 2) == TRUE) {
            BattleHallApp_CancelConfirm(app);
            app->partnerCursor = NO_LINK_CHOICE;
            app->linkChoice = NO_LINK_CHOICE;
            app->reselect = 1;
            return TRUE;
        }
        break;
    case 8:
        LinkSync_ClearPackets();
        LinkSync_StartSync(0x68);
        app->subState = 9;
        break;
    case 9:
        if (LinkSync_IsSynced(0x68) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_Init(HEAP_ID_105);
            app->linkChoice = NO_LINK_CHOICE;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL BattleHallApp_LinkSync(BattleHallApp *app) {
    switch (app->subState) {
    case 0:
        LinkSync_ClearPackets();
        LinkSync_StartSync(0x68);
        app->subState++;
        break;
    case 1:
        if (LinkSync_IsSynced(0x68) == 1) {
            LinkSync_ClearPackets();
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL BattleHallApp_FadeOut(BattleHallApp *app) {
    switch (app->subState) {
    case 0:
        app->timer = 10;
        app->subState++;
        break;
    case 1:
        app->timer--;
        if (app->timer == 0) {
            BattleHallApp_ClearMessage(app);
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_105);
            app->subState++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL BattleHallApp_ClosePrompt(BattleHallApp *app) {
    switch (app->subState) {
    case 0:
        BattleHallApp_ResetYesNo(app);
        BattleHallSprite_SetVisible(app->monIcon, TRUE);
        ClearFrameAndWindow2(&app->windows[0], FALSE);
        app->subState++;
        break;
    case 1:
        return TRUE;
    }
    return FALSE;
}

void BattleHallApp_FreeGraphics(BattleHallApp *app) {
    int i;

    if (app->cursorSprite != NULL) {
        BattleHallSprite_Delete(app->cursorSprite);
    }
    if (app->monIcon != NULL) {
        BattleHallSprite_Delete(app->monIcon);
    }
    BattleHallApp_DestroyYesNo(app->yesNoPrompt);
    WirelessStatusIcon_Destroy();
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->paletteData);
    app->paletteData = NULL;
    BattleHallSpriteGfx_Free(&app->spriteGfx);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
    String_Delete(app->expandedString);
    String_Delete(app->rawString);
    MessagePrinter_Delete(app->messagePrinter);
    i = 0;
    do {
        String_Delete(app->strings[i]);
        i++;
    } while (i < 2);
    BattleHallApp_RemoveWindows(app->windows);
    BattleHallApp_FreeBgLayers(app->bgConfig);
    NARC_Delete(app->narc);
}

void BattleHallApp_ResumeAfterSummary(BattleHallApp *app) {
    BattleHallApp_DisableDisplay();
    app->bgConfig = BgConfig_Alloc(HEAP_ID_105);
    BattleHallApp_InitGraphics(app);
    if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
        BattleHallSprite_SetAnim(app->cursorSprite, 2);
        BattleHallSprite_SetPosition(app->cursorSprite, 128, 168);
    }
}

void BattleHallApp_DisableDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

#pragma force_active on
void BattleHallApp_InitGraphics(BattleHallApp *app) {
    int i, x;
    u16 y;

    app->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_105);
    BattleHallApp_InitBgGraphics(app);
    BattleHallApp_InitSprites(app);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0441_bin, HEAP_ID_105);
    app->msgFormat = MessageFormat_New(HEAP_ID_105);
    app->expandedString = String_New(600, HEAP_ID_105);
    app->rawString = String_New(600, HEAP_ID_105);
    for (i = 0; i < 2; i++) {
        app->strings[i] = String_New(32, HEAP_ID_105);
    }
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_105);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_105);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_105);
    app->messagePrinter = MessagePrinter_New(15, 14, 0, HEAP_ID_105);
    BattleHallApp_AddWindows(app->bgConfig, app->windows);
    BattleHallApp_GrayOutTypes(app, app->bgConfig);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    GfGfx_BothDispOn();
    x = BattleHallApp_GetCursorX(app);
    y = BattleHallApp_GetCursorY(app);
    app->cursorSprite = BattleHallSprite_Create(&app->spriteGfx, 0, 1, x, y, NULL);
    app->monIcon = BattleHallSprite_Create(&app->spriteGfx, 1, 1, 160, 160, NULL);
    BattleHallSprite_SetIconPalette(app->monIcon, Party_GetMonByIndex(app->party, 0));
    if (sub_02037474()) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        WirelessStatusIcon_Create();
    }
    app->yesNoPrompt = BattleHallApp_CreateYesNo(HEAP_ID_105);
    Main_SetVBlankIntrCB((GFIntrCB)BattleHallApp_VBlankCallback, app);
}

void BattleHallApp_InitBgGraphics(BattleHallApp *app) {
    BattleHallApp_SetGraphicsBanks();
    BattleHallApp_InitBgLayers(app->bgConfig);
    BattleHallApp_InitLinkWindow(app);
    app->paletteData = PaletteData_Init(HEAP_ID_105);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_105);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_105);
    BattleHallApp_LoadTypeGridGraphics(app, GF_BG_LYR_MAIN_3);
    BattleHallApp_LoadBgPalette();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    BattleHallApp_LoadSubBgGraphics(app, GF_BG_LYR_SUB_1);
}
#pragma force_active reset

void BattleHallApp_InitSprites(BattleHallApp *app) {
    Pokemon *mon = Party_GetMonByIndex(app->party, 0);
    BattleHallSpriteGfx_Init(&app->spriteGfx, mon);
}

void BattleHallApp_FreeBgLayers(BgConfig *bgConfig) {
    reg_GX_DISPCNT &= ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    Heap_Free(bgConfig);
}

void BattleHallApp_VBlankCallback(BattleHallApp *app) {
    if (app->summaryApp == NULL) {
        if (app->paletteData != NULL) {
            PaletteData_PushTransparentBuffers(app->paletteData);
        }
        DoScheduledBgGpuUpdates(app->bgConfig);
        GF_RunVramTransferTasks();
        OamManager_ApplyAndResetBuffers();
        OS_SetIrqCheckFlag(OS_IE_V_BLANK);
    }
}

void BattleHallApp_SetGraphicsBanks(void) {
    GraphicsBanks banks = ov82_0223FEC4;
    GfGfx_SetBanks(&banks);
}

void BattleHallApp_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes = ov82_0223FE28;
    SetBothScreensModesAndDisable(&modes);
    {
        BgTemplate bg = ov82_0223FE54;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_105);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    }
    {
        BgTemplate bg = ov82_0223FE70;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_105);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    }
    {
        BgTemplate bg = ov82_0223FE8C;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    }
    {
        BgTemplate bg = ov82_0223FEA8;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    }
    {
        BgTemplate bg = ov82_0223FE38;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bg, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
}

// In link mode the top 16 lines of the main screen are masked
void BattleHallApp_InitLinkWindow(BattleHallApp *app) {
    if (BattleHall_IsMultiType(app->mode) != FALSE) {
        reg_GX_DISPCNT &= ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK);
        G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
        G2_SetWnd0Position(240, 0, 0, 16);
        GX_SetVisibleWnd(GX_WNDMASK_W0);
    }
}

void BattleHallApp_LoadTypeGridGraphics(BattleHallApp *app, u8 bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x17, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_105);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x18, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_105);
}

void BattleHallApp_LoadBgPalette(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x99, &plttData, HEAP_ID_105);
    DC_FlushRange(plttData->pRawData, 0xC0);
    GX_LoadBGPltt(plttData->pRawData, 0, 0xC0);
    Heap_Free(file);
}

void BattleHallApp_LoadSubBgGraphics(BattleHallApp *app, u32 bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x81, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_105);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x82, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_105);
    GfGfxLoader_GXLoadPalFromOpenNarc(app->narc, 0xBE, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_105);
}

void BattleHallApp_LoadTitleFrameGraphics(BattleHallApp *app, u32 bgId) {
    BG_ClearCharDataRange((u8)bgId, 0x20, 0, HEAP_ID_105);
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x85, app->bgConfig, (GFBgLayer)bgId, 1, 0, TRUE, HEAP_ID_105);
    GfGfxLoader_GXLoadPalFromOpenNarc(app->narc, 0xC0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, 0x20, HEAP_ID_105);
}

u8 BattleHallApp_PrintMessage(BattleHallApp *app, Window *window, u32 msgId, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    FillWindowPixelBuffer(window, bg);
    ReadMsgDataIntoString(app->msgData, msgId, app->rawString);
    StringExpandPlaceholders(app->msgFormat, app->expandedString, app->rawString);
    return AddTextPrinterParameterizedWithColor(window, fontId, app->expandedString, x, y, textSpeed, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
}

u8 BattleHallApp_ShowMessage(BattleHallApp *app, u32 msgId, u8 fontId) {
    u8 printerId = BattleHallApp_PrintMessage(app, &app->windows[0], msgId, 1, 1, 0, 1, 2, 15, fontId);
    CopyWindowToVram(&app->windows[0]);
    return printerId;
}

void BattleHallApp_BufferNumber(BattleHallApp *app, u32 field, u32 value) {
    BufferIntegerAsString(app->msgFormat, field, value, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
}

#pragma force_active on
void BattleHallApp_PrintMonName(BattleHallApp *app, Window *window, int x, int y, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    u16 name[12];
    String *string;

    GetMonData(Party_GetMonByIndex(app->party, 0), MON_DATA_SPECIES_NAME, name);
    FillWindowPixelBuffer(window, bg);
    string = String_New(11, HEAP_ID_105);
    CopyU16ArrayToString(string, name);
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
    String_Delete(string);
    CopyWindowToVram(window);
}

void BattleHallApp_PrintTypeNames(BattleHallApp *app, Window *window, int fg, int shadow, u8 bg, u8 fontId) {
    int row;
    MsgData *msgData;
    int base;
    int y;
    int column;
    int index;
    String *string;
    int x;

    FillWindowPixelBuffer(window, bg);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0735_bin, HEAP_ID_105);
    string = String_New(11, HEAP_ID_105);
    for (row = 0, base = 0, y = 16; row < TYPE_GRID_ROWS; base += TYPE_GRID_COLUMNS, y += 36, row++) {
        for (column = 0, x = 1; column < TYPE_GRID_COLUMNS; column++, x += 64) {
            index = column + base;
            if (BattleHall_GetCellType(index) != TYPE_CELL_EMPTY) {
                String_SetEmpty(string);
                ReadMsgDataIntoString(msgData, BattleHall_GetCellType(index), string);
                AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
            }
        }
    }
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}
#pragma force_active reset

void BattleHallApp_PrintNumber(BattleHallApp *app, Window *window, u32 value, u32 x, u32 y) {
    PrintUIntOnWindow(app->messagePrinter, value, 2, PRINTING_MODE_LEFT_ALIGN, window, x, y);
    ScheduleWindowCopyToVram(window);
}

void BattleHallApp_PrintTypeRanks(BattleHallApp *app, Window *window) {
    int row;
    int base;
    int y;

    row = 0;
    base = 0;
    y = 4;
    do {
        int column = 0;
        int x = 18;
        do {
            int index = column + base;
            u32 type = BattleHall_GetCellType((u8)index);
            if (type != TYPE_CELL_EMPTY && type != TYPE_MYSTERY) {
                u8 rank = (u8)(HallTypeRanks_Get(BattleHallApp_GetRecordIndex((u8)index), app->hallRecords) + 1);
                if (rank > MAX_TYPE_RANK) {
                    rank = MAX_TYPE_RANK;
                }
                BattleHallApp_PrintNumber(app, window, rank, x, y);
            }
            column++;
            x += 64;
        } while (column < TYPE_GRID_COLUMNS);
        base += TYPE_GRID_COLUMNS;
        y += 36;
        row++;
    } while (row < TYPE_GRID_ROWS);
}

void BattleHallApp_PrintSelectPrompt(BattleHallApp *app, Window *window, u32 x, u32 y, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    MsgData *msgData;
    String *string;

    FillWindowPixelBuffer(window, bg);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0441_bin, HEAP_ID_105);
    string = NewString_ReadMsgData(msgData, msg_0441_00037);
    FillWindowPixelBuffer(window, 15);
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}

void BattleHallApp_CreateSummaryArgs(BattleHallApp *app) {
    app->summaryArgs = Heap_Alloc(HEAP_ID_105, sizeof(PokemonSummaryArgs));
    memset(app->summaryArgs, 0, sizeof(PokemonSummaryArgs));
    app->summaryArgs->party = app->party;
    app->summaryArgs->unk11 = 1;
    app->summaryArgs->options = app->options;
    app->summaryArgs->unk12 = 1;
    app->summaryArgs->partyCount = Party_GetCount(app->party);
    app->summaryArgs->partySlot = 0;
    app->summaryArgs->moveToLearn = 0;
    app->summaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(app->saveData);
    app->summaryArgs->unk2C = sub_02088288(app->saveData);
    app->summaryArgs->ribbons = Save_SpecialRibbons_Get(app->saveData);
    app->summaryArgs->isFlag982Set = 0;
    PokemonSummary_SetPages(app->summaryArgs, _0223FE20);
    PokemonSummaryArgs_SetPlayer(app->summaryArgs, Save_PlayerData_GetProfile(app->saveData));
}

void BattleHallApp_SetMainState(BattleHallApp *app, int *state, int nextState) {
    app->subState = 0;
    *state = nextState;
}

// The grid is 4 columns by 5 rows; the empty cells (row 4) select the summary button
void BattleHallApp_HandleDpad(BattleHallApp *app, int unused) {
    BOOL moved = FALSE;
    u8 type;

    if (gSystem.newKeys & PAD_KEY_LEFT) {
        if (BattleHall_GetCellType(app->cursor) != TYPE_CELL_EMPTY) {
            app->prevCursor = app->cursor;
        }
        if (app->cursor % 4 == 0) {
            app->cursor += 3;
        } else if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
            app->cursor = 16;
        } else {
            app->cursor--;
        }
        moved = TRUE;
    }
    if (gSystem.newKeys & PAD_KEY_RIGHT) {
        if (BattleHall_GetCellType(app->cursor) != TYPE_CELL_EMPTY) {
            app->prevCursor = app->cursor;
        }
        if (app->cursor % 4 == 3) {
            app->cursor -= 3;
        } else if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
            app->cursor = 19;
        } else {
            app->cursor++;
        }
        moved = TRUE;
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        if (BattleHall_GetCellType(app->cursor) != TYPE_CELL_EMPTY) {
            app->prevCursor = app->cursor;
        }
        if (app->cursor < 4) {
            app->cursor += 16;
        } else if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
            if (app->prevCursor == 16) {
                app->cursor = 13;
            } else if (app->prevCursor == 19) {
                app->cursor = 14;
            } else if (app->prevCursor == 13 || app->prevCursor == 1) {
                app->cursor = 13;
            } else if (app->prevCursor == 14 || app->prevCursor == 2) {
                app->cursor = 14;
            } else {
                app->cursor = 13;
            }
        } else {
            app->cursor -= 4;
        }
        moved = TRUE;
    }
    if (gSystem.newKeys & PAD_KEY_DOWN) {
        if (BattleHall_GetCellType(app->cursor) != TYPE_CELL_EMPTY) {
            app->prevCursor = app->cursor;
        }
        if (app->cursor >= 16) {
            app->cursor -= 16;
        } else {
            app->cursor += 4;
        }
        moved = TRUE;
    }
    if (moved == TRUE) {
        PlaySE(SEQ_SE_DP_SELECT);
        BattleHallSprite_SetPosition(app->cursorSprite, BattleHallApp_GetCursorX(app), BattleHallApp_GetCursorY(app));
    }
    type = BattleHall_GetCellType(app->cursor);
    if (type == TYPE_CELL_EMPTY) {
        BattleHallSprite_SetAnim(app->cursorSprite, 2);
        BattleHallSprite_SetPosition(app->cursorSprite, 128, 168);
    } else {
        BattleHallSprite_SetAnim(app->cursorSprite, 1);
    }
}

#pragma force_active on
BOOL BattleHallApp_HandleTouch(BattleHallApp *app) {
    u16 x, y;
    int row, column;
    int top, bottom, left, right;

    if (gSystem.touchNew != 0) {
        x = gSystem.touchX;
        y = gSystem.touchY;
        for (row = 0; row < TYPE_GRID_ROWS; row++) {
            top = row * 36 + 3;
            bottom = top + 35;
            for (column = 0; column < TYPE_GRID_COLUMNS; column++) {
                left = column * 64 + 1;
                right = left + 63;
                if (left <= x && x <= right && top <= y && y <= bottom) {
                    app->cursor = column + row * 4;
                    BattleHallSprite_SetPosition(app->cursorSprite, BattleHallApp_GetCursorX(app), BattleHallApp_GetCursorY(app));
                    if (BattleHall_GetCellType(app->cursor) == TYPE_CELL_EMPTY) {
                        BattleHallSprite_SetAnim(app->cursorSprite, 2);
                        BattleHallSprite_SetPosition(app->cursorSprite, 128, 168);
                    } else {
                        BattleHallSprite_SetAnim(app->cursorSprite, 1);
                    }
                    app->usedTouch = 1;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}
#pragma force_active reset

BOOL BattleHallApp_HandleConfirm(BattleHallApp *app) {
    if (gSystem.newKeys & PAD_BUTTON_A) {
        app->usedTouch = 0;
        return TRUE;
    }
    return FALSE;
}

u16 BattleHallApp_GetCursorX(BattleHallApp *app) {
    return (u16)((app->cursor % 4) * 64 + 32);
}

u16 BattleHallApp_GetCursorY(BattleHallApp *app) {
    return (u16)((app->cursor / 4) * 36 + 16);
}

void BattleHallApp_GrayOutTypes(BattleHallApp *app, BgConfig *bgConfig) {
    int i;

    if (BattleHallApp_IsMatronMode(app) == 1) {
        for (i = 0; i < 17; i++) {
            BattleHallApp_SetCellPalette(bgConfig, (u8)i, 3);
        }
    } else {
        for (i = 0; i < 17; i++) {
            if (HallTypeRanks_Get((u8)i, app->hallRecords) >= MAX_TYPE_RANK) {
                BattleHallApp_SetCellPalette(bgConfig, (u8)i, 3);
            }
        }
        BattleHallApp_SetCellPalette(bgConfig, TYPE_CELL_MATRON, 3);
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_MAIN_3);
}

#pragma force_active on
// state: 0 = normal, 1 = selected, 2 = partner's choice, 3 = unavailable
void BattleHallApp_SetCellPalette(BgConfig *bgConfig, int index, int state) {
    u8 palette;
    int x;
    u8 cellX;
    int column;
    u8 height;
    u8 y;

    if (state == 0) {
        palette = 0;
    } else if (state == 1) {
        palette = 5;
    } else if (state == 2) {
        palette = 4;
    } else {
        palette = 3;
    }
    x = (index % 4) * 8;
    cellX = x;
    column = index % 8;
    if (column < 4) {
        height = 5;
    } else {
        height = 4;
    }
    if ((u32)index < 4) {
        y = 0;
    } else if ((u32)index < 8) {
        y = 5;
    } else if ((u32)index < 12) {
        y = 9;
    } else if ((u32)index < 16) {
        y = 14;
    } else {
        y = 18;
    }
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, cellX, y, 8, height, palette);
    if (state == 0) {
        cellX = x;
        if (column < 4) {
            height = 2;
        } else {
            height = 3;
        }
        if ((u32)index < 4) {
            y = 2;
        } else if ((u32)index < 8) {
            y = 6;
        } else if ((u32)index < 12) {
            y = 11;
        } else if ((u32)index < 16) {
            y = 15;
        } else {
            y = 20;
        }
        if ((u32)index < 9) {
            BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, cellX, y, 1, height, 1);
        } else {
            BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, cellX, y, 1, height, 2);
        }
    }
}
#pragma force_active reset

u8 BattleHallApp_GetRecordIndex(u8 index) {
    if (index >= 17) {
        index = 17;
    }
    return index;
}

// The Hall Matron appears in the 50th and 170th single battles
BOOL BattleHallApp_IsMatronBattle(BattleHallApp *app) {
    if (app->mode == 0) {
        if (app->battleNo == 50 || app->battleNo == 170) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 BattleHallApp_IsMatronMode(BattleHallApp *app) {
    return app->matronMode;
}

BOOL BattleHallApp_SendLinkCommand(BattleHallApp *app, u16 command, u16 arg) {
    u32 id;

    if (BattleHall_IsMultiType(app->mode) == FALSE) {
        return FALSE;
    }
    switch (command) {
    case 4:
        id = 0x27;
        BattleHallApp_PrepareLinkPing(app, command);
        break;
    case 5:
        id = 0x28;
        BattleHallApp_PrepareLinkChoice(app, command, arg);
        break;
    case 6:
        id = 0x29;
        BattleHallApp_PrepareLinkAnswer(app, command, arg);
        break;
    }
    if (Comm_SendCommand(id, &app->linkCommand, 0x2C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void BattleHallApp_PrepareLinkPing(BattleHallApp *app, u16 command) {
    Save_PlayerData_GetProfile(app->saveData);
    app->linkCommand = command;
}

void BattleHall_ReceivePing(u32 netId) {
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
}

void BattleHallApp_PrepareLinkChoice(BattleHallApp *app, u16 command, u16 choice) {
    Pokemon *mon;

    app->linkCommand = command;
    app->linkArg = choice;
    if (Comm_GetLocalNetId() == 0 && app->linkChoice == NO_LINK_CHOICE) {
        app->linkChoice = (u8)choice;
    }
    app->linkChoiceSent = app->linkChoice;
    mon = Party_GetMonByIndex(app->party, 0);
    app->linkLevel = (u16)GetMonData(mon, MON_DATA_LEVEL, NULL);
}

void BattleHall_ReceiveChoice(u32 netId, u32 unused, const u16 *msg, BattleHallApp *app) {
    app->recvCount++;
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    app->partnerCursor = (u8)msg[1];
    if (Comm_GetLocalNetId() == 0) {
        if (app->linkChoice != NO_LINK_CHOICE) {
            app->partnerCursor = 0;
        } else {
            app->linkChoice = (u8)(app->partnerCursor + 20);
        }
    } else {
        app->linkChoice = (u8)msg[2];
    }
    *app->unk_21C = msg[3];
}

void BattleHallApp_PrepareLinkAnswer(BattleHallApp *app, u16 command, u16 answer) {
    app->linkCommand = command;
    app->linkArg = answer;
}

void BattleHall_ReceiveAnswer(u32 netId, u32 unused, const u16 *msg, BattleHallApp *app) {
    u16 answer;

    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    answer = msg[1];
    if (answer != 0) {
        app->partnerAnswer = (u8)answer;
    }
}

void BattleHallApp_ResetYesNo(BattleHallApp *app) {
    if (app->yesNoActive == 1) {
        app->yesNoActive = 0;
        YesNoPrompt_Reset(app->yesNoPrompt);
    }
}

#pragma force_active on
void BattleHallApp_ShowConfirmPrompt(BattleHallApp *app) {
    u8 rank;
    int msgId;

    BattleHallSprite_SetVisible(app->monIcon, FALSE);
    BattleHallApp_DrawMessageFrame(&app->windows[0], Options_GetFrame(app->options));
    BufferTypeName(app->msgFormat, 0, BattleHall_GetCellType(app->cursor));
    rank = HallTypeRanks_Get(BattleHallApp_GetRecordIndex(app->cursor), app->hallRecords) + 1;
    if (rank > MAX_TYPE_RANK) {
        rank = MAX_TYPE_RANK;
    }
    BattleHallApp_BufferNumber(app, 1, rank);
    if (BattleHallApp_IsMatronMode(app) == 1) {
        msgId = msg_0441_00031;
    } else {
        msgId = msg_0441_00024;
    }
    app->printerId = BattleHallApp_ShowMessage(app, msgId, 1);
    BattleHallSprite_SetAnim(app->cursorSprite, 0);
    BattleHallApp_InitYesNo(app->yesNoPrompt, app->bgConfig, app->usedTouch);
    app->yesNoActive = 1;
}
#pragma force_active reset

void BattleHallApp_CancelConfirm(BattleHallApp *app) {
    BattleHallApp_ClearMessage(app);
    BattleHallApp_SetCellPalette(app->bgConfig, app->cursor, 0);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_MAIN_3);
    BattleHallSprite_SetAnim(app->cursorSprite, 1);
}

void BattleHallApp_ClearMessage(BattleHallApp *app) {
    BattleHallSprite_SetVisible(app->monIcon, TRUE);
    ClearFrameAndWindow2(&app->windows[0], FALSE);
    BattleHallApp_PrintMonName(app, &app->windows[1], 0, 0, 1, 2, 0, 0);
}

void BattleHallApp_SetBrightness(s32 brightness) {
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, brightness);
}

void BattleHallSpriteGfx_Init(BattleHallSpriteGfx *gfx, Pokemon *mon) {
    NARC *narc;
    int i;

    BattleHallSpriteGfx_InitTransfers();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, HEAP_ID_105);
    gfx->spriteList = G2dRenderer_Init(2, &gfx->renderer, HEAP_ID_105);
    for (i = 0; i < 4; i++) {
        gfx->resMan[i] = Create2DGfxResObjMan(ov82_0223FEEC[i], (GfGfxResType)i, HEAP_ID_105);
    }
    gfx->resObj[0][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_1_8_4, 0xC, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_105);
    gfx->resObj[0][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_1_8_4, 0x36, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_105);
    gfx->resObj[0][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_1_8_4, 0xE, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    gfx->resObj[0][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_1_8_4, 0xD, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);
    narc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_105);
    gfx->resObj[1][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, Pokemon_GetIconNaix(mon), FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_105);
    gfx->resObj[1][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_poketool_icongra_poke_icon, sub_02074490(), FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_105);
    gfx->resObj[1][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, sub_02074498(), FALSE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    gfx->resObj[1][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, sub_020744A4(), FALSE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);
    for (i = 0; i < 2; i++) {
        SpriteTransfer_CreateCharTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_CreateExtPlttTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_PLTT]);
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    NARC_Delete(narc);
}

Sprite *BattleHallSpriteGfx_CreateSprite(BattleHallSpriteGfx *gfx, int resourceId, int animSeq, int drawPriority, u8 screen) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&header, resourceId, resourceId, resourceId, resourceId, -1, -1, 0, 1, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = gfx->spriteList;
    template.header = &header;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = drawPriority;
    if (screen == 0) {
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    }
    template.heapID = HEAP_ID_105;
    if (screen == 1) {
        template.position.y += 3 << 18;
    }
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimSpeed(sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(sprite, animSeq);
    return sprite;
}

void BattleHallSpriteGfx_Free(BattleHallSpriteGfx *gfx) {
    u8 i;

    for (i = 0; i < 2; i++) {
        SpriteTransfer_DeleteCharTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_DeletePlttTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_PLTT]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resMan[i]);
    }
    SpriteList_Delete(gfx->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

void BattleHallSpriteGfx_InitTransfers(void) {
    ObjCharTransferTemplate template = ov82_0223FEF0;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(4, HEAP_ID_105);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

BattleHallSprite *BattleHallSprite_Create(BattleHallSpriteGfx *gfx, u32 resourceId, u32 animSeq, s32 x, u16 y, const u8 *anim) {
    BattleHallSprite *sprite = Heap_Alloc(HEAP_ID_105, sizeof(BattleHallSprite));
    VecFx32 position;

    memset(sprite, 0, sizeof(BattleHallSprite));
    sprite->anim = anim;
    sprite->sprite = BattleHallSpriteGfx_CreateSprite(gfx, resourceId, animSeq, 0, 0);
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    Sprite_SetMatrix(sprite->sprite, &position);
    return sprite;
}

u32 BattleHallSprite_Delete(BattleHallSprite *sprite) {
    Sprite_Delete(sprite->sprite);
    Heap_Free(sprite);
    return 0;
}

void BattleHallSprite_SetVisible(BattleHallSprite *sprite, BOOL visible) {
    Sprite_SetDrawFlag(sprite->sprite, visible);
}

void BattleHallSprite_SetPosition(BattleHallSprite *sprite, s32 x, s32 y) {
    VecFx32 position;

    if (sprite->anim != NULL) {
        Sprite_TryChangeAnimSeq(sprite->sprite, sprite->anim[0]);
    }
    position = *Sprite_GetMatrixPtr(sprite->sprite);
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    Sprite_SetMatrix(sprite->sprite, &position);
}

void BattleHallSprite_SetAnim(BattleHallSprite *sprite, u32 animSeq) {
    Sprite_SetAnimSpeed(sprite->sprite, FX32_ONE);
    Sprite_TryChangeAnimSeq(sprite->sprite, animSeq);
}

void BattleHallSprite_SetIconPalette(BattleHallSprite *sprite, Pokemon *mon) {
    Sprite_SetPalOffsetRespectVramOffset(sprite->sprite, Pokemon_GetIconPalette(mon));
}

void BattleHallApp_AddWindows(BgConfig *bgConfig, Window *windows) {
    u8 i;

    for (i = 0; i < 4; i++) {
        AddWindow(bgConfig, &windows[i], &ov82_0223FF00[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

void BattleHallApp_RemoveWindows(Window *windows) {
    u16 i;

    for (i = 0; i < 4; i++) {
        RemoveWindow(&windows[i]);
    }
}

void BattleHallApp_DrawMessageFrame(Window *window, u32 frame) {
    LoadUserFrameGfx2(window->bgConfig, (GFBgLayer)GetWindowBgId(window), 0x3D9, 10, (u8)frame, HEAP_ID_105);
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, FALSE, 0x3D9, 10);
}

YesNoPrompt *BattleHallApp_CreateYesNo(enum HeapID heapId) {
    return YesNoPrompt_Create(heapId);
}

void BattleHallApp_DestroyYesNo(YesNoPrompt *prompt) {
    YesNoPrompt_Destroy(prompt);
}

#pragma force_active on
#pragma push
#pragma opt_propagation off
void BattleHallApp_InitYesNo(YesNoPrompt *prompt, BgConfig *bgConfig, u8 ignoreTouch) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(template));
    {
        int zero = 0;
        template.tileStart = 109;
        template.plttSlot = 14;
        template.bgConfig = bgConfig;
        template.bgId = zero;
        template.x = 24;
        template.y = 10;
        template.ignoreTouchFlag = ignoreTouch;
        template.initialCursorPos = 0;
        template.shapeParam = zero;
    }
    YesNoPrompt_InitFromTemplate(prompt, &template);
}
#pragma pop
#pragma force_active reset

u32 BattleHallApp_HandleYesNo(YesNoPrompt *prompt) {
    return YesNoPrompt_HandleInput(prompt);
}
