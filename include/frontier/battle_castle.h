#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_H

#include "bg_window.h"
#include "gf_gfx_planes.h"
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
#include "touchscreen_list_menu.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "grid_input_handler.h"
#include "yes_no_prompt.h"

// Battle Castle (overlay 83): the party app (battle_castle_party_app.c) and the
// opponent app (battle_castle_opponent_app.c), which also holds the helpers both use.

// Sprite created by BattleCastleSprite_New / BattleCastleSprite_NewSubScreen (also used by the opponent app)
typedef struct BattleCastleSprite {
    s16 x;            // 0x0
    s16 y;            // 0x2
    Sprite *sprite;   // 0x4
} BattleCastleSprite; // size 0x8

// Yes/No prompt wrapper (BattleCastlePrompt_Init / BattleCastlePrompt_Show / BattleCastlePrompt_Hide)
typedef struct BattleCastlePrompt {
    YesNoPrompt *prompt; // 0x0
    u32 active;          // 0x4
    BgConfig *bgConfig;  // 0x8
} BattleCastlePrompt;    // size 0xC

// Sprite renderer and resources, set up by BattleCastle_InitSpriteGfx, freed by BattleCastle_FreeSpriteGfx
typedef struct BattleCastleSpriteGfx {
    SpriteList *spriteList;        // 0x000
    G2dRenderer renderer;          // 0x004
    GF_2DGfxResMan *resMan[4];     // 0x12C
    SpriteResource *resObj[14][4]; // 0x13C
} BattleCastleSpriteGfx;           // size 0x21C

// app->flags (0x0E); also read and written as a whole byte
typedef union BattleCastlePartyAppFlags {
    u8 raw;
    struct {
        u8 unk_0 : 1;
        u8 linkSyncPending : 1; // Main goes to the link exchange state (BattleCastlePartyApp_LinkAction)
        u8 effectActive : 1;    // app->effectSprite is playing (BattleCastlePartyApp_PlayActionEffect)
        u8 itemMenuOpen : 1;    // rental item grid is open (BattleCastlePartyApp_OpenItemGrid / BattleCastlePartyApp_CloseItemGrid)
        u8 listMenuOpen : 1;    // touchscreen list menu is open (BattleCastlePartyApp_CreateListMenu / BattleCastlePartyApp_DestroyListMenu)
        u8 linkResume : 2;      // menu to reopen after a link exchange (BattleCastlePartyApp_HandleMenu state 0)
        u8 unk_7 : 1;
    } bits;
} BattleCastlePartyAppFlags;

// Summary of the selected mon, filled by BattleCastlePartyApp_LoadMonSummary
typedef struct BattleCastleMonSummary {
    Pokemon *mon;         // 0x00 (app 0x804)
    BoxPokemon *boxMon;   // 0x04
    u16 species;          // 0x08
    u8 gender : 7;        // 0x0A
    u8 unk_0A_7 : 1;      // 0 when MON_DATA_NO_PRINT_GENDER == 1; gender is printed only when 0
    u8 level;             // 0x0B
    u8 ability;           // 0x0C
    u8 nature;            // 0x0D
    u16 heldItem;         // 0x0E
    u32 personality;      // 0x10
    u16 hp;               // 0x14
    u16 maxHp;            // 0x16
    u16 attack;           // 0x18
    u16 spAttack;         // 0x1A
    u16 defense;          // 0x1C
    u16 spDefense;        // 0x1E
    u16 speed;            // 0x20
    u8 form;              // 0x22
    u8 unk_23;            // 0x23
    u16 moves[4];         // 0x24
    u8 pp[4];             // 0x2C
    u8 maxPp[4];          // 0x30
} BattleCastleMonSummary; // size 0x34

