#ifndef POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H

#include "global.h"

#include "application/dress_up/accessory_portrait.h"
#include "global_terminal/global_terminal_net.h"

#include "battle_record.h"
#include "bg_window.h"
#include "fashion_case.h"
#include "frontier_card.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"

// VS Recorder app: the VS Recorder item (mode 0) and the Global Terminal data browser (modes 1-6).

typedef struct VsRecorderWork VsRecorderWork;

SaveFashionDataSub *FashionEntry_New(enum HeapID heapID);
void FashionEntry_GetMonSecondText(SaveFashionDataSub *entry, String *string);
u16 FashionEntry_GetMailMessageWord(SaveFashionDataSub *entry);

// one step of a screen; TRUE when the screen is done
typedef BOOL (*VsRecorderScreenFunc)(VsRecorderWork *work);

// per-screen step tables, indexed by VsRecorderWork.subState
extern const VsRecorderScreenFunc gVsRecorderTitleSubStates[];
extern const VsRecorderScreenFunc gVsRecorderMenuSubStates[];
extern const VsRecorderScreenFunc gVsRecorderSavedVideoSubStates[];
extern const VsRecorderScreenFunc gVsRecorderFrontierSubStates[];
extern const VsRecorderScreenFunc gVsRecorderDressUpSendSubStates[];
extern const VsRecorderScreenFunc gVsRecorderDressUpViewSubStates[];

// VsRecorderWork.screenPtr
#define VS_RECORDER_SCREEN_TITLE         0
#define VS_RECORDER_SCREEN_MENU          1
#define VS_RECORDER_SCREEN_SAVED_VIDEO   2
#define VS_RECORDER_SCREEN_FRONTIER      3
#define VS_RECORDER_SCREEN_DRESS_UP_SEND 4
#define VS_RECORDER_SCREEN_DRESS_UP_VIEW 5
#define VS_RECORDER_SCREEN_BOX_DATA_SEND 6
#define VS_RECORDER_SCREEN_BOX_DATA_VIEW 7
#define VS_RECORDER_SCREEN_RANKINGS      8
#define VS_RECORDER_SCREEN_RANKINGS_LAST 9
#define VS_RECORDER_SCREEN_COLOSSEUM_TOP 10
#define VS_RECORDER_SCREEN_FRONTIER_TOP  11
#define VS_RECORDER_SCREEN_NEWEST        12
#define VS_RECORDER_SCREEN_SEARCH        13
#define VS_RECORDER_SCREEN_SEARCH_NUMBER 14
#define VS_RECORDER_SCREEN_SEND_VIDEO    15
#define VS_RECORDER_SCREEN_EXIT          16
#define VS_RECORDER_SCREEN_DELETE        17
#define VS_RECORDER_SCREEN_MESSAGE       18
#define VS_RECORDER_SCREEN_NONE          0xFF

// substates of the menu (gVsRecorderMenuSubStates)
#define VS_RECORDER_MENU_MAIN        0
#define VS_RECORDER_MENU_INVALID     1
#define VS_RECORDER_MENU_SLIDE_IN    5
#define VS_RECORDER_MENU_PREV_SCHEME 7
#define VS_RECORDER_MENU_NEXT_SCHEME 8

typedef struct VsRecorderMenuEntry {
    int enabled;
    int unk_04;
    int msgId;
    int anim;
    int action;       // VS_RECORDER_MENU_ACTION_*, run when the menu substate ends
    int nextSubState; // menu substate after the touch (VS_RECORDER_MENU_ACTION_CANCEL: leave)
    u32 selection;    // copied to VsRecorderWork.selection when chosen
    int unk_1C;
    const struct VsRecorderMenuEntry *child;
} VsRecorderMenuEntry;

enum VsRecorderSelection {
    VS_RECORDER_SELECTION_SEND = 100,
    VS_RECORDER_SELECTION_NEWEST_30 = 200,
    VS_RECORDER_SELECTION_SEARCH = 210,
    VS_RECORDER_SELECTION_SEARCH_BY_NUMBER = 220,
    VS_RECORDER_SELECTION_COLOSSEUM = 300,
    VS_RECORDER_SELECTION_BATTLE_FRONTIER = 301,
    VS_RECORDER_SELECTION_CANCEL = 999,
    VS_RECORDER_SELECTION_SEND_VIDEO = 9999,
};

enum VsRecorderRequest {
    VS_RECORDER_REQUEST_DRESS_UP_UPLOAD,
    VS_RECORDER_REQUEST_DRESS_UP_DOWNLOAD,
    VS_RECORDER_REQUEST_BOX_UPLOAD,
    VS_RECORDER_REQUEST_BOX_DOWNLOAD,
    VS_RECORDER_REQUEST_RANKINGS_HEADER,
    VS_RECORDER_REQUEST_RANKINGS,
    VS_RECORDER_REQUEST_VIDEO_UPLOAD,
    VS_RECORDER_REQUEST_VIDEO_SEARCH,
    VS_RECORDER_REQUEST_VIDEO_DOWNLOAD,
};

// VsRecorderWork.step: skip to the last step of a substate
#define VS_RECORDER_STEP_FINISH 0xFF

enum VsRecorderMessageScreenStep {
    VS_RECORDER_MESSAGE_SCREEN_SHOW,
    VS_RECORDER_MESSAGE_SCREEN_WAIT_TOUCH,
    VS_RECORDER_MESSAGE_SCREEN_RETURN,
};

// VsRecorderMenuEntry.action
#define VS_RECORDER_MENU_ACTION_CANCEL 2

