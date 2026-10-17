#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_APP_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_APP_INTERNAL_H

#include "global.h"

#include "battle_frontier_communication.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "grid_input_handler.h"
#include "list_menu_2d.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_summary.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "yes_no_prompt.h"

// app->screen
#define FACTORY_SCREEN_RENTAL 0 // choose the rental Pokemon
#define FACTORY_SCREEN_SWAP   1 // trade one of the party for an opponent's Pokemon

// A sprite placed by its top-left corner in pixels
typedef struct BattleFactorySprite {
    int x;          // 0x0
    int y;          // 0x4
    Sprite *sprite; // 0x8
} BattleFactorySprite;

// One of the Poke Balls on the conveyor
typedef struct BattleFactoryBall {
    u16 selected;   // 0x0
    int x;          // 0x4
    int y;          // 0x8
    Sprite *sprite; // 0xC
} BattleFactoryBall;

typedef struct BattleFactorySpriteGfx {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObj[6][4];
} BattleFactorySpriteGfx;

typedef struct BattleFactoryLinkMessage {
    u16 command;  // 0x0
    u16 accepted; // 0x2
    u16 picked0;  // 0x4
    u16 picked1;  // 0x6
} BattleFactoryLinkMessage;

typedef struct BattleFactoryApp {
    OverlayManager *manager;    // 0x000
    OverlayManager *summaryApp; // 0x004
    u8 subState;                // 0x008
    u8 frontierType;            // 0x009
    u8 levelMode;               // 0x00A
    u8 screen;                  // 0x00B
    int bgScrollX;              // 0x00C
    u8 printerId;               // 0x010
    u8 numPicked;               // 0x011
    u8 numBalls;                // 0x012
    u8 cancelRequested : 1;     // 0x013
    u8 summaryOpen : 1;
    u8 swapOutPicked : 1;
    u8 swapAccepted : 1;
    u8 shown : 1;
    u8 switchingPage : 1;
    u8 summaryFading : 1;
    u8 unk_013_7 : 1;
    u32 bgAnimTimer;                      // 0x014
    u8 numPartnerMons;                    // 0x018
    u8 timer;                             // 0x019
    u8 unk_01A;                           // 0x01A
    u8 unk_01B;                           // 0x01B
    MsgData *msgData;                     // 0x01C
    MessageFormat *msgFormat;             // 0x020
    String *expandedString;               // 0x024
    String *rawString;                    // 0x028
    String *strings[4];                   // 0x02C
    u8 unk_03C[0x10];                     // 0x03C
    BgConfig *bgConfig;                   // 0x04C
    Window windows[18];                   // 0x050
    u8 unk_170[4];                        // 0x170
    Window *unk_174;                      // 0x174
    u8 unk_178[4];                        // 0x178
    struct ListMenu2D *unk_17C;           // 0x17C
    u8 unk_180[0x20];                     // 0x180
    PaletteData *paletteData;             // 0x1A0
    GF3DVramMan *vramMan;                 // 0x1A4
    PokepicManager *pokepicManager;       // 0x1A8
    Pokepic *pokepics[3];                 // 0x1AC
    Options *options;                     // 0x1B8
    SaveData *saveData;                   // 0x1BC
    PokemonSummaryArgs *summaryArgs;      // 0x1C0
    BattleFactorySpriteGfx spriteGfx;     // 0x1C4
    BattleFactoryBall *balls[6];          // 0x360
    BattleFactoryBall *unk_378[2];        // 0x378
    BattleFactorySprite *centerSprite;    // 0x380
    BattleFactorySprite *unk_384;         // 0x384
    BattleFactorySprite *cursor;          // 0x388
    BattleFactorySprite *buttonCursor;    // 0x38C
    BattleFactorySprite *picCursor;       // 0x390
    BattleFactorySprite *monIcons[2];     // 0x394
    BattleFactorySprite *slotMarkers[3];  // 0x39C
    BattleFactorySprite *orderSprites[6]; // 0x3A8
    Party *party;                         // 0x3C0
    Party *otherParty;                    // 0x3C4
    u16 picked[6];                        // 0x3C8
    u16 *resultOut;                       // 0x3D4
    u32 summaryResult;                    // 0x3D8
    NARC *narc;                           // 0x3DC
    u16 sendBuf[60];                      // 0x3E0
    u16 partnerSwapState;                 // 0x458
    u16 partnerSpecies[2];                // 0x45A
    u16 partnerGender[2];                 // 0x45E
    u8 recvCount;                         // 0x462
    u8 partnerUpdated;                    // 0x463
    GridInputHandler *grid;               // 0x464
    u32 cursorPos;                        // 0x468
    YesNoPrompt *yesNoPrompt;             // 0x46C
    u8 unk_470[4];                        // 0x470
    BgCellManager *bgCellManager;         // 0x474
    int paletteBlinkState;                // 0x478
    u32 numPokepics;                      // 0x47C
    u32 monIconIndex;                     // 0x480
    u8 flashX;                            // 0x484
    u8 flashY;                            // 0x485
    u8 flashWidth;                        // 0x486
    u8 flashHeight;                       // 0x487
    u8 flashCount;                        // 0x488
    u8 flashTimer;                        // 0x489
    u8 flashPalette;                      // 0x48A
    u8 flashNextState;                    // 0x48B
} BattleFactoryApp;

