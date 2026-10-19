#include "wifi_plaza/wifi_plaza_visitor_profiles.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0801.h"
#include "wifi_plaza/wifi_plaza_internal.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "location_gmm_dat.h"
#include "math_util.h"
#include "message_format.h"
#include "model_animation.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "save_wifi_history.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sprite_transfer.h"
#include "swap_3d_buffers.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"
#include "yes_no_prompt.h"

// Wi-Fi Plaza "Visitor Profiles": the top screen shows a 3D globe with one dot per country / region; dots where a
// current Plaza visitor lives get a marker model.

#define VISITOR_PROFILES_NUM_VISITORS      50 // entries of the lobby's visitor table
#define VISITOR_PROFILES_MAX_PLACES        1024
#define VISITOR_PROFILES_NUM_MARKER_MODELS 3
#define VISITOR_PROFILES_NO_MARKER         3
#define VISITOR_PROFILES_HISTORY_SIZE      4 // ring buffer; at most 3 entries are kept
#define VISITOR_PROFILES_NUM_SHOWN_PLACES  3
#define VISITOR_PROFILES_NUM_PANEL_ROWS    18
#define VISITOR_PROFILES_NUM_BG_ANIMS      5
#define VISITOR_PROFILES_NUM_HOUR_SPRITES  12
#define VISITOR_PROFILES_NUM_BG_LAYERS     5
#define VISITOR_PROFILES_NUM_HOURS         24

// Entry of the lobby's visitor table.
typedef struct VisitorProfilesVisitor {
    u16 country;
    u8 region;
    u8 unk_03_0 : 4;
    u8 language : 4;
} VisitorProfilesVisitor;

typedef struct VisitorProfilesSettings {
    u32 fullGlobe : 1; // clear: rotation range clamped, camera near, small markers
    u32 japaneseGame : 1;
    u32 unused : 30;
    u32 unk_04;
} VisitorProfilesSettings;

// The place a dot stands for, as kept in the bottom-screen history.
typedef struct VisitorProfilesPlaceInfo {
    u32 hour; // local hour at the place
    u32 country;
    u32 region;
} VisitorProfilesPlaceInfo;

typedef struct VisitorProfilesGraphics {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    GF3DVramMan *vramMan;
    NARC *narc;
} VisitorProfilesGraphics;

typedef struct VisitorProfilesMsg {
    MsgData *msgData;
    MessageFormat *format;
    String *buffer;
    String *raw;
} VisitorProfilesMsg;

typedef struct VisitorProfilesGlobe {
    VecFx32 position;
    VecFx32 scale;
    VecFx32 rotation; // x: latitude, y: longitude (angle indices)
    ModelInstance model;
    ModelResource resource;
} VisitorProfilesGlobe;

typedef struct VisitorProfilesPlace {
    s16 latitude;
    s16 longitude;
    MtxFx33 rotation;
    s16 marker; // marker model, VISITOR_PROFILES_NO_MARKER if nobody lives there
    s16 country;
    s16 region;
} VisitorProfilesPlace;

typedef struct VisitorProfilesPlaces {
    u32 count;
    VisitorProfilesPlace places[VISITOR_PROFILES_MAX_PLACES];
    VecFx32 markerScale;
    ModelInstance markers[VISITOR_PROFILES_NUM_MARKER_MODELS];
    ModelResource markerResources[VISITOR_PROFILES_NUM_MARKER_MODELS];
} VisitorProfilesPlaces;

typedef struct VisitorProfilesCamera {
    Camera *camera;
    int unk_04;
    int unk_08;
    fx32 distance;
    u16 zoomedIn; // 1: near (distance 0x50000), 0: far (0x128000); toggled by A / a tap
    u16 zooming;
} VisitorProfilesCamera;

// Touch input; the result uses the key bit layout (PAD_BUTTON_A = tap, PAD_BUTTON_B = EXIT box, PAD_KEY_* = drag).
typedef struct VisitorProfilesTouch {
    u32 result;
    int state;
    int x;
    int y;
    u32 dragX;
    u32 dragY;
    int timer;
    Window exitButton;
} VisitorProfilesTouch;

typedef struct VisitorProfilesRow {
    u8 active;
    s8 state;
    u8 unk_02[2];
} VisitorProfilesRow;

// Animated background of one time-of-day band (5 of them), drawn into the bottom-screen place rows.
typedef struct VisitorProfilesBgAnim {
    u16 counter;
    u16 period;
    u8 enabled[VISITOR_PROFILES_NUM_SHOWN_PLACES];
    u8 unk_07;
    void *screenRaw[2];
    NNSG2dScreenData *screens[2];
    u16 numScreens;
    u16 curScreen;
    void *plttRaw;
    NNSG2dPaletteData *pltt;
    u8 numPltts;
    u8 curPltt;
    u8 plttSlot;
    u8 unk_27;
} VisitorProfilesBgAnim;

typedef struct VisitorProfilesBgAnimTemplate {
    u16 screenFiles[2];
    u16 numScreens;
    u16 plttFile;
    u16 numPltts;
    u16 plttSlot;
} VisitorProfilesBgAnimTemplate;

// Flight of the time-of-day sprite over one place row.
typedef struct VisitorProfilesHourSpriteMotion {
    s16 x;
    s16 y;
    s16 step;
    s16 phase;
    u16 joinNext;    // the next row shows the same time of day
    u16 joinPrev;    // the previous row shows the same time of day
    const s16 *path; // start x/y, end x/y, steps, amplitude, period (sVisitorProfilesHourSpritePaths)
} VisitorProfilesHourSpriteMotion;

typedef struct VisitorProfilesHourSprites {
    VisitorProfilesHourSpriteMotion motions[VISITOR_PROFILES_NUM_SHOWN_PLACES];
    s16 current;
    s16 delay;
    u8 active[4];
    u8 sprite[4];
    Sprite *sprites[VISITOR_PROFILES_NUM_HOUR_SPRITES];
    // indexed by GfGfxResType (char, pltt, cell, anim)
    SpriteResource *resources[VISITOR_PROFILES_NUM_HOUR_SPRITES][4];
} VisitorProfilesHourSprites;

// Sub-screen window 0 / window 1 vertical ranges, written on VBlank.
typedef struct VisitorProfilesWindowMask {
    s16 update0;
    s16 update1;
    s16 top0;
    s16 bottom0;
    s16 top1;
    s16 bottom1;
    SysTask *task;
} VisitorProfilesWindowMask;

// Bottom screen: the place history and its row-by-row reveal.
typedef struct VisitorProfilesPanel {
    int state;
    VisitorProfilesPlaceInfo history[VISITOR_PROFILES_HISTORY_SIZE];
    u16 head;
    u16 tail;
    u32 count;
    u16 ticks;
    u16 numRows;
    VisitorProfilesRow rows[VISITOR_PROFILES_NUM_PANEL_ROWS];
    u32 bgAnim[VISITOR_PROFILES_NUM_SHOWN_PLACES]; // background animation per shown place
    void *baseScreenRaw;
    NNSG2dScreenData *baseScreen;
    u32 unk_9C;
    VisitorProfilesBgAnim bgAnims[VISITOR_PROFILES_NUM_BG_ANIMS];
    Window placeWindows[VISITOR_PROFILES_NUM_SHOWN_PLACES];
    Window blankWindow;
    Window hintWindow;
    VisitorProfilesHourSprites hourSprites;
    VisitorProfilesWindowMask windowMask;
} VisitorProfilesPanel;

typedef struct VisitorProfilesExitPrompt {
    u32 state;
    u32 printerId;
    u32 textSpeed;
    String *string;
    Window window;
    YesNoPrompt *prompt;
} VisitorProfilesExitPrompt;

typedef struct VisitorProfilesWork {
    u16 mode;
    u16 unk_02; // the panel is updated while 0
    VisitorProfilesSettings settings;
    SaveWiFiHistory *wifiHistory;
    Options *options;
    u32 ownCountry;
    u32 ownRegion;
    VisitorProfilesClock clock;
    VisitorProfilesGraphics graphics;
    VisitorProfilesMsg msg;
    VisitorProfilesGlobe globe;
    VisitorProfilesPlaces places;
    VisitorProfilesCamera camera;
    VisitorProfilesTouch touch;
    VisitorProfilesPanel panel;
    VisitorProfilesExitPrompt exitPrompt;
} VisitorProfilesWork;

enum VisitorProfilesState {
    VISITOR_PROFILES_STATE_FADE_IN,
    VISITOR_PROFILES_STATE_WAIT_FADE_IN,
    VISITOR_PROFILES_STATE_UPDATE,
    VISITOR_PROFILES_STATE_FADE_OUT = 5,
    VISITOR_PROFILES_STATE_WAIT_FADE_OUT,
};

enum VisitorProfilesInput {
    VISITOR_PROFILES_INPUT_NONE,
    VISITOR_PROFILES_INPUT_ZOOM,
    VISITOR_PROFILES_INPUT_EXIT,
};

enum VisitorProfilesTouchValue {
    VISITOR_PROFILES_TOUCH_RESULT,
    VISITOR_PROFILES_TOUCH_DRAG_X,
    VISITOR_PROFILES_TOUCH_DRAG_Y,
};

enum VisitorProfilesTouchState {
    VISITOR_PROFILES_TOUCH_STATE_WAIT_HOLD,
    VISITOR_PROFILES_TOUCH_STATE_DRAG,
};

enum VisitorProfilesExitPromptState {
    VISITOR_PROFILES_EXIT_PROMPT_WAIT_PRINT,
    VISITOR_PROFILES_EXIT_PROMPT_HANDLE_YES_NO,
};

enum VisitorProfilesPanelState {
    VISITOR_PROFILES_PANEL_IDLE,
    VISITOR_PROFILES_PANEL_START_REVEAL,
    VISITOR_PROFILES_PANEL_REVEAL,
    VISITOR_PROFILES_PANEL_START_FAST_REVEAL,
    VISITOR_PROFILES_PANEL_FAST_REVEAL,
};

enum VisitorProfilesRowState {
    VISITOR_PROFILES_ROW_SHOW_BLANK,
    VISITOR_PROFILES_ROW_SHOW_PLACE,
    VISITOR_PROFILES_ROW_DONE,
};

enum VisitorProfilesMode {
    VISITOR_PROFILES_MODE_BROWSE,
    VISITOR_PROFILES_MODE_ZOOM,
    VISITOR_PROFILES_MODE_OPEN_EXIT_PROMPT,
    VISITOR_PROFILES_MODE_EXIT_PROMPT,
};