// VsRecorderMenuEntry.action: open a screen
#define VS_RECORDER_MENU_ACTION_SAVED_VIDEO   4
#define VS_RECORDER_MENU_ACTION_FRONTIER      5
#define VS_RECORDER_MENU_ACTION_DELETE        7
#define VS_RECORDER_MENU_ACTION_DRESS_UP_SEND 8
#define VS_RECORDER_MENU_ACTION_DRESS_UP_VIEW 9
#define VS_RECORDER_MENU_ACTION_BOX_DATA_SEND 10
#define VS_RECORDER_MENU_ACTION_BOX_DATA_VIEW 11
#define VS_RECORDER_MENU_ACTION_RANKINGS      12
#define VS_RECORDER_MENU_ACTION_RANKINGS_LAST 13
#define VS_RECORDER_MENU_ACTION_COLOSSEUM_TOP 14
#define VS_RECORDER_MENU_ACTION_FRONTIER_TOP  15
#define VS_RECORDER_MENU_ACTION_NEWEST        16
#define VS_RECORDER_MENU_ACTION_SEARCH        17
#define VS_RECORDER_MENU_ACTION_SEARCH_NUMBER 18
#define VS_RECORDER_MENU_ACTION_SEND_VIDEO    19

// one menu row sprite with its label (left column: the path taken, right column: the current choices)
typedef struct VsRecorderMenuRow {
    ManagedSprite *sprite;
    u8 unk_04[8];
    int enabled;
    TouchscreenHitbox *hitbox;
    TextOBJ *text;
    TextOBJLayout *textLayout;
    ObjCharSpaceReservation charAlloc;
} VsRecorderMenuRow;

// a sprite with a one-line text label, used by several screens
typedef struct VsRecorderTextSprite {
    u32 unk_00;
    ManagedSprite *sprite;
    TextOBJ *text;
    TextOBJLayout *textLayout;
    ObjCharSpaceReservation charAlloc;
} VsRecorderTextSprite;

// the resource block of VsRecorderWork from narc to fontSystem; the text sprite helpers get &work->narc
typedef struct VsRecorderGfx {
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u32 unk_0C;
    BgConfig *bgConfig;
    PaletteData *plttData;
    TouchHitboxController *hitboxController;
    TouchscreenHitbox menuHitboxes[5];
    u32 unk_30;
    MsgData *msgData;
    MsgData *recordMsgData;
    FontSystem *fontSystem;
} VsRecorderGfx;

// a sprite pair (main + sub screen) drifting in from the side, animated by a V-blank task
typedef struct VsRecorderDecor {
    ManagedSprite *sprites[2];
    SysTask *task;
    fx32 startX;
    f32 scale;
    s16 stepX;
    s16 targetX;
    s8 direction;
    u8 phase;
    u8 unk_1A[2];
    int tick;
    int angle;
    int duration;
    int counter;
    int state;
} VsRecorderDecor;

// trainer profile panel (frontier card) on the top screen
typedef struct VsRecorderProfile {
    FrontierCard *card;
    ManagedSprite *monIcon;
    ManagedSprite *avatar;
    Window windows[8];
} VsRecorderProfile;

typedef struct VsRecorderIconPos {
    s16 x;
    s16 y;
} VsRecorderIconPos;

typedef struct VsRecorderIconPositions {
    VsRecorderIconPos pos[5 * 6];
} VsRecorderIconPositions;

// party slots in icon order, by VsRecorder_HeaderUsesThreeMonLayout (0: singles, 1: the other side starts at the fourth icon)
typedef struct VsRecorderPartyIconOrder {
    int slots[2][6];
} VsRecorderPartyIconOrder;

typedef struct VsRecorderSlotButtonTable {
    int values[4];
} VsRecorderSlotButtonTable;

// one entry of a list window: message, value (message / month / species) and a ranking time
typedef struct VsRecorderListEntry {
    u32 msgId;
    int value;
    u64 time;
} VsRecorderListEntry;

typedef struct VsRecorderListConfig {
    VsRecorderListEntry *entries;
    int count;
    int lineHeight; // in 16-pixel units
    int x;
    int y;
    int width;
    int height;
    int baseTile;
    int bgId;
    int rows;
    const TouchscreenHitbox *hitboxes; // one per visible row, or NULL
} VsRecorderListConfig;

// a scrolling list in one BG window
typedef struct VsRecorderListWindow {
    int state;
    int count;
    s16 scroll; // first shown entry (scrolled lists)
    u8 unk_0A[2];
    int drawnPos; // scroll / cursor of the last redraw
    int rows;     // visible rows
    int pages;
    Window window;
    VsRecorderListConfig *config;
    ManagedSprite *arrows[2]; // up, down
    MsgData *msgData;
    int cursor;    // selected entry (cursor lists)
    int cursorRow; // row of the cursor on screen
    int topMargin;
    int bottomMargin;
    int isPairList;
    u32 unk_4C;
} VsRecorderListWindow;

// drag scrolling on the touch screen (two side markers)
typedef struct VsRecorderTouchScroll {
    u32 timer;
    BOOL held;
    u32 startX;
    int lastY;
    s16 direction; // -1 up, 1 down, 0 none
    u8 unk_12[2];
    BOOL enabled;
    ManagedSprite *sprites[2];
} VsRecorderTouchScroll;

typedef struct VsRecorderFlipArgs {
    int flipped;
    u32 done;
} VsRecorderFlipArgs;

