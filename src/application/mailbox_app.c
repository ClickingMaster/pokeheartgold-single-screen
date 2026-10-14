#include "mailbox_app.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0232.h"

#include "bag.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "list_menu_items.h"
#include "mail.h"
#include "mail_message.h"
#include "mail_misc.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "sprite_resource_header.h"
#include "grid_input_handler.h"
#include "touchpad.h"
#include "yes_no_prompt.h"

#define MAILBOX_MAIL_PER_PAGE  10
#define MAILBOX_NO_MAIL        0xFF
#define MAILBOX_NUM_WINDOWS    30
#define MAILBOX_NUM_SPRITES    7
#define MAILBOX_STATE_FINISHED 25

// Window indices: 0-19 one name plate per mail slot, 20 the plate of the selected mail, then:
#define MAILBOX_WIN_CANCEL   21
#define MAILBOX_WIN_CLOSE    22
#define MAILBOX_WIN_PAGE     23
#define MAILBOX_WIN_MESSAGE  24
#define MAILBOX_WIN_TITLE    25
#define MAILBOX_WIN_MAIL_MSG 26 // 26-28: the three lines of the mail being read
#define MAILBOX_WIN_AUTHOR   29

// BgCellManager cell 10 shows the selected mail's name plate; 0-9 the plates of the current page.
#define MAILBOX_CELL_SELECTED 10

typedef struct MailboxAppPaletteFlash {
    u8 isBg : 1;          // 0x0: 0 = flash a sprite, 1 = flash a cell's tilemap rectangle
    u8 target : 7;        // 0x0: sprite index or bg layer
    u8 firstPalette : 4;  // 0x1
    u8 secondPalette : 4; // 0x1
    u8 state;             // 0x2
    u8 timer;             // 0x3
    u8 x;                 // 0x4
    u8 y;                 // 0x5
    u8 width;             // 0x6
    u8 height;            // 0x7
} MailboxAppPaletteFlash;

typedef struct MailboxAppData {
    BgConfig *bgConfig;                          // 0x000
    BgCellManager *cellManager;                  // 0x004
    u8 nameplateTileLeft[0x20];                  // 0x008
    u8 nameplateTileRight[0x20];                 // 0x028
    Window windows[MAILBOX_NUM_WINDOWS];         // 0x048
    MsgData *msgData;                            // 0x228
    MessageFormat *msgFormat;                    // 0x22C
    String *string;                              // 0x230
    u8 printerId;                                // 0x234
    u8 waitForInput;                             // 0x235
    u8 filler_236[2];                            // 0x236
    YesNoPrompt *yesNoPrompt;                    // 0x238
    u32 yesNoQuestion;                           // 0x23C
    TouchscreenListMenuSpawner *listMenuSpawner; // 0x240
    TouchscreenListMenu *listMenu;               // 0x244
    ListMenuItem *listMenuItems;                 // 0x248
    u8 filler_24C[4];                            // 0x24C
    SpriteSystem *spriteSystem;                  // 0x250
    SpriteManager *spriteManager;                // 0x254
    ManagedSprite *sprites[MAILBOX_NUM_SPRITES]; // 0x258: 0/1 page arrows, 2 cursor, 4-6 icons
    NARC *monIconNarc;                           // 0x274
    GridInputHandler *gridInput;                 // 0x278
    Mail *mail[MAILBOX_MSG_COUNT];               // 0x27C
    u8 mailOrder[MAILBOX_MSG_COUNT];             // 0x2CC: mailbox slot of each listed mail
    u16 mailCount;                               // 0x2E0
    u16 lastPage;                                // 0x2E2
    MailboxAppPaletteFlash flash;                // 0x2E4
    int stateAfterFade;                          // 0x2EC
} MailboxAppData;                                // size: 0x2F0

typedef struct MailboxApp {
    Options *options;         // 0x00
    Mailbox *mailbox;         // 0x04
    MailboxAppArgs *args; // 0x08
    MailboxAppData *data;     // 0x0C
    OverlayManager *subApp;   // 0x10
    void *subAppArgs;         // 0x14
    u32 subAppId;             // 0x18: index into gMailboxSubApps
    u16 page;                 // 0x1C
    u8 cursorPos;             // 0x1E
    u8 selectedMail;          // 0x1F
    u8 partyMenuAction;       // 0x20
    u8 partySlot : 7;         // 0x21
    u8 bagFull : 1;           // 0x21: the erased mail could not be put back in the bag
    u16 itemId;               // 0x22
    u32 useMailResult;        // 0x24
    int nextState;            // 0x28
} MailboxApp;                 // size: 0x2C

typedef int (*MailboxAppStateFunc)(MailboxApp *app);

typedef struct MailboxAppSubApp {
    MailboxAppStateFunc start;
    MailboxAppStateFunc finish;
    int nextState;
} MailboxAppSubApp;

extern const TouchscreenListMenuTemplate gMailboxMailMenuTemplate;
extern const GraphicsModes gMailboxAppGraphicsModes;
extern const BgTemplate gMailboxBgTemplate1;
extern const BgTemplate gMailboxBgTemplate2;
extern const BgTemplate gMailboxBgTemplate6;
extern const BgTemplate gMailboxBgTemplate5;
extern const BgTemplate gMailboxBgTemplate4;
extern const BgTemplate gMailboxBgTemplate0;
extern const BgTemplate gMailboxBgTemplate3;
extern const BgTemplate gMailboxBgTemplate7;
extern const GraphicsBanks gMailboxAppGraphicsBanks;
extern const OverlayManagerTemplate gMailboxUseMailOverlayTemplate;
extern const MailboxAppStateFunc gMailboxYesNoHandlers[][2];
extern const MailboxAppSubApp gMailboxSubApps[];
extern const MailboxAppStateFunc gMailboxStateFuncs[];
extern const OamCharTransferParam gMailboxAppDataOamCharTransferParam;
extern const SpriteResourceCountsListUnion gMailboxAppDataSpriteResourceCounts;
extern const OamManagerParam gMailboxAppDataOamManagerParam;
extern const ManagedSpriteTemplate gMailboxSpriteTemplates[MAILBOX_NUM_SPRITES];
extern const WindowTemplate gMailboxWindowTemplates[MAILBOX_NUM_WINDOWS];
extern const TouchscreenHitbox gMailboxCloseMailHitboxes[];
extern const TouchscreenHitbox gMailboxPageArrowHitboxes[];
extern const GridCallbacks gMailboxGridCallbacks;
extern const TouchscreenHitbox gMailboxGridHitboxes[];
extern const DpadMenuBox gMailboxGridBoxes[];

