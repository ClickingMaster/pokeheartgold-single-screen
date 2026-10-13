#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0194.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_2d.h"
#include "message_format.h"
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
#include "pokepic.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "oam_manager.h"
#include "unk_0200FA24.h"
#include "bg_cell_manager.h"
#include "grid_input_handler.h"
#include "swap_3d_buffers.h"
#include "communication_player_state.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "wireless_status_icon.h"
#include "unk_02088288.h"
#include "battle_frontier_communication.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

FS_EXTERN_OVERLAY(OVY_80);

// app->screen
#define FACTORY_SCREEN_RENTAL 0 // choose the rental Pokemon
#define FACTORY_SCREEN_SWAP   1 // trade one of the party for an opponent's Pokemon

// Main loop states
#define FACTORY_STATE_SLIDE_IN           0
#define FACTORY_STATE_SHOW_SCREEN        1
#define FACTORY_STATE_SUMMARY            2
#define FACTORY_STATE_SELECT_RENTALS     3
#define FACTORY_STATE_CONFIRM_RENTALS    4
#define FACTORY_STATE_CANCEL_LAST_RENTAL 5
#define FACTORY_STATE_SELECT_SWAP_OUT    6
#define FACTORY_STATE_CANCEL_SWAP        7
#define FACTORY_STATE_SELECT_SWAP_IN     8
#define FACTORY_STATE_CANCEL_SWAP_IN     9
#define FACTORY_STATE_SWITCH_PAGE        10
#define FACTORY_STATE_LINK_SEND_SWAP     11
#define FACTORY_STATE_LINK_SYNC          12
#define FACTORY_STATE_FADE_OUT           13
#define FACTORY_STATE_LINK_TRADE_RESULT  14

// Link commands (sendBuf[0])
#define FACTORY_LINK_RENTALS 8
#define FACTORY_LINK_SWAP    10

typedef struct BattleFactoryAppArgs {
    SaveData *saveData; // 0x00
    u8 frontierType;    // 0x04
    u8 unk_05;          // 0x05
    u8 screen;          // 0x06
    u8 unk_07;          // 0x07
    Party *party;       // 0x08
    Party *otherParty;  // 0x0C
    u16 result[3];      // 0x10
} BattleFactoryAppArgs;

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
    u8 unk_00A;                 // 0x00A
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
    BgCellManager *bgCellManager;          // 0x474
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

extern const u8 _02243458[];
extern const u16 ov81_0224345C[];
extern const u16 ov81_0224345E[];
extern const u16 ov81_02243464[];
extern const u16 ov81_02243466[];
extern const u16 ov81_02243470[];
extern const u16 ov81_02243472[];
extern const GraphicsModes ov81_02243480;
extern const u16 ov81_02243490[];
extern const u16 ov81_02243492[];
extern const BgTemplate ov81_022434A8;
extern const BgTemplate ov81_022434C4;
extern const BgTemplate ov81_022434E0;
extern const BgTemplate ov81_022434FC;
extern const BgTemplate ov81_02243518;
extern const BgTemplate ov81_02243534;
extern const BgTemplate ov81_02243550;
extern const GraphicsBanks ov81_0224356C;
extern const u8 ov81_02243594[4];
extern const ObjCharTransferTemplate ov81_02243598;
extern const WindowTemplate ov81_022435A8[18];
extern const GridCallbacks ov81_02243638;
extern const GridCallbacks ov81_02243648;
extern const GridCallbacks ov81_02243658;
extern const TouchscreenHitbox ov81_02243668[];
extern const TouchscreenHitbox ov81_02243688[];
extern const TouchscreenHitbox ov81_022436A8[];
extern const TouchscreenHitbox ov81_022436CC[];
extern const TouchscreenHitbox ov81_022436F0[];
extern const DpadMenuBox ov81_02243718[];
extern const DpadMenuBox ov81_02243750[];
extern const DpadMenuBox ov81_02243788[];
extern const DpadMenuBox ov81_022437C8[];
extern const DpadMenuBox ov81_02243808[];

// Frontier overlay (80) helpers
int BattleFactory_GetPartySize(int frontierType);
BOOL BattleFactory_IsMultiType(int frontierType);
void Frontier_BufferPartnerName(void *msgFmt, u32 field);

// text.h (and render_text.h, which includes it) are not included: text.h declares fontId as u8
// (FontID), but this module passes it unnarrowed
#define TEXT_COLOR(fg, sh, bg) ((((u32)(fg) << 24) >> 8) | (((u32)(sh) << 24) >> 16) | (bg))
u8 AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
#define MAKE_TEXT_COLOR(fg, sh, bg) ((((fg) & 0xFF) << 16) | (((sh) & 0xFF) << 8) | (((bg) & 0xFF) << 0))
#define TEXT_SPEED_NOTRANSFER       0xFF
u8 TextPrinterCheckActive(u8 printerId);
void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);

BOOL BattleFactoryApp_Init(OverlayManager *man, int *state);
BOOL BattleFactoryApp_Main(OverlayManager *man, int *state);
BOOL BattleFactoryApp_Exit(OverlayManager *man, int *state);
BOOL BattleFactoryApp_SlideIn(BattleFactoryApp *app);
BOOL BattleFactoryApp_ShowScreen(BattleFactoryApp *app);
void BattleFactoryApp_SetupRentalScreen(BattleFactoryApp *app);
void BattleFactoryApp_CreateRentalBalls(BattleFactoryApp *app);
void BattleFactoryApp_CreateRentalPokepics(BattleFactoryApp *app);
void BattleFactoryApp_SetupSwapOutScreen(BattleFactoryApp *app);
void BattleFactoryApp_SetupSwapInScreen(BattleFactoryApp *app);
void BattleFactoryApp_CreateSwapBalls(BattleFactoryApp *app);
void BattleFactoryApp_CreateSwapPokepics(BattleFactoryApp *app);
BOOL BattleFactoryApp_WaitSummary(BattleFactoryApp *app);
BOOL BattleFactoryApp_SelectRentals(BattleFactoryApp *app);
void BattleFactoryApp_UndoLastRental(BattleFactoryApp *app);
void BattleFactoryApp_AddRental(BattleFactoryApp *app);
void BattleFactoryApp_RemoveRental(BattleFactoryApp *app);
BOOL BattleFactoryApp_ConfirmRentals(BattleFactoryApp *app);
BOOL BattleFactoryApp_CancelLastRental(BattleFactoryApp *app);
void BattleFactoryApp_DeselectCursorRental(BattleFactoryApp *app);
BOOL BattleFactoryApp_SelectSwapOut(BattleFactoryApp *app);
void BattleFactoryApp_PickSwapOut(BattleFactoryApp *app);
BOOL BattleFactoryApp_ConfirmCancelSwap(BattleFactoryApp *app);
void BattleFactoryApp_ResumeSwapOut(BattleFactoryApp *app);
BOOL BattleFactoryApp_SelectSwapIn(BattleFactoryApp *app);
void BattleFactoryApp_UnpickSwapOut(BattleFactoryApp *app);
void BattleFactoryApp_PickSwapIn(BattleFactoryApp *app);
void BattleFactoryApp_ShowSwapInPrompt(BattleFactoryApp *app);
BOOL BattleFactoryApp_ConfirmCancelSwapIn(BattleFactoryApp *app);
void BattleFactoryApp_ResumeSwapIn(BattleFactoryApp *app);
BOOL BattleFactoryApp_SwitchPage(BattleFactoryApp *app);
BOOL BattleFactoryApp_LinkSendSwap(BattleFactoryApp *app);
BOOL BattleFactoryApp_LinkSync(BattleFactoryApp *app);
BOOL BattleFactoryApp_FadeOut(BattleFactoryApp *app);
BOOL BattleFactoryApp_ShowLinkTradeResult(BattleFactoryApp *app);
void BattleFactoryApp_VBlankCallback(BattleFactoryApp *app);
void BattleFactoryApp_SetGraphicsBanks(void);
void BattleFactoryApp_InitBgLayers(BgConfig *bgConfig);
void BattleFactoryApp_InitBgGraphics(BattleFactoryApp *app);
void BattleFactoryApp_InitSprites(BattleFactoryApp *app);
void BattleFactoryApp_FreeBgLayers(BgConfig *bgConfig);
void BattleFactoryApp_SetMainState(BattleFactoryApp *app, int *state, int nextState);
BOOL BattleFactoryApp_ScrollBallsIn(BattleFactoryApp *app);
BOOL BattleFactoryApp_ScrollBallsOut(BattleFactoryApp *app);
void BattleFactoryApp_AnimateBackground(BattleFactoryApp *app);
void BattleFactoryApp_SetBgPalette(BattleFactoryApp *app, int bgId, u8 palette, u8 width, u8 height);
void BattleFactoryApp_SetCenterAnim(BattleFactoryApp *app, int kind);
void ov81_02240698(BattleFactoryApp *app, GFBgLayer bgId);
void ov81_022406E0(BattleFactoryApp *app, GFBgLayer bgId);
void ov81_02240728(BattleFactoryApp *app, GFBgLayer bgId);
void BattleFactoryApp_LoadBgPalette(void);
void BattleFactoryApp_LoadSubScreenBg(BattleFactoryApp *app, GFBgLayer bgId);
u8 BattleFactoryApp_PrintMessage(BattleFactoryApp *app, Window *window, u32 msgId, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId);
u8 BattleFactoryApp_ShowMessage(BattleFactoryApp *app, u32 msgId);
void BattleFactoryApp_BufferNumber(BattleFactoryApp *app, u32 field, s32 value);
void BattleFactoryApp_BufferSpeciesName(BattleFactoryApp *app, u32 field, BoxPokemon *boxMon);
void BattleFactoryApp_PrintPlayerName(BattleFactoryApp *app, Window *window, int x, int y, u8 fontId);
void BattleFactoryApp_PrintPartnerName(BattleFactoryApp *app, Window *window, int x, int y, u8 fontId);
void BattleFactoryApp_PrintMonName(BattleFactoryApp *app, Window *window, int monIndex, int x, int y, u8 fg, u8 shadow, volatile u8 bg, u8 fontId, Party *party);
void BattleFactoryApp_PrintPartnerMonName(BattleFactoryApp *app, Window *window, int x, int y, u8 fg, u8 shadow, volatile u8 bg, u8 fontId, u16 species, u8 gender);
void BattleFactoryApp_FreeGraphics(BattleFactoryApp *app);
void BattleFactoryApp_ResumeAfterSummary(BattleFactoryApp *app);
void BattleFactoryApp_DisableDisplay(void);
void BattleFactoryApp_InitGraphics(BattleFactoryApp *app);
void BattleFactoryApp_CreateSummaryArgs(BattleFactoryApp *app);
BOOL BattleFactoryApp_IsScreen(BattleFactoryApp *app, u8 screen);
int BattleFactory_GetRentalCount(int frontierType);
BOOL BattleFactoryApp_IsCancelRequested(BattleFactoryApp *app);
void BattleFactoryApp_SetCancelRequested(BattleFactoryApp *app, u8 value);
void BattleFactoryApp_CreatePokepic(BattleFactoryApp *app, int index, Pokemon *mon, int x, int y, int hFlip);
BOOL BattleFactoryApp_SendLinkCommand(BattleFactoryApp *app, int command, int arg);
void BattleFactoryApp_PrepareLinkPing(BattleFactoryApp *app, int command);
void BattleFactory_ReceivePing(int netId);
void BattleFactoryApp_PrepareLinkRentals(BattleFactoryApp *app, int command, int arg);
void BattleFactory_ReceiveRentals(int netId, int size, const u16 *data, BattleFactoryApp *app);
void BattleFactory_ReceiveUnused(void);
void BattleFactoryApp_PrintPartnerRentals(BattleFactoryApp *app);
void BattleFactoryApp_PrepareLinkSwap(BattleFactoryApp *app, int command, int accepted);
void BattleFactory_ReceiveSwap(int netId, int size, const BattleFactoryLinkMessage *msg, BattleFactoryApp *app);
BattleFactoryBall *BattleFactoryApp_CreateBall(BattleFactoryApp *app, int index);
BattleFactorySprite *BattleFactoryApp_CreateCenterSprite(BattleFactoryApp *app, int animSeq);
void BattleFactoryApp_DeleteCenterSprites(BattleFactoryApp *app);
void BattleFactoryApp_InitWindows(BattleFactoryApp *app);
void BattleFactoryApp_CloseWindow(Window *window);
void BattleFactoryApp_MoveRentalCursor(BattleFactoryApp *app, int target);
void BattleFactoryApp_MoveButtonCursor(BattleFactoryApp *app, int target);
void BattleFactoryApp_OpenRentalMenu(BattleFactoryApp *app);
void BattleFactoryApp_CloseRentalMenu(BattleFactoryApp *app);
void BattleFactoryApp_PrintRentalMenu(BattleFactoryApp *app);
void BattleFactoryApp_MoveSwapOutCursor(BattleFactoryApp *app, int target);
void BattleFactoryApp_MoveSwapInCursor(BattleFactoryApp *app, int target);
void BattleFactoryApp_OpenSwapOutMenu(BattleFactoryApp *app);
void BattleFactoryApp_CloseSwapOutMenu(BattleFactoryApp *app);
void BattleFactoryApp_RedrawSwapOutButtons(BattleFactoryApp *app);
void BattleFactoryApp_ClearSwapOutMenu(BattleFactoryApp *app);
void BattleFactoryApp_PrintSwapOutMenu(BattleFactoryApp *app);
void BattleFactoryApp_OpenSwapInMenu(BattleFactoryApp *app);
void BattleFactoryApp_CloseSwapInMenu(BattleFactoryApp *app);
void BattleFactoryApp_RedrawSwapInButtons(BattleFactoryApp *app);
void BattleFactoryApp_ClearSwapInMenu(BattleFactoryApp *app);
void BattleFactoryApp_PrintSwapInMenu(BattleFactoryApp *app);
void BattleFactoryApp_CreateYesNo(YesNoPrompt **prompt);
void BattleFactoryApp_DestroyYesNo(YesNoPrompt *prompt);
void BattleFactoryApp_InitYesNo(YesNoPrompt *prompt, BgConfig *bgConfig);
void BattleFactoryApp_UpdatePreviewPokepic(BattleFactoryApp *app);
void BattleFactory_GetPokepicPos(int slot, int numPokepics, int *x, int *y);
void BattleFactoryApp_GetCursorPokepicPos(BattleFactoryApp *app, int *x, int *y);
void BattleFactoryApp_MovePicCursor(BattleFactoryApp *app);
int BattleFactoryApp_GetCursorSlot(BattleFactoryApp *app);
void BattleFactoryApp_UpdateSwapOutPicCursor(BattleFactoryApp *app);
void BattleFactoryApp_UpdateSwapInPicCursor(BattleFactoryApp *app);
void BattleFactoryApp_CreatePartyPokepics(BattleFactoryApp *app, int hFlip);
void BattleFactoryApp_ReplacePokepic(BattleFactoryApp *app, int slot, int monIndex, int hFlip);
void BattleFactoryApp_ReplacePokepicFromOther(BattleFactoryApp *app, int slot, int monIndex, int hFlip);
void BattleFactoryApp_CreateSlotMarkers(BattleFactoryApp *app);
void BattleFactoryApp_DeleteSlotMarkers(BattleFactoryApp *app);
void BattleFactoryApp_UpdateSlotMarkers(BattleFactoryApp *app);
void BattleFactoryApp_SetSlotMarkers(BattleFactoryApp *app, int highlightCursor);
void BattleFactoryApp_CreateOrderSprites(BattleFactoryApp *app);
void BattleFactoryApp_DeleteOrderSprites(BattleFactoryApp *app);
void BattleFactoryApp_UpdateOrderSprites(BattleFactoryApp *app);
void BattleFactoryApp_ShowMonIcon(BattleFactoryApp *app, Party *party, u32 index, u32 limit);
void BattleFactoryApp_PrintRentalNames(BattleFactoryApp *app, u32 target, u32 limit);
void BattleFactoryApp_PrintPartyNames(BattleFactoryApp *app, Party *party);
void BattleFactoryApp_PrintSwapNames(BattleFactoryApp *app, u32 swapIn);
void BattleFactoryApp_PrintSwapInName(BattleFactoryApp *app, u32 index);
void BattleFactoryApp_LoadSubPalette(BattleFactoryApp *app);
void BattleFactoryApp_UpdatePaletteBlink(BattleFactoryApp *app);
void BattleFactoryApp_StartButtonFlash(BattleFactoryApp *app, u8 x, u8 y, u8 width, u8 height, u8 nextState);
void BattleFactoryApp_FlashWideButton(BattleFactoryApp *app, u8 x, u8 y, u8 nextState);
void BattleFactoryApp_FlashNarrowButton(BattleFactoryApp *app, u8 x, u8 y, u8 nextState);
BOOL BattleFactoryApp_UpdateButtonFlash(BattleFactoryApp *app);
void BattleFactoryApp_SetBallsTranslucent(BattleFactoryApp *app, int enable);
BOOL BattleFactory_IsLinkMode(int frontierType);
BOOL BattleFactoryApp_IsLinkMode(BattleFactoryApp *app);
void BattleFactoryApp_InitBgCellManager(BattleFactoryApp *app);
void BattleFactoryApp_FreeBgCellManager(BattleFactoryApp *app);
void BattleFactorySpriteGfx_Init(BattleFactorySpriteGfx *gfx);
Sprite *BattleFactorySpriteGfx_CreateSprite(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int drawPriority, int priority, u8 screen);
void BattleFactorySpriteGfx_Free(BattleFactorySpriteGfx *gfx);
void BattleFactoryApp_InitObjTransfers(void);
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
void BattleFactoryBall_SetSelected(BattleFactoryBall *ball, u16 selected);
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
void BattleFactoryApp_PrintStringAligned(Window *window, String *string, int x, int y, int fontId, u32 color, int align);
void BattleFactoryApp_PrintMsgAligned(Window *window, MsgData *msgData, int msgId, int x, int y, int fontId, u32 color, int align);
void BattleFactoryApp_SetLinkNameWindows(Window *windows);
void BattleFactoryApp_LoadButtonGraphics(BgConfig *bgConfig, NARC *narc);
void BattleFactoryApp_DrawButtons(BgConfig *bgConfig, NARC *narc, int which);
void BattleFactoryApp_ClearButtons(BgConfig *bgConfig, int which);
void BattleFactory_FreeGrid(GridInputHandler *grid);
void BattleFactory_GetGridCursorPos(GridInputHandler *grid, int target, u32 *x, u32 *y);
void BattleFactory_GridNop(void);
GridInputHandler *BattleFactoryApp_CreateRentalGrid(BattleFactoryApp *app, int cursor);
void BattleFactory_SetRentalGridMenu(GridInputHandler *grid, int menuOpen);
void BattleFactoryApp_OnRentalCursorMove(BattleFactoryApp *app, int target);
GridInputHandler *BattleFactoryApp_CreateSwapOutGrid(BattleFactoryApp *app, int cursor);
GridInputHandler *BattleFactoryApp_CreateLinkSwapOutGrid(BattleFactoryApp *app, int cursor);
void BattleFactory_SetSwapOutGridMenu(GridInputHandler *grid, int menuOpen, int frontierType);
void BattleFactoryApp_OnSwapOutCursorMove(BattleFactoryApp *app, int target);
GridInputHandler *BattleFactoryApp_RecreateSwapOutGrid(BattleFactoryApp *app, GridInputHandler *grid);
GridInputHandler *BattleFactoryApp_CreateSwapInGrid(BattleFactoryApp *app, GridInputHandler *grid);
void BattleFactory_SetSwapInGridMenu(GridInputHandler *grid, int menuOpen, int frontierType);
void BattleFactoryApp_OnSwapInCursorMove(BattleFactoryApp *app, int target);

