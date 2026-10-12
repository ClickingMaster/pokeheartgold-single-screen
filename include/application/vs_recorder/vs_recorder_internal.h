#ifndef POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "global_terminal_net.h"
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

// VS Recorder app (overlay 40): the VS Recorder item (mode 0) and the Global Terminal data browser (modes 1-6).
// The work is allocated by the main-ARM9 launcher (unk_02087E70.c), so only the fields known so far are named.

typedef struct VsRecorderWork VsRecorderWork;

// frontier_card.c (no public header)
typedef struct FrontierCard FrontierCard;
String *FrontierCard_NewNameString(const u16 *name, enum HeapID heapID); // the name is the card's first field
u32 FrontierCard_GetGender(FrontierCard *card);
u32 FrontierCard_GetSpecies(FrontierCard *card);
u32 sub_020315F0(FrontierCard *card);
u32 FrontierCard_IsEgg(FrontierCard *card);
u32 FrontierCard_GetCountry(FrontierCard *card);
u32 FrontierCard_GetRegion(FrontierCard *card);
String *FrontierCard_GetGreeting(FrontierCard *card, MailMessage *message, enum HeapID heapID);
u32 FrontierCard_GetBirthdayMonth(FrontierCard *card);
u32 FrontierCard_GetAvatar(FrontierCard *card);
FrontierCard *FrontierCard_New(enum HeapID heapID);
void FrontierCard_InitFromSave(FrontierCard *card, SaveData *saveData);

// battle_record.c (not in battle_recording.h)
BOOL BattleRecord_Load(SaveData *save, enum HeapID heapID, int *out, void *battle, int index);
void BattleRecord_Delete(void);
void *BattleRecord_CopyHeader(enum HeapID heapID);
FrontierCard *BattleRecord_CopyProfile(enum HeapID heapID);
void BattleRecord_Import(void *profile, void *summary, void *data, void *battle, SaveData *save);
u64 BattleRecordHeader_GetField(void *summary, int field, int index);

// one step of a screen; TRUE when the screen is done
typedef BOOL (*VsRecorderScreenFunc)(VsRecorderWork *work);

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

// one menu entry (main-ARM9 menu trees, 36 bytes)
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

// VsRecorderMenuEntry.action
#define VS_RECORDER_MENU_ACTION_CANCEL 2

// one menu row sprite with its label (left column: the path taken, right column: the current choices)
typedef struct VsRecorderMenuRow {
    ManagedSprite *sprite;
    u8 unk_04[8];
    int enabled;
    TouchscreenHitbox *hitbox;
    TextOBJ *text;
    TextOBJLayout *textLayout;
    UnkStruct_02021AC8 charAlloc;
} VsRecorderMenuRow; // size 0x28

// a sprite with a one-line text label, used by several screens
typedef struct VsRecorderTextSprite {
    u32 unk_00;
    ManagedSprite *sprite;
    TextOBJ *text;
    TextOBJLayout *textLayout;
    UnkStruct_02021AC8 charAlloc;
} VsRecorderTextSprite; // size 0x1C

// the resource block of VsRecorderWork from narc to fontSystem (0x14-0x54); the text sprite helpers get &work->narc
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
} VsRecorderDecor; // size 0x30

// trainer profile panel (frontier card) on the top screen
typedef struct VsRecorderProfile {
    FrontierCard *card;
    ManagedSprite *monIcon;
    ManagedSprite *avatar;
    Window windows[8];
} VsRecorderProfile; // size 0x8C

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
    TouchscreenHitbox *hitboxes; // one per visible row, or NULL
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
    int unk_48;
    u32 unk_4C;
} VsRecorderListWindow; // size 0x50

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
} VsRecorderTouchScroll; // size 0x20

typedef struct VsRecorderFlipArgs {
    int flipped;
    u32 done;
} VsRecorderFlipArgs;

// video list received from the server
typedef struct VsRecorderVideoList {
    GlobalTerminalNetVideoHeader *headers[30];
    GlobalTerminalNetVideoHeader videos[30]; // copies of the headers
    volatile s32 count;
    u8 flags[30]; // data[0x27] of each video
    u8 unk_1B52[2];
} VsRecorderVideoList; // size 0x1B54

// server data of the work (0x8B4)
typedef struct VsRecorderNetData {
    u8 *downloadedVideo; // profile 0x80 + header 0x64 + battle
    u8 unk_0004[0x1D54 - 4];
    VsRecorderVideoList videoList;
} VsRecorderNetData; // size 0x38A8

