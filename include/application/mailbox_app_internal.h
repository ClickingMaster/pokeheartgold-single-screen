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
    u8 isBg : 1;   // 0 = flash a sprite, 1 = flash a cell's tilemap rectangle
    u8 target : 7; // sprite index or bg layer
    u8 firstPalette : 4;
    u8 secondPalette : 4;
    u8 state;
    u8 timer;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} MailboxAppPaletteFlash;

typedef struct MailboxAppData {
    BgConfig *bgConfig;
    BgCellManager *cellManager;
    u8 nameplateTileLeft[0x20];
    u8 nameplateTileRight[0x20];
    Window windows[MAILBOX_NUM_WINDOWS];
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    u8 printerId;
    u8 waitForInput;
    u8 padding_236[2];
    YesNoPrompt *yesNoPrompt;
    u32 yesNoQuestion;
    TouchscreenListMenuSpawner *listMenuSpawner;
    TouchscreenListMenu *listMenu;
    ListMenuItem *listMenuItems;
    u8 padding_24C[4];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[MAILBOX_NUM_SPRITES]; // 0/1 page arrows, 2 cursor, 4-6 icons
    NARC *monIconNarc;
    GridInputHandler *gridInput;
    Mail *mail[MAILBOX_MSG_COUNT];
    u8 mailOrder[MAILBOX_MSG_COUNT]; // mailbox slot of each listed mail
    u16 mailCount;
    u16 lastPage;
    MailboxAppPaletteFlash flash;
    int stateAfterFade;
} MailboxAppData;

typedef struct MailboxApp {
    Options *options;
    Mailbox *mailbox;
    MailboxAppArgs *args;
    MailboxAppData *data;
    OverlayManager *subApp;
    void *subAppArgs;
    u32 subAppId; // index into sMailboxSubApps
    u16 page;
    u8 cursorPos;
    u8 selectedMail;
    u8 partyMenuAction;
    u8 partySlot : 7;
    u8 bagFull : 1; // the erased mail could not be put back in the bag
    u16 itemId;
    u32 useMailResult;
    int nextState;
} MailboxApp;

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