BOOL BattleFactoryApp_Init(OverlayManager *man, int *state) {
    BattleFactoryApp *app;
    BattleFactoryAppArgs *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    BattleFactoryApp_DisableDisplay();
    Heap_Create(HEAP_ID_3, HEAP_ID_100, 0x30000);
    app = OverlayManager_CreateAndGetData(man, sizeof(BattleFactoryApp), HEAP_ID_100);
    memset(app, 0, sizeof(BattleFactoryApp));
    app->vramMan = GF_3DVramMan_Create(HEAP_ID_100, 0, 2, 0, 2, BattleFactoryApp_Setup3D);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_100);
    app->manager = man;
    args = OverlayManager_GetArgs(man);
    app->saveData = args->saveData;
    app->frontierType = args->frontierType;
    app->unk_00A = args->unk_05;
    app->screen = args->screen;
    app->party = args->party;
    app->otherParty = args->otherParty;
    app->resultOut = args->result;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->bgAnimTimer = 8;
    if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
        app->numBalls = 6;
        app->grid = BattleFactoryApp_CreateRentalGrid(app, 0);
    } else if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
        app->numBalls = 2;
        app->unk_01A = 3;
        app->unk_01B = 6;
        app->grid = BattleFactoryApp_CreateLinkSwapOutGrid(app, 0);
    } else {
        app->numBalls = 3;
        app->unk_01A = 4;
        app->unk_01B = 5;
        app->grid = BattleFactoryApp_CreateSwapOutGrid(app, 0);
    }
    if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
        app->numPokepics = 2;
    } else {
        app->numPokepics = 3;
    }
    BattleFactoryApp_CreateYesNo(&app->yesNoPrompt);
    BattleFactoryApp_InitGraphics(app);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
        FrontierLink_RegisterCommands(app);
    }
    *state = 0;
    return TRUE;
}

#pragma force_active on
BOOL BattleFactoryApp_Main(OverlayManager *man, int *state) {
    BattleFactoryApp *app = OverlayManager_GetData(man);

    if (app->partnerSwapState == 1) {
        switch (*state) {
        case FACTORY_STATE_SELECT_SWAP_OUT:
        case FACTORY_STATE_CANCEL_SWAP:
        case FACTORY_STATE_SELECT_SWAP_IN:
        case FACTORY_STATE_CANCEL_SWAP_IN:
            if (app->summaryFading == FALSE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SEND_SWAP);
            }
            break;
        }
    }
    BattleFactoryApp_UpdatePaletteBlink(app);
    switch (*state) {
    case FACTORY_STATE_SLIDE_IN:
        if (BattleFactoryApp_SlideIn(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SHOW_SCREEN);
        }
        break;
    case FACTORY_STATE_SUMMARY:
        if (BattleFactoryApp_WaitSummary(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SHOW_SCREEN);
        } else {
            return FALSE;
        }
    case FACTORY_STATE_SHOW_SCREEN:
        if (BattleFactoryApp_ShowScreen(app) == TRUE) {
            if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_RENTALS);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_OUT);
            }
        }
        break;
    case FACTORY_STATE_SELECT_RENTALS:
        BattleFactoryApp_PrintPartnerRentals(app);
        if (BattleFactoryApp_SelectRentals(app) == TRUE) {
            if (app->summaryOpen == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SUMMARY);
            } else if (app->numPicked == BattleFactory_GetRentalCount(app->frontierType)) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_CONFIRM_RENTALS);
            } else if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_RENTALS);
            }
        }
        break;
    case FACTORY_STATE_CONFIRM_RENTALS:
        BattleFactoryApp_PrintPartnerRentals(app);
        if (BattleFactoryApp_ConfirmRentals(app) == TRUE) {
            if (app->numPicked == BattleFactory_GetRentalCount(app->frontierType)) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_CANCEL_LAST_RENTAL);
            }
        }
        break;
    case FACTORY_STATE_CANCEL_LAST_RENTAL:
        if (BattleFactoryApp_CancelLastRental(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_RENTALS);
        }
        break;
    case FACTORY_STATE_SELECT_SWAP_OUT:
        if (BattleFactoryApp_SelectSwapOut(app) == TRUE) {
            if (app->summaryOpen == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SUMMARY);
            } else if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetCancelRequested(app, FALSE);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_CANCEL_SWAP);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SWITCH_PAGE);
            }
        }
        break;
    case FACTORY_STATE_CANCEL_SWAP:
        if (BattleFactoryApp_ConfirmCancelSwap(app) == TRUE) {
            if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetCancelRequested(app, FALSE);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_OUT);
            } else if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->swapAccepted = FALSE;
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SEND_SWAP);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            }
        }
        break;
    case FACTORY_STATE_SELECT_SWAP_IN:
        if (BattleFactoryApp_SelectSwapIn(app) == TRUE) {
            if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetCancelRequested(app, FALSE);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_CANCEL_SWAP_IN);
            } else if (app->numPicked == 0) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SWITCH_PAGE);
            } else if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SEND_SWAP);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            }
        }
        break;
    case FACTORY_STATE_CANCEL_SWAP_IN:
        if (BattleFactoryApp_ConfirmCancelSwapIn(app) == TRUE) {
            if (BattleFactoryApp_IsCancelRequested(app) == TRUE) {
                BattleFactoryApp_SetCancelRequested(app, FALSE);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_IN);
            } else if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->swapAccepted = FALSE;
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SEND_SWAP);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
            }
        }
        break;
    case FACTORY_STATE_SWITCH_PAGE:
        if (BattleFactoryApp_SwitchPage(app) == TRUE) {
            if (app->numPicked == 0) {
                BattleFactoryApp_SetupSwapOutScreen(app);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_OUT);
            } else {
                BattleFactoryApp_SetupSwapInScreen(app);
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_SELECT_SWAP_IN);
            }
        }
        break;
    case FACTORY_STATE_LINK_SEND_SWAP:
        if (BattleFactoryApp_LinkSendSwap(app) == TRUE) {
            if (app->partnerSwapState == 1) {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_TRADE_RESULT);
            } else {
                BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SYNC);
            }
        }
        break;
    case FACTORY_STATE_LINK_SYNC:
        if (BattleFactoryApp_LinkSync(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_FADE_OUT);
        }
        break;
    case FACTORY_STATE_FADE_OUT:
        if (BattleFactoryApp_FadeOut(app) == TRUE) {
            return TRUE;
        }
        break;
    case FACTORY_STATE_LINK_TRADE_RESULT:
        if (BattleFactoryApp_ShowLinkTradeResult(app) == TRUE) {
            BattleFactoryApp_SetMainState(app, state, FACTORY_STATE_LINK_SYNC);
        }
        break;
    }
    SpriteList_RenderAndAnimateSprites(app->spriteGfx.spriteList);
    BattleFactoryApp_Draw3D(app->pokepicManager);
    return FALSE;
}
#pragma force_active reset

BOOL BattleFactoryApp_Exit(OverlayManager *man, int *state) {
#pragma unused(state)
    BattleFactoryApp *app = OverlayManager_GetData(man);
    int i;

    if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0) {
        PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(app->paletteData);
        app->paletteBlinkState = 0xFF;
        return FALSE;
    }
    if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
        for (i = 0; i < BattleFactory_GetRentalCount(app->frontierType); i++) {
            app->resultOut[i] = app->picked[i];
        }
    } else {
        for (i = 0; i < 2; i++) {
            app->resultOut[i] = app->picked[i];
        }
        if (app->swapAccepted == FALSE) {
            app->resultOut[0] = 0xFF;
            app->resultOut[1] = 0xFF;
        }
    }
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    BattleFactoryApp_DestroyYesNo(app->yesNoPrompt);
    BattleFactory_FreeGrid(app->grid);
    BattleFactoryApp_FreeGraphics(app);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_100);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return TRUE;
}

#pragma force_active on
BOOL BattleFactoryApp_SlideIn(BattleFactoryApp *app) {
    int i;
    u32 j;

    switch (app->subState) {
    case 0:
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(237);
        }
        app->subState++;
        break;
    case 1:
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            if (LinkSync_IsSynced(237) == TRUE) {
                LinkSync_ClearPackets();
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 2:
        for (i = 0; i < app->numBalls; i++) {
            BattleFactoryBall_GetPosition(app->balls[i]);
            BattleFactoryBall_MoveOffscreen(app->balls[i]);
        }
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, 264);
        Pokepic_SetAttr(app->pokepics[0], POKEPIC_VANISHED, TRUE);
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 3, HEAP_ID_100);
        app->subState++;
        break;
    case 3:
        if (!IsPaletteFadeFinished()) {
            break;
        }
        PlaySE(SEQ_SE_DP_ELEBETA2);
        app->timer = 0;
        app->subState++;
        break;
    case 4:
        if (BattleFactoryApp_ScrollBallsIn(app) != TRUE) {
            break;
        }
        StopSE(SEQ_SE_DP_ELEBETA2, 0);
        PlaySE(SEQ_SE_DP_KASYA);
        for (i = 0; i < app->numBalls; i++) {
            BattleFactoryBall_SetAnim(app->balls[i], 4);
        }
        BattleFactoryApp_SetCenterAnim(app, 0);
        app->bgAnimTimer = 8;
        app->timer = 0;
        app->subState++;
        break;
    case 5:
        if (BattleFactorySprite_IsAnimated(app->centerSprite) == TRUE) {
            break;
        }
        app->timer = 0;
        app->subState++;
        break;
    case 6:
        if (app->timer == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
            if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
                Pokepic_SetAttr(app->pokepics[0], POKEPIC_VANISHED, FALSE);
                Pokepic_StartPaletteFadeAll(app->pokepicManager, 16, 0, 1, 0xFFFF);
            } else {
                for (j = 0; j < app->numPokepics; j++) {
                    Pokepic_SetAttr(app->pokepics[j], POKEPIC_VANISHED, FALSE);
                }
                Pokepic_StartPaletteFadeAll(app->pokepicManager, 16, 0, 1, 0xFFFF);
            }
        }
        Pokepic_ResumePaletteFade(app->pokepics[0]);
        app->timer++;
        if (app->timer >= 2) {
            app->timer = 0;
            BattleFactoryApp_InitWindows(app);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
#pragma force_active reset

BOOL BattleFactoryApp_ShowScreen(BattleFactoryApp *app) {
    switch (app->subState) {
    case 0:
        if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
            BattleFactoryApp_SetupRentalScreen(app);
        } else {
            BattleFactoryApp_SetupSwapOutScreen(app);
        }
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            BattleFactoryApp_PrintPartnerName(app, &app->windows[1], 0, 0, 0);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        }
        if (app->shown == TRUE) {
            BeginNormalPaletteFade(0, 1, 1, 0, 6, 3, HEAP_ID_100);
        }
        app->shown = TRUE;
        app->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on
void BattleFactoryApp_SetupRentalScreen(BattleFactoryApp *app) {
    int x;
    int y;
    int slot;
    int maxPartnerMons = BattleFactory_GetPartySize(app->frontierType);
    int i;

    BattleFactoryApp_SetCancelRequested(app, FALSE);
    BattleFactoryApp_PrintRentalMenu(app);
    BattleFactory_GetGridCursorPos(app->grid, app->cursorPos, (u32 *)&x, (u32 *)&y);
    app->cursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 2, 0, 0, x, y);
    BattleFactory_GetGridCursorPos(app->grid, 6, (u32 *)&x, (u32 *)&y);
    app->buttonCursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 3, 0, 0, x, y);
    BattleFactoryApp_GetCursorPokepicPos(app, &x, &y);
    app->picCursor = BattleFactorySprite_Create(&app->spriteGfx, 1, 0, 0, 0, x, y);
    BattleFactoryApp_CreateSlotMarkers(app);
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_CreateOrderSprites(app);
    BattleFactoryApp_UpdateOrderSprites(app);
    app->monIcons[0] = BattleFactorySprite_CreateSub(&app->spriteGfx, 4, 0, 0, 0, 80, 12);
    app->monIcons[1] = BattleFactorySprite_CreateSub(&app->spriteGfx, 5, 0, 0, 0, 80, 12);
    BattleFactoryApp_ShowMonIcon(app, app->party, app->cursorPos, 6);
    BattleFactoryApp_PrintRentalNames(app, app->cursorPos, 6);
    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
    BattleFactoryApp_PrintPlayerName(app, &app->windows[0], 0, 0, 0);
    for (i = 0; i < app->numPicked; i++) {
        BattleFactoryBall_Select(app->balls[app->picked[i]]);
        BattleFactoryBall_SetPaletteOverride(app->balls[app->picked[i]], 0);
        BattleFactoryBall_SetAnim(app->balls[app->picked[i]], 1);
    }
    BattleFactoryApp_DrawMessageFrame(&app->windows[7], Options_GetFrame(app->options));
    BattleFactoryApp_BufferNumber(app, 0, app->numPicked + 1);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    if (app->summaryOpen == TRUE) {
        slot = BattleFactoryApp_GetCursorSlot(app);
        for (i = 0; i < app->numPicked; i++) {
            Pokepic_SetAttr(app->pokepics[i], POKEPIC_VANISHED, FALSE);
        }
        Pokepic_SetAttr(app->pokepics[slot], POKEPIC_VANISHED, FALSE);
        BattleFactoryApp_ReplacePokepic(app, slot, app->cursorPos, 0);
        BattleFactoryApp_UpdateSlotMarkers(app);
        BattleFactory_GetPokepicPos(slot, app->numPokepics, &x, &y);
        BattleFactorySprite_SetPosition(app->picCursor, x, y);
        BattleFactoryApp_SetCenterAnim(app, 0xFF);
        BattleFactoryApp_OpenRentalMenu(app);
        if (app->numPartnerMons != 0) {
            for (i = 0; i < maxPartnerMons; i++) {
                FillWindowPixelBuffer(&app->windows[i + 5], 0);
                if (i < app->numPartnerMons) {
                    BattleFactoryApp_PrintPartnerMonName(app, &app->windows[i + 5], 0, 0, 15, 2, 0, 0, app->partnerSpecies[i], app->partnerGender[i]);
                }
                ScheduleWindowCopyToVram(&app->windows[i + 5]);
            }
        }
    } else {
        BattleFactoryApp_CloseRentalMenu(app);
    }
}
#pragma force_active reset

void BattleFactoryApp_CreateRentalBalls(BattleFactoryApp *app) {
    int i;

    for (i = 0; i < app->numBalls; i++) {
        app->balls[i] = BattleFactoryApp_CreateBall(app, i);
        BattleFactoryBall_SetVisible(app->balls[i], TRUE);
    }
}

void BattleFactoryApp_CreateRentalPokepics(BattleFactoryApp *app) {
    BattleFactoryApp_CreatePartyPokepics(app, 0);
}