void MailboxApp_VBlankCB(void *cbArg);
void MailboxApp_SetGfxBanks(void);
void MailboxApp_InitBgLayers(MailboxApp *app);
void MailboxApp_FreeBgLayers(MailboxApp *app);
void MailboxApp_LoadBgGraphics(MailboxApp *app);
void MailboxApp_LoadFrameAndFontGfx(MailboxApp *app);
void MailboxApp_CreateNameplateCells(MailboxApp *app);
void MailboxApp_DeleteNameplateCells(MailboxApp *app);
void MailboxAppData_CreateStrings(MailboxAppData *data);
void MailboxAppData_DeleteStrings(MailboxAppData *data);
void MailboxApp_CreateYesNoPrompt(MailboxApp *app);
void MailboxApp_DestroyYesNoPrompt(MailboxApp *app);
void MailboxApp_ShowYesNoPrompt(MailboxApp *app);
void MailboxApp_CreateMailMenuItems(MailboxApp *app);
void MailboxApp_DeleteMailMenuItems(MailboxApp *app);
void MailboxApp_OpenMailMenu(MailboxApp *app);
void MailboxApp_CloseMailMenu(MailboxApp *app);
void MailboxApp_LoadMail(MailboxApp *app);
void MailboxApp_FreeMail(MailboxApp *app);
void MailboxApp_DrawPage(MailboxApp *app);
void MailboxApp_DrawSelectedNameplate(MailboxApp *app);
BOOL MailboxApp_RunPaletteFlash(MailboxApp *app);
void MailboxApp_LoadStationeryGfx(MailboxApp *app);
u32 MailboxApp_GetMailMenuItemCount(MailboxApp *app);
BOOL MailboxApp_RunState(MailboxApp *app, int *state);
int MailboxApp_State_Setup(MailboxApp *app);
int MailboxApp_State_Teardown(MailboxApp *app);
int MailboxApp_State_WaitFade(MailboxApp *app);
int MailboxApp_State_WaitText(MailboxApp *app);
int MailboxApp_State_YesNo(MailboxApp *app);
int MailboxApp_State_PaletteFlash(MailboxApp *app);
int MailboxApp_State_StartSubApp(MailboxApp *app);
int MailboxApp_State_RunSubApp(MailboxApp *app);
int MailboxApp_State_FadeIn(MailboxApp *app);
int MailboxApp_State_SelectMail(MailboxApp *app);
int MailboxApp_State_Quit(MailboxApp *app);
int MailboxApp_State_ChangePage(MailboxApp *app);
int MailboxApp_State_ShowMailPrompt(MailboxApp *app);
int MailboxApp_State_OpenMailMenu(MailboxApp *app);
int MailboxApp_State_MailMenu(MailboxApp *app);
int MailboxApp_State_ReadMail(MailboxApp *app);
int MailboxApp_State_CloseMail(MailboxApp *app);
int MailboxApp_State_ConfirmErase(MailboxApp *app);
int MailboxApp_State_PrintHoldMailPrompt(MailboxApp *app);
int MailboxApp_State_ConfirmHoldMail(MailboxApp *app);
int ov103_021ED960(MailboxApp *app);
int ov103_021ED97C(MailboxApp *app);
int MailboxApp_State_ShowMailToBagResult(MailboxApp *app);
int ov103_021ED9AC(MailboxApp *app);
int MailboxApp_ReturnToMailList(MailboxApp *app);
int MailboxApp_StartFadeIn(MailboxApp *app, int nextState);
int MailboxApp_StartFadeOut(MailboxApp *app, int nextState);
int MailboxApp_WaitForText(MailboxApp *app, u8 waitForInput, int nextState);
int MailboxApp_AskYesNo(MailboxApp *app, u32 question);
int MailboxApp_StartCellFlash(MailboxApp *app, int cell, int nextState);
int MailboxApp_StartSpriteFlash(MailboxApp *app, int sprite, int nextState);
void MailboxAppData_SetListDimmed(MailboxAppData *data, BOOL enable);
void MailboxApp_ShowMail(MailboxApp *app);
void MailboxApp_HideMail(MailboxApp *app);
void MailboxApp_PutMailInBag(MailboxApp *app);
void MailboxApp_DeleteSelectedMail(MailboxApp *app);
void MailboxApp_CreatePartyMenuArgs(MailboxApp *app, u8 context, u16 itemId, u8 partySlot);
int MailboxApp_StartPartyMenu(MailboxApp *app);
int MailboxApp_FinishGiveMail(MailboxApp *app);
int MailboxApp_FinishSelectMonForMail(MailboxApp *app);
int MailboxApp_FinishPartyMenu(MailboxApp *app);
int MailboxApp_StartUseMail(MailboxApp *app);
int MailboxApp_FinishUseMail(MailboxApp *app);
int MailboxApp_EraseMail(MailboxApp *app);
int MailboxApp_SelectMonToHoldMail(MailboxApp *app);
int MailboxApp_PrintMailToBagResult(MailboxApp *app);
void MailboxApp_CreateSprites(MailboxApp *app);
void MailboxApp_DeleteSprites(MailboxApp *app);
void MailboxAppData_TickSprites(MailboxAppData *data);
void MailboxAppData_InitSpriteSystem(MailboxAppData *data);
void MailboxAppData_FreeSpriteSystem(MailboxAppData *data);
ManagedSprite *MailboxAppData_NewSprite(MailboxAppData *data, const ManagedSpriteTemplate *template);
void MailboxAppData_DeleteSprite(MailboxAppData *data, u32 index);
void MailboxAppData_DeleteAllSprites(MailboxAppData *data);
void MailboxAppData_SetSpriteAnim(MailboxAppData *data, u32 index, u32 anim);
void MailboxAppData_SetSpriteDrawFlag(MailboxAppData *data, u32 index, BOOL draw);
void MailboxAppData_SetSpritePosition(MailboxAppData *data, u32 index, s16 x, s16 y);
void MailboxAppData_SetSpriteTranslucent(MailboxAppData *data, u32 index, BOOL blend);
void MailboxAppData_OpenMonIconNarc(MailboxAppData *data);
void MailboxAppData_CloseMonIconNarc(MailboxAppData *data);
void MailboxAppData_LoadMonIconResources(MailboxAppData *data);
void MailboxApp_ShowMailMonIcons(MailboxApp *app);
void MailboxAppData_LoadSpriteResources(MailboxAppData *data);
void MailboxAppData_MoveCursorToCloseButton(MailboxAppData *data);
void MailboxApp_AddWindows(MailboxApp *app);
void MailboxApp_RemoveWindows(MailboxApp *app);
void MailboxApp_PrintString(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align);
void MailboxAppData_PrintMsg(MailboxAppData *data, MsgData *msgData, u32 windowId, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align);
void MailboxApp_DrawNameplate(MailboxApp *app, u32 cell, u32 windowId, u32 mailIdx);
void MailboxApp_PrintTitleAndButtons(MailboxApp *app);
void MailboxAppData_ShowCloseButton(MailboxAppData *data);
void MailboxAppData_ShowCancelButton(MailboxAppData *data);
void MailboxApp_PrintPageNumber(MailboxApp *app);
void MailboxApp_BufferAuthorName(MailboxApp *app, u32 unused);
void MailboxApp_PrintMessageBox(MailboxApp *app);
void MailboxApp_PrintMailPrompt(MailboxApp *app);
void MailboxApp_DrawMessageBox(MailboxApp *app);
void MailboxApp_ClearMessageBox(MailboxApp *app);
void MailboxApp_PrintMessage(MailboxApp *app, u32 message);
void MailboxApp_PrintMail(MailboxApp *app);
void MailboxApp_PrintMailLine(MailboxApp *app, u32 windowId, u32 line);
int MailboxApp_GetTouchedPageArrow(MailboxApp *app);
void MailboxApp_CreateGridInput(MailboxApp *app);
void MailboxApp_FreeGridInput(MailboxApp *app);
void MailboxApp_GridOnButton(void);
void MailboxApp_GridOnSwitchToTouchMode(void);
void MailboxApp_GridOnKeyMove(MailboxApp *app, u32 input);
void MailboxApp_GridOnTouch(MailboxApp *app, u32 input);
void MailboxApp_MoveCursor(MailboxApp *app, u32 input);
BOOL MailboxApp_IsCloseMailInput(void);

// text.h (and render_text.h, which includes it) is not included: it declares fontId as u8 (FontID), but
// this module passes it unnarrowed
void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);
u8 TextPrinterCheckActive(u8 printerId);

// Defined in src/bg_cell_manager.c but not declared in bg_cell_manager.h (other modules declare them with their
// own parameter types)
void BgCellManager_LoadCellTilemap(BgCellManager *manager, int index, int narcId, int fileId, int compressed);
void BgCellManager_DrawCell(BgCellManager *manager, int index);
void BgCellManager_GetCellPosition(BgCellManager *manager, int index, s8 *x, s8 *y);
void BgCellManager_PutWindowTiles(BgCellManager *manager, int index, Window *window);
u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);

