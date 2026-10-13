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
#include "sprite_resource.h"
#include "save_wii_message.h"
#include "evolution_scene.h"
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
    u8 unk_0F7[0x11];                        // 0x0F7
    u32 exchangeId;                          // 0x108, exchange.asp is sent it after the offered record to pick this one
    u16 trainerName[PLAYER_NAME_LENGTH + 1]; // 0x10C
    u16 trainerId;                           // 0x11C
    u8 country;                              // 0x11E
    u8 city;                                 // 0x11F
    u8 trainerClass;                         // 0x120
    s8 unk_121;                              // 0x121, nonzero once the Pokemon was traded (get.asp)
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
    u16 *alphabeticalSpecies;     // 0x18
    u32 alphabeticalSpeciesCount; // 0x1C
    u32 genderRatio;              // 0x20, of the wanted species
    int column;                   // 0x24
    int leftRow;                  // 0x28
    int rightRow;                 // 0x2C
} GtsSearchCursor;

// The icon of a Pokemon of the shown box, loaded into VRAM in the next VBlank (gts_select_mon.c).
typedef struct GtsMonIcon GtsMonIcon;

typedef struct GtsUnk1234 {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04[10];
    u16 unk_18[10];
} GtsUnk1234;

// The selector of species, gender, level and country (gts_selector.c).
typedef struct GtsSelector GtsSelector;

