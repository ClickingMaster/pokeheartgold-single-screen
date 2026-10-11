#ifndef POKEHEARTGOLD_GTS_INTERNAL_H
#define POKEHEARTGOLD_GTS_INTERNAL_H

#include "nnsys/fnd/expheap.h"

#include "bg_window.h"
#include "gts.h"
#include "gts_server.h"
#include "launch_application_internal.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokemon.h"
#include "render_window.h"
#include "sprite.h"
#include "touchscreen_list_menu.h"
#include "trade_anim.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_020755E8.h"
#include "yes_no_prompt.h"

// A deposited Pokemon as the server stores it (the payload of post.asp, the answers of get.asp/result.asp,
// search.asp returns a list of them).
#define GTS_RECORD_SIZE 0x124

// GtsApp.offeredBox of a Pokemon offered from the party.
#define GTS_BOX_PARTY 18

typedef struct GtsRecord {
    Pokemon mon;                             // 0x000
    u8 unk_0EC[0x20];                        // 0x0EC
    u16 trainerName[PLAYER_NAME_LENGTH + 1]; // 0x10C
    u8 unk_11C[6];                           // 0x11C
    u8 version;                              // 0x122
    u8 language;                             // 0x123
} GtsRecord;                                 // size: GTS_RECORD_SIZE

typedef enum GtsServerState {
    GTS_SERVER_STATE_NONE,
    GTS_SERVER_STATE_IDLE,
    GTS_SERVER_STATE_POST,
    GTS_SERVER_STATE_POST_FINISH = 4,
    GTS_SERVER_STATE_GET = 6,
    GTS_SERVER_STATE_RESULT = 8,
    GTS_SERVER_STATE_DELETE = 10,
    GTS_SERVER_STATE_RETURN = 12,
    GTS_SERVER_STATE_SEARCH = 14,
    GTS_SERVER_STATE_EXCHANGE = 16,
    GTS_SERVER_STATE_EXCHANGE_FINISH = 18,
    GTS_SERVER_STATE_INFO = 20,
    GTS_SERVER_STATE_SET_PROFILE = 22,
    GTS_SERVER_STATE_DONE = 24,
} GtsServerState;

// The server client keeps its state in one .bss block (defined in the overlay's data asm, gGtsServer).
// The code reaches parts of the block from nearer base addresses, so the asm labels those parts too:
// gGtsServerFriendKey (+0x0C), gGtsServerRequest (+0x14), ov70_02246900 (+0x100), ov70_02246920 (+0x120),
// gGtsServerShortResponse (+0x13C). The views below follow that split.
// What search.asp is sent: the species and further criteria, then the number of records wanted.
typedef struct GtsSearchCriteria {
    u8 bytes[6];
} GtsSearchCriteria;

typedef struct GtsSearchCriteriaEx {
    u8 bytes[6];
    s8 count; // 0x6
    u8 unk_7;
} GtsSearchCriteriaEx;

typedef struct GtsServer {
    int state;        // 0x00, GtsServerState
    int result;       // 0x04
    int pid;          // 0x08
    u32 friendKey[2]; // 0x0C

    union {
        u8 bytes[GTS_RECORD_SIZE + 4];
        GtsSearchCriteriaEx search;
    } request; // 0x14, the payload of the request in flight
} GtsServer;

typedef struct GtsFriendKey {
    u32 lo;
    u32 hi;
} GtsFriendKey;

typedef struct GtsServerAt100 {
    u8 filler_00[0x38];
    void *exchangeArg; // 0x38, sent after the record by exchange.asp
    u8 filler_3C[4];
    u8 *response; // 0x40, buffer of the request in flight
} GtsServerAt100;

typedef struct GtsServerAt120 {
    u8 filler_00[0x1C];
    u8 shortResponse[4]; // 0x1C, gGtsServerShortResponse; [0] is the server's answer code
} GtsServerAt120;

extern volatile GtsServer gGtsServer; // polled every frame (GtsServer_Update)
extern GtsFriendKey gGtsServerFriendKey;
extern u8 gGtsServerRequest[];
extern GtsServerAt100 ov70_02246900;
extern volatile GtsServerAt120 ov70_02246920; // written by the response callback
extern u8 gGtsServerShortResponse[];

typedef struct GtsApp GtsApp;

typedef struct GtsGlobals {
    NNSFndHeapHandle heap; // 0x0, allocations of the network library
    GtsApp *app;           // 0x4
} GtsGlobals;

extern GtsGlobals gGtsGlobals;

typedef void (*GtsVBlankFunc)(GtsApp *app);