BOOL MailboxApp_Init(OverlayManager *man, int *state) {
    MailboxApp *app;

    Heap_Create(HEAP_ID_3, HEAP_ID_156, 0x10000);
    app = OverlayManager_CreateAndGetData(man, sizeof(MailboxApp), HEAP_ID_156);
    MI_CpuFill8(app, 0, sizeof(MailboxApp));
    app->args = OverlayManager_GetArgs(man);
    app->options = Save_PlayerData_GetOptionsAddr(app->args->saveData);
    app->mailbox = Save_Mailbox_Get(app->args->saveData);
    app->nextState = 8;
    return TRUE;
}

BOOL MailboxApp_Main(OverlayManager *man, int *state) {
    MailboxApp *app = OverlayManager_GetData(man);
    if (MailboxApp_RunState(app, state) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL MailboxApp_Exit(OverlayManager *man, int *state) {
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_156);
    return TRUE;
}

#pragma force_active on
void MailboxApp_VBlankCB(void *cbArg) {
    MailboxApp *app = cbArg;
    vu32 *dtcm = (vu32 *)0x027E0000;

    DoScheduledBgGpuUpdates(app->data->bgConfig);
    SpriteSystem_TransferOam();
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}
#pragma force_active reset

void MailboxApp_SetGfxBanks(void) {
    GfGfx_SetBanks(&gMailboxAppGraphicsBanks);
}

void MailboxApp_InitBgLayers(MailboxApp *app) {
    GraphicsModes modes;
    BgTemplate bg4, bg5, bg6, bg7, bg0, bg1, bg2, bg3;

    app->data->bgConfig = BgConfig_Alloc(HEAP_ID_157);
    modes = gMailboxAppGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    bg4 = gMailboxBgTemplate4;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_SUB_0, &bg4, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_157);
    bg5 = gMailboxBgTemplate5;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_SUB_1, &bg5, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_1);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_157);
    bg6 = gMailboxBgTemplate6;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_SUB_2, &bg6, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_2);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_157);
    bg7 = gMailboxBgTemplate7;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_SUB_3, &bg7, GF_BG_TYPE_TEXT);
    bg0 = gMailboxBgTemplate0;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_MAIN_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_157);
    bg1 = gMailboxBgTemplate1;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_157);
    bg2 = gMailboxBgTemplate2;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_MAIN_2);
    bg3 = gMailboxBgTemplate3;
    InitBgFromTemplate(app->data->bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
}

void MailboxApp_FreeBgLayers(MailboxApp *app) {
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(app->data->bgConfig, GF_BG_LYR_SUB_0);
    Heap_Free(app->data->bgConfig);
}

void MailboxApp_LoadBgGraphics(MailboxApp *app) {
    NARC *narc = NARC_New(NARC_a_2_5_0, HEAP_ID_157);
    NNSG2dCharacterData *charData;
    void *charFile;
    u8 *rawData;

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->data->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, app->data->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_157);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, app->data->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, app->data->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, app->data->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_157);
    charFile = GfGfxLoader_GetCharDataFromOpenNarc(narc, 1, TRUE, &charData, HEAP_ID_157);
    rawData = charData->pRawData;
    MI_CpuCopy32(rawData + 0xE0, app->data->nameplateTileLeft, 0x20);
    MI_CpuCopy32(rawData + 0x160, app->data->nameplateTileRight, 0x20);
    Heap_Free(charFile);
    NARC_Delete(narc);
}

void MailboxApp_LoadFrameAndFontGfx(MailboxApp *app) {
    MailboxAppData *data = app->data;
    u8 frame = Options_GetFrame(app->options);

    LoadUserFrameGfx2(data->bgConfig, GF_BG_LYR_MAIN_0, 0x3E2, 13, frame, HEAP_ID_157);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(7 << 6), HEAP_ID_157);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(0x1E << 4), HEAP_ID_157);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)(0x1E << 4), HEAP_ID_157);
}

void MailboxApp_CreateNameplateCells(MailboxApp *app) {
    u32 i;
    u16 *firstTilemap;

    app->data->cellManager = BgCellManager_New(app->data->bgConfig, 2, 11, HEAP_ID_157);
    for (i = 0; i < MAILBOX_MAIL_PER_PAGE; i++) {
        BgCellManager_InitCell(app->data->cellManager, i, 2, 14, 4);
    }
    BgCellManager_InitCell(app->data->cellManager, MAILBOX_CELL_SELECTED, 1, 14, 4);
    BgCellManager_LoadCellTilemap(app->data->cellManager, 0, NARC_a_2_5_0, 3, TRUE);
    firstTilemap = BgCellManager_GetCellTilemap(app->data->cellManager, 0);
    for (i = 1; i <= MAILBOX_MAIL_PER_PAGE; i++) {
        MI_CpuCopy8(firstTilemap, BgCellManager_GetCellTilemap(app->data->cellManager, i), 0x70);
    }
    for (i = 0; i < MAILBOX_MAIL_PER_PAGE; i++) {
        BgCellManager_DrawCellAt(app->data->cellManager, i, (s8)((i & 1) * 16 + 1), (s8)((i >> 1) * 4));
    }
}

void MailboxApp_DeleteNameplateCells(MailboxApp *app) {
    BgCellManager_Delete(app->data->cellManager);
}

void MailboxAppData_CreateStrings(MailboxAppData *data) {
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0232_bin, HEAP_ID_157);
    data->msgFormat = MessageFormat_New(HEAP_ID_157);
    data->string = String_New(0x400, HEAP_ID_157);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
}

void MailboxAppData_DeleteStrings(MailboxAppData *data) {
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    String_Delete(data->string);
    MessageFormat_Delete(data->msgFormat);
    DestroyMsgData(data->msgData);
}

void MailboxApp_CreateYesNoPrompt(MailboxApp *app) {
    app->data->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_157);
}

void MailboxApp_DestroyYesNoPrompt(MailboxApp *app) {
    YesNoPrompt_Destroy(app->data->yesNoPrompt);
}

void MailboxApp_ShowYesNoPrompt(MailboxApp *app) {
    YesNoPromptTemplate template;

    template.bgConfig = app->data->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = 0x3A6;
    template.plttSlot = 10;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = FALSE;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(app->data->yesNoPrompt, &template);
}

// READ / ERASE / GIVE / CANCEL; only READ and CANCEL when the party is empty.
void MailboxApp_CreateMailMenuItems(MailboxApp *app) {
    u32 numItems;
    u16 i;

    app->data->listMenuSpawner = TouchscreenListMenuSpawner_Create(HEAP_ID_157, NULL);
    numItems = MailboxApp_GetMailMenuItemCount(app);
    app->data->listMenuItems = ListMenuItems_New(numItems, HEAP_ID_157);
    i = 0;
    do {
        if (numItems != 2 || (i != 2 && i != 1)) {
            ListMenuItems_AppendFromMsgData(app->data->listMenuItems, app->data->msgData, msg_0232_00005 + i, i);
        }
        i++;
    } while (i < 4);
}

void MailboxApp_DeleteMailMenuItems(MailboxApp *app) {
    ListMenuItems_Delete(app->data->listMenuItems);
    TouchscreenListMenuSpawner_Destroy(app->data->listMenuSpawner);
}

