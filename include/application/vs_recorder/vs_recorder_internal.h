#ifndef POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "save.h"
#include "sprite_system.h"
#include "string.h"
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

struct VsRecorderWork {
    u32 mode;                                // 0x0000, 0 VS Recorder item, 1-6 Global Terminal
    u32 subState;                            // 0x0004, index into the screen's function table
    u32 step;                                // 0x0008
    u32 counter;                             // 0x000C
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
    u8 unk_0054[4];                          // 0x0054
    u32 blendColor;                          // 0x0058
    u8 colorScheme;                          // 0x005C, Save_Misc
    GF3DVramMan *vramMan;                    // 0x0060
    PokepicManager *pokepicManager;          // 0x0064
    u8 unk_0068[0x534 - 0x68];               // 0x0068, starts with the OVY_39 client work
    VsRecorderMenuRow pathRows[5];           // 0x0534
    VsRecorderMenuRow choiceRows[5];         // 0x05FC
    const VsRecorderMenuEntry *menuPath[5];  // 0x06C4, entry chosen at each depth
    int menuDepth;                           // 0x06D8
    u32 unk_06DC;                            // 0x06DC
    int unk_06E0;                            // 0x06E0, row layout
    u32 menuCursor;                          // 0x06E4
    u8 unk_06E8[8];                          // 0x06E8
    void *banners[2];                        // 0x06F0, top / bottom screen banner (OrbitCursor_New)
    VsRecorderDecor decor[6];                // 0x06F8
    const VsRecorderMenuEntry *menu;         // 0x0818, current choices
    const VsRecorderMenuEntry *menuStack[5]; // 0x081C
    SaveData *saveData;                      // 0x0830
    u32 unk_0834;                            // 0x0834
    u32 hasResumeScreen;                     // 0x0838
    u32 resumeScreen;                        // 0x083C
    u32 unk_0840[8];                         // 0x0840
    void *screenWork;                        // 0x0860
    void *unk_0864;                          // 0x0864
    void *result;                            // 0x0868, Pair87 of the launcher
    u32 selection;                           // 0x086C
    u32 unk_0870;                            // 0x0870
    u8 unk_0874[0x87C - 0x874];              // 0x0874
    void *savedVideos[4];                    // 0x087C
    FrontierCard *videoOwners[4];            // 0x088C
    u32 unk_089C;                            // 0x089C
    u8 unk_08A0[0x416C - 0x8A0];             // 0x08A0
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
void VsRecorder_ReloadSchemePalettes(VsRecorderWork *work);
void VsRecorder_ClearPaletteBlend(VsRecorderWork *work);
u16 *ov40_0222DD68(enum HeapID heapID, int unused, int *count);
void ov40_0222DD8C(VsRecorderWork *work, u32 value);
u32 ov40_0222DD94(VsRecorderWork *work);

// other OVY_40 modules (still address-named)
void ov40_02230DCC(VsRecorderWork *work, String *name);
void ov40_0222DD9C(VsRecorderWork *work, int msgId);
void ov40_0222DE40(VsRecorderWork *work);
void ov40_0222DFE8(VsRecorderWork *work, int msgId);
void ov40_0222FBF8(VsRecorderWork *work);
void ov40_0222FCCC(VsRecorderWork *work);
void ov40_02230D20(VsRecorderWork *work);
void ov40_0223D544(VsRecorderWork *work);
void ov40_0223D5E8(VsRecorderWork *work);
void ov40_0223D600(VsRecorderWork *work);

// main ARM9 (unk_02087E70 launcher, banner object)
void sub_02087A84(void *result, u32 first, u32 second);
BOOL OrbitCursor_SetTarget(void *banner, int x, int y);
BOOL OrbitCursor_SetPosition(void *banner, s16 x, s16 y);
BOOL OrbitCursor_SetVisible(void *banner, int flag);
void sub_02088030(VsRecorderWork *work);

#endif // POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H