static void VisitorProfilesPlaces_Load(VisitorProfilesPlaces *places, void *visitors, enum HeapID heapID);
static void VisitorProfilesPlaces_Set(VisitorProfilesPlaces *places, u32 index, s32 latitude, s32 longitude, u16 country, u16 region, void *visitors);
static void VisitorProfiles_CalcGlobeMatrix(MtxFx33 *mtx, VecFx32 *rotation);
static void VisitorProfiles_CalcPlaceMatrix(MtxFx33 *mtx, VecFx32 *rotation);
static void VisitorProfiles_GetTouchDrag(s32 x, s32 y, u32 *dirX, u32 *distX, u32 *dirY, u32 *distY);
static u32 VisitorProfiles_HandleGlobeInput(VisitorProfilesWork *work, u32 trg, u32 held);
static void VisitorProfiles_WrapAngles(VecFx32 *angles);
static u32 VisitorProfiles_Update(VisitorProfilesWork *work);
static void VisitorProfiles_UpdatePanel(VisitorProfilesWork *work);
static void VisitorProfiles_Draw(VisitorProfilesWork *work);
static void VisitorProfiles_VBlankCB(void *data);
static u32 VisitorProfiles_HandleInput(VisitorProfilesWork *work);
static void VisitorProfiles_FocusStartPlace(VisitorProfilesWork *work, VisitorProfilesArgs *args);
static BOOL VisitorProfiles_PickPlace(VisitorProfilesWork *work, VisitorProfilesPlaceInfo *info);
static u32 VisitorProfiles_FindNearestPlace(VisitorProfilesWork *work);
static s32 VisitorProfiles_AngleDistance(VecFx32 *a, VecFx32 *b);
static int VisitorProfiles_CalcLocalHour(VisitorProfilesWork *work, u32 index, VisitorProfilesClock clock);
static BOOL VisitorProfiles_FocusPlace(VisitorProfilesWork *work, u8 country, u8 region);
static void VisitorProfiles_InitSettings(VisitorProfilesWork *work, VisitorProfilesArgs *args);
static void VisitorProfiles_InitGraphics(VisitorProfilesGraphics *gfx, Options *options, enum HeapID heapID);
static void VisitorProfiles_FreeGraphics(VisitorProfilesGraphics *gfx);
static void VisitorProfiles_DrawSprites(VisitorProfilesGraphics *gfx);
static void VisitorProfiles_GraphicsVBlank(VisitorProfilesGraphics *gfx);
static void VisitorProfiles_InitBgs(VisitorProfilesGraphics *gfx, Options *options, enum HeapID heapID);
static void VisitorProfiles_FreeBgs(VisitorProfilesGraphics *gfx);
static void VisitorProfiles_InitSprites(VisitorProfilesGraphics *gfx, enum HeapID heapID);
static void VisitorProfiles_FreeSprites(VisitorProfilesGraphics *gfx);
static void VisitorProfiles_Init3D(VisitorProfilesGraphics *gfx, enum HeapID heapID);
static void VisitorProfiles_Free3D(VisitorProfilesGraphics *gfx);
static void VisitorProfiles_Setup3D(void);
static void VisitorProfilesGlobe_Init(VisitorProfilesGlobe *globe, VisitorProfilesGraphics *gfx, enum HeapID heapID);
static void VisitorProfilesGlobe_Free(VisitorProfilesGlobe *globe);
static void VisitorProfilesGlobe_Draw(VisitorProfilesGlobe *globe);
static void VisitorProfilesGlobe_GetPosition(VisitorProfilesGlobe *globe, VecFx32 *dst);
static void VisitorProfilesGlobe_GetMatrix(VisitorProfilesGlobe *globe, MtxFx33 *mtx);
static void VisitorProfilesGlobe_GetRotation(VisitorProfilesGlobe *globe, VecFx32 *dst);
static void VisitorProfilesGlobe_SetRotation(VisitorProfilesGlobe *globe, VecFx32 *src);
static void VisitorProfilesPlaces_Init(VisitorProfilesPlaces *places, void *visitors, VisitorProfilesGraphics *gfx, VisitorProfilesSettings settings, volatile enum HeapID heapID);
static void VisitorProfilesPlaces_Free(VisitorProfilesPlaces *places);
static void VisitorProfilesPlaces_Draw(VisitorProfilesPlaces *places, VisitorProfilesGlobe *globe);
static int VisitorProfiles_GetVisitorMarker(void *visitors, int country, int region);
static u16 VisitorProfilesPlaces_Find(VisitorProfilesPlaces *places, u16 country, u16 region, VecFx32 *coords);
static u32 VisitorProfilesPlaces_FindIndex(VisitorProfilesPlaces *places, int country, int region);
static void VisitorProfilesPlaces_GetCoords(VisitorProfilesPlaces *places, VecFx32 *coords, u32 index);
static u16 VisitorProfilesPlaces_GetMarker(VisitorProfilesPlaces *places, u32 index);
static u16 VisitorProfilesPlaces_GetCountry(VisitorProfilesPlaces *places, u32 index);
static u16 VisitorProfilesPlaces_GetRegion(VisitorProfilesPlaces *places, u32 index);
static u32 VisitorProfilesPlaces_GetCount(VisitorProfilesPlaces *places);
static void VisitorProfilesCamera_Init(VisitorProfilesCamera *camera, VisitorProfilesSettings settings, enum HeapID heapID);
static void VisitorProfilesCamera_Free(VisitorProfilesCamera *camera);
static void VisitorProfilesCamera_Apply(VisitorProfilesCamera *camera);
static void VisitorProfilesCamera_ToggleZoom(VisitorProfilesCamera *camera);
static int VisitorProfilesCamera_UpdateZoom(VisitorProfilesCamera *camera, VisitorProfilesPlaces *places);
static u16 VisitorProfilesCamera_GetZoomMode(VisitorProfilesCamera *camera);
static void VisitorProfilesTouch_Init(VisitorProfilesTouch *touch, VisitorProfilesGraphics *gfx, VisitorProfilesMsg *msg, enum HeapID heapID);
static void VisitorProfilesTouch_Free(VisitorProfilesTouch *touch);
static void VisitorProfilesTouch_Update(VisitorProfilesTouch *touch);
static u32 VisitorProfilesTouch_Get(VisitorProfilesTouch *touch, s32 which);
static void VisitorProfilesTouch_HideExitButton(VisitorProfilesTouch *touch);
static void VisitorProfilesTouch_ShowExitButton(VisitorProfilesTouch *touch);
static void VisitorProfilesExitPrompt_Init(VisitorProfilesExitPrompt *prompt, VisitorProfilesGraphics *gfx, VisitorProfilesMsg *msg, SaveData *saveData, enum HeapID heapID);
static void VisitorProfilesExitPrompt_Free(VisitorProfilesExitPrompt *prompt);
static void VisitorProfilesExitPrompt_Open(VisitorProfilesExitPrompt *prompt);
static int VisitorProfilesExitPrompt_HandleInput(VisitorProfilesExitPrompt *prompt);
static void VisitorProfilesExitPrompt_Close(VisitorProfilesExitPrompt *prompt);
static void VisitorProfilesPanel_Init(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx, VisitorProfilesMsg *msg, enum HeapID heapID);
static void VisitorProfilesPanel_Free(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx);
static void VisitorProfilesPanel_Update(VisitorProfilesPanel *panel, VisitorProfilesMsg *msg, VisitorProfilesGraphics *gfx);
static void VisitorProfilesPanel_HideHint(VisitorProfilesPanel *panel);
static void VisitorProfilesPanel_ShowHint(VisitorProfilesPanel *panel);
static BOOL VisitorProfilesPanel_AddPlace(VisitorProfilesPanel *panel, VisitorProfilesPlaceInfo *info);
static BOOL VisitorProfilesPanel_HasPlace(VisitorProfilesPanel *panel, VisitorProfilesPlaceInfo *info);
static void VisitorProfilesPanel_InitHistory(VisitorProfilesPanel *panel);
static void VisitorProfilesPanel_ClearHistory(VisitorProfilesPanel *panel);
static void VisitorProfilesPanel_PushHistory(VisitorProfilesPanel *panel, const VisitorProfilesPlaceInfo *info);
static void VisitorProfilesPanel_PopHistory(VisitorProfilesPanel *panel);
static BOOL VisitorProfilesPanel_GetHistory(VisitorProfilesPanel *panel, VisitorProfilesPlaceInfo *info, u32 index);
static void VisitorProfilesPanel_AnimateBgs(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx);
static void VisitorProfilesPanel_StartReveal(VisitorProfilesPanel *panel, VisitorProfilesMsg *msg);
static BOOL VisitorProfilesPanel_RevealSlow(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx);
static void VisitorProfilesPanel_HalveRevealTicks(VisitorProfilesPanel *panel, VisitorProfilesMsg *msg, VisitorProfilesGraphics *gfx);
static BOOL VisitorProfilesPanel_RevealFast(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx);
static BOOL VisitorProfilesPanel_Reveal(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx, int ticksPerRow);
static void VisitorProfilesPanel_ActivateRow(VisitorProfilesPanel *panel, s32 row);
static BOOL VisitorProfilesPanel_UpdateRow(VisitorProfilesPanel *panel, int row, VisitorProfilesGraphics *gfx);
static void VisitorProfilesPanel_CopyBaseRow(VisitorProfilesPanel *panel, int row, VisitorProfilesGraphics *gfx);
static void VisitorProfilesWindowMask_Init(VisitorProfilesWindowMask *mask);
static void VisitorProfilesWindowMask_Free(VisitorProfilesWindowMask *mask);
static void VisitorProfilesWindowMask_SetWindow0(VisitorProfilesWindowMask *mask, s32 top, s32 bottom);
static void VisitorProfilesWindowMask_SetWindow1(VisitorProfilesWindowMask *mask, s32 top, s32 bottom);
static inline void VisitorProfilesWindowMask_SetWindow(u32 top, u32 bottom, vu16 *regs);
static void VisitorProfilesWindowMask_Task(SysTask *task, void *data);
static void VisitorProfilesPanel_AddWindows(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx, enum HeapID heapID);
static void VisitorProfilesPanel_RemoveWindows(VisitorProfilesPanel *panel);
static void VisitorProfilesPanel_PrintPlace(VisitorProfilesPanel *panel, u32 index, VisitorProfilesPlaceInfo *info, VisitorProfilesMsg *msg);
static void *VisitorProfilesPanel_GetRowPixels(VisitorProfilesPanel *panel, u32 index, u32 row);
static void *VisitorProfilesPanel_GetBlankRowPixels(VisitorProfilesPanel *panel, u32 row);
static void VisitorProfilesPanel_TransferRowPixels(VisitorProfilesPanel *panel, int index, int row, void *pixels);
static void VisitorProfilesBgAnim_Init(VisitorProfilesBgAnim *anim, VisitorProfilesGraphics *gfx, const VisitorProfilesBgAnimTemplate *template, int period, enum HeapID heapID);
static void VisitorProfilesBgAnim_Free(VisitorProfilesBgAnim *anim);
static void VisitorProfilesBgAnim_Update(VisitorProfilesBgAnim *anim, VisitorProfilesGraphics *gfx);
static void VisitorProfilesBgAnim_SetEnabled(VisitorProfilesBgAnim *anim, u32 index, u8 enabled);
static void VisitorProfilesBgAnim_DisableAll(VisitorProfilesBgAnim *anim);
static void VisitorProfilesBgAnim_CopyRow(VisitorProfilesBgAnim *anim, int row, VisitorProfilesGraphics *gfx);
static void VisitorProfilesHourSprites_Init(VisitorProfilesHourSprites *hourSprites, VisitorProfilesGraphics *gfx, VisitorProfilesWindowMask *mask, enum HeapID heapID);
static void VisitorProfilesHourSprites_Free(VisitorProfilesHourSprites *hourSprites, VisitorProfilesGraphics *gfx);
static void VisitorProfilesHourSprites_Reset(VisitorProfilesHourSprites *hourSprites);
static void VisitorProfilesHourSprites_Set(VisitorProfilesHourSprites *hourSprites, u8 index, u8 hour, u8 nextHour);
static void VisitorProfilesHourSprites_HideAll(VisitorProfilesHourSprites *hourSprites);
static void VisitorProfilesHourSprites_Update(VisitorProfilesHourSprites *hourSprites, VisitorProfilesWindowMask *mask);
static BOOL VisitorProfilesHourSprites_Start(VisitorProfilesHourSprites *hourSprites, u32 index, VisitorProfilesWindowMask *mask);
static BOOL VisitorProfilesHourSprites_Step(VisitorProfilesHourSprites *hourSprites, u32 index, VisitorProfilesWindowMask *mask);
static void VisitorProfilesHourSpriteMotion_Init(VisitorProfilesHourSpriteMotion *motion, const s16 *path);
static void VisitorProfilesHourSpriteMotion_SetJoinNext(VisitorProfilesHourSpriteMotion *motion, u16 join);
static void VisitorProfilesHourSpriteMotion_SetJoinPrev(VisitorProfilesHourSpriteMotion *motion, u16 join);
static BOOL VisitorProfilesHourSpriteMotion_Step(VisitorProfilesHourSpriteMotion *motion, VisitorProfilesWindowMask *mask);
static void VisitorProfilesHourSpriteMotion_Start(VisitorProfilesHourSpriteMotion *motion, VisitorProfilesWindowMask *mask);
static void VisitorProfilesHourSpriteMotion_GetPosition(VisitorProfilesHourSpriteMotion *motion, fx32 *pos);
static void VisitorProfilesHourSpriteMotion_CalcPosition(VisitorProfilesHourSpriteMotion *motion);
static void VisitorProfilesHourSpriteMotion_UpdateWindowMask(VisitorProfilesHourSpriteMotion *motion, VisitorProfilesWindowMask *mask);
static void VisitorProfilesHourSprites_UpdateSprite(VisitorProfilesHourSprites *hourSprites, u32 index);
static u8 VisitorProfiles_GetHourBgAnim(u32 hour);
static u8 VisitorProfiles_GetHourSprite(u32 hour);
static void VisitorProfilesMsg_Init(VisitorProfilesMsg *msg, enum HeapID heapID);
static void VisitorProfilesMsg_Free(VisitorProfilesMsg *msg);
static String *VisitorProfilesMsg_Get(VisitorProfilesMsg *msg, s32 msgId);
static void VisitorProfilesMsg_Read(VisitorProfilesMsg *msg, s32 msgId, String *dest);
static String *VisitorProfilesMsg_GetCountryName(VisitorProfilesMsg *msg, u32 country);
static String *VisitorProfilesMsg_GetRegionName(VisitorProfilesMsg *msg, u32 country, u32 region);
static s32 VisitorProfiles_FindFirstVisitor(void *visitors);

fx32 FX_Sqrt(fx32 x);


static const u8 sVisitorProfilesBgAnimPeriods[VISITOR_PROFILES_NUM_BG_ANIMS] = { 30, 30, 30, 30, 30 };

static const VecFx32 sVisitorProfilesCameraPos = {
    .x = 0,
    .y = 0,
    .z = FX32_CONST(296),
};

static const u32 sVisitorProfilesMarkerModelFiles[VISITOR_PROFILES_NUM_MARKER_MODELS] = { 0, 1, 2 };

static const VecFx32 sVisitorProfilesCameraTarget = {
    .x = 0,
    .y = 0,
    .z = 0,
};

static const GraphicsModes sVisitorProfilesGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const ObjCharTransferTemplate sVisitorProfilesObjCharTransferTemplate = {
    .maxTasks = 32,
    .sizeMain = 0x4000,
    .sizeSub = 0x4000,
    .heapID = HEAP_ID_112,
};

static const YesNoPromptTemplate sVisitorProfilesYesNoTemplate = {
    .bgConfig = NULL,
    .bgId = 1,
    .tileStart = 160,
    .plttSlot = 3,
    .x = 0x19,
    .y = 6,
    .ignoreTouchFlag = 0,
    .initialCursorPos = 0,
    .shapeParam = 0,
};

static const u32 sVisitorProfilesPlaceNameColors[VISITOR_PROFILES_NUM_BG_ANIMS] = {
    MAKE_TEXT_COLOR(5, 6, 0),
    MAKE_TEXT_COLOR(7, 8, 0),
    MAKE_TEXT_COLOR(9, 10, 0),
    MAKE_TEXT_COLOR(11, 12, 0),
    MAKE_TEXT_COLOR(11, 12, 0),
};

static const u32 sVisitorProfilesBgLayers[VISITOR_PROFILES_NUM_BG_LAYERS] = { 1, 6, 7, 4, 5 };

static const MtxFx33 sVisitorProfilesIdentityMtx = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };

static const GraphicsBanks sVisitorProfilesGraphicsBanks = {
    .bg = GX_VRAM_BG_16_F,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_16_G,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_01_AB,
    .texpltt = GX_VRAM_TEXPLTT_0123_E,
};

// Background animation and sprite for each hour of the day
static const u8 sVisitorProfilesHourAnims[VISITOR_PROFILES_NUM_HOURS][2] = {
    { 0, 0  },
    { 0, 0  },
    { 0, 1  },
    { 0, 1  },
    { 0, 2  },
    { 0, 2  },
    { 1, 3  },
    { 1, 3  },
    { 1, 4  },
    { 1, 4  },
    { 1, 5  },
    { 1, 5  },
    { 2, 6  },
    { 2, 6  },
    { 2, 7  },
    { 2, 7  },
    { 3, 8  },
    { 3, 8  },
    { 3, 9  },
    { 3, 9  },
    { 4, 10 },
    { 4, 10 },
    { 4, 11 },
    { 4, 11 },
};