void MailboxApp_OpenMailMenu(MailboxApp *app) {
    TouchscreenListMenuHeader header;
    TouchscreenListMenuTemplate template = gMailboxMailMenuTemplate;

    header.template = template;
    header.listMenuItems = app->data->listMenuItems;
    header.bgConfig = app->data->bgConfig;
    header.numWindows = MailboxApp_GetMailMenuItemCount(app);
    if (header.numWindows == 4) {
        app->data->listMenu = TouchscreenListMenu_Create(app->data->listMenuSpawner, &header, 0, 18, 5, 11, 0);
    } else {
        app->data->listMenu = TouchscreenListMenu_Create(app->data->listMenuSpawner, &header, 0, 18, 11, 11, 0);
    }
}

void MailboxApp_CloseMailMenu(MailboxApp *app) {
    TouchscreenListMenu_Destroy(app->data->listMenu);
}

void MailboxApp_LoadMail(MailboxApp *app) {
    u32 i;

    app->data->mailCount = 0;
    for (i = 0; i < MAILBOX_MSG_COUNT; i++) {
        app->data->mailOrder[i] = MAILBOX_NO_MAIL;
        app->data->mail[i] = Mailbox_AllocAndFetchMailI(app->mailbox->msgs, 0, i, HEAP_ID_157);
        if (Mail_TypeIsValid(app->data->mail[i]) == TRUE) {
            app->data->mailOrder[app->data->mailCount] = i;
            app->data->mailCount++;
        }
    }
    if (app->data->mailCount == 0) {
        app->data->lastPage = 0;
    } else {
        app->data->lastPage = (app->data->mailCount - 1) / MAILBOX_MAIL_PER_PAGE;
    }
}

void MailboxApp_FreeMail(MailboxApp *app) {
    u32 i;

    for (i = 0; i < MAILBOX_MSG_COUNT; i++) {
        Heap_Free(app->data->mail[i]);
    }
}

#pragma force_active on
void MailboxApp_DrawPage(MailboxApp *app) {
    u32 first = (u16)(app->page * MAILBOX_MAIL_PER_PAGE);
    u16 cell = 0;
    u8 *mailOrder = app->data->mailOrder + first;
    u16 windowId = first;
    u16 i;

    for (i = 0; i < MAILBOX_MAIL_PER_PAGE; i++) {
        u8 mailIdx = mailOrder[i];
        if (mailIdx == MAILBOX_NO_MAIL) {
            BgCellManager_EraseCell(app->data->cellManager, cell);
        } else {
            MailboxApp_DrawNameplate(app, cell, windowId, mailIdx);
            BgCellManager_DrawCell(app->data->cellManager, cell);
        }
        windowId++;
        cell++;
    }
}
#pragma force_active reset

void MailboxApp_DrawSelectedNameplate(MailboxApp *app) {
    MailboxApp_DrawNameplate(app, MAILBOX_CELL_SELECTED, 20, app->selectedMail);
    BgCellManager_DrawCellAt(app->data->cellManager, MAILBOX_CELL_SELECTED, 4, 7);
}

