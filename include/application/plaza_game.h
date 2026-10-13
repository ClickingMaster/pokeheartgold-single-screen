#ifndef POKEHEARTGOLD_APPLICATION_PLAZA_GAME_H
#define POKEHEARTGOLD_APPLICATION_PLAZA_GAME_H

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "yes_no_prompt.h"

// Shared code of the Wi-Fi Plaza games (SWALOT Plop, MIME JR. Top and WOBBUFFET Pop,
// overlays 91, 92 and 93): the player list shown before a game, the results screen
// shown after it, and the countdown / finish sign used during the game.

// Launch arguments of a Plaza Game, shared with the Wi-Fi Plaza lobby.
typedef struct PlazaGameArgs {
    u32 tapToy;                 // 0x00
    u8 slots[4];                // 0x04
    u8 specialVisitors[4];      // 0x08
    u8 unk0C[0x10];             // 0x0C
    BOOL connectionLost;        // 0x1C
    PlayerProfile *profiles[4]; // 0x20
    void *unk30;                // 0x30
    SaveData *saveData;         // 0x34
    u8 unk38;                   // 0x38
    u8 fromLobby;               // 0x39
    u8 unk3A[2];                // 0x3A
    u32 unk3C;                  // 0x3C
} PlazaGameArgs;

typedef struct PlazaGameRoster {
    SaveData *saveData;   // 0x0
    u8 netIds[4];         // 0x4
    u8 count;             // 0x8
    u8 localIndex;        // 0x9
    u8 voiceChat;              // 0xA
    u8 launchedFromField; // 0xB
    PlazaGameArgs *args;  // 0xC
} PlazaGameRoster;

typedef struct PlazaGameResultData {
    u32 scores[4]; // 0x00
    u8 ranks[4];   // 0x10
    u32 total;     // 0x14
} PlazaGameResultData;

typedef struct PlazaGamePlayerInfo {
    u8 countries[4];            // 0x00
    u8 regions[4];              // 0x04
    PlayerProfile *profiles[4]; // 0x08
} PlazaGamePlayerInfo;

typedef struct PlazaGameBgTemplate {
    u32 bgId;
    BgTemplate template;
} PlazaGameBgTemplate;

typedef struct PlazaGameBg {
    BgConfig *bgConfig;
    const PlazaGameBgTemplate *templates;
    u32 count;
} PlazaGameBg;

typedef struct PlazaGameText {
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    String *buffer;
} PlazaGameText;

typedef struct PlazaGameMessageBox {
    Window windows[2];            // 0x00
    u16 printerIds[2];            // 0x20
    String *strings[2];           // 0x24
    WaitingIcon *waitingIcons[2]; // 0x2C
    u32 colorVariant;             // 0x34
} PlazaGameMessageBox;

typedef struct PlazaGameSprites {
    SpriteList *spriteList;
    G2dRenderer renderer;
} PlazaGameSprites;

typedef struct PlazaGameGfxRes {
    u32 active;
    SpriteResource *resources[4];
    SpriteResourcesHeader header;
} PlazaGameGfxRes;

typedef struct PlazaGameGfx {
    GF_2DGfxResMan *managers[4];
    PlazaGameGfxRes *records;
    u32 count;
} PlazaGameGfx;

// Uniformly accelerated motion from start, reaching its end position after duration frames.
typedef struct PlazaGameMotion {
    fx32 current;
    fx32 start;
    fx32 velocity;
    fx32 accel;
    int duration;
} PlazaGameMotion;

// The game name banner that slides in at the top of the player list and the results screen.
typedef struct PlazaGameTitle {
    Window window;          // 0x00
    u32 game;               // 0x10
    String *string;         // 0x14
    PlazaGameMotion motion; // 0x18
    s16 frame;              // 0x2C
    s16 state;              // 0x2E
} PlazaGameTitle;

// One row of the player list.
typedef struct PlazaGamePlayer {
    Window window;          // 0x00
    Sprite *sprite;         // 0x10
    Sprite *rankSprite;     // 0x14
    u16 count;              // 0x18
    u8 index;               // 0x1A
    u8 seat;                // 0x1B
    PlazaGameMotion motion; // 0x1C
    s16 frame;              // 0x30
    s16 delay;              // 0x32
    u32 se;                 // 0x34
} PlazaGamePlayer;

