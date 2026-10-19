#ifndef POKEHEARTGOLD_APPLICATION_PC_BOX_PC_BOX_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_PC_BOX_PC_BOX_INTERNAL_H

#include "bag_types_def.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "grid_input_handler.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "overlay_14.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "yes_no_prompt.h"

BOOL BgCellManager_UpdateCellMove(BgCellManager *manager, int index);
u8 BgCellManager_GetCellBgId(BgCellManager *manager, int index);

TextOBJ *FontOAM_Create(TextOBJTemplate *template);
void FontOAM_Delete(TextOBJ *object);
int FontOAM_GetCharSize(Window *window, NNS_G2D_VRAM_TYPE vram, int heapId);
void TextOBJ_SetPriority(TextOBJ *object, u8 priority);
void TextOBJ_SetPalOffset(TextOBJ *object, u8 value);
void TextOBJ_SetPalOffsetRespectVramOffset(TextOBJ *object, int value);

void DrawFrame2AroundWindow(Window *window, u32 baseTile, u32 paletteNum);

// Launch modes (PCBoxArgs.mode).
#define PC_BOX_MODE_DEPOSIT      0
#define PC_BOX_MODE_WITHDRAW     1
#define PC_BOX_MODE_MOVE_POKEMON 2
#define PC_BOX_MODE_MOVE_ITEMS   3

// Slots: 0-29 are the active box, 30-35 the party.
#define PC_BOX_SLOT_PARTY_START 30
#define PC_BOX_SLOT_COUNT       36
#define PC_BOX_SLOT_NONE        0xFF