// battle video info panel on the top screen: owner, battle mode, video number, party icons
typedef struct VsRecorderVideoInfo {
    const u16 *ownerName;
    void *header; // battle record header
    u8 unk_08[4];
    ManagedSprite *monIcons[12];
    Window windows[5];
} VsRecorderVideoInfo; // size 0x8C

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
} VsRecorderSavedVideo;              // size 0x19C

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
    ManagedSprite *unk_98;
    u8 unk_9C[0x1C];
    VsRecorderTextSprite exitButton;
    int eva; // top screen blend
    int evb;
} VsRecorderFrontierRecord; // size 0xDC

struct VsRecorderWork {
    u32 mode;                                // 0x0000, 0 VS Recorder item, 1-6 Global Terminal
    u32 subState;                            // 0x0004, index into the screen's function table
    int step;                                // 0x0008, step of the current substate
    int counter;                             // 0x000C, frame / fade counter of the current step
    int *screenPtr;                          // 0x0010, OverlayManager state = current screen
    NARC *narc;                              // 0x0014, NARC_a_1_9_1
    SpriteSystem *spriteSystem;              // 0x0018
    SpriteManager *spriteManager;            // 0x001C
    u32 unk_0020;                            // 0x0020
    BgConfig *bgConfig;                      // 0x0024
    PaletteData *plttData;                   // 0x0028
    TouchHitboxController *hitboxController; // 0x002C, menu choices
    TouchscreenHitbox menuHitboxes[5];       // 0x0030
    u32 unk_0044;                            // 0x0044, 1: the title goes on to the menu's substate 0
    MsgData *msgData;                        // 0x0048, msg_0013
    MsgData *recordMsgData;                  // 0x004C, msg_0026 (frontier record)
    FontSystem *fontSystem;                  // 0x0050
    int fade;                                // 0x0054, colour scheme change / exit fade
    u32 blendColor;                          // 0x0058
    u8 colorScheme;                          // 0x005C, Save_Misc
    GF3DVramMan *vramMan;                    // 0x0060
    PokepicManager *pokepicManager;          // 0x0064
    u8 unk_0068[0x504 - 0x68];               // 0x0068, starts with the OVY_39 client work
    VsRecorderMessageScreen messageScreen;   // 0x0504
    u32 unk_0520;                            // 0x0520
    u32 vblankBusy;                          // 0x0524
    u32 vblankPaused;                        // 0x0528
    VsRecorderFlipArgs flipArgs;             // 0x052C
    VsRecorderMenuRow pathRows[5];           // 0x0534
    VsRecorderMenuRow choiceRows[5];         // 0x05FC
    const VsRecorderMenuEntry *menuPath[5];  // 0x06C4, entry chosen at each depth
    int menuDepth;                           // 0x06D8
    u32 unk_06DC;                            // 0x06DC
    int choiceCount;                         // 0x06E0, rows of the current choices
    u32 menuCursor;                          // 0x06E4
    u8 unk_06E8[8];                          // 0x06E8
    struct OrbitCursor *orbitCursors[2];     // 0x06F0, touch cursors (orbit_cursor.h)
    VsRecorderDecor decor[6];                // 0x06F8
    const VsRecorderMenuEntry *menu;         // 0x0818, current choices
    const VsRecorderMenuEntry *menuStack[5]; // 0x081C
    SaveData *saveData;                      // 0x0830
    u16 unk_0834;                            // 0x0834
    u16 unk_0836;                            // 0x0836
    u32 hasResumeScreen;                     // 0x0838
    u32 resumeScreen;                        // 0x083C
    u32 unk_0840[8];                         // 0x0840
    void *screenWork;                        // 0x0860
    void *unk_0864;                          // 0x0864
    void *result;                            // 0x0868, Pair87 of the launcher
    u32 selection;                           // 0x086C
    u32 unk_0870;                            // 0x0870
    u32 battleResult;                        // 0x0874
    u32 unk_0878;                            // 0x0878
    void *savedVideos[4];                    // 0x087C
    FrontierCard *videoOwners[4];            // 0x088C
    u32 menuNoticeShown;                     // 0x089C, notice raised by the menu touch
    u32 messageShown;                        // 0x08A0
    Window messageWindow;                    // 0x08A4
    VsRecorderNetData net;                   // 0x08B4
    VsRecorderTouchIndicator touchIndicator; // 0x415C
    SysTask *task;                           // 0x416C
}; // size 0x4170

