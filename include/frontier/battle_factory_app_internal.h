#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_APP_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_APP_INTERNAL_H

#include "global.h"

#include "frontier/battle_factory_app.h"

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
    int x;
    int y;
    Sprite *sprite;
} BattleFactorySprite;

// One of the Poke Balls on the conveyor
typedef struct BattleFactoryBall {
    u16 selected;
    int x;
    int y;
    Sprite *sprite;
} BattleFactoryBall;

typedef struct BattleFactorySpriteGfx {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObj[6][4];
} BattleFactorySpriteGfx;

typedef struct BattleFactoryLinkMessage {
    u16 command;
    u16 accepted;
    u16 picked0;
    u16 picked1;
} BattleFactoryLinkMessage;

typedef struct BattleFactoryApp {
    OverlayManager *manager;
    OverlayManager *summaryApp;
    u8 subState;
    u8 frontierType;
    u8 levelMode;
    u8 screen;
    int bgScrollX;
    u8 printerId;
    u8 numPicked;
    u8 numBalls;
    u8 cancelRequested : 1;
    u8 summaryOpen : 1;
    u8 swapOutPicked : 1;
    u8 swapAccepted : 1;
    u8 shown : 1;
    u8 switchingPage : 1;
    u8 summaryFading : 1;
    u8 unk_013_7 : 1;
    u32 bgAnimTimer;
    u8 numPartnerMons;
    u8 timer;
    u8 unk_01A;
    u8 unk_01B;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *expandedString;
    String *rawString;
    String *strings[4];
    u8 unk_03C[0x10];
    BgConfig *bgConfig;
    Window windows[18];
    u8 unk_170[4];
    Window *unk_174;
    u8 unk_178[4];
    struct ListMenu2D *unk_17C;
    u8 unk_180[0x20];
    PaletteData *paletteData;
    GF3DVramMan *vramMan;
    PokepicManager *pokepicManager;
    Pokepic *pokepics[3];
    Options *options;
    SaveData *saveData;
    PokemonSummaryArgs *summaryArgs;
    BattleFactorySpriteGfx spriteGfx;
    BattleFactoryBall *balls[6];
    BattleFactoryBall *unk_378[2];
    BattleFactorySprite *centerSprite;
    BattleFactorySprite *unk_384;
    BattleFactorySprite *cursor;
    BattleFactorySprite *buttonCursor;
    BattleFactorySprite *picCursor;
    BattleFactorySprite *monIcons[2];
    BattleFactorySprite *slotMarkers[3];
    BattleFactorySprite *orderSprites[6];
    Party *party;
    Party *otherParty;
    u16 picked[6];
    u16 *resultOut;
    u32 summaryResult;
    NARC *narc;
    u16 sendBuf[60];
    u16 partnerSwapState;
    u16 partnerSpecies[2];
    u16 partnerGender[2];
    u8 recvCount;
    u8 partnerUpdated;
    GridInputHandler *grid;
    u32 cursorPos;
    YesNoPrompt *yesNoPrompt;
    u8 unk_470[4];
    BgCellManager *bgCellManager;
    int paletteBlinkState;
    u32 numPokepics;
    u32 monIconIndex;
    u8 flashX;
    u8 flashY;
    u8 flashWidth;
    u8 flashHeight;
    u8 flashCount;
    u8 flashTimer;
    u8 flashPalette;
    u8 flashNextState;
} BattleFactoryApp;

#define TEXT_COLOR(fg, sh, bg) ((((u32)(fg) << 24) >> 8) | (((u32)(sh) << 24) >> 16) | (bg))
u8 AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
#define MAKE_TEXT_COLOR(fg, sh, bg) ((((fg) & 0xFF) << 16) | (((sh) & 0xFF) << 8) | (((bg) & 0xFF) << 0))
#define TEXT_SPEED_NOTRANSFER       0xFF
u8 TextPrinterCheckActive(u8 printerId);
void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);

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