BOOL MailboxApp_RunPaletteFlash(MailboxApp *app) {
    MailboxAppPaletteFlash *flash = &app->data->flash;

    switch (flash->state) {
    case 0:
        if (!flash->isBg) {
            ManagedSprite_SetPaletteOverride(app->data->sprites[flash->target], flash->firstPalette);
        } else {
            BgTilemapRectChangePalette(app->data->bgConfig, flash->target, flash->x, flash->y, flash->width, flash->height, flash->firstPalette);
            ScheduleBgTilemapBufferTransfer(app->data->bgConfig, flash->target);
        }
        flash->state++;
        break;
    case 1:
        flash->timer++;
        if (flash->timer == 4) {
            if (!flash->isBg) {
                ManagedSprite_SetPaletteOverride(app->data->sprites[flash->target], flash->secondPalette);
            } else {
                BgTilemapRectChangePalette(app->data->bgConfig, flash->target, flash->x, flash->y, flash->width, flash->height, flash->secondPalette);
                ScheduleBgTilemapBufferTransfer(app->data->bgConfig, flash->target);
            }
            flash->timer = 0;
            flash->state++;
        }
        break;
    case 2:
        flash->timer++;
        if (flash->timer == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

void MailboxApp_LoadStationeryGfx(MailboxApp *app) {
    NARC *narc = NARC_New(NARC_a_0_7_9, HEAP_ID_157);
    u32 mailType = Mail_GetType(app->data->mail[app->selectedMail]);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, mailType + 12, app->data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, mailType, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, mailType + 24, app->data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_157);
    NARC_Delete(narc);
}

u32 MailboxApp_GetMailMenuItemCount(MailboxApp *app) {
    if (Party_GetCount(SaveArray_Party_Get(app->args->saveData)) > 0) {
        return 4;
    }
    return 2;
}

BOOL MailboxApp_RunState(MailboxApp *app, int *state) {
    *state = gMailboxStateFuncs[*state](app);
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

int MailboxApp_State_Setup(MailboxApp *app) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(0);
    GfGfx_EngineBSetPlanes(0);
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

int MailboxApp_State_Teardown(MailboxApp *app) {
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
    GfGfx_EngineASetPlanes(0);
    GfGfx_EngineBSetPlanes(0);
    Heap_Free(app->data);
    app->data = NULL;
    Heap_Destroy(HEAP_ID_157);
    return app->nextState;
}

int MailboxApp_State_WaitFade(MailboxApp *app) {
    if (IsPaletteFadeFinished() == TRUE) {
        return app->data->stateAfterFade;
    }
    return 2;
}

int MailboxApp_State_WaitText(MailboxApp *app) {
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
    return 3;
}

int MailboxApp_State_YesNo(MailboxApp *app) {
    switch (YesNoPrompt_HandleInput(app->data->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        YesNoPrompt_Reset(app->data->yesNoPrompt);
        return gMailboxYesNoHandlers[app->data->yesNoQuestion][0](app);
    case YESNORESPONSE_NO:
        YesNoPrompt_Reset(app->data->yesNoPrompt);
        return gMailboxYesNoHandlers[app->data->yesNoQuestion][1](app);
    default:
        return 4;
    }
}

int MailboxApp_State_PaletteFlash(MailboxApp *app) {
    if (MailboxApp_RunPaletteFlash(app) == FALSE) {
        return app->nextState;
    }
    return 5;
}

int MailboxApp_State_StartSubApp(MailboxApp *app) {
    gMailboxSubApps[app->subAppId].start(app);
    return 7;
}

int MailboxApp_State_RunSubApp(MailboxApp *app) {
    if (OverlayManager_Run(app->subApp) == FALSE) {
        return 7;
    }
    OverlayManager_Delete(app->subApp);
    app->nextState = gMailboxSubApps[app->subAppId].finish(app);
    return gMailboxSubApps[app->subAppId].nextState;
}

int MailboxApp_State_FadeIn(MailboxApp *app) {
    return MailboxApp_StartFadeIn(app, 9);
}

int MailboxApp_State_SelectMail(MailboxApp *app) {
    u32 input;

    switch (MailboxApp_GetTouchedPageArrow(app)) {
    case 0:
        if (app->page != 0) {
            app->page--;
            PlaySE(SEQ_SE_DP_SELECT);
            return MailboxApp_StartSpriteFlash(app, 0, 11);
        } else {
            return 9;
        }
    case 1:
        if (app->page != app->data->lastPage) {
            app->page++;
            PlaySE(SEQ_SE_DP_SELECT);
            return MailboxApp_StartSpriteFlash(app, 1, 11);
        } else {
            return 9;
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
            return MailboxApp_StartCellFlash(app, input, 12);
        }
        break;
    case 10:
    case -2:
        PlaySE(SEQ_SE_DP_DECIDE);
        return MailboxApp_StartSpriteFlash(app, 3, 10);
    case -1:
        input = GridInputHandler_GetNextInput(app->data->gridInput);
        if ((gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) && (input == 1 || input == 3 || input == 5 || input == 7 || input == 9) && app->page != app->data->lastPage) {
            app->page++;
            PlaySE(SEQ_SE_DP_SELECT);
            return MailboxApp_StartSpriteFlash(app, 1, 11);
        }
        if ((gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) && (input == 0 || input == 2 || input == 4 || input == 6 || input == 8) && app->page != 0) {
            app->page--;
            PlaySE(SEQ_SE_DP_SELECT);
            return MailboxApp_StartSpriteFlash(app, 0, 11);
        }
        break;
    case -3:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case -4:
        break;
    }
    return 9;
}

int MailboxApp_State_Quit(MailboxApp *app) {
    app->nextState = MAILBOX_STATE_FINISHED;
    return MailboxApp_StartFadeOut(app, 1);
}

int MailboxApp_State_ChangePage(MailboxApp *app) {
    if (app->page == 0) {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, FALSE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, TRUE);
    } else {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, TRUE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, FALSE);
    }
    MailboxApp_PrintPageNumber(app);
    MailboxApp_DrawPage(app);
    return 9;
}

int MailboxApp_State_ShowMailPrompt(MailboxApp *app) {
    MailboxAppData_SetSpriteDrawFlag(app->data, 2, FALSE);
    MailboxAppData_SetListDimmed(app->data, TRUE);
    MailboxApp_DrawSelectedNameplate(app);
    MailboxApp_DrawMessageBox(app);
    MailboxApp_PrintMailPrompt(app);
    return MailboxApp_WaitForText(app, FALSE, 13);
}

int MailboxApp_State_OpenMailMenu(MailboxApp *app) {
    MailboxApp_OpenMailMenu(app);
    return 14;
}

int MailboxApp_State_MailMenu(MailboxApp *app) {
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
        return 15;
    case 1: // ERASE
        MailboxApp_CloseMailMenu(app);
        MailboxApp_PrintMessage(app, 0);
        return MailboxApp_WaitForText(app, FALSE, 17);
    case 2: // GIVE
        MailboxApp_CloseMailMenu(app);
        itemId = MailToItemId(Mail_GetType(app->data->mail[app->selectedMail]));
        MailboxApp_CreatePartyMenuArgs(app, 14, itemId, 0);
        app->subAppId = 0;
        app->nextState = 6;
        return MailboxApp_StartFadeOut(app, 1);
    case 3: // CANCEL
    case -2:
        MailboxApp_CloseMailMenu(app);
        MailboxApp_ClearMessageBox(app);
        BgCellManager_EraseCell(app->data->cellManager, MAILBOX_CELL_SELECTED);
        MailboxAppData_SetListDimmed(app->data, FALSE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 2, TRUE);
        return 9;
    case -1:
    default:
        return 14;
    }
}

int MailboxApp_State_ReadMail(MailboxApp *app) {
    if (MailboxApp_IsCloseMailInput() == TRUE) {
        PlaySE(SEQ_SE_DP_DECIDE);
        return MailboxApp_StartSpriteFlash(app, 3, 16);
    }
    return 15;
}

int MailboxApp_State_CloseMail(MailboxApp *app) {
    MailboxApp_HideMail(app);
    MailboxApp_MoveCursor(app, GridInputHandler_GetNextInput(app->data->gridInput));
    MailboxAppData_ShowCancelButton(app->data);
    BgCellManager_EraseCell(app->data->cellManager, MAILBOX_CELL_SELECTED);
    MailboxAppData_SetListDimmed(app->data, FALSE);
    return 9;
}

int MailboxApp_State_ConfirmErase(MailboxApp *app) {
    return MailboxApp_AskYesNo(app, 0);
}

int MailboxApp_State_PrintHoldMailPrompt(MailboxApp *app) {
    MailboxApp_PrintMessage(app, 2);
    return MailboxApp_WaitForText(app, FALSE, 19);
}

int MailboxApp_State_ConfirmHoldMail(MailboxApp *app) {
    return MailboxApp_AskYesNo(app, 1);
}

int ov103_021ED960(MailboxApp *app) {
    if (app->partyMenuAction != 6) {
        app->nextState = 21;
        return 0;
    }
    app->subAppId = 2;
    return 6;
}

int ov103_021ED97C(MailboxApp *app) {
    MailboxAppData_SetSpriteDrawFlag(app->data, 2, FALSE);
    MailboxAppData_SetListDimmed(app->data, TRUE);
    return MailboxApp_StartFadeIn(app, 22);
}

int MailboxApp_State_ShowMailToBagResult(MailboxApp *app) {
    MailboxApp_DrawMessageBox(app);
    return MailboxApp_PrintMailToBagResult(app);
}

int ov103_021ED9AC(MailboxApp *app) {
    if (app->useMailResult == 0) {
        app->nextState = 21;
        return 0;
    }
    MailboxApp_CreatePartyMenuArgs(app, 11, app->itemId, app->partySlot);
    app->subAppId = 3;
    return 6;
}

int MailboxApp_ReturnToMailList(MailboxApp *app) {
    MailboxApp_ClearMessageBox(app);
    BgCellManager_EraseCell(app->data->cellManager, MAILBOX_CELL_SELECTED);
    MailboxApp_MoveCursor(app, GridInputHandler_GetNextInput(app->data->gridInput));
    MailboxAppData_SetSpriteDrawFlag(app->data, 2, TRUE);
    MailboxAppData_SetListDimmed(app->data, FALSE);
    return 9;
}

int MailboxApp_StartFadeIn(MailboxApp *app, int nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_157);
    app->data->stateAfterFade = nextState;
    return 2;
}

int MailboxApp_StartFadeOut(MailboxApp *app, int nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_157);
    app->data->stateAfterFade = nextState;
    return 2;
}

int MailboxApp_WaitForText(MailboxApp *app, u8 waitForInput, int nextState) {
    app->data->waitForInput = waitForInput;
    app->nextState = nextState;
    return 3;
}

int MailboxApp_AskYesNo(MailboxApp *app, u32 question) {
    app->data->yesNoQuestion = question;
    MailboxApp_ShowYesNoPrompt(app);
    return 4;
}

int MailboxApp_StartCellFlash(MailboxApp *app, int cell, int nextState) {
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
    return 5;
}

int MailboxApp_StartSpriteFlash(MailboxApp *app, int sprite, int nextState) {
    MailboxAppPaletteFlash *flash = &app->data->flash;

    flash->isBg = FALSE;
    flash->target = (u8)sprite;
    flash->firstPalette = 1;
    flash->secondPalette = 0;
    flash->state = 0;
    flash->timer = 0;
    app->nextState = nextState;
    return 5;
}