// Per-scanline horizontal scroll of the player rows.
typedef struct PlazaGameLineScroll {
    PlazaGameBg *bg;
    u16 buffers[2][192];
    struct DoubleBuffer *buffer;
} PlazaGameLineScroll;

typedef struct PlazaGameWindowBits {
    u8 planes : 5;
    u8 effect : 1;
    u8 unused : 2;
} PlazaGameWindowBits;

// Window 0 frame around the countdown / finish sign, opened by two sliding bars.
typedef struct PlazaGameSignWindow {
    Sprite *sprites[2];               // 0x000
    s16 step;                         // 0x008
    s16 offset;                       // 0x00A
    u8 lines[192];                    // 0x00C
    u8 pendingLines[192];             // 0x0CC
    SysTask *task;                    // 0x18C
    PlazaGameWindowBits savedOutside; // 0x190
    PlazaGameWindowBits signOutside;  // 0x191
    u8 unk192[2];                     // 0x192
    u32 savedWindows;                 // 0x194
} PlazaGameSignWindow;

typedef struct PlazaGameSign {
    enum HeapID heapId;         // 0x000
    u16 state;                  // 0x004
    s16 timer;                  // 0x006
    SpriteList *spriteList;     // 0x008
    NARC *narc;                 // 0x00C
    PlazaGameGfx gfx;           // 0x010
    PlazaGameGfxRes *gfxRes;    // 0x028
    SysTask *task;              // 0x02C
    Sprite *sprite;             // 0x030
    PlazaGameSignWindow window; // 0x034
} PlazaGameSign;

// "Would you like to keep playing?" prompt and the vote of all players.
typedef struct PlazaGameContinue {
    u8 state;                     // 0x00
    u8 voiceChat;                     // 0x01
    u16 timer;                    // 0x02
    YesNoPrompt *prompt;          // 0x04
    YesNoPromptTemplate template; // 0x08
    u8 votes;                     // 0x1C
    u8 allYes;                    // 0x1D
    u8 received;                  // 0x1E
    u8 result;                    // 0x1F
} PlazaGameContinue;

// Popped balloon counter of the WOBBUFFET Pop results.
typedef struct PlazaGameBalloons {
    void *screenRaw;          // 0x00
    NNSG2dScreenData *screen; // 0x04
    Sprite *sprites[15];      // 0x08
    PlazaGameGfxRes *gfxRes;  // 0x44
    u8 state;                 // 0x48
    u8 timer;                 // 0x49
    u8 count;                 // 0x4A
    u8 total;                 // 0x4B
} PlazaGameBalloons;

// Player list shown before a game ("These players have been accepted.").
typedef struct PlazaGameEntry {
    u8 state;                       // 0x000
    u8 timer;                       // 0x001
    u16 heapId;                     // 0x002
    u16 game;                       // 0x004
    u8 selection;                   // 0x006
    u8 selectionReceived;           // 0x007
    PlazaGameRoster roster;         // 0x008
    PlazaGamePlayerInfo info;       // 0x018
    PlazaGameBg bg;                 // 0x030
    PlazaGameText text;             // 0x03C
    PlazaGameMessageBox messageBox; // 0x04C
    PlazaGameSprites sprites;       // 0x084
    PlazaGameGfx gfx;               // 0x1B0
    PlazaGameGfxRes *gfxRes;        // 0x1C8
    PlazaGameTitle title;           // 0x1CC
    void *screenRaw;                // 0x1FC
    NNSG2dScreenData *screen;       // 0x200
    PlazaGamePlayer players[4];     // 0x204
    PlazaGameLineScroll lineScroll; // 0x2E4
    SysTask *task;                  // 0x5EC
    SysTask *vblankTask;            // 0x5F0
} PlazaGameEntry;