static const VisitorProfilesBgAnimTemplate sVisitorProfilesBgAnimTemplates[VISITOR_PROFILES_NUM_BG_ANIMS] = {
    {
     .screenFiles = { 6, 0 },
     .numScreens = 1,
     .plttFile = 13,
     .numPltts = 4,
     .plttSlot = 0,
     },
    {
     .screenFiles = { 16, 17 },
     .numScreens = 2,
     .plttFile = 0,
     .numPltts = 0,
     .plttSlot = 1,
     },
    {
     .screenFiles = { 8, 0 },
     .numScreens = 1,
     .plttFile = 14,
     .numPltts = 4,
     .plttSlot = 2,
     },
    {
     .screenFiles = { 9, 0 },
     .numScreens = 1,
     .plttFile = 15,
     .numPltts = 16,
     .plttSlot = 3,
     },
    {
     .screenFiles = { 10, 0 },
     .numScreens = 1,
     .plttFile = 15,
     .numPltts = 16,
     .plttSlot = 3,
     },
};

static const BgTemplate sVisitorProfilesBgTemplates[VISITOR_PROFILES_NUM_BG_LAYERS] = {
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0x3800,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 0,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xd000,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 2,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xe000,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 3,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xe800,
     .charBase = GX_BG_CHARBASE_0x10000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 0,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xd800,
     .charBase = GX_BG_CHARBASE_0x10000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 1,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
};

// Path of each time-of-day sprite over each shown place row: start x/y, end x/y, steps, amplitude, period
static const s16 sVisitorProfilesHourSpritePaths[VISITOR_PROFILES_NUM_HOUR_SPRITES][VISITOR_PROFILES_NUM_SHOWN_PLACES][8] = {
    {
     { 160, 48, 190, 0, 319, 8, 80, 0 },
     { 206, 96, 160, 48, 319, 8, 80, 0 },
     { 150, 144, 206, 96, 319, 8, 80, 0 },
     },
    {
     { 190, 48, 140, 0, 220, 12, 54, 0 },
     { 150, 96, 190, 48, 220, 12, 54, 0 },
     { 200, 144, 150, 96, 220, 12, 54, 0 },
     },
    {
     { 128, 48, 160, 0, 220, 12, 54, 0 },
     { 210, 96, 128, 48, 220, 12, 54, 0 },
     { 160, 144, 210, 96, 220, 12, 54, 0 },
     },
    {
     { 180, 48, 138, 0, 220, 12, 54, 0 },
     { 138, 96, 180, 48, 220, 12, 54, 0 },
     { 180, 144, 138, 96, 220, 12, 54, 0 },
     },
    {
     { 180, 48, 160, 0, 399, 4, 100, 0 },
     { 130, 96, 180, 48, 399, 4, 100, 0 },
     { 128, 144, 130, 96, 399, 4, 100, 0 },
     },
    {
     { 180, 48, 116, 0, 319, 8, 80, 0 },
     { 116, 96, 180, 48, 319, 8, 80, 0 },
     { 180, 144, 116, 96, 319, 8, 80, 0 },
     },
    {
     { 180, 48, 240, 0, 220, 12, 54, 0 },
     { 160, 96, 180, 48, 220, 12, 54, 0 },
     { 96, 144, 160, 96, 220, 12, 54, 0 },
     },
    {
     { 144, 48, 218, 0, 220, 12, 54, 0 },
     { 218, 96, 144, 48, 220, 12, 54, 0 },
     { 144, 144, 218, 96, 220, 12, 54, 0 },
     },
    {
     { 160, 48, 192, 0, 399, 4, 100, 0 },
     { 192, 96, 160, 48, 399, 4, 100, 0 },
     { 160, 144, 192, 96, 399, 4, 100, 0 },
     },
    {
     { 140, 48, 96, 0, 319, 8, 80, 0 },
     { 190, 96, 140, 48, 319, 8, 80, 0 },
     { 240, 144, 190, 96, 319, 8, 80, 0 },
     },
    {
     { 212, 48, 160, 0, 399, 4, 100, 0 },
     { 212, 96, 212, 48, 399, 4, 100, 0 },
     { 160, 144, 212, 96, 399, 4, 100, 0 },
     },
    {
     { 123, 48, 170, 0, 319, 8, 80, 0 },
     { 198, 96, 123, 48, 319, 8, 80, 0 },
     { 133, 144, 198, 96, 319, 8, 80, 0 },
     },
};

BOOL VisitorProfiles_Init(OverlayManager *man, int *state) {
    VisitorProfilesArgs *args;
    VisitorProfilesWork *work;

#pragma unused(state)
    args = OverlayManager_GetArgs(man);
    Heap_Create(HEAP_ID_3, HEAP_ID_112, 0x50000);
    work = OverlayManager_CreateAndGetData(man, sizeof(VisitorProfilesWork), HEAP_ID_112);
    memset(work, 0, sizeof(VisitorProfilesWork));
    work->wifiHistory = Save_WiFiHistory_Get(args->saveData);
    work->options = Save_PlayerData_GetOptionsAddr(args->saveData);
    work->ownCountry = WifiHistory_GetPlayerCountry(work->wifiHistory);
    work->ownRegion = WiFiHistory_GetPlayerRegion(work->wifiHistory);
    work->clock = args->clock;
    VisitorProfiles_InitSettings(work, args);
    VisitorProfiles_InitGraphics(&work->graphics, work->options, HEAP_ID_112);
    VisitorProfilesGlobe_Init(&work->globe, &work->graphics, HEAP_ID_112);
    VisitorProfilesMsg_Init(&work->msg, HEAP_ID_112);
    VisitorProfilesPlaces_Init(&work->places, args->visitors, &work->graphics, work->settings, HEAP_ID_112);
    VisitorProfilesCamera_Init(&work->camera, work->settings, HEAP_ID_112);
    VisitorProfilesTouch_Init(&work->touch, &work->graphics, &work->msg, HEAP_ID_112);
    VisitorProfilesExitPrompt_Init(&work->exitPrompt, &work->graphics, &work->msg, args->saveData, HEAP_ID_112);
    VisitorProfilesPanel_Init(&work->panel, &work->graphics, &work->msg, HEAP_ID_112);
    VisitorProfiles_FocusStartPlace(work, args);
    Main_SetVBlankIntrCB(VisitorProfiles_VBlankCB, work);
    HBlankInterruptDisable();
    return TRUE;
}

BOOL VisitorProfiles_Main(OverlayManager *man, int *state) {
    VisitorProfilesWork *work;
    VisitorProfilesArgs *args;
    u32 done;

    work = OverlayManager_GetData(man);
    args = OverlayManager_GetArgs(man);
    switch (*state) {
    case VISITOR_PROFILES_STATE_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_112);
        WiFiPlazaWork_SetLowVolume(args->lobby, 1);
        (*state)++;
        break;
    case VISITOR_PROFILES_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    case VISITOR_PROFILES_STATE_UPDATE:
        done = VisitorProfiles_Update(work);
        VisitorProfiles_UpdatePanel(work);
        if (WiFiPlazaWork_IsClosing(args->lobby) == 1) {
            WiFiPlazaWork_SetAppQuitForClosing(args->lobby);
            done = 1;
        }
        if (WiFiPlaza_HasConnectionFailed() == 1) {
            done = 1;
        }
        if (done == 1) {
            *state = VISITOR_PROFILES_STATE_FADE_OUT;
        }
        break;
    case 3:
        break;
    case 4:
        break;
    case VISITOR_PROFILES_STATE_FADE_OUT:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_112);
        (*state)++;
        break;
    case VISITOR_PROFILES_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    VisitorProfiles_Draw(work);
    return FALSE;
}

BOOL VisitorProfiles_Exit(OverlayManager *man, int *state) {
    VisitorProfilesWork *work;

#pragma unused(state)
    work = OverlayManager_GetData(man);
    OverlayManager_GetArgs(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    VisitorProfilesPanel_Free(&work->panel, &work->graphics);
    VisitorProfilesExitPrompt_Free(&work->exitPrompt);
    VisitorProfilesTouch_Free(&work->touch);
    VisitorProfilesCamera_Free(&work->camera);
    VisitorProfilesGlobe_Free(&work->globe);
    VisitorProfilesPlaces_Free(&work->places);
    VisitorProfilesMsg_Free(&work->msg);
    VisitorProfiles_FreeGraphics(&work->graphics);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_112);
    return TRUE;
}

// Earth-place records (NARC_a_0_9_8): member 0x12 lists the countries, the per-country members their regions.
typedef struct VisitorProfilesCountryRecord {
    u16 type; // 2: the country is shown through its regions
    s16 latitude;
    s16 longitude;
} VisitorProfilesCountryRecord;

typedef struct VisitorProfilesRegionRecord {
    s16 latitude;
    s16 longitude;
} VisitorProfilesRegionRecord;

static void VisitorProfilesPlaces_Load(VisitorProfilesPlaces *places, void *visitors, enum HeapID heapID) {
    u32 countriesSize;
    u32 regionsSize;
    NARC *narc;
    void *countries;
    void *regions;
    s32 numCountries;
    s32 num;
    s32 i;
    s32 j;
    const VisitorProfilesCountryRecord *country;
    const VisitorProfilesRegionRecord *region;

    narc = NARC_New(NARC_a_0_9_8, heapID);
    places->count = 0;
    countries = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, 0x12, FALSE, heapID, FALSE, &countriesSize);
    num = countriesSize / sizeof(VisitorProfilesCountryRecord);
    i = 1;
    country = (const VisitorProfilesCountryRecord *)countries + 1;
    for (; i < num; i++) {
        if (country->type != 2) {
            VisitorProfilesPlaces_Set(places, places->count, country->latitude, country->longitude, (u16)i, 0, visitors);
            places->count++;
        }
        country++;
    }
    Heap_Free(countries);
    i = 1;
    numCountries = LocationGmmDatCountGet();
    for (; i < numCountries; i++) {
        regions = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, LocationGmmDatGetEarthPlaceDatId(i), FALSE, heapID, FALSE, &regionsSize);
        num = regionsSize / sizeof(VisitorProfilesRegionRecord);
        j = 1;
        region = (const VisitorProfilesRegionRecord *)regions + 1;
        for (; j < num; j++) {
            VisitorProfilesPlaces_Set(places, places->count, region->latitude, region->longitude, (u16)LocationGmmDatGetCountryMsgNo(i), (u16)j, visitors);
            places->count++;
            region++;
        }
        Heap_Free(regions);
    }
    NARC_Delete(narc);
}

static void VisitorProfilesPlaces_Set(VisitorProfilesPlaces *places, u32 index, s32 latitude, s32 longitude, u16 country, u16 region, void *visitors) {
    MtxFx33 mtx;
    VecFx32 rotation;

    mtx = sVisitorProfilesIdentityMtx;
    places->places[index].latitude = (s16)latitude;
    places->places[index].longitude = (s16)longitude;
    rotation.x = latitude;
    rotation.y = longitude;
    rotation.z = 0;
    VisitorProfiles_CalcPlaceMatrix(&mtx, &rotation);
    places->places[index].rotation = mtx;
    if (visitors != NULL) {
        places->places[index].marker = (s16)VisitorProfiles_GetVisitorMarker(visitors, country, region);
    }
    places->places[index].country = (s16)country;
    places->places[index].region = (s16)region;
}

static void VisitorProfiles_CalcGlobeMatrix(MtxFx33 *mtx, VecFx32 *rotation) {
    MtxFx33 tmp;

    MTX_RotY33_(mtx, FX_SinIdx((u16)rotation->y), FX_CosIdx((u16)rotation->y));
    MTX_RotX33_(&tmp, FX_SinIdx((u16)rotation->x), FX_CosIdx((u16)rotation->x));
    MTX_Concat33(mtx, &tmp, mtx);
    MTX_RotZ33_(&tmp, FX_SinIdx((u16)rotation->z), FX_CosIdx((u16)rotation->z));
    MTX_Concat33(mtx, &tmp, mtx);
}

static void VisitorProfiles_CalcPlaceMatrix(MtxFx33 *mtx, VecFx32 *rotation) {
    MtxFx33 tmp;

    MTX_RotY33_(mtx, FX_SinIdx((u16)rotation->x), FX_CosIdx((u16)rotation->x));
    MTX_RotX33_(&tmp, FX_SinIdx((u16)(-rotation->y)), FX_CosIdx((u16)(-rotation->y)));
    MTX_Concat33(mtx, &tmp, mtx);
    MTX_RotZ33_(&tmp, FX_CosIdx((u16)rotation->z), FX_SinIdx((u16)rotation->z));
    MTX_Concat33(mtx, &tmp, mtx);
}

// Turns the touch movement since (x, y) into key bits (PAD_KEY_*) and 6-bit distances.
static void VisitorProfiles_GetTouchDrag(s32 x, s32 y, u32 *dirX, u32 *distX, u32 *dirY, u32 *distY) {
    u32 keyX;
    u32 keyY;
    s32 dx;
    s32 dy;

    keyX = 0;
    keyY = 0;
    dx = 0;
    dy = 0;
    if (gSystem.touchX != 0xFFFF) {
        dx = gSystem.touchX - x;
        if (dx < 0) {
            dx = dx ^ -1;
            keyX = PAD_KEY_RIGHT;
        } else if (dx > 0) {
            keyX = PAD_KEY_LEFT;
        }
    }
    *dirX = keyX;
    *distX = dx & 0x3F;
    if (gSystem.touchY != 0xFFFF) {
        dy = gSystem.touchY - y;
        if (dy < 0) {
            dy = dy ^ -1;
            keyY = PAD_KEY_DOWN;
        } else if (dy > 0) {
            keyY = PAD_KEY_UP;
        }
    }
    *dirY = keyY;
    *distY = dy & 0x3F;
}