// sPCBoxStateTable: state N runs PCBox_State_<Name>
enum PCBoxState {
    PC_BOX_STATE_INIT,
    PC_BOX_STATE_TEARDOWN,
    PC_BOX_STATE_WAIT_FADE,
    PC_BOX_STATE_WAIT_PALETTE_BUFFERS,
    PC_BOX_STATE_DELAY,
    PC_BOX_STATE_WAIT_UPDATE_FUNC,
    PC_BOX_STATE_WAIT_BUTTON,
    PC_BOX_STATE_HANDLE_YES_NO,
    PC_BOX_STATE_WAIT_PALETTE_FLASH,
    PC_BOX_STATE_START_SUB_APP,
    PC_BOX_STATE_RUN_SUB_APP,
    PC_BOX_STATE_START,
    PC_BOX_STATE_MOVE_MODE_INPUT,
    PC_BOX_STATE_APPLY_MON_MOVE,
    PC_BOX_STATE_RETURN_TO_ACTION_MENU,
    PC_BOX_STATE_RETURN_FROM_SUMMARY,
    PC_BOX_STATE_RETURN_FROM_NAMING_SCREEN,
    PC_BOX_STATE_GIVE_ITEM_FROM_BAG,
    PC_BOX_STATE_PRINT_ITEM_HELD,
    PC_BOX_STATE_MARKING_CHOSEN,
    PC_BOX_STATE_OPEN_MARKING_MENU,
    PC_BOX_STATE_SETUP_MARKING_MENU_INPUT,
    PC_BOX_STATE_MARKING_MENU_INPUT,
    PC_BOX_STATE_FINISH_CLOSE_MARKING_MENU,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_MARKING_MENU,
    PC_BOX_STATE_TRY_RELEASE,
    PC_BOX_STATE_RELEASE_MOVE_CHECK,
    PC_BOX_STATE_PRINT_RELEASED,
    PC_BOX_STATE_PRINT_BYE_BYE,
    PC_BOX_STATE_REMOVE_RELEASED_MON,
    PC_BOX_STATE_RELEASE_CAME_BACK,
    PC_BOX_STATE_PRINT_WORRIED,
    PC_BOX_STATE_CANCEL_RELEASE,
    PC_BOX_STATE_END_PARTY_RELEASE_SHIFT,
    PC_BOX_STATE_OPEN_PARTY_PANEL,
    PC_BOX_STATE_SETUP_PARTY_PANEL_INPUT,
    PC_BOX_STATE_MOVE_MODE_PARTY_INPUT,
    PC_BOX_STATE_SLIDE_OUT_PARTY_PANEL,
    PC_BOX_STATE_PARTY_PANEL_CLOSED,
    PC_BOX_STATE_END_MOVE_MODE_PARTY_DRAG,
    PC_BOX_STATE_SETUP_BOX_TRAY_INPUT,
    PC_BOX_STATE_BOX_TRAY_INPUT,
    PC_BOX_STATE_END_BOX_TRAY_DRAG,
    PC_BOX_STATE_END_TRAY_HOLDING_DROP,
    PC_BOX_STATE_RESUME_BOX_TRAY_INPUT,
    PC_BOX_STATE_OPEN_TRAY_BESIDE_PARTY,
    PC_BOX_STATE_SETUP_PARTY_INPUT_AFTER_TRAY,
    PC_BOX_STATE_OPEN_TRAY_FROM_BOX,
    PC_BOX_STATE_SLIDE_IN_TRAY_PARTY_PANEL,
    PC_BOX_STATE_SLIDE_OUT_TRAY,
    PC_BOX_STATE_TRAY_CLOSED,
    PC_BOX_STATE_RETURN_TO_BOX_FROM_TRAY,
    PC_BOX_STATE_HIDE_BUTTONS_FOR_BOX_MENU,
    PC_BOX_STATE_OPEN_BOX_MENU_TRAY,
    PC_BOX_STATE_SHOW_BOX_MENU_EXIT_BUTTON,
    PC_BOX_STATE_OPEN_BOX_MENU,
    PC_BOX_STATE_SETUP_BOX_MENU_INPUT,
    PC_BOX_STATE_CLOSE_BOX_MENU,
    PC_BOX_STATE_SLIDE_OUT_BOX_MENU,
    PC_BOX_STATE_CLOSE_BOX_MENU_TRAY,
    PC_BOX_STATE_BOX_MENU_CLOSED,
    PC_BOX_STATE_BOX_MENU_INPUT,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_BOX_MENU,
    PC_BOX_STATE_OPEN_WALLPAPER_MENU,
    PC_BOX_STATE_SHOW_WALLPAPER_MENU_TITLE,
    PC_BOX_STATE_SETUP_WALLPAPER_MENU_INPUT,
    PC_BOX_STATE_WALLPAPER_MENU_INPUT,
    PC_BOX_STATE_CLOSE_WALLPAPER_MENU,
    PC_BOX_STATE_SLIDE_OUT_WALLPAPER_LIST,
    PC_BOX_STATE_RETURN_TO_BOX_MENU,
    PC_BOX_STATE_APPLY_WALLPAPER,
    PC_BOX_STATE_WALLPAPER_FADE_OUT,
    PC_BOX_STATE_WALLPAPER_FADE_IN,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_WALLPAPER_MENU,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_MOVE_MODE,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_BOX_TRAY,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_MOVE_MODE_PARTY,
    PC_BOX_STATE_RESUME_BOX_INPUT,
    PC_BOX_STATE_RESUME_MOVE_MODE_PARTY_INPUT,
    PC_BOX_STATE_PRINT_EGG_CANT_HOLD_ITEM,
    PC_BOX_STATE_PRINT_MAIL_CANT_BE_TAKEN,
    PC_BOX_STATE_WITHDRAW_INPUT,
    PC_BOX_STATE_RETURN_TO_WITHDRAW_INPUT,
    PC_BOX_STATE_SLIDE_IN_WITHDRAW_PARTY,
    PC_BOX_STATE_START_WITHDRAW_MOVE,
    PC_BOX_STATE_END_WITHDRAW_MOVE,
    PC_BOX_STATE_WITHDRAW_PARTY_CLOSED,
    PC_BOX_STATE_END_WITHDRAW_DRAG,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_WITHDRAW,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_WITHDRAW_PROMPT,
    PC_BOX_STATE_RESUME_WITHDRAW_INPUT,
    PC_BOX_STATE_DEPOSIT_INPUT,
    PC_BOX_STATE_TRY_DEPOSIT,
    PC_BOX_STATE_RETURN_FROM_DEPOSIT_ERROR,
    PC_BOX_STATE_SHOW_DEPOSIT_BOX_SELECT,
    PC_BOX_STATE_SHOW_DEPOSIT_TITLE,
    PC_BOX_STATE_SETUP_DEPOSIT_BOX_INPUT,
    PC_BOX_STATE_DEPOSIT_BOX_INPUT,
    PC_BOX_STATE_CANCEL_DEPOSIT_BOX_SELECT,
    PC_BOX_STATE_HIDE_DEPOSIT_EXIT_BUTTON,
    PC_BOX_STATE_HIDE_DEPOSIT_BOX_SELECT,
    PC_BOX_STATE_DEPOSIT_BOX_SELECT_CLOSED,
    PC_BOX_STATE_CONFIRM_DEPOSIT_BOX,
    PC_BOX_STATE_CHECK_DEPOSIT_BOX,
    PC_BOX_STATE_SHIFT_PARTY_FOR_DEPOSIT,
    PC_BOX_STATE_SWITCH_BOX,
    PC_BOX_STATE_START_DEPOSIT_MOVE,
    PC_BOX_STATE_END_DEPOSIT_MOVE,
    PC_BOX_STATE_SHOW_DEPOSIT_RETURN_BUTTON,
    PC_BOX_STATE_RESET_DEPOSIT_INPUT,
    PC_BOX_STATE_END_DEPOSIT_DRAG,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_DEPOSIT,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_DEPOSIT_PROMPT,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_DEPOSIT_BOX,
    PC_BOX_STATE_RESUME_DEPOSIT_INPUT,
    PC_BOX_STATE_BOX_TRAY_HOLDING_INPUT,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_BOX_TRAY_HOLDING,
    PC_BOX_STATE_ITEM_MODE_INPUT,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_ITEM_MODE,
    PC_BOX_STATE_CLOSE_ITEM_MESSAGE,
    PC_BOX_STATE_ASK_PUT_AWAY_ITEM,
    PC_BOX_STATE_WAIT_PUT_AWAY_ITEM,
    PC_BOX_STATE_CANCEL_PUT_AWAY_ITEM,
    PC_BOX_STATE_END_ITEM_COMMAND,
    PC_BOX_STATE_ITEM_MODE_PRINT_EGG_CANT_HOLD_ITEM,
    PC_BOX_STATE_ITEM_MODE_PRINT_ITEM_HELD,
    PC_BOX_STATE_END_ITEM_MODE_BOX_DRAG,
    PC_BOX_STATE_SWAP_HELD_ITEMS,
    PC_BOX_STATE_DROP_SWAPPED_ITEM,
    PC_BOX_STATE_FINISH_ITEM_SWAP,
    PC_BOX_STATE_ITEM_MODE_BOX_TRAY_INPUT,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_ITEM_MODE_TRAY,
    PC_BOX_STATE_END_ITEM_TRAY_DRAG,
    PC_BOX_STATE_APPLY_ITEM_TRAY_DROP,
    PC_BOX_STATE_FINISH_ITEM_TRAY_DROP,
    PC_BOX_STATE_RESUME_ITEM_MODE_TRAY_INPUT,
    PC_BOX_STATE_ITEM_TRAY_HOLDING_INPUT,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_ITEM_TRAY_HOLDING,
    PC_BOX_STATE_APPLY_ITEM_TRAY_HOLDING_DROP,
    PC_BOX_STATE_ITEM_MODE_PARTY_INPUT,
    PC_BOX_STATE_END_CURSOR_SLIDE_TO_ITEM_MODE_PARTY,
    PC_BOX_STATE_END_ITEM_MODE_PARTY_DRAG,
    PC_BOX_STATE_SWAP_PARTY_HELD_ITEMS,
    PC_BOX_STATE_RESUME_ITEM_MODE_PARTY_INPUT,
    PC_BOX_STATE_DROP_SWAPPED_PARTY_ITEM,
    PC_BOX_STATE_FINISH_PARTY_ITEM_SWAP,
    PC_BOX_STATE_RESUME_ITEM_MODE_PARTY_AFTER_MAIL,
    PC_BOX_STATE_ASK_EXIT,
    PC_BOX_STATE_ASK_CONTINUE,
    PC_BOX_STATE_CHOOSE_PARTY,
    PC_BOX_STATE_CHOOSE_TRAY_TAB,
    PC_BOX_STATE_CHOOSE_SUMMARY,
    PC_BOX_STATE_CHOOSE_ITEM,
    PC_BOX_STATE_CHOOSE_MARKING,
    PC_BOX_STATE_CHOOSE_MARKING_DONE,
    PC_BOX_STATE_CHOOSE_RELEASE,
    PC_BOX_STATE_CLOSE_MOVE_MODE_ACTION_MENU,
    PC_BOX_STATE_PARTY_CHOOSE_TRAY,
    PC_BOX_STATE_CLOSE_MOVE_MODE_PARTY_ACTION_MENU,
    PC_BOX_STATE_CHOOSE_PARTY_PANEL_EXIT,
    PC_BOX_STATE_CHOOSE_TRAY_BOX,
    PC_BOX_STATE_CHOOSE_TRAY_EXIT,
    PC_BOX_STATE_BOX_MENU_SWITCH_BOX,
    PC_BOX_STATE_CHOOSE_WALLPAPER,
    PC_BOX_STATE_CHOOSE_RENAME,
    PC_BOX_STATE_SWITCH_BOX_FOR_WALLPAPER,
    PC_BOX_STATE_CHOOSE_WALLPAPER_MENU_EXIT,
    PC_BOX_STATE_CHOOSE_WITHDRAW,
    PC_BOX_STATE_CLOSE_WITHDRAW_ACTION_MENU,
    PC_BOX_STATE_CHOOSE_DEPOSIT,
    PC_BOX_STATE_CLOSE_DEPOSIT_ACTION_MENU,
    PC_BOX_STATE_ITEM_MODE_CHOOSE_PARTY,
    PC_BOX_STATE_ITEM_MODE_CHOOSE_TRAY_TAB,
    PC_BOX_STATE_CHOOSE_BAG_GIVE,
    PC_BOX_STATE_CLOSE_ITEM_MODE_ACTION_MENU,
    PC_BOX_STATE_ITEM_MODE_CHOOSE_TRAY_BOX,
    PC_BOX_STATE_ITEM_MODE_PARTY_CHOOSE_TRAY,
    PC_BOX_STATE_CLOSE_ITEM_MODE_PARTY_ACTION_MENU,
    PC_BOX_STATE_CHOOSE_MOVE_FROM_BOX,
};

#define PC_BOX_STATE_EXIT 0xB3

enum PCBoxMonMessage {
    PC_BOX_MON_MSG_WHICH_ONE,
    PC_BOX_MON_MSG_SELECTED,
    PC_BOX_MON_MSG_PARTY_FULL,
    PC_BOX_MON_MSG_DEPOSIT_WHERE,
    PC_BOX_MON_MSG_BOX_FULL,
    PC_BOX_MON_MSG_DETACH_CAPSULE,
    PC_BOX_MON_MSG_REMOVE_MAIL,
};