// video list received from the server
typedef struct VsRecorderVideoList {
    GlobalTerminalNetVideoHeader *headers[30];
    GlobalTerminalNetVideoHeader videos[30]; // copies of the headers
    volatile s32 count;
    u8 flags[30]; // flag byte of each video
    u8 unk_1B52[2];
} VsRecorderVideoList;

// server data of the work
typedef struct VsRecorderNetData {
    u8 *downloadedVideo; // profile + header + battle
    u8 videoBuffer[0x1D54 - 4];
    VsRecorderVideoList videoList;
} VsRecorderNetData;

// battle video info panel on the top screen: owner, battle mode, video number, party icons
typedef struct VsRecorderVideoInfo {
    const u16 *ownerName;
    void *header; // battle record header
    u8 unk_08[4];
    ManagedSprite *monIcons[12];
    Window windows[5];
} VsRecorderVideoInfo;

// message screen (VS_RECORDER_SCREEN_MESSAGE)
typedef struct VsRecorderMessageScreen {
    u32 savedSubState;
    u32 savedStep;
    int returnScreen;
    int msgId;
    int step;
    int timer;
    BOOL active;
} VsRecorderMessageScreen;

// ring shown at a confirmed touch (one of the orbit cursors)
typedef struct VsRecorderTouchIndicator {
    BOOL active;
    int counter;
    struct OrbitCursor *cursor;
    SysTask *task;
} VsRecorderTouchIndicator;

// VsRecorderWork.screenWork of the saved video screen
typedef struct VsRecorderSavedVideo {
    int eva; // top screen blend
    int evb;
    int fade;
    u32 showProfile; // top screen: 0 video info, 1 owner's profile
    VsRecorderProfile profile;
    VsRecorderVideoInfo info;
    u8 unk_128[0x30];
    VsRecorderTextSprite exitButton;
    TouchHitboxController *hitboxes; // profile, play, exit
    Window windows[2];               // profile button / YES, NO
    u32 exitMode;                    // save prompt: 1 YES, 2 NO; resume: 1 the video still needs saving
} VsRecorderSavedVideo;

// VsRecorderWork.screenWork of the Frontier record screen
typedef struct VsRecorderFrontierRecord {
    int fade;
    f32 scales[5]; // per print, while spinning
    int radii[5];
    int angles[5];
    ManagedSprite *playerIcons[5];   // left of each print
    ManagedSprite *facilityIcons[5]; // right of each print
    ManagedSprite *prints[5];
    ManagedSprite *digits[5]; // Battle Points: 4 digits and "BP"
    int battlePoints;
    u8 unk_94[4];
    ManagedSprite *buttonSprite;
    u8 unk_9C[0x1C];
    VsRecorderTextSprite exitButton;
    int eva; // top screen blend
    int evb;
} VsRecorderFrontierRecord;

// a dress-up photo from the server: the sender's profile and the photo
typedef struct VsRecorderDressUpDownload {
    u8 header[0x80]; // GlobalTerminalNet_CopyRequestHeader source
    GlobalTerminalNetDressUpForm form;
} VsRecorderDressUpDownload;

// VsRecorderWork.screenWork of the dress-up photo screens (send the own photo / view photos of a species)
typedef struct VsRecorderDressUp {
    int eva; // top screen blend
    int evb;
    int fade;
    BOOL showProfile; // view: 0 the photo, 1 the sender's profile
    Window caption;   // the Pokemon's name and the phrase, under the photo
    u8 unk_20[0xB0];
    Window windows[9]; // species groups; [0] is the photo / profile toggle while a photo is shown
    u8 unk_160[0x38];
    VsRecorderTextSprite exitButton;
    VsRecorderTextSprite sendButton;
    TouchHitboxController *hitboxes; // species groups
    int speciesCount;                // seen species of the chosen group
    int listSize;                    // whole species list
    u16 *speciesList;                // seen species of the group first
    MsgData *speciesNames;
    BOOL categoryChosen;
    VsRecorderListEntry *entries; // species list window
    VsRecorderListConfig listConfig;
    AccessoryPortraitConfig portraitConfig;
    AccessoryPortrait *portrait;
    SaveFashionDataSub *ownPhoto;
    int unk_230;
    int photoIndex;
    SaveFashionDataSub *photo; // unpacked downloaded photo
    VsRecorderDressUpDownload *downloads[5];
    VsRecorderProfile profile;
    FrontierCard *uploadCard;
    int species; // chosen species
    int downloadCount;
} VsRecorderDressUp;

// a box from the server: the sender's profile and the box
typedef struct VsRecorderBoxDataDownload {
    u8 header[0x80]; // GlobalTerminalNet_CopyRequestHeader source, starts with the sender's name
    GlobalTerminalNetBoxForm form;
} VsRecorderBoxDataDownload;

// VsRecorderWork.screenWork of the box data screens (send a box of the PC / view boxes by category)
typedef struct VsRecorderBoxData {
    PCStorage *storage;
    GlobalTerminalNetBoxForm form; // the shown box
    int box;                       // send: box of the PC (0-17)
    int eva;                       // top screen blend
    int evb;
    int fade;
    int cursor;        // slot of the shown Pokemon (0-29)
    Window windows[3]; // Pokemon name (box list: list title), box name, message
    u8 unk_1E4[0x50];
    Window buttons[6]; // categories; box list: profile button and heading; box: "Next Pokemon"
    u8 unk_294[0x20];
    ManagedSprite *monIcons[30];
    Pokepic *pokepic;                    // the shown Pokemon
    TouchHitboxController *hitboxes[5];  // send box, send category, view category, box list, box
    TouchHitboxController *activeHitbox; // one of hitboxes
    VsRecorderTextSprite exitButton;
    VsRecorderTextSprite sendButton;
    int category; // 0-5
    FrontierCard *card;
    int downloadCount;
    VsRecorderBoxDataDownload *downloads[20];
    VsRecorderBoxDataDownload unk_3DC[20];
    String *listItems[20][2]; // box name, sender's name
    VsRecorderListConfig listConfig;
    VsRecorderProfile profile; // sender of the chosen box
    BOOL showProfile;          // box list: 1 the sender's profile is shown
    int cursorX;               // cursor column / row in the box
    int cursorY;
} VsRecorderBoxData;

