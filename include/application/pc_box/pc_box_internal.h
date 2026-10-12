#ifndef POKEHEARTGOLD_APPLICATION_PC_BOX_PC_BOX_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_PC_BOX_PC_BOX_INTERNAL_H

#include "bag_types_def.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font_sprite.h"
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
#include "unk_02019BA4.h"
#include "yes_no_prompt.h"

// Defined in src/bg_cell_manager.c, not declared in bg_cell_manager.h.
void BgCellManager_LoadCellTilemap(BgCellManager *manager, int index, int narcId, int fileId, int compressed);
void BgCellManager_DrawCell(BgCellManager *manager, int index);
BOOL BgCellManager_UpdateCellMove(BgCellManager *manager, int index);
void BgCellManager_PutWindowTiles(BgCellManager *manager, int index, Window *window);
u8 BgCellManager_GetCellBgId(BgCellManager *manager, int index);
void BgCellManager_GetCellPosition(BgCellManager *manager, int index, s8 *x, s8 *y);

// Defined in src/font_sprite.c, not declared in font_sprite.h.
TextOBJ *FontOAM_Create(TextOBJTemplate *template);
void FontOAM_Delete(TextOBJ *object);
int FontOAM_GetCharSize(Window *window, NNS_G2D_VRAM_TYPE vram, int heapId);
void TextOBJ_SetPriority(TextOBJ *object, u8 priority);
void TextOBJ_SetPalOffset(TextOBJ *object, u8 value);
void TextOBJ_SetPalOffsetRespectVramOffset(TextOBJ *object, int value);

// Defined in src/render_window.c, not declared in render_window.h.
void DrawFrame2AroundWindow(Window *window, u32 baseTile, u32 paletteNum);

// Launch modes (PCBoxArgs.unk8).
#define PC_BOX_MODE_DEPOSIT      0
#define PC_BOX_MODE_WITHDRAW     1
#define PC_BOX_MODE_MOVE_POKEMON 2
#define PC_BOX_MODE_MOVE_ITEMS   3

// Slots: 0-29 are the active box, 30-35 the party.
#define PC_BOX_SLOT_PARTY_START 30
#define PC_BOX_SLOT_COUNT       36
#define PC_BOX_SLOT_NONE        0xFF

#define PC_BOX_STATE_EXIT 0xB3

typedef struct PCBoxWork PCBoxWork;

// Run each vblank by PCBox_VBlankCallback until it returns FALSE.
typedef BOOL (*PCBoxUpdateFunc)(PCBoxWork *work);

// Info about the mon in a slot, shown on the top screen (PCBox_CreateMonInfo).
typedef struct PCBoxMonInfo {
    BoxPokemon *boxMon; // 0x00
    u16 species;        // 0x04
    u16 heldItem;       // 0x06
    u32 personality;    // 0x08
    u8 type1;           // 0x0C
    u8 type2;           // 0x0D
    u8 ability;         // 0x0E
    u8 nature;          // 0x0F
    u16 markings;       // 0x10
    u8 level : 7;       // 0x12
    u8 isEgg : 1;
    u8 gender : 7; // 0x13
    u8 showGender : 1;
    u16 moves[4]; // 0x14
} PCBoxMonInfo;   // size: 0x1C

// Releasing a mon: the scan of all boxes and the party for another mon that knows the mon's HM moves
// (PCBox_StartReleaseMoveCheck / PCBox_RunReleaseMoveCheck), then the shrink animation of its icon.
typedef struct PCBoxReleaseWork {
    ManagedSprite *sprite; // 0x00
    u16 scanIndex;         // 0x04 box * 30 + slot, 540 + party slot
    u8 moveFlags;          // 0x06 HM moves not found on another mon yet
    u8 scaleCounter;       // 0x07
    f32 scale;             // 0x08
} PCBoxReleaseWork;        // size: 0x0C

// One mon icon moving between two slots (PCBox_AnimateMonMove).
typedef struct PCBoxMoveEntry {
    void *mon;
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
    UnkStruct_02021AC8 charTransfer;
} PCBoxTextObj;

