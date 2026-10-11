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
#include "unk_02088288.h"
#include "yes_no_prompt.h"

// A deposited Pokemon as the server stores it (the payload of post.asp, the answers of get.asp/result.asp,
// search.asp returns a list of them).
#define GTS_RECORD_SIZE 0x124

// GtsApp.offeredBox of a Pokemon offered from the party.
#define GTS_BOX_PARTY 18

// What a record says about its Pokemon. gender: 1 male, 2 female, 3 genderless.
typedef struct GtsMonSummary {
    s16 species;
    s8 gender;
    s8 level;
} GtsMonSummary;

// The Pokemon the depositor wants in exchange.
typedef struct GtsWantedMon {
    s16 species;
    s8 gender;
    s8 minLevel;
    s8 maxLevel;
    s8 unk_5;
} GtsWantedMon;

typedef struct GtsRecord {
    Pokemon mon;                             // 0x000
    GtsMonSummary summary;                   // 0x0EC
    GtsWantedMon wanted;                     // 0x0F0
    u8 trainerGender;                        // 0x0F6
    u8 unk_0F7[0x15];                        // 0x0F7
    u16 trainerName[PLAYER_NAME_LENGTH + 1]; // 0x10C
    u8 unk_11C[2];                           // 0x11C
    u8 country;                              // 0x11E
    u8 city;                                 // 0x11F
    u8 trainerClass;                         // 0x120
    u8 unk_121;                              // 0x121
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

// The cursor of SEEK POKEMON (column 0: rows 0..3, column 1: rows 0..2).
typedef struct GtsSearchCursor {
    u8 unk_00[4];
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    u8 unk_0C[8];
    void *unk_14;
    u8 unk_18[8];
    u32 genderRatio; // 0x20, of the wanted species
    int column;      // 0x24
    int leftRow;     // 0x28
    int rightRow;    // 0x2C
} GtsSearchCursor;

// What the selector of species, gender, level and country is created with.
typedef struct GtsSelectorArgs {
    BgConfig *bgConfig;
    Window *windows;
    Window *messageWindow;
    Sprite *sprites[4];
    MsgData *gtsMsgData;
    MsgData *speciesNames;
    MsgData *countryNames;
    Pokedex *pokedex;
    void *unk_2C;
} GtsSelectorArgs;

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
    int unk_001C;                            // 0x001C
    int prevScreen;                          // 0x0020
    int nextScreenArg;                       // 0x0024
    u8 filler_0028[4];                       // 0x0028
    int state;                               // 0x002C, of the current screen
    int nextState;                           // 0x0030
    u16 mainMenuShown;                       // 0x0034, the main menu was entered before
    u16 hasDepositedMon;                     // 0x0036, the player has a Pokemon on the GTS
    u8 filler_0038[2];                       // 0x0038
    u16 timer;                               // 0x003A, frames until the GTS may be checked again
    u8 filler_003C[0x10];                    // 0x003C
    void *heapMemory;                        // 0x004C
    NNSFndHeapHandle heap;                   // 0x0050, in heapMemory
    u8 filler_0054[0x64];                    // 0x0054
    OverlayManager *tradeSequence;           // 0x00B8
    PokemonSummaryArgs summaryArgs;          // 0x00BC, of screen 8
    TRADE_ANIM_WORK tradeAnim;               // 0x00F8, args of the trade sequence (OVY_71)
    EvolutionTaskData *evolutionTask;        // 0x0110
    u32 reloadGraphics;                      // 0x0114, the screen asks Gts_Main to rebuild the graphics
    u16 unk_0118;                            // 0x0118
    u8 filler_011A[2];                       // 0x011A
    u16 menuCursor;                          // 0x011C
    u8 filler_011E[2];                       // 0x011E
    u16 selectedBox;                         // 0x0120, GTS_BOX_PARTY for the party
    u16 selectedSlot;                        // 0x0122
    u8 filler_0124[4];                       // 0x0124
    u32 searchResultCount;                   // 0x0128, in searchResults
    int selectedResult;                      // 0x012C, in searchResults
    PlayerProfile *partnerProfile;           // 0x0130
    u32 offeredBox;                          // 0x0134, GTS_BOX_PARTY for the party
    u32 offeredSlot;                         // 0x0138
    GtsRecord record;                        // 0x013C
    GtsRecord searchResults[8];              // 0x0260
    u8 filler_0B80[0xA];                     // 0x0B80
    GtsWantedMon searchCriteria;             // 0x0B8A, of SEEK POKEMON
    GtsWantedMon lastSearch;                 // 0x0B90
    u8 filler_0B96[2];                       // 0x0B96
    u32 lastSearchCountry;                   // 0x0B98
    MessageFormat *msgFormat;                // 0x0B9C
    MsgData *gtsMsgData;                     // 0x0BA0, message bank 775
    MsgData *speciesNames;                   // 0x0BA4
    MsgData *wifiMsgData;                    // 0x0BA8, message bank 778
    MsgData *wifiErrorMsgData;               // 0x0BAC, message bank 800
    MsgData *countryNames;                   // 0x0BB0
    u8 filler_0BB4[8];                       // 0x0BB4
    String *msgBuffer;                       // 0x0BBC
    String *titleString;                     // 0x0BC0
    String *unk_0BC4[10];                    // 0x0BC4
    u8 filler_0BEC[4];                       // 0x0BEC
    int textPrinterId;                       // 0x0BF0
    SpriteList *spriteList;                  // 0x0BF4
    G2dRenderer renderer;                    // 0x0BF8
    GF_2DGfxResMan *resourceManagers[4];     // 0x0D20, char/pltt/cell/anim
    SpriteResource *resources[8];            // 0x0D30, [main/sub screen][char/pltt/cell/anim]
    u8 filler_0D50[0x10];                    // 0x0D50
    SpriteResourcesHeader headers[2];        // 0x0D60
    u8 filler_0DA8[0x24];                    // 0x0DA8
    Sprite *cursorSprite;                    // 0x0DCC
    Sprite *unk_0DD0;                        // 0x0DD0
    u8 filler_0DD4[0x10C];                   // 0x0DD4
    Sprite *monSprite;                       // 0x0EE0
    Sprite *avatarSprites[8];                // 0x0EE4, [0] the player, [1 + i] the trainer of searchResults[i]
    Sprite *unk_0F04;                        // 0x0F04
    Sprite *unk_0F08;                        // 0x0F08
    Sprite *avatarMarkerSprite;              // 0x0F0C, over the selected trainer
    Sprite *panelSprite;                     // 0x0F10, of the sub screen panel
    int scrollY;                             // 0x0F14, of a search result's screens
    Window waitingIconWindow;                // 0x0F18, also the message window
    u8 filler_0F28[0x10];                    // 0x0F28
    Window titleWindow;                      // 0x0F38
    u8 filler_0F48[0x10];                    // 0x0F48
    Window windows[16];                      // 0x0F58, each screen's own windows
    Window infoWindows[16];                  // 0x1058, a Pokemon's summary, trainer and wanted Pokemon
    Window promptWindow;                     // 0x1158, the message window of a Yes/No prompt
    Window selectorWindow;                   // 0x1168
    Window wideWindows[2];                   // 0x1178
    Window panelWindow;                      // 0x1198, the caption of the sub screen panel
    void *selector;                          // 0x11A8, the species/gender/level/country selector
    ListMenuItem *listItems;                 // 0x11AC
    u8 filler_11B0[0xC];                     // 0x11B0
    WaitingIcon *waitingIcon;                // 0x11BC
    int waitFrames;                          // 0x11C0
    GtsSearchCursor *searchCursor;           // 0x11C4
    YesNoPrompt *yesNoPrompt;                // 0x11C8
    TouchscreenListMenuSpawner *menuSpawner; // 0x11CC
    TouchscreenListMenu *menu;               // 0x11D0
    u8 filler_11D4[4];                       // 0x11D4
    SysTask *playerAnimTask;                 // 0x11D8
    u16 playerAnimDone;                      // 0x11DC, set by the player's walk in/out task (Gts_StartPlayer*Anim)
    u16 hasSearchResults;                    // 0x11DE
    void *avatarCharAlloc;                   // 0x11E0
    NNSG2dCharacterData *avatarCharData;     // 0x11E4
    void *avatarPlttAlloc;                   // 0x11E8
    NNSG2dPaletteData *avatarPlttData;       // 0x11EC
    Pokemon *depositedMon;                   // 0x11F0, copy of the deposited Pokemon (GtsTrade)
    u8 filler_11F4[4];                       // 0x11F4
    u16 boxMonCount;                         // 0x11F8, Pokemon and Eggs in the PC, counted one box per frame
    u16 countingBox;                         // 0x11FA, 1-based box being counted, 0 when done
    int unk_11FC;                            // 0x11FC
    u8 filler_1200[4];                       // 0x1200
    GtsVBlankFunc vblankTask;                // 0x1204, run once
    GtsVBlankFunc vblankFunc;                // 0x1208, run every frame
    s16 avatarPositions[8][2];               // 0x120C, {x, y} of avatarSprites
    u8 filler_122C[8];                       // 0x122C
    u32 unk_1234[4];                         // 0x1234
    u8 filler_1244[0x88];                    // 0x1244
    u32 searchCountry;                       // 0x12CC, of SEEK POKEMON, 0 for any
    u8 filler_12D0[0x334];                   // 0x12D0
    u32 requestFrames;                       // 0x1604, frames the request in flight took
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
int Gts_GetWifiIconLevel(void);
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

// The GTS main menu (gts_main_menu.c).
void Gts_InitSubScreenBgs(BgConfig *bgConfig, int bg1Y, BOOL keepBg2Tilemap);
void Gts_FreeSubScreenBgs(BgConfig *bgConfig);

// The sub screen panel and the Pokemon summary windows (gts_summary.c).
void Gts_LoadSubScreenBg(GtsApp *app);
void Gts_ShowSubScreenPanel(GtsApp *app);
void Gts_CreatePanelWindow(GtsApp *app, int caption);
void Gts_DrawMonSummary(MsgData *msgData, MsgData *speciesNames, MessageFormat *msgFormat, Window *windows, BoxPokemon *boxMon, GtsMonSummary *summary);
void Gts_DrawTrainerInfo(MsgData *msgData, Window *trainerWindows, const u16 *trainerName, Pokemon *mon, Window *otWindows);
void Gts_LoadMonPicture(Pokemon *mon);

// SEEK POKEMON (gts_search.c; the last of them link with screen 5).
void GtsSearch_PrintMessage(GtsApp *app, int msgId, int textSpeed, int unused3, u32 unused4);
void GtsSearch_DrawFieldLabels(Window *fieldWindows, Window *lineWindow, MsgData *msgData);
void GtsSearch_DrawResultsButton(BgConfig *bgConfig, Window *window, MsgData *msgData, BOOL enabled);
BOOL GtsSearch_IsSameSearch(GtsWantedMon *criteria, GtsWantedMon *lastSearch, u32 country, u32 lastCountry);
void GtsSearch_ScrollBgs(GtsApp *app);
void GtsSearch_SetBlendBrightness(int brightness);

// The avatars on the sub screen (gts_avatars.c).
void Gts_CreateAvatars(GtsApp *app, u32 gender);
void Gts_StartPlayerEnterAnim(GtsApp *app, int gender);
void Gts_StartPlayerExitAnim(GtsApp *app, int gender);
void Gts_SetAvatarPosition(Sprite *sprite, int x, int y);
int Gts_GetTouchedAvatar(int count);
void Gts_ShowResultAvatars(GtsApp *app, int count, int animate);
void Gts_StopAvatarAnims(GtsApp *app);
void Gts_FreeAvatarGraphics(GtsApp *app);
void Gts_SetAvatarMarker(GtsApp *app, u32 index, u32 scrollY);
void Gts_ResetAvatarPositions(GtsApp *app);
void Gts_LowerAvatars(GtsApp *app);

// Defined in pieces not merged yet.
void *ov70_0223E49C(Party *party, PCStorage *pcStorage, int box, int slot);
BOOL ov70_0223EDE4(GtsWantedMon *wanted, u32 genderRatio);
void ov70_0223F1D8(Window *window, MsgData *speciesNames, int species, int x, int y, u32 color);
void ov70_0223F244(Window *window, MsgData *countryNames, MsgData *msgData, u32 country, int x, int y, u32 color);
void ov70_0223F2BC(Window *window, MsgData *msgData, int gender, BOOL showGenderless, int x, int y, u32 color);
void ov70_0223F370(Window *window, MsgData *msgData, int levelRange, int x, int y, u32 color, int kind);
void *ov70_0223F684(enum HeapID heapID);
void ov70_0223F828(GtsWantedMon *wanted, int levelRange, int kind);
void ov70_0223F8A8(GtsApp *app, int index);
void ov70_0223F948(u32 *arg);
void ov70_0223F960(u32 *arg, int arg1, int arg2, int arg3);
void *ov70_02242014(GtsSelectorArgs *args, int arg1, int arg2);
void ov70_022420C4(void *selector, int mode);
void ov70_0224212C(void *selector);
int ov70_02242144(void *selector);
BOOL ov70_0223E76C(Pokemon *mon);
void ov70_0223F470(MsgData *msgData, MsgData *speciesNames, MessageFormat *msgFormat, Window *windows, int species, int gender, int levelRange);
int ov70_0223F864(int minLevel, int maxLevel, int kind);
void ov70_0223F20C(Window *window, MsgData *msgData, int msgId, int y, u32 color);
void ov70_0223F324(Window *window, MsgData *msgData, int gender, BOOL showGenderless, int x, int y, u32 color);
void ov70_0223F38C(Window *window, MsgData *msgData, int levelRange, int align, int x, int y, u32 color, int kind);
void ov70_02245084(Window *window, String *string, int x, int y, int align, u32 color);
void ov70_022450B8(Window *window, String *string, int x, int y, int align, u32 color);
void ov70_022450EC(Window *window, MsgData *msgData, int caption);
int ov70_02245124(GtsApp *app);

#endif // POKEHEARTGOLD_GTS_INTERNAL_H