// Returns 1 when A / a tap asks for the zoom (only in the full globe view).
static u32 VisitorProfiles_HandleGlobeInput(VisitorProfilesWork *work, u32 trg, u32 held) {
    u16 zoomMode;
    u32 touch;
    u16 speedLongitude;
    u16 speedLatitude;
    u32 ret;
    u32 dragX;
    VecFx32 rotation;
    s16 latitude;
    u32 dragY;
    s16 longitude;

    ret = VISITOR_PROFILES_INPUT_NONE;
    VisitorProfilesGlobe_GetRotation(&work->globe, &rotation);
    touch = VisitorProfilesTouch_Get(&work->touch, VISITOR_PROFILES_TOUCH_RESULT);
    dragX = VisitorProfilesTouch_Get(&work->touch, VISITOR_PROFILES_TOUCH_DRAG_X);
    dragY = VisitorProfilesTouch_Get(&work->touch, VISITOR_PROFILES_TOUCH_DRAG_Y);
    zoomMode = VisitorProfilesCamera_GetZoomMode(&work->camera);
    latitude = rotation.x;
    longitude = rotation.y;
    if ((trg & PAD_BUTTON_A) || (touch & PAD_BUTTON_A)) {
        if (work->settings.fullGlobe == 1) {
            ret = VISITOR_PROFILES_INPUT_ZOOM;
        }
        return ret;
    }
    if (zoomMode == 0) {
        if (dragX != 0 || dragY != 0) {
            speedLongitude = dragX * 0x55;
            speedLatitude = dragY * 0x55;
        } else {
            speedLongitude = 0x200;
            speedLatitude = 0x200;
        }
    } else {
        if (dragX != 0 || dragY != 0) {
            speedLongitude = dragX * 10;
            speedLatitude = dragY * 10;
        } else {
            speedLongitude = 0x20;
            speedLatitude = 0x20;
        }
    }
    if ((held & PAD_KEY_LEFT) || (touch & PAD_KEY_LEFT)) {
        if (work->settings.fullGlobe == 1) {
            rotation.y += speedLongitude;
        } else if (longitude < -10208) {
            rotation.y += speedLongitude;
        }
    }
    if ((held & PAD_KEY_RIGHT) || (touch & PAD_KEY_RIGHT)) {
        if (work->settings.fullGlobe == 1) {
            rotation.y -= speedLongitude;
        } else if (longitude > -13184) {
            rotation.y -= speedLongitude;
        }
    }
    if ((held & PAD_KEY_UP) || (touch & PAD_KEY_UP)) {
        if (work->settings.fullGlobe == 1) {
            if (latitude + speedLatitude < 0x3E00) {
                rotation.x += speedLatitude;
            } else {
                rotation.x = 0x3E00;
            }
        } else if (latitude < 0x2020) {
            rotation.x += speedLatitude;
        }
    }
    if ((held & PAD_KEY_DOWN) || (touch & PAD_KEY_DOWN)) {
        if (work->settings.fullGlobe == 1) {
            if (latitude - speedLatitude > -0x3E00) {
                rotation.x -= speedLatitude;
            } else {
                rotation.x = -0x3E00;
            }
        } else if (latitude > 0x1300) {
            rotation.x -= speedLatitude;
        }
    }
    VisitorProfilesGlobe_SetRotation(&work->globe, &rotation);
    return 0;
}

static void VisitorProfiles_WrapAngles(VecFx32 *angles) {
    if (angles->x >= 0) {
        angles->x = angles->x % 0xFFFF;
    } else {
        angles->x = angles->x + ((angles->x >= 0 ? angles->x : -angles->x) / 0xFFFF + 1) * 0xFFFF;
    }
    if (angles->y >= 0) {
        angles->y = angles->y % 0xFFFF;
    } else {
        angles->y = angles->y + ((angles->y >= 0 ? angles->y : -angles->y) / 0xFFFF + 1) * 0xFFFF;
    }
}