// Graphics, text and input state of the app (heap 10, PCBoxWork.view).
typedef struct PCBoxView {
    SysTask *vblankTask;               // 0x0000
    PCBoxUpdateFunc updateFunc;        // 0x0004
    PCBoxUpdateFunc pendingUpdateFunc; // 0x0008
    void *temp;                        // 0x000C
    u16 subState;                      // 0x0010
    u16 counter;                       // 0x0012
    BgConfig *bgConfig;                // 0x0014
    PaletteData *paletteData;          // 0x0018
    MessagePrinter *msgPrinter;        // 0x001C
    MsgData *msgData;                  // 0x0020
    MessageFormat *msgFormat;          // 0x0024
    String *string;                    // 0x0028
    GridInputHandler *gridInput;       // 0x002C
    Window windows[0x2C];              // 0x0030
    BgCellManager *panels;             // 0x02F0
    SpriteSystem *spriteSystem;        // 0x02F4
    SpriteManager *spriteManager;      // 0x02F8
    ManagedSprite *sprites[69];        // 0x02FC
    FontSystem *fontSystem;            // 0x0410
    PCBoxTextObj textObjs[2];          // 0x0414
    YesNoPrompt *yesNoPrompt;          // 0x0434
    u16 yesNoHandler;                  // 0x0438 index into sPCBoxYesNoHandlers
    u16 unk_043A;                      // 0x043A
    u32 unk_043C;                      // 0x043C
    u32 fadeNextState;                 // 0x0440
    u32 delay;                         // 0x0444
    s8 unk_0448;                       // 0x0448
    u8 unk_0449;                       // 0x0449
    u8 unk_044A;                       // 0x044A
    u8 unk_044B;                       // 0x044B
    u8 unk_044C;                       // 0x044C
    u8 wallpaperSelection;             // 0x044D
    u8 infoWindowToggle : 4;           // 0x044E which of the two info window sets is drawn next
    u8 movesDrawn : 3;
    u8 unk_044E_7 : 1;
    u8 unk_044F;                     // 0x044F
    NARC *personalNarc;              // 0x0450
    NARC *iconNarc;                  // 0x0454
    u8 iconChars[30][0x200];         // 0x0458
    u8 iconDraw[30];                 // 0x4058
    u8 iconPalettes[30];             // 0x4076
    u8 slotIcons[PC_BOX_SLOT_COUNT]; // 0x4094
    int touchX;                      // 0x40B8
    int touchY;                      // 0x40BC
    u32 markings;                    // 0x40C0
    u32 unk_40C4;                    // 0x40C4
    u8 boxThumbnails[18][0x400];     // 0x40C8
    u16 heldItem;                    // 0x88C8
    u16 unk_88CA;                    // 0x88CA
    u32 unk_88CC;                    // 0x88CC
    u16 monPicBuffer;                // 0x88D0
    u16 spriteDrawLock;              // 0x88D2
    PCBoxPaletteFlash paletteFlash;  // 0x88D4
    PCBoxReleaseWork *releaseWork;   // 0x88DC
} PCBoxView;                         // size: 0x88E0

struct PCBoxWork {
    PCBoxArgs *args;        // 0x00
    PCStorage *storage;     // 0x04
    Party *party;           // 0x08
    Bag *bag;               // 0x0C
    Options *options;       // 0x10
    OverlayManager *subApp; // 0x14
    void *subAppArgs;       // 0x18
    u16 itemId;             // 0x1C
    u8 subAppIndex;         // 0x1E
    u8 activeBox;           // 0x1F
    u8 unk_20;              // 0x20
    u8 selectedSlot;        // 0x21
    u8 partyLayout;         // 0x22
    u8 unk_23;              // 0x23
    u8 trayOpen;            // 0x24
    u8 trayBox;             // 0x25
    u8 unk_26;              // 0x26
    u8 unk_27;              // 0x27
    u8 unk_28;              // 0x28
    u8 unk_29;              // 0x29
    u8 unk_2A;              // 0x2A
    u8 unk_2B;              // 0x2B
    u32 unk_2C;             // 0x2C
    u32 nextState;          // 0x30
    PCBoxView *view;        // 0x34
}; // size: 0x38