#define TEXT_COLOR(fg, sh, bg) ((((u32)(fg) << 24) >> 8) | (((u32)(sh) << 24) >> 16) | (bg))
u8 AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
#define MAKE_TEXT_COLOR(fg, sh, bg) ((((fg) & 0xFF) << 16) | (((sh) & 0xFF) << 8) | (((bg) & 0xFF) << 0))
#define TEXT_SPEED_NOTRANSFER       0xFF
u8 TextPrinterCheckActive(u8 printerId);
void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);

BOOL BattleFactoryApp_Init(OverlayManager *man, int *state);
BOOL BattleFactoryApp_Main(OverlayManager *man, int *state);
BOOL BattleFactoryApp_Exit(OverlayManager *man, int *state);
void BattleFactory_ReceivePing(int netId);
void BattleFactory_ReceiveRentals(int netId, int size, const u16 *data, BattleFactoryApp *app);
void BattleFactory_ReceiveUnused(void);
void BattleFactory_ReceiveSwap(int netId, int size, const BattleFactoryLinkMessage *msg, BattleFactoryApp *app);

void BattleFactoryApp_MoveRentalCursor(BattleFactoryApp *app, int target);
void BattleFactoryApp_MoveButtonCursor(BattleFactoryApp *app, int target);
void BattleFactoryApp_MoveSwapOutCursor(BattleFactoryApp *app, int target);
void BattleFactoryApp_MoveSwapInCursor(BattleFactoryApp *app, int target);
BOOL BattleFactory_IsLinkMode(int frontierType);
BOOL BattleFactoryApp_IsLinkMode(BattleFactoryApp *app);
void BattleFactoryApp_InitBgCellManager(BattleFactoryApp *app);
void BattleFactoryApp_FreeBgCellManager(BattleFactoryApp *app);
void BattleFactorySpriteGfx_Init(BattleFactorySpriteGfx *gfx);
void BattleFactorySpriteGfx_Free(BattleFactorySpriteGfx *gfx);
void BattleFactoryApp_Setup3D(void);
void BattleFactoryApp_Draw3D(PokepicManager *pokepicManager);
Pokepic *BattleFactory_CreatePokepic(PokepicManager *pokepicManager, int polygonId, Pokemon *mon, int x, int y, int z);
void BattleFactory_SetPokepicHFlip(Pokepic *pokepic, int hFlip);
BattleFactorySprite *BattleFactorySprite_Create(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int priority, int drawPriority, int x, int y);
BattleFactorySprite *BattleFactorySprite_CreateSub(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int priority, int drawPriority, int x, int y);
BattleFactorySprite *BattleFactorySprite_Delete(BattleFactorySprite *sprite);
void BattleFactorySprite_SetVisible(BattleFactorySprite *sprite, int visible);
void BattleFactorySprite_SetPosition(BattleFactorySprite *sprite, int x, int y);
void BattleFactorySprite_SetPositionSub(BattleFactorySprite *sprite, int x, int y);
void BattleFactorySprite_SetMatrix(BattleFactorySprite *sprite, VecFx32 *pos);
BOOL BattleFactorySprite_IsAnimated(BattleFactorySprite *sprite);
void BattleFactorySprite_SetAnim(BattleFactorySprite *sprite, int animSeq);
void BattleFactorySprite_SetPaletteOverride(BattleFactorySprite *sprite, int palette);
void BattleFactorySprite_SetPalette(BattleFactorySprite *sprite, int palette);
void BattleFactorySprite_LoadCharData(BattleFactorySprite *sprite, void *data, u32 size, int screen);
BattleFactoryBall *BattleFactoryBall_Create(BattleFactorySpriteGfx *gfx, u32 x, u32 y, enum HeapID heapID);
BattleFactoryBall *BattleFactoryBall_Delete(BattleFactoryBall *ball);
void BattleFactoryBall_SetVisible(BattleFactoryBall *ball, int visible);
void BattleFactoryBall_Move(VecFx32 *out, BattleFactoryBall *ball, int dx, int dy);
void BattleFactoryBall_SetPosition(BattleFactoryBall *ball, int x, int y);
VecFx32 *BattleFactoryBall_GetPosition(BattleFactoryBall *ball);
u8 BattleFactoryBall_IsSelected(BattleFactoryBall *ball);
void BattleFactoryBall_Select(BattleFactoryBall *ball);
void BattleFactoryBall_Deselect(BattleFactoryBall *ball);
void BattleFactoryBall_MoveOffscreen(BattleFactoryBall *ball);
int BattleFactoryBall_GetX(BattleFactoryBall *ball);
int BattleFactoryBall_GetY(BattleFactoryBall *ball);
void BattleFactoryBall_SetAnim(BattleFactoryBall *ball, int animSeq);
void BattleFactoryBall_SetPaletteOverride(BattleFactoryBall *ball, int palette);
void BattleFactoryBall_SetOamMode(BattleFactoryBall *ball, GXOamMode mode);
BattleFactoryBall *BattleFactoryBall_Free(BattleFactoryBall *ball);
void BattleFactoryApp_AddWindows(BgConfig *bgConfig, Window *windows);
void BattleFactoryApp_RemoveWindows(Window *windows);
void BattleFactoryApp_DrawMessageFrame(Window *window, int frame);
void BattleFactoryApp_PrintMsgAligned(Window *window, MsgData *msgData, int msgId, int x, int y, int fontId, u32 color, int align);
void BattleFactoryApp_SetLinkNameWindows(Window *windows);
void BattleFactoryApp_LoadButtonGraphics(BgConfig *bgConfig, NARC *narc);
void BattleFactoryApp_DrawButtons(BgConfig *bgConfig, NARC *narc, int which);
void BattleFactoryApp_ClearButtons(BgConfig *bgConfig, int which);
void BattleFactory_FreeGrid(GridInputHandler *grid);
void BattleFactory_GetGridCursorPos(GridInputHandler *grid, int target, u32 *x, u32 *y);
GridInputHandler *BattleFactoryApp_CreateRentalGrid(BattleFactoryApp *app, int cursor);
void BattleFactory_SetRentalGridMenu(GridInputHandler *grid, int menuOpen);
GridInputHandler *BattleFactoryApp_CreateSwapOutGrid(BattleFactoryApp *app, int cursor);
GridInputHandler *BattleFactoryApp_CreateLinkSwapOutGrid(BattleFactoryApp *app, int cursor);
void BattleFactory_SetSwapOutGridMenu(GridInputHandler *grid, int menuOpen, int frontierType);
GridInputHandler *BattleFactoryApp_RecreateSwapOutGrid(BattleFactoryApp *app, GridInputHandler *grid);
GridInputHandler *BattleFactoryApp_CreateSwapInGrid(BattleFactoryApp *app, GridInputHandler *grid);
void BattleFactory_SetSwapInGridMenu(GridInputHandler *grid, int menuOpen, int frontierType);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_APP_INTERNAL_H
