#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_H

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "grid_input_handler.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "pokemon.h"
#include "save.h"
#include "save_frontier.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"

typedef struct BattleCastleSprite {
    s16 x;
    s16 y;
    Sprite *sprite;
} BattleCastleSprite;

typedef struct BattleCastlePrompt {
    YesNoPrompt *prompt;
    u32 active;
    BgConfig *bgConfig;
} BattleCastlePrompt;

typedef struct BattleCastleSpriteGfx {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObj[14][4];
} BattleCastleSpriteGfx;

typedef union BattleCastlePartyAppFlags {
    u8 raw;
    struct {
        u8 unk_0 : 1;
        u8 linkSyncPending : 1; // Main goes to the link exchange state
        u8 effectActive : 1;
        u8 itemMenuOpen : 1;
        u8 listMenuOpen : 1;
        u8 linkResume : 2; // menu to reopen after a link exchange
        u8 unk_7 : 1;
    } bits;
} BattleCastlePartyAppFlags;

typedef struct BattleCastleMonSummary {
    Pokemon *mon;
    BoxPokemon *boxMon;
    u16 species;
    u8 gender : 7;
    u8 hideGender : 1;
    u8 level;
    u8 ability;
    u8 nature;
    u16 heldItem;
    u32 personality;
    u16 hp;
    u16 maxHp;
    u16 attack;
    u16 spAttack;
    u16 defense;
    u16 spDefense;
    u16 speed;
    u8 form;
    u8 unk_23;
    u16 moves[4];
    u8 pp[4];
    u8 maxPp[4];
} BattleCastleMonSummary;

enum CastleButtonAnimState {
    CASTLE_BUTTON_ANIM_HIGHLIGHT,
    CASTLE_BUTTON_ANIM_RESTORE,
    CASTLE_BUTTON_ANIM_WAIT,
};

typedef struct BattleCastlePartyApp {
    OverlayManager *manager;
    FrontierSave *frontierSave;
    u8 subState;
    u8 frontierType;
    u8 printerId;
    u8 timer;
    u8 prevCursorPos;
    u8 cursorPos; // written back through selectionOut on exit
    BattleCastlePartyAppFlags flags;
    u8 linkRecvCount; // incremented by the link receive callbacks
    u16 selectedItem;
    u8 linkSlot; // slot of the pending link action, 0xFF = none
    u8 action;   // menu action id: 1-3 heal, 4/8 rank up, 6/7 rental, 9/10 summary
    u8 numMons;  // 3, or 4 for the link challenge
    u8 numOwnSlots;
    u8 unk_016[6];
    MsgData *itemDescMsgData;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *expandedString;
    String *rawString;
    String *strings[3];
    u8 unk_03C[0x10];
    BgConfig *bgConfig;
    Window windows[70]; // [6] is the message box, [18-47] the summary, [48-53] the item grid
    u8 unk_4B0[0x2C];
    ListMenuItem *itemList; // rental items
    u8 unk_4E0[0x20];
    PaletteData *paletteData;
    MessagePrinter *msgPrinter;
    Options *options;
    SaveData *saveData;
    void *castleChallenge;
    u32 castleFlags;
    BattleCastleSpriteGfx spriteGfx;
    BattleCastleSprite *unk_734;
    BattleCastleSprite *unk_738;
    BattleCastleSprite *monIcons[4];
    BattleCastleSprite *heldItemIcons[4];
    BattleCastleSprite *effectSprite;
    BattleCastleSprite *cursor;
    BattleCastleSprite *partnerCursor;
    BattleCastleSprite *hpBars[4];
    BattleCastleSprite *menuOpenSprite;
    BattleCastleSprite *itemCursor;
    BattleCastleSprite *monPic;
    BattleCastleSprite *itemIcons[6];
    BattleCastleSprite *selectedItemIcon;
    u16 *selectionOut;
    Party *party;
    NARC *narc;
    u16 linkSendBuf[20];
    u8 unk_7D4[0x28];
    u8 partnerCursorPos;
    u8 partnerSlot;
    u8 partnerExited;
    u8 partnerRanks[3];
    u16 partnerPoints;
    BattleCastleMonSummary summary;
    GridInputHandler *partyGrid;
    TouchscreenListMenuSpawner *listMenuSpawner;
    TouchscreenListMenu *listMenu;
    ListMenuItem *listMenuItems;
    u32 listCursorPos;
    BattleCastlePrompt yesNoPrompt;
    u8 unk_858[4];
    GridInputHandler *itemGrid;
    u8 rentalItemIndex;
    u8 numItems;
    s16 itemPage;
    u16 pressedButton;
    u8 buttonAnimState;
    u8 buttonAnimTimer;
    u32 bgToggleRequests; // 4 bits per BG layer, applied in VBlank
} BattleCastlePartyApp;