// Returns 1 when the app should close.
static u32 VisitorProfiles_Update(VisitorProfilesWork *work) {
    u32 result;

    switch (work->mode) {
    case VISITOR_PROFILES_MODE_BROWSE:
        switch (VisitorProfiles_HandleInput(work)) {
        case VISITOR_PROFILES_INPUT_NONE:
            break;
        case VISITOR_PROFILES_INPUT_ZOOM:
            VisitorProfilesCamera_ToggleZoom(&work->camera);
            work->mode = VISITOR_PROFILES_MODE_ZOOM;
            break;
        case VISITOR_PROFILES_INPUT_EXIT:
            work->mode = VISITOR_PROFILES_MODE_OPEN_EXIT_PROMPT;
            break;
        }
        break;
    case VISITOR_PROFILES_MODE_ZOOM:
        if (VisitorProfilesCamera_UpdateZoom(&work->camera, &work->places) == 1) {
            work->mode = VISITOR_PROFILES_MODE_BROWSE;
        }
        break;
    case VISITOR_PROFILES_MODE_OPEN_EXIT_PROMPT:
        VisitorProfilesExitPrompt_Open(&work->exitPrompt);
        VisitorProfilesPanel_HideHint(&work->panel);
        VisitorProfilesTouch_HideExitButton(&work->touch);
        work->mode++;
        break;
    case VISITOR_PROFILES_MODE_EXIT_PROMPT:
        result = VisitorProfilesExitPrompt_HandleInput(&work->exitPrompt);
        if (result != YESNORESPONSE_WAIT) {
            if (result == YESNORESPONSE_YES) {
                goto close;
            }
            if (result != YESNORESPONSE_NO) {
                goto close;
            }
            VisitorProfilesPanel_ShowHint(&work->panel);
            VisitorProfilesExitPrompt_Close(&work->exitPrompt);
            VisitorProfilesTouch_ShowExitButton(&work->touch);
            work->mode = VISITOR_PROFILES_MODE_BROWSE;
        }
        break;
close:
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

static void VisitorProfiles_UpdatePanel(VisitorProfilesWork *work) {
    if (work->unk_02 == 0) {
        VisitorProfilesPanel_Update(&work->panel, &work->msg, &work->graphics);
    }
}

static void VisitorProfiles_Draw(VisitorProfilesWork *work) {
    Thunk_G3X_Reset();
    VisitorProfilesCamera_Apply(&work->camera);
    VisitorProfilesGlobe_Draw(&work->globe);
    VisitorProfilesPlaces_Draw(&work->places, &work->globe);
    VisitorProfiles_DrawSprites(&work->graphics);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
}

static void VisitorProfiles_VBlankCB(void *data) {
    VisitorProfilesWork *work = data;

    VisitorProfiles_GraphicsVBlank(&work->graphics);
}

// Returns 0: browsing, 1: zoom requested, 2: EXIT pressed.
static u32 VisitorProfiles_HandleInput(VisitorProfilesWork *work) {
    u32 result;
    VisitorProfilesPlaceInfo info;

    VisitorProfilesTouch_Update(&work->touch);
    if ((work->touch.result & PAD_BUTTON_B) != 0) {
        PlaySE(SEQ_SE_DP_DECIDE);
        return VISITOR_PROFILES_INPUT_EXIT;
    }

    if ((gSystem.newKeys & PAD_BUTTON_X) != 0) {
        if (VisitorProfiles_PickPlace(work, &info) == TRUE && VisitorProfilesPanel_HasPlace(&work->panel, &info) == FALSE && VisitorProfilesPanel_AddPlace(&work->panel, &info) != FALSE) {
            VisitorProfiles_FocusPlace(work, info.country, info.region);
            PlaySE(SEQ_SE_PL_TIMER04);
        }
    } else {
        result = VisitorProfiles_HandleGlobeInput(work, gSystem.newKeys, gSystem.heldKeys);
    }

    // result is left unset on the X button path
    return result;
}

// Turns the globe to the player's own place, or else to the first visitor's.
static void VisitorProfiles_FocusStartPlace(VisitorProfilesWork *work, VisitorProfilesArgs *args) {
    u32 index;
    u32 country;
    u32 region;

    if (VisitorProfiles_FocusPlace(work, work->ownCountry, work->ownRegion) != FALSE) {
        return;
    }

    index = VisitorProfiles_FindFirstVisitor(args->visitors);
    if (index == VISITOR_PROFILES_NUM_VISITORS) {
        return;
    }

    country = WiFiPlazaPlaces_GetCountry(args->visitors, index);
    region = WiFiPlazaPlaces_GetRegion(args->visitors, index);
    if (VisitorProfiles_FocusPlace(work, country, region) != TRUE) {
        GF_AssertFail();
    }
}

static BOOL VisitorProfiles_PickPlace(VisitorProfilesWork *work, VisitorProfilesPlaceInfo *info) {
    u32 index = VisitorProfiles_FindNearestPlace(work);
    if (index >= VisitorProfilesPlaces_GetCount(&work->places)) {
        return FALSE;
    }
    info->hour = VisitorProfiles_CalcLocalHour(work, index, work->clock);
    info->country = VisitorProfilesPlaces_GetCountry(&work->places, index);
    info->region = VisitorProfilesPlaces_GetRegion(&work->places, index);
    return TRUE;
}

// Index of the dot nearest to the globe centre within 0x80 in both angles, or the place count.
static u32 VisitorProfiles_FindNearestPlace(VisitorProfilesWork *work) {
    VecFx32 center;
    VecFx32 point;
    s32 maxLongitude;
    s32 wrappedMin;
    s32 wrappedMax;
    u32 count;
    u32 bestIndex;
    s32 minLatitude;
    s32 maxLatitude;
    u32 i;
    u32 marker;
    s32 minLongitude;
    s32 range;
    u32 distance;
    u32 bestDistance;

    count = VisitorProfilesPlaces_GetCount(&work->places);
    VisitorProfilesGlobe_GetRotation(&work->globe, &center);

    minLatitude = (s16)(center.x - 0x80);
    maxLatitude = (s16)(center.x + 0x80);
    minLongitude = (s16)(center.y - 0x80);
    maxLongitude = (s16)(center.y + 0x80);

    range = maxLongitude - minLongitude;
    if (range < 0) {
        range = -range;
    }
    if (range > 0x100) {
        minLongitude = (s16)center.y;
        maxLongitude = minLongitude;
        if (minLongitude > 0) {
            wrappedMin = minLongitude - 0x10000;
            wrappedMax = wrappedMin;
        } else {
            wrappedMin = minLongitude + 0x10000;
            wrappedMax = wrappedMin;
        }
        maxLongitude += 0x80;
        wrappedMin -= 0x80;
        wrappedMax += 0x80;
        minLongitude -= 0x80;
    } else {
        wrappedMin = minLongitude;
        wrappedMax = maxLongitude;
    }

    bestDistance = 0x100;
    bestIndex = count;
    for (i = 0; i < count; i++) {
        VisitorProfilesPlaces_GetCoords(&work->places, &point, i);
        marker = VisitorProfilesPlaces_GetMarker(&work->places, i);
        if (point.x > minLatitude && point.x < maxLatitude && ((point.y > minLongitude && point.y < maxLongitude) || (point.y > wrappedMin && point.y < wrappedMax)) && marker != VISITOR_PROFILES_NO_MARKER) {
            VisitorProfiles_WrapAngles(&center);
            VisitorProfiles_WrapAngles(&point);
            distance = VisitorProfiles_AngleDistance(&center, &point);
            if (distance < bestDistance) {
                bestDistance = distance;
                bestIndex = i;
            }
        }
    }
    return bestIndex;
}

static s32 VisitorProfiles_AngleDistance(VecFx32 *a, VecFx32 *b) {
    s32 dx;
    s32 dy;
    dx = a->x - b->x;
    if (dx < 0) {
        dx = -dx;
    }
    dy = a->y - b->y;
    if (dy < 0) {
        dy = -dy;
    }
    if (dx > GF_DegreeToSinCosIdxNoWrap(180)) {
        dx = 0xFFFF - dx;
    }
    if (dy > GF_DegreeToSinCosIdxNoWrap(180)) {
        dy = 0xFFFF - dy;
    }
    return FX_Sqrt((dx * dx + dy * dy) << FX32_SHIFT) >> FX32_SHIFT;
}

// Local hour at a place; the clock is 4 hours ahead of the reference place (country 219, region 3).
static int VisitorProfiles_CalcLocalHour(VisitorProfilesWork *work, u32 index, VisitorProfilesClock clock) {
    VecFx32 reference;
    VecFx32 place;
    u32 referenceIndex;
    int value = clock.hour - 4;

    referenceIndex = VisitorProfilesPlaces_FindIndex(&work->places, 219, 3);
    GF_ASSERT(referenceIndex < VisitorProfilesPlaces_GetCount(&work->places));
    VisitorProfilesPlaces_GetCoords(&work->places, &reference, referenceIndex);
    VisitorProfilesPlaces_GetCoords(&work->places, &place, index);
    value += (VISITOR_PROFILES_NUM_HOURS * (u16)(reference.y - place.y)) / 0xFFFF;
    if (value < 0) {
        value += VISITOR_PROFILES_NUM_HOURS;
    }
    if (value >= VISITOR_PROFILES_NUM_HOURS) {
        value %= VISITOR_PROFILES_NUM_HOURS;
    }
    return value;
}

static BOOL VisitorProfiles_FocusPlace(VisitorProfilesWork *work, u8 country, u8 region) {
    struct {
        VecFx32 current;
        VecFx32 next;
    } rotation;

    if (VisitorProfilesPlaces_Find(&work->places, country, region, &rotation.next) != VISITOR_PROFILES_NO_MARKER) {
        VisitorProfilesGlobe_GetRotation(&work->globe, &rotation.current);
        rotation.next.z = rotation.current.z;
        VisitorProfilesGlobe_SetRotation(&work->globe, &rotation.next);
        return TRUE;
    }
    return FALSE;
}

// Outside a Japanese game the full globe is always shown; in one only if some visitor entry has the Japanese
// language and unk_00 != 103.
static void VisitorProfiles_InitSettings(VisitorProfilesWork *work, VisitorProfilesArgs *args) {
    int i;

    work->settings.japaneseGame = 0;
    work->settings.fullGlobe = 0;
    if (gGameLanguage == LANGUAGE_JAPANESE) {
        work->settings.japaneseGame = 1;
    } else {
        work->settings.japaneseGame = 0;
    }
    if (work->settings.japaneseGame == 1) {
        for (i = 0; i < VISITOR_PROFILES_NUM_VISITORS; i++) {
            if (((VisitorProfilesVisitor *)args->visitors)[i].language == LANGUAGE_JAPANESE && ((VisitorProfilesVisitor *)args->visitors)[i].country != 103) {
                work->settings.fullGlobe = 1;
            }
        }
    } else {
        work->settings.fullGlobe = 1;
    }
}

static void VisitorProfiles_InitGraphics(VisitorProfilesGraphics *gfx, Options *options, enum HeapID heapID) {
    gfx->narc = NARC_New(NARC_a_1_9_2, heapID);
    GF_CreateVramTransferManager(0x40, heapID);
    GfGfx_SetBanks(&sVisitorProfilesGraphicsBanks);
    VisitorProfiles_InitBgs(gfx, options, heapID);
    VisitorProfiles_InitSprites(gfx, heapID);
    VisitorProfiles_Init3D(gfx, heapID);
}

static void VisitorProfiles_FreeGraphics(VisitorProfilesGraphics *gfx) {
    NARC_Delete(gfx->narc);
    GF_DestroyVramTransferManager();
    VisitorProfiles_FreeBgs(gfx);
    VisitorProfiles_FreeSprites(gfx);
    VisitorProfiles_Free3D(gfx);
}

static void VisitorProfiles_DrawSprites(VisitorProfilesGraphics *gfx) {
    SpriteList_RenderAndAnimateSprites(gfx->spriteList);
}

static void VisitorProfiles_GraphicsVBlank(VisitorProfilesGraphics *gfx) {
    DoScheduledBgGpuUpdates(gfx->bgConfig);
    OamManager_ApplyAndResetBuffers();
    GF_RunVramTransferTasks();
}

static void VisitorProfiles_InitBgs(VisitorProfilesGraphics *gfx, Options *options, enum HeapID heapID) {
    volatile int i;
    int next;
    const BgTemplate *template;
    const u32 *layer;
    u8 frame;

    SetBothScreensModesAndDisable(&sVisitorProfilesGraphicsModes);
    gfx->bgConfig = BgConfig_Alloc(heapID);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    i = 0;
    template = sVisitorProfilesBgTemplates;
    layer = sVisitorProfilesBgLayers;
    do {
        InitBgFromTemplate(gfx->bgConfig, (u8)*layer, template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange((u8)*layer, 32, 0, heapID);
        BgClearTilemapBufferAndCommit(gfx->bgConfig, (u8)*layer);
        next = i + 1;
        template ++;
        i = next;
        layer++;
    } while (next < VISITOR_PROFILES_NUM_BG_LAYERS);
    GfGfxLoader_GXLoadPalFromOpenNarc(gfx->narc, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(gfx->narc, 11, gfx->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(gfx->narc, 12, gfx->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(gfx->narc, 4, gfx->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(gfx->narc, 18, gfx->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, heapID);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_11_OFFSET, heapID);
    LoadUserFrameGfx1(gfx->bgConfig, GF_BG_LYR_MAIN_1, 31, 0, 0, heapID);
    LoadUserFrameGfx1(gfx->bgConfig, GF_BG_LYR_MAIN_1, 31, 0, 0, heapID);
    frame = (u8)Options_GetFrame(options);
    LoadUserFrameGfx2(gfx->bgConfig, GF_BG_LYR_SUB_0, 10, 10, frame, heapID);
    LoadUserFrameGfx2(gfx->bgConfig, GF_BG_LYR_MAIN_1, 1, 2, frame, heapID);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, RGB(10, 22, 28));
}

static void VisitorProfiles_FreeBgs(VisitorProfilesGraphics *gfx) {
    int i;

    for (i = 0; i < VISITOR_PROFILES_NUM_BG_LAYERS; i++) {
        FreeBgTilemapBuffer(gfx->bgConfig, sVisitorProfilesBgLayers[i]);
    }
    Heap_Free(gfx->bgConfig);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
}

static void VisitorProfiles_InitSprites(VisitorProfilesGraphics *gfx, enum HeapID heapID) {
    int i;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, heapID);
    ObjCharTransfer_InitEx(&sVisitorProfilesObjCharTransferTemplate, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(0x20, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    gfx->spriteList = G2dRenderer_Init(0x20, &gfx->renderer, heapID);
    G2dRenderer_SetSubSurfaceCoords(&gfx->renderer, 0, FX32_CONST(256));
    for (i = 0; i < 4; i++) {
        gfx->resMan[i] = Create2DGfxResObjMan(0x20, (GfGfxResType)i, heapID);
    }
    WirelessStatusIcon_Create();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void VisitorProfiles_FreeSprites(VisitorProfilesGraphics *gfx) {
    s32 i;
    SpriteList_Delete(gfx->spriteList);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resMan[i]);
    }
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

static void VisitorProfiles_Init3D(VisitorProfilesGraphics *gfx, enum HeapID heapID) {
    gfx->vramMan = GF_3DVramMan_Create(heapID, GF_3D_TEXALLOC_LNK, 2, GF_3D_PLTTALLOC_LNK, 4, VisitorProfiles_Setup3D);
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0, 0, -FX16_ONE + 1);
}

static void VisitorProfiles_Free3D(VisitorProfilesGraphics *gfx) {
    GF_3DVramMan_Delete(gfx->vramMan);
}

static void VisitorProfiles_Setup3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 1;
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & ~0x3002;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & ~0x3000) | 16;
    reg_G3X_DISP3DCNT = reg_G3X_DISP3DCNT & 0xCFFB;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & ~0x3000) | 8;
    reg_G3X_DISP3DCNT = (reg_G3X_DISP3DCNT & ~0x3000) | 32;
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(RGB(26, 26, 26), 0, 0x7FFF, 63, FALSE);
    reg_G3_VIEWPORT = 0xBFFF0000;
}

static void VisitorProfilesGlobe_Init(VisitorProfilesGlobe *globe, VisitorProfilesGraphics *gfx, enum HeapID heapID) {
    globe->position.x = 0;
    globe->position.y = 0;
    globe->position.z = 0;
    globe->scale.x = FX32_ONE;
    globe->scale.y = FX32_ONE;
    globe->scale.z = FX32_ONE;
    globe->rotation.x = 0x69 << 6;
    globe->rotation.y = 0x1F << 10;
    globe->rotation.z = 0;
    ModelResource_LoadFromNarc(&globe->resource, gfx->narc, 3, heapID);
    ModelInstance_Init(&globe->model, &globe->resource);
}

static void VisitorProfilesGlobe_Free(VisitorProfilesGlobe *globe) {
    ModelResource_Free(&globe->resource);
}

static void VisitorProfilesGlobe_Draw(VisitorProfilesGlobe *globe) {
    MtxFx33 mtx;
    VisitorProfilesGlobe_GetMatrix(globe, &mtx);
    ModelInstance_SetPosition(&globe->model, globe->position.x, globe->position.y, globe->position.z);
    ModelInstance_SetScale(&globe->model, globe->scale.x, globe->scale.y, globe->scale.z);
    ModelInstance_DrawWithRotation(&globe->model, &mtx);
}

static void VisitorProfilesGlobe_GetPosition(VisitorProfilesGlobe *globe, VecFx32 *dst) {
    *dst = globe->position;
}

static void VisitorProfilesGlobe_GetMatrix(VisitorProfilesGlobe *globe, MtxFx33 *mtx) {
    VisitorProfiles_CalcGlobeMatrix(mtx, &globe->rotation);
}

static void VisitorProfilesGlobe_GetRotation(VisitorProfilesGlobe *globe, VecFx32 *dst) {
    *dst = globe->rotation;
}

static void VisitorProfilesGlobe_SetRotation(VisitorProfilesGlobe *globe, VecFx32 *src) {
    globe->rotation = *src;
}

static void VisitorProfilesPlaces_Init(VisitorProfilesPlaces *places, void *visitors, VisitorProfilesGraphics *gfx, VisitorProfilesSettings settings, volatile enum HeapID heapID) {
    int i;
    enum HeapID heap = heapID;

    heapID = heap;
    VisitorProfilesPlaces_Load(places, visitors, heap);
    for (i = 0; i < VISITOR_PROFILES_NUM_MARKER_MODELS; i++) {
        ModelResource_LoadFromNarc(&places->markerResources[i], gfx->narc, sVisitorProfilesMarkerModelFiles[i], heapID);
        ModelInstance_Init(&places->markers[i], &places->markerResources[i]);
    }
    if (settings.fullGlobe == 0) {
        places->markerScale.x = 768;
        places->markerScale.y = 768;
        places->markerScale.z = FX32_ONE;
    } else {
        places->markerScale.x = FX32_ONE;
        places->markerScale.y = FX32_ONE;
        places->markerScale.z = FX32_ONE;
    }
}

static void VisitorProfilesPlaces_Free(VisitorProfilesPlaces *places) {
    s32 i;
    for (i = 0; i < VISITOR_PROFILES_NUM_MARKER_MODELS; i++) {
        ModelResource_Free(&places->markerResources[i]);
    }
}

// Each marker model is drawn once per place that uses it (the render object keeps the last matrix of each).
static void VisitorProfilesPlaces_Draw(VisitorProfilesPlaces *places, VisitorProfilesGlobe *globe) {
    MtxFx33 result;
    MtxFx33 globeMtx;
    VecFx32 position;
    ModelInstance *marker;
    int i;
    u32 j;

    VisitorProfilesGlobe_GetPosition(globe, &position);
    VisitorProfilesGlobe_GetMatrix(globe, &globeMtx);
    for (i = 0; i < VISITOR_PROFILES_NUM_MARKER_MODELS; i++) {
        marker = &places->markers[i];
        ModelInstance_SetPosition(marker, position.x, position.y, position.z);
        if (i == 2) {
            ModelInstance_SetScale(marker, places->markerScale.x, places->markerScale.y, places->markerScale.z + 410);
        } else {
            ModelInstance_SetScale(marker, places->markerScale.x, places->markerScale.y, places->markerScale.z);
        }
    }
    MTX_Identity33(&result);
    ModelInstance_DrawWithRotation(&places->markers[2], &result);
    j = 0;
    if (j < places->count) {
        do {
            MTX_Concat33(&places->places[j].rotation, &globeMtx, &result);
            if ((u16)places->places[j].marker != VISITOR_PROFILES_NO_MARKER) {
                ModelInstance_DrawWithRotation(&places->markers[(u16)places->places[j].marker], &result);
            }
            j++;
        } while (j < places->count);
    }
}

// Marker of a place: 1 or 0 for a place where a visitor lives (by the visitor's flag), else VISITOR_PROFILES_NO_MARKER.
static int VisitorProfiles_GetVisitorMarker(void *visitors, int country, int region) {
    int i;
    for (i = 0; i < VISITOR_PROFILES_NUM_VISITORS; i++) {
        if (WiFiPlazaPlaces_IsOccupied(visitors, (u8)i)) {
            int flag = WiFiPlazaPlaces_GetValue(visitors, (u8)i);
            u8 visitorCountry = WiFiPlazaPlaces_GetCountry(visitors, (u8)i);
            int visitorRegion = WiFiPlazaPlaces_GetRegion(visitors, (u8)i);
            if (visitorCountry == country && visitorRegion == region) {
                if (flag == 1) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }
    return VISITOR_PROFILES_NO_MARKER;
}

static u16 VisitorProfilesPlaces_Find(VisitorProfilesPlaces *places, u16 country, u16 region, VecFx32 *coords) {
    u16 marker = VISITOR_PROFILES_NO_MARKER;
    s32 count;
    s32 index;
    count = VisitorProfilesPlaces_GetCount(places);
    index = VisitorProfilesPlaces_FindIndex(places, country, region);
    if (index < count) {
        marker = VisitorProfilesPlaces_GetMarker(places, index);
        VisitorProfilesPlaces_GetCoords(places, coords, index);
    }
    return marker;
}

static u32 VisitorProfilesPlaces_FindIndex(VisitorProfilesPlaces *places, int country, int region) {
    int i = 0;
    u32 count = places->count;
    for (; (u32)i < count; i++) {
        if (country == (u16)places->places[i].country && region == (u16)places->places[i].region) {
            return i;
        }
    }
    return count;
}

static void VisitorProfilesPlaces_GetCoords(VisitorProfilesPlaces *places, VecFx32 *coords, u32 index) {
    GF_ASSERT(index < places->count);
    coords->x = places->places[index].latitude;
    coords->y = places->places[index].longitude;
    coords->z = 0;
}

static u16 VisitorProfilesPlaces_GetMarker(VisitorProfilesPlaces *places, u32 index) {
    GF_ASSERT(index < places->count);
    return places->places[index].marker;
}

static u16 VisitorProfilesPlaces_GetCountry(VisitorProfilesPlaces *places, u32 index) {
    GF_ASSERT(index < places->count);
    return places->places[index].country;
}

static u16 VisitorProfilesPlaces_GetRegion(VisitorProfilesPlaces *places, u32 index) {
    GF_ASSERT(index < places->count);
    return places->places[index].region;
}

static u32 VisitorProfilesPlaces_GetCount(VisitorProfilesPlaces *places) {
    return places->count;
}

static void VisitorProfilesCamera_Init(VisitorProfilesCamera *camera, VisitorProfilesSettings settings, enum HeapID heapID) {
    fx32 distance;

    camera->camera = Camera_New(heapID);
    Camera_Init_FromTargetAndPos(&sVisitorProfilesCameraTarget, &sVisitorProfilesCameraPos, 0x5C1, 0, FALSE, camera->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(100), camera->camera);
    Camera_ApplyPerspectiveType(0, camera->camera);
    Camera_SetStaticPtr(camera->camera);
    if (settings.fullGlobe == 0) {
        camera->zoomedIn = 1;
        distance = 0x50000;
    } else {
        camera->zoomedIn = 0;
        distance = 0x128000;
    }
    *(volatile fx32 *)&camera->distance = distance;
    Camera_SetDistance(*(volatile fx32 *)&camera->distance, camera->camera);
}

static void VisitorProfilesCamera_Free(VisitorProfilesCamera *camera) {
    Camera_Delete(camera->camera);
}

static void VisitorProfilesCamera_Apply(VisitorProfilesCamera *camera) {
    Camera_PushLookAtToNNSGlb();
}

static void VisitorProfilesCamera_ToggleZoom(VisitorProfilesCamera *camera) {
    if (camera->zoomedIn == 0) {
        camera->zoomedIn = 1;
        PlaySE(SEQ_SE_PL_TIMER03);
    } else {
        camera->zoomedIn = 0;
        PlaySE(SEQ_SE_PL_TIMER03);
    }
    camera->zooming = 1;
}

// Returns 1 once the zoom is done; the marker size follows the distance.
static int VisitorProfilesCamera_UpdateZoom(VisitorProfilesCamera *camera, VisitorProfilesPlaces *places) {
    if (camera->zooming == 0) {
        return 1;
    }
    switch (camera->zoomedIn) {
    case 1:
        if (camera->distance > 0x58000) {
            camera->distance -= 0x8000;
            *(volatile fx32 *)&places->markerScale.x -= 128;
            *(volatile fx32 *)&places->markerScale.y = *(volatile fx32 *)&places->markerScale.x;
        } else {
            camera->distance = 0x50000;
            camera->zooming = 0;
        }
        break;
    case 0:
        if (camera->distance < 0x120000) {
            camera->distance += 0x8000;
            *(volatile fx32 *)&places->markerScale.x += 128;
            *(volatile fx32 *)&places->markerScale.y = *(volatile fx32 *)&places->markerScale.x;
        } else {
            camera->distance = 0x128000;
            camera->zooming = 0;
        }
        break;
    }
    Camera_SetDistance(camera->distance, camera->camera);
    return 0;
}

static u16 VisitorProfilesCamera_GetZoomMode(VisitorProfilesCamera *camera) {
    return camera->zoomedIn;
}

static void VisitorProfilesTouch_Init(VisitorProfilesTouch *touch, VisitorProfilesGraphics *gfx, VisitorProfilesMsg *msg, enum HeapID heapID) {
    String *string;
    int x;

    memset(touch, 0, sizeof(VisitorProfilesTouch));
    AddWindowParameterized(gfx->bgConfig, &touch->exitButton, GF_BG_LYR_MAIN_1, 25, 21, 6, 2, 1, 40);
    FillWindowPixelBuffer(&touch->exitButton, 15);
    string = VisitorProfilesMsg_Get(msg, msg_0801_00001);
    FontID_Alloc(2, heapID);
    x = FontID_String_GetCenterAlignmentX(2, string, 0, 48);
    AddTextPrinterParameterizedWithColor(&touch->exitButton, 2, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(2, 1, 15), NULL);
    FontID_Release(2);
    DrawFrameAndWindow1(&touch->exitButton, FALSE, 31, 0);
}

static void VisitorProfilesTouch_Free(VisitorProfilesTouch *touch) {
    RemoveWindow(&touch->exitButton);
}

static void VisitorProfilesTouch_Update(VisitorProfilesTouch *touch) {
    u32 dirX;
    u32 distX;
    u32 dirY;
    u32 distY;

    touch->result = 0;
    if (gSystem.touchNew) {
        if (gSystem.touchX >= 200 && gSystem.touchX <= 248 && gSystem.touchY >= 168 && gSystem.touchY <= 184) {
            touch->result = PAD_BUTTON_B;
            return;
        }
        touch->state = VISITOR_PROFILES_TOUCH_STATE_WAIT_HOLD;
        touch->dragX = 0;
        touch->dragY = 0;
        touch->timer = 0;
        touch->result = 0;
        touch->x = gSystem.touchX;
        touch->y = gSystem.touchY;
        touch->timer = 4;
    }
    if (gSystem.touchHeld) {
        switch (touch->state) {
        case VISITOR_PROFILES_TOUCH_STATE_WAIT_HOLD:
            if (!touch->timer) {
                touch->state++;
            } else {
                touch->timer--;
            }
        case VISITOR_PROFILES_TOUCH_STATE_DRAG:
            VisitorProfiles_GetTouchDrag(touch->x, touch->y, &dirX, &distX, &dirY, &distY);
            touch->result = dirX | dirY;
            touch->dragX = distX;
            touch->dragY = distY;
            touch->x = gSystem.touchX;
            touch->y = gSystem.touchY;
            break;
        }
    } else {
        if (touch->timer) {
            touch->result = PAD_BUTTON_A;
        }
        touch->state = VISITOR_PROFILES_TOUCH_STATE_WAIT_HOLD;
        touch->dragX = 0;
        touch->dragY = 0;
        touch->timer = 0;
    }
}

static u32 VisitorProfilesTouch_Get(VisitorProfilesTouch *touch, s32 which) {
    switch (which) {
    case VISITOR_PROFILES_TOUCH_RESULT:
        return touch->result;
    case VISITOR_PROFILES_TOUCH_DRAG_X:
        return touch->dragX;
    case VISITOR_PROFILES_TOUCH_DRAG_Y:
        return touch->dragY;
    }
}

static void VisitorProfilesTouch_HideExitButton(VisitorProfilesTouch *touch) {
    EraseFrameAndWindow1(&touch->exitButton, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&touch->exitButton);
}

static void VisitorProfilesTouch_ShowExitButton(VisitorProfilesTouch *touch) {
    ScheduleWindowCopyToVram(&touch->exitButton);
    DrawFrameAndWindow1(&touch->exitButton, FALSE, 31, 0);
}

static void VisitorProfilesExitPrompt_Init(VisitorProfilesExitPrompt *prompt, VisitorProfilesGraphics *gfx, VisitorProfilesMsg *msg, SaveData *saveData, enum HeapID heapID) {
    memset(prompt, 0, 44);
    prompt->textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
    prompt->string = String_New(128, heapID);
    AddWindowParameterized(gfx->bgConfig, &prompt->window, GF_BG_LYR_MAIN_1, 2, 1, 27, 4, 1, 52);
    FillWindowPixelBuffer(&prompt->window, 15);
    VisitorProfilesMsg_Read(msg, msg_0801_00005, prompt->string);
    prompt->prompt = YesNoPrompt_Create(heapID);
}

static void VisitorProfilesExitPrompt_Free(VisitorProfilesExitPrompt *prompt) {
    if (TextPrinterCheckActive((u8)prompt->printerId) != 0) {
        RemoveTextPrinter((u8)prompt->printerId);
    }
    String_Delete(prompt->string);
    YesNoPrompt_Destroy(prompt->prompt);
    RemoveWindow(&prompt->window);
}

static void VisitorProfilesExitPrompt_Open(VisitorProfilesExitPrompt *prompt) {
    FillWindowPixelBuffer(&prompt->window, 15);
    DrawFrameAndWindow2(&prompt->window, TRUE, 1, 2);
    ScheduleWindowCopyToVram(&prompt->window);
    prompt->printerId = AddTextPrinterParameterized(&prompt->window, 1, prompt->string, 0, 0, prompt->textSpeed, NULL);
    prompt->state = VISITOR_PROFILES_EXIT_PROMPT_WAIT_PRINT;
}

static int VisitorProfilesExitPrompt_HandleInput(VisitorProfilesExitPrompt *prompt) {
    int result;

    switch (prompt->state) {
    case VISITOR_PROFILES_EXIT_PROMPT_WAIT_PRINT: {
        YesNoPromptTemplate template;
        result = YESNORESPONSE_WAIT;
        if (TextPrinterCheckActive((u8)prompt->printerId)) {
            break;
        }
        template = sVisitorProfilesYesNoTemplate;
        template.bgConfig = GetWindowBgConfig(&prompt->window);
        YesNoPrompt_InitFromTemplate(prompt->prompt, &template);
        prompt->state++;
        break;
    }
    case VISITOR_PROFILES_EXIT_PROMPT_HANDLE_YES_NO:
        result = YesNoPrompt_HandleInput(prompt->prompt);
        break;
    }
    return result;
}

static void VisitorProfilesExitPrompt_Close(VisitorProfilesExitPrompt *prompt) {
    YesNoPrompt_Reset(prompt->prompt);
    ClearFrameAndWindow2(&prompt->window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&prompt->window);
}

static void VisitorProfilesPanel_Init(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx, VisitorProfilesMsg *msg, enum HeapID heapID) {
    int i;

    panel->state = VISITOR_PROFILES_PANEL_IDLE;
    VisitorProfilesPanel_InitHistory(panel);
    for (i = 0; i < VISITOR_PROFILES_NUM_BG_ANIMS; i++) {
        VisitorProfilesBgAnim_Init(&panel->bgAnims[i], gfx, &sVisitorProfilesBgAnimTemplates[i], sVisitorProfilesBgAnimPeriods[i], heapID);
    }
    AddWindowParameterized(gfx->bgConfig, &panel->hintWindow, GF_BG_LYR_SUB_0, 2, 19, 27, 4, 11, 40);
    FillWindowPixelBuffer(&panel->hintWindow, 15);
    AddTextPrinterParameterized(&panel->hintWindow, 1, VisitorProfilesMsg_Get(msg, msg_0801_00000), 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    DrawFrameAndWindow2(&panel->hintWindow, FALSE, 10, 10);
    VisitorProfilesPanel_AddWindows(panel, gfx, heapID);
    panel->baseScreenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, 19, FALSE, &panel->baseScreen, heapID);
    VisitorProfilesHourSprites_Init(&panel->hourSprites, gfx, &panel->windowMask, heapID);
    VisitorProfilesWindowMask_Init(&panel->windowMask);
}

static void VisitorProfilesPanel_Free(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx) {
    int i;

    VisitorProfilesWindowMask_Free(&panel->windowMask);
    VisitorProfilesHourSprites_Free(&panel->hourSprites, gfx);
    Heap_Free(panel->baseScreenRaw);
    VisitorProfilesPanel_RemoveWindows(panel);
    RemoveWindow(&panel->hintWindow);
    VisitorProfilesPanel_ClearHistory(panel);
    for (i = 0; i < VISITOR_PROFILES_NUM_BG_ANIMS; i++) {
        VisitorProfilesBgAnim_Free(&panel->bgAnims[i]);
    }
}

static void VisitorProfilesPanel_Update(VisitorProfilesPanel *panel, VisitorProfilesMsg *msg, VisitorProfilesGraphics *gfx) {
    switch (panel->state) {
    case VISITOR_PROFILES_PANEL_IDLE:
        VisitorProfilesPanel_AnimateBgs(panel, gfx);
        VisitorProfilesHourSprites_Update(&panel->hourSprites, &panel->windowMask);
        break;
    case VISITOR_PROFILES_PANEL_START_REVEAL:
        VisitorProfilesPanel_StartReveal(panel, msg);
        panel->state = VISITOR_PROFILES_PANEL_REVEAL;
        break;
    case VISITOR_PROFILES_PANEL_REVEAL:
        if (VisitorProfilesPanel_RevealSlow(panel, gfx) == TRUE) {
            panel->state = VISITOR_PROFILES_PANEL_IDLE;
        }
        break;
    case VISITOR_PROFILES_PANEL_START_FAST_REVEAL:
        VisitorProfilesPanel_HalveRevealTicks(panel, msg, gfx);
        VisitorProfilesPanel_RevealFast(panel, gfx);
        panel->state = VISITOR_PROFILES_PANEL_FAST_REVEAL;
        break;
    case VISITOR_PROFILES_PANEL_FAST_REVEAL:
        if (VisitorProfilesPanel_RevealFast(panel, gfx) == TRUE) {
            panel->state = VISITOR_PROFILES_PANEL_IDLE;
        }
        break;
    }
}

static void VisitorProfilesPanel_HideHint(VisitorProfilesPanel *panel) {
    ClearFrameAndWindow2(&panel->hintWindow, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&panel->hintWindow);
}

static void VisitorProfilesPanel_ShowHint(VisitorProfilesPanel *panel) {
    ScheduleWindowCopyToVram(&panel->hintWindow);
    DrawFrameAndWindow2(&panel->hintWindow, TRUE, 10, 10);
}

static BOOL VisitorProfilesPanel_AddPlace(VisitorProfilesPanel *panel, VisitorProfilesPlaceInfo *info) {
    switch (panel->state) {
    case VISITOR_PROFILES_PANEL_IDLE:
        panel->state = VISITOR_PROFILES_PANEL_START_REVEAL;
        break;
    case VISITOR_PROFILES_PANEL_REVEAL:
        panel->state = VISITOR_PROFILES_PANEL_START_FAST_REVEAL;
        return FALSE;
    case VISITOR_PROFILES_PANEL_START_REVEAL:
    case VISITOR_PROFILES_PANEL_START_FAST_REVEAL:
    case VISITOR_PROFILES_PANEL_FAST_REVEAL:
        return FALSE;
    }
    VisitorProfilesPanel_PushHistory(panel, info);
    return TRUE;
}

static BOOL VisitorProfilesPanel_HasPlace(VisitorProfilesPanel *panel, VisitorProfilesPlaceInfo *info) {
    VisitorProfilesPlaceInfo entry;
    s32 i;
    for (i = 0; i < VISITOR_PROFILES_NUM_SHOWN_PLACES; i++) {
        if (VisitorProfilesPanel_GetHistory(panel, &entry, i) == FALSE) {
            return FALSE;
        }
        if (entry.country == info->country && entry.region == info->region) {
            return TRUE;
        }
    }
    return FALSE;
}

static void VisitorProfilesPanel_InitHistory(VisitorProfilesPanel *panel) {
    panel->head = 0;
    panel->tail = 0;
    panel->count = 0;
}

static void VisitorProfilesPanel_ClearHistory(VisitorProfilesPanel *panel) {
    panel->head = 0;
    panel->tail = 0;
    panel->count = 0;
}

static void VisitorProfilesPanel_PushHistory(VisitorProfilesPanel *panel, const VisitorProfilesPlaceInfo *info) {
    if ((panel->tail + 1) % VISITOR_PROFILES_HISTORY_SIZE == panel->head) {
        VisitorProfilesPanel_PopHistory(panel);
    }
    panel->history[panel->tail] = *info;
    panel->tail = (panel->tail + 1) % VISITOR_PROFILES_HISTORY_SIZE;
    panel->count++;
}

static void VisitorProfilesPanel_PopHistory(VisitorProfilesPanel *panel) {
    u16 head = panel->head;
    if (panel->tail != head) {
        panel->head = (head + 1) % VISITOR_PROFILES_HISTORY_SIZE;
        panel->count = panel->count - 1;
    }
}

// index 0 is the newest entry.
static BOOL VisitorProfilesPanel_GetHistory(VisitorProfilesPanel *panel, VisitorProfilesPlaceInfo *info, u32 index) {
    s32 k;
    if (index >= panel->count) {
        return FALSE;
    }
    k = (panel->tail - 1) - index;
    if (k < 0) {
        k += VISITOR_PROFILES_HISTORY_SIZE;
    }
    *info = panel->history[k];
    return TRUE;
}

static void VisitorProfilesPanel_AnimateBgs(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx) {
    int i;
    for (i = 0; i < VISITOR_PROFILES_NUM_BG_ANIMS; i++) {
        VisitorProfilesBgAnim_Update(&panel->bgAnims[i], gfx);
    }
}

// Prints the history (newest place in the bottom slot) and starts the row-by-row reveal.
static void VisitorProfilesPanel_StartReveal(VisitorProfilesPanel *panel, VisitorProfilesMsg *msg) {
    VisitorProfilesPlaceInfo info;
    int slot;
    int i;
    int anim;
    int previousHour;
    VisitorProfilesBgAnim *bgAnim;

    memset(panel->rows, 0, sizeof(panel->rows));
    panel->ticks = 0;
    panel->numRows = 0;
    bgAnim = panel->bgAnims;
    for (i = 0; i < VISITOR_PROFILES_NUM_BG_ANIMS; i++, bgAnim++) {
        VisitorProfilesBgAnim_DisableAll(bgAnim);
    }
    VisitorProfilesHourSprites_Reset(&panel->hourSprites);
    previousHour = -1;
    for (i = 0; i < VISITOR_PROFILES_NUM_SHOWN_PLACES; i++) {
        slot = 2 - i;
        if (VisitorProfilesPanel_GetHistory(panel, &info, i) == TRUE) {
            anim = VisitorProfiles_GetHourBgAnim(info.hour);
            VisitorProfilesBgAnim_SetEnabled(&panel->bgAnims[anim], slot, 1);
            VisitorProfilesPanel_PrintPlace(panel, slot, &info, msg);
            panel->bgAnim[slot] = anim;
            VisitorProfilesHourSprites_Set(&panel->hourSprites, slot, info.hour, previousHour);
            panel->numRows += 6;
            previousHour = info.hour;
        } else {
            panel->bgAnim[slot] = VISITOR_PROFILES_NUM_BG_ANIMS;
            previousHour = -1;
        }
    }
}

static BOOL VisitorProfilesPanel_RevealSlow(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx) {
    return VisitorProfilesPanel_Reveal(panel, gfx, 2);
}

static void VisitorProfilesPanel_HalveRevealTicks(VisitorProfilesPanel *panel, VisitorProfilesMsg *msg, VisitorProfilesGraphics *gfx) {
    panel->ticks = (u32)panel->ticks >> 1;
}

static BOOL VisitorProfilesPanel_RevealFast(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx) {
    return VisitorProfilesPanel_Reveal(panel, gfx, 1);
}

// One more row (from the bottom) every ticksPerRow frames; TRUE once all rows are done.
static BOOL VisitorProfilesPanel_Reveal(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx, int ticksPerRow) {
    int done = TRUE;
    int i;
    int ticks = panel->ticks;
    if (ticks % ticksPerRow == 0) {
        VisitorProfilesPanel_ActivateRow(panel, ticks / ticksPerRow);
    }
    ticks = panel->ticks + 1;
    if (ticks < VISITOR_PROFILES_NUM_PANEL_ROWS * ticksPerRow) {
        panel->ticks = ticks;
    }
    for (i = 0; i < panel->numRows; i++) {
        if (VisitorProfilesPanel_UpdateRow(panel, i, gfx) == FALSE) {
            done = FALSE;
        }
    }
    if (done == TRUE) {
        VisitorProfilesHourSprites_HideAll(&panel->hourSprites);
        VisitorProfilesWindowMask_SetWindow1(&panel->windowMask, 0, 0);
    }
    return done;
}

static void VisitorProfilesPanel_ActivateRow(VisitorProfilesPanel *panel, s32 row) {
    panel->rows[row].active = 1;
}

// Row i counts from the bottom: first the blank row and the base screen, then the place text and its background.
static BOOL VisitorProfilesPanel_UpdateRow(VisitorProfilesPanel *panel, int row, VisitorProfilesGraphics *gfx) {
    u32 line;
    s8 *state;
    s8 rowState;
    u32 slot;
    u32 lineInSlot;
    u32 anim;

    if (!panel->rows[row].active) {
        return FALSE;
    }
    state = &panel->rows[row].state;
    rowState = *state;
    if (rowState >= 2) {
        return TRUE;
    }
    line = 17 - (u32)row;
    slot = line / 6;
    lineInSlot = line % 6;
    anim = panel->bgAnim[slot];
    switch (rowState) {
    case VISITOR_PROFILES_ROW_SHOW_BLANK:
        VisitorProfilesPanel_TransferRowPixels(panel, slot, lineInSlot, VisitorProfilesPanel_GetBlankRowPixels(panel, lineInSlot));
        VisitorProfilesPanel_CopyBaseRow(panel, line, gfx);
        VisitorProfilesWindowMask_SetWindow1(&panel->windowMask, (s16)(line * 8), 144);
        break;
    case VISITOR_PROFILES_ROW_SHOW_PLACE:
        VisitorProfilesPanel_TransferRowPixels(panel, slot, lineInSlot, VisitorProfilesPanel_GetRowPixels(panel, slot, lineInSlot));
        VisitorProfilesBgAnim_CopyRow(&panel->bgAnims[anim], line, gfx);
        break;
    }
    (*state)++;
    return FALSE;
}

static void VisitorProfilesPanel_CopyBaseRow(VisitorProfilesPanel *panel, int row, VisitorProfilesGraphics *gfx) {
    NNSG2dScreenData *screen = panel->baseScreen;

    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_SUB_2, 0, (u8)row, 32, 1, screen->rawData, 0, (u8)row, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_SUB_2);
}

static void VisitorProfilesWindowMask_Init(VisitorProfilesWindowMask *mask) {
    reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & 0xFFFF1FFF) | 0x6000;
    reg_G2S_DB_WINOUT = (reg_G2S_DB_WINOUT & ~63) | 31;
    reg_G2S_DB_WININ = (reg_G2S_DB_WININ & ~63) | 15;
    reg_G2S_DB_WININ = (reg_G2S_DB_WININ & 0xFFFFC0FF) | 0xF00;
    VisitorProfilesWindowMask_SetWindow0(mask, 0, 0);
    VisitorProfilesWindowMask_SetWindow1(mask, 0, 0);
    mask->task = SysTask_CreateOnVWaitQueue(VisitorProfilesWindowMask_Task, mask, 0);
}

static void VisitorProfilesWindowMask_Free(VisitorProfilesWindowMask *mask) {
    SysTask_Destroy(mask->task);
    reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xFFFF1FFF;
}

static void VisitorProfilesWindowMask_SetWindow0(VisitorProfilesWindowMask *mask, s32 top, s32 bottom) {
    if (top < 0) {
        top = 0;
    }
    if (bottom < 0) {
        bottom = 0;
    }
    mask->update0 = 1;
    mask->top0 = top;
    mask->bottom0 = bottom;
}

static void VisitorProfilesWindowMask_SetWindow1(VisitorProfilesWindowMask *mask, s32 top, s32 bottom) {
    if (top < 0) {
        top = 0;
    }
    if (bottom < 0) {
        bottom = 0;
    }
    mask->update1 = 1;
    mask->top1 = top;
    mask->bottom1 = bottom;
}

static inline void VisitorProfilesWindowMask_SetWindow(u32 top, u32 bottom, vu16 *regs) {
    regs[0] = 255;
    regs[2] = ((top << 8) & 0xFF00) | (u8)bottom;
}

static void VisitorProfilesWindowMask_Task(SysTask *task, void *data) {
    VisitorProfilesWindowMask *mask = data;

    if (mask->update0) {
        VisitorProfilesWindowMask_SetWindow(mask->top0, mask->bottom0, &reg_G2S_DB_WIN0H);
        mask->update0 = 0;
    }
    if (mask->update1) {
        VisitorProfilesWindowMask_SetWindow(mask->top1, mask->bottom1, &reg_G2S_DB_WIN1H);
        mask->update1 = 0;
    }
}

static void VisitorProfilesPanel_AddWindows(VisitorProfilesPanel *panel, VisitorProfilesGraphics *gfx, enum HeapID heapID) {
    int i;
    int baseTile;
    int y;
    Window *window;

    baseTile = 148;
    i = 0;
    y = i;
    window = panel->placeWindows;
    for (; i < VISITOR_PROFILES_NUM_SHOWN_PLACES; i++, y += 6, window++) {
        AddWindowParameterized(gfx->bgConfig, window, GF_BG_LYR_SUB_1, 0, (u8)y, 27, 6, 14, (u16)baseTile);
        baseTile += 162;
        FillWindowPixelBuffer(window, 0);
        CopyWindowToVram(window);
    }
    AddWindowParameterized(gfx->bgConfig, &panel->blankWindow, GF_BG_LYR_SUB_1, 0, 0, 27, 6, 14, 148);
    FillWindowPixelBuffer(&panel->blankWindow, 0);
}

static void VisitorProfilesPanel_RemoveWindows(VisitorProfilesPanel *panel) {
    int i;

    for (i = 0; i < VISITOR_PROFILES_NUM_SHOWN_PLACES; i++) {
        RemoveWindow(&panel->placeWindows[i]);
    }
    RemoveWindow(&panel->blankWindow);
}

static void VisitorProfilesPanel_PrintPlace(VisitorProfilesPanel *panel, u32 index, VisitorProfilesPlaceInfo *info, VisitorProfilesMsg *msg) {
    Window *window;
    String *string;
    u32 color;

    if (index >= VISITOR_PROFILES_NUM_SHOWN_PLACES) {
        GF_AssertFail();
    }
    window = &panel->placeWindows[index];
    FillWindowPixelBuffer(window, 0);
    color = VisitorProfiles_GetHourBgAnim(info->hour);
    string = VisitorProfilesMsg_Get(msg, msg_0801_00002);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, TEXT_SPEED_NOTRANSFER, sVisitorProfilesPlaceNameColors[color], NULL);
    string = VisitorProfilesMsg_GetCountryName(msg, info->country);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    string = VisitorProfilesMsg_GetRegionName(msg, info->country, info->region);
    AddTextPrinterParameterizedWithColor(window, 1, string, 0, 32, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

// One row (8 pixel lines) of a 27-tile wide window is 27 * 32 = 0x360 bytes.
static void *VisitorProfilesPanel_GetRowPixels(VisitorProfilesPanel *panel, u32 index, u32 row) {
    GF_ASSERT(index < VISITOR_PROFILES_NUM_SHOWN_PLACES);
    return (u8 *)panel->placeWindows[index].pixelBuffer + row * 0x360;
}

static void *VisitorProfilesPanel_GetBlankRowPixels(VisitorProfilesPanel *panel, u32 row) {
    return (u8 *)panel->blankWindow.pixelBuffer + row * 0x360;
}

static void VisitorProfilesPanel_TransferRowPixels(VisitorProfilesPanel *panel, int index, int row, void *pixels) {
    u16 tile = GetWindowBaseTile(&panel->placeWindows[index]);
    tile += row * 27;
    GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG1_CHAR_SUB, tile * 32, pixels, 0x360);
}

static void VisitorProfilesBgAnim_Init(VisitorProfilesBgAnim *anim, VisitorProfilesGraphics *gfx, const VisitorProfilesBgAnimTemplate *template, int period, enum HeapID heapID) {
    int i;

    memset(anim, 0, sizeof(VisitorProfilesBgAnim));
    anim->period = period;
    if (template->numScreens) {
        anim->numScreens = template->numScreens;
        for (i = 0; i < anim->numScreens; i++) {
            anim->screenRaw[i] = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, template->screenFiles[i], FALSE, &anim->screens[i], heapID);
        }
    }
    if (template->numPltts) {
        anim->numPltts = template->numPltts;
        anim->plttSlot = template->plttSlot;
        anim->plttRaw = GfGfxLoader_GetPlttDataFromOpenNarc(gfx->narc, template->plttFile, &anim->pltt, heapID);
    }
}

static void VisitorProfilesBgAnim_Free(VisitorProfilesBgAnim *anim) {
    int i;

    if (anim->numScreens != 0) {
        for (i = 0; i < anim->numScreens; i++) {
            Heap_Free(anim->screenRaw[i]);
        }
    }
    if (anim->numPltts != 0) {
        Heap_Free(anim->plttRaw);
    }
}

static void VisitorProfilesBgAnim_Update(VisitorProfilesBgAnim *anim, VisitorProfilesGraphics *gfx) {
    u16 counter = anim->counter;
    u16 period = anim->period;
    int screen = counter * anim->numScreens / period;
    int pltt = counter * anim->numPltts / period;
    int i;
    NNSG2dScreenData *screenData;

    anim->counter = (counter + 1) % period;
    if (anim->numScreens != 0 && screen != anim->curScreen) {
        anim->curScreen = screen;
        for (i = 0; i < VISITOR_PROFILES_NUM_SHOWN_PLACES; i++) {
            if (anim->enabled[i] == 1) {
                screenData = anim->screens[anim->curScreen];
                CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_SUB_2, 0, i * 6, 32, 6, screenData->rawData, 0, i * 6, screenData->screenWidth >> 3, screenData->screenHeight >> 3);
                ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_SUB_2);
            }
        }
    }
    if (anim->numPltts != 0 && pltt != anim->curPltt) {
        anim->curPltt = pltt;
        GF_ASSERT(GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, anim->plttSlot * 32, (u8 *)anim->pltt->pRawData + anim->curPltt * 32, 32));
    }
}