// one ranking of this week: the record and the current order of its teams (no scores yet)
typedef struct VsRecorderRankingStandings {
    u8 recordId; // msg_0026 message + 1
    u8 unk_01[3];
    u8 trainerTypes[16];
    u8 birthMonths[12];
    u16 species[20];
} VsRecorderRankingStandings;

// last week's result of a ranking: the teams in rank order with their scores
typedef struct VsRecorderRankingResults {
    u8 recordId; // msg_0026 message + 1
    u8 unk_01[3];
    u8 trainerTypes[16];
    u64 trainerTypeScores[16];
    u8 birthMonths[12];
    u64 birthMonthScores[12];
    u16 species[20];
    u64 speciesScores[20];
} VsRecorderRankingResults;

// the player's value of a ranked record, sent with the rankings request (GlobalTerminalNetPair layout)
typedef struct VsRecorderRankingScore {
    u8 recordId;
    u32 value;
} VsRecorderRankingScore;

// team kinds of a ranking (VsRecorderRankings.team)
#define VS_RECORDER_RANKING_TEAM_TRAINER_TYPE 0
#define VS_RECORDER_RANKING_TEAM_BIRTH_MONTH  1
#define VS_RECORDER_RANKING_TEAM_SPECIES      2

// VsRecorderWork.screenWork of the rankings screens (this week's rankings / last week's results)
typedef struct VsRecorderRankings {
    int eva; // top screen blend
    int evb;
    int fade;
    int ranking;      // shown ranking, SWITCH RANKINGS
    int rankingCount; // 3, 2 before flypoint flag 27 is set
    int team;         // VS_RECORDER_RANKING_TEAM_*, SWITCH TEAMS
    int teamKindCount;
    BOOL lastWeek;     // VsRecorderWork.selection
    int teamCount;     // entries of the shown team kind (16 / 12 / 20)
    Window windows[2]; // ranking title, team kind and the player's own team
    u8 unk_044[0x40];
    Window buttonLabels[2];          // SWITCH RANKINGS, SWITCH TEAMS
    VsRecorderTextSprite sendButton; // hidden
    VsRecorderTextSprite exitButton;
    TouchHitboxController *hitboxController;
    VsRecorderRankingResults resultsBuf[3]; // copy of the response (onResponse[5])
    VsRecorderRankingStandings standingsBuf[3];
    u8 recordIdsBuf[4];                    // copy of the response (onResponse[4])
    VsRecorderRankingResults *results;     // the response or resultsBuf
    VsRecorderRankingStandings *standings; // the response or standingsBuf
    u8 *recordIds;                         // records of this week's rankings
    u8 unk_720[8];
    u16 playHours;
    u8 playMinutes;
    u8 playSeconds;
    VsRecorderRankingScore scores[3]; // request
    MsgData *msgData;                 // VsRecorderWork.msgData
    String *valueTemplate;            // last week: score format of the ranking's record
    VsRecorderListEntry entries[3][20];
    VsRecorderListConfig listConfig;
} VsRecorderRankings;

struct VsRecorderWork {
    u32 mode;       // 0 VS Recorder item, 1-6 Global Terminal
    u32 subState;   // index into the screen's function table
    int step;       // step of the current substate
    int counter;    // frame / fade counter of the current step
    int *screenPtr; // OverlayManager state = current screen
    NARC *narc;     // NARC_a_1_9_1
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u32 unk_0020;
    BgConfig *bgConfig;
    PaletteData *plttData;
    TouchHitboxController *hitboxController; // menu choices
    TouchscreenHitbox menuHitboxes[5];
    u32 cursorHidden;       // 1: the title goes on to the menu's substate 0
    MsgData *msgData;       // msg_0013
    MsgData *recordMsgData; // msg_0026 (frontier record)
    FontSystem *fontSystem;
    int fade; // colour scheme change / exit fade
    u32 blendColor;
    u8 colorScheme; // Save_Misc
    GF3DVramMan *vramMan;
    PokepicManager *pokepicManager;
    GlobalTerminalNet netClient;
    VsRecorderTouchScroll touchScroll;
    VsRecorderListWindow listWindow;
    u8 unk_04EC[0x504 - 0x4EC];
    VsRecorderMessageScreen messageScreen;
    u32 unk_0520;
    u32 vblankBusy;
    u32 vblankPaused;
    VsRecorderFlipArgs flipArgs;
    VsRecorderMenuRow pathRows[5];
    VsRecorderMenuRow choiceRows[5];
    const VsRecorderMenuEntry *menuPath[5]; // entry chosen at each depth
    int menuDepth;
    u32 unk_06DC;
    int choiceCount; // rows of the current choices
    u32 menuCursor;
    u8 unk_06E8[8];
    struct OrbitCursor *orbitCursors[2]; // touch cursors
    VsRecorderDecor decor[6];
    const VsRecorderMenuEntry *menu; // current choices
    const VsRecorderMenuEntry *menuStack[5];
    SaveData *saveData;
    u16 recordSaveState;
    u16 recordWriteState;
    u32 hasResumeScreen;
    u32 resumeScreen;
    u32 unk_0840[8];
    void *screenWork;
    void *unk_0864;
    void *result; // VsRecorderAppResult of the launcher
    u32 selection;
    u32 boxUploaded; // a box was sent (only one per visit)
    u32 battleResult;
    u32 videoFlagPtr;
    void *savedVideos[4];
    FrontierCard *videoOwners[4];
    u32 menuNoticeShown; // notice raised by the menu touch
    u32 messageShown;
    Window messageWindow;
    VsRecorderNetData net;
    VsRecorderTouchIndicator touchIndicator;
    SysTask *task;
};