BOOL BattleCastlePartyApp_AnimatePressedButton(BattleCastlePartyApp *app);
void BattleCastle_InitSpriteGfx(BattleCastleSpriteGfx *gfx, Party *party, u8 isLink);
void BattleCastle_FreeSpriteGfx(BattleCastleSpriteGfx *gfx);
void BattleCastle_LoadItemIconChar(BattleCastleSpriteGfx *gfx, int resId, u16 item);
void BattleCastle_LoadItemIconPalette(BattleCastleSpriteGfx *gfx, int resId, u16 item);
BattleCastleSprite *BattleCastleSprite_New(BattleCastleSpriteGfx *gfx, int charId, int plttId, int cellId, int animSeq, s16 x, s16 y, int priority, int unused);
BattleCastleSprite *BattleCastleSprite_NewSubScreen(BattleCastleSpriteGfx *gfx, int charId, int plttId, int cellId, int animSeq, s16 x, s16 y, int priority, int unused);
int BattleCastleSprite_Delete(BattleCastleSprite *sprite);
void BattleCastleSprite_SetDrawFlag(BattleCastleSprite *sprite, int draw);
void BattleCastleSprite_SetPosition(BattleCastleSprite *sprite, int x, int y);
void BattleCastleSprite_SetPositionSubScreen(BattleCastleSprite *sprite, int x, int y);
void BattleCastleSprite_SetAnim(BattleCastleSprite *sprite, int seq);
void BattleCastleSprite_SetMonIconPalette(BattleCastleSprite *sprite, Pokemon *mon);
void BattleCastleSprite_ChangeAnim(BattleCastleSprite *sprite, int value);
void BattleCastleSprite_SetBounce(BattleCastleSprite *sprite, int value);
int BattleCastleSprite_IsAnimated(BattleCastleSprite *sprite);
void BattleCastleSprite_ShowAnim11WithSE(BattleCastleSprite *sprite, int x, u32 y);
void BattleCastleSprite_LoadMonPic(BattleCastleSprite *sprite, BoxPokemon *boxMon, u16 species, u32 personality);
void BattleCastle_SetSpritesTranslucent(BattleCastleSprite **sprites, u32 count, int mode);
u32 BattleCastle_GetMonIndex(u32 numMons, u32 slot);
u32 BattleCastle_GetSideSlot(u32 numOwnSlots, u32 slot);
u8 BattleCastle_GetRank(SaveData *saveData, u8 frontierType, u8 kind);
void BattleCastle_PlaySEOnInput(int input, u16 seqNo);
void BattleCastle_BufferPartnerName(MessageFormat *msgFormat, int field);
void BattleCastle_ClearBgToggleRequests(u32 *requests);
void BattleCastle_RequestBgToggle(int layer, int value, u32 *requests);
void BattleCastle_ApplyBgToggleRequests(u32 *requests);
void BattleCastlePrompt_Init(BattleCastlePrompt *prompt);
void BattleCastlePrompt_Free(BattleCastlePrompt *prompt);
void BattleCastlePrompt_Show(BattleCastlePrompt *prompt, BgConfig *bgConfig);
void BattleCastlePrompt_Hide(BattleCastlePrompt *prompt);
void BattleCastle_AddWindows(BgConfig *bgConfig, Window *windows, u32 set);
void BattleCastle_RemoveWindows(Window *windows, u32 set);
void BattleCastle_DrawWindowFrame(Window *window, int frame);
void BattleCastle_GetMenuTiles(u16 *tileStart, u16 *tileCount);
void BattleCastle_PrintString(Window *window, String *string, int x, int y, int fontId, int color, int alignment);
void BattleCastle_PrintMsg(Window *window, MsgData *msgData, s32 msgNo, int x, int y, int fontId, int color, int alignment);
void BattleCastle_FreeGrid(GridInputHandler *grid);
GridInputHandler *BattleCastlePartyApp_CreatePartyGrid(BattleCastlePartyApp *app, int initialPos, int numMons);
int BattleCastle_HandlePartyGridInput(GridInputHandler *grid);
void BattleCastle_SetGridInputsToNext(GridInputHandler *grid);
GridInputHandler *BattleCastlePartyApp_CreateItemGrid(BattleCastlePartyApp *app);
int BattleCastle_HandleItemGridInput(GridInputHandler *grid);
TouchscreenListMenuSpawner *BattleCastle_CreateListMenuSpawner(SpriteList *spriteList, PaletteData *paletteData);
void BattleCastle_DestroyListMenuSpawner(TouchscreenListMenuSpawner *spawner);
TouchscreenListMenu *BattleCastle_CreateListMenu(TouchscreenListMenuSpawner *spawner, TouchscreenListMenuHeader *header, u8 x, u8 y, u8 width);
void BattleCastle_DestroyListMenu(TouchscreenListMenu *menu);
BOOL BattleCastle_IsAdvancePressed(void);