void BattleFactoryApp_SetupSwapOutScreen(BattleFactoryApp *app) {
    u32 x;
    u32 y;
    u32 i;

    BattleFactoryApp_SetCancelRequested(app, FALSE);
    app->grid = BattleFactoryApp_RecreateSwapOutGrid(app, app->grid);
    BattleFactoryApp_PrintSwapOutMenu(app);
    BattleFactory_GetGridCursorPos(app->grid, app->cursorPos, &x, &y);
    app->cursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 2, 0, 0, x, y);
    app->picCursor = BattleFactorySprite_Create(&app->spriteGfx, 1, 0, 0, 0, 0, 0);
    BattleFactoryApp_UpdateSwapOutPicCursor(app);
    BattleFactory_GetGridCursorPos(app->grid, 4, &x, &y);
    app->buttonCursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 3, 0, 0, x, y);
    BattleFactoryApp_CreateSlotMarkers(app);
    BattleFactoryApp_SetSlotMarkers(app, FALSE);
    BattleFactoryApp_CreateOrderSprites(app);
    app->monIcons[0] = BattleFactorySprite_CreateSub(&app->spriteGfx, 4, 0, 0, 0, 80, 12);
    app->monIcons[1] = BattleFactorySprite_CreateSub(&app->spriteGfx, 5, 0, 0, 0, 80, 12);
    BattleFactoryApp_ShowMonIcon(app, app->party, app->cursorPos, 4);
    BattleFactoryApp_PrintPartyNames(app, app->party);
    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
    BattleFactoryApp_PrintPlayerName(app, &app->windows[0], 0, 0, 0);
    BattleFactoryApp_DrawMessageFrame(&app->windows[7], Options_GetFrame(app->options));
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00009);
    if (app->summaryOpen == TRUE) {
        for (i = 0; i < app->numPokepics; i++) {
            Pokepic_SetAttr(app->pokepics[i], POKEPIC_VANISHED, FALSE);
        }
        BattleFactory_GetPokepicPos(app->cursorPos, app->numPokepics, (int *)&x, (int *)&y);
        BattleFactorySprite_SetPosition(app->picCursor, x, y);
        BattleFactoryApp_SetCenterAnim(app, 0xFF);
        BattleFactoryApp_OpenSwapOutMenu(app);
        BattleFactorySprite_SetPalette(app->cursor, 1);
    } else {
        BattleFactoryApp_CloseSwapOutMenu(app);
        BattleFactorySprite_SetPalette(app->cursor, 0);
    }
}

void BattleFactoryApp_SetupSwapInScreen(BattleFactoryApp *app) {
    u32 x;
    u32 y;

    app->grid = BattleFactoryApp_CreateSwapInGrid(app, app->grid);
    BattleFactoryApp_PrintSwapInMenu(app);
    BattleFactory_GetGridCursorPos(app->grid, 0, &x, &y);
    app->cursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 2, 0, 0, x, y);
    BattleFactory_GetPokepicPos(app->cursorPos, app->numPokepics, (int *)&x, (int *)&y);
    app->picCursor = BattleFactorySprite_Create(&app->spriteGfx, 1, 0, 0, 0, x, y);
    BattleFactory_GetGridCursorPos(app->grid, 6, &x, &y);
    app->buttonCursor = BattleFactorySprite_CreateSub(&app->spriteGfx, 0, 3, 0, 0, x, y);
    BattleFactoryApp_CreateSlotMarkers(app);
    BattleFactoryApp_SetSlotMarkers(app, TRUE);
    BattleFactoryApp_CreateOrderSprites(app);
    app->monIcons[0] = BattleFactorySprite_CreateSub(&app->spriteGfx, 4, 0, 0, 0, 80, 12);
    app->monIcons[1] = BattleFactorySprite_CreateSub(&app->spriteGfx, 5, 0, 0, 0, 80, 12);
    BattleFactoryApp_ShowMonIcon(app, app->otherParty, 0, 4);
    BattleFactoryApp_PrintSwapNames(app, 0);
    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
    BattleFactoryApp_PrintPlayerName(app, &app->windows[0], 0, 0, 0);
    app->cursorPos = 0;
    BattleFactoryApp_CloseSwapInMenu(app);
}

#pragma force_active on
void BattleFactoryApp_CreateSwapBalls(BattleFactoryApp *app) {
    int i;

    for (i = 0; i < app->numBalls; i++) {
        app->balls[i] = BattleFactoryApp_CreateBall(app, i);
        BattleFactoryBall_SetVisible(app->balls[i], TRUE);
        if (!app->swapOutPicked) {
            BattleFactoryBall_Select(app->balls[i]);
            BattleFactoryBall_SetPaletteOverride(app->balls[i], 0);
            BattleFactoryBall_SetAnim(app->balls[i], 0);
        }
    }
}
#pragma force_active reset

void BattleFactoryApp_CreateSwapPokepics(BattleFactoryApp *app) {
    u32 i;

    BattleFactoryApp_CreatePartyPokepics(app, 1);
    for (i = 0; i < app->numPokepics; i++) {
        BattleFactoryApp_ReplacePokepic(app, i, i, 1);
        Pokepic_SetAttr(app->pokepics[i], POKEPIC_VANISHED, TRUE);
    }
}

BOOL BattleFactoryApp_WaitSummary(BattleFactoryApp *app) {
    if (app->subState == 0 && OverlayManager_Run(app->summaryApp) == TRUE) {
        u8 partySlot = app->summaryArgs->partySlot;
        app->summaryResult = partySlot;
        app->cursorPos = app->summaryArgs->partySlot;
        Heap_Free(app->summaryArgs);
        Heap_Free(app->summaryApp);
        app->summaryApp = NULL;
        BattleFactoryApp_ResumeAfterSummary(app);
        app->summaryFading = FALSE;
        app->paletteBlinkState = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
BOOL BattleFactoryApp_SelectRentals(BattleFactoryApp *app) {
    u32 input;

    switch (app->subState) {
    case 0:
        app->subState = 1;
        app->summaryOpen = FALSE;
        break;
    case 1:
        input = GridInputHandler_HandleInput_NoHold(app->grid);
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorPos = input;
            BattleFactoryApp_UpdatePreviewPokepic(app);
            BattleFactoryApp_ShowMonIcon(app, app->party, app->cursorPos, 6);
            BattleFactoryApp_PrintRentalNames(app, app->cursorPos, 6);
            BattleFactoryApp_OpenRentalMenu(app);
            break;
        case 6:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 2, 12, 6);
            app->subState = 2;
            break;
        case 7:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 18, 12, 7);
            app->subState = 2;
            break;
        case 8:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 10, 15, 8);
            app->subState = 2;
            break;
        case GRID_MENU_CURSOR_MOVE:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_UpdatePreviewPokepic(app);
            BattleFactoryApp_ShowMonIcon(app, app->party, GridInputHandler_GetNextInput(app->grid), 6);
            BattleFactoryApp_PrintRentalNames(app, GridInputHandler_GetNextInput(app->grid), 6);
            break;
        case GRID_MENU_CANCEL:
            if ((u32)GridInputHandler_GetNextInput(app->grid) >= 6) {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_CloseRentalMenu(app);
                return TRUE;
            }
            if (app->numPicked != 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_UndoLastRental(app);
                if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                    BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_RENTALS, 0);
                }
                return TRUE;
            }
            break;
        }
        break;
    case 2:
        BattleFactoryApp_UpdateButtonFlash(app);
        break;
    case 3:
        if (IsPaletteFadeFinished() == TRUE) {
            if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0) {
                PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(app->paletteData);
                app->paletteBlinkState = 0xFF;
            } else {
                BattleFactoryApp_CreateSummaryArgs(app);
                BattleFactoryApp_FreeGraphics(app);
                app->summaryApp = OverlayManager_New(&gOverlayTemplate_PokemonSummary, app->summaryArgs, HEAP_ID_100);
                app->summaryOpen = TRUE;
                return TRUE;
            }
        }
        break;
    case 4:
        if (app->summaryApp == NULL) {
            return TRUE;
        }
        break;
    case 5:
        if (IsPaletteFadeFinished() == TRUE) {
            app->subState = 1;
        }
        break;
    case 6:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_100);
        app->summaryFading = TRUE;
        app->subState = 3;
        break;
    case 7:
        BattleFactoryApp_CloseRentalMenu(app);
        if (BattleFactoryBall_IsSelected(app->balls[app->cursorPos]) == 0) {
            BattleFactoryApp_AddRental(app);
        } else {
            BattleFactoryApp_RemoveRental(app);
        }
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_RENTALS, 0);
        }
        return TRUE;
    case 8:
        BattleFactoryApp_CloseRentalMenu(app);
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

void BattleFactoryApp_UndoLastRental(BattleFactoryApp *app) {
    Pokepic_SetAttr(app->pokepics[app->numPicked], POKEPIC_VANISHED, TRUE);
    FillWindowPixelBuffer(&app->windows[app->numPicked + 2], 0);
    ScheduleWindowCopyToVram(&app->windows[app->numPicked + 2]);
    app->numPicked--;
    BattleFactoryApp_BufferNumber(app, 0, app->numPicked + 1);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    BattleFactoryBall_Deselect(app->balls[app->picked[app->numPicked]]);
    BattleFactoryBall_SetPaletteOverride(app->balls[app->picked[app->numPicked]], 1);
    BattleFactoryBall_SetAnim(app->balls[app->picked[app->numPicked]], 0);
    app->picked[app->numPicked] = 0;
    BattleFactoryApp_UpdatePreviewPokepic(app);
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_UpdateOrderSprites(app);
    BattleFactoryApp_PrintRentalNames(app, GridInputHandler_GetNextInput(app->grid), 6);
}

void BattleFactoryApp_AddRental(BattleFactoryApp *app) {
    u32 numPicked;

    BattleFactoryBall_Select(app->balls[app->cursorPos]);
    BattleFactoryBall_SetPaletteOverride(app->balls[app->cursorPos], 0);
    BattleFactoryBall_SetAnim(app->balls[app->cursorPos], 1);
    app->picked[app->numPicked] = app->cursorPos;
    app->numPicked++;
    numPicked = app->numPicked;
    if (numPicked != BattleFactory_GetRentalCount(app->frontierType)) {
        BattleFactoryApp_BufferNumber(app, 0, numPicked + 1);
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    }
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_UpdateOrderSprites(app);
}

void BattleFactoryApp_RemoveRental(BattleFactoryApp *app) {
    int count;
    int i;

    BattleFactoryBall_Deselect(app->balls[app->cursorPos]);
    BattleFactoryBall_SetPaletteOverride(app->balls[app->cursorPos], 1);
    BattleFactoryBall_SetAnim(app->balls[app->cursorPos], 0);
    if (app->numPicked >= 2 && app->picked[0] == app->cursorPos) {
        app->picked[0] = app->picked[1];
        BattleFactoryApp_ReplacePokepic(app, 0, app->picked[0], 0);
        Pokepic_SetAttr(app->pokepics[0], POKEPIC_VANISHED, FALSE);
    }
    app->numPicked--;
    app->picked[app->numPicked] = 0;
    count = BattleFactory_GetRentalCount(app->frontierType);
    for (i = 0; i < count; i++) {
        FillWindowPixelBuffer(&app->windows[i + 2], 0);
    }
    BattleFactoryApp_BufferNumber(app, 0, app->numPicked + 1);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    BattleFactoryApp_UpdatePreviewPokepic(app);
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_UpdateOrderSprites(app);
    BattleFactoryApp_PrintRentalNames(app, app->cursorPos, 6);
}

#pragma force_active on
BOOL BattleFactoryApp_ConfirmRentals(BattleFactoryApp *app) {
    int result = BattleFactory_GetPartySize(app->frontierType);

    switch (app->subState) {
    case 0:
        BattleFactorySprite_SetVisible(app->cursor, FALSE);
        BattleFactorySprite_SetVisible(app->picCursor, FALSE);
        app->timer = 0;
        app->subState++;
        break;
    case 1:
        app->timer = 0;
        app->subState++;
        break;
    case 2:
        app->subState++;
        break;
    case 3:
        app->timer = 0;
        app->subState++;
        break;
    case 4:
        BattleFactoryApp_BufferNumber(app, 0, result);
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00001);
        BattleFactoryApp_InitYesNo(app->yesNoPrompt, app->bgConfig);
        BattleFactoryApp_SetBallsTranslucent(app, TRUE);
        app->timer = 0;
        app->subState++;
        break;
    case 5:
        result = YesNoPrompt_HandleInput(app->yesNoPrompt);
        switch (result) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactorySprite_SetVisible(app->cursor, TRUE);
            BattleFactorySprite_SetVisible(app->picCursor, TRUE);
            app->numPicked--;
            app->picked[app->numPicked] = 0;
            if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_RENTALS, 0);
            }
            return TRUE;
        }
        break;
    case 6:
        if (BattleFactory_IsMultiType(app->frontierType) == FALSE) {
            return TRUE;
        }
        if (BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_RENTALS, 0) == TRUE) {
            app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00002);
            LinkSync_ClearPackets();
            LinkSync_StartSync(164);
            app->subState++;
        }
        break;
    case 7:
        if (LinkSync_IsSynced(164) == TRUE) {
            LinkSync_ClearPackets();
            return TRUE;
        }
        break;
    }
    return FALSE;
}
#pragma force_active reset

BOOL BattleFactoryApp_CancelLastRental(BattleFactoryApp *app) {
    BattleFactoryApp_DeselectCursorRental(app);
    return TRUE;
}

void BattleFactoryApp_DeselectCursorRental(BattleFactoryApp *app) {
    BattleFactory_GetPartySize(app->frontierType);
    BattleFactoryBall_Deselect(app->balls[app->cursorPos]);
    BattleFactoryBall_SetPaletteOverride(app->balls[app->cursorPos], 1);
    BattleFactoryBall_SetAnim(app->balls[app->cursorPos], 0);
    BattleFactoryApp_BufferNumber(app, 0, app->numPicked + 1);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00000);
    BattleFactoryApp_UpdateSlotMarkers(app);
    BattleFactoryApp_UpdateOrderSprites(app);
}

#pragma force_active on
BOOL BattleFactoryApp_SelectSwapOut(BattleFactoryApp *app) {
    u32 input;

    switch (app->subState) {
    case 0:
        BattleFactoryApp_SetCancelRequested(app, FALSE);
        app->subState = 1;
        app->summaryOpen = FALSE;
        break;
    case 1:
        input = GridInputHandler_HandleInput_NoHold(app->grid);
        switch (input) {
        case 0:
        case 1:
        case 2:
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorPos = input;
            BattleFactoryApp_ShowMonIcon(app, app->party, app->cursorPos, 4);
            BattleFactoryApp_UpdateSwapOutPicCursor(app);
            BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
            BattleFactoryApp_OpenSwapOutMenu(app);
            BattleFactorySprite_SetPalette(app->cursor, 1);
            break;
        case 4:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 2, 12, 7);
            app->subState = 2;
            break;
        case 5:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 18, 12, 8);
            app->subState = 2;
            break;
        case 6:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 10, 15, 9);
            app->subState = 2;
            break;
        case 3:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
            BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            BgCellManager_EraseCell(app->bgCellManager, 0);
            BattleFactoryApp_UpdateSwapOutPicCursor(app);
            BattleFactoryApp_FlashNarrowButton(app, 23, 15, 6);
            app->subState = 2;
            break;
        case GRID_MENU_CURSOR_MOVE:
            PlaySE(SEQ_SE_DP_SELECT);
            {
                u32 next = GridInputHandler_GetNextInput(app->grid);
                if (next == 3) {
                    BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
                    BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
                    ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
                    BgCellManager_EraseCell(app->bgCellManager, 0);
                } else {
                    BattleFactoryApp_ShowMonIcon(app, app->party, next, 3);
                    ScheduleWindowCopyToVram(&app->windows[8]);
                    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
                }
                BattleFactoryApp_UpdateSwapOutPicCursor(app);
            }
            break;
        case GRID_MENU_CANCEL:
            if ((u32)GridInputHandler_GetNextInput(app->grid) >= 4) {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_CloseSwapOutMenu(app);
                BattleFactorySprite_SetPalette(app->cursor, 0);
            } else {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_ClearSwapOutMenu(app);
                BattleFactorySprite_SetPalette(app->cursor, 1);
                BattleFactoryApp_SetCancelRequested(app, TRUE);
                return TRUE;
            }
            break;
        }
        break;
    case 2:
        BattleFactoryApp_UpdateButtonFlash(app);
        break;
    case 3:
        if (IsPaletteFadeFinished() == TRUE) {
            if (PaletteData_GetSelectedBuffersBitmask(app->paletteData) != 0) {
                PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(app->paletteData);
                app->paletteBlinkState = 0xFF;
            } else {
                BattleFactoryApp_CreateSummaryArgs(app);
                BattleFactoryApp_FreeGraphics(app);
                app->summaryApp = OverlayManager_New(&gOverlayTemplate_PokemonSummary, app->summaryArgs, HEAP_ID_100);
                app->summaryOpen = TRUE;
                return TRUE;
            }
        }
        break;
    case 4:
        if (app->summaryApp == NULL) {
            return TRUE;
        }
        break;
    case 5:
        if (IsPaletteFadeFinished() == TRUE) {
            app->subState = 1;
        }
        break;
    case 6:
        BattleFactoryApp_ClearSwapOutMenu(app);
        BattleFactoryApp_SetCancelRequested(app, TRUE);
        return TRUE;
    case 7:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_100);
        app->summaryFading = TRUE;
        app->subState = 3;
        break;
    case 8:
        BattleFactoryApp_PickSwapOut(app);
        BattleFactoryApp_CloseSwapOutMenu(app);
        BattleFactoryApp_ClearSwapOutMenu(app);
        return TRUE;
    case 9:
        BattleFactoryApp_CloseSwapOutMenu(app);
        BattleFactorySprite_SetPalette(app->cursor, 0);
        app->subState = 1;
        break;
    }
    return FALSE;
}
#pragma force_active reset

