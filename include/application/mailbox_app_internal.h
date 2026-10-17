#ifndef POKEHEARTGOLD_APPLICATION_MAILBOX_APP_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_MAILBOX_APP_INTERNAL_H

#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "grid_input_handler.h"
#include "list_menu_items.h"
#include "mail.h"
#include "mailbox_app.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"

enum {
    MAILBOX_STATE_SETUP,
    MAILBOX_STATE_TEARDOWN,
    MAILBOX_STATE_WAIT_FADE,
    MAILBOX_STATE_WAIT_TEXT,
    MAILBOX_STATE_YES_NO,
    MAILBOX_STATE_PALETTE_FLASH,
    MAILBOX_STATE_START_SUB_APP,
    MAILBOX_STATE_RUN_SUB_APP,
    MAILBOX_STATE_FADE_IN,
    MAILBOX_STATE_SELECT_MAIL,
    MAILBOX_STATE_QUIT,
    MAILBOX_STATE_CHANGE_PAGE,
    MAILBOX_STATE_SHOW_MAIL_PROMPT,
    MAILBOX_STATE_OPEN_MAIL_MENU,
    MAILBOX_STATE_MAIL_MENU,
    MAILBOX_STATE_READ_MAIL,
    MAILBOX_STATE_CLOSE_MAIL,
    MAILBOX_STATE_CONFIRM_ERASE,
    MAILBOX_STATE_PRINT_HOLD_MAIL_PROMPT,
    MAILBOX_STATE_CONFIRM_HOLD_MAIL,
    MAILBOX_STATE_SELECT_MON_FOR_MAIL_DONE,
    MAILBOX_STATE_FADE_IN_DIMMED_LIST,
    MAILBOX_STATE_SHOW_MAIL_TO_BAG_RESULT,
    MAILBOX_STATE_USE_MAIL_DONE,
    MAILBOX_STATE_RETURN_TO_MAIL_LIST,
};

#define MAILBOX_MAIL_PER_PAGE 10
#define MAILBOX_NO_MAIL       0xFF
#define MAILBOX_NUM_WINDOWS   30
#define MAILBOX_NUM_SPRITES   7

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
    Options *options;       // 0x00
    Mailbox *mailbox;       // 0x04
    MailboxAppArgs *args;   // 0x08
    MailboxAppData *data;   // 0x0C
    OverlayManager *subApp; // 0x10
    void *subAppArgs;       // 0x14
    u32 subAppId;           // 0x18: index into sMailboxSubApps
    u16 page;               // 0x1C
    u8 cursorPos;           // 0x1E
    u8 selectedMail;        // 0x1F
    u8 partyMenuAction;     // 0x20
    u8 partySlot : 7;       // 0x21
    u8 bagFull : 1;         // 0x21: the erased mail could not be put back in the bag
    u16 itemId;             // 0x22
    u32 useMailResult;      // 0x24
    int nextState;          // 0x28
} MailboxApp;               // size: 0x2C

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

BOOL MailboxApp_RunState(MailboxApp *app, int *state);

void MailboxApp_CreateSprites(MailboxApp *app);
void MailboxApp_DeleteSprites(MailboxApp *app);
void MailboxAppData_TickSprites(MailboxAppData *data);
void MailboxAppData_SetSpriteAnim(MailboxAppData *data, u32 index, u32 anim);
void MailboxAppData_SetSpriteDrawFlag(MailboxAppData *data, u32 index, BOOL draw);
void MailboxAppData_SetSpritePosition(MailboxAppData *data, u32 index, s16 x, s16 y);
void MailboxAppData_SetSpriteTranslucent(MailboxAppData *data, u32 index, BOOL blend);
void MailboxApp_ShowMailMonIcons(MailboxApp *app);
void MailboxAppData_MoveCursorToCloseButton(MailboxAppData *data);

void MailboxApp_AddWindows(MailboxApp *app);
void MailboxApp_RemoveWindows(MailboxApp *app);
void MailboxApp_DrawNameplate(MailboxApp *app, u32 cell, u32 windowId, u32 mailIdx);
void MailboxApp_PrintTitleAndButtons(MailboxApp *app);
void MailboxAppData_ShowCloseButton(MailboxAppData *data);
void MailboxAppData_ShowCancelButton(MailboxAppData *data);
void MailboxApp_PrintPageNumber(MailboxApp *app);
void MailboxApp_PrintMailPrompt(MailboxApp *app);
void MailboxApp_DrawMessageBox(MailboxApp *app);
void MailboxApp_ClearMessageBox(MailboxApp *app);
void MailboxApp_PrintMessage(MailboxApp *app, u32 message);
void MailboxApp_PrintMail(MailboxApp *app);

int MailboxApp_GetTouchedPageArrow(MailboxApp *app);
void MailboxApp_CreateGridInput(MailboxApp *app);
void MailboxApp_FreeGridInput(MailboxApp *app);
void MailboxApp_MoveCursor(MailboxApp *app, u32 input);
BOOL MailboxApp_IsCloseMailInput(void);

#endif // POKEHEARTGOLD_APPLICATION_MAILBOX_APP_INTERNAL_H