static void VisitorProfilesBgAnim_SetEnabled(VisitorProfilesBgAnim *anim, u32 index, u8 enabled) {
    if (index >= VISITOR_PROFILES_NUM_SHOWN_PLACES) {
        GF_AssertFail();
    }
    anim->enabled[index] = enabled;
}

static void VisitorProfilesBgAnim_DisableAll(VisitorProfilesBgAnim *anim) {
    anim->enabled[0] = 0;
    anim->enabled[1] = 0;
    anim->enabled[2] = 0;
    anim->unk_07 = 0;
}

static void VisitorProfilesBgAnim_CopyRow(VisitorProfilesBgAnim *anim, int row, VisitorProfilesGraphics *gfx) {
    if (anim->numScreens) {
        NNSG2dScreenData *screen = anim->screens[anim->curScreen];
        CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_SUB_2, 0, (u8)row, 32, 1, screen->rawData, 0, (u8)row, screen->screenWidth >> 3, screen->screenHeight >> 3);
        ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_SUB_2);
    }
}

static void VisitorProfilesHourSprites_Init(VisitorProfilesHourSprites *hourSprites, VisitorProfilesGraphics *gfx, VisitorProfilesWindowMask *mask, enum HeapID heapID) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template = { 0 };
    int i;

    memset(hourSprites, 0, sizeof(VisitorProfilesHourSprites));
    hourSprites->current = 2;
    hourSprites->delay = 0x80;
    template.spriteList = gfx->spriteList;
    template.header = &header;
    template.priority = 0x40;
    template.heapID = heapID;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    hourSprites->resources[0][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->narc, 20, FALSE, 0, NNS_G2D_VRAM_TYPE_2DSUB, 12, heapID);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(hourSprites->resources[0][GF_GFX_RES_TYPE_PLTT]));
    GF2DGfxResObj_FreeData(hourSprites->resources[0][GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < VISITOR_PROFILES_NUM_HOUR_SPRITES; i++) {
        hourSprites->resources[i][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->narc, 21 + i * 3, FALSE, i, NNS_G2D_VRAM_TYPE_2DSUB, heapID);
        hourSprites->resources[i][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->narc, 22 + i * 3, FALSE, i, GF_GFX_RES_TYPE_CELL, heapID);
        hourSprites->resources[i][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], gfx->narc, 23 + i * 3, FALSE, i, GF_GFX_RES_TYPE_ANIM, heapID);
        GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(hourSprites->resources[i][GF_GFX_RES_TYPE_CHAR]));
        GF2DGfxResObj_FreeData(hourSprites->resources[i][GF_GFX_RES_TYPE_CHAR]);
        CreateSpriteResourcesHeader(&header, i, 0, i, i, -1, -1, 0, 1, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
        hourSprites->sprites[i] = Sprite_Create(&template);
        Sprite_SetAnimActiveFlag(hourSprites->sprites[i], TRUE);
        Sprite_SetAnimSpeed(hourSprites->sprites[i], FX32_ONE);
    }
    VisitorProfilesHourSprites_HideAll(hourSprites);
    VisitorProfilesWindowMask_SetWindow1(mask, 0, 0);
}