void BattleFactoryApp_PickSwapOut(BattleFactoryApp *app) {
    app->picked[app->numPicked] = app->cursorPos;
    app->numPicked++;
    app->swapOutPicked = TRUE;
}

#pragma force_active on
BOOL BattleFactoryApp_ConfirmCancelSwap(BattleFactoryApp *app) {
    switch (app->subState) {
    case 0:
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00010);
        BattleFactoryApp_InitYesNo(app->yesNoPrompt, app->bgConfig);
        BattleFactoryApp_SetBallsTranslucent(app, TRUE);
        app->subState++;
        break;
    case 1:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00002);
            }
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactoryApp_ResumeSwapOut(app);
            BattleFactorySprite_SetPalette(app->cursor, 0);
            app->subState++;
            break;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

void BattleFactoryApp_ResumeSwapOut(BattleFactoryApp *app) {
    BattleFactoryApp_RedrawSwapOutButtons(app);
    BattleFactoryApp_SetCancelRequested(app, TRUE);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00009);
}

#pragma force_active on
BOOL BattleFactoryApp_SelectSwapIn(BattleFactoryApp *app) {
    u32 input;

    switch (app->subState) {
    case 0:
        BattleFactoryApp_DrawMessageFrame(&app->windows[7], Options_GetFrame(app->options));
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00013);
        app->subState = 1;
        break;
    case 1:
        input = GridInputHandler_HandleInput_NoHold(app->grid);
        switch (input) {
        case 0:
        case 1:
        case 2:
        case 3:
            PlaySE(SEQ_SE_DP_SELECT);
            app->cursorPos = input;
            BattleFactoryApp_ShowMonIcon(app, app->otherParty, app->cursorPos, 4);
            BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
            BattleFactoryApp_UpdateSwapInPicCursor(app);
            BattleFactoryApp_PrintSwapInName(app, app->cursorPos);
            BattleFactoryApp_OpenSwapInMenu(app);
            BattleFactorySprite_SetPalette(app->cursor, 1);
            break;
        case 4:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
            BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
            Pokepic_SetAttr(app->pokepics[app->picked[0]], POKEPIC_VANISHED, TRUE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[app->picked[0] + 2]);
            BgCellManager_EraseCell(app->bgCellManager, 0);
            BattleFactoryApp_FlashNarrowButton(app, 2, 15, 5);
            app->subState = 4;
            break;
        case 5:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
            BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
            Pokepic_SetAttr(app->pokepics[app->picked[0]], POKEPIC_VANISHED, TRUE);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[app->picked[0] + 2]);
            BgCellManager_EraseCell(app->bgCellManager, 0);
            BattleFactoryApp_FlashNarrowButton(app, 23, 15, 6);
            app->subState = 4;
            break;
        case 6:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 2, 15, 7);
            app->subState = 4;
            break;
        case 7:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleFactoryApp_FlashWideButton(app, 18, 15, 8);
            app->subState = 4;
            break;
        case GRID_MENU_CURSOR_MOVE:
            PlaySE(SEQ_SE_DP_SELECT);
            input = GridInputHandler_GetNextInput(app->grid);
            if ((u32)(input - 4) <= 1) {
                BattleFactorySprite_SetVisible(app->monIcons[0], FALSE);
                BattleFactorySprite_SetVisible(app->monIcons[1], FALSE);
                Pokepic_SetAttr(app->pokepics[app->picked[0]], POKEPIC_VANISHED, TRUE);
                ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
                ClearWindowTilemapAndScheduleTransfer(&app->windows[app->picked[0] + 2]);
                BgCellManager_EraseCell(app->bgCellManager, 0);
            } else {
                switch (input) {
                case 6:
                case 7:
                    break;
                default:
                    BattleFactoryApp_ShowMonIcon(app, app->otherParty, input, 4);
                    ScheduleWindowCopyToVram(&app->windows[8]);
                    BgCellManager_DrawCellAt(app->bgCellManager, 0, 7, 0);
                    BattleFactoryApp_UpdateSwapInPicCursor(app);
                    BattleFactoryApp_PrintSwapInName(app, input);
                    break;
                }
            }
            break;
        case GRID_MENU_CANCEL:
            if ((u32)GridInputHandler_GetNextInput(app->grid) >= 6) {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_CloseSwapInMenu(app);
                BattleFactorySprite_SetPalette(app->cursor, 0);
            } else {
                PlaySE(SEQ_SE_DP_SELECT);
                BattleFactoryApp_ClearSwapInMenu(app);
                BattleFactorySprite_SetPalette(app->cursor, 1);
                BattleFactoryApp_SetCancelRequested(app, TRUE);
                return TRUE;
            }
            break;
        }
        break;
    case 2:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactoryApp_PickSwapIn(app);
            if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00002);
            }
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactoryApp_ShowSwapInPrompt(app);
            BattleFactoryApp_RedrawSwapInButtons(app);
            BattleFactorySprite_SetPalette(app->cursor, 0);
            app->subState = 1;
            break;
        }
        break;
    case 3:
        return TRUE;
    case 4:
        BattleFactoryApp_UpdateButtonFlash(app);
        break;
    case 5:
        BattleFactoryApp_ClearSwapInMenu(app);
        BattleFactoryApp_UnpickSwapOut(app);
        return TRUE;
    case 6:
        BattleFactoryApp_ClearSwapInMenu(app);
        BattleFactoryApp_SetCancelRequested(app, TRUE);
        return TRUE;
    case 7:
        BattleFactoryApp_CloseSwapInMenu(app);
        BattleFactoryApp_ClearSwapInMenu(app);
        BattleFactoryApp_InitYesNo(app->yesNoPrompt, app->bgConfig);
        BattleFactoryApp_SetBallsTranslucent(app, TRUE);
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00014);
        app->subState = 2;
        break;
    case 8:
        BattleFactoryApp_CloseSwapInMenu(app);
        BattleFactorySprite_SetPalette(app->cursor, 0);
        app->subState = 1;
        break;
    }
    return FALSE;
}
#pragma force_active reset

void BattleFactoryApp_UnpickSwapOut(BattleFactoryApp *app) {
    app->swapOutPicked = FALSE;
    app->numPicked--;
    app->picked[0] = 0;
    app->cursorPos = 0;
    GridInputHandler_SetNextInput(app->grid, 0);
}

void BattleFactoryApp_PickSwapIn(BattleFactoryApp *app) {
    app->swapAccepted = TRUE;
    app->picked[app->numPicked] = app->cursorPos;
    app->numPicked++;
}

void BattleFactoryApp_ShowSwapInPrompt(BattleFactoryApp *app) {
    u32 frame = Options_GetFrame(app->options);
    BattleFactoryApp_DrawMessageFrame(&app->windows[7], frame);
    app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00013);
}

#pragma force_active on
BOOL BattleFactoryApp_ConfirmCancelSwapIn(BattleFactoryApp *app) {
    switch (app->subState) {
    case 0:
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00010);
        BattleFactoryApp_InitYesNo(app->yesNoPrompt, app->bgConfig);
        BattleFactoryApp_SetBallsTranslucent(app, TRUE);
        app->subState++;
        break;
    case 1:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            app->swapAccepted = FALSE;
            if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
                app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00002);
            }
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            BattleFactoryApp_SetBallsTranslucent(app, FALSE);
            BattleFactoryApp_ResumeSwapIn(app);
            BattleFactorySprite_SetPalette(app->cursor, 0);
            app->subState++;
            break;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

void BattleFactoryApp_ResumeSwapIn(BattleFactoryApp *app) {
    BattleFactoryApp_RedrawSwapInButtons(app);
    BattleFactoryApp_SetCancelRequested(app, TRUE);
}