// Results screen shown after a game ("Here are the results!").
typedef struct PlazaGameResults {
    u16 timer;                        // 0x000
    u16 heapId;                       // 0x002
    u8 state;                         // 0x004
    u8 showCountries;                 // 0x005
    u8 done;                          // 0x006
    u8 game;                          // 0x007
    u8 continueResult;                // 0x008
    u8 tapToyPoweredUp;               // 0x009
    u8 unk0A[2];                      // 0x00A
    PlazaGameRoster roster;           // 0x00C
    PlazaGameResultData data;         // 0x01C
    PlazaGamePlayerInfo info;         // 0x034
    PlazaGameBg bg;                   // 0x04C
    PlazaGameText text;               // 0x058
    PlazaGameMessageBox messageBox;   // 0x068
    PlazaGameSprites sprites;         // 0x0A0
    PlazaGameGfx gfx;                 // 0x1CC
    PlazaGameGfxRes *gfxRes;          // 0x1E4
    PlazaGameContinue continuePrompt; // 0x1E8
    PlazaGameBalloons balloons;       // 0x208
    void *screenRaw;                  // 0x254
    NNSG2dScreenData *screen;         // 0x258
    PlazaGamePlayer players[4];       // 0x25C
    PlazaGameLineScroll lineScroll;   // 0x33C
    SysTask *task;                    // 0x644
    SysTask *vblankTask;              // 0x648
    u32 prevTapToy;                   // 0x64C
    u16 rankStep;                     // 0x650
    u16 delay;                        // 0x652
    void *flashPlttRaw;               // 0x654
    NNSG2dPaletteData *flashPltt;     // 0x658
    u16 flashFrame;                   // 0x65C
    u16 flashEnabled;                 // 0x65E
    u32 fanfareState;                 // 0x660
} PlazaGameResults;

// Used by overlays 91, 92 and 93.
void PlazaGameRoster_Init(PlazaGameRoster *roster, u32 fromLobby, SaveData *saveData, u8 unkA, PlazaGameArgs *args);
int PlazaGameRoster_GetIndexOfNetId(const PlazaGameRoster *roster, u32 netId);
u8 PlazaGameRoster_GetNetId(const PlazaGameRoster *roster, u32 index);
u32 PlazaGameRoster_IsSpecialVisitor(const PlazaGameRoster *roster, u32 netId);
PlayerProfile *PlazaGameRoster_GetProfile(const PlazaGameRoster *roster, u32 netId);
PlazaGameEntry *PlazaGameEntry_NewSwalotPlop(const PlazaGameRoster *roster, enum HeapID heapId);
PlazaGameEntry *PlazaGameEntry_NewMimeJrTop(const PlazaGameRoster *roster, enum HeapID heapId);
PlazaGameEntry *PlazaGameEntry_NewWobbuffetPop(const PlazaGameRoster *roster, enum HeapID heapId);
void PlazaGameEntry_Free(PlazaGameEntry *entry);
BOOL PlazaGameEntry_IsDone(const PlazaGameEntry *entry);
u8 PlazaGameEntry_GetSelection(const PlazaGameEntry *entry);
PlazaGameResults *PlazaGameResults_NewSwalotPlop(const PlazaGameRoster *roster, const PlazaGameResultData *data, enum HeapID heapId);
PlazaGameResults *PlazaGameResults_NewMimeJrTop(const PlazaGameRoster *roster, const PlazaGameResultData *data, enum HeapID heapId);
PlazaGameResults *PlazaGameResults_NewWobbuffetPop(const PlazaGameRoster *roster, const PlazaGameResultData *data, enum HeapID heapId);
void PlazaGameResults_Free(PlazaGameResults *results);
u8 PlazaGameResults_IsDone(const PlazaGameResults *results);
u8 PlazaGameResults_GetContinue(const PlazaGameResults *results);
void PlazaGame_CalcRanks(PlazaGameResultData *data, u32 count);
BOOL PlazaGameRoster_CheckConnectionLost(PlazaGameRoster *roster);
BOOL PlazaGameRoster_ProcessConnectionLost(PlazaGameRoster *roster);
PlazaGameSign *PlazaGameSign_New(SpriteList *spriteList, enum HeapID heapId);
void PlazaGameSign_Free(PlazaGameSign *sign);
int PlazaGameSign_GetPlttOffset(PlazaGameSign *sign);
void PlazaGameSign_StartCountdown(PlazaGameSign *sign);
void PlazaGameSign_StartFinish(PlazaGameSign *sign);
BOOL PlazaGameSign_IsDone(const PlazaGameSign *sign);