// vs_recorder.c
void VsRecorder_InitResources(VsRecorderWork *work);
void VsRecorder_FreeResources(VsRecorderWork *work);
void VsRecorder_InitBgLayers(BgConfig *bgConfig);
void VsRecorder_SetScreensFlipped(int flipped);
void VsRecorder_ResetBg2(VsRecorderWork *work);
void VsRecorder_InitSpriteSystem(VsRecorderWork *work);
void VsRecorder_VBlankCB(void *data);
BOOL VsRecorder_Main(VsRecorderWork *work, int *screen);

// vs_recorder_ui.c
void VsRecorder_SetScreenIfDone(VsRecorderWork *work, int nextScreen, BOOL done, int *screen);
void VsRecorder_SetSubState(VsRecorderWork *work, int subState);
void VsRecorder_SetStep(VsRecorderWork *work, u32 step);
void VsRecorder_RefreshMenu(VsRecorderWork *work);
BOOL VsRecorder_MenuEnterChild(VsRecorderWork *work);
BOOL VsRecorder_MenuRestore(VsRecorderWork *work);
void VsRecorder_MenuRunAction(VsRecorderWork *work);
BOOL VsRecorder_MenuCanSelect(VsRecorderWork *work);
void VsRecorder_OnMenuTouch(u32 index, u32 event, void *arg);
void VsRecorder_InitMenuHitboxes(VsRecorderWork *work);
void VsRecorder_UpdateMenuHitboxes(VsRecorderWork *work);
void VsRecorder_ClearMenuHitboxes(VsRecorderWork *work);
void VsRecorder_UpdateMenuTouch(VsRecorderWork *work);
void VsRecorder_MenuPop(VsRecorderWork *work);
void VsRecorder_ClearResumeState(VsRecorderWork *work);
u32 VsRecorder_HasResumeScreen(VsRecorderWork *work);
void VsRecorder_SetResumeScreen(VsRecorderWork *work, u32 screen);
void VsRecorder_LoadFrameBg(VsRecorderWork *work);
void ov40_0222C6C8(VsRecorderWork *work, GFBgLayer layer, BOOL alternate);
void ov40_0222C710(VsRecorderWork *work, GFBgLayer layer);
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
void VsRecorder_DecorTask(SysTask *task, void *data);
void VsRecorder_SetSpritePos(ManagedSprite *sprite, s16 x, s16 y);
void VsRecorder_GetSpritePos(ManagedSprite *sprite, s16 *x, s16 *y);

// vs_recorder_row_text.c
void VsRecorder_CreateMenuRowText(VsRecorderWork *work);

// vs_recorder_gfx.c
void VsRecorder_SetMenuRowText(VsRecorderWork *work, VsRecorderMenuRow *row, int msgId, const VsRecorderMenuEntry *entry);
void VsRecorder_FreeMenuRowText(VsRecorderWork *work);
void VsRecorder_CreateTextSprite(VsRecorderTextSprite *textSprite, VsRecorderGfx *gfx, NNS_G2D_VRAM_TYPE vram);
void VsRecorder_SetTextSpriteMsg(VsRecorderTextSprite *textSprite, VsRecorderGfx *gfx, int msgId);
void VsRecorder_FreeTextSprite(VsRecorderTextSprite *textSprite);
void ov40_0222D6EC(VsRecorderWork *work, int vram);
void ov40_0222D73C(VsRecorderWork *work, int vram);
void ov40_0222D78C(VsRecorderWork *work, int vram);
void ov40_0222D7DC(VsRecorderWork *work);
ManagedSprite *ov40_0222D800(VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram);
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
u32 ov40_0222DAF0(VsRecorderWork *work);
u32 VsRecorder_GetSchemeFileId(VsRecorderWork *work, u32 kind);
void VsRecorder_ReloadSchemePalettes(VsRecorderWork *work, u32 unused);
void VsRecorder_ClearPaletteBlend(VsRecorderWork *work);
u16 *ov40_0222DD68(enum HeapID heapID, int unused, int *count);
void ov40_0222DD8C(VsRecorderWork *work, u32 value);
u32 ov40_0222DD94(VsRecorderWork *work);