BOOL BattleFactoryApp_SwitchPage(BattleFactoryApp *app) {
    int count;
    int i;

    BattleFactory_GetPartySize(app->frontierType);
    switch (app->subState) {
    case 0:
        app->switchingPage = TRUE;
        ClearWindowTilemapAndCopyToVram(&app->windows[1]);
        ClearWindowTilemapAndCopyToVram(&app->windows[5]);
        ClearWindowTilemapAndCopyToVram(&app->windows[6]);
        FillWindowPixelBuffer(&app->windows[0], 0);
        ScheduleWindowCopyToVram(&app->windows[0]);
        ClearWindowTilemapAndCopyToVram(&app->windows[2]);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        ClearWindowTilemapAndCopyToVram(&app->windows[8]);
        BgCellManager_EraseCell(app->bgCellManager, 0);
        ClearFrameAndWindow2(&app->windows[7], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[7]);
        BattleFactoryApp_RemoveWindows(app->windows);
        Pokepic_StartPaletteFadeAll(app->pokepicManager, 0, 16, 0, 0xFFFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        app->timer = 0;
        app->subState++;
        break;
    case 1:
        app->timer++;
        if (app->timer < 2) {
            break;
        }
        for (i = 0; i < app->numPokepics; i++) {
            Pokepic_Delete(app->pokepics[i]);
        }
        BattleFactorySprite_Delete(app->cursor);
        BattleFactorySprite_Delete(app->buttonCursor);
        BattleFactorySprite_Delete(app->picCursor);
        BattleFactorySprite_Delete(app->monIcons[0]);
        BattleFactorySprite_Delete(app->monIcons[1]);
        BattleFactoryApp_DeleteSlotMarkers(app);
        BattleFactoryApp_DeleteOrderSprites(app);
        BattleFactorySprite_SetAnim(app->centerSprite, 2);
        app->timer = 0;
        app->subState++;
        break;
    case 2:
        Pokepic_ResumePaletteFade(app->pokepics[0]);
        if (BattleFactorySprite_IsAnimated(app->centerSprite) == TRUE) {
            break;
        }
        BattleFactorySprite_SetVisible(app->centerSprite, FALSE);
        ov81_02240728(app, GF_BG_LYR_SUB_2);
        ov81_022406E0(app, GF_BG_LYR_MAIN_2);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, app->bgScrollX);
        PlaySE(SEQ_SE_DP_ELEBETA2);
        app->timer = 0;
        app->subState++;
        break;
    case 3:
        if (BattleFactoryApp_ScrollBallsOut(app) != TRUE) {
            break;
        }
        StopSE(SEQ_SE_DP_ELEBETA2, 0);
        PlaySE(SEQ_SE_DP_KASYA);
        app->bgAnimTimer = 8;
        for (count = 0; count < app->numBalls; count++) {
            app->balls[count] = BattleFactoryBall_Delete(app->balls[count]);
            app->balls[count] = NULL;
        }
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            if (app->numBalls == 2) {
                app->numBalls = 4;
            } else {
                app->numBalls = 2;
            }
        }
        BattleFactoryApp_CreateSwapBalls(app);
        for (i = 0; i < app->numBalls; i++) {
            BattleFactoryBall_MoveOffscreen(app->balls[i]);
        }
        PlaySE(SEQ_SE_DP_ELEBETA2);
        app->timer = 0;
        app->subState++;
        break;
    case 4:
        if (BattleFactoryApp_ScrollBallsIn(app) != TRUE) {
            break;
        }
        StopSE(SEQ_SE_DP_ELEBETA2, 0);
        PlaySE(SEQ_SE_DP_KASYA);
        for (i = 0; i < app->numBalls; i++) {
            BattleFactoryBall_SetAnim(app->balls[i], 4);
        }
        app->bgAnimTimer = 8;
        BattleFactorySprite_SetVisible(app->centerSprite, TRUE);
        BattleFactorySprite_SetAnim(app->centerSprite, 1);
        app->timer = 0;
        app->subState++;
        break;
    case 5:
        if (BattleFactorySprite_IsAnimated(app->centerSprite) == TRUE) {
            break;
        }
        app->timer = 0;
        app->subState++;
        break;
    case 6:
        if (app->timer == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
            if (app->swapOutPicked == FALSE) {
                for (count = 0; count < app->numPokepics; count++) {
                    BattleFactoryApp_ReplacePokepic(app, count, count, 1);
                }
            } else {
                for (count = 0; count < app->numPokepics; count++) {
                    if (count == app->picked[0]) {
                        BattleFactoryApp_ReplacePokepicFromOther(app, count, 0, 0);
                    } else {
                        BattleFactoryApp_ReplacePokepic(app, count, count, 1);
                    }
                }
            }
            Pokepic_StartPaletteFadeAll(app->pokepicManager, 16, 0, 1, 0xFFFF);
        }
        Pokepic_ResumePaletteFade(app->pokepics[0]);
        app->timer++;
        if (app->timer < 21) {
            break;
        }
        BattleFactoryApp_InitWindows(app);
        app->switchingPage = FALSE;
        if (BattleFactory_IsMultiType(app->frontierType) == TRUE) {
            app->partnerUpdated = TRUE;
            BattleFactoryApp_PrintPartnerRentals(app);
        }
        app->timer = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL BattleFactoryApp_LinkSendSwap(BattleFactoryApp *app) {
    switch (app->subState) {
    case 0:
        if (BattleFactoryApp_SendLinkCommand(app, FACTORY_LINK_SWAP, app->swapAccepted) == TRUE) {
            app->subState++;
        }
        break;
    case 1:
        if (app->recvCount >= 2) {
            app->recvCount = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL BattleFactoryApp_LinkSync(BattleFactoryApp *app) {
    switch (app->subState) {
    case 0:
        app->printerId = BattleFactoryApp_ShowMessage(app, msg_0194_00015);
        LinkSync_ClearPackets();
        LinkSync_StartSync(165);
        app->subState++;
        break;
    case 1:
        if (LinkSync_IsSynced(165) == TRUE) {
            LinkSync_ClearPackets();
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL BattleFactoryApp_FadeOut(BattleFactoryApp *app) {
    switch (app->subState) {
    case 0:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_100);
        app->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL BattleFactoryApp_ShowLinkTradeResult(BattleFactoryApp *app) {
    switch (app->subState) {
    case 0:
        if (app->unk_378[1] != NULL) {
            Delete2dMenu(app->unk_17C, NULL);
            BattleFactoryApp_CloseWindow(app->unk_174);
            BattleFactoryBall_Free(app->unk_378[1]);
            app->unk_378[1] = NULL;
        }
        Frontier_BufferPartnerName(app->msgFormat, 0);
        BattleFactoryApp_BufferSpeciesName(app, 1, Mon_GetBoxMon(Party_GetMonByIndex(app->party, app->picked[0] + 2)));
        BattleFactoryApp_BufferSpeciesName(app, 2, Mon_GetBoxMon(Party_GetMonByIndex(app->otherParty, app->picked[1])));
        BattleFactoryApp_DrawMessageFrame(&app->windows[7], Options_GetFrame(app->options));
        app->printerId = BattleFactoryApp_PrintMessage(app, &app->windows[7], msg_0194_00016, 1, 1, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(app->saveData)), 1, 2, 15, 1);
        ScheduleWindowCopyToVram(&app->windows[7]);
        app->timer = 80;
        app->subState++;
        break;
    case 1:
        if (TextPrinterCheckActive(app->printerId) == 0) {
            app->timer = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void BattleFactoryApp_VBlankCallback(BattleFactoryApp *app) {
    if (app->summaryApp == NULL) {
        PokepicManager_HandleLoadImgAndOrPltt(app->pokepicManager);
        if (app->paletteData != NULL) {
            PaletteData_PushTransparentBuffers(app->paletteData);
        }
        DoScheduledBgGpuUpdates(app->bgConfig);
        GF_RunVramTransferTasks();
        OamManager_ApplyAndResetBuffers();
        {
            // OS_SetIrqCheckFlag(OS_IE_V_BLANK) with the DTCM address written out
            vu32 *dtcm = (vu32 *)0x027E0000;
            dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
        }
    }
}

void BattleFactoryApp_SetGraphicsBanks(void) {
    GraphicsBanks banks = ov81_0224356C;
    GfGfx_SetBanks(&banks);
}

void BattleFactoryApp_InitBgLayers(BgConfig *bgConfig) {
    {
        GraphicsModes modes = ov81_02243480;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        BgTemplate template = ov81_022434A8;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
    }
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    {
        BgTemplate template = ov81_022434C4;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    {
        BgTemplate template = ov81_022434FC;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    {
        BgTemplate template = ov81_02243518;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
    }
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    {
        BgTemplate template = ov81_022434E0;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
    }
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    {
        BgTemplate template = ov81_02243534;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    {
        BgTemplate template = ov81_02243550;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &template, GF_BG_TYPE_TEXT);
    }
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    G2_SetBG0Priority(0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
}

void BattleFactoryApp_InitBgGraphics(BattleFactoryApp *app) {
    BattleFactoryApp_SetGraphicsBanks();
    BattleFactoryApp_InitBgLayers(app->bgConfig);
    app->paletteData = PaletteData_Init(HEAP_ID_100);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_100);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_100);
    BattleFactoryApp_LoadBgPalette();
    ov81_02240728(app, GF_BG_LYR_SUB_2);
    ov81_02240698(app, GF_BG_LYR_MAIN_3);
    ov81_022406E0(app, GF_BG_LYR_MAIN_2);
    if (app->shown) {
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, app->bgScrollX);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    BattleFactoryApp_LoadSubScreenBg(app, GF_BG_LYR_SUB_3);
}

void BattleFactoryApp_InitSprites(BattleFactoryApp *app) {
    BattleFactorySpriteGfx_Init(&app->spriteGfx);
}

#pragma force_active on
void BattleFactoryApp_FreeBgLayers(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(bgConfig);
}
#pragma force_active reset

void BattleFactoryApp_SetMainState(BattleFactoryApp *app, int *state, int nextState) {
    app->subState = 0;
    *state = nextState;
}

#pragma force_active on
BOOL BattleFactoryApp_ScrollBallsIn(BattleFactoryApp *app) {
    int done = FALSE;
    int i;
    VecFx32 pos;

    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_ADD_X, 8);
    app->bgScrollX = Bg_GetXpos(app->bgConfig, GF_BG_LYR_SUB_2);
    for (i = 0; i < app->numBalls; i++) {
        if (BattleFactoryBall_GetPosition(app->balls[i])->x / FX32_ONE - 8 <= BattleFactoryBall_GetX(app->balls[i])) {
            int x = BattleFactoryBall_GetX(app->balls[i]);
            int y = BattleFactoryBall_GetY(app->balls[i]);
            BattleFactoryBall_SetPosition(app->balls[i], x, y);
            done = TRUE;
        } else {
            BattleFactoryBall_Move(&pos, app->balls[i], -8, 0);
        }
    }
    BattleFactoryApp_AnimateBackground(app);
    if (done == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL BattleFactoryApp_ScrollBallsOut(BattleFactoryApp *app) {
    int hidden = 0;
    int i;
    VecFx32 pos;

    BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_ADD_X, 8);
    app->bgScrollX = Bg_GetXpos(app->bgConfig, GF_BG_LYR_SUB_2);
    for (i = 0; i < app->numBalls; i++) {
        int x = BattleFactoryBall_GetPosition(app->balls[i])->x / FX32_ONE - 8;
        if (x < -24) {
            BattleFactoryBall_SetVisible(app->balls[i], FALSE);
            hidden++;
        } else {
            BattleFactoryBall_Move(&pos, app->balls[i], -8, 0);
        }
    }
    BattleFactoryApp_AnimateBackground(app);
    if (hidden == app->numBalls) {
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

void BattleFactoryApp_AnimateBackground(BattleFactoryApp *app) {
    u32 timer = app->bgAnimTimer;
    if ((timer & 3) == 0) {
        if ((timer >> 2) >= 4) {
            app->bgAnimTimer = 0;
        }
        BattleFactoryApp_SetBgPalette(app, GF_BG_LYR_SUB_3, (u8)((app->bgAnimTimer >> 2) + 2), 32, 32);
    }
    app->bgAnimTimer++;
}

void BattleFactoryApp_SetBgPalette(BattleFactoryApp *app, int bgId, u8 palette, u8 width, u8 height) {
    BgTilemapRectChangePalette(app->bgConfig, bgId, 0, 0, width, height, palette);
    ScheduleBgTilemapBufferTransfer(app->bgConfig, bgId);
}

void BattleFactoryApp_SetCenterAnim(BattleFactoryApp *app, int kind) {
    int animSeq;

    switch (kind) {
    case 0:
        animSeq = 1;
        break;
    case 1:
        animSeq = 2;
        break;
    case 4:
        animSeq = 1;
        break;
    case 5:
        animSeq = 2;
        break;
    default:
        animSeq = 4;
        break;
    }
    app->centerSprite = BattleFactoryApp_CreateCenterSprite(app, animSeq);
}

void ov81_02240698(BattleFactoryApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x81, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x82, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
}

void ov81_022406E0(BattleFactoryApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x92, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x91, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
}

void ov81_02240728(BattleFactoryApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 4, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 8, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
}

void BattleFactoryApp_LoadBgPalette(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0xC1, &plttData, HEAP_ID_100);
    DC_FlushRange(plttData->pRawData, 0x160);
    GX_LoadBGPltt(plttData->pRawData, 0, 0x160);
    Heap_Free(file);
}

void BattleFactoryApp_LoadSubScreenBg(BattleFactoryApp *app, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 4, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 7, app->bgConfig, bgId, 0, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_GXLoadPalFromOpenNarc(app->narc, 0x95, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x160, HEAP_ID_100);
}

#pragma force_active on
u8 BattleFactoryApp_PrintMessage(BattleFactoryApp *app, Window *window, u32 msgId, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    FillWindowPixelBuffer(window, bg);
    ReadMsgDataIntoString(app->msgData, msgId, app->rawString);
    StringExpandPlaceholders(app->msgFormat, app->expandedString, app->rawString);
    return AddTextPrinterParameterizedWithColor(window, fontId, app->expandedString, x, y, textSpeed, TEXT_COLOR(fg, shadow, bg), NULL);
}

u8 BattleFactoryApp_ShowMessage(BattleFactoryApp *app, u32 msgId) {
    u8 printerId = BattleFactoryApp_PrintMessage(app, &app->windows[7], msgId, 1, 1, TEXT_SPEED_NOTRANSFER, 1, 2, 15, 1);
    ScheduleWindowCopyToVram(&app->windows[7]);
    return printerId;
}

void BattleFactoryApp_BufferNumber(BattleFactoryApp *app, u32 field, s32 value) {
    BufferIntegerAsString(app->msgFormat, field, value, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
}
#pragma force_active reset

void BattleFactoryApp_BufferSpeciesName(BattleFactoryApp *app, u32 field, BoxPokemon *boxMon) {
    BufferBoxMonSpeciesName(app->msgFormat, field, boxMon);
}

void BattleFactoryApp_PrintPlayerName(BattleFactoryApp *app, Window *window, int x, int y, u8 fontId) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(app->saveData);
    String *string = String_New(PLAYER_NAME_LENGTH + 1, HEAP_ID_100);
    u32 color;

    FillWindowPixelBuffer(window, 0);
    CopyU16ArrayToString(string, PlayerProfile_GetNamePtr(profile));
    if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
        color = MAKE_TEXT_COLOR(7, 8, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

void BattleFactoryApp_PrintPartnerName(BattleFactoryApp *app, Window *window, int x, int y, u8 fontId) {
    PlayerProfile *profile = CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId());
    String *string = String_New(PLAYER_NAME_LENGTH + 1, HEAP_ID_100);
    u32 color;

    FillWindowPixelBuffer(window, 0);
    PlayerName_FlatToString(profile, string);
    if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
        color = MAKE_TEXT_COLOR(7, 8, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

#pragma force_active on
void BattleFactoryApp_PrintMonName(BattleFactoryApp *app, Window *window, int monIndex, int x, int y, u8 fg, u8 shadow, volatile u8 bg, u8 fontId, Party *party) {
    Pokemon *mon;
    String *name;
    String *format;
    int gender;
    u16 right;

    FillWindowPixelBuffer(window, bg);
    mon = Party_GetMonByIndex(party, monIndex);
    name = String_New(POKEMON_NAME_LENGTH + 1, HEAP_ID_100);
    format = NewString_ReadMsgData(app->msgData, msg_0194_00028);
    BufferBoxMonSpeciesName(app->msgFormat, 0, Mon_GetBoxMon(mon));
    StringExpandPlaceholders(app->msgFormat, name, format);
    // the (int) cast keeps the original register allocation
    AddTextPrinterParameterizedWithColor(window, fontId, name, x, (int)y, TEXT_SPEED_NOTRANSFER, TEXT_COLOR(fg, shadow, bg), NULL);
    String_Delete(format);
    String_Delete(name);
    if (GetMonData(mon, MON_DATA_NO_PRINT_GENDER, NULL) == TRUE) {
        gender = GetMonGender(mon);
        right = ((GetWindowWidth(window) - 1) << 3) - 4;
        if (gender == MON_MALE) {
            name = NewString_ReadMsgData(app->msgData, msg_0194_00026);
            AddTextPrinterParameterizedWithColor(window, fontId, name, right, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 0), NULL);
            String_Delete(name);
        } else if (gender == MON_FEMALE) {
            name = NewString_ReadMsgData(app->msgData, msg_0194_00027);
            AddTextPrinterParameterizedWithColor(window, fontId, name, right, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
            String_Delete(name);
        }
    }
    ScheduleWindowCopyToVram(window);
}

void BattleFactoryApp_PrintPartnerMonName(BattleFactoryApp *app, Window *window, int x, int y, u8 fg, u8 shadow, volatile u8 bg, u8 fontId, u16 species, u8 gender) {
    MsgData *speciesNames;
    String *string;
    u8 width;
    int genderMsg;
    int color;

    FillWindowPixelBuffer(window, (u8)bg);
    speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_100);
    string = NewString_ReadMsgData(speciesNames, (u16)species);
    DestroyMsgData(speciesNames);
    AddTextPrinterParameterizedWithColor(window, (u8)fontId, string, x, y, TEXT_SPEED_NOTRANSFER, TEXT_COLOR(fg, shadow, bg), NULL);
    width = GetWindowWidth(window) - 1;
    genderMsg = gender == MON_MALE ? msg_0194_00026 : msg_0194_00027;
    color = gender == MON_MALE ? MAKE_TEXT_COLOR(7, 8, 0) : MAKE_TEXT_COLOR(3, 4, 0);
    String_SetEmpty(string);
    if (gender != MON_GENDERLESS) {
        ReadMsgDataIntoString(app->msgData, genderMsg, string);
        AddTextPrinterParameterizedWithColor(window, (u8)fontId, string, width << 3, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    }
    String_Delete(string);
}
#pragma force_active reset

void BattleFactoryApp_FreeGraphics(BattleFactoryApp *app) {
    BattleFactoryApp_FreeBgCellManager(app);
    {
        int i;
        for (i = 0; i < app->numBalls; i++) {
            if (app->balls[i] != NULL) {
                app->balls[i] = BattleFactoryBall_Delete(app->balls[i]);
            }
        }
    }
    BattleFactorySprite_Delete(app->cursor);
    BattleFactorySprite_Delete(app->buttonCursor);
    BattleFactorySprite_Delete(app->picCursor);
    BattleFactorySprite_Delete(app->monIcons[0]);
    BattleFactorySprite_Delete(app->monIcons[1]);
    BattleFactoryApp_DeleteCenterSprites(app);
    BattleFactoryApp_DeleteSlotMarkers(app);
    BattleFactoryApp_DeleteOrderSprites(app);
    WirelessStatusIcon_Destroy();
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(app->paletteData);
    app->paletteData = NULL;
    BattleFactorySpriteGfx_Free(&app->spriteGfx);
    {
        int i;
        for (i = 0; i < 3; i++) {
            if (app->pokepics[i] != NULL) {
                Pokepic_Delete(app->pokepics[i]);
            }
        }
    }
    PokepicManager_Delete(app->pokepicManager);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
    String_Delete(app->expandedString);
    String_Delete(app->rawString);
    {
        int i;
        for (i = 0; i < 4; i++) {
            String_Delete(app->strings[i]);
        }
    }
    FontID_Release(4);
    BattleFactoryApp_RemoveWindows(app->windows);
    BattleFactoryApp_FreeBgLayers(app->bgConfig);
    NARC_Delete(app->narc);
    GF_3DVramMan_Delete(app->vramMan);
}

void BattleFactoryApp_ResumeAfterSummary(BattleFactoryApp *app) {
    BattleFactoryApp_DisableDisplay();
    app->vramMan = GF_3DVramMan_Create(HEAP_ID_100, 0, 2, 0, 2, BattleFactoryApp_Setup3D);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_100);
    BattleFactoryApp_InitGraphics(app);
    BattleFactoryApp_AddWindows(app->bgConfig, app->windows);
    if (app->numPokepics == 2) {
        BattleFactoryApp_SetLinkNameWindows(&app->windows[2]);
    }
}

void BattleFactoryApp_DisableDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

#pragma force_active on
void BattleFactoryApp_InitGraphics(BattleFactoryApp *app) {
    int i;

    app->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_100);
    BattleFactoryApp_InitBgGraphics(app);
    BattleFactoryApp_InitSprites(app);
    FontID_Alloc(4, HEAP_ID_100);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0194_bin, HEAP_ID_100);
    app->msgFormat = MessageFormat_New(HEAP_ID_100);
    app->expandedString = String_New(800, HEAP_ID_100);
    app->rawString = String_New(800, HEAP_ID_100);
    for (i = 0; i < 4; i++) {
        app->strings[i] = String_New(64, HEAP_ID_100);
    }
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_100);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_100);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_100);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_100);
    app->pokepicManager = PokepicManager_Create(HEAP_ID_100);
    if (Comm_IsActive()) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        WirelessStatusIcon_Create();
    }
    if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
        BattleFactoryApp_CreateRentalBalls(app);
        BattleFactoryApp_CreateRentalPokepics(app);
    } else {
        BattleFactoryApp_CreateSwapBalls(app);
        BattleFactoryApp_CreateSwapPokepics(app);
    }
    BattleFactoryApp_LoadSubPalette(app);
    BattleFactoryApp_LoadButtonGraphics(app->bgConfig, app->narc);
    BattleFactoryApp_InitBgCellManager(app);
    GfGfx_BothDispOn();
    Main_SetVBlankIntrCB((GFIntrCB)BattleFactoryApp_VBlankCallback, app);
}
#pragma force_active reset

void BattleFactoryApp_CreateSummaryArgs(BattleFactoryApp *app) {
    app->summaryArgs = Heap_Alloc(HEAP_ID_100, sizeof(PokemonSummaryArgs));
    memset(app->summaryArgs, 0, sizeof(PokemonSummaryArgs));
    app->summaryArgs->party = app->party;
    app->summaryArgs->options = app->options;
    app->summaryArgs->monDataType = 1;
    app->summaryArgs->mode = 1;
    app->summaryArgs->partyCount = app->numBalls;
    app->summaryArgs->partySlot = app->cursorPos;
    app->summaryArgs->moveToLearn = 0;
    app->summaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(app->saveData);
    app->summaryArgs->unk2C = 0;
    app->summaryArgs->isFlag982Set = 0;
    PokemonSummary_SetPages(app->summaryArgs, _02243458);
    PokemonSummaryArgs_SetPlayer(app->summaryArgs, Save_PlayerData_GetProfile(app->saveData));
}

BOOL BattleFactoryApp_IsScreen(BattleFactoryApp *app, u8 screen) {
    return app->screen == screen;
}

int BattleFactory_GetRentalCount(int frontierType) {
    return frontierType == 0 || frontierType == 1 ? 3 : 2;
}

#pragma force_active on
BOOL BattleFactoryApp_IsCancelRequested(BattleFactoryApp *app) {
    if (app->cancelRequested) {
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

void BattleFactoryApp_SetCancelRequested(BattleFactoryApp *app, u8 value) {
    app->cancelRequested = value;
}

#pragma force_active on
void BattleFactoryApp_CreatePokepic(BattleFactoryApp *app, int index, Pokemon *mon, int x, int y, int hFlip) {
    u32 species;

    app->pokepics[index] = BattleFactory_CreatePokepic(app->pokepicManager, 0, mon, x, y, 0);
    Pokepic_SetAttr(app->pokepics[index], POKEPIC_VANISHED, FALSE);
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    if (GetMonBaseStat_HandleAlternateForm(species, GetMonData(mon, MON_DATA_FORM, NULL), BASE_FLIP) == 0) {
        BattleFactory_SetPokepicHFlip(app->pokepics[index], hFlip);
    }
}
#pragma force_active reset

BOOL BattleFactoryApp_SendLinkCommand(BattleFactoryApp *app, int command, int arg) {
    int linkCommand;

    if (!BattleFactory_IsMultiType(app->frontierType)) {
        return FALSE;
    }
    switch (command) {
    case 7:
        linkCommand = 29;
        BattleFactoryApp_PrepareLinkPing(app, command);
        break;
    case FACTORY_LINK_RENTALS:
        linkCommand = 30;
        BattleFactoryApp_PrepareLinkRentals(app, command, arg);
        break;
    case FACTORY_LINK_SWAP:
        linkCommand = 32;
        BattleFactoryApp_PrepareLinkSwap(app, command, arg);
        break;
    }
    if (Comm_SendCommand(linkCommand, app->sendBuf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void BattleFactoryApp_PrepareLinkPing(BattleFactoryApp *app, int command) {
    Save_PlayerData_GetProfile(app->saveData);
    app->sendBuf[0] = command;
}

void BattleFactory_ReceivePing(int netId) {
    if (netId != Comm_GetLocalNetId()) {
        return;
    }
}

#pragma force_active on
void BattleFactoryApp_PrepareLinkRentals(BattleFactoryApp *app, int command, int arg) {
    int i = 0;
    int pos = i;

    for (; i < 60; i++) {
        app->sendBuf[i] = 0;
    }
    app->sendBuf[0] = app->numPicked;
    pos++;
    for (i = 0; i < app->numPicked; i++) {
        app->sendBuf[pos + i] = (u16)GetMonData(Party_GetMonByIndex(app->party, app->picked[i]), MON_DATA_SPECIES, NULL);
    }
    pos += app->numPicked;
    for (i = 0; i < app->numPicked; i++) {
        app->sendBuf[pos + i] = (u16)GetMonData(Party_GetMonByIndex(app->party, app->picked[i]), MON_DATA_GENDER, NULL);
    }
}

void BattleFactory_ReceiveRentals(int netId, int size, const u16 *data, BattleFactoryApp *app) {
    int i;
    int pos = 0;

    BattleFactory_GetPartySize(app->frontierType);
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    app->numPartnerMons = data[0];
    pos++;
    for (i = 0; i < app->numPartnerMons; i++) {
        app->partnerSpecies[i] = data[pos + i];
    }
    pos += app->numPartnerMons;
    for (i = 0; i < app->numPartnerMons; i++) {
        app->partnerGender[i] = data[pos + i];
    }
    app->partnerUpdated = TRUE;
}
#pragma force_active reset

void BattleFactory_ReceiveUnused(void) {
    GF_AssertFail();
}

#pragma force_active on
void BattleFactoryApp_PrintPartnerRentals(BattleFactoryApp *app) {
    int count = BattleFactory_GetPartySize(app->frontierType);

    if (app->switchingPage == TRUE) {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[1]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[5]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[6]);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        return;
    }
    if (app->partnerUpdated == TRUE) {
        int i;
        for (i = 0; i < count; i++) {
            Window *window = &app->windows[i + 5];
            FillWindowPixelBuffer(window, 0);
            if (i < app->numPartnerMons) {
                BattleFactoryApp_PrintPartnerMonName(app, window, 0, 0, 15, 2, 0, 0, app->partnerSpecies[i], app->partnerGender[i]);
            }
            ScheduleWindowCopyToVram(window);
        }
        BattleFactoryApp_PrintPartnerName(app, &app->windows[1], 0, 0, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    }
    app->partnerUpdated = FALSE;
}
#pragma force_active reset

void BattleFactoryApp_PrepareLinkSwap(BattleFactoryApp *app, int command, int accepted) {
    app->sendBuf[0] = command;
    app->sendBuf[1] = accepted;
    app->sendBuf[2] = app->picked[0];
    app->sendBuf[3] = app->picked[1];
}

void BattleFactory_ReceiveSwap(int netId, int size, const BattleFactoryLinkMessage *msg, BattleFactoryApp *app) {
#pragma unused(size)
    app->recvCount++;
    if (netId != Comm_GetLocalNetId()) {
        app->partnerSwapState = (u8)msg->accepted;
        if (Comm_GetLocalNetId() == 0) {
            if (app->swapAccepted == TRUE) {
                app->partnerSwapState = 0;
                return;
            }
            if (app->partnerSwapState == 1) {
                app->picked[0] = (u8)msg->picked0;
                app->picked[1] = (u8)msg->picked1;
            }
        } else if (app->partnerSwapState == 1) {
            app->swapAccepted = FALSE;
            app->picked[0] = (u8)msg->picked0;
            app->picked[1] = (u8)msg->picked1;
        }
    }
}

BattleFactoryBall *BattleFactoryApp_CreateBall(BattleFactoryApp *app, int index) {
    u16 x;
    u16 y;

    if (BattleFactoryApp_IsScreen(app, FACTORY_SCREEN_RENTAL) == TRUE) {
        x = ov81_02243490[index * 2];
        y = ov81_02243492[index * 2];
    } else if (BattleFactory_IsMultiType(app->frontierType) == FALSE) {
        x = ov81_02243464[index * 2];
        y = ov81_02243466[index * 2];
    } else if (app->numBalls == 2) {
        x = ov81_0224345C[index * 2];
        y = ov81_0224345E[index * 2];
    } else {
        x = ov81_02243470[index * 2];
        y = ov81_02243472[index * 2];
    }
    return BattleFactoryBall_Create(&app->spriteGfx, x, y, HEAP_ID_100);
}

BattleFactorySprite *BattleFactoryApp_CreateCenterSprite(BattleFactoryApp *app, int animSeq) {
    return BattleFactorySprite_Create(&app->spriteGfx, 1, animSeq, 2, 10, 128, 124);
}

void BattleFactoryApp_DeleteCenterSprites(BattleFactoryApp *app) {
    if (app->centerSprite != NULL) {
        BattleFactorySprite_Delete(app->centerSprite);
        app->centerSprite = NULL;
    }
    if (app->unk_384 != NULL) {
        BattleFactorySprite_Delete(app->unk_384);
        app->unk_384 = NULL;
    }
}

void BattleFactoryApp_InitWindows(BattleFactoryApp *app) {
    SetBgPriority(GF_BG_LYR_MAIN_1, 1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_100);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BattleFactoryApp_AddWindows(app->bgConfig, app->windows);
    if (app->numPokepics == 2) {
        BattleFactoryApp_SetLinkNameWindows(&app->windows[2]);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
}

void BattleFactoryApp_CloseWindow(Window *window) {
    EraseFrameAndWindow1(window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(window);
}

void BattleFactoryApp_MoveRentalCursor(BattleFactoryApp *app, int target) {
    u32 x;
    u32 y;

    BattleFactory_GetGridCursorPos(app->grid, target, &x, &y);
    BattleFactorySprite_SetPositionSub(app->cursor, x, y);
}

void BattleFactoryApp_MoveButtonCursor(BattleFactoryApp *app, int target) {
    u32 x;
    u32 y;

    BattleFactory_GetGridCursorPos(app->grid, target, &x, &y);
    BattleFactorySprite_SetAnim(app->buttonCursor, 3);
    BattleFactorySprite_SetPositionSub(app->buttonCursor, x, y);
}

void BattleFactoryApp_OpenRentalMenu(BattleFactoryApp *app) {
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 0);
    ScheduleWindowCopyToVram(&app->windows[11]);
    ScheduleWindowCopyToVram(&app->windows[14]);
    if (!BattleFactoryBall_IsSelected(app->balls[app->cursorPos])) {
        ScheduleWindowCopyToVram(&app->windows[12]);
    } else {
        ScheduleWindowCopyToVram(&app->windows[13]);
    }
    GridInputHandler_SetNextInput(app->grid, 6);
    BattleFactoryApp_MoveButtonCursor(app, 6);
    BattleFactory_SetRentalGridMenu(app->grid, TRUE);
    BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
    BattleFactorySprite_SetPalette(app->cursor, 1);
}

void BattleFactoryApp_CloseRentalMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 0);
    GridInputHandler_SetNextInput(app->grid, (u8)app->cursorPos);
    BattleFactory_SetRentalGridMenu(app->grid, FALSE);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
    BattleFactorySprite_SetPalette(app->cursor, 0);
}

#pragma force_active on
void BattleFactoryApp_PrintRentalMenu(BattleFactoryApp *app) {
    int width;

    FillWindowPixelBuffer(&app->windows[11], 2);
    FillWindowPixelBuffer(&app->windows[12], 2);
    FillWindowPixelBuffer(&app->windows[13], 2);
    FillWindowPixelBuffer(&app->windows[14], 2);
    width = GetWindowWidth(&app->windows[11]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[11], app->msgData, msg_0194_00005, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[12]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[12], app->msgData, msg_0194_00006, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[13]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[13], app->msgData, msg_0194_00008, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[14]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[14], app->msgData, msg_0194_00007, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[11]);
    CopyWindowPixelsToVram_TextMode(&app->windows[12]);
    CopyWindowPixelsToVram_TextMode(&app->windows[13]);
    CopyWindowPixelsToVram_TextMode(&app->windows[14]);
}
#pragma force_active reset

void BattleFactoryApp_MoveSwapOutCursor(BattleFactoryApp *app, int target) {
    u32 x;
    u32 y;

    BattleFactory_GetGridCursorPos(app->grid, target, &x, &y);
    if (target == 3) {
        BattleFactorySprite_SetVisible(app->cursor, FALSE);
        BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
        BattleFactorySprite_SetAnim(app->buttonCursor, 6);
        BattleFactorySprite_SetPositionSub(app->buttonCursor, x, y);
    } else {
        BattleFactorySprite_SetVisible(app->cursor, TRUE);
        BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
        BattleFactorySprite_SetPositionSub(app->cursor, x, y);
    }
}

void BattleFactoryApp_MoveSwapInCursor(BattleFactoryApp *app, int target) {
    u32 x;
    u32 y;

    BattleFactory_GetGridCursorPos(app->grid, target, &x, &y);
    if ((u32)(target - 4) <= 1) {
        BattleFactorySprite_SetVisible(app->cursor, FALSE);
        BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
        BattleFactorySprite_SetAnim(app->buttonCursor, 6);
        BattleFactorySprite_SetPositionSub(app->buttonCursor, x, y);
    } else {
        BattleFactorySprite_SetVisible(app->cursor, TRUE);
        BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
        BattleFactorySprite_SetPositionSub(app->cursor, x, y);
    }
}

void BattleFactoryApp_OpenSwapOutMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 3);
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 0);
    ScheduleWindowCopyToVram(&app->windows[11]);
    ScheduleWindowCopyToVram(&app->windows[15]);
    ScheduleWindowCopyToVram(&app->windows[14]);
    GridInputHandler_SetNextInput(app->grid, 4);
    BattleFactoryApp_MoveButtonCursor(app, 4);
    BattleFactory_SetSwapOutGridMenu(app->grid, TRUE, app->frontierType);
    BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
}

void BattleFactoryApp_CloseSwapOutMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 0);
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 3);
    ScheduleWindowCopyToVram(&app->windows[10]);
    GridInputHandler_SetNextInput(app->grid, (u8)app->cursorPos);
    BattleFactory_SetSwapOutGridMenu(app->grid, FALSE, app->frontierType);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
}

void BattleFactoryApp_RedrawSwapOutButtons(BattleFactoryApp *app) {
    int target = GridInputHandler_GetNextInput(app->grid);

    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 3);
    ScheduleWindowCopyToVram(&app->windows[10]);
    if (target == 3) {
        BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
    }
}

void BattleFactoryApp_ClearSwapOutMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 3);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
}

void BattleFactoryApp_PrintSwapOutMenu(BattleFactoryApp *app) {
    FillWindowPixelBuffer(&app->windows[11], 2);
    FillWindowPixelBuffer(&app->windows[15], 2);
    FillWindowPixelBuffer(&app->windows[14], 2);
    FillWindowPixelBuffer(&app->windows[10], 2);
    BattleFactoryApp_PrintMsgAligned(&app->windows[11], app->msgData, msg_0194_00020, GetWindowWidth(&app->windows[11]) * 8 / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    BattleFactoryApp_PrintMsgAligned(&app->windows[15], app->msgData, msg_0194_00021, GetWindowWidth(&app->windows[15]) * 8 / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    BattleFactoryApp_PrintMsgAligned(&app->windows[14], app->msgData, msg_0194_00022, GetWindowWidth(&app->windows[14]) * 8 / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    BattleFactoryApp_PrintMsgAligned(&app->windows[10], app->msgData, msg_0194_00019, GetWindowWidth(&app->windows[10]) * 8 / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[11]);
    CopyWindowPixelsToVram_TextMode(&app->windows[15]);
    CopyWindowPixelsToVram_TextMode(&app->windows[14]);
    CopyWindowPixelsToVram_TextMode(&app->windows[10]);
}

void BattleFactoryApp_OpenSwapInMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 2);
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 1);
    ScheduleWindowCopyToVram(&app->windows[16]);
    ScheduleWindowCopyToVram(&app->windows[17]);
    GridInputHandler_SetNextInput(app->grid, 6);
    BattleFactoryApp_MoveButtonCursor(app, 6);
    BattleFactory_SetSwapInGridMenu(app->grid, TRUE, app->frontierType);
    BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
}

void BattleFactoryApp_CloseSwapInMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 1);
    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 2);
    ScheduleWindowCopyToVram(&app->windows[9]);
    ScheduleWindowCopyToVram(&app->windows[10]);
    GridInputHandler_SetNextInput(app->grid, (u8)app->cursorPos);
    BattleFactory_SetSwapInGridMenu(app->grid, FALSE, app->frontierType);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
}