static void VisitorProfilesHourSprites_Free(VisitorProfilesHourSprites *hourSprites, VisitorProfilesGraphics *gfx) {
    int i;
    int j;

    StopSE(SEQ_SE_PL_TOKEI3, 0);
    for (i = 0; i < VISITOR_PROFILES_NUM_HOUR_SPRITES; i++) {
        Sprite_Delete(hourSprites->sprites[i]);
        SpriteTransfer_DeleteCharTransferTask(hourSprites->resources[i][GF_GFX_RES_TYPE_CHAR]);
        if (i == 0) {
            SpriteTransfer_DeletePlttTransferTask(hourSprites->resources[i][GF_GFX_RES_TYPE_PLTT]);
            DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_PLTT], hourSprites->resources[i][GF_GFX_RES_TYPE_PLTT]);
        }
        for (j = 0; j < 4; j++) {
            if (j != GF_GFX_RES_TYPE_PLTT) {
                DestroySingle2DGfxResObj(gfx->resMan[j], hourSprites->resources[i][j]);
            }
        }
    }
}

static void VisitorProfilesHourSprites_Reset(VisitorProfilesHourSprites *hourSprites) {
    u8 *a = hourSprites->active;
    u8 *b = hourSprites->active;
    u8 zero = 0;

    a[0] = zero;
    b[1] = zero;
    b[2] = zero;
    b[3] = zero;
    a = hourSprites->sprite;
    b = hourSprites->sprite;
    a[0] = zero;
    b[1] = zero;
    b[2] = zero;
    b[3] = zero;
    hourSprites->current = 2;
    hourSprites->delay = 0x80;
}