enum PCBoxReleaseMessage {
    PC_BOX_RELEASE_MSG_CONFIRM,
    PC_BOX_RELEASE_MSG_RELEASED,
    PC_BOX_RELEASE_MSG_BYE_BYE,
    PC_BOX_RELEASE_MSG_EGG,
    PC_BOX_RELEASE_MSG_CAME_BACK,
    PC_BOX_RELEASE_MSG_WORRIED,
    PC_BOX_RELEASE_MSG_LAST_MON,
};

enum PCBoxItemMessage {
    PC_BOX_ITEM_MSG_PUT_AWAY = 2,
    PC_BOX_ITEM_MSG_PLACED_IN_BAG,
    PC_BOX_ITEM_MSG_MAIL_CANT_BE_TAKEN,
    PC_BOX_ITEM_MSG_EGG_CANT_HOLD,
    PC_BOX_ITEM_MSG_BAG_FULL,
    PC_BOX_ITEM_MSG_MAIL_CANT_BE_MOVED,
};

typedef struct PCBoxWork PCBoxWork;

// Run each vblank by PCBox_VBlankCallback until it returns FALSE.
typedef BOOL (*PCBoxUpdateFunc)(PCBoxWork *work);

// Info about the mon in a slot, shown on the top screen (PCBox_CreateMonInfo).
typedef struct PCBoxMonInfo {
    BoxPokemon *boxMon;
    u16 species;
    u16 heldItem;
    u32 personality;
    u8 type1;
    u8 type2;
    u8 ability;
    u8 nature;
    u16 markings;
    u8 level : 7;
    u8 isEgg : 1;
    u8 gender : 7;
    u8 showGender : 1;
    u16 moves[4];
} PCBoxMonInfo;

// Releasing a mon: the scan of all boxes and the party for another mon that knows the mon's HM moves
// (PCBox_StartReleaseMoveCheck / PCBox_RunReleaseMoveCheck), then the shrink animation of its icon.
typedef struct PCBoxReleaseWork {
    ManagedSprite *sprite;
    u16 scanIndex; // box * 30 + slot, 540 + party slot
    u8 moveFlags;  // HM moves not found on another mon yet
    u8 scaleCounter;
    f32 scale;
} PCBoxReleaseWork;

// One mon icon moving between two slots (PCBox_AnimateMonMove).
typedef struct PCBoxMoveEntry {
    Pokemon *mon;
    u32 from;
    u32 to;
    u32 active;
    u32 dx;
    u32 dy;
    s16 sx;
    s16 sy;
    s16 px;
    s16 py;
} PCBoxMoveEntry;

typedef struct PCBoxMoveWork {
    PCBoxMoveEntry entries[7];
    u32 timer;
    u32 from;
    u32 to;
    u32 sel;
} PCBoxMoveWork;

// State of PCBox_AnimatePaletteFlash (filled by PCBox_FlashPanel).
typedef struct PCBoxPaletteFlash {
    u8 isBg : 1;
    u8 index : 7;
    u8 palA : 4;
    u8 palB : 4;
    u8 state;
    u8 timer;
    u8 x;
    u8 y;
    u8 w;
    u8 h;
} PCBoxPaletteFlash;

// Cursor slide between two grid input positions (PCBox_StartCursorSlide), in view->temp.
typedef struct PCBoxCursorSlide {
    u8 x;
    u8 y;
    u8 dx;
    u8 dy;
    u32 xNegative : 1;
    u32 yNegative : 1;
    u32 frames : 30;
} PCBoxCursorSlide;

// Held item icon moving to a slot (PCBox_StartHeldItemMove), in view->temp. Positions in 24.8 fixed point.
typedef struct PCBoxHeldItemMove {
    u16 toSlot;
    u16 fromSlot;
    int partyLayout;
    int x;
    int y;
    int dx;
    int dy;
    u32 xNegative : 1;
    u32 yNegative : 1;
    u32 frames : 30;
} PCBoxHeldItemMove;

// {msgId, kind} entries of the action menu (PCBox_DrawActionMenu).
typedef struct PCBoxMenuEntry {
    u16 msgId;
    u16 kind;
} PCBoxMenuEntry;

// Text drawn as sprites (the tray / wallpaper menu titles).
typedef struct PCBoxTextObj {
    TextOBJ *textObj;
    ObjCharSpaceReservation charTransfer;
} PCBoxTextObj;

// Graphics, text and input state of the app (heap 10, PCBoxWork.view).
typedef struct PCBoxView {
    SysTask *vblankTask;
    PCBoxUpdateFunc updateFunc;
    PCBoxUpdateFunc pendingUpdateFunc;
    void *temp;
    u16 subState;
    u16 counter;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    MessagePrinter *msgPrinter;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    GridInputHandler *gridInput;
    Window windows[0x2C];
    BgCellManager *panels;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[69];
    FontSystem *fontSystem;
    PCBoxTextObj textObjs[2];
    YesNoPrompt *yesNoPrompt;
    u16 yesNoHandler; // index into sPCBoxYesNoHandlers
    u16 unk_043A;
    u32 rowCursorPos;
    u32 fadeNextState;
    u32 delay;
    s8 wallpaperScroll;
    u8 wallpaperBuffer;
    u8 partyPanelLowered;
    u8 cursorHolding;
    u8 dropTarget;
    u8 wallpaperSelection;
    u8 infoWindowToggle : 4; // which of the two info window sets is drawn next
    u8 movesDrawn : 3;
    u8 restoreIconsOnMenu : 1;
    u8 unk_044F;
    NARC *personalNarc;
    NARC *iconNarc;
    u8 iconChars[30][0x200];
    u8 iconDraw[30];
    u8 iconPalettes[30];
    u8 slotIcons[PC_BOX_SLOT_COUNT];
    int touchX;
    int touchY;
    u32 markings;
    u32 showOutlineAfterMove;
    u8 boxThumbnails[18][0x400];
    u16 heldItem;
    u16 itemFromSlot;
    u32 dropCancelled;
    u16 monPicBuffer;
    u16 spriteDrawLock;
    PCBoxPaletteFlash paletteFlash;
    PCBoxReleaseWork *releaseWork;
} PCBoxView;

struct PCBoxWork {
    PCBoxArgs *args;
    PCStorage *storage;
    Party *party;
    Bag *bag;
    Options *options;
    OverlayManager *subApp;
    void *subAppArgs;
    u16 itemId;
    u8 subAppIndex;
    u8 activeBox;
    u8 unk_20;
    u8 selectedSlot;
    u8 partyLayout;
    u8 partyPanelOpen;
    u8 trayOpen;
    u8 trayBox;
    u8 summaryFromTray;
    u8 summaryFromHolding;
    u8 summaryCursorPos;
    u8 trayFromPartyPanel;
    u8 moveWithButtons;
    u8 moveFromSlot;
    u32 returnCursorPos;
    u32 nextState;
    PCBoxView *view;
};