void MailboxAppData_SetListDimmed(MailboxAppData *data, BOOL enable) {
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

void MailboxApp_ShowMail(MailboxApp *app) {
    MailboxApp_LoadStationeryGfx(app);
    MailboxApp_PrintMail(app);
    MailboxApp_ShowMailMonIcons(app);
}

void MailboxApp_HideMail(MailboxApp *app) {
    MailboxAppData_SetSpriteDrawFlag(app->data, 4, FALSE);
    MailboxAppData_SetSpriteDrawFlag(app->data, 5, FALSE);
    MailboxAppData_SetSpriteDrawFlag(app->data, 6, FALSE);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(app->data->bgConfig, GF_BG_LYR_SUB_1);
}

void MailboxApp_PutMailInBag(MailboxApp *app) {
    Bag *bag = Save_Bag_Get(app->args->saveData);

    app->itemId = MailToItemId(Mail_GetType(app->data->mail[app->selectedMail]));
    if (Bag_AddItem(bag, app->itemId, 1, HEAP_ID_157) == TRUE) {
        app->bagFull = FALSE;
    } else {
        app->bagFull = TRUE;
    }
}

void MailboxApp_DeleteSelectedMail(MailboxApp *app) {
    Mailbox_DeleteSlotI(app->mailbox->msgs, 0, app->selectedMail);
}

void MailboxApp_CreatePartyMenuArgs(MailboxApp *app, u8 context, u16 itemId, u8 partySlot) {
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

int MailboxApp_StartPartyMenu(MailboxApp *app) {
    app->subApp = OverlayManager_New(&gOverlayTemplate_PartyMenu, app->subAppArgs, HEAP_ID_156);
    return 0;
}

int MailboxApp_FinishGiveMail(MailboxApp *app) {
    PartyMenuArgs *args = app->subAppArgs;
    u32 partySlot = args->partySlot;

    if (partySlot <= 5) {
        Mailbox_MoveMessageToMon(app->mailbox->msgs, app->selectedMail, Party_GetMonByIndex(SaveArray_Party_Get(app->args->saveData), partySlot), HEAP_ID_156);
    }
    Heap_Free(app->subAppArgs);
    return 8;
}

int MailboxApp_FinishSelectMonForMail(MailboxApp *app) {
    PartyMenuArgs *args = app->subAppArgs;

    app->partySlot = args->partySlot;
    app->partyMenuAction = args->selectedAction;
    Heap_Free(app->subAppArgs);
    return 0;
}

int MailboxApp_FinishPartyMenu(MailboxApp *app) {
    Heap_Free(app->subAppArgs);
    return 8;
}

int MailboxApp_StartUseMail(MailboxApp *app) {
    UseMailArgs *args = UseMailArgs_NewCompose(app->args->saveData, 2, app->partySlot, ItemToMailId(app->itemId), HEAP_ID_156);

    args->menuInputStatePtr = app->args->menuInputStatePtr;
    app->subAppArgs = args;
    app->subApp = OverlayManager_New(&gMailboxUseMailOverlayTemplate, app->subAppArgs, HEAP_ID_156);
    return 0;
}

int MailboxApp_FinishUseMail(MailboxApp *app) {
    app->useMailResult = UseMailArgs_WasMailWritten(app->subAppArgs);
    if (app->useMailResult == 1) {
        Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(app->args->saveData), app->partySlot);
        UseMailArgs_GiveMailToMon(app->subAppArgs, mon);
        Bag_TakeItem(Save_Bag_Get(app->args->saveData), app->itemId, 1, HEAP_ID_156);
    }
    UseMailArgs_Free(app->subAppArgs);
    return 0;
}

int MailboxApp_EraseMail(MailboxApp *app) {
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
    return MailboxApp_WaitForText(app, TRUE, 18);
}

int MailboxApp_SelectMonToHoldMail(MailboxApp *app) {
    MailboxApp_CreatePartyMenuArgs(app, 9, app->itemId, 0);
    app->subAppId = 1;
    app->nextState = 6;
    return MailboxApp_StartFadeOut(app, 1);
}

int MailboxApp_PrintMailToBagResult(MailboxApp *app) {
    if (app->bagFull == FALSE) {
        MailboxApp_PrintMessage(app, 3);
    } else {
        MailboxApp_PrintMessage(app, 4);
    }
    MailboxApp_WaitForText(app, TRUE, 24);
}

void MailboxApp_CreateSprites(MailboxApp *app) {
    u32 i;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    MailboxAppData_OpenMonIconNarc(app->data);
    MailboxAppData_InitSpriteSystem(app->data);
    MailboxAppData_LoadSpriteResources(app->data);
    MailboxAppData_LoadMonIconResources(app->data);
    for (i = 0; i < MAILBOX_NUM_SPRITES; i++) {
        app->data->sprites[i] = MailboxAppData_NewSprite(app->data, &gMailboxSpriteTemplates[i]);
    }
    if (app->data->mailCount <= MAILBOX_MAIL_PER_PAGE) {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, FALSE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, FALSE);
    } else if (app->page == 0) {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, FALSE);
    } else {
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, FALSE);
    }
    MailboxAppData_SetSpriteDrawFlag(app->data, 4, FALSE);
    MailboxAppData_SetSpriteDrawFlag(app->data, 5, FALSE);
    MailboxAppData_SetSpriteDrawFlag(app->data, 6, FALSE);
}

void MailboxApp_DeleteSprites(MailboxApp *app) {
    MailboxAppData_DeleteAllSprites(app->data);
    MailboxAppData_FreeSpriteSystem(app->data);
    MailboxAppData_CloseMonIconNarc(app->data);
}

void MailboxAppData_TickSprites(MailboxAppData *data) {
    u32 i;

    for (i = 0; i < MAILBOX_NUM_SPRITES; i++) {
        if (data->sprites[i] != NULL) {
            ManagedSprite_TickFrame(data->sprites[i]);
        }
    }
}

void MailboxAppData_InitSpriteSystem(MailboxAppData *data) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion resourceCounts;

    data->spriteSystem = SpriteSystem_Alloc(HEAP_ID_157);
    data->spriteManager = SpriteManager_New(data->spriteSystem);
    oamManagerParam = gMailboxAppDataOamManagerParam;
    oamCharTransferParam = gMailboxAppDataOamCharTransferParam;
    SpriteSystem_Init(data->spriteSystem, &oamManagerParam, &oamCharTransferParam, 0x20);
    resourceCounts = gMailboxAppDataSpriteResourceCounts;
    SpriteSystem_InitSprites(data->spriteSystem, data->spriteManager, MAILBOX_NUM_SPRITES);
    SpriteSystem_InitManagerWithCapacities(data->spriteSystem, data->spriteManager, &resourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(data->spriteSystem), 0, FX32_CONST(512));
}

void MailboxAppData_FreeSpriteSystem(MailboxAppData *data) {
    SpriteSystem_FreeResourcesAndManager(data->spriteSystem, data->spriteManager);
    SpriteSystem_Free(data->spriteSystem);
}

ManagedSprite *MailboxAppData_NewSprite(MailboxAppData *data, const ManagedSpriteTemplate *template) {
    if (template->vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        return SpriteSystem_NewSprite(data->spriteSystem, data->spriteManager, template);
    }
    return SpriteSystem_NewSpriteWithYOffset(data->spriteSystem, data->spriteManager, template, FX32_CONST(512));
}

void MailboxAppData_DeleteSprite(MailboxAppData *data, u32 index) {
    if (data->sprites[index] != NULL) {
        Sprite_DeleteAndFreeResources(data->sprites[index]);
        data->sprites[index] = NULL;
    }
}

void MailboxAppData_DeleteAllSprites(MailboxAppData *data) {
    u32 i;

    for (i = 0; i < MAILBOX_NUM_SPRITES; i++) {
        MailboxAppData_DeleteSprite(data, i);
    }
}

void MailboxAppData_SetSpriteAnim(MailboxAppData *data, u32 index, u32 anim) {
    ManagedSprite_SetAnimationFrame(data->sprites[index], 0);
    ManagedSprite_SetAnim(data->sprites[index], anim);
}

void MailboxAppData_SetSpriteDrawFlag(MailboxAppData *data, u32 index, BOOL draw) {
    if (draw == TRUE) {
        ManagedSprite_SetDrawFlag(data->sprites[index], TRUE);
    } else {
        ManagedSprite_SetDrawFlag(data->sprites[index], FALSE);
    }
}

void MailboxAppData_SetSpritePosition(MailboxAppData *data, u32 index, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(data->sprites[index], x, y);
}