void VsRecorder_InitResources(VsRecorderWork *work);
void VsRecorder_FreeResources(VsRecorderWork *work);
void VsRecorder_SetScreensFlipped(int flipped);
void VsRecorder_ResetBg2(VsRecorderWork *work);
void VsRecorder_VBlankCB(void *data);
BOOL VsRecorder_Main(VsRecorderWork *work, int *screen);

void VsRecorder_SetScreenIfDone(VsRecorderWork *work, int nextScreen, BOOL done, int *screen);
void VsRecorder_SetSubState(VsRecorderWork *work, int subState);
BOOL VsRecorder_MenuEnterChild(VsRecorderWork *work);
BOOL VsRecorder_MenuRestore(VsRecorderWork *work);
void VsRecorder_MenuRunAction(VsRecorderWork *work);
void VsRecorder_OnMenuTouch(u32 index, u32 event, void *arg);
void VsRecorder_InitMenuHitboxes(VsRecorderWork *work);
void VsRecorder_UpdateMenuHitboxes(VsRecorderWork *work);
void VsRecorder_UpdateMenuTouch(VsRecorderWork *work);
void VsRecorder_MenuPop(VsRecorderWork *work);
void VsRecorder_ClearResumeState(VsRecorderWork *work);
u32 VsRecorder_HasResumeScreen(VsRecorderWork *work);
void VsRecorder_SetResumeScreen(VsRecorderWork *work, u32 screen);
void VsRecorder_LoadFrameBg(VsRecorderWork *work);
void VsRecorder_LoadMessageBg(VsRecorderWork *work, GFBgLayer layer, BOOL alternate);
void VsRecorder_LoadBoxDataMessageBg(VsRecorderWork *work, GFBgLayer layer);
void VsRecorder_LoadDecorGfx(VsRecorderWork *work);
void VsRecorder_CreateDecorSprites(VsRecorderWork *work);
void VsRecorder_DeleteDecorSprites(VsRecorderWork *work);
void VsRecorder_DestroyDecorTasks(VsRecorderWork *work);
void VsRecorder_LoadMenuRowGfx(VsRecorderWork *work);
void VsRecorder_UnloadMenuRowChars(VsRecorderWork *work);
void VsRecorder_LayoutMenuRows(VsRecorderWork *work);
void VsRecorder_CreateMenuRowSprites(VsRecorderWork *work);
void VsRecorder_RefreshChoiceRows(VsRecorderWork *work);
void VsRecorder_AttachMenuRowText(VsRecorderWork *work);
void VsRecorder_RefreshPathRows(VsRecorderWork *work);
void VsRecorder_DeleteMenuRowSprites(VsRecorderWork *work);
void VsRecorder_SetMenuRowsDrawFlag(VsRecorderWork *work, BOOL draw);
void VsRecorder_SetSpritePos(ManagedSprite *sprite, s16 x, s16 y);
void VsRecorder_GetSpritePos(ManagedSprite *sprite, s16 *x, s16 *y);

void VsRecorder_CreateMenuRowText(VsRecorderWork *work);

void VsRecorder_SetMenuRowText(VsRecorderWork *work, VsRecorderMenuRow *row, int msgId, const VsRecorderMenuEntry *entry);
void VsRecorder_FreeMenuRowText(VsRecorderWork *work);
void VsRecorder_CreateTextSprite(VsRecorderTextSprite *textSprite, VsRecorderGfx *gfx, NNS_G2D_VRAM_TYPE vram);
void VsRecorder_SetTextSpriteMsg(VsRecorderTextSprite *textSprite, VsRecorderGfx *gfx, int msgId);
void VsRecorder_FreeTextSprite(VsRecorderTextSprite *textSprite);
void VsRecorder_LoadButtonGfx31(VsRecorderWork *work, int vram);
void VsRecorder_LoadButtonGfx(VsRecorderWork *work, int vram);
void VsRecorder_LoadSlotButtonGfx(VsRecorderWork *work, int vram);
void VsRecorder_FreeButtonGfx(VsRecorderWork *work);
ManagedSprite *VsRecorder_NewButtonSprite(VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram);
void VsRecorder_FreeMenuRows(VsRecorderWork *work);
void VsRecorder_RebuildMenuRowsParent(VsRecorderWork *work);
void VsRecorder_RebuildMenuRows(VsRecorderWork *work);
void VsRecorder_InitBlend(int *eva, int *evb, int mainPlane1, int mainPlane2, int subPlane1, int subPlane2, int mode);
void VsRecorder_ApplyBlend(int *eva, int *evb, int mainPlane1, int mainPlane2, int subPlane1, int subPlane2, int mode);
void VsRecorder_InitBgBlend(int *eva, int *evb, int mode);
BOOL VsRecorder_StepBlend(int *eva, int *evb, BOOL reverse, int mode);
BOOL VsRecorder_StepFade(int *fade, int direction);
void VsRecorder_ResetFade(int *fade);
MessageFormat *VsRecorder_NewMessageFormat(enum HeapID heapID);
u32 VsRecorder_GetBlendColor(VsRecorderWork *work);
u32 VsRecorder_GetMenuBlendColor(VsRecorderWork *work);
u32 VsRecorder_GetSchemeFileId(VsRecorderWork *work, u32 kind);
void VsRecorder_ReloadSchemePalettes(VsRecorderWork *work, u32 unused);
void VsRecorder_ClearPaletteBlend(VsRecorderWork *work);
u16 *VsRecorder_LoadSpeciesList(enum HeapID heapID, int unused, int *count);
void VsRecorder_SetBoxUploaded(VsRecorderWork *work, u32 uploaded);
u32 VsRecorder_IsBoxUploaded(VsRecorderWork *work);