#define CASTLE_ALIGN_LEFT   0
#define CASTLE_ALIGN_RIGHT  1
#define CASTLE_ALIGN_CENTER 2

typedef union BattleCastleOpponentAppFlags {
    u8 raw;
    struct {
        u8 effectActive : 1;
        u8 unk_1 : 1;
        u8 listMenuOpen : 1;
        u8 linkResume : 5; // CASTLE_OPP_RESUME_*
    } bits;
} BattleCastleOpponentAppFlags;

typedef struct BattleCastleOpponentApp {
    OverlayManager *manager;
    FrontierSave *frontierSave;
    u8 subState;
    u8 frontierType;
    u8 printerId;
    u8 unk_00B;
    u8 prevCursorPos;
    u8 cursorPos;   // written back through selectionOut on exit
    u8 levelChange; // CASTLE_LEVEL_UP / DOWN chosen in the LEVEL menu
    BattleCastleOpponentAppFlags flags;
    u8 linkActionPending; // Main goes to the link exchange state
    u8 linkSlot;          // slot of the pending link action, 0xFF = none
    u8 linkLevelChange;
    u8 action;      // CASTLE_OPP_ACTION_*
    u8 numMons;     // 3, or 4 for the link challenge
    u8 numOwnSlots; // grid positions from here on are the EXIT button and the partner's slots
    u8 timer;
    u8 linkRecvCount; // incremented by the link receive callbacks
    u8 unk_018[8];
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *expandedString;
    String *rawString;
    String *strings[3];
    u8 unk_03C[0x10];
    BgConfig *bgConfig;
    Window windows[35]; // window set 1; [7] is the message box, [12-34] the summary
    u8 unk_280[0x30];
    PaletteData *paletteData;
    MessagePrinter *msgPrinter;
    Options *options;
    SaveData *saveData;
    void *castleChallenge;
    u32 castleFlags;
    BattleCastleSpriteGfx spriteGfx;
    BattleCastleSprite *monIcons[4];
    BattleCastleSprite *hiddenMonIcons[4]; // shown until the mon is checked
    BattleCastleSprite *effectSprite;
    BattleCastleSprite *cursor;
    BattleCastleSprite *partnerCursor;
    BattleCastleSprite *hpBars[4];
    BattleCastleSprite *boughtMarks[4][2]; // [0] stats bought, [1] moves bought
    BattleCastleSprite *menuOpenSprite;
    BattleCastleSprite *monPic;
    u16 *selectionOut;
    u8 *checked;      // per mon, identity checked
    u8 *levelChanges; // per mon, CASTLE_LEVEL_*
    u8 *statsBought;
    u8 *movesBought;
    Party *party; // the opponent's party
    NARC *narc;
    u16 linkSendBuf[20];
    u8 unk_58C[0x28];
    u8 partnerCursorPos;
    u8 partnerSlot;
    u8 partnerExited;
    u8 partnerRanks[3];
    u16 partnerPoints;
    BattleCastleMonSummary summary;
    GridInputHandler *partyGrid;
    TouchscreenListMenuSpawner *listMenuSpawner;
    TouchscreenListMenu *listMenu;
    ListMenuItem *listMenuItems;
    u32 listCursorPos;
    BattleCastlePrompt yesNoPrompt;
    u8 unk_610[4];
} BattleCastleOpponentApp;
void BattleCastlePartyApp_SetCursorPos(BattleCastlePartyApp *app, u32 cursorPos, u32 prevCursorPos);
void BattleCastlePartyApp_SetItemCursor(BattleCastlePartyApp *app, int x, int y, int animSeq);
void BattleCastlePartyApp_ShowItemDescription(BattleCastlePartyApp *app, int index);
void BattleCastleOpponentApp_SetCursorPos(BattleCastleOpponentApp *app, u32 cursorPos, u32 prevCursorPos);
GridInputHandler *BattleCastleOpponentApp_CreatePartyGrid(BattleCastleOpponentApp *app, int initialPos, int numMons);

BOOL BattleCastlePartyApp_Init(OverlayManager *manager, int *state);
int BattleCastlePartyApp_Main(OverlayManager *manager, int *state);
BOOL BattleCastlePartyApp_Exit(OverlayManager *manager, int *state);

BOOL BattleCastleOpponentApp_Init(OverlayManager *manager, int *state);
BOOL BattleCastleOpponentApp_Main(OverlayManager *manager, int *state);
BOOL BattleCastleOpponentApp_Exit(OverlayManager *manager, int *state);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_H