void MailboxAppData_SetSpriteTranslucent(MailboxAppData *data, u32 index, BOOL blend) {
    if (blend == TRUE) {
        ManagedSprite_SetOamMode(data->sprites[index], GX_OAM_MODE_XLU);
    } else {
        ManagedSprite_SetOamMode(data->sprites[index], GX_OAM_MODE_NORMAL);
    }
}

void MailboxAppData_OpenMonIconNarc(MailboxAppData *data) {
    data->monIconNarc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_157);
}

void MailboxAppData_CloseMonIconNarc(MailboxAppData *data) {
    NARC_Delete(data->monIconNarc);
}

void MailboxAppData_LoadMonIconResources(MailboxAppData *data) {
    u32 resId;

    for (resId = 0x37F0; resId <= 0x37F2; resId++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(data->spriteSystem, data->spriteManager, data->monIconNarc, GetMonIconNaixEx(SPECIES_NONE, FALSE, 0), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, resId);
    }
    SpriteSystem_LoadPlttResObjFromOpenNarc(data->spriteSystem, data->spriteManager, data->monIconNarc, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0x37F0);
    SpriteSystem_LoadCellResObjFromOpenNarc(data->spriteSystem, data->spriteManager, data->monIconNarc, GetMonIconStaticCellNaix(), FALSE, 0x37F0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(data->spriteSystem, data->spriteManager, data->monIconNarc, GetMonIconCell32KNaix(), FALSE, 0x37F0);
}

// Shows the icons of the (up to three) Pokemon the mail's author attached.
void MailboxApp_ShowMailMonIcons(MailboxApp *app) {
    union {
        u16 raw;
        struct {
            u16 species : 12;
            u16 palette : 4;
        };
    } icon;
    u32 i;
    u32 offset;
    register Mail *mail = app->data->mail[app->selectedMail];
    NNSG2dCharacterData *charData;
    void *charFile;
    u32 imageLoc;
    u32 iconId;

    for (i = 0, offset = 0; i < 3; i++, offset += 4) {
        icon.raw = Mail_GetMonIconData(mail, (u8)i, 2, Mail_GetFormFlags(mail));
        if (icon.raw == 0xFFFF || (iconId = icon.species) == 7) {
            MailboxAppData_SetSpriteDrawFlag(app->data, i + 4, FALSE);
        } else {
            imageLoc = NNS_G2dGetImageLocation(Sprite_GetImageProxy((*(ManagedSprite **)((u8 *)app->data->sprites + 0x10 + offset))->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
            charFile = GfGfxLoader_GetCharDataFromOpenNarc(app->data->monIconNarc, iconId, FALSE, &charData, HEAP_ID_157);
            DC_FlushRange(charData->pRawData, 0x200);
            GXS_LoadOBJ(charData->pRawData, imageLoc, 0x200);
            Heap_Free(charFile);
            ManagedSprite_SetPaletteOverride(*(ManagedSprite **)((u8 *)app->data->sprites + 0x10 + offset), icon.palette);
            MailboxAppData_SetSpriteDrawFlag(app->data, i + 4, TRUE);
        }
    }
}

void MailboxAppData_LoadSpriteResources(MailboxAppData *data) {
    NARC *narc = NARC_New(NARC_a_2_5_0, HEAP_ID_157);

    SpriteSystem_LoadCharResObjFromOpenNarc(data->spriteSystem, data->spriteManager, narc, 7, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0x37F3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(data->spriteSystem, data->spriteManager, narc, 10, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x37F1);
    SpriteSystem_LoadCellResObjFromOpenNarc(data->spriteSystem, data->spriteManager, narc, 8, TRUE, 0x37F1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(data->spriteSystem, data->spriteManager, narc, 9, TRUE, 0x37F1);
    NARC_Delete(narc);
}

void MailboxAppData_MoveCursorToCloseButton(MailboxAppData *data) {
    MailboxAppData_SetSpritePosition(data, 2, 224, 176);
    MailboxAppData_SetSpriteAnim(data, 2, 4);
}

void MailboxApp_AddWindows(MailboxApp *app) {
    u32 i;

    FontID_Alloc(4, HEAP_ID_157);
    for (i = 0; i < MAILBOX_NUM_WINDOWS; i++) {
        AddWindow(app->data->bgConfig, &app->data->windows[i], &gMailboxWindowTemplates[i]);
    }
}

void MailboxApp_RemoveWindows(MailboxApp *app) {
    u32 i;

    for (i = 0; i < MAILBOX_NUM_WINDOWS; i++) {
        RemoveWindow(&app->data->windows[i]);
    }
    FontID_Release(4);
}

// align: 0 left, 1 right, 2 centred on x
void MailboxApp_PrintString(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFF, color, NULL);
}

void MailboxAppData_PrintMsg(MailboxAppData *data, MsgData *msgData, u32 windowId, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    MailboxApp_PrintString(&data->windows[windowId], string, x, y, fontId, color, align);
    String_Delete(string);
}

// Draws a name plate (author name on the plate tiles) into a window and puts it into a cell.
void MailboxApp_DrawNameplate(MailboxApp *app, u32 cell, u32 windowId, u32 mailIdx) {
    Window *window = &app->data->windows[windowId];
    Mail *mail = app->data->mail[mailIdx];
    u32 i;
    u32 x;

    CopyU16ArrayToString(app->data->string, Mail_GetAuthorNamePtr(mail));
    x = 0;
    for (i = 0; i < 8; i++) {
        BlitBitmapRectToWindow(window, app->data->nameplateTileLeft, 0, 0, 8, 8, (u16)x, 0, 8, 8);
        BlitBitmapRectToWindow(window, app->data->nameplateTileRight, 0, 0, 8, 8, (u16)x, 8, 8, 8);
        x += 8;
    }
    if (Mail_GetAuthorGender(mail) == 0) {
        MailboxApp_PrintString(window, app->data->string, 0, 0, 4, 0x30200, 0);
    } else {
        MailboxApp_PrintString(window, app->data->string, 0, 0, 4, 0x50400, 0);
    }
    CopyWindowPixelsToVram_TextMode(window);
    BgCellManager_PutWindowTiles(app->data->cellManager, cell, window);
}

void MailboxApp_PrintTitleAndButtons(MailboxApp *app) {
    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_TITLE], 0);
    MailboxAppData_PrintMsg(app->data, app->data->msgData, MAILBOX_WIN_TITLE, msg_0232_00000, 44, 0, 0, 0x10200, 2);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_TITLE]);
    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_CANCEL], 0);
    MailboxAppData_PrintMsg(app->data, app->data->msgData, MAILBOX_WIN_CANCEL, msg_0232_00003, 24, 0, 4, 0xF0100, 2);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_CANCEL]);
    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_CLOSE], 0);
    MailboxAppData_PrintMsg(app->data, app->data->msgData, MAILBOX_WIN_CLOSE, msg_0232_00004, 24, 0, 4, 0xF0100, 2);
}

void MailboxAppData_ShowCloseButton(MailboxAppData *data) {
    ClearWindowTilemapAndScheduleTransfer(&data->windows[MAILBOX_WIN_CANCEL]);
    ScheduleWindowCopyToVram(&data->windows[MAILBOX_WIN_CLOSE]);
}

void MailboxAppData_ShowCancelButton(MailboxAppData *data) {
    ClearWindowTilemapAndScheduleTransfer(&data->windows[MAILBOX_WIN_CLOSE]);
    ScheduleWindowCopyToVram(&data->windows[MAILBOX_WIN_CANCEL]);
}