void VsRecorder_ShowNotice(VsRecorderWork *work, int msgId);
void VsRecorder_HideNotice(VsRecorderWork *work);
void VsRecorder_RemoveTopMessage(VsRecorderWork *work);
void VsRecorder_ShowTopMessage(VsRecorderWork *work, int msgId);
void VsRecorder_SetMessageText(VsRecorderWork *work, int msgId);
void VsRecorder_HideTopMessage(VsRecorderWork *work);
void VsRecorder_ShowMenuNotice(VsRecorderWork *work, int msgId);
u32 VsRecorder_GetAvatarFileId(int avatar, int kind);
void VsRecorder_CreateProfile(VsRecorderProfile *profile, VsRecorderWork *work);
void VsRecorder_FreeProfile(VsRecorderProfile *profile, VsRecorderWork *work);
void VsRecorder_SetProfileDrawFlag(VsRecorderProfile *profile, BOOL draw);

void VsRecorder_InitMessageList(VsRecorderListWindow *list, VsRecorderWork *work, VsRecorderListConfig *config);
void VsRecorder_InitList(VsRecorderListWindow *list, VsRecorderWork *work, MsgData *msgData, VsRecorderListConfig *config);
void VsRecorder_InitRankingList(VsRecorderListWindow *list, VsRecorderWork *work, MsgData *msgData, VsRecorderListConfig *config, int unused, int type, String *valueTemplate);
void VsRecorder_InitPairList(VsRecorderListWindow *list, VsRecorderWork *work, VsRecorderListConfig *config, String **items);
int VsRecorder_RedrawPairList(VsRecorderListWindow *list, VsRecorderWork *work, String **items);
int VsRecorder_RedrawRankingList(VsRecorderListWindow *list, VsRecorderWork *work, void *unused, u32 type, String *valueTemplate);
u32 VsRecorder_UpdateMessageList(VsRecorderListWindow *list, VsRecorderWork *work);
int VsRecorder_RedrawNameList(VsRecorderListWindow *list, VsRecorderWork *work);
void VsRecorder_ListMoveCursor(VsRecorderListWindow *list, s16 delta);
void VsRecorder_ListScroll(VsRecorderListWindow *list, s16 delta);

void VsRecorder_RemoveListWindow(VsRecorderListWindow *list);
void VsRecorder_ClearListWindow(VsRecorderListWindow *list);
void VsRecorder_CreateListArrows(VsRecorderListWindow *list, VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram);
void VsRecorder_SetListArrowsY(VsRecorderListWindow *list, s16 upY, s16 downY);
void VsRecorder_UpdateListArrows(VsRecorderListWindow *list);
void VsRecorder_UpdateListArrowsCursor(VsRecorderListWindow *list);
void VsRecorder_FreeListArrows(VsRecorderListWindow *list, VsRecorderWork *work);
void VsRecorder_CreateTouchScrollSprites(VsRecorderTouchScroll *scroll, VsRecorderWork *work);
void VsRecorder_CreateTouchScrollSpritesWithAnim(VsRecorderTouchScroll *scroll, VsRecorderWork *work, int anim);
void VsRecorder_InitTouchScroll(VsRecorderTouchScroll *scroll);
void VsRecorder_FreeTouchScroll(VsRecorderTouchScroll *scroll);
void VsRecorder_TouchScrollSetEnabled(VsRecorderTouchScroll *scroll, VsRecorderListWindow *list);
void VsRecorder_UpdateTouchScroll(VsRecorderTouchScroll *scroll);
void VsRecorder_AddGameStatsScore(VsRecorderWork *work, int score);
void VsRecorder_ImportBattleRecord(VsRecorderWork *work);
SysTask *VsRecorder_StartFlipScreens(VsRecorderWork *work, int flipped);
u32 VsRecorder_IsFlipScreensDone(VsRecorderWork *work);
void VsRecorder_StartVBlankTask(VsRecorderWork *work);
BOOL VsRecorder_StepRecordSave(VsRecorderWork *work, u32 index, u32 sealed); // TRUE when done
void VsRecorder_SetBattleResult(VsRecorderWork *work, u32 result);
void VsRecorder_OnVideoListReceived(VsRecorderWork *work, int *state);
void VsRecorder_LoadSavedVideos(VsRecorderWork *work);
void VsRecorder_FreeSavedVideos(VsRecorderWork *work);
void VsRecorder_LoadMonIconGfx(VsRecorderWork *work);
void VsRecorder_UnloadMonIconGfx(VsRecorderWork *work);
NARC *VsRecorder_OpenMonIconNarc(enum HeapID heapID);
void VsRecorder_CloseMonIconNarc(NARC *narc);
ManagedSprite *VsRecorder_NewMonIcon(VsRecorderWork *work, NARC *narc, int index, int species, int form, int isEgg);
void VsRecorder_DeleteMonIcon(VsRecorderWork *work, u32 index, ManagedSprite *sprite);
void VsRecorder_DeleteSprite(void *unused0, void *unused1, ManagedSprite *sprite);