void BattleFactoryApp_RedrawSwapInButtons(BattleFactoryApp *app) {
    int target = GridInputHandler_GetNextInput(app->grid);

    BattleFactoryApp_DrawButtons(app->bgConfig, app->narc, 2);
    ScheduleWindowCopyToVram(&app->windows[9]);
    ScheduleWindowCopyToVram(&app->windows[10]);
    if ((u32)target >= 4) {
        BattleFactorySprite_SetVisible(app->buttonCursor, TRUE);
    }
}

void BattleFactoryApp_ClearSwapInMenu(BattleFactoryApp *app) {
    BattleFactoryApp_ClearButtons(app->bgConfig, 2);
    BattleFactorySprite_SetVisible(app->buttonCursor, FALSE);
}

#pragma force_active on
void BattleFactoryApp_PrintSwapInMenu(BattleFactoryApp *app) {
    int width;

    FillWindowPixelBuffer(&app->windows[16], 2);
    FillWindowPixelBuffer(&app->windows[17], 2);
    FillWindowPixelBuffer(&app->windows[9], 2);
    FillWindowPixelBuffer(&app->windows[10], 2);
    width = GetWindowWidth(&app->windows[16]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[16], app->msgData, msg_0194_00025, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[17]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[17], app->msgData, msg_0194_00022, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[9]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[9], app->msgData, msg_0194_00023, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    width = GetWindowWidth(&app->windows[10]);
    BattleFactoryApp_PrintMsgAligned(&app->windows[10], app->msgData, msg_0194_00024, (width * 8) / 2, 0, 4, MAKE_TEXT_COLOR(15, 14, 2), 2);
    CopyWindowPixelsToVram_TextMode(&app->windows[16]);
    CopyWindowPixelsToVram_TextMode(&app->windows[17]);
    CopyWindowPixelsToVram_TextMode(&app->windows[9]);
    CopyWindowPixelsToVram_TextMode(&app->windows[10]);
}
#pragma force_active reset

void BattleFactoryApp_CreateYesNo(YesNoPrompt **prompt) {
    *prompt = YesNoPrompt_Create(HEAP_ID_100);
}

void BattleFactoryApp_DestroyYesNo(YesNoPrompt *prompt) {
    YesNoPrompt_Destroy(prompt);
}

void BattleFactoryApp_InitYesNo(YesNoPrompt *prompt, BgConfig *bgConfig) {
    YesNoPromptTemplate template;

    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_SUB_0;
    template.tileStart = 0x380;
    template.plttSlot = 14;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = FALSE;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
}

#pragma force_active on
void BattleFactoryApp_UpdatePreviewPokepic(BattleFactoryApp *app) {
    u32 target = GridInputHandler_GetNextInput(app->grid);

    if (target < 6) {
        int count = app->numPicked;
        u16 i = 0;
        for (; i < count; i++) {
            if (target == app->picked[i]) {
                break;
            }
        }
        if (count != 0 && i != count) {
            Pokepic_SetAttr(app->pokepics[count], POKEPIC_VANISHED, TRUE);
        } else {
            BattleFactoryApp_ReplacePokepic(app, count, target, 0);
            Pokepic_SetAttr(app->pokepics[app->numPicked], POKEPIC_VANISHED, FALSE);
        }
        BattleFactoryApp_MovePicCursor(app);
    }
}
#pragma force_active reset

void BattleFactory_GetPokepicPos(int slot, int numPokepics, int *x, int *y) {
    if (numPokepics == 3) {
        *x = slot * 80 + 48;
    } else {
        *x = slot * 112 + 72;
    }
    *y = 144;
}

#pragma force_active on
void BattleFactoryApp_GetCursorPokepicPos(BattleFactoryApp *app, int *x, int *y) {
    u32 target = GridInputHandler_GetNextInput(app->grid);
    u16 i;

    if (target < 6) {
        for (i = 0; i < app->numPicked; i++) {
            if (target == app->picked[i]) {
                break;
            }
        }
        BattleFactory_GetPokepicPos(i, app->numPokepics, x, y);
    }
}
#pragma force_active reset

void BattleFactoryApp_MovePicCursor(BattleFactoryApp *app) {
    int x;
    int y;

    BattleFactoryApp_GetCursorPokepicPos(app, &x, &y);
    BattleFactorySprite_SetPosition(app->picCursor, x, y);
}

int BattleFactoryApp_GetCursorSlot(BattleFactoryApp *app) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (app->cursorPos == app->picked[i]) {
            return i;
        }
    }
    return app->numPicked;
}

void BattleFactoryApp_UpdateSwapOutPicCursor(BattleFactoryApp *app) {
    int x;
    int y;
    u32 target = GridInputHandler_GetNextInput(app->grid);

    if (target >= 4) {
        return;
    }
    if (target == 3) {
        BattleFactorySprite_SetVisible(app->picCursor, FALSE);
        return;
    }
    BattleFactorySprite_SetVisible(app->picCursor, TRUE);
    BattleFactory_GetPokepicPos(target, app->numPokepics, &x, &y);
    BattleFactorySprite_SetPosition(app->picCursor, x, y);
}