struct GtsApp {
    GtsArgs *args;                           // 0x0000
    BgConfig *bgConfig;                      // 0x0004
    u8 filler_0008[0xC];                     // 0x0008
    int screen;                              // 0x0014, index into sGtsScreens
    int nextScreen;                          // 0x0018
    u8 filler_001C[4];                       // 0x001C
    int prevScreen;                          // 0x0020
    int nextScreenArg;                       // 0x0024
    u8 filler_0028[4];                       // 0x0028
    int state;                               // 0x002C, of the current screen
    int nextState;                           // 0x0030
    u8 filler_0034[6];                       // 0x0034
    u16 timer;                               // 0x003A
    u8 filler_003C[0x10];                    // 0x003C
    void *heapMemory;                        // 0x004C
    NNSFndHeapHandle heap;                   // 0x0050, in heapMemory
    u8 filler_0054[0x64];                    // 0x0054
    OverlayManager *tradeSequence;           // 0x00B8
    u8 filler_00BC[0x3C];                    // 0x00BC
    TRADE_ANIM_WORK tradeAnim;               // 0x00F8, args of the trade sequence (OVY_71)
    EvolutionTaskData *evolutionTask;        // 0x0110
    u32 reloadGraphics;                      // 0x0114, the screen asks Gts_Main to rebuild the graphics
    u8 filler_0118[4];                       // 0x0118
    u16 unk_011C;                            // 0x011C
    u8 filler_011E[2];                       // 0x011E
    u16 unk_0120;                            // 0x0120
    u8 filler_0122[6];                       // 0x0122
    u32 unk_0128;                            // 0x0128
    int selectedResult;                      // 0x012C, in searchResults
    PlayerProfile *partnerProfile;           // 0x0130
    u32 offeredBox;                          // 0x0134, GTS_BOX_PARTY for the party
    u32 offeredSlot;                         // 0x0138
    GtsRecord record;                        // 0x013C
    GtsRecord searchResults[8];              // 0x0260
    u8 filler_0B80[0xA];                     // 0x0B80
    u16 unk_0B8A;                            // 0x0B8A
    u8 unk_0B8C;                             // 0x0B8C
    u8 unk_0B8D;                             // 0x0B8D
    u8 unk_0B8E;                             // 0x0B8E
    u8 filler_0B8F;                          // 0x0B8F
    u16 unk_0B90;                            // 0x0B90
    u8 filler_0B92[0xA];                     // 0x0B92
    MessageFormat *msgFormat;                // 0x0B9C
    MsgData *gtsMsgData;                     // 0x0BA0, message bank 775
    MsgData *speciesNames;                   // 0x0BA4
    MsgData *wifiMsgData;                    // 0x0BA8, message bank 778
    MsgData *wifiErrorMsgData;               // 0x0BAC, message bank 800
    MsgData *countryNames;                   // 0x0BB0
    u8 filler_0BB4[0x40];                    // 0x0BB4
    SpriteList *spriteList;                  // 0x0BF4
    G2dRenderer renderer;                    // 0x0BF8
    GF_2DGfxResMan *resourceManagers[4];     // 0x0D20, char/pltt/cell/anim
    SpriteResource *resources[8];            // 0x0D30, [main/sub screen][char/pltt/cell/anim]
    u8 filler_0D50[0x10];                    // 0x0D50
    SpriteResourcesHeader headers[2];        // 0x0D60
    u8 filler_0DA8[0x170];                   // 0x0DA8
    Window waitingIconWindow;                // 0x0F18
    u8 filler_0F28[0x284];                   // 0x0F28
    ListMenuItem *listItems;                 // 0x11AC
    u8 filler_11B0[0xC];                     // 0x11B0
    WaitingIcon *waitingIcon;                // 0x11BC
    u8 filler_11C0[8];                       // 0x11C0
    YesNoPrompt *yesNoPrompt;                // 0x11C8
    TouchscreenListMenuSpawner *menuSpawner; // 0x11CC
    TouchscreenListMenu *menu;               // 0x11D0
    u8 filler_11D4[8];                       // 0x11D4
    u16 unk_11DC;                            // 0x11DC
    u16 unk_11DE;                            // 0x11DE
    u8 filler_11E0[0x10];                    // 0x11E0
    Pokemon *depositedMon;                   // 0x11F0, copy of the deposited Pokemon (GtsTrade)
    u8 filler_11F4[4];                       // 0x11F4
    u16 boxMonCount;                         // 0x11F8, Pokemon and Eggs in the PC, counted one box per frame
    u16 countingBox;                         // 0x11FA, 1-based box being counted, 0 when done
    u8 filler_11FC[8];                       // 0x11FC
    GtsVBlankFunc vblankTask;                // 0x1204, run once
    GtsVBlankFunc vblankFunc;                // 0x1208, run every frame
    u8 filler_120C[0xC0];                    // 0x120C
    u32 unk_12CC;                            // 0x12CC
    u8 filler_12D0[0x338];                   // 0x12D0
}; // size: 0x1608

// Maps a failed request's error to its result (gts.c).
int GtsServer_GetErrorResult(int error);

// Helpers shared by the GTS screens (gts.c).
void Gts_SetupWindow0(void);
void Gts_InitObjTransfer(void);
void Gts_InitSpriteResources(GtsApp *app);
void Gts_InitSpriteTemplate(SpriteTemplate *template, GtsApp *app, const SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE screen);
void Gts_InitSpriteResourcesHeaders(GtsApp *app);
YesNoPrompt *Gts_CreateYesNoPrompt(BgConfig *bgConfig, u8 y, int tileStart, u32 palette, u8 dimScreen);
YesNoPrompt *Gts_CreateYesNoPromptOnBg(BgConfig *bgConfig, u8 y, int tileStart, u32 palette, int bgId, u8 dimScreen);
YesNoResponse Gts_HandleYesNoPrompt(GtsApp *app);
TouchscreenListMenu *Gts_CreateListMenu(GtsApp *app, u8 numWindows, int y);
void Gts_DestroyListMenu(GtsApp *app);
void Gts_SetState(GtsApp *app, int state, int nextState);
void Gts_SetSpritePositionRounded(Sprite *sprite, int x, int y);
int ov70_02238E44(void);
void Gts_SetNextScreen(GtsApp *app, int screen, int arg);
void Gts_SwitchScreen(GtsApp *app);
int Gts_GetTextFrameDelay(GtsApp *app);
void Gts_InitGraphics(GtsApp *app);
void Gts_FreeGraphics(GtsApp *app);
void Gts_StartCountingBoxMons(GtsApp *app);
void Gts_CreateWaitingIcon(GtsApp *app);
void Gts_DeleteWaitingIcon(GtsApp *app);
void Gts_SetSpritePosition(Sprite *sprite, int x, int y);
void Gts_DimScreen(BOOL mainScreen);
void Gts_ClearDim(void);

#endif // POKEHEARTGOLD_GTS_INTERNAL_H