typedef struct BattleCastlePartyApp {
    OverlayManager *manager;                     // 0x000
    FrontierSave *frontierSave;                  // 0x004
    u8 subState;                                 // 0x008
    u8 frontierType;                             // 0x009 (args 0x04; BattleCastle_IsMultiType() == TRUE for the link challenge)
    u8 printerId;                                // 0x00A
    u8 timer;                                    // 0x00B
    u8 prevCursorPos;                            // 0x00C
    u8 cursorPos;                                // 0x00D (written back through selectionOut on exit)
    BattleCastlePartyAppFlags flags;             // 0x00E
    u8 linkRecvCount;                            // 0x00F (incremented by the link receive callbacks)
    u16 selectedItem;                            // 0x010
    u8 unk_012;                                  // 0x012 (slot of the pending link action, 0xFF = none)
    u8 action;                                   // 0x013 (menu action id: 1-3 heal, 4/8 rank up, 6/7 rental, 9/10 summary)
    u8 numMons;                                  // 0x014 (3, or 4 for the link challenge)
    u8 unk_015;                                  // 0x015 (4)
    u8 unk_016[6];                               // 0x016
    MsgData *itemDescMsgData;                    // 0x01C (msg_0221)
    MsgData *msgData;                            // 0x020 (msg_0031)
    MessageFormat *msgFormat;                    // 0x024
    String *expandedString;                      // 0x028
    String *rawString;                           // 0x02C
    String *strings[3];                          // 0x030
    u8 unk_03C[0x10];                            // 0x03C
    BgConfig *bgConfig;                          // 0x04C
    Window windows[70];                          // 0x050 ([6] at 0x0B0 is the message box; [18-47] summary; [48-53] item grid)
    u8 unk_4B0[0x2C];                            // 0x4B0
    ListMenuItem *itemList;                      // 0x4DC (rental items)
    u8 unk_4E0[0x20];                            // 0x4E0
    PaletteData *paletteData;                    // 0x500
    MessagePrinter *msgPrinter;                  // 0x504
    Options *options;                            // 0x508
    SaveData *saveData;                          // 0x50C
    void *unk_510;                               // 0x510 (Save_Frontier_GetCastleChallenge)
    u32 unk_514;                                 // 0x514 (Save_Frontier_GetCastleFlags)
    BattleCastleSpriteGfx spriteGfx;             // 0x518
    BattleCastleSprite *unk_734;                 // 0x734
    BattleCastleSprite *unk_738;                 // 0x738
    BattleCastleSprite *monIcons[4];             // 0x73C
    BattleCastleSprite *heldItemIcons[4];        // 0x74C
    BattleCastleSprite *effectSprite;            // 0x75C
    BattleCastleSprite *cursor;                  // 0x760
    BattleCastleSprite *partnerCursor;           // 0x764
    BattleCastleSprite *hpBars[4];               // 0x768
    BattleCastleSprite *unk_778;                 // 0x778
    BattleCastleSprite *itemCursor;              // 0x77C
    BattleCastleSprite *monPic;                  // 0x780
    BattleCastleSprite *itemIcons[6];            // 0x784
    BattleCastleSprite *selectedItemIcon;        // 0x79C
    u16 *selectionOut;                           // 0x7A0 (args + 0x20)
    Party *party;                                // 0x7A4 (args 0x18)
    NARC *narc;                                  // 0x7A8
    u16 linkSendBuf[20];                         // 0x7AC (Comm_SendCommand payload, 0x28 bytes)
    u8 unk_7D4[0x28];                            // 0x7D4
    u8 partnerCursorPos;                         // 0x7FC
    u8 unk_7FD;                                  // 0x7FD
    u8 unk_7FE;                                  // 0x7FE
    u8 partnerRanks[3];                          // 0x7FF
    u16 partnerPoints;                           // 0x802 (args 0x28)
    BattleCastleMonSummary summary;              // 0x804
    GridInputHandler *partyGrid;                 // 0x838
    TouchscreenListMenuSpawner *listMenuSpawner; // 0x83C
    TouchscreenListMenu *listMenu;               // 0x840
    ListMenuItem *listMenuItems;                 // 0x844
    u32 listCursorPos;                           // 0x848 (one piece writes it as int)
    BattleCastlePrompt yesNoPrompt;              // 0x84C
    u8 unk_858[4];                               // 0x858
    GridInputHandler *itemGrid;                  // 0x85C
    u8 unk_860;                                  // 0x860
    u8 numItems;                                 // 0x861
    s16 itemPage;                                // 0x862 (one piece writes it as u16)
    u16 pressedButton;                           // 0x864
    u8 buttonAnimState;                          // 0x866
    u8 buttonAnimTimer;                          // 0x867
    u32 bgToggleRequests;                        // 0x868 (4 bits per BG layer, applied in VBlank by BattleCastle_ApplyBgToggleRequests)
} BattleCastlePartyApp;                          // size 0x86C

// Defined in battle_castle_opponent_app.c
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
void ov83_02247600(BattleCastleSprite *sprite, int value);
void ov83_0224760C(BattleCastleSprite *sprite, int value);
int BattleCastleSprite_IsAnimated(BattleCastleSprite *sprite);
void ov83_02247630(BattleCastleSprite *sprite, int x, u32 y);
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
void ov83_02247B04(GridInputHandler *grid);
GridInputHandler *BattleCastlePartyApp_CreateItemGrid(BattleCastlePartyApp *app);
int BattleCastle_HandleItemGridInput(GridInputHandler *grid);
TouchscreenListMenuSpawner *BattleCastle_CreateListMenuSpawner(SpriteList *spriteList, PaletteData *paletteData);
void BattleCastle_DestroyListMenuSpawner(TouchscreenListMenuSpawner *spawner);
TouchscreenListMenu *BattleCastle_CreateListMenu(TouchscreenListMenuSpawner *spawner, TouchscreenListMenuHeader *header, u8 x, u8 y, u8 width);
void BattleCastle_DestroyListMenu(TouchscreenListMenu *menu);
BOOL BattleCastle_IsAdvancePressed(void);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_CASTLE_H