// "page / last page"
void MailboxApp_PrintPageNumber(MailboxApp *app) {
    String *template;
    u16 slashWidth;
    u16 pageWidth;

    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_PAGE], 0);
    ReadMsgDataIntoString(app->data->msgData, msg_0232_00002, app->data->string);
    MailboxApp_PrintString(&app->data->windows[MAILBOX_WIN_PAGE], app->data->string, 20, 0, 0, 0x10200, 2);
    slashWidth = FontID_String_GetWidth(0, app->data->string, 0);
    BufferIntegerAsString(app->data->msgFormat, 0, app->page + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    template = NewString_ReadMsgData(app->data->msgData, msg_0232_00001);
    StringExpandPlaceholders(app->data->msgFormat, app->data->string, template);
    String_Delete(template);
    pageWidth = FontID_String_GetWidth(0, app->data->string, 0);
    MailboxApp_PrintString(&app->data->windows[MAILBOX_WIN_PAGE], app->data->string, 20 - (slashWidth / 2) - pageWidth, 0, 0, 0x10200, 0);
    BufferIntegerAsString(app->data->msgFormat, 0, app->data->lastPage + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    template = NewString_ReadMsgData(app->data->msgData, msg_0232_00001);
    StringExpandPlaceholders(app->data->msgFormat, app->data->string, template);
    String_Delete(template);
    MailboxApp_PrintString(&app->data->windows[MAILBOX_WIN_PAGE], app->data->string, slashWidth / 2 + 20, 0, 0, 0x10200, 0);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_PAGE]);
}

void MailboxApp_BufferAuthorName(MailboxApp *app, u32 unused) {
    Mail *mail = app->data->mail[app->selectedMail];
    String *name = String_New(8, HEAP_ID_157);

    CopyU16ArrayToString(name, Mail_GetAuthorNamePtr(mail));
    BufferString(app->data->msgFormat, 0, name, Mail_GetAuthorGender(mail), 1, 2);
    String_Delete(name);
}

static inline u32 MailboxAppData_GetMessageWindowOffset(void) {
    return offsetof(MailboxAppData, windows[MAILBOX_WIN_MESSAGE]);
}

#pragma force_active on
void MailboxApp_PrintMessageBox(MailboxApp *app) {
    // Matching quirk: the window, the string and the printer id are addressed relative to the message
    // window's offset, which must not be folded into the member offsets at compile time.
    MailboxAppData *data = app->data;
    u32 offset = MailboxAppData_GetMessageWindowOffset();

    FillWindowPixelBuffer((Window *)((u32)data + offset), 15);
    *(u8 *)((u32)app->data + offset + (offsetof(MailboxAppData, printerId) - offsetof(MailboxAppData, windows[MAILBOX_WIN_MESSAGE]))) = AddTextPrinterParameterizedWithColor((Window *)((u32)data + offset), 1, *(String **)((u32)app->data + offset + (offsetof(MailboxAppData, string) - offsetof(MailboxAppData, windows[MAILBOX_WIN_MESSAGE]))), 0, 0, Options_GetTextFrameDelay(app->options), 0x1020F, NULL);
}
#pragma force_active reset

void MailboxApp_PrintMailPrompt(MailboxApp *app) {
    String *template;

    MailboxApp_BufferAuthorName(app, 0);
    template = NewString_ReadMsgData(app->data->msgData, msg_0232_00009);
    StringExpandPlaceholders(app->data->msgFormat, app->data->string, template);
    String_Delete(template);
    MailboxApp_PrintMessageBox(app);
}

void MailboxApp_DrawMessageBox(MailboxApp *app) {
    DrawFrameAndWindow2(&app->data->windows[MAILBOX_WIN_MESSAGE], TRUE, 0x3E2, 13);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_MESSAGE]);
}

void MailboxApp_ClearMessageBox(MailboxApp *app) {
    ClearFrameAndWindow2(&app->data->windows[MAILBOX_WIN_MESSAGE], TRUE);
    ScheduleBgTilemapBufferTransfer(app->data->bgConfig, GF_BG_LYR_MAIN_0);
}

void MailboxApp_PrintMessage(MailboxApp *app, u32 message) {
    switch (message) {
    case 0:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00010, app->data->string);
        break;
    case 1:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00011, app->data->string);
        break;
    case 2:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00012, app->data->string);
        break;
    case 3:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00013, app->data->string);
        break;
    case 4:
        ReadMsgDataIntoString(app->data->msgData, msg_0232_00014, app->data->string);
        break;
    }
    MailboxApp_PrintMessageBox(app);
}

void MailboxApp_PrintMail(MailboxApp *app) {
    u32 i;

    for (i = 0; i < 3; i++) {
        FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_MAIL_MSG + i], 0);
        MailboxApp_PrintMailLine(app, MAILBOX_WIN_MAIL_MSG + i, i);
        ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_MAIL_MSG + i]);
    }
    FillWindowPixelBuffer(&app->data->windows[MAILBOX_WIN_AUTHOR], 0);
    CopyU16ArrayToString(app->data->string, Mail_GetAuthorNamePtr(app->data->mail[app->selectedMail]));
    MailboxApp_PrintString(&app->data->windows[MAILBOX_WIN_AUTHOR], app->data->string, 0, 1, 1, 0x10200, 0);
    ScheduleWindowCopyToVram(&app->data->windows[MAILBOX_WIN_AUTHOR]);
}

void MailboxApp_PrintMailLine(MailboxApp *app, u32 windowId, u32 line) {
    MailMessage *message;
    String *string;
    u8 selectedMail = app->selectedMail;
    Mail *mail = app->data->mail[selectedMail];

    message = Mail_GetMessage(mail, (u8)line);
    if (MailMsg_IsInit(message)) {
        string = MailMsg_GetExpandedString(message, HEAP_ID_157);
        MailboxApp_PrintString(&app->data->windows[windowId], string, 0, 0, 1, 0x10200, 0);
        String_Delete(string);
    }
}

// 0 / 1: the page arrows were touched.
int MailboxApp_GetTouchedPageArrow(MailboxApp *app) {
    if (app->data->lastPage == 0) {
        return -1;
    }
    return TouchscreenHitbox_FindRectAtTouchNew(gMailboxPageArrowHitboxes);
}

void MailboxApp_CreateGridInput(MailboxApp *app) {
    u32 cursorPos;

    if (app->page > app->data->lastPage) {
        app->page = app->data->lastPage;
        cursorPos = 0;
    } else {
        cursorPos = app->cursorPos;
    }
    app->data->gridInput = GridInputHandler_Create(gMailboxGridHitboxes, gMailboxGridBoxes, &gMailboxGridCallbacks, app, TRUE, (u8)cursorPos, HEAP_ID_157);
    MailboxApp_MoveCursor(app, cursorPos);
}

void MailboxApp_FreeGridInput(MailboxApp *app) {
    GridInputHandler_Free(app->data->gridInput);
}

void MailboxApp_GridOnButton(void) {
}

void MailboxApp_GridOnSwitchToTouchMode(void) {
}

void MailboxApp_GridOnKeyMove(MailboxApp *app, u32 input) {
    MailboxApp_MoveCursor(app, input);
}

void MailboxApp_GridOnTouch(MailboxApp *app, u32 input) {
    MailboxApp_MoveCursor(app, input);
}

void MailboxApp_MoveCursor(MailboxApp *app, u32 input) {
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(app->data->gridInput, input);

    MailboxAppData_SetSpritePosition(app->data, 2, box->left, box->top);
    if (input == 10) {
        MailboxAppData_SetSpriteAnim(app->data, 2, 4);
    } else {
        MailboxAppData_SetSpriteAnim(app->data, 2, 5);
    }
}

// The mail being read is closed by touching the CLOSE button or pressing A or B.
BOOL MailboxApp_IsCloseMailInput(void) {
    if (TouchscreenHitbox_FindRectAtTouchNew(gMailboxCloseMailHitboxes) != -1) {
        return TRUE;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return TRUE;
    }
    return FALSE;
}
