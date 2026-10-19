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
#include "sprite_resource_header.h"
#include "sprite_resource.h"
#include "yes_no_prompt.h"

// Shared code of the Wi-Fi Plaza games (SWALOT Plop, MIME JR. Top and WOBBUFFET Pop): the player list
// shown before a game, the results screen shown after it, and the countdown / finish sign.

// Launch arguments of a Plaza Game, shared with the Wi-Fi Plaza lobby.
typedef struct PlazaGameArgs {
    u32 tapToy;
    u8 slots[4];
    u8 specialVisitors[20]; // per lobby slot
    BOOL connectionLost;
    PlayerProfile *profiles[4];
    void *plazaWork;
    SaveData *saveData;
    u8 voiceChat;
    u8 fromLobby;
    u8 padding_3A[2];
    u32 mimeJrTopVoiceChat;
} PlazaGameArgs;

typedef struct PlazaGameRoster {
    SaveData *saveData;
    u8 netIds[4];
    u8 count;
    u8 localIndex;
    u8 voiceChat;
    u8 launchedFromField;
    PlazaGameArgs *args;
} PlazaGameRoster;

typedef struct PlazaGameResultData {
    u32 scores[4];
    u8 ranks[4];
    u32 total;
} PlazaGameResultData;

typedef struct PlazaGamePlayerInfo {
    u8 countries[4];
    u8 regions[4];
    PlayerProfile *profiles[4];
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
    Window windows[2];
    u16 printerIds[2];
    String *strings[2];
    WaitingIcon *waitingIcons[2];
    u32 colorVariant;
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
    Window window;
    u32 game;
    String *string;
    PlazaGameMotion motion;
    s16 frame;
    s16 state;
} PlazaGameTitle;

// One row of the player list.
typedef struct PlazaGamePlayer {
    Window window;
    Sprite *sprite;
    Sprite *rankSprite;
    u16 count;
    u8 index;
    u8 seat;
    PlazaGameMotion motion;
    s16 frame;
    s16 delay;
    u32 se;
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
    Sprite *sprites[2];
    s16 step;
    s16 offset;
    u8 lines[192];
    u8 pendingLines[192];
    SysTask *task;
    PlazaGameWindowBits savedOutside;
    PlazaGameWindowBits signOutside;
    u8 unk192[2];
    u32 savedWindows;
} PlazaGameSignWindow;

typedef struct PlazaGameSign {
    enum HeapID heapId;
    u16 state;
    s16 timer;
    SpriteList *spriteList;
    NARC *narc;
    PlazaGameGfx gfx;
    PlazaGameGfxRes *gfxRes;
    SysTask *task;
    Sprite *sprite;
    PlazaGameSignWindow window;
} PlazaGameSign;

// "Would you like to keep playing?" prompt and the vote of all players.
typedef struct PlazaGameContinue {
    u8 state;
    u8 voiceChat;
    u16 timer;
    YesNoPrompt *prompt;
    YesNoPromptTemplate template;
    u8 votes;
    u8 allYes;
    u8 received;
    u8 result;
} PlazaGameContinue;

// Popped balloon counter of the WOBBUFFET Pop results.
typedef struct PlazaGameBalloons {
    void *screenRaw;
    NNSG2dScreenData *screen;
    Sprite *sprites[15];
    PlazaGameGfxRes *gfxRes;
    u8 state;
    u8 timer;
    u8 count;
    u8 total;
} PlazaGameBalloons;

// Player list shown before a game ("These players have been accepted.").
typedef struct PlazaGameEntry {
    u8 state;
    u8 timer;
    u16 heapId;
    u16 game;
    u8 selection;
    u8 selectionReceived;
    PlazaGameRoster roster;
    PlazaGamePlayerInfo info;
    PlazaGameBg bg;
    PlazaGameText text;
    PlazaGameMessageBox messageBox;
    PlazaGameSprites sprites;
    PlazaGameGfx gfx;
    PlazaGameGfxRes *gfxRes;
    PlazaGameTitle title;
    void *screenRaw;
    NNSG2dScreenData *screen;
    PlazaGamePlayer players[4];
    PlazaGameLineScroll lineScroll;
    SysTask *task;
    SysTask *vblankTask;
} PlazaGameEntry;

// Results screen shown after a game ("Here are the results!").
typedef struct PlazaGameResults {
    u16 timer;
    u16 heapId;
    u8 state;
    u8 showCountries;
    u8 done;
    u8 game;
    u8 continueResult;
    u8 tapToyPoweredUp;
    u8 unk0A[2];
    PlazaGameRoster roster;
    PlazaGameResultData data;
    PlazaGamePlayerInfo info;
    PlazaGameBg bg;
    PlazaGameText text;
    PlazaGameMessageBox messageBox;
    PlazaGameSprites sprites;
    PlazaGameGfx gfx;
    PlazaGameGfxRes *gfxRes;
    PlazaGameContinue continuePrompt;
    PlazaGameBalloons balloons;
    void *screenRaw;
    NNSG2dScreenData *screen;
    PlazaGamePlayer players[4];
    PlazaGameLineScroll lineScroll;
    SysTask *task;
    SysTask *vblankTask;
    u32 prevTapToy;
    u16 rankStep;
    u16 delay;
    void *flashPlttRaw;
    NNSG2dPaletteData *flashPltt;
    u16 flashFrame;
    u16 flashEnabled;
    u32 fanfareState;
} PlazaGameResults;

void PlazaGameRoster_Init(PlazaGameRoster *roster, u32 fromLobby, SaveData *saveData, u8 voiceChat, PlazaGameArgs *args);
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
void PlazaGameContinue_Init(PlazaGameContinue *prompt, PlazaGameBg *bg, u8 voiceChat, enum HeapID heapId);
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