// vs_recorder_windows.c
void VsRecorder_ShowNotice(VsRecorderWork *work, int msgId);
void VsRecorder_HideNotice(VsRecorderWork *work);
void VsRecorder_RemoveTopMessage(VsRecorderWork *work);
void VsRecorder_ShowTopMessage(VsRecorderWork *work, int msgId);
void VsRecorder_SetMessageText(VsRecorderWork *work, int msgId);
void VsRecorder_HideTopMessage(VsRecorderWork *work);
void VsRecorder_ShowMenuNotice(VsRecorderWork *work, int msgId);
void VsRecorder_PrintProfile(VsRecorderProfile *profile, VsRecorderWork *work);
void VsRecorder_RemoveProfileWindows(VsRecorderProfile *profile);
void VsRecorder_SetProfileMonIconDrawFlag(VsRecorderProfile *profile, BOOL draw);
void VsRecorder_FreeProfileMonIcon(VsRecorderProfile *profile, VsRecorderWork *work);
void VsRecorder_CreateProfileMonIcon(VsRecorderProfile *profile, VsRecorderWork *work);
void VsRecorder_SetProfileAvatarDrawFlag(VsRecorderProfile *profile, BOOL draw);
void VsRecorder_FreeProfileAvatar(VsRecorderProfile *profile, VsRecorderWork *work);
u32 VsRecorder_GetAvatarFileId(int avatar, int kind);
void VsRecorder_CreateProfileAvatar(VsRecorderProfile *profile, VsRecorderWork *work);
void VsRecorder_CreateProfile(VsRecorderProfile *profile, VsRecorderWork *work);
void VsRecorder_FreeProfile(VsRecorderProfile *profile, VsRecorderWork *work);
void VsRecorder_SetProfileDrawFlag(VsRecorderProfile *profile, BOOL draw);

// vs_recorder_list.c
void VsRecorder_PrintListLocation(VsRecorderListWindow *list, VsRecorderWork *work);
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

// vs_recorder_util.c
void VsRecorder_RemoveListWindow(VsRecorderListWindow *list);
void VsRecorder_ClearListWindow(VsRecorderListWindow *list);
void VsRecorder_CreateListArrows(VsRecorderListWindow *list, VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram);
void VsRecorder_SetListArrowsY(VsRecorderListWindow *list, int upY, int downY);
void VsRecorder_UpdateListArrows(VsRecorderListWindow *list);
void VsRecorder_UpdateListArrowsCursor(VsRecorderListWindow *list);
void VsRecorder_FreeListArrows(VsRecorderListWindow *list, VsRecorderWork *work);
ManagedSprite *ov40_0222F950(void *unused, VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram, int anim);
void VsRecorder_DeleteTouchScrollSprites(VsRecorderTouchScroll *scroll);
void VsRecorder_CreateTouchScrollSprites(VsRecorderTouchScroll *scroll, VsRecorderWork *work);
void VsRecorder_CreateTouchScrollSpritesWithAnim(VsRecorderTouchScroll *scroll, VsRecorderWork *work, int anim);
void VsRecorder_InitTouchScroll(VsRecorderTouchScroll *scroll);
void VsRecorder_FreeTouchScroll(VsRecorderTouchScroll *scroll);
void VsRecorder_TouchScrollTrack(VsRecorderTouchScroll *scroll, int y);
void VsRecorder_TouchScrollSetEnabled(VsRecorderTouchScroll *scroll, VsRecorderListWindow *list);
void VsRecorder_UpdateTouchScroll(VsRecorderTouchScroll *scroll);
void VsRecorder_AddGameStatsScore(VsRecorderWork *work, int score);
void VsRecorder_ImportBattleRecord(VsRecorderWork *work);
void VsRecorder_FlipScreensTask(SysTask *task, void *data);
SysTask *VsRecorder_StartFlipScreens(VsRecorderWork *work, int flipped);
u32 VsRecorder_IsFlipScreensDone(VsRecorderWork *work);
void VsRecorder_VBlankTask(SysTask *task, void *data);
void VsRecorder_StartVBlankTask(VsRecorderWork *work);
BOOL ov40_0222FC14(VsRecorderWork *work, u32 a1, u32 a2);
void VsRecorder_SetBattleResult(VsRecorderWork *work, u32 result);
void ov40_0222FC4C(VsRecorderWork *work, int *state);
void VsRecorder_LoadSavedVideos(VsRecorderWork *work);
void VsRecorder_FreeSavedVideos(VsRecorderWork *work);
void VsRecorder_LoadMonIconGfx(VsRecorderWork *work);
void VsRecorder_UnloadMonIconGfx(VsRecorderWork *work);
NARC *VsRecorder_OpenMonIconNarc(void);
void VsRecorder_CloseMonIconNarc(NARC *narc);
ManagedSprite *VsRecorder_NewMonIcon(VsRecorderWork *work, NARC *narc, int index, int species, int form, int isEgg);
void VsRecorder_DeleteMonIcon(VsRecorderWork *work, u32 index, ManagedSprite *sprite);
void VsRecorder_DeleteSprite(void *unused0, void *unused1, ManagedSprite *sprite);