void BattleFactoryApp_UpdateSwapInPicCursor(BattleFactoryApp *app) {
    u32 target = GridInputHandler_GetNextInput(app->grid);

    if (target >= 6) {
        return;
    }
    if (target >= 4) {
        BattleFactorySprite_SetVisible(app->picCursor, FALSE);
        return;
    }
    BattleFactorySprite_SetVisible(app->picCursor, TRUE);
    BattleFactoryApp_ReplacePokepicFromOther(app, app->picked[0], target, 0);
}

void BattleFactoryApp_CreatePartyPokepics(BattleFactoryApp *app, int hFlip) {
    u32 offset;
    u32 x;
    u32 step;
    u32 i;
    u8 *slot;
    u8 *pic;

    if (app->numPokepics == 3) {
        x = 48;
        step = 80;
    } else {
        x = 72;
        step = 112;
    }
    // byte cursors over the work struct reproduce the original code
    i = 0;
    if (i < app->numPokepics) {
        offset = i;
        slot = (u8 *)app;
        pic = (u8 *)app;
        do {
            Pokemon *mon = Party_GetMonByIndex(app->party, ((BattleFactoryApp *)slot)->picked[0]);
            BattleFactoryApp_CreatePokepic(app, (u8)i, mon, x + offset, 144, hFlip);
            Pokepic_SetAttr(((BattleFactoryApp *)pic)->pokepics[0], POKEPIC_VANISHED, TRUE);
            i++;
            offset += step;
            pic += sizeof(Pokepic *);
            slot += sizeof(u16);
        } while (i < app->numPokepics);
    }
}

void BattleFactoryApp_ReplacePokepic(BattleFactoryApp *app, int slot, int monIndex, int hFlip) {
    Pokepic_Delete(app->pokepics[slot]);
    if (app->numPokepics == 3) {
        Pokemon *mon = Party_GetMonByIndex(app->party, monIndex);
        BattleFactoryApp_CreatePokepic(app, (u8)slot, mon, slot * 80 + 48, 144, hFlip);
    } else {
        Pokemon *mon = Party_GetMonByIndex(app->party, monIndex);
        BattleFactoryApp_CreatePokepic(app, (u8)slot, mon, slot * 112 + 72, 144, hFlip);
    }
}

void BattleFactoryApp_ReplacePokepicFromOther(BattleFactoryApp *app, int slot, int monIndex, int hFlip) {
    Pokepic_Delete(app->pokepics[slot]);
    if (app->numPokepics == 3) {
        Pokemon *mon = Party_GetMonByIndex(app->otherParty, monIndex);
        BattleFactoryApp_CreatePokepic(app, (u8)slot, mon, slot * 80 + 48, 144, hFlip);
    } else {
        Pokemon *mon = Party_GetMonByIndex(app->otherParty, monIndex);
        BattleFactoryApp_CreatePokepic(app, (u8)slot, mon, slot * 112 + 72, 144, hFlip);
    }
}

void BattleFactoryApp_CreateSlotMarkers(BattleFactoryApp *app) {
    u32 x;
    u32 step;
    u32 i;

    if (app->numPokepics == 3) {
        x = 48;
        step = 80;
    } else {
        x = 72;
        step = 112;
    }
    for (i = 0; i < app->numPokepics; i++) {
        app->slotMarkers[i] = BattleFactorySprite_Create(&app->spriteGfx, 1, 3, 1, 0, x + i * step, 144);
        BattleFactorySprite_SetVisible(app->slotMarkers[i], FALSE);
    }
}

void BattleFactoryApp_DeleteSlotMarkers(BattleFactoryApp *app) {
    u32 i;

    for (i = 0; i < app->numPokepics; i++) {
        BattleFactorySprite_Delete(app->slotMarkers[i]);
    }
}

void BattleFactoryApp_UpdateSlotMarkers(BattleFactoryApp *app) {
    u32 i;

    for (i = 0; i < app->numPokepics; i++) {
        if (i == app->numPicked) {
            BattleFactorySprite_SetPaletteOverride(app->slotMarkers[i], 3);
            BattleFactorySprite_SetVisible(app->slotMarkers[i], TRUE);
        } else if (i < app->numPicked) {
            BattleFactorySprite_SetPaletteOverride(app->slotMarkers[i], 2);
            BattleFactorySprite_SetVisible(app->slotMarkers[i], TRUE);
        } else {
            BattleFactorySprite_SetVisible(app->slotMarkers[i], FALSE);
        }
    }
}

void BattleFactoryApp_SetSlotMarkers(BattleFactoryApp *app, int highlightCursor) {
    u32 i;

    for (i = 0; i < app->numPokepics; i++) {
        if (i == app->cursorPos && highlightCursor == TRUE) {
            BattleFactorySprite_SetPaletteOverride(app->slotMarkers[i], 3);
        } else {
            BattleFactorySprite_SetPaletteOverride(app->slotMarkers[i], 2);
        }
        BattleFactorySprite_SetVisible(app->slotMarkers[i], TRUE);
    }
}

void BattleFactoryApp_CreateOrderSprites(BattleFactoryApp *app) {
    u16 i;
    u16 animSeq;
    int step;
    int x;

    if (app->numPokepics == 3) {
        x = 16;
        step = 80;
    } else {
        x = 40;
        step = 112;
    }
    animSeq = 0;
    i = 0;
    do {
        app->orderSprites[i] = BattleFactorySprite_Create(&app->spriteGfx, 2, animSeq, 0, 0, x + step * i, 176);
        animSeq++;
        i++;
    } while (i < 3);
    if (app->numPokepics == 2) {
        BattleFactorySprite_SetVisible(app->orderSprites[2], FALSE);
    }
    animSeq = 0;
    while (i < 6) {
        app->orderSprites[i] = BattleFactorySprite_CreateSub(&app->spriteGfx, 3, animSeq, 0, 0, 0, 0);
        BattleFactorySprite_SetVisible(app->orderSprites[i], FALSE);
        animSeq++;
        i++;
    }
}

void BattleFactoryApp_DeleteOrderSprites(BattleFactoryApp *app) {
    u32 i = 0;

    for (; i < 6; i++) {
        BattleFactorySprite_Delete(app->orderSprites[i]);
    }
}

void BattleFactoryApp_UpdateOrderSprites(BattleFactoryApp *app) {
    u16 i = 3;

    do {
        u8 n = (u8)(i - 3);
        if (n < app->numPicked) {
            VecFx32 pos = *BattleFactoryBall_GetPosition(app->balls[app->picked[n]]);
            pos.x -= 4 * FX32_ONE;
            pos.y += 4 * FX32_ONE;
            BattleFactorySprite_SetMatrix(app->orderSprites[i], &pos);
            BattleFactorySprite_SetVisible(app->orderSprites[i], TRUE);
        } else {
            BattleFactorySprite_SetVisible(app->orderSprites[i], FALSE);
        }
        i++;
    } while (i < 6);
}

void BattleFactoryApp_ShowMonIcon(BattleFactoryApp *app, Party *party, u32 index, u32 limit) {
    Pokemon *mon;
    u16 species;
    u16 form;
    u32 fileId;
    u32 palette;
    void *buffer;
    NNSG2dCharacterData *charData;
    BattleFactorySprite *icon;

    if (index >= limit) {
        return;
    }
    mon = Party_GetMonByIndex(party, index);
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    form = GetMonData(mon, MON_DATA_FORM, NULL);
    fileId = GetMonIconNaixEx(species, FALSE, form);
    buffer = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, fileId, FALSE, &charData, HEAP_ID_100);
    icon = app->monIcons[app->monIconIndex];
    BattleFactorySprite_LoadCharData(icon, charData->pRawData, 0x200, 2);
    Heap_Free(buffer);
    palette = GetMonIconPaletteEx(species, form, FALSE);
    BattleFactorySprite_SetPalette(app->monIcons[app->monIconIndex], palette);
    BattleFactorySprite_SetVisible(app->monIcons[app->monIconIndex], TRUE);
    app->monIconIndex ^= 1;
    icon = app->monIcons[app->monIconIndex];
    BattleFactorySprite_SetVisible(icon, FALSE);
    BattleFactoryApp_PrintMonName(app, &app->windows[8], (u8)index, 4, 0, 15, 2, 0, 0, party);
}

void BattleFactoryApp_PrintRentalNames(BattleFactoryApp *app, u32 target, u32 limit) {
    u16 i;
    volatile BOOL found;
    s32 count;

    if (target >= limit) {
        return;
    }
    i = 0;
    found = FALSE;
    count = *(volatile u8 *)&app->numPicked;
    if (count > 0) {
        do {
            u16 picked = app->picked[i];
            if (target == picked) {
                found = TRUE;
            }
            BattleFactoryApp_PrintMonName(app, &app->windows[i + 2], (u8)picked, 4, 0, 15, 2, 0, 0, app->party);
            i++;
        } while (i < app->numPicked);
    }
    if (!found) {
        BattleFactoryApp_PrintMonName(app, &app->windows[i + 2], (u8)target, 4, 0, 15, 2, 0, 0, app->party);
        i++;
    }
    for (; i < app->numPokepics; i++) {
        FillWindowPixelBuffer(&app->windows[i + 2], 0);
        ScheduleWindowCopyToVram(&app->windows[i + 2]);
    }
}

void BattleFactoryApp_PrintPartyNames(BattleFactoryApp *app, Party *party) {
    u32 i;

    for (i = 0; i < app->numPokepics; i++) {
        BattleFactoryApp_PrintMonName(app, &app->windows[i + 2], (u8)i, 4, 0, 15, 2, 0, 0, party);
    }
}

void BattleFactoryApp_PrintSwapNames(BattleFactoryApp *app, u32 swapIn) {
    u32 i = 0;

    if (i < app->numPokepics) {
        Window *windows = app->windows;
        u8 swapInIndex = swapIn;
        do {
            if (app->picked[0] == i) {
                BattleFactoryApp_PrintMonName(app, &windows[i + 2], swapInIndex, 4, 0, 15, 2, 0, 0, app->otherParty);
            } else {
                BattleFactoryApp_PrintMonName(app, &windows[i + 2], (u8)i, 4, 0, 15, 2, 0, 0, app->party);
            }
            i++;
        } while (i < app->numPokepics);
    }
}

void BattleFactoryApp_PrintSwapInName(BattleFactoryApp *app, u32 index) {
    u32 slot = app->picked[0] + 2;

    if (index >= 4) {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[slot]);
    } else {
        BattleFactoryApp_PrintMonName(app, &app->windows[slot], (u8)index, 4, 0, 15, 2, 0, 0, app->otherParty);
        ScheduleWindowCopyToVram(&app->windows[slot]);
    }
}

void BattleFactoryApp_LoadSubPalette(BattleFactoryApp *app) {
    PaletteData_LoadPaletteSlotFromHardware(app->paletteData, PLTTBUF_MAIN_OBJ, 0, 0x200);
}

#pragma force_active on
void BattleFactoryApp_UpdatePaletteBlink(BattleFactoryApp *app) {
    if (app->paletteData != NULL && app->paletteBlinkState != 0xFF && PaletteData_GetSelectedBuffersBitmask(app->paletteData) == 0) {
        if (app->paletteBlinkState == 0) {
            PaletteData_BeginPaletteFade(app->paletteData, PLTTBUF_MAIN_OBJ_F, 8, 2, 0, 16, 0xFFFF);
        } else {
            PaletteData_BeginPaletteFade(app->paletteData, PLTTBUF_MAIN_OBJ_F, 8, 2, 16, 0, 0xFFFF);
        }
        app->paletteBlinkState ^= 1;
    }
}

void BattleFactoryApp_StartButtonFlash(BattleFactoryApp *app, u8 x, u8 y, u8 width, u8 height, u8 nextState) {
    app->flashX = x;
    app->flashY = y;
    app->flashWidth = width;
    app->flashHeight = height;
    app->flashNextState = nextState;
    app->flashCount = 0;
    app->flashTimer = 0;
    app->flashPalette = 0;
}
#pragma force_active reset

void BattleFactoryApp_FlashWideButton(BattleFactoryApp *app, u8 x, u8 y, u8 nextState) {
    BattleFactoryApp_StartButtonFlash(app, x, y, 12, 2, nextState);
}

void BattleFactoryApp_FlashNarrowButton(BattleFactoryApp *app, u8 x, u8 y, u8 nextState) {
    BattleFactoryApp_StartButtonFlash(app, x, y, 7, 2, nextState);
}

#pragma force_active on
BOOL BattleFactoryApp_UpdateButtonFlash(BattleFactoryApp *app) {
    if (app->flashTimer == 0) {
        if (app->flashPalette == 0) {
            BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_1, app->flashX, app->flashY, app->flashWidth, app->flashHeight, 10);
        } else {
            BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_1, app->flashX, app->flashY, app->flashWidth, app->flashHeight, 9);
        }
        ScheduleBgTilemapBufferTransfer(app->bgConfig, GF_BG_LYR_SUB_1);
        app->flashPalette ^= 1;
    }
    app->flashTimer++;
    if (app->flashTimer == 2) {
        app->flashCount++;
        if (app->flashCount == 4) {
            app->subState = app->flashNextState;
            return FALSE;
        }
        app->flashTimer = 0;
    }
    return TRUE;
}

void BattleFactoryApp_SetBallsTranslucent(BattleFactoryApp *app, int enable) {
    u32 i;

    if (enable == TRUE) {
        G2S_SetBlendAlpha(0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 6, 10);
        for (i = 0; i < app->numBalls; i++) {
            if (app->balls[i] != NULL) {
                BattleFactoryBall_SetOamMode(app->balls[i], GX_OAM_MODE_XLU);
            }
        }
    } else {
        G2_BlendNone();
        for (i = 0; i < app->numBalls; i++) {
            if (app->balls[i] != NULL) {
                BattleFactoryBall_SetOamMode(app->balls[i], GX_OAM_MODE_NORMAL);
            }
        }
    }
}
#pragma force_active reset

BOOL BattleFactory_IsLinkMode(int frontierType) {
    return BattleFactory_IsMultiType((u8)frontierType);
}

BOOL BattleFactoryApp_IsLinkMode(BattleFactoryApp *app) {
    return BattleFactory_IsMultiType(app->frontierType);
}

#pragma force_active on
void BattleFactoryApp_InitBgCellManager(BattleFactoryApp *app) {
    app->bgCellManager = BgCellManager_New(app->bgConfig, 2, 1, HEAP_ID_100);
    BgCellManager_InitCell(app->bgCellManager, 0, 4, 18, 4);
    BgCellManager_LoadCellTilemapFromOpenNarc(app->bgCellManager, 0, app->narc, 144, 1);
}
#pragma force_active reset

void BattleFactoryApp_FreeBgCellManager(BattleFactoryApp *app) {
    BgCellManager_Delete(app->bgCellManager);
}