// pc_box.c
void PCBox_VBlankCallback(SysTask *task, void *data);
void PCBox_UpdateDetailPanel(PCBoxWork *work);
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
void PCBox_SwapBoxAndPartyMon(PCBoxWork *work, PCBoxMoveEntry *a, PCBoxMoveEntry *b);
void PCBox_MoveBoxMonToParty(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_MovePartyMonToBox(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_SwapPartyMons(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_MovePartyMonToEnd(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_MoveBoxMonToOtherBox(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_MovePartyMonToOtherBox(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_ApplyMonMove(PCBoxWork *work);
u32 PCBox_GetTargetBoxId(PCBoxWork *work, u32 v);
u32 PCBox_PartyHasOtherUsableMon(PCBoxWork *work, u32 skip);
u32 PCBox_UpdateArceusGiratinaForm(BoxPokemon *boxMon);
void PCBox_RevertShayminFormOnMove(PCBoxWork *work, u32 a, u32 b);
u32 PCBox_AnimateMonMove(PCBoxWork *work);
u32 ov14_021E66F4(PCBoxWork *work);
u32 ov14_021E6814(PCBoxWork *work);
u32 PCBox_CanMoveMon(PCBoxWork *work, u32 a, u32 b);
u32 PCBox_CanMovePartyMonToEmptySlot(PCBoxWork *work, u32 a, u32 b);
u32 PCBox_CanSwapHeldItems(PCBoxWork *work, u32 a, u32 b);
void PCBox_SetupMoveEntryToSlot(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_SetupMoveEntryToSprite(PCBoxWork *work, PCBoxMoveEntry *e);
void PCBox_SetupMonSwapEntries(PCBoxWork *work, u32 a, u32 b);
void PCBox_SetupMonMove(PCBoxWork *work, u32 a, u32 b);
void ov14_021E6F3C(PCBoxWork *work, u32 a, u32 b);
void PCBox_SetupMonMoveToPartyEnd(PCBoxWork *work, u32 a, u32 b);
u32 PCBox_GetMoveEntryTarget(PCBoxWork *work, u32 v);
void PCBox_SetupPartyShiftAfterRelease(PCBoxWork *work, u32 a);
void PCBox_ApplyMoveEntryIcons(PCBoxWork *work, PCBoxMoveEntry *e);
s32 PCBox_GetReleaseCheckMoveIndex(u16 v);
void PCBox_StartReleaseMoveCheck(PCBoxWork *work);
void PCBox_FreeReleaseMoveCheck(PCBoxWork *work);
u32 PCBox_RunReleaseMoveCheck(PCBoxWork *work);
PCBoxMonInfo *PCBox_CreateMonInfo(BoxPokemon *boxMon);
void PCBox_FreeMonInfo(PCBoxMonInfo *monInfo);
void PCBox_DrawMonInfo(PCBoxWork *work, PCBoxMonInfo *monInfo);
void PCBox_DrawMonItemInfo(PCBoxWork *work, PCBoxMonInfo *monInfo);
BOOL PCBox_ShowSlotMonInfo(PCBoxWork *work, u32 index);
void PCBox_RedrawSlotMonInfo(PCBoxWork *work, u32 index);
void PCBox_ClearMonInfo(PCBoxWork *work);
void PCBox_ClearMonInfoAndSelection(PCBoxWork *work);
void PCBox_LoadWallpaperChars(PCBoxWork *work, s32 member, u32 unused, u32 offset);
void PCBox_LoadWallpaperPalette(PCBoxWork *work, s32 member, u32 palette);
void PCBox_LoadWallpaperTilemap(PCBoxWork *work, s32 unused, u32 tile, u32 addend, u32 palette);
void PCBox_LoadWallpaper(PCBoxWork *work, s32 member, u32 a2, u32 a3, u32 a4);
void PCBox_LoadScrolledWallpaper(PCBoxWork *work, s32 member, u32 direction);
void PCBox_SwapWallpaper(PCBoxWork *work, s32 member);
u32 PCBox_GetWallpaperGfxIndex(PCBoxWork *work, u32 box);
u32 PCBox_IsPointInRect(s32 x, s32 y, const u8 *r);
u32 PCBox_GetTouchedBoxSlot(s16 x, s16 y);
u32 PCBox_GetTouchedPartySlot(s16 x, s16 y, const u8 *rect);
u32 PCBox_GetTouchedTrayBox(s16 x, s16 y);

// pc_box_ui.c
void PCBox_StartHeldItemMove(PCBoxWork *work, int fromSlot, int toSlot, int partyLayout, int toItemIcon);
void PCBox_StartHeldItemMoveWithIcon(PCBoxWork *work, u32 a1, u32 a2, u32 a3);
void PCBox_StartHeldItemMoveFromDropTarget(PCBoxWork *work, int toSlot, int partyLayout);
BOOL PCBox_UpdateHeldItemMove(PCBoxWork *work, int toItemIcon);
u32 PCBox_UpdateHeldItemMoveWithIcon(PCBoxWork *work);
u32 PCBox_UpdateHeldItemMoveNoIcon(PCBoxWork *work);
void PCBox_InitPanels(PCBoxWork *work);
void PCBox_FreePanels(PCBoxView *view);
void PCBox_LoadPanelTilemap(BgCellManager *panels, u32 a1, u32 a2);
void PCBox_DrawMarkingMenuEntry(PCBoxWork *work, u32 index);
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
void PCBox_LowerPartyPanel(BgCellManager *panels);
void PCBox_RaisePartyPanel(BgCellManager *panels);
void PCBox_ShiftPartyPanelAndReturnButtonRight(BgCellManager *panels);
void PCBox_ShiftPartyPanelRight(BgCellManager *panels);
void PCBox_ShiftPartyPanelAndReturnButtonLeft(BgCellManager *panels);
void PCBox_ShiftPartyPanelLeft(BgCellManager *panels);
u32 PCBox_UpdatePartyPanelSlide(PCBoxWork *work);
u32 ov14_021E813C(PCBoxWork *work);
void PCBox_LoadPartyPanelTilemap(BgCellManager *panels, u32 index);
void PCBox_SlideInWallpaperList(BgCellManager *panels);
void PCBox_SlideOutWallpaperList(BgCellManager *panels);
void PCBox_ShowPartyButton(PCBoxWork *work);
void PCBox_HidePartyButton(PCBoxWork *work);
void PCBox_SlideOutPartyButton(BgCellManager *p);
void PCBox_SlideInPartyButton(BgCellManager *p);
void PCBox_ShowTrayTab(PCBoxWork *work);
void PCBox_HideTrayTab(PCBoxWork *work);
void PCBox_SlideOutTrayTab(BgCellManager *panels);
void PCBox_SlideInTrayTab(BgCellManager *panels);
void PCBox_ShowReturnButton(PCBoxWork *work);
void PCBox_ShowExitButton(PCBoxWork *work);
void PCBox_HideReturnButton(BgCellManager *panels);
void PCBox_SlideOutReturnButton(BgCellManager *panels);
void PCBox_SlideInReturnButton(BgCellManager *panels);
u32 PCBox_IsMenuExitButtonShown(BgCellManager *panels);
void PCBox_HidePartyButtonAndTrayTab(BgCellManager *panels);
void PCBox_HideActionMenu(BgCellManager *panels);
void PCBox_ShowActionMenu(BgCellManager *panels);
void PCBox_ResetActionMenu(BgCellManager *panels);
void PCBox_SlideInActionMenu(BgCellManager *panels);
void PCBox_SlideOutActionMenu(BgCellManager *panels);
u32 PCBox_UpdateActionMenuSlide(BgCellManager *panels);
u32 PCBox_IsActionMenuOpen(BgCellManager *panels);
void PCBox_SlideInTitlePanel(BgCellManager *panels);
void PCBox_SlideOutTitlePanel(BgCellManager *panels);
void PCBox_HideChangeBoxButton(BgCellManager *panels);
void PCBox_SlideInChangeBoxButton(BgCellManager *panels);
void PCBox_SlideOutChangeBoxButton(BgCellManager *panels);
u32 PCBox_IsChangeBoxButtonShown(BgCellManager *panels);
void PCBox_HideSummaryButton(BgCellManager *panels);
void PCBox_ShowSummaryButton(BgCellManager *panels);
void PCBox_SlideInSummaryButton(BgCellManager *panels);
void PCBox_SlideOutSummaryButton(BgCellManager *panels);
u32 PCBox_IsSummaryButtonShown(BgCellManager *panels);
void PCBox_ShowBoxTray(PCBoxWork *work);
void PCBox_SlideInBoxTray(BgCellManager *panels);
void PCBox_SlideOutBoxTray(BgCellManager *panels);
void PCBox_ShowBoxMenuPanels(BgCellManager *panels);
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
void ov14_021E954C(PCBoxWork *work);
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

// pc_box_states.c
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
u32 PCBox_StartFadeIn(PCBoxWork *work, u32 v);
u32 PCBox_StartFadeOut(PCBoxWork *work, u32 v);
u32 PCBox_WaitForUpdateFunc(PCBoxWork *work, void *cb, u32 value);
u32 PCBox_WaitForPendingUpdateFunc(PCBoxWork *work, u32 value);
int PCBox_ShowYesNo(PCBoxWork *work, int state);
u32 PCBox_StartSubApp(PCBoxWork *work, u32 value);
u32 PCBox_ScrollToPrevBox(PCBoxWork *work, u32 next);
u32 PCBox_ScrollToNextBox(PCBoxWork *work, u32 next);
u32 PCBox_StartMoveModeBoxDrag(PCBoxWork *work, u32 idx);
u32 PCBox_StartWithdrawDrag(PCBoxWork *work, u32 idx);
u32 PCBox_SelectWithdrawMon(PCBoxWork *work, u32 idx);
u32 PCBox_SelectMoveModeBoxMon(PCBoxWork *work, u32 idx);
u32 PCBox_StartMoveModePartyDrag(PCBoxWork *work, u32 idx);
u32 PCBox_StartDepositDrag(PCBoxWork *work, u32 idx);
u32 PCBox_SelectDepositMon(PCBoxWork *work, u32 idx);
u32 PCBox_SelectMoveModePartyMon(PCBoxWork *work, u32 idx);
u32 PCBox_StartBoxTrayDrag(PCBoxWork *work, u32 idx);
void PCBox_AllocMoveWork(PCBoxWork *work);
void PCBox_FreeMoveWork(PCBoxWork *work);
u32 PCBox_HandleItemCommand(PCBoxWork *work);
u32 PCBox_CloseMarkingMenu(PCBoxWork *work);
u32 PCBox_StartMarking(PCBoxWork *work);
u32 PCBox_StartRelease(PCBoxWork *work);
u32 PCBox_OpenPartyPanel(PCBoxWork *work);
u32 PCBox_ClosePartyPanel(PCBoxWork *work);
void PCBox_ShowPartyPanelLeft(PCBoxWork *work);
void PCBox_ShowPartyPanelRight(PCBoxWork *work);
void PCBox_ShowDepositPartyPanel(PCBoxWork *work);
u32 PCBox_OpenTrayFromPartyPanel(PCBoxWork *work);
u32 PCBox_CloseTrayToPartyPanel(PCBoxWork *work);
u32 PCBox_OpenTrayFromBox(PCBoxWork *work);
u32 PCBox_CloseTray(PCBoxWork *work);
u32 PCBox_HideInfoPanelAndWait(PCBoxWork *work, u32 next);
u32 PCBox_SelectTrayBox(PCBoxWork *work, s32 delta);
u32 PCBox_HideChangeBoxButtonAndWait(PCBoxWork *work, u32 next);
u32 PCBox_JumpToTrayBox(PCBoxWork *work, u32 which);
void PCBox_ChangeTrayPage(PCBoxWork *work, s32 delta);
u32 PCBox_OpenBoxTray(PCBoxWork *work, u32 next);
u32 PCBox_CloseBoxTray(PCBoxWork *work, u32 next);
u32 PCBox_ShowExitButtonAndWait(PCBoxWork *work, u32 next);
u32 PCBox_SlideOutReturnButtonAndWait(PCBoxWork *work, u32 next);
u32 PCBox_CloseActionMenuAndWait(PCBoxWork *work, u32 next);
u32 PCBox_StartBoxMenu(PCBoxWork *work);
u32 PCBox_SelectBoxMenuBox(PCBoxWork *work, s32 delta);
u32 PCBox_ChangeBoxMenuPage(PCBoxWork *work, s32 delta);
void PCBox_ChangeWallpaperPage(PCBoxWork *work, s32 delta);
void PCBox_SelectWallpaperInPage(PCBoxWork *work, u32 delta);
u32 PCBox_ConfirmWallpaperChoice(PCBoxWork *work);
u32 PCBox_TryWithdraw(PCBoxWork *work);
u32 PCBox_StartDeposit(PCBoxWork *work);
u32 PCBox_SelectDepositBox(PCBoxWork *work, s32 delta);
u32 PCBox_ChangeDepositBoxPage(PCBoxWork *work, s32 delta);
u32 PCBox_CancelDepositBoxSelect(PCBoxWork *work, u32 unused);
u32 PCBox_ConfirmDepositBox(PCBoxWork *work);
u32 PCBox_PickUpTrayMon(PCBoxWork *work, u32 value);
u32 PCBox_DropTrayMon(PCBoxWork *work, u32 index);
u32 PCBox_SelectItemModeBoxMon(PCBoxWork *work, u32 value);
u32 PCBox_StartItemModeBoxDrag(PCBoxWork *work, u32 value);
u32 PCBox_StartItemTrayDrag(PCBoxWork *work, u32 value);
u32 PCBox_PickUpTrayItem(PCBoxWork *work, u32 value);
u32 PCBox_ReturnTrayItem(PCBoxWork *work);
u32 PCBox_DropTrayItem(PCBoxWork *work, u32 value);
u32 PCBox_SelectItemModePartyMon(PCBoxWork *work, u32 value);
u32 PCBox_StartItemModePartyDrag(PCBoxWork *work, u32 value);
void PCBox_HandleBagGiveCommand(PCBoxWork *work);
void PCBox_AllocHeldItemMove(PCBoxWork *work);
void PCBox_FreeHeldItemMove(PCBoxWork *work);
u32 PCBox_State_BoxTrayHoldingInput(PCBoxWork *work);
u32 PCBox_State_EndCursorSlideToBoxTrayHolding(PCBoxWork *work);
u32 PCBox_TakeHeldItemToBag(PCBoxWork *work);
u32 PCBox_ConfirmRelease(PCBoxWork *work);
u32 PCBox_PutAwayHeldItem(PCBoxWork *work);
u32 PCBox_ConfirmExit(PCBoxWork *work);
u32 PCBox_CancelExit(PCBoxWork *work);
u32 PCBox_FlashPanel(PCBoxWork *work, u32 a1, u32 a2);
u32 ov14_021F2330(PCBoxWork *work, u32 a1, u32 a2);
u32 PCBox_FlashMarkingMenuButton(PCBoxWork *work, u32 a1, u32 a2);
u32 PCBox_FlashPartyPanelButton(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_DimForExitPrompt(PCBoxWork *work);
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
void PCBox_InitSprites(PCBoxWork *work);

// pc_box_sprites.c
void PCBox_FreeSprites(PCBoxView *view);
void PCBox_TickSprites(PCBoxView *view);
void PCBox_SetSpriteAnim(PCBoxView *view, u32 index, u32 anim);
u32 PCBox_IsSpriteAnimated(PCBoxView *view, u32 i);
void PCBox_SetSpriteVisible(PCBoxView *view, int index, int state);
int PCBox_IsSpriteVisible(PCBoxView *view, int index);
void PCBox_SetSpritePriority(PCBoxView *view, u32 i, u32 pri);
void PCBox_SetSpriteDrawPriority(PCBoxView *view, u32 slot, u32 priority);
void PCBox_SetSpriteTranslucent(PCBoxView *view, u32 slot, u32 enabled);
void PCBox_InitSpriteSystem(PCBoxView *view);
void PCBox_FreeSpriteSystem(PCBoxView *view);
void PCBox_LoadSpriteResources(PCBoxView *view);
void PCBox_CreateSprites(PCBoxWork *work);
void PCBox_DeleteSprite(PCBoxView *view, u32 slot);
void PCBox_DeleteAllSprites(PCBoxView *view);
void PCBox_LoadSpriteCharsMain(PCBoxView *view, int i, void *src, u32 size);
void PCBox_LoadSpriteCharsSub(PCBoxView *view, int i, void *src, u32 size);
void PCBox_LoadMonIconResources(PCBoxView *view);
void PCBox_CreateMonIconSprites(PCBoxView *view);
void *PCBox_LoadMonIconChars(PCBoxView *view, BoxPokemon *boxMon, NNSG2dCharacterData **charData);
void PCBox_LoadBoxIconImages(PCBoxWork *work, int box);
void PCBox_SetIconImage(PCBoxView *view, void *src, int i, int pal);
void PCBox_LoadMonIcon(PCBoxWork *work, BoxPokemon *boxMon, int index);
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
void PCBox_SetIconRangeTranslucent(PCBoxWork *work, u32 enabled, u32 first, u32 end, u32 check);
void PCBox_SetIconsTranslucent(PCBoxWork *work, u32 flags, u32 enabled);
void PCBox_SetSlotIconTranslucent(PCBoxView *view, u32 off, u32 v);
void PCBox_UpdateSlotIconItemTranslucency(PCBoxWork *work, u32 v);
void PCBox_LoadMonPicResources(PCBoxView *view);
void PCBox_CreateMonPicSprites(PCBoxWork *work);
void PCBox_LoadMonPic(PCBoxView *view, PCBoxMonInfo *monInfo, u32 slot);
void PCBox_ShowMonPic(PCBoxWork *work, PCBoxMonInfo *monInfo, u32 slot);
void PCBox_LoadItemIconResources(PCBoxView *view);
void PCBox_CreateItemIconSprites(PCBoxView *view);
void PCBox_LoadHeldItemIcon(PCBoxView *view, u32 item);
void PCBox_LoadInfoItemIcon(PCBoxView *view, u32 item);
void PCBox_SetHeldItemIconAffine(PCBoxView *view, u32 on);
void PCBox_SetHeldItemIconPosition(PCBoxView *view, s32 x, s32 y);
void PCBox_MoveHeldItemIconToSlot(PCBoxView *view, u32 index, u32 mode);
void PCBox_PlaceHeldItemIconAtSlot(PCBoxView *view, u32 index, u32 mode);
void PCBox_CreateHeldItemIconOutline(PCBoxView *view);
void PCBox_ShowHeldItemIconOutline(PCBoxView *view);
void PCBox_UpdateHeldItemIconOutline(PCBoxView *view);
void PCBox_UpdateInfoItemIconPosition(PCBoxView *view);
void PCBox_LoadTypeIconResources(PCBoxView *view);
void PCBox_CreateTypeIconSprites(PCBoxView *view);
void PCBox_LoadTypeIcon(PCBoxView *view, u32 a1, u32 index, u32 resourceId);
void PCBox_DrawTypeIcons(PCBoxView *view, PCBoxMonInfo *monInfo);
void PCBox_LoadUiSpriteResources(PCBoxView *view);
void PCBox_CreateUiSprites(PCBoxView *view);
void PCBox_CreateSelectedIconOutline(PCBoxWork *work);
void PCBox_HideSelectedIconOutline(PCBoxWork *work);
void PCBox_SetSelectedIconOutlineVisible(PCBoxWork *work, u32 a1);
void PCBox_UpdateSelectedIconOutline(PCBoxWork *work);
void PCBox_LoadTrayBoxResources(PCBoxView *view);
void PCBox_CreateTrayBoxSprites(PCBoxView *p);
void PCBox_LoadWallpaperListResources(PCBoxView *view);
void PCBox_CreateWallpaperListSprites(PCBoxView *p);
void PCBox_SetBoxArrowsVisible(PCBoxView *view, u32 a1);
void PCBox_PlaceWallpaperMenuTextObj(PCBoxView *view);
void PCBox_SetupWallpaperMenuSprites(PCBoxWork *work);
void PCBox_MoveWallpaperMenuSprites(PCBoxView *view, s32 dy);
void PCBox_UpdateCurrentWallpaperMarker(PCBoxWork *work);
void PCBox_UpdateWallpaperCursor(PCBoxWork *work);
void PCBox_LoadWallpaperThumbnails(PCBoxWork *work);
void PCBox_ColorWallpaperThumbnail(PCBoxWork *work, u8 *buf, u32 wallpaper, u32 match, u32 size);
void PCBox_PlaceTrayTextObjs(PCBoxView *view);
void PCBox_SetupBoxTraySprites(PCBoxWork *work);
void PCBox_MoveBoxTraySprites(PCBoxWork *work, s32 dy);
void PCBox_UpdateCurrentBoxMarker(PCBoxWork *work);
void PCBox_UpdateTrayCursor(PCBoxWork *work);
void PCBox_GetTrayBoxPosition(PCBoxView *view, u32 i, s16 *x, s16 *y);
void PCBox_DrawBoxThumbnail(PCBoxWork *work, int n);
void PCBox_DrawAllBoxThumbnails(PCBoxWork *work);

// pc_box_text.c
void PCBox_LoadTrayPageThumbnails(PCBoxWork *work);
void PCBox_RefreshTrayBoxThumbnail(PCBoxWork *work, u32 box);
void PCBox_DrawBoxThumbnailMons(PCBoxWork *work, u32 box, void *dst);
void PCBox_PlaceSpriteOnTop(PCBoxView *view, u32 i, s32 x, s32 y, u32 v);
void PCBox_SetupDepositBoxSprites(PCBoxWork *work);
void PCBox_HideBoxTraySprites(PCBoxWork *work);
void PCBox_CreateTrayTextObjs(PCBoxView *view);
void PCBox_DestroyTrayTextObjs(PCBoxView *view);
void PCBox_CopyWindowToTextObj(PCBoxView *view, Window *window, u32 index);
void PCBox_AddWindows(PCBoxWork *work);
void PCBox_RemoveWindows(PCBoxWork *work);
void PCBox_PrintAligned(Window *window, String *str, s32 x, s32 y, u32 font, u32 color, u32 align);
void PCBox_PrintMsg(PCBoxView *view, void *msgData, u32 win, u32 msgId, s32 x, s32 y, u32 font, u32 color, u32 align);
void PCBox_PrintExpandedMsg(PCBoxView *view, void *msgData, u32 win, u32 msgId, s32 x, s32 y, u32 font, u32 color, u32 align);
void PCBox_PrintSpeciesName(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win);
void PCBox_PrintNickname(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win);
void PCBox_PrintLevel(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win);
void PCBox_PrintGender(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win);
void PCBox_PrintDexNumber(PCBoxWork *work, PCBoxMonInfo *monInfo, u32 win);
void PCBox_PrintNature(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win);
void PCBox_PrintAbility(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win);
void PCBox_PrintHeldItem(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win);
void PCBox_PrintMonInfo(PCBoxWork *work, PCBoxMonInfo *monInfo);
void PCBox_ClearMonInfoWindows(PCBoxView *view);
int PCBox_DrawMoveWindows(PCBoxWork *work, PCBoxMonInfo *monInfo);
u32 PCBox_PrintItemNameAndDesc(PCBoxWork *work, u16 itemId);
void PCBox_PrintModeTitle(PCBoxWork *work);
void PCBox_PrintInfoLabels(PCBoxWork *work);
void PCBox_DrawBoxNameOnWallpaper(PCBoxWork *work, void *src, u32 baseTile, u32 width, u32 height);
void PCBox_DrawTrayBoxNameAndCount(PCBoxWork *work);
void PCBox_DrawWallpaperName(PCBoxWork *work);
void PCBox_DrawButtonWindow(PCBoxWork *work, u32 win, u32 msgId, u32 style);
void ov14_021F5BD8(PCBoxWork *work, u32 win, u32 msgId, u32 style, u8 align);
void PCBox_DrawPartyButton(PCBoxWork *work, u32 style);
void PCBox_DrawTabWindow(PCBoxWork *work, u32 win, u32 msgId);
void PCBox_DrawTrayTab(PCBoxWork *work, u32 unused);
void PCBox_DrawReturnButton(PCBoxWork *work, u32 style);
void PCBox_DrawExitButton(PCBoxWork *work, u32 style);
void PCBox_DrawMenuExitButton(PCBoxWork *work, u32 style);
void PCBox_DrawActionMenu(PCBoxWork *work, const PCBoxMenuEntry *list, u32 count);
void PCBox_DrawBagGiveButton(PCBoxWork *work, u32 which);
void PCBox_DrawChangeBoxButton(PCBoxWork *work);
void PCBox_DrawChangeBoxTitle(PCBoxWork *work);
void PCBox_DrawBoxMenu(PCBoxWork *work);
void PCBox_DrawChangeWallpaperTitle(PCBoxWork *work);
void PCBox_DrawDepositTitle(PCBoxWork *work);
void PCBox_SetPanelWindowFrame(BgCellManager *mgr, int id, Window *win);
void PCBox_AttachWindowToPanel(BgCellManager *mgr, int id, Window *window);
void PCBox_AttachPanelWindows(PCBoxView *view);
void PCBox_AttachPartyButtonWindow(PCBoxView *view);
void PCBox_AttachTrayTabWindow(PCBoxView *view);
void PCBox_AttachReturnButtonWindow(PCBoxView *view);
void PCBox_AttachMenuExitButtonWindow(PCBoxWork *work);
void PCBox_PrintCenteredLabel(PCBoxView *view, u32 windowIndex, u32 messageId, u32 unused);
void PCBox_DrawConfirmCancel(PCBoxView *view);
void PCBox_PrintPartyPanelSwitchLabel(PCBoxView *view);
void PCBox_PrintPartyPanelExitLabel(PCBoxView *view);
void PCBox_AttachMarkingMenuWindows(PCBoxView *view);
void PCBox_AttachPartyPanelLabelWindow(PCBoxView *view);
void PCBox_DrawSummaryButton(PCBoxWork *work, u32 unused);
MsgData *PCBox_LoadBoxMessages(void);
void PCBox_DrawMessageFrame(Window *window);
void PCBox_CloseMessageWindow(PCBoxView *view, int windowId);
void PCBox_ClearWindowFrameNoTransfer(PCBoxView *view, u32 idx);
void PCBox_ClearWindowFrame(PCBoxView *view, u32 idx);
void PCBox_PrintMessage(PCBoxWork *work, int msgNo, int windowId);
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

// pc_box_grid_input.c
void PCBox_GridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_GridOnTouch(PCBoxWork *work, u32 a1);
u32 PCBox_GetDepositMenuInput(PCBoxWork *work);
void PCBox_DepositGridOnButton(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_DepositBoxGridOnButton(PCBoxWork *work, u32 a1);
void PCBox_DepositBoxGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_DepositBoxGridOnTouch(PCBoxWork *work, u32 a1);
u32 PCBox_GetWithdrawMenuInput(PCBoxWork *work);
void PCBox_WithdrawGridOnButton(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_WithdrawGridOnTouch(PCBoxWork *work, u32 a1);
u32 PCBox_GetMoveModeMenuInput(PCBoxWork *work);
void PCBox_MoveModeGridOnButton(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_MoveModeGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_MoveModeGridOnTouch(PCBoxWork *work, u32 a1);
u32 PCBox_GetBoxTrayInput(PCBoxWork *work);
void PCBox_TrayGridOnButton(PCBoxWork *work, u32 a1);
void PCBox_TrayGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_TrayGridOnTouch(PCBoxWork *work, u32 a1);
u32 PCBox_GetTrayHoldingInput(PCBoxWork *work);
u32 PCBox_GetMoveModePartyMenuInput(PCBoxWork *work);
void PCBox_MoveModePartyGridOnButton(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_MoveModePartyGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
u32 PCBox_GetItemModeMenuInput(PCBoxWork *work);
void PCBox_ItemModeGridOnButton(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_ItemModeGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_ItemModeGridOnTouch(PCBoxWork *work, u32 a1);
u32 PCBox_GetItemModePartyMenuInput(PCBoxWork *work);
void PCBox_ItemModePartyGridOnButton(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_ItemModePartyGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_BoxMenuGridOnButton(PCBoxWork *work, u32 a1);
void PCBox_BoxMenuGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_BoxMenuGridOnTouch(PCBoxWork *work, u32 a1);
void PCBox_WallpaperGridOnButton(PCBoxWork *work, u32 a1);
void PCBox_WallpaperGridOnKeyMove(PCBoxWork *work, u32 a1, u32 a2);
void PCBox_WallpaperGridOnTouch(PCBoxWork *work, u32 a1);
u32 PCBox_GetItemTrayHoldingInput(PCBoxWork *work);
void PCBox_StartCursorSlide(PCBoxView *view, u32 from, u32 to);
u32 PCBox_IsSummaryShortcutPressed(PCBoxWork *work);

#endif // POKEHEARTGOLD_APPLICATION_PC_BOX_PC_BOX_INTERNAL_H