BOOL VsRecorder_HeaderUsesThreeMonLayout(void *header);
int VsRecorderVideoInfo_GetScreenId(VsRecorderVideoInfo *info);
void VsRecorder_CreateVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work);
void VsRecorder_FreeVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work);
void VsRecorder_SetVideoPartyIconsDrawFlag(VsRecorderVideoInfo *info, BOOL draw);
int VsRecorder_GetCenteredTextX(Window *window, String *str);
void VsRecorder_ResetRecordSave(VsRecorderWork *work);
BOOL VsRecorder_ClearSavedVideoKeys(VsRecorderWork *work);
void VsRecorder_ResetBgPriorities(void);
void VsRecorder_PlaceOrbitCursor(VsRecorderWork *work, struct OrbitCursor *cursor, s16 x, s16 y);
int VsRecorder_CountDigits(u64 value);
void VsRecorder_LoadScreen(VsRecorderWork *work, int fileId, GFBgLayer layer);
void VsRecorder_OnTouchConfirm(VsRecorderWork *work);
u32 VsRecorder_IsVBlankPaused(VsRecorderWork *work);
void VsRecorder_SetVBlankPaused(VsRecorderWork *work, u32 paused);
void VsRecorder_CreateLabeledSprite(VsRecorderWork *work, VsRecorderTextSprite *textSprite, NNS_G2D_VRAM_TYPE vram, int msgId, s16 x, s16 y, s16 textX, s16 textY, int anim, BOOL draw);
void VsRecorder_FreeLabeledSprite(VsRecorderTextSprite *textSprite);
u32 VsRecorder_SetupMessageScreenMsg(VsRecorderWork *work, u32 group, u32 index, u32 type);
void VsRecorder_ShowMessageScreen(VsRecorderWork *work, int group, u32 index, u32 type);
void VsRecorder_MessageScreen(VsRecorderWork *work);
BOOL VsRecorder_IsVideoSaved(VsRecorderWork *work, u64 number);
void VsRecorder_SanitizeName(VsRecorderWork *work, String *str);
void VsRecorder_ShowTouchHere(VsRecorderWork *work);
void VsRecorder_HideTouchHere(VsRecorderWork *work);

BOOL VsRecorder_TitleIntro(VsRecorderWork *work);
BOOL VsRecorder_TitleShowMenu(VsRecorderWork *work);
BOOL VsRecorder_TitleFlash(VsRecorderWork *work);
BOOL VsRecorder_TitleSlideInChoices(VsRecorderWork *work);
BOOL VsRecorder_SlideInChoiceRows(VsRecorderWork *work);

BOOL VsRecorder_MenuMain(VsRecorderWork *work);
BOOL VsRecorder_MenuInvalid(VsRecorderWork *work);
BOOL VsRecorder_MenuExit(VsRecorderWork *work);
BOOL VsRecorder_MenuSlideIn(VsRecorderWork *work);
BOOL VsRecorder_MenuShow(VsRecorderWork *work);
BOOL VsRecorder_MenuSelect(VsRecorderWork *work);
BOOL VsRecorder_MenuBack(VsRecorderWork *work);
BOOL VsRecorder_MenuPrevScheme(VsRecorderWork *work);