// Internal.
void PlazaGameEntry_SetSelection(PlazaGameEntry *entry, int selection);
void PlazaGameResults_ReceiveVote(PlazaGameResults *results, int netId, int yes);
void PlazaGameResults_ReceiveContinue(PlazaGameResults *results, u8 result);
void PlazaGameMotion_Init(PlazaGameMotion *motion, fx32 start, fx32 end, fx32 velocity, int duration);
int PlazaGameMotion_Update(PlazaGameMotion *motion, int frame);
void PlazaGame_OffsetScreenTiles(NNSG2dScreenData *screen, u16 offset);
void PlazaGameGfx_Init(PlazaGameGfx *gfx, int count, enum HeapID heapId);
void PlazaGameGfx_Free(PlazaGameGfx *gfx);
PlazaGameGfxRes *PlazaGameGfx_Load(PlazaGameGfx *gfx, NARC *narc, int plttFile, int plttCount, int charFile, int cellFile, int animFile, int id, enum HeapID heapId);
Sprite *PlazaGameGfx_CreateSprite(PlazaGameGfxRes *gfxRes, SpriteList *spriteList, s16 x, s16 y, u16 priority, enum HeapID heapId);
void PlazaGameGfx_LoadInto(PlazaGameGfx *gfx, PlazaGameGfxRes *gfxRes, NARC *narc, int plttFile, int plttCount, int charFile, int cellFile, int animFile, int id, enum HeapID heapId);
void PlazaGameGfx_Unload(PlazaGameGfx *gfx, PlazaGameGfxRes *gfxRes);
void PlazaGameBg_Init(PlazaGameBg *bg, const GraphicsModes *modes, const PlazaGameBgTemplate *templates, u32 count, enum HeapID heapId);
void PlazaGameBg_Free(PlazaGameBg *bg);
void PlazaGameBg_Update(PlazaGameBg *bg);
void PlazaGameSprites_Init(PlazaGameSprites *sprites, int count, int charCount, int plttCount, enum HeapID heapId);
void PlazaGameSprites_Free(PlazaGameSprites *sprites);
void PlazaGameSprites_Update(PlazaGameSprites *sprites);
void PlazaGameSprites_ApplyOam(PlazaGameSprites *sprites);
void PlazaGameText_Init(PlazaGameText *text, enum HeapID heapId);
void PlazaGameText_Free(PlazaGameText *text);
void PlazaGameText_ResetBuffers(PlazaGameText *text);
void PlazaGameText_BufferPlayerName(PlazaGameText *text, PlayerProfile *profile);
void PlazaGameText_BufferCountry(PlazaGameText *text, int country);
void PlazaGameText_BufferCity(PlazaGameText *text, int country, int region);
void PlazaGameText_BufferNumber(PlazaGameText *text, int value, int digits);
void PlazaGameText_BufferTwoDigitNumber(PlazaGameText *text, int value);
void PlazaGameText_BufferTapToyName(PlazaGameText *text, int tapToy);
void PlazaGameText_BufferActivityName(PlazaGameText *text, int game);
void PlazaGameText_Format(PlazaGameText *text, String *dest, int msgNo);
void PlazaGameText_Print(PlazaGameText *text, int msgNo, Window *window, u8 x, u8 y);
int PlazaGameText_PrintRightAligned(PlazaGameText *text, int msgNo, Window *window, u8 right, u8 y);
int PlazaGameText_PrintMessage(PlazaGameText *text, int msgNo, Window *window, String *string, int textSpeed);
int PlazaGameText_PrintWithColor(PlazaGameText *text, int msgNo, Window *window, u8 x, u8 y, u32 color);
void PlazaGameMessageBox_Init(PlazaGameMessageBox *messageBox, PlazaGameBg *bg, SaveData *saveData, u32 colorVariant, enum HeapID heapId);
void PlazaGameMessageBox_Free(PlazaGameMessageBox *messageBox);
void PlazaGameMessageBox_Show(PlazaGameMessageBox *messageBox, PlazaGameText *text, int msgNo, int index);
void PlazaGameMessageBox_Hide(PlazaGameMessageBox *messageBox, int index);
BOOL PlazaGameMessageBox_IsPrinterFinished(PlazaGameMessageBox *messageBox, int index);
void PlazaGameMessageBox_ShowWaitingIcon(PlazaGameMessageBox *messageBox, int index);
void PlazaGameMessageBox_HideWaitingIcon(PlazaGameMessageBox *messageBox, int index);
PlazaGameEntry *PlazaGameEntry_New(const PlazaGameRoster *roster, u16 game, enum HeapID heapId);
void PlazaGameEntry_LoadGraphics(PlazaGameEntry *entry, enum HeapID heapId);
void PlazaGameEntry_FreeGraphics(PlazaGameEntry *entry);
void PlazaGameEntry_MainTask(SysTask *task, void *data);
void PlazaGameEntry_VBlankTask(SysTask *task, void *data);
u32 PlazaGame_PickWeighted(const u8 *weights, u32 count);
BOOL PlazaGameEntry_UpdatePlayers(PlazaGameEntry *entry);
void PlazaGamePlayer_Init(PlazaGamePlayer *player, int count, int game, NNSG2dScreenData *screen, PlazaGameBg *bg, int index, u32 localIndex, PlazaGameSprites *sprites, PlazaGameGfxRes *gfxRes, enum HeapID heapId);
void PlazaGamePlayer_PrintName(PlazaGamePlayer *player, PlazaGameText *text, PlayerProfile *profile, int x, s16 y, int highlight);
void PlazaGamePlayer_PrintLocation(PlazaGamePlayer *player, PlazaGameText *text, int showCountry, int country, int region, s16 x);
void PlazaGamePlayer_ShowRank(PlazaGamePlayer *player, int rank);
void PlazaGamePlayer_PrintScore(PlazaGamePlayer *player, PlazaGameText *text, int score, int right, s16 y, int digits);
void PlazaGamePlayer_ScheduleTransfer(PlazaGamePlayer *player, PlazaGameBg *bg);
void PlazaGamePlayer_ShowSprite(PlazaGamePlayer *player);
void PlazaGamePlayer_Free(PlazaGamePlayer *player);
void PlazaGamePlayer_StartSlideIn(PlazaGamePlayer *player, int x, s16 delay);
void PlazaGamePlayer_StartSlideOut(PlazaGamePlayer *player, s16 delay);
int PlazaGamePlayer_UpdateSlide(PlazaGamePlayer *player);
void PlazaGamePlayer_SetSpritePos(PlazaGamePlayer *player, u32 x);
int PlazaGame_GetRelativeSeat(u32 count, int index, u32 localIndex);
void PlazaGameLineScroll_Init(PlazaGameLineScroll *lineScroll, PlazaGameBg *bg, enum HeapID heapId);
void PlazaGameLineScroll_Free(PlazaGameLineScroll *lineScroll);
void PlazaGameLineScroll_SetPlayer(PlazaGameLineScroll *lineScroll, PlazaGamePlayer *player);
void PlazaGameLineScroll_Swap(PlazaGameLineScroll *lineScroll);
void PlazaGameLineScroll_HBlank(void *data);
void PlazaGamePlayerInfo_Init(PlazaGamePlayerInfo *info, const PlazaGameRoster *roster);
int PlazaGamePlayerInfo_HasDifferentCountries(const PlazaGameRoster *roster, const u8 *countries);
u32 PlazaGame_GetPoweredUpTapToy(u32 tapToy);
void PlazaGameTitle_Init(PlazaGameTitle *title, PlazaGameBg *bg, PlazaGameText *text, int game, NARC *narc, enum HeapID heapId);
void PlazaGameTitle_Free(PlazaGameTitle *title);
void PlazaGameTitle_StartSlideIn(PlazaGameTitle *title, PlazaGameBg *bg, enum HeapID heapId);
void PlazaGameTitle_StartSlideOut(PlazaGameTitle *title, PlazaGameBg *bg);
BOOL PlazaGameTitle_UpdateSlideIn(PlazaGameTitle *title, PlazaGameBg *bg);
BOOL PlazaGameTitle_UpdateSlideOut(PlazaGameTitle *title, PlazaGameBg *bg);
void PlazaGameTitle_SetScroll(PlazaGameTitle *title, PlazaGameBg *bg, int value);
void PlazaGameTitle_ScheduleScroll(PlazaGameTitle *title, PlazaGameBg *bg, int value);
void PlazaGame_LoadGamePalette(NARC *narc, int game, enum HeapID heapId);
PlazaGameResults *PlazaGameResults_New(const PlazaGameRoster *roster, const PlazaGameResultData *data, u8 game, enum HeapID heapId);
void PlazaGameResults_LoadGraphics(PlazaGameResults *results, enum HeapID heapId);
void PlazaGameResults_FreeGraphics(PlazaGameResults *results);
void PlazaGameResults_RankingTask(SysTask *task, void *data);
void PlazaGameResults_TeamTask(SysTask *task, void *data);
void PlazaGameResults_VBlankTask(SysTask *task, void *data);
void PlazaGameResults_LoadFlashPalette(PlazaGameResults *results, NARC *narc, enum HeapID heapId);
void PlazaGameResults_StartFlash(PlazaGameResults *results);
void PlazaGameResults_UpdateFlash(PlazaGameResults *results);
void PlazaGameResults_FreeFlashPalette(PlazaGameResults *results);
void PlazaGameContinue_Init(PlazaGameContinue *prompt, PlazaGameBg *bg, u8 unk01, enum HeapID heapId);
void PlazaGameContinue_Free(PlazaGameContinue *prompt);
BOOL PlazaGameContinue_Run(PlazaGameContinue *prompt, PlazaGameMessageBox *messageBox, PlazaGameText *text, int game, enum HeapID heapId);
u8 PlazaGameContinue_GetResult(const PlazaGameContinue *prompt);
void PlazaGameContinue_ReceiveVote(PlazaGameContinue *prompt, int netId, int yes, int count);
void PlazaGameContinue_SetResult(PlazaGameContinue *prompt, u8 result);
void PlazaGameBalloons_Init(PlazaGameBalloons *balloons, PlazaGameBg *bg, PlazaGameSprites *sprites, PlazaGameGfx *gfx, NARC *narc, enum HeapID heapId);
void PlazaGameBalloons_Free(PlazaGameBalloons *balloons);
void PlazaGameBalloons_Start(PlazaGameBalloons *balloons, int total);
BOOL PlazaGameBalloons_Update(PlazaGameBalloons *balloons, PlazaGameBg *bg);
void PlazaGameResults_ShowPlayer(PlazaGameResults *results, int index, int rank);
BOOL PlazaGameResults_UpdatePlayers(PlazaGameResults *results);
void PlazaGameResults_PlayFanfare(PlazaGameResults *results);
BOOL PlazaGameResults_IsFanfareDone(PlazaGameResults *results);
void PlazaGameResults_RecordFriendPlays(PlazaGameResults *results);
int PlazaGameResults_GetScoreThreshold(PlazaGameResults *results);
void PlazaGameResults_ReportWinners(PlazaGameResults *results);
void PlazaGameResults_ReportTeamWin(PlazaGameResults *results);
void PlazaGameSignWindow_Init(PlazaGameSignWindow *window, PlazaGameGfxRes *gfxRes, SpriteList *spriteList, enum HeapID heapId);
BOOL PlazaGameSignWindow_Open(PlazaGameSignWindow *window);
BOOL PlazaGameSignWindow_Close(PlazaGameSignWindow *window);
void PlazaGameSignWindow_Move(PlazaGameSignWindow *window, int delta);
void PlazaGameSignWindow_Free(PlazaGameSignWindow *window);
void PlazaGameSignWindow_UpdateLines(PlazaGameSignWindow *window);
void PlazaGameSignWindow_CommitLinesTask(SysTask *task, void *data);
void PlazaGameSignWindow_HBlank(void *data);
void PlazaGameSign_CountdownTask(SysTask *task, void *data);
void PlazaGameSign_FinishTask(SysTask *task, void *data);
const void *PlazaGame_GetCommandTable(void);
int PlazaGame_GetCommandCount(void);
void PlazaGameResults_InitCommands(PlazaGameResults *results);
void PlazaGameEntry_InitCommands(PlazaGameEntry *entry);
int PlazaGame_GetCommandSizeNone(void);
int PlazaGame_GetCommandSizeU32(void);
void PlazaGame_HandleSelectionCommand(int netId, int size, int *data, PlazaGameEntry *entry);
void PlazaGame_HandleVoteYesCommand(int netId, int size, void *data, PlazaGameResults *results);
void PlazaGame_HandleVoteNoCommand(int netId, int size, void *data, PlazaGameResults *results);
void PlazaGame_HandleContinueCommand(int netId, int size, void *data, PlazaGameResults *results);
void PlazaGame_HandleQuitCommand(int netId, int size, void *data, PlazaGameResults *results);

#endif // POKEHEARTGOLD_APPLICATION_PLAZA_GAME_H