// vs_recorder_screen.c
void VsRecorder_PrintVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work);
void VsRecorder_RemoveVideoInfoWindows(VsRecorderVideoInfo *info);
BOOL ov40_022303B8(void *header);
BOOL ov40_02230404(VsRecorderVideoInfo *info);
int ov40_02230410(VsRecorderVideoInfo *info);
void VsRecorder_CreateVideoPartyIcons(VsRecorderVideoInfo *info, VsRecorderWork *work);
void VsRecorder_CreateVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work);
void VsRecorder_FreeVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work);
void VsRecorder_SetVideoPartyIconsDrawFlag(VsRecorderVideoInfo *info, BOOL draw);
int VsRecorder_GetCenteredTextX(Window *window, String *str);
void ov40_022306E0(VsRecorderWork *work);
BOOL ov40_022306F0(VsRecorderWork *work);
void VsRecorder_ResetBgPriorities(void);
void VsRecorder_PlaceOrbitCursor(VsRecorderWork *work, struct OrbitCursor *cursor, s16 x, s16 y);
int VsRecorder_CountDigits(u64 value);
void VsRecorder_LoadScreen(VsRecorderWork *work, int fileId, GFBgLayer layer);
void VsRecorder_TouchIndicatorTask(SysTask *task, void *data);
void VsRecorder_ShowTouchIndicator(VsRecorderWork *work);
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
void VsRecorder_SetPlaceholderName(String *str, enum HeapID heapID);
void VsRecorder_ShowTouchHere(VsRecorderWork *work);
void VsRecorder_HideTouchHere(VsRecorderWork *work);

// vs_recorder_title.c
BOOL VsRecorder_TitleIntro(VsRecorderWork *work);
BOOL VsRecorder_TitleShowMenu(VsRecorderWork *work);
BOOL VsRecorder_TitleFlash(VsRecorderWork *work);
BOOL VsRecorder_TitleSlideInChoices(VsRecorderWork *work);
BOOL VsRecorder_SlideInChoiceRows(VsRecorderWork *work);

// vs_recorder_menu.c
BOOL VsRecorder_MenuMain(VsRecorderWork *work);
int VsRecorder_CheckSchemeArrowTouch(void);
BOOL VsRecorder_MenuInvalid(VsRecorderWork *work);
BOOL VsRecorder_MenuExit(VsRecorderWork *work);
BOOL VsRecorder_MenuSlideIn(VsRecorderWork *work);
BOOL VsRecorder_MenuShow(VsRecorderWork *work);
void VsRecorder_RowMoveTask(SysTask *task, void *data);
BOOL VsRecorder_MenuSelect(VsRecorderWork *work);
BOOL VsRecorder_MenuBack(VsRecorderWork *work);
BOOL VsRecorder_MenuPrevScheme(VsRecorderWork *work);

// vs_recorder_saved_video.c
BOOL VsRecorder_MenuNextScheme(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoInit(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoEnter(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoShow(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoMain(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoToggleProfile(VsRecorderWork *work);
void VsRecorder_ShowYesNoWindows(VsRecorderWork *work);
void VsRecorder_HideYesNoWindows(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoSavePrompt(VsRecorderWork *work);
BOOL ov40_02232A48(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoPlay(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoExit(VsRecorderWork *work);
BOOL VsRecorder_SavedVideoResume(VsRecorderWork *work);
void VsRecorder_OnSavedVideoTouch(u32 index, u32 event, void *arg);
void VsRecorder_SetSavedVideoSlot(VsRecorderWork *work);
void VsRecorder_ShowProfileButton(VsRecorderWork *work);
void VsRecorder_RemoveProfileButton(VsRecorderWork *work);
void VsRecorder_PrintProfileButton(VsRecorderWork *work, u32 msgId);
void VsRecorder_CreateExitButton(VsRecorderWork *work);

// vs_recorder_frontier_record.c
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

// other OVY_40 modules (still address-named)
GlobalTerminalNet *VsRecorder_GetNet(VsRecorderWork *work);
u32 VsRecorder_WriteRecordAsync(SaveData *save, u32 a1, u32 a2, u16 *a3, void *a4);
int VsRecorder_WriteRecord(SaveData *save, int index);
void VsRecorder_InitNet(VsRecorderWork *work);
void VsRecorder_UpdateNet(VsRecorderWork *work);
void VsRecorder_FreeNet(VsRecorderWork *work);

// main ARM9 (unk_02087E70 launcher, banner object)
void sub_02087A84(void *result, u32 first, u32 second);
void sub_02088030(VsRecorderWork *work);

#endif // POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H