void PCBox_VBlankCallback(SysTask *task, void *data);
void PCBox_SetUpdateFunc(PCBoxView *view, void *v);
void PCBox_SetPendingUpdateFunc(PCBoxView *view, void *v);
void PCBox_StartPendingUpdateFunc(PCBoxView *view);
void PCBox_SetGfxBanks(void);
void PCBox_InitBgLayers(PCBoxWork *work);
void PCBox_FreeBgLayers(PCBoxWork *work);
void PCBox_LoadBgGraphics(PCBoxWork *work);
void PCBox_CreateMsgResources(PCBoxWork *work);
void PCBox_DestroyMsgResources(PCBoxWork *work);
void PCBox_LoadWindowGraphics(PCBoxWork *p);
void PCBox_CreatePaletteData(PCBoxWork *work);
void PCBox_FreePaletteData(PCBoxWork *work);
void PCBox_SetBlendEnabled(int enabled);
void PCBox_CreateYesNoPrompt(PCBoxWork *work);
void PCBox_DestroyYesNoPrompt(PCBoxWork *work);
void PCBox_ShowYesNoPrompt(PCBoxWork *work, u32 cursorPos);
u32 PCBox_AnimatePaletteFlash(PCBoxWork *work);
void PCBox_SaveMenuInputState(PCBoxWork *work);
u32 PCBox_GetSlotMonData(PCBoxWork *work, u32 idx, u32 field, void *dest);
void PCBox_SetSlotMonData(PCBoxWork *work, u32 idx, u32 field, void *src);
BoxPokemon *PCBox_GetSlotBoxMon(PCBoxWork *work, u32 boxId, u32 idx);
void PCBox_DeleteSlotMon(PCBoxWork *work, u32 boxId, u32 idx);
void PCBox_ApplyMonMove(PCBoxWork *work);
u32 PCBox_PartyHasOtherUsableMon(PCBoxWork *work, u32 skip);
u32 PCBox_UpdateArceusGiratinaForm(BoxPokemon *boxMon);
void PCBox_RevertShayminFormOnMove(PCBoxWork *work, u32 a, u32 b);
u32 PCBox_AnimateMonMove(PCBoxWork *work);
u32 PCBox_AnimateSelectedSlotMoveEntry(PCBoxWork *work);
u32 PCBox_AnimateMoveEntries(PCBoxWork *work);
u32 PCBox_CanSwapHeldItems(PCBoxWork *work, u32 a, u32 b);
void PCBox_SetupMonMove(PCBoxWork *work, u32 a, u32 b);
void PCBox_SetupDepositMoveEntries(PCBoxWork *work, u32 a, u32 b);
void PCBox_SetupMonMoveToPartyEnd(PCBoxWork *work, u32 a, u32 b);
u32 PCBox_GetMoveEntryTarget(PCBoxWork *work, u32 v);
void PCBox_SetupPartyShiftAfterRelease(PCBoxWork *work, u32 a);
void PCBox_ApplyMoveEntryIcons(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_StartReleaseMoveCheck(PCBoxWork *work);
void PCBox_FreeReleaseMoveCheck(PCBoxWork *work);
u32 PCBox_RunReleaseMoveCheck(PCBoxWork *work);
PCBoxMonInfo *PCBox_CreateMonInfo(BoxPokemon *boxMon);
BOOL PCBox_ShowSlotMonInfo(PCBoxWork *work, u32 index);
void PCBox_RedrawSlotMonInfo(PCBoxWork *work, u32 index);
void PCBox_ClearMonInfo(PCBoxWork *work);
void PCBox_ClearMonInfoAndSelection(PCBoxWork *work);
void PCBox_LoadScrolledWallpaper(PCBoxWork *work, s32 member, u32 direction);
void PCBox_SwapWallpaper(PCBoxWork *work, s32 member);
u32 PCBox_GetWallpaperGfxIndex(PCBoxWork *work, u32 box);
u32 PCBox_GetTouchedBoxSlot(s16 x, s16 y);
u32 PCBox_GetTouchedPartySlot(s16 x, s16 y, const u8 *rect);
u32 PCBox_GetTouchedTrayBox(s16 x, s16 y);

void PCBox_StartHeldItemMove(PCBoxWork *work, int fromSlot, int toSlot, int partyLayout, int toItemIcon);
void PCBox_InitPanels(PCBoxWork *work);
void PCBox_FreePanels(PCBoxView *view);
void PCBox_DrawMarkingMenu(PCBoxWork *work);
void PCBox_ToggleMarking(PCBoxWork *work, int bit);
void PCBox_SlideInMarkingMenu(BgCellManager *panels);
void PCBox_SlideOutMarkingMenu(BgCellManager *panels);
void PCBox_PlacePartyPanelLeft(BgCellManager *panels);
void PCBox_PlacePartyPanelRight(BgCellManager *panels);
void PCBox_ClearPartyPanelTiles(PCBoxWork *work);
void PCBox_PlacePartyPanelLeftHidden(BgCellManager *panels);
void PCBox_PlacePartyPanelRightHidden(BgCellManager *panels);
void PCBox_SlideInPartyPanel(BgCellManager *panels);
void PCBox_SlideOutPartyPanel(BgCellManager *panels);
void PCBox_ShiftPartyPanelAndReturnButtonRight(BgCellManager *panels);
void PCBox_ShiftPartyPanelRight(BgCellManager *panels);
void PCBox_ShiftPartyPanelAndReturnButtonLeft(BgCellManager *panels);
void PCBox_ShiftPartyPanelLeft(BgCellManager *panels);
void PCBox_LoadPartyPanelTilemap(BgCellManager *panels, u32 index);
void PCBox_SlideInWallpaperList(BgCellManager *panels);
void PCBox_SlideOutWallpaperList(BgCellManager *panels);
void PCBox_ShowPartyButton(PCBoxWork *work);
void PCBox_SlideOutPartyButton(BgCellManager *p);
void PCBox_SlideInPartyButton(BgCellManager *p);
void PCBox_ShowTrayTab(PCBoxWork *work);
void PCBox_SlideOutTrayTab(BgCellManager *panels);
void PCBox_SlideInTrayTab(BgCellManager *panels);
void PCBox_ShowReturnButton(PCBoxWork *work);
void PCBox_ShowExitButton(PCBoxWork *work);
void PCBox_HideReturnButton(BgCellManager *panels);
void PCBox_SlideOutReturnButton(BgCellManager *panels);
void PCBox_SlideInReturnButton(BgCellManager *panels);
u32 PCBox_IsMenuExitButtonShown(BgCellManager *panels);
void PCBox_HidePartyButtonAndTrayTab(BgCellManager *panels);
void PCBox_ShowActionMenu(BgCellManager *panels);
void PCBox_ResetActionMenu(BgCellManager *panels);
void PCBox_SlideInActionMenu(BgCellManager *panels);
void PCBox_SlideOutActionMenu(BgCellManager *panels);
u32 PCBox_IsActionMenuOpen(BgCellManager *panels);
void PCBox_SlideInTitlePanel(BgCellManager *panels);
void PCBox_SlideOutTitlePanel(BgCellManager *panels);
void PCBox_SlideOutChangeBoxButton(BgCellManager *panels);
u32 PCBox_IsChangeBoxButtonShown(BgCellManager *panels);
void PCBox_ShowSummaryButton(BgCellManager *panels);
void PCBox_SlideInSummaryButton(BgCellManager *panels);
void PCBox_SlideOutSummaryButton(BgCellManager *panels);
u32 PCBox_IsSummaryButtonShown(BgCellManager *panels);
void PCBox_ShowBoxTray(PCBoxWork *work);
void PCBox_SlideInBoxTray(BgCellManager *panels);
void PCBox_SlideOutBoxTray(BgCellManager *panels);
void PCBox_ShowBoxTrayAndMenu(PCBoxWork *work);
void PCBox_SlideInBoxMenu(BgCellManager *panels);
void PCBox_SlideOutBoxMenu(BgCellManager *panels);
void PCBox_HideBottomButtons(PCBoxWork *work);
void PCBox_SlideInMovesPanel(PCBoxView *view);
void PCBox_SlideOutMovesPanel(PCBoxView *view);
void PCBox_ShowMovesPanel(PCBoxView *view);
void PCBox_HideMovesPanel(BgCellManager *panels);
void PCBox_SlideInItemInfoPanel(BgCellManager *panels);
void PCBox_SlideOutItemInfoPanel(BgCellManager *panels);
void PCBox_ShowItemInfoPanel(BgCellManager *panels);
void PCBox_HideItemInfoPanel(BgCellManager *panels);
void PCBox_DrawMarkings(PCBoxWork *work, u32 mask);
u32 PCBox_StartSummaryApp(PCBoxWork *work);
u32 PCBox_EndSummaryApp(PCBoxWork *work);
u32 PCBox_StartBagApp(PCBoxWork *work);
u32 PCBox_EndBagApp(PCBoxWork *work);
u32 PCBox_StartNamingScreenApp(PCBoxWork *work);
u32 PCBox_EndNamingScreenApp(PCBoxWork *work);
u32 PCBox_UpdateMoveModeBoxDrag(PCBoxWork *work);
u32 PCBox_UpdateMoveModePartyDrag(PCBoxWork *work);
u32 PCBox_UpdateBoxTrayDrag(PCBoxWork *work);
u32 PCBox_AnimatePartyShiftAfterRelease(PCBoxWork *work);
u32 PCBox_AnimateMoveToPartyEnd(PCBoxWork *work);
u32 PCBox_AnimateMoveToFirstEmptySlot(PCBoxWork *work);
u32 PCBox_UpdateScrollToPrevBox(PCBoxWork *work);
u32 PCBox_UpdateScrollToNextBox(PCBoxWork *work);
u32 PCBox_WaitActionMenuSlide(PCBoxWork *work);
u32 PCBox_WaitActionMenuAndButtonsSlide(PCBoxWork *work);
void PCBox_UpdateMarkingMenuSlide(PCBoxWork *work);
BOOL PCBox_WaitBottomButtonsSlide(PCBoxWork *work);
u32 PCBox_WaitPartyPanelSlide(PCBoxWork *work);
int PCBox_WaitPartyPanelAndActionMenuSlide(PCBoxWork *work);
void PCBox_WaitPartyPanelSlideFunc(PCBoxWork *work);
int PCBox_UpdateBoxTraySlideIn(PCBoxWork *work);
int PCBox_UpdateBoxTraySlideOut(PCBoxWork *work);
void PCBox_WaitReturnButtonSlide(PCBoxWork *work);
int PCBox_WaitActionMenuThenTitleSlide(PCBoxWork *work);
void PCBox_WaitTitlePanelSlide(PCBoxWork *work);
u32 PCBox_UpdateBoxMenuSlide(PCBoxWork *work);
u32 PCBox_WaitBoxMenuThenWallpaperList(PCBoxWork *work);
void PCBox_WaitWallpaperListSlideOut(PCBoxWork *work);
BOOL PCBox_AnimateDepositBoxSelectOpen(PCBoxWork *work);
u32 PCBox_AnimateDepositBoxSelectClose(PCBoxWork *work);
u32 PCBox_WaitSummaryOutChangeBoxIn(PCBoxWork *work);
u32 PCBox_WaitTrayButtonsSlide(PCBoxWork *work);
u32 PCBox_WaitChangeBoxOutSummaryIn(PCBoxWork *work);
u32 PCBox_WaitTitleAndReturnButtonSlide(PCBoxWork *work);
u32 PCBox_UpdateDepositDrag(PCBoxWork *p);
u32 PCBox_UpdateWithdrawDrag(PCBoxWork *p);
u32 PCBox_UpdateCursorSlide(PCBoxWork *work);
u32 PCBox_WaitSelectWithActionMenu(PCBoxWork *work);
u32 PCBox_WaitSelectWithActionMenuAndButtons(PCBoxWork *work);
u32 PCBox_WaitDeselectThenReturnButton(PCBoxWork *work);
u32 PCBox_WaitDeselectThenButtons(PCBoxWork *work);
void PCBox_AttachIconToCursor(PCBoxWork *work);
BOOL PCBox_AnimateTrayPickUp(PCBoxWork *work);
u32 PCBox_AnimateTrayDrop(PCBoxWork *work);
u32 PCBox_WaitItemModeSelect(PCBoxWork *work);
int PCBox_UpdateItemModeBoxDrag(PCBoxWork *work);
u32 PCBox_WaitItemModeDeselect(PCBoxWork *work);
u32 PCBox_AnimateSwappedItemMove(PCBoxWork *work);
int PCBox_UpdateItemTrayDrag(PCBoxWork *work);
u32 PCBox_AnimateTraySwappedItemMove(PCBoxWork *work);
BOOL PCBox_AnimateTrayItemPickUp(PCBoxWork *work);
u32 PCBox_AnimateTrayItemDrop(PCBoxWork *work);
u32 PCBox_AnimateTrayItemReturn(PCBoxWork *work);
int PCBox_UpdateItemModePartyDrag(PCBoxWork *work);
u32 PCBox_AnimatePartySwappedItemMove(PCBoxWork *work);
BOOL PCBox_RunStateMachine(PCBoxWork *work, int *state);

u32 PCBox_State_Init(PCBoxWork *work);
u32 PCBox_State_Exit(PCBoxWork *work);
u32 PCBox_State_WaitFade(PCBoxWork *work);
u32 PCBox_State_WaitPaletteBuffers(PCBoxWork *work);
u32 PCBox_State_Delay(PCBoxWork *work);
u32 PCBox_State_WaitUpdateFunc(PCBoxWork *work);
u32 PCBox_State_WaitButton(PCBoxWork *work);
u32 PCBox_State_HandleYesNo(PCBoxWork *work);
u32 PCBox_State_WaitPaletteFlash(PCBoxWork *work);
u32 PCBox_State_StartSubApp(PCBoxWork *work);
u32 PCBox_State_RunSubApp(PCBoxWork *work);
void PCBox_State_Start(PCBoxWork *work);
u32 PCBox_State_MoveModeInput(PCBoxWork *work);
u32 PCBox_State_ApplyMonMove(PCBoxWork *work);
void PCBox_State_ReturnToActionMenu(PCBoxWork *work);
void PCBox_State_ReturnFromSummary(PCBoxWork *work);
void PCBox_State_ReturnFromNamingScreen(PCBoxWork *work);
void PCBox_State_GiveItemFromBag(PCBoxWork *work);
u32 PCBox_State_PrintItemHeld(PCBoxWork *work);
void PCBox_State_MarkingChosen(PCBoxWork *work);
void PCBox_State_OpenMarkingMenu(PCBoxWork *work);
u32 PCBox_State_SetupMarkingMenuInput(PCBoxWork *work);
u32 PCBox_State_MarkingMenuInput(PCBoxWork *work);
u32 PCBox_State_FinishCloseMarkingMenu(PCBoxWork *work);
u32 PCBox_State_TryRelease(PCBoxWork *work);
u32 PCBox_State_ReleaseMoveCheck(PCBoxWork *work);
u32 PCBox_State_PrintReleased(PCBoxWork *work);
u32 PCBox_State_PrintByeBye(PCBoxWork *work);
u32 PCBox_State_RemoveReleasedMon(PCBoxWork *work);
u32 PCBox_State_EndPartyReleaseShift(PCBoxWork *work);
u32 PCBox_State_ReleaseCameBack(PCBoxWork *work);
u32 PCBox_State_PrintWorried(PCBoxWork *work);
u32 PCBox_State_CancelRelease(PCBoxWork *work);
u32 PCBox_State_ResumeDepositInput(PCBoxWork *work);
u32 PCBox_State_OpenPartyPanel(PCBoxWork *work);
u32 PCBox_State_SetupPartyPanelInput(PCBoxWork *work);
int PCBox_State_MoveModePartyInput(PCBoxWork *work);
u32 PCBox_State_SlideOutPartyPanel(PCBoxWork *work);
u32 PCBox_State_PartyPanelClosed(PCBoxWork *work);
u32 PCBox_State_EndMoveModePartyDrag(PCBoxWork *work);
u32 PCBox_State_SetupBoxTrayInput(PCBoxWork *work);
u32 PCBox_State_BoxTrayInput(PCBoxWork *work);
u32 PCBox_State_EndBoxTrayDrag(PCBoxWork *p);
u32 PCBox_State_EndTrayHoldingDrop(PCBoxWork *p);
u32 PCBox_State_ResumeBoxTrayInput(PCBoxWork *work);
u32 PCBox_State_OpenTrayBesideParty(PCBoxWork *work);
u32 PCBox_State_SetupPartyInputAfterTray(PCBoxWork *work);
u32 PCBox_State_OpenTrayFromBox(PCBoxWork *work);
void PCBox_State_SlideInTrayPartyPanel(PCBoxWork *work);
u32 PCBox_State_SlideOutTray(PCBoxWork *work);
void PCBox_State_TrayClosed(PCBoxWork *work);
void PCBox_State_ReturnToBoxFromTray(PCBoxWork *work);
u32 PCBox_State_BoxMenuInput(PCBoxWork *work);
u32 PCBox_State_HideButtonsForBoxMenu(PCBoxWork *work);
u32 PCBox_State_OpenBoxMenuTray(PCBoxWork *work);
u32 PCBox_State_ShowBoxMenuExitButton(PCBoxWork *work);
void PCBox_State_OpenBoxMenu(PCBoxWork *work);
u32 PCBox_State_SetupBoxMenuInput(PCBoxWork *work);
void PCBox_State_CloseBoxMenu(PCBoxWork *work);
void PCBox_State_SlideOutBoxMenu(PCBoxWork *work);
u32 PCBox_State_CloseBoxMenuTray(PCBoxWork *work);
u32 PCBox_State_BoxMenuClosed(PCBoxWork *work);
u32 PCBox_State_OpenWallpaperMenu(PCBoxWork *work);
u32 PCBox_State_ShowWallpaperMenuTitle(PCBoxWork *work);
u32 PCBox_State_SetupWallpaperMenuInput(PCBoxWork *work);
u32 PCBox_State_WallpaperMenuInput(PCBoxWork *work);
u32 PCBox_State_CloseWallpaperMenu(PCBoxWork *work);
u32 PCBox_State_SlideOutWallpaperList(PCBoxWork *work);
u32 PCBox_State_ReturnToBoxMenu(PCBoxWork *work);
u32 PCBox_State_ApplyWallpaper(PCBoxWork *work);
u32 PCBox_State_WallpaperFadeOut(PCBoxWork *work);
u32 PCBox_State_WallpaperFadeIn(PCBoxWork *work);
u32 PCBox_State_ResumeBoxInput(PCBoxWork *work);
u32 PCBox_State_ResumeMoveModePartyInput(PCBoxWork *work);
int PCBox_State_WithdrawInput(PCBoxWork *work);
void PCBox_State_ReturnToWithdrawInput(PCBoxWork *work);
void PCBox_State_SlideInWithdrawParty(PCBoxWork *work);
void PCBox_State_StartWithdrawMove(PCBoxWork *work);
void PCBox_State_EndWithdrawMove(PCBoxWork *work);
void PCBox_State_WithdrawPartyClosed(PCBoxWork *work);
u32 PCBox_State_EndWithdrawDrag(PCBoxWork *work);
u32 PCBox_State_ResumeWithdrawInput(PCBoxWork *work);
int PCBox_State_DepositInput(PCBoxWork *work);
int PCBox_State_TryDeposit(PCBoxWork *work);
u32 PCBox_State_ReturnFromDepositError(PCBoxWork *work);
u32 PCBox_State_ShowDepositBoxSelect(PCBoxWork *work);
u32 PCBox_State_ShowDepositTitle(PCBoxWork *work);
u32 PCBox_State_SetupDepositBoxInput(PCBoxWork *work);
u32 PCBox_State_DepositBoxInput(PCBoxWork *work);
u32 PCBox_State_CancelDepositBoxSelect(PCBoxWork *work);
u32 PCBox_State_HideDepositExitButton(PCBoxWork *work);
u32 PCBox_State_HideDepositBoxSelect(PCBoxWork *work);
u32 PCBox_State_DepositBoxSelectClosed(PCBoxWork *work);
u32 PCBox_State_ConfirmDepositBox(PCBoxWork *work);
u32 PCBox_State_CheckDepositBox(PCBoxWork *work);
u32 PCBox_State_ShiftPartyForDeposit(PCBoxWork *work);
u32 PCBox_State_SwitchBox(PCBoxWork *work);
u32 PCBox_State_StartDepositMove(PCBoxWork *work);
u32 PCBox_State_EndDepositMove(PCBoxWork *work);
u32 PCBox_State_ShowDepositReturnButton(PCBoxWork *work);
u32 PCBox_State_ResetDepositInput(PCBoxWork *work);
u32 PCBox_State_EndDepositDrag(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToMoveMode(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToBoxTray(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToMoveModeParty(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToDeposit(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToDepositPrompt(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToDepositBox(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToMarkingMenu(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToBoxMenu(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToWallpaperMenu(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToWithdraw(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToWithdrawPrompt(PCBoxWork *work);
u32 PCBox_State_ItemModeInput(PCBoxWork *work);
u32 PCBox_State_CloseItemMessage(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToItemMode(PCBoxWork *work);
u32 PCBox_State_AskPutAwayItem(PCBoxWork *work);
u32 PCBox_State_WaitPutAwayItem(PCBoxWork *work);
u32 PCBox_State_CancelPutAwayItem(PCBoxWork *work);
u32 PCBox_State_EndItemCommand(PCBoxWork *work);
u32 PCBox_State_ItemModePrintEggCantHoldItem(PCBoxWork *work);
u32 PCBox_State_ItemModePrintItemHeld(PCBoxWork *work);
u32 PCBox_State_EndItemModeBoxDrag(PCBoxWork *work);
u32 PCBox_State_SwapHeldItems(PCBoxWork *work);
u32 PCBox_State_DropSwappedItem(PCBoxWork *work);
u32 PCBox_State_FinishItemSwap(PCBoxWork *work);
u32 PCBox_State_ItemModeBoxTrayInput(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToItemModeTray(PCBoxWork *work);
u32 PCBox_State_EndItemTrayDrag(PCBoxWork *work);
u32 PCBox_State_ApplyItemTrayDrop(PCBoxWork *work);
u32 PCBox_State_FinishItemTrayDrop(PCBoxWork *work);
u32 PCBox_State_ResumeItemModeTrayInput(PCBoxWork *work);
u32 PCBox_State_ItemTrayHoldingInput(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToItemTrayHolding(PCBoxWork *work);
u32 PCBox_State_ApplyItemTrayHoldingDrop(PCBoxWork *work);
u32 PCBox_State_ItemModePartyInput(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToItemModeParty(PCBoxWork *work);
u32 PCBox_State_EndItemModePartyDrag(PCBoxWork *work);
u32 PCBox_State_SwapPartyHeldItems(PCBoxWork *work);
u32 PCBox_State_ResumeItemModePartyInput(PCBoxWork *work);
u32 PCBox_State_DropSwappedPartyItem(PCBoxWork *work);
u32 PCBox_State_FinishPartyItemSwap(PCBoxWork *work);
u32 PCBox_State_ResumeItemModePartyAfterMail(PCBoxWork *work);
u32 PCBox_State_PrintEggCantHoldItem(PCBoxWork *work);
u32 PCBox_State_PrintMailCantBeTaken(PCBoxWork *work);
u32 PCBox_State_BoxTrayHoldingInput(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToBoxTrayHolding(PCBoxWork *work);
u32 PCBox_TakeHeldItemToBag(PCBoxWork *work);
u32 PCBox_ConfirmRelease(PCBoxWork *work);
u32 PCBox_PutAwayHeldItem(PCBoxWork *work);
u32 PCBox_ConfirmExit(PCBoxWork *work);
u32 PCBox_CancelExit(PCBoxWork *work);
u32 PCBox_FlashPanel(PCBoxWork *work, u32 panelIndex, u32 nextState);
u32 PCBox_State_AskExit(PCBoxWork *work);
u32 PCBox_State_AskContinue(PCBoxWork *work);
void PCBox_State_ChooseParty(PCBoxWork *work);
void PCBox_State_ChooseTrayTab(PCBoxWork *work);
u32 PCBox_State_ChooseMoveFromBox(PCBoxWork *work);
u32 PCBox_State_ChooseSummary(PCBoxWork *work);
void PCBox_State_ChooseItem(PCBoxWork *work);
void PCBox_State_ChooseMarking(PCBoxWork *work);
void PCBox_State_ChooseMarkingDone(PCBoxWork *work);
void PCBox_State_ChooseRelease(PCBoxWork *work);
void PCBox_State_CloseMoveModeActionMenu(PCBoxWork *work);
void PCBox_State_PartyChooseTray(PCBoxWork *work);
void PCBox_State_CloseMoveModePartyActionMenu(PCBoxWork *work);
void PCBox_State_ChoosePartyPanelExit(PCBoxWork *work);
void PCBox_State_ChooseTrayBox(PCBoxWork *work);
void PCBox_State_ChooseTrayExit(PCBoxWork *work);
u32 PCBox_State_BoxMenuSwitchBox(PCBoxWork *work);
void PCBox_State_ChooseWallpaper(PCBoxWork *work);
u32 PCBox_State_ChooseRename(PCBoxWork *work);
u32 PCBox_State_SwitchBoxForWallpaper(PCBoxWork *work);
int PCBox_State_ChooseWallpaperMenuExit(PCBoxWork *work);
void PCBox_State_ChooseWithdraw(PCBoxWork *work);
void PCBox_State_CloseWithdrawActionMenu(PCBoxWork *work);
void PCBox_State_ChooseDeposit(PCBoxWork *work);
void PCBox_State_CloseDepositActionMenu(PCBoxWork *work);
void PCBox_State_ItemModeChooseParty(PCBoxWork *work);
void PCBox_State_ItemModeChooseTrayTab(PCBoxWork *work);
void PCBox_State_ChooseBagGive(PCBoxWork *work);
void PCBox_State_CloseItemModeActionMenu(PCBoxWork *work);
void PCBox_State_ItemModeChooseTrayBox(PCBoxWork *work);
void PCBox_State_ItemModePartyChooseTray(PCBoxWork *work);
void PCBox_State_CloseItemModePartyActionMenu(PCBoxWork *work);

void PCBox_FreeSprites(PCBoxView *view);
void PCBox_TickSprites(PCBoxView *view);
void PCBox_SetSpriteAnim(PCBoxView *view, u32 index, u32 anim);
u32 PCBox_IsSpriteAnimated(PCBoxView *view, u32 i);
void PCBox_SetSpriteVisible(PCBoxView *view, int index, int state);
int PCBox_IsSpriteVisible(PCBoxView *view, int index);
void PCBox_SetSpritePriority(PCBoxView *view, u32 i, u32 pri);
void PCBox_InitSpriteSystem(PCBoxView *view);
void PCBox_LoadSpriteResources(PCBoxView *view);
void PCBox_CreateSprites(PCBoxWork *work);
void PCBox_LoadSpriteCharsMain(PCBoxView *view, int i, void *src, u32 size);
void PCBox_LoadBoxIconImages(PCBoxWork *work, int box);
void PCBox_RefreshSlotIcon(PCBoxWork *work, int a, int b, int i);
void PCBox_LoadBoxIcons(PCBoxWork *work);
void PCBox_LoadPartyIcons(PCBoxWork *work);
void PCBox_GetSlotPosition(u32 i, s16 *x, s16 *y, int mode);
void PCBox_PlacePartyIconsLeftHidden(PCBoxWork *work);
void PCBox_PlacePartyIconsRightHidden(PCBoxWork *work);
void PCBox_PlacePartyIconsLeft(PCBoxWork *work);
void PCBox_PlacePartyIconsRight(PCBoxWork *work);
void PCBox_SetIconLayer(PCBoxView *view, u32 i, int active);
void PCBox_SetMovingIconPriority(PCBoxView *view, int i, u32 which);
void PCBox_ScrollBoxIcons(PCBoxWork *work, s32 amount);
void PCBox_UpdatePartyIconPositions(PCBoxWork *work);
void PCBox_StartReleaseScale(PCBoxReleaseWork *releaseWork);
BOOL PCBox_UpdateReleaseShrink(PCBoxReleaseWork *releaseWork);
BOOL PCBox_UpdateReleaseGrow(PCBoxReleaseWork *releaseWork);
void PCBox_EndReleaseShrink(PCBoxReleaseWork *releaseWork);
void PCBox_ResetSpriteAffine(PCBoxReleaseWork *releaseWork);
void PCBox_SetIconsTranslucent(PCBoxWork *work, u32 flags, u32 enabled);
void PCBox_SetSlotIconTranslucent(PCBoxView *view, u32 off, u32 v);
void PCBox_ShowMonPic(PCBoxWork *work, PCBoxMonInfo *monInfo, u32 slot);
void PCBox_LoadHeldItemIcon(PCBoxView *view, u16 item);
void PCBox_LoadInfoItemIcon(PCBoxView *view, u16 item);
void PCBox_SetHeldItemIconAffine(PCBoxView *view, u32 on);
void PCBox_SetHeldItemIconPosition(PCBoxView *view, s16 x, s16 y);
void PCBox_MoveHeldItemIconToSlot(PCBoxView *view, u32 index, u32 mode);
void PCBox_PlaceHeldItemIconAtSlot(PCBoxView *view, u32 index, u32 mode);
void PCBox_CreateHeldItemIconOutline(PCBoxView *view);
void PCBox_ShowHeldItemIconOutline(PCBoxView *view);
void PCBox_UpdateHeldItemIconOutline(PCBoxView *view);
void PCBox_UpdateInfoItemIconPosition(PCBoxView *view);
void PCBox_DrawTypeIcons(PCBoxView *view, PCBoxMonInfo *monInfo);
void PCBox_CreateSelectedIconOutline(PCBoxWork *work);
void PCBox_HideSelectedIconOutline(PCBoxWork *work);
void PCBox_SetSelectedIconOutlineVisible(PCBoxWork *work, u32 visible);
void PCBox_UpdateSelectedIconOutline(PCBoxWork *work);
void PCBox_SetBoxArrowsVisible(PCBoxView *view, u32 visible);
void PCBox_SetupWallpaperMenuSprites(PCBoxWork *work);
void PCBox_MoveWallpaperMenuSprites(PCBoxView *view, s32 dy);
void PCBox_UpdateCurrentWallpaperMarker(PCBoxWork *work);
void PCBox_UpdateWallpaperCursor(PCBoxWork *work);
void PCBox_LoadWallpaperThumbnails(PCBoxWork *work);
void PCBox_PlaceTrayTextObjs(PCBoxView *view);
void PCBox_SetupBoxTraySprites(PCBoxWork *work);
void PCBox_MoveBoxTraySprites(PCBoxWork *work, s32 dy);
void PCBox_UpdateCurrentBoxMarker(PCBoxWork *work);
void PCBox_UpdateTrayCursor(PCBoxWork *work);
void PCBox_GetTrayBoxPosition(PCBoxView *view, u32 i, s16 *x, s16 *y);
void PCBox_DrawBoxThumbnail(PCBoxWork *work, int n);
void PCBox_DrawAllBoxThumbnails(PCBoxWork *work);

void PCBox_LoadTrayPageThumbnails(PCBoxWork *work);
void PCBox_RefreshTrayBoxThumbnail(PCBoxWork *work, u32 box);
void PCBox_DrawBoxThumbnailMons(PCBoxWork *work, u32 box, void *dst);
void PCBox_SetupDepositBoxSprites(PCBoxWork *work);
void PCBox_HideBoxTraySprites(PCBoxWork *work);
void PCBox_CreateTrayTextObjs(PCBoxView *view);
void PCBox_DestroyTrayTextObjs(PCBoxView *view);
void PCBox_AddWindows(PCBoxWork *work);
void PCBox_RemoveWindows(PCBoxWork *work);
void PCBox_PrintMonInfo(PCBoxWork *work, PCBoxMonInfo *monInfo);
void PCBox_ClearMonInfoWindows(PCBoxView *view);
int PCBox_DrawMoveWindows(PCBoxWork *work, PCBoxMonInfo *monInfo);
u32 PCBox_PrintItemNameAndDesc(PCBoxWork *work, u16 itemId);
void PCBox_PrintModeTitle(PCBoxWork *work);
void PCBox_PrintInfoLabels(PCBoxWork *work);
void PCBox_DrawBoxNameOnWallpaper(PCBoxWork *work, void *src, u32 baseTile, u32 width, u32 height);
void PCBox_DrawTrayBoxNameAndCount(PCBoxWork *work);
void PCBox_DrawWallpaperName(PCBoxWork *work);
void PCBox_DrawPartyButton(PCBoxWork *work, u32 style);
void PCBox_DrawTrayTab(PCBoxWork *work, u32 unused);
void PCBox_DrawReturnButton(PCBoxWork *work, u32 style);
void PCBox_DrawExitButton(PCBoxWork *work, u32 style);
void PCBox_DrawActionMenu(PCBoxWork *work, const PCBoxMenuEntry *list, u32 count);
void PCBox_DrawBagGiveButton(PCBoxWork *work, u32 which);
void PCBox_DrawChangeBoxButton(PCBoxWork *work);
void PCBox_DrawBoxMenu(PCBoxWork *work);
void PCBox_DrawChangeWallpaperTitle(PCBoxWork *work);
void PCBox_DrawDepositTitle(PCBoxWork *work);
void PCBox_AttachPanelWindows(PCBoxView *view);
void PCBox_AttachPartyButtonWindow(PCBoxView *view);
void PCBox_AttachTrayTabWindow(PCBoxView *view);
void PCBox_AttachReturnButtonWindow(PCBoxView *view);
void PCBox_AttachMenuExitButtonWindow(PCBoxWork *work);
void PCBox_DrawConfirmCancel(PCBoxView *view);
void PCBox_PrintPartyPanelSwitchLabel(PCBoxView *view);
void PCBox_PrintPartyPanelExitLabel(PCBoxView *view);
void PCBox_AttachMarkingMenuWindows(PCBoxView *view);
void PCBox_AttachPartyPanelLabelWindow(PCBoxView *view);
void PCBox_DrawSummaryButton(PCBoxWork *work, u32 unused);
void PCBox_CloseMessageWindow(PCBoxView *view, int windowId);
void PCBox_ClearWindowFrameNoTransfer(PCBoxView *view, u32 idx);
void PCBox_ClearWindowFrame(PCBoxView *view, u32 idx);
void PCBox_PrintExitPrompt(PCBoxWork *work, int x, int kind);
void PCBox_PrintTakeItemPrompt(PCBoxWork *work, int item, int x);
void PCBox_PrintMailCantBeTaken(PCBoxWork *work, int x);
void PCBox_PrintMailCantBeMoved(PCBoxWork *work, int x);
void PCBox_PrintTookItem(PCBoxWork *work, int item, int x);
void PCBox_PrintBagFull(PCBoxWork *work, int x);
void PCBox_PrintItemHeld(PCBoxWork *work, int item, int x);
void PCBox_PrintTouchTheMark(PCBoxWork *work, int x);
void PCBox_PrintReleaseMessage(PCBoxWork *work, int mode, int windowId);
void PCBox_PrintMenuPrompt(PCBoxWork *work, u32 which, u32 v);
void PCBox_PrintMonActionMessage(PCBoxWork *work, u32 a, u32 kind, u32 v);
void PCBox_PrintItemActionMessage(PCBoxWork *work, u32 kind, u32 v);
void PCBox_PrintPanelHint(PCBoxWork *work, u32 idx, u32 kind);
void PCBox_DrawWindowFrame(PCBoxWork *work, int windowId);
s32 PCBox_FindTouchedBoxSlot(void);
s32 PCBox_FindTouchedPartySlot(void);
s32 PCBox_FindTouchedTrayPartySlot(void);
void PCBox_CreateModeGridInput(PCBoxWork *work);
void PCBox_CreateGridInput(PCBoxWork *work, int layout, int initialPos);
void PCBox_FreeGridInput(PCBoxWork *work);
void PCBox_PlaceCursorOnGridInput(PCBoxWork *work, int target);
void PCBox_GridOnButton(PCBoxWork *work, int target);
void PCBox_GridOnSwitchToTouchMode(PCBoxWork *work);

void PCBox_GridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_GridOnTouch(PCBoxWork *work, u32 newTarget);
u32 PCBox_GetDepositMenuInput(PCBoxWork *work);
void PCBox_DepositGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_DepositBoxGridOnButton(PCBoxWork *work, u32 newTarget);
void PCBox_DepositBoxGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_DepositBoxGridOnTouch(PCBoxWork *work, u32 newTarget);
u32 PCBox_GetWithdrawMenuInput(PCBoxWork *work);
void PCBox_WithdrawGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_WithdrawGridOnTouch(PCBoxWork *work, u32 newTarget);
u32 PCBox_GetMoveModeMenuInput(PCBoxWork *work);
void PCBox_MoveModeGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_MoveModeGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_MoveModeGridOnTouch(PCBoxWork *work, u32 newTarget);
u32 PCBox_GetBoxTrayInput(PCBoxWork *work);
void PCBox_TrayGridOnButton(PCBoxWork *work, u32 newTarget);
void PCBox_TrayGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_TrayGridOnTouch(PCBoxWork *work, u32 newTarget);
u32 PCBox_GetTrayHoldingInput(PCBoxWork *work);
u32 PCBox_GetMoveModePartyMenuInput(PCBoxWork *work);
void PCBox_MoveModePartyGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_MoveModePartyGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
u32 PCBox_GetItemModeMenuInput(PCBoxWork *work);
void PCBox_ItemModeGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_ItemModeGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_ItemModeGridOnTouch(PCBoxWork *work, u32 newTarget);
u32 PCBox_GetItemModePartyMenuInput(PCBoxWork *work);
void PCBox_ItemModePartyGridOnButton(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_ItemModePartyGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_BoxMenuGridOnButton(PCBoxWork *work, u32 newTarget);
void PCBox_BoxMenuGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_BoxMenuGridOnTouch(PCBoxWork *work, u32 newTarget);
void PCBox_WallpaperGridOnButton(PCBoxWork *work, u32 newTarget);
void PCBox_WallpaperGridOnKeyMove(PCBoxWork *work, u32 newTarget, u32 prevTarget);
void PCBox_WallpaperGridOnTouch(PCBoxWork *work, u32 newTarget);
u32 PCBox_GetItemTrayHoldingInput(PCBoxWork *work);
void PCBox_StartCursorSlide(PCBoxView *view, u32 newTarget, u32 prevTarget);
u32 PCBox_IsSummaryShortcutPressed(PCBoxWork *work);

#endif // POKEHEARTGOLD_APPLICATION_PC_BOX_PC_BOX_INTERNAL_H