void BattleFactorySpriteGfx_Init(BattleFactorySpriteGfx *gfx) {
    int i;

    BattleFactoryApp_InitObjTransfers();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, HEAP_ID_100);
    gfx->spriteList = G2dRenderer_Init(32, &gfx->renderer, HEAP_ID_100);
    G2dRenderer_SetSubSurfaceCoords(&gfx->renderer, 0, 2 << 20);
    for (i = 0; i < 4; i++) {
        gfx->resMan[i] = Create2DGfxResObjMan(ov81_02243594[i], (GfGfxResType)i, HEAP_ID_100);
    }
    gfx->resObj[1][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0x28, TRUE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_100);
    gfx->resObj[1][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_a_1_8_4, 0x3E, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 4, HEAP_ID_100);
    gfx->resObj[1][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 0x29, TRUE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    gfx->resObj[1][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 0x2A, TRUE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    gfx->resObj[2][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0x2B, TRUE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_100);
    gfx->resObj[2][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_a_1_8_4, 0x3F, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_100);
    gfx->resObj[2][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 0x2C, TRUE, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    gfx->resObj[2][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 0x2D, TRUE, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    gfx->resObj[0][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0, TRUE, 0, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_100);
    gfx->resObj[0][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_a_1_8_4, 0x34, FALSE, 0, NNS_G2D_VRAM_TYPE_2DSUB, 8, HEAP_ID_100);
    gfx->resObj[0][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 2, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    gfx->resObj[0][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 1, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    gfx->resObj[3][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0x2B, TRUE, 3, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_100);
    gfx->resObj[3][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_a_1_8_4, 0x3F, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_100);
    gfx->resObj[3][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 0x2C, TRUE, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    gfx->resObj[3][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 0x2D, TRUE, 3, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    for (i = 0; i < 2; i++) {
        gfx->resObj[i + 4][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0x2E, TRUE, i + 4, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_100);
        gfx->resObj[i + 4][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, i + 4, NNS_G2D_VRAM_TYPE_2DSUB, 3, HEAP_ID_100);
        gfx->resObj[i + 4][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 0x2F, TRUE, i + 4, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
        gfx->resObj[i + 4][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 0x30, TRUE, i + 4, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    }
    for (i = 0; i < 6; i++) {
        SpriteTransfer_CreateCharTransferTask(gfx->resObj[i][0]);
        SpriteTransfer_CreateExtPlttTransferTask(gfx->resObj[i][1]);
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

Sprite *BattleFactorySpriteGfx_CreateSprite(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int drawPriority, int priority, u8 screen) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&header, resourceId, resourceId, resourceId, resourceId, -1, -1, 0, priority, gfx->resMan[0], gfx->resMan[1], gfx->resMan[2], gfx->resMan[3], NULL, NULL);
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
    template.heapID = HEAP_ID_100;
    if (screen == 1) {
        template.position.y += 2 << 20;
    }
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, animSeq);
    return sprite;
}

void BattleFactorySpriteGfx_Free(BattleFactorySpriteGfx *gfx) {
    u8 i;

    for (i = 0; i < 6; i++) {
        SpriteTransfer_DeleteCharTransferTask(gfx->resObj[i][0]);
        SpriteTransfer_DeletePlttTransferTask(gfx->resObj[i][1]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resMan[i]);
    }
    SpriteList_Delete(gfx->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

void BattleFactoryApp_InitObjTransfers(void) {
    ObjCharTransferTemplate template = ov81_02243598;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K);
    ObjPlttTransfer_Init(8, HEAP_ID_100);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void BattleFactoryApp_Setup3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0, 0, 0x7FFF, 0x3F, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}

void BattleFactoryApp_Draw3D(PokepicManager *pokepicManager) {
    Thunk_G3X_Reset();
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_Identity();
    NNS_G3dGlbFlushP();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(pokepicManager);
    G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

Pokepic *BattleFactory_CreatePokepic(PokepicManager *pokepicManager, int polygonId, Pokemon *mon, int x, int y, int z) {
    PokepicTemplate template;

    GetPokemonSpriteCharAndPlttNarcIds(&template, mon, 2);
    return PokepicManager_CreatePokepic(pokepicManager, &template, x, y, z, polygonId, NULL, NULL);
}

void BattleFactory_SetPokepicHFlip(Pokepic *pokepic, int hFlip) {
    Pokepic_SetAttr(pokepic, POKEPIC_HFLIP, hFlip);
}

#pragma force_active on
BattleFactorySprite *BattleFactorySprite_Create(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int priority, int drawPriority, int x, int y) {
    BattleFactorySprite *sprite = Heap_Alloc(HEAP_ID_100, sizeof(BattleFactorySprite));

    ((u8 *)sprite)[0] = 0;
    ((u8 *)sprite)[1] = 0;
    ((u8 *)sprite)[2] = 0;
    ((u8 *)sprite)[3] = 0;
    ((u8 *)sprite)[4] = 0;
    ((u8 *)sprite)[5] = 0;
    ((u8 *)sprite)[6] = 0;
    ((u8 *)sprite)[7] = 0;
    ((u8 *)sprite)[8] = 0;
    ((u8 *)sprite)[9] = 0;
    ((u8 *)sprite)[10] = 0;
    ((u8 *)sprite)[11] = 0;
    sprite->x = x;
    sprite->y = y;
    sprite->sprite = BattleFactorySpriteGfx_CreateSprite(gfx, resourceId, animSeq, drawPriority, priority, 0);
    BattleFactorySprite_SetPosition(sprite, x, y);
    return sprite;
}

BattleFactorySprite *BattleFactorySprite_CreateSub(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int priority, int drawPriority, int x, int y) {
    BattleFactorySprite *sprite = Heap_Alloc(HEAP_ID_100, sizeof(BattleFactorySprite));

    ((u8 *)sprite)[0] = 0;
    ((u8 *)sprite)[1] = 0;
    ((u8 *)sprite)[2] = 0;
    ((u8 *)sprite)[3] = 0;
    ((u8 *)sprite)[4] = 0;
    ((u8 *)sprite)[5] = 0;
    ((u8 *)sprite)[6] = 0;
    ((u8 *)sprite)[7] = 0;
    ((u8 *)sprite)[8] = 0;
    ((u8 *)sprite)[9] = 0;
    ((u8 *)sprite)[10] = 0;
    ((u8 *)sprite)[11] = 0;
    sprite->x = x;
    sprite->y = y;
    sprite->sprite = BattleFactorySpriteGfx_CreateSprite(gfx, resourceId, animSeq, drawPriority, priority, 1);
    BattleFactorySprite_SetPositionSub(sprite, x, y);
    return sprite;
}
#pragma force_active reset

BattleFactorySprite *BattleFactorySprite_Delete(BattleFactorySprite *sprite) {
    Sprite_Delete(sprite->sprite);
    Heap_Free(sprite);
    return NULL;
}

void BattleFactorySprite_SetVisible(BattleFactorySprite *sprite, int visible) {
    Sprite_SetDrawFlag(sprite->sprite, visible);
}

void BattleFactorySprite_SetPosition(BattleFactorySprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = x * FX32_ONE;
    pos.y = y * FX32_ONE;
    Sprite_SetMatrix(sprite->sprite, &pos);
}

void BattleFactorySprite_SetPositionSub(BattleFactorySprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = x * FX32_ONE;
    pos.y = y * FX32_ONE + (2 << 20);
    Sprite_SetMatrix(sprite->sprite, &pos);
}

void BattleFactorySprite_SetMatrix(BattleFactorySprite *sprite, VecFx32 *pos) {
    Sprite_SetMatrix(sprite->sprite, pos);
}

BOOL BattleFactorySprite_IsAnimated(BattleFactorySprite *sprite) {
    return Sprite_IsAnimated(sprite->sprite);
}

void BattleFactorySprite_SetAnim(BattleFactorySprite *sprite, int animSeq) {
    Sprite_SetAnimationFrame(sprite->sprite, 0);
    Sprite_SetAnimCtrlSeq(sprite->sprite, animSeq);
}

void BattleFactorySprite_SetPaletteOverride(BattleFactorySprite *sprite, int palette) {
    Sprite_SetPaletteOverride(sprite->sprite, palette);
}

void BattleFactorySprite_SetPalette(BattleFactorySprite *sprite, int palette) {
    Sprite_SetPalIndexRespectVramOffset(sprite->sprite, palette);
}

#pragma force_active on
void BattleFactorySprite_LoadCharData(BattleFactorySprite *sprite, void *data, u32 size, int screen) {
    u32 addr = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite->sprite), (NNS_G2D_VRAM_TYPE)screen);

    DC_FlushRange(data, size);
    if (screen == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJ(data, addr, size);
    } else {
        GXS_LoadOBJ(data, addr, size);
    }
}

BattleFactoryBall *BattleFactoryBall_Create(BattleFactorySpriteGfx *gfx, u32 x, u32 y, enum HeapID heapID) {
    BattleFactoryBall *ball = Heap_Alloc(heapID, sizeof(BattleFactoryBall));

    memset(ball, 0, sizeof(BattleFactoryBall));
    ball->selected = FALSE;
    ball->x = x;
    ball->y = y;
    ball->sprite = BattleFactorySpriteGfx_CreateSprite(gfx, 0, 0, 10, 0, 1);
    BattleFactoryBall_SetPosition(ball, x, y);
    Sprite_SetPaletteOverride(ball->sprite, 1);
    return ball;
}
#pragma force_active reset

BattleFactoryBall *BattleFactoryBall_Delete(BattleFactoryBall *ball) {
    Sprite_Delete(ball->sprite);
    Heap_Free(ball);
    return NULL;
}

void BattleFactoryBall_SetVisible(BattleFactoryBall *ball, int visible) {
    Sprite_SetDrawFlag(ball->sprite, visible);
}

#pragma force_active on
void BattleFactoryBall_Move(VecFx32 *out, BattleFactoryBall *ball, int dx, int dy) {
    VecFx32 pos = *Sprite_GetMatrixPtr(ball->sprite);

    pos.x += dx * FX32_ONE;
    pos.y += dy * FX32_ONE;
    Sprite_SetMatrix(ball->sprite, &pos);
    *out = *Sprite_GetMatrixPtr(ball->sprite);
}
#pragma force_active reset

void BattleFactoryBall_SetPosition(BattleFactoryBall *ball, int x, int y) {
    VecFx32 pos;

    pos.x = x * FX32_ONE;
    pos.y = y * FX32_ONE + (2 << 20);
    Sprite_SetMatrix(ball->sprite, &pos);
}

VecFx32 *BattleFactoryBall_GetPosition(BattleFactoryBall *ball) {
    return (VecFx32 *)Sprite_GetMatrixPtr(ball->sprite);
}

void BattleFactoryBall_SetSelected(BattleFactoryBall *ball, u16 selected) {
    ball->selected = selected;
}

u8 BattleFactoryBall_IsSelected(BattleFactoryBall *ball) {
    return ball->selected;
}

void BattleFactoryBall_Select(BattleFactoryBall *ball) {
    BattleFactoryBall_SetSelected(ball, TRUE);
}

void BattleFactoryBall_Deselect(BattleFactoryBall *ball) {
    BattleFactoryBall_SetSelected(ball, FALSE);
}

#pragma force_active on
void BattleFactoryBall_MoveOffscreen(BattleFactoryBall *ball) {
    VecFx32 pos;
    VecFx32 *cur = BattleFactoryBall_GetPosition(ball);

    pos.x = (cur->x / FX32_ONE + 248) * FX32_ONE;
    pos.y = cur->y;
    Sprite_SetMatrix(ball->sprite, &pos);
}
#pragma force_active reset

int BattleFactoryBall_GetX(BattleFactoryBall *ball) {
    return ball->x;
}

int BattleFactoryBall_GetY(BattleFactoryBall *ball) {
    return ball->y;
}

void BattleFactoryBall_SetAnim(BattleFactoryBall *ball, int animSeq) {
    Sprite_SetAnimSpeed(ball->sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(ball->sprite, animSeq);
}

void BattleFactoryBall_SetPaletteOverride(BattleFactoryBall *ball, int palette) {
    Sprite_SetPaletteOverride(ball->sprite, palette);
}

void BattleFactoryBall_SetOamMode(BattleFactoryBall *ball, GXOamMode mode) {
    Sprite_SetOamMode(ball->sprite, mode);
}

BattleFactoryBall *BattleFactoryBall_Free(BattleFactoryBall *ball) {
    Sprite_Delete(ball->sprite);
    Heap_Free(ball);
    return NULL;
}

#pragma force_active on
void BattleFactoryApp_AddWindows(BgConfig *bgConfig, Window *windows) {
    u8 i;

    for (i = 0; i < 18; i++) {
        AddWindow(bgConfig, &windows[i], &ov81_022435A8[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}
#pragma force_active reset

void BattleFactoryApp_RemoveWindows(Window *windows) {
    u16 i;

    for (i = 0; i < 18; i++) {
        RemoveWindow(&windows[i]);
    }
}

#pragma force_active on
void BattleFactoryApp_DrawMessageFrame(Window *window, int frame) {
    LoadUserFrameGfx2(window->bgConfig, (GFBgLayer)GetWindowBgId(window), 994, 11, (u8)frame, HEAP_ID_100);
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, TRUE, 994, 11);
}

void BattleFactoryApp_PrintStringAligned(Window *window, String *string, int x, int y, int fontId, u32 color, int align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0) / 2;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void BattleFactoryApp_PrintMsgAligned(Window *window, MsgData *msgData, int msgId, int x, int y, int fontId, u32 color, int align) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    BattleFactoryApp_PrintStringAligned(window, string, x, y, fontId, color, align);
    String_Delete(string);
}
#pragma force_active reset

void BattleFactoryApp_SetLinkNameWindows(Window *windows) {
    SetWindowX(&windows[0], 4);
    SetWindowX(&windows[1], 18);
}

void BattleFactoryApp_LoadButtonGraphics(BgConfig *bgConfig, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x86, bgConfig, GF_BG_LYR_SUB_1, 0x3D9, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC2, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_9_OFFSET, 0x40, HEAP_ID_100);
}

#pragma force_active on
void BattleFactoryApp_DrawButtons(BgConfig *bgConfig, NARC *narc, int which) {
    NNSG2dScreenData *scrnData;
    void *data;
    u8 top;
    u16 *tiles;
    u8 rows;
    u8 y;
    u8 x;

    data = GfGfxLoader_GetScrnDataFromOpenNarc(narc, which + 0x87, TRUE, &scrnData, HEAP_ID_100);
    if (which == 0) {
        top = 11;
        rows = 7;
    } else {
        top = 14;
        rows = 4;
    }
    tiles = (u16 *)scrnData->rawData;
    for (y = 0; y < rows; y++) {
        for (x = 0; x < 32; x++) {
            FillBgTilemapRect(bgConfig, GF_BG_LYR_SUB_1, tiles[y * 32 + x] + 0x3D9, x, top + y, 1, 1, 9);
        }
    }
    Heap_Free(data);
}
#pragma force_active reset

void BattleFactoryApp_ClearButtons(BgConfig *bgConfig, int which) {
    int top;
    int height;

    if (which == 0) {
        top = 11;
        height = 7;
    } else {
        top = 14;
        height = 4;
    }
    FillBgTilemapRect(bgConfig, GF_BG_LYR_SUB_1, 0, 0, (u8)top, 32, (u8)height, 9);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_1);
}

void BattleFactory_FreeGrid(GridInputHandler *grid) {
    GridInputHandler_Free(grid);
}

void BattleFactory_GetGridCursorPos(GridInputHandler *grid, int target, u32 *x, u32 *y) {
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(grid, target);
    *x = box->left;
    *y = box->top;
}

void BattleFactory_GridNop(void) {
}

#pragma force_active on
GridInputHandler *BattleFactoryApp_CreateRentalGrid(BattleFactoryApp *app, int cursor) {
    return GridInputHandler_Create(ov81_022436F0, ov81_02243808, &ov81_02243658, app, TRUE, (u8)cursor, HEAP_ID_100);
}

void BattleFactory_SetRentalGridMenu(GridInputHandler *grid, int menuOpen) {
    if (menuOpen == TRUE) {
        GridInputHandler_SetAllEnabled(grid);
    } else {
        GridInputHandler_ClearEnabledFlag(grid, 6);
        GridInputHandler_ClearEnabledFlag(grid, 7);
        GridInputHandler_ClearEnabledFlag(grid, 8);
    }
}
#pragma force_active reset

void BattleFactoryApp_OnRentalCursorMove(BattleFactoryApp *app, int target) {
    if (target >= 6) {
        BattleFactoryApp_MoveButtonCursor(app, target);
    } else {
        BattleFactoryApp_MoveRentalCursor(app, target);
    }
}

#pragma force_active on
GridInputHandler *BattleFactoryApp_CreateSwapOutGrid(BattleFactoryApp *app, int cursor) {
    return GridInputHandler_Create(ov81_02243668, ov81_02243750, &ov81_02243648, app, TRUE, (u8)cursor, HEAP_ID_100);
}

GridInputHandler *BattleFactoryApp_CreateLinkSwapOutGrid(BattleFactoryApp *app, int cursor) {
    GridInputHandler *grid = GridInputHandler_Create(ov81_02243688, ov81_02243718, &ov81_02243648, app, TRUE, (u8)cursor, HEAP_ID_100);
    GridInputHandler_ClearEnabledFlag(grid, 2);
    return grid;
}

void BattleFactory_SetSwapOutGridMenu(GridInputHandler *grid, int menuOpen, int frontierType) {
    if (menuOpen == TRUE) {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 3);
    } else {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 4);
        GridInputHandler_ClearEnabledFlag(grid, 5);
        GridInputHandler_ClearEnabledFlag(grid, 6);
    }
    if (BattleFactory_IsLinkMode(frontierType) == TRUE) {
        GridInputHandler_ClearEnabledFlag(grid, 2);
    }
}
#pragma force_active reset

void BattleFactoryApp_OnSwapOutCursorMove(BattleFactoryApp *app, int target) {
    if (target >= 4) {
        BattleFactoryApp_MoveButtonCursor(app, target);
    } else {
        BattleFactoryApp_MoveSwapOutCursor(app, target);
    }
}

GridInputHandler *BattleFactoryApp_RecreateSwapOutGrid(BattleFactoryApp *app, GridInputHandler *grid) {
    BattleFactory_FreeGrid(grid);
    if (BattleFactoryApp_IsLinkMode(app) == TRUE) {
        return BattleFactoryApp_CreateLinkSwapOutGrid(app, 0);
    } else {
        return BattleFactoryApp_CreateSwapOutGrid(app, 0);
    }
}

GridInputHandler *BattleFactoryApp_CreateSwapInGrid(BattleFactoryApp *app, GridInputHandler *grid) {
    BattleFactory_FreeGrid(grid);
    if (BattleFactoryApp_IsLinkMode(app) == TRUE) {
        return GridInputHandler_Create(ov81_022436CC, ov81_02243788, &ov81_02243638, app, TRUE, 0, HEAP_ID_100);
    }
    return GridInputHandler_Create(ov81_022436A8, ov81_022437C8, &ov81_02243638, app, TRUE, 0, HEAP_ID_100);
}

void BattleFactory_SetSwapInGridMenu(GridInputHandler *grid, int menuOpen, int frontierType) {
    if (menuOpen == TRUE) {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 4);
        GridInputHandler_ClearEnabledFlag(grid, 5);
    } else {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 6);
        GridInputHandler_ClearEnabledFlag(grid, 7);
    }
    if (BattleFactory_IsLinkMode(frontierType) == FALSE) {
        GridInputHandler_ClearEnabledFlag(grid, 3);
    }
}

void BattleFactoryApp_OnSwapInCursorMove(BattleFactoryApp *app, int target) {
    if (target >= 6) {
        BattleFactoryApp_MoveButtonCursor(app, target);
    } else {
        BattleFactoryApp_MoveSwapInCursor(app, target);
    }
}