// What the selector is created with.
typedef struct GtsSelectorArgs {
    BgConfig *bgConfig;
    Window *windows;
    Window *messageWindow;
    Sprite *sprites[4]; // the cursor, the right and left arrows, the screen's own cursor (may be NULL)
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

// The background palette cycle of screen 0, run as a VBlank task (gts_connection.c).
typedef struct GtsPaletteAnim {
    SysTask *task;         // 0x000
    BOOL active;           // 0x004
    u16 basePltt[4 * 16];  // 0x008
    u16 fadePltt[21 * 16]; // 0x088, palette 0 blended in 7 steps towards each of palettes 1, 2 and 3
    s16 frame;             // 0x328, in fadePltt
    s8 reverse;            // 0x32A
    u8 skipFrame;          // 0x32B, the palette changes every other frame
    u32 screens;           // 0x32C, 0: both, 1: main, 2: sub
} GtsPaletteAnim;          // size: 0x330

struct GtsApp {
    GtsArgs *args;                           // 0x0000
    BgConfig *bgConfig;                      // 0x0004
    u8 filler_0008[0xC];                     // 0x0008
    int screen;                              // 0x0014, index into sGtsScreens
    int nextScreen;                          // 0x0018
    int unk_001C;                            // 0x001C
    int prevScreen;                          // 0x0020
    int nextScreenArg;                       // 0x0024
    int afterSaveMsgId;                      // 0x0028, printed once the save of screen 7 is written
    int state;                               // 0x002C, of the current screen
    int nextState;                           // 0x0030
    u16 mainMenuShown;                       // 0x0034, the main menu was entered before
    u16 hasDepositedMon;                     // 0x0036, the player has a Pokemon on the GTS
    u8 filler_0038[2];                       // 0x0038
    u16 timer;                               // 0x003A, frames until the GTS may be checked again
    int serverResult;                        // 0x003C, of the failed request (screen 7 picks the error message from it)
    int connectionResult;                    // 0x0040, of a failed Wi-Fi connection (ov00_021EC11C)
    int connectionErrorCode;                 // 0x0044
    int connectionErrorType;                 // 0x0048
    void *heapMemory;                        // 0x004C
    NNSFndHeapHandle heap;                   // 0x0050, in heapMemory
    u8 wifiConnection[0x64];                 // 0x0054, passed to ov00_021EC3F0 when connecting (screen 0)
    OverlayManager *tradeSequence;           // 0x00B8
    PokemonSummaryArgs summaryArgs;          // 0x00BC, of screen 8
    TRADE_ANIM_WORK tradeAnim;               // 0x00F8, args of the trade sequence (OVY_71)
    EvolutionTaskData *evolutionTask;        // 0x0110
    u32 reloadGraphics;                      // 0x0114, the screen asks Gts_Main to rebuild the graphics
    u16 unk_0118;                            // 0x0118
    u8 filler_011A[2];                       // 0x011A
    u16 menuCursor;                          // 0x011C
    u8 filler_011E[2];                       // 0x011E
    u16 selectedBox;                         // 0x0120, shown by screen 5, GTS_BOX_PARTY for the party
    u16 selectedSlot;                        // 0x0122, screen 5's cursor: slot 0..29, 30 BACK, 31 the box name
    BoxPokemon *selectedMon;                 // 0x0124, the Pokemon to deposit or to trade
    u32 searchResultCount;                   // 0x0128, in searchResults
    int selectedResult;                      // 0x012C, in searchResults
    PlayerProfile *partnerProfile;           // 0x0130
    u32 offeredBox;                          // 0x0134, GTS_BOX_PARTY for the party
    u32 offeredSlot;                         // 0x0138
    GtsRecord record;                        // 0x013C
    GtsRecord searchResults[8];              // 0x0260, [7] also receives the record of an exchange (screen 7)
    GtsMonSummary depositSummary;            // 0x0B80, of the Pokemon to deposit
    GtsWantedMon depositWanted;              // 0x0B84, what the player wants for it
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
    String *boxName;                         // 0x0BB4
    String *cancelText;                      // 0x0BB8
    String *msgBuffer;                       // 0x0BBC
    String *titleString;                     // 0x0BC0
    String *unk_0BC4[10];                    // 0x0BC4
    String *errorBuffer;                     // 0x0BEC, the Wi-Fi error message of screen 0
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
    u8 filler_0DD4[4];                       // 0x0DD4
    Sprite *monIconSprites[30];              // 0x0DD8, of the shown box
    Sprite *heldItemSprites[30];             // 0x0E50
    Sprite *capsuleSprites[6];               // 0x0EC8, the party's Ball Capsules
    Sprite *monSprite;                       // 0x0EE0
    Sprite *avatarSprites[8];                // 0x0EE4, [0] the player, [1 + i] the trainer of searchResults[i]
    Sprite *arrowSprites[2];                 // 0x0F04, screen 5's box arrows; the selector's on screen 4
    Sprite *avatarMarkerSprite;              // 0x0F0C, over the selected trainer
    Sprite *panelSprite;                     // 0x0F10, of the sub screen panel
    int scrollY;                             // 0x0F14, of a search result's screens
    Window waitingIconWindow;                // 0x0F18, also the message window
    u8 filler_0F28[0x10];                    // 0x0F28
    Window titleWindow;                      // 0x0F38
    Window boxNameWindow;                    // 0x0F48, of screen 5; screen 0's Wi-Fi error window
    Window windows[16];                      // 0x0F58, each screen's own windows
    Window infoWindows[16];                  // 0x1058, a Pokemon's summary, trainer and wanted Pokemon
    Window promptWindow;                     // 0x1158, the message window of a Yes/No prompt
    Window selectorWindow;                   // 0x1168
    Window wideWindows[2];                   // 0x1178
    Window panelWindow;                      // 0x1198, the caption of the sub screen panel
    GtsSelector *selector;                   // 0x11A8, the species/gender/level/country selector
    ListMenuItem *listItems;                 // 0x11AC
    struct ListMenu2D *unk_11B0;             // 0x11B0, only read by screen 0 (GtsConnection_HandleDisconnectMenu)
    u8 filler_11B4[8];                       // 0x11B4
    WaitingIcon *waitingIcon;                // 0x11BC
    int waitFrames;                          // 0x11C0
    GtsSearchCursor *searchCursor;           // 0x11C4
    YesNoPrompt *yesNoPrompt;                // 0x11C8
    TouchscreenListMenuSpawner *menuSpawner; // 0x11CC
    TouchscreenListMenu *menu;               // 0x11D0
    u16 saveMidState;                        // 0x11D4, screen 7's state once the first part of the save is written
    u16 saveDoneState;                       // 0x11D6, and once the save is complete
    SysTask *playerAnimTask;                 // 0x11D8
    u16 playerAnimDone;                      // 0x11DC, set by the player's walk in/out task (Gts_StartPlayer*Anim)
    u16 hasSearchResults;                    // 0x11DE
    void *avatarCharAlloc;                   // 0x11E0
    NNSG2dCharacterData *avatarCharData;     // 0x11E4
    void *avatarPlttAlloc;                   // 0x11E8
    NNSG2dPaletteData *avatarPlttData;       // 0x11EC
    Pokemon *depositedMon;                   // 0x11F0, copy of the deposited Pokemon (GtsTrade)
    GtsMonSummary *boxSummaries;             // 0x11F4, of the shown box's Pokemon
    u16 boxMonCount;                         // 0x11F8, Pokemon and Eggs in the PC, counted one box per frame
    u16 countingBox;                         // 0x11FA, 1-based box being counted, 0 when done
    int unk_11FC;                            // 0x11FC
    GtsMonIcon *monIcons;                    // 0x1200, of the shown box, freed by the VBlank task
    GtsVBlankFunc vblankTask;                // 0x1204, run once
    GtsVBlankFunc vblankFunc;                // 0x1208, run every frame
    s16 avatarPositions[8][2];               // 0x120C, {x, y} of avatarSprites
    u8 filler_122C[8];                       // 0x122C
    GtsUnk1234 unk_1234;                     // 0x1234
    WifiServerProfile profile;                     // 0x1260, sent by GtsServer_RequestSetProfile (screen 0)
    u32 profileResponse[2];                  // 0x12C4, its answer
    u32 searchCountry;                       // 0x12CC, of SEEK POKEMON, 0 for any
    GtsPaletteAnim paletteAnim;              // 0x12D0, of screen 0
    s16 subState;                            // 0x1600, of screen 0's state, reset when the state changes
    s16 subTimer;                            // 0x1602, likewise
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

// Choosing the Pokemon to deposit or to trade (gts_select_mon.c).
BOOL Gts_IsPartyBox(int box);
BoxPokemon *Gts_GetBoxMon(Party *party, PCStorage *pcStorage, int box, int slot);
BOOL Gts_MonHoldsMail(Pokemon *mon);

// Choosing the wanted Pokemon of a deposit (gts_wanted.c), and the helpers that follow it: the wanted Pokemon's
// texts, level ranges, species and country lists, the record of a deposit or trade.
// align: 0 left, 1 centered, 2 right in the window.
BOOL Gts_SetFixedGender(GtsWantedMon *wanted, u32 genderRatio);
void Gts_PrintSpeciesName(Window *window, MsgData *speciesNames, int species, int align, int y, u32 color);
void Gts_PrintSpeciesNameNoTransfer(Window *window, MsgData *speciesNames, int species, int y, u32 color);
void Gts_PrintCountryName(Window *window, MsgData *countryNames, MsgData *msgData, u32 country, int align, int y, u32 color);
void Gts_PrintGender(Window *window, MsgData *msgData, int gender, BOOL showGenderless, int y, int xOrAlign, u32 color);
void Gts_PrintGenderNoTransfer(Window *window, MsgData *msgData, int gender, BOOL showGenderless, int x, int y, u32 color);
void Gts_PrintLevelRange(Window *window, MsgData *msgData, int levelRange, int align, int y, u32 color, int kind);
void Gts_PrintLevelRangeAt(Window *window, MsgData *msgData, int levelRange, int align, int x, int y, u32 color, int kind);
void Gts_DrawWantedMon(MsgData *msgData, MsgData *speciesNames, MessageFormat *msgFormat, Window *windows, int species, int gender, int levelRange);
void Gts_DrawWantedMonLevelLeft(MsgData *msgData, MsgData *speciesNames, MessageFormat *msgFormat, Window *windows, int species, int gender, int levelRange);
u16 *Gts_LoadAlphabeticalSpecies(enum HeapID heapID, int unused, u32 *count);
u16 *Gts_LoadSpeciesByLetter(enum HeapID heapID, int letter, u32 *count);
void *Gts_CreateJohtoDexFlags(enum HeapID heapID);
void Gts_InitRecord(GtsRecord *record, GtsApp *app);
int Gts_CreateLevelRangeListItems(ListMenuItem **items, MsgData *msgData, int kind);
void Gts_SetLevelRange(GtsWantedMon *wanted, int levelRange, int kind);
int Gts_FindLevelRange(int minLevel, int maxLevel, int kind);
void Gts_SetSearchCountry(GtsApp *app, int index);
int Gts_GetCountryGroupRange(int group, int *start);
int Gts_CreateCountryListItems(ListMenuItem **items, MsgData *msgData, int group);
void ov70_0223F948(GtsUnk1234 *arg);
void ov70_0223F960(GtsUnk1234 *arg, int index, u16 value1, u16 value2);

// The selector (gts_selector.c). mode: 0 species, 1 gender, 2 level, 3 country. GtsSelector_Main returns -1 while
// the player chooses, -2 for BACK, else the choice.
GtsSelector *GtsSelector_New(GtsSelectorArgs *args, int bgId, int levelRangeKind);
void GtsSelector_Start(GtsSelector *selector, int mode);
void GtsSelector_Delete(GtsSelector *selector);
int GtsSelector_Main(GtsSelector *selector);
// The count of the countries starting with the letter (index A = 0); unused is not read.
int GtsSelector_CountCountriesOfLetter(void *unused, u32 letter);

// Connecting to and disconnecting from Nintendo Wi-Fi Connection (screen 0, gts_connection.c), and the text helpers
// every screen uses. Gts_PrintMessage prints in waitingIconWindow (the message window); align: 0 left at x,
// 1 centered, 2 right in the window.
// The last function of the selector (it links with screen 0): whether a letter group has a letter with countries.
BOOL GtsSelector_HasCountriesInGroup(GtsSelector *selector, int group);
void Gts_PrintMessage(GtsApp *app, MsgData *msgData, int msgId, int textSpeed, u32 unused4);
void Gts_PrintAlignedFont0(Window *window, String *string, int x, int y, int align, u32 color);
void Gts_PrintAlignedFont4(Window *window, String *string, int x, int y, int align, u32 color);
void Gts_PrintPanelCaption(Window *window, MsgData *msgData, int caption);
void Gts_CreateWirelessStatusIcon(GtsApp *app);

#endif // POKEHEARTGOLD_GTS_INTERNAL_H