// Two adjacent rows with the same sprite let it fly on from one row into the next.
static void VisitorProfilesHourSprites_Set(VisitorProfilesHourSprites *hourSprites, u8 index, u8 hour, u8 nextHour) {
    hourSprites->active[index] = 1;
    hourSprites->sprite[index] = VisitorProfiles_GetHourSprite(hour);
    VisitorProfilesHourSpriteMotion_Init(&hourSprites->motions[index], sVisitorProfilesHourSpritePaths[hourSprites->sprite[index]][index]);
    if (nextHour < VISITOR_PROFILES_NUM_HOURS) {
        u8 sprite = VisitorProfiles_GetHourSprite(nextHour);
        if (sprite == hourSprites->sprite[index] && index < 2) {
            VisitorProfilesHourSpriteMotion_SetJoinPrev(&hourSprites->motions[index + 1], 1);
            VisitorProfilesHourSpriteMotion_SetJoinNext(&hourSprites->motions[index], 1);
        }
    }
}

static void VisitorProfilesHourSprites_HideAll(VisitorProfilesHourSprites *hourSprites) {
    s32 i;
    for (i = 0; i < VISITOR_PROFILES_NUM_HOUR_SPRITES; i++) {
        Sprite_SetDrawFlag(hourSprites->sprites[i], FALSE);
    }
}

// The sprites fly one after another, from the bottom slot up, after a delay of 0x80 frames.
static void VisitorProfilesHourSprites_Update(VisitorProfilesHourSprites *hourSprites, VisitorProfilesWindowMask *mask) {
    if (hourSprites->delay > 0) {
        hourSprites->delay--;
        if (hourSprites->delay == 0) {
            if (VisitorProfilesHourSprites_Start(hourSprites, hourSprites->current, mask) == TRUE) {
                PlaySE(SEQ_SE_PL_TOKEI3);
            }
        }
    } else if (VisitorProfilesHourSprites_Step(hourSprites, hourSprites->current, mask) == TRUE) {
        s32 next = hourSprites->current - 1;
        if (next >= 0) {
            hourSprites->current = next;
            VisitorProfilesHourSprites_Start(hourSprites, hourSprites->current, mask);
        } else {
            StopSE(SEQ_SE_PL_TOKEI3, 0);
            hourSprites->current = 2;
            hourSprites->delay = 0x80;
        }
    }
}

static BOOL VisitorProfilesHourSprites_Start(VisitorProfilesHourSprites *hourSprites, u32 index, VisitorProfilesWindowMask *mask) {
    u32 sprite;
    if (hourSprites->active[index] == 0) {
        return FALSE;
    }
    VisitorProfilesHourSpriteMotion_Start(&hourSprites->motions[index], mask);
    sprite = hourSprites->sprite[index];
    Sprite_SetDrawFlag(hourSprites->sprites[sprite], TRUE);
    VisitorProfilesHourSprites_UpdateSprite(hourSprites, index);
    return TRUE;
}

static BOOL VisitorProfilesHourSprites_Step(VisitorProfilesHourSprites *hourSprites, u32 index, VisitorProfilesWindowMask *mask) {
    BOOL result;
    u32 sprite;
    if (hourSprites->active[index] == 0) {
        return TRUE;
    }
    result = VisitorProfilesHourSpriteMotion_Step(&hourSprites->motions[index], mask);
    VisitorProfilesHourSprites_UpdateSprite(hourSprites, index);
    if (result == TRUE) {
        sprite = hourSprites->sprite[index];
        Sprite_SetDrawFlag(hourSprites->sprites[sprite], FALSE);
    }
    return result;
}

static void VisitorProfilesHourSpriteMotion_Init(VisitorProfilesHourSpriteMotion *motion, const s16 *path) {
    motion->path = path;
    motion->x = path[0];
    motion->y = path[1] + 0x10;
    motion->step = 0;
    motion->phase = 0;
    motion->joinNext = 0;
    motion->joinPrev = 0;
}

static void VisitorProfilesHourSpriteMotion_SetJoinNext(VisitorProfilesHourSpriteMotion *motion, u16 join) {
    motion->joinNext = join;
}

static void VisitorProfilesHourSpriteMotion_SetJoinPrev(VisitorProfilesHourSpriteMotion *motion, u16 join) {
    motion->joinPrev = join;
}

static BOOL VisitorProfilesHourSpriteMotion_Step(VisitorProfilesHourSpriteMotion *motion, VisitorProfilesWindowMask *mask) {
    s32 phase;
    if (motion->step > motion->path[4]) {
        return TRUE;
    }
    VisitorProfilesHourSpriteMotion_CalcPosition(motion);
    motion->step = motion->step + 1;
    phase = motion->phase + 1;
    if (phase > motion->path[6]) {
        motion->phase = 0;
    } else {
        motion->phase = phase;
    }
    VisitorProfilesHourSpriteMotion_UpdateWindowMask(motion, mask);
    return FALSE;
}

static void VisitorProfilesHourSpriteMotion_Start(VisitorProfilesHourSpriteMotion *motion, VisitorProfilesWindowMask *mask) {
    motion->step = 0;
    motion->phase = 0;
    VisitorProfilesHourSpriteMotion_CalcPosition(motion);
    VisitorProfilesHourSpriteMotion_UpdateWindowMask(motion, mask);
}

// Sub-screen sprite coordinates (the sub screen starts 256 pixels below the main screen).
static void VisitorProfilesHourSpriteMotion_GetPosition(VisitorProfilesHourSpriteMotion *motion, fx32 *pos) {
    pos[0] = motion->x << FX32_SHIFT;
    pos[1] = (motion->y << FX32_SHIFT) + FX32_CONST(256);
}

// Moves along the path, swinging sideways on a sine wave.
static void VisitorProfilesHourSpriteMotion_CalcPosition(VisitorProfilesHourSpriteMotion *motion) {
    s16 endY;
    s16 startY;
    s16 startX;
    s16 sine;
    s32 amplitude;
    u16 angle;
    s64 product;
    s32 scaled;
    float amplitudeFloat;

    if (motion->joinPrev == 0) {
        endY = motion->path[3] - 0x10;
    } else {
        endY = motion->path[3];
    }
    if (motion->joinNext == 0) {
        startY = motion->path[1] + 0x10;
    } else {
        startY = motion->path[1];
    }
    startX = motion->path[0];
    motion->x = startX + (motion->path[2] - startX) * motion->step / motion->path[4];
    motion->y = startY + (endY - startY) * motion->step / motion->path[4];

    angle = (u16)((0xFFFF * motion->phase) / motion->path[6]);
    sine = FX_SinCosTable_[(angle >> 4) * 2];
    if (motion->path[5] > 0) {
        amplitudeFloat = (float)(motion->path[5] << FX32_SHIFT) + 0.5f;
    } else {
        amplitudeFloat = (float)(motion->path[5] << FX32_SHIFT) - 0.5f;
    }
    amplitude = (s32)amplitudeFloat;
    product = (s64)amplitude * sine;
    scaled = (s32)((product + 0x800) >> FX32_SHIFT);
    motion->x += scaled >> FX32_SHIFT;
}

// Window 0 hides the sprite where it leaves its row (unless the neighbouring row continues the flight).
static void VisitorProfilesHourSpriteMotion_UpdateWindowMask(VisitorProfilesHourSpriteMotion *motion, VisitorProfilesWindowMask *mask) {
    s16 bound;

    VisitorProfilesWindowMask_SetWindow0(mask, 0, 0);
    if (motion->joinNext == 0) {
        bound = motion->path[1];
        if (motion->y > bound - 0x10) {
            VisitorProfilesWindowMask_SetWindow0(mask, bound, (s16)(bound + 0x20));
        }
    }
    if (motion->joinPrev == 0) {
        bound = motion->path[3];
        if (motion->y - 0x10 <= bound) {
            VisitorProfilesWindowMask_SetWindow0(mask, (s16)(bound - 0x20), bound);
        }
    }
}

static void VisitorProfilesHourSprites_UpdateSprite(VisitorProfilesHourSprites *hourSprites, u32 index) {
    VecFx32 pos;

    VisitorProfilesHourSpriteMotion_GetPosition(&hourSprites->motions[index], &pos.x);
    Sprite_SetMatrix(hourSprites->sprites[hourSprites->sprite[index]], &pos);
}

static u8 VisitorProfiles_GetHourBgAnim(u32 hour) {
    GF_ASSERT(hour < VISITOR_PROFILES_NUM_HOURS);
    return sVisitorProfilesHourAnims[hour][0];
}

static u8 VisitorProfiles_GetHourSprite(u32 hour) {
    GF_ASSERT(hour < VISITOR_PROFILES_NUM_HOURS);
    return sVisitorProfilesHourAnims[hour][1];
}

static void VisitorProfilesMsg_Init(VisitorProfilesMsg *msg, enum HeapID heapID) {
    msg->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0801_bin, heapID);
    msg->format = MessageFormat_New_Custom(8, 64, heapID);
    msg->buffer = String_New(128, heapID);
    msg->raw = String_New(128, heapID);
}

static void VisitorProfilesMsg_Free(VisitorProfilesMsg *msg) {
    DestroyMsgData(msg->msgData);
    MessageFormat_Delete(msg->format);
    String_Delete(msg->buffer);
    String_Delete(msg->raw);
}

static String *VisitorProfilesMsg_Get(VisitorProfilesMsg *msg, s32 msgId) {
    ReadMsgDataIntoString(msg->msgData, msgId, msg->buffer);
    return msg->buffer;
}

static void VisitorProfilesMsg_Read(VisitorProfilesMsg *msg, s32 msgId, String *dest) {
    ReadMsgDataIntoString(msg->msgData, msgId, dest);
}

static String *VisitorProfilesMsg_GetCountryName(VisitorProfilesMsg *msg, u32 country) {
    ReadMsgDataIntoString(msg->msgData, msg_0801_00003, msg->raw);
    BufferCountryName(msg->format, 0, country);
    StringExpandPlaceholders(msg->format, msg->buffer, msg->raw);
    return msg->buffer;
}

static String *VisitorProfilesMsg_GetRegionName(VisitorProfilesMsg *msg, u32 country, u32 region) {
    MessageFormat_ResetBuffers(msg->format);
    ReadMsgDataIntoString(msg->msgData, msg_0801_00003, msg->raw);
    BufferCityName(msg->format, 0, country, region);
    StringExpandPlaceholders(msg->format, msg->buffer, msg->raw);
    return msg->buffer;
}

static s32 VisitorProfiles_FindFirstVisitor(void *visitors) {
    s32 i;
    for (i = 0; i < VISITOR_PROFILES_NUM_VISITORS; i++) {
        if ((u8)WiFiPlazaPlaces_IsOccupied(visitors, (u8)i) == 1) {
            return i;
        }
    }
    return VISITOR_PROFILES_NUM_VISITORS;
}