BOOL VsRecorder_MenuNextScheme(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoInit(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoEnter(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoShow(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoMain(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoToggleProfile(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoSavePrompt(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoSave(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoPlay(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoExit(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoResume(VsRecorderWork *work);

void VsRecorder_FreeExitButton(VsRecorderWork *work);
void VsRecorder_PlaceFrontierExitButton(VsRecorderWork *work, BOOL left);
void VsRecorder_LoadFrontierButtonGfx(VsRecorderWork *work, u32 vram);
void VsRecorder_CreateFrontierButtons(VsRecorderWork *work);
void VsRecorder_FreeFrontierButtons(VsRecorderWork *work);
void VsRecorder_LoadFrontierGfx(VsRecorderWork *work);
void VsRecorder_GrayscalePalette(u16 *buf, int start, int count);
void VsRecorder_GrayFrontierPrints(VsRecorderWork *work);
void VsRecorder_CreateFrontierSprites(VsRecorderWork *work);
void VsRecorder_FreeFrontierSprites(VsRecorderWork *work);
BOOL VsRecorder_SpinInFrontierSprites(VsRecorderWork *work);
BOOL VsRecorder_SpinOutFrontierSprites(VsRecorderWork *work);
void VsRecorder_PlaceFrontierSprites(VsRecorderWork *work);
BOOL VsRecorder_FrontierEnter(VsRecorderWork *work);
BOOL VsRecorder_FrontierMain(VsRecorderWork *work);

BOOL VsRecorder_FrontierExit(VsRecorderWork *work);
BOOL VsRecorder_DressUpInit(VsRecorderWork *work);
BOOL VsRecorder_DressUpEnter(VsRecorderWork *work);
BOOL VsRecorder_DressUpSendShow(VsRecorderWork *work);
BOOL VsRecorder_DressUpSendMain(VsRecorderWork *work);
BOOL VsRecorder_DressUpSendUpload(VsRecorderWork *work);
BOOL VsRecorder_DressUpSendExit(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewInit(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewSelectCategory(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewSelectSpecies(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewExit(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewDownload(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewShowPhoto(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewMain(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewClosePhoto(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewToggleProfile(VsRecorderWork *work);
BOOL VsRecorder_DressUpViewChangePhoto(VsRecorderWork *work);

void VsRecorder_OnDressUpUploaded(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnDressUpListReceived(void *arg, GlobalTerminalNetError *error);
void VsRecorder_LoadBoxDataOwner(VsRecorderWork *work, int index);
void VsRecorder_FreeBoxDataOwner(VsRecorderWork *work);
void VsRecorder_FreeBoxDataButtons(VsRecorderWork *work);
void VsRecorder_SelectFirstBoxMon(VsRecorderBoxData *boxData);
BOOL VsRecorder_BoxDataInit(VsRecorderWork *work);
BOOL VsRecorder_BoxDataEnter(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendChangeBox(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendShowCategories(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendSelectCategory(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendUpload(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendCancelCategory(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendQuit(VsRecorderWork *work);
BOOL VsRecorder_BoxDataViewQuit(VsRecorderWork *work);
void VsRecorder_CreateBoxDataWindows(VsRecorderWork *work);
void VsRecorder_CreateBoxDataCategoryButtons(VsRecorderWork *work);
void VsRecorder_RemoveBoxDataCategoryButtons(VsRecorderWork *work);
void VsRecorder_RemoveBoxDataWindows(VsRecorderWork *work);
void VsRecorder_PrintBoxDataMessage(VsRecorderWork *work, int msgId);
void VsRecorder_PrintBoxDataOwnerMessage(VsRecorderWork *work, int unused);
void VsRecorder_ClearBoxDataInfoWindows(VsRecorderWork *work);
BOOL VsRecorder_HasAllBits(u32 flags, u32 mask);
void VsRecorder_SetBoxMonIconsDrawFlag(VsRecorderWork *work, BOOL draw);
void VsRecorder_FreeBoxMonIcons(VsRecorderWork *work);
void VsRecorder_SetBoxMonPicVanished(VsRecorderWork *work, BOOL vanished);
void VsRecorder_FreeBoxMonPic(VsRecorderWork *work);
void VsRecorder_ShowBox(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendShow(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendMain(VsRecorderWork *work);
BOOL VsRecorder_BoxDataSendExit(VsRecorderWork *work);
BOOL VsRecorder_BoxDataViewShow(VsRecorderWork *work);
BOOL VsRecorder_BoxDataViewSelectCategory(VsRecorderWork *work);
void VsRecorder_CreateBoxListButtons(VsRecorderWork *work);
void VsRecorder_PrintBoxListProfileButton(VsRecorderWork *work, u32 msgId);
void VsRecorder_CreateNextMonButton(VsRecorderWork *work);
void VsRecorder_RemoveNextMonButton(VsRecorderWork *work);
void VsRecorder_RemoveBoxListButtons(VsRecorderWork *work);
void VsRecorder_CreateBoxListTitle(VsRecorderWork *work);

void VsRecorder_RemoveBoxListTitle(VsRecorderWork *work);

BOOL VsRecorder_BoxDataViewDownload(VsRecorderWork *work);
BOOL VsRecorder_BoxListMain(VsRecorderWork *work);
BOOL VsRecorder_BoxListBack(VsRecorderWork *work);
BOOL VsRecorder_BoxDataViewOpenBox(VsRecorderWork *work);
BOOL VsRecorder_BoxDataViewBoxMain(VsRecorderWork *work);
BOOL VsRecorder_BoxDataViewCloseBox(VsRecorderWork *work);
BOOL VsRecorder_BoxListShowProfile(VsRecorderWork *work);
BOOL VsRecorder_BoxListHideProfile(VsRecorderWork *work);
BOOL VsRecorder_BoxDataViewExit(VsRecorderWork *work);

void VsRecorder_OnBoxUploaded(void *arg, GlobalTerminalNetError *error);
void VsRecorder_OnBoxListReceived(void *arg, GlobalTerminalNetError *error);
BOOL VsRecorder_RankingsInit(VsRecorderWork *work);
BOOL VsRecorder_RankingsEnter(VsRecorderWork *work);
BOOL VsRecorder_RankingsDownload(VsRecorderWork *work);
BOOL VsRecorder_RankingsShow(VsRecorderWork *work);
BOOL VsRecorder_RankingsMain(VsRecorderWork *work);
BOOL VsRecorder_RankingsCloseList(VsRecorderWork *work);
BOOL VsRecorder_RankingsExit(VsRecorderWork *work);
BOOL VsRecorder_RankingsInvalid(VsRecorderWork *work);
BOOL VsRecorder_RankingsSwitch(VsRecorderWork *work);

BOOL VsRecorder_IsNetIdle(VsRecorderWork *work);
GlobalTerminalNet *VsRecorder_GetNet(VsRecorderWork *work);
u32 VsRecorder_WriteRecordAsync(SaveData *save, u32 index, u32 sealed, u16 *state, void *writeState);
int VsRecorder_WriteRecord(SaveData *save, int index);
void VsRecorder_InitNet(VsRecorderWork *work);
void VsRecorder_UpdateNet(VsRecorderWork *work);
void VsRecorder_FreeNet(VsRecorderWork *work);

void VsRecorderApp_SetResult(void *result, u32 first, u32 second);
void VsRecorderApp_CountMenuChoices(VsRecorderWork *work);

#endif // POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H
