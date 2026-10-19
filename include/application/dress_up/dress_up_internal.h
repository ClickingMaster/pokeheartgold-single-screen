#ifndef POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H

#include "global.h"

#include "application/dress_up/accessory_portrait.h"

#include "bg_window.h"
#include "fashion_case.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "gf_3d_loader.h"
#include "graphics_object_manager.h"
#include "heap.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "yes_no_prompt.h"

#define DRESS_UP_IMAGE_MAX     0x76 // 100 accessories + 18 backdrops
#define DRESS_UP_PALETTE_MAX   0x13 // the accessory palette + 18 backdrop palettes
#define DRESS_UP_HIT_MAP_MAX   0x77 // images + the mon
#define DRESS_UP_ACCESSORY_NUM 100
#define DRESS_UP_BACKDROP_NUM  18

// graphics core shared by the Dress-Up editor and the Accessory Portrait renderer
typedef struct DressUpGfx {
    GraphicsManager *graphicsManager;
    GraphicsImage **images;
    int imageMax;
    int imageCount;
    GraphicsPalette **palettes;
    int paletteMax;
    int paletteCount;
    BOOL graphicsManagerActive;
    PokepicManager *pokepicManager;
    NNSGfdTexKey texKey;
    NNSGfdPlttKey plttKey;
    BOOL pokepicActive;
    void *monPicAttrs;
    GF_2DGfxRawResMan *hitMapResMan;
    NNSG2dCharacterData **hitMaps; // [accessory], [100 + backdrop], [DRESS_UP_IMAGE_MAX] mon
    int hitMapCount;
    BgConfig *bgConfig;
    SpriteList *spriteList;
    GF_2DGfxResMan *charResMan;
    GF_2DGfxResMan *plttResMan;
    GF_2DGfxResMan *cellResMan;
    GF_2DGfxResMan *animResMan;
    G2dRenderer renderer;
    NARC *narc;
} DressUpGfx;

// image and palette templates loaded into the graphics manager
typedef struct DressUpGfxResources {
    GraphicsImageTemplate *images;
    int imageCount;
    GraphicsPaletteTemplate *palettes;
    int paletteCount;
    GF_2DGfxRawResMan *charResMan;
    GF_2DGfxRawResMan *plttResMan;
} DressUpGfxResources;

typedef struct DressUpSlot {
    u32 id;
    GraphicsObject *object;
} DressUpSlot;

typedef struct DressUpSlotList {
    DressUpSlot *slots;
    int count;
} DressUpSlotList;

typedef struct DressUpSlotTemplate {
    DressUpSlotList *list;
    GraphicsManager *graphicsManager;
    GraphicsImage *image;
    GraphicsPalette *palette;
    int x;
    int y;
    u32 id;
    u32 paletteSlot;
} DressUpSlotTemplate;

typedef struct DressUpScale {
    fx32 x;
    fx32 y;
    fx32 z;
} DressUpScale;

// DressUpWork.uiState
#define DRESS_UP_UI_ACCESSORIES 0
#define DRESS_UP_UI_BACKDROPS   1
#define DRESS_UP_UI_DONE        3 // DONE pressed
#define DRESS_UP_UI_DRESSED_UP  4 // "Is your Pokemon all dressed up?" open
#define DRESS_UP_UI_SAVE        5 // "Would you like to save this design?" open
#define DRESS_UP_UI_SAVE_YES    6
#define DRESS_UP_UI_SAVE_NO     7
#define DRESS_UP_UI_DRESSED_YES 8
#define DRESS_UP_UI_DRESSED_NO  9
#define DRESS_UP_UI_EXIT        10

// object on the canvas or in the tray; lists are circular with a sentinel node
typedef struct DressUpNode {
    void *object; // DressUpSlot (accessory, backdrop) or DressUpMonPic (mon)
    u32 mode;     // 0 accessory, 1/2 backdrop, 3 mon
    struct DressUpNode *next;
    struct DressUpNode *prev;
} DressUpNode;

typedef struct DressUpNodePool {
    DressUpNode *nodes;
    int count;
} DressUpNodePool;

typedef struct DressUpMonPic {
    Pokepic *pokepic;
    TouchscreenHitbox hitbox;
    u8 margins[4]; // transparent left, right, top, bottom
    Pokemon *mon;
} DressUpMonPic;

typedef struct DressUpBgScreenTemplate {
    BgConfig *bgConfig;
    NarcId narcId;
    u32 charFileId;
    u32 plttFileId;
    u32 screenFileId;
    int x; // pixels
    int y;
    GFBgLayer layer;
    u32 plttCount;
    u32 plttSlot;
    u32 charOffset;
    enum HeapID heapID;
} DressUpBgScreenTemplate;

typedef struct DressUpBgScreen {
    BgConfig *bgConfig;
    NarcId narcId;
    u32 screenFileId;
    int xTile;
    int yTile;
    int widthTiles;
    int heightTiles;
    GFBgLayer layer;
    u32 plttSlot;
    u32 plttCount;
    u32 charOffset;
} DressUpBgScreen;

typedef struct DressUpCanvasTemplate {
    GraphicsManager *graphicsManager;
    GraphicsImage **images;
    GraphicsPalette **palettes;
    u8 *paletteSlots; // per accessory (DressUpGfx.monPicAttrs)
    PokepicManager *pokepicManager;
    BgConfig *bgConfig;
    DressUpSlotList *slotList;
    DressUpNodePool *nodePool;
    int limit; // accessories on the canvas
} DressUpCanvasTemplate;

// the stage: backdrop, the mon and the accessories placed on it (editor and Accessory Portrait)
typedef struct DressUpCanvas {
    DressUpNodePool *nodePool;
    DressUpNode looseList; // accessories not touching the mon
    DressUpNode monList;   // the mon and the accessories on it
    int count;
    int limit;
    GraphicsManager *graphicsManager;
    GraphicsImage **images;
    GraphicsPalette **palettes;
    u8 *paletteSlots;
    PokepicManager *pokepicManager;
    BgConfig *bgConfig;
    DressUpSlotList *slotList;
    DressUpBgScreen backdrop;
    u32 backdropId;
    DressUpMonPic monPic;
} DressUpCanvas;

typedef struct DressUpTrayTemplate {
    GraphicsManager *graphicsManager;
    GraphicsImage **images;
    GraphicsPalette **palettes;
    u8 *paletteSlots; // per accessory (DressUpGfx.monPicAttrs)
    BgConfig *bgConfig;
    DressUpSlotList *slotList;
    DressUpNodePool *nodePool;
    int pageCounts[3]; // per item group
    u32 *caseTable;    // DressUpWork.caseTable
} DressUpTrayTemplate;

// pages of one tray group; each page is a list sentinel
typedef struct DressUpTrayGroup {
    DressUpNode *pages;
    int pageCount;
    int curPage;
} DressUpTrayGroup;

// accessory / backdrop picker
typedef struct DressUpTray {
    int group; // shown group: 0 accessories, 1 backdrops, 3 none (DONE)
    u32 *caseTable;
    DressUpNodePool *nodePool;
    DressUpTrayGroup groups[4];
    BOOL pageSwapActive;
    int tabBgIndex;
    GraphicsManager *graphicsManager;
    GraphicsImage **images;
    GraphicsPalette **palettes;
    u8 *paletteSlots;
    BgConfig *bgConfig;
    DressUpSlotList *slotList;
    DressUpBgScreen tabBg;
    u32 unk_88;
} DressUpTray;

// an object dropping out of / into the tray during a page swap
typedef struct DressUpSlideAnim {
    DressUpNode *node;
    int remaining;
    int delta;
} DressUpSlideAnim;

// alternates a BG screen between its own screen file and another one
typedef struct DressUpBgScreenAnimTemplate {
    DressUpBgScreen *screen;
    u32 altScreenFileId;
    int delay;
    enum HeapID heapID;
} DressUpBgScreenAnimTemplate;

typedef struct DressUpBgScreenAnim {
    DressUpBgScreen *screen;
    u32 altScreenFileId;
    int delay;
    int ticks;
    BOOL showingAlt;
    enum HeapID heapID;
} DressUpBgScreenAnim;

// the backdrop's animation while the finished design is shown
typedef struct DressUpBackdropAnim {
    DressUpCanvas *canvas;
    DressUpBgScreenAnim anim;
} DressUpBackdropAnim;

struct DressUpTouch;
typedef void (*DressUpTouchCallback)(struct DressUpTouch *touch);

typedef struct DressUpDrag {
    DressUpGfx *gfx;
    DressUpCanvas *canvas;
    DressUpTray *tray;
    struct DressUpHud *hud;
    DressUpNode *selected;
    int grabX; // touch position within the object
    int grabY;
    s16 originX; // where it was picked up
    s16 originY;
    u8 fromCanvas;
    u8 trayPage;
    int monX; // last position of the mon while it is dragged
    int monY;
    BOOL allowMonDrag;
    BOOL dragging;
} DressUpDrag;

// touch dispatcher: new touch / held / released
typedef struct DressUpTouch {
    DressUpDrag *drag;
    DressUpTouchCallback onTouch;
    DressUpTouchCallback onRelease;
    DressUpTouchCallback onHeld;
    DressUpTouchCallback destroy;
    u16 lastX;
    u16 lastY;
    u8 wasHeld;
} DressUpTouch;

typedef void (*DressUpButtonCallback)(void *button, void *arg);

typedef struct DressUpButtonTemplate {
    const SpriteTemplate *spriteTemplate;
    DressUpButtonCallback callback;
    void *arg;
    u32 trigger; // touch state that fires the callback
} DressUpButtonTemplate;

typedef struct DressUpTextButtonTemplate {
    DressUpButtonTemplate button;
    Window *window;
    FontSystem *fontSystem;
    NNSG2dImagePaletteProxy *plttProxy;
    int x;
    int y;
    u32 charOffset;
} DressUpTextButtonTemplate;

typedef struct DressUpButton {
    Sprite *sprite;
    u32 trigger;
    void *arg;
    DressUpButtonCallback callback;
} DressUpButton;

typedef struct DressUpTextButton {
    DressUpButton button;
    TextOBJ *textObj;
    ObjCharSpaceReservation charAlloc;
} DressUpTextButton;

// page arrows, the two tabs and DONE
typedef struct DressUpButtonPanel {
    DressUpButton buttons[4]; // next page, previous page, accessory tab, backdrop tab
    DressUpTextButton done;
    BOOL visible;
    FontSystem *fontSystem;
    TouchHitboxController *controller;
    TouchscreenHitbox hitboxes[5];
    MenuInputState *inputState;
    u16 keyRepeat; // frames a key-pressed button stays pressed
    u16 key;
} DressUpButtonPanel;

typedef struct DressUpHudTemplate {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    GF_2DGfxResMan **resMans; // DressUpGfx.charResMan..animResMan
    Options *options;
    int countMax;    // accessories the meter shows
    u32 infoMsgBank; // info window (DRESS_UP_HUD_INFO_WINDOW)
    u32 infoMsgId;
    int countdownSeconds; // countdown (DRESS_UP_HUD_COUNTDOWN)
    int *countdownOutput;
    NARC *narc;
} DressUpHudTemplate;

// DressUpHud_Init flags
#define DRESS_UP_HUD_CLEAR       (1 << 0)
#define DRESS_UP_HUD_SUB_BG      (1 << 1)
#define DRESS_UP_HUD_MSG_WINDOW  (1 << 2)
#define DRESS_UP_HUD_COUNT_ICONS (1 << 3)
#define DRESS_UP_HUD_COUNTDOWN   (1 << 4)
#define DRESS_UP_HUD_INFO_WINDOW (1 << 5)

#define DRESS_UP_COUNT_ICON_NUM 20

// linear fx32 interpolation
typedef struct DressUpTween {
    fx32 value;
    fx32 start;
    fx32 distance;
    s32 frame;
    s32 frames;
} DressUpTween;

// the countdown digits shrinking back to normal size after each tick of the last ten seconds
typedef struct DressUpCountdownPulse {
    BOOL active;
    Sprite *digits[2];
    VecFx32 positions[2];
    DressUpTween scale;
    DressUpTween x;
    DressUpTween y;
} DressUpCountdownPulse;

// accessories placed, as a 10x2 grid of icons on the sub screen
typedef struct DressUpCountIcons {
    SpriteResource *resources[4]; // char, pltt, cell, anim
    Sprite *sprites[DRESS_UP_COUNT_ICON_NUM];
    int count;
    u32 unk_64;
} DressUpCountIcons;

// two-digit countdown
typedef struct DressUpCountdown {
    SpriteResource *resources[4];
    Sprite *digits[2];
    Window *window;
    int value; // seconds shown
    int frames;
    SysTask *tasks[2];
    int *output; // [0] seconds left (set by the owner), [2] start value
    DressUpCountdownPulse pulse;
    BOOL warning; // digits switched to the warning palette
} DressUpCountdown;

// sub screen of the editor: message box, accessory count meter and countdown
typedef struct DressUpHud {
    DressUpBgScreen subBg;
    Window *msgWindow;
    SpriteList *spriteList;
    GF_2DGfxResMan **resMans;
    DressUpCountIcons countIcons;
    DressUpCountdown countdown;
    Window *infoWindow;
    u32 flags;
    u32 frame; // Options frame
    u32 textFrameDelay;
    String *msgString;
} DressUpHud;

typedef struct DressUpWork {
    DressUpGfx gfx;
    u32 caseTable[DRESS_UP_ACCESSORY_NUM + DRESS_UP_BACKDROP_NUM]; // owned count per accessory, then backdrop
                                                                   // per order (18 = none)
    DressUpNodePool nodePool;
    DressUpSlotList *slotList;
    DressUpTray tray;
    DressUpCanvas canvas;
    DressUpBackdropAnim backdropAnim;
    DressUpTouch touch;
    DressUpBgScreen frameBg;
    DressUpButtonPanel buttonPanel;
    DressUpHud hud;
    int uiState;
    u32 transitionDone;
    YesNoPrompt *yesNoPrompt;
    Window *msgWindow;
    BOOL saveRequested;
    u8 unk_6C4[0x18];
    Options *options;
    u32 printerId;
    u32 unk_6E4;
    MessageFormat *msgFormat;
    MenuInputState menuInputState;
} DressUpWork;

// argument block of the Accessory Portrait renderer: one saved design plus the caller's config
typedef struct AccessoryPortraitArgs {
    struct SaveFashionMon *mon;
    u8 *items[20]; // the placed accessories
    int count;
    BgConfig *bgConfig;
    u32 background;
    int x;
    int y;
    enum HeapID heapID;
} AccessoryPortraitArgs;

// renders a saved dress-up design (Accessory Portrait app, VS Recorder dress-up photos)
struct AccessoryPortrait {
    DressUpGfx gfx;
    DressUpNodePool nodePool;
    DressUpSlotList *slotList;
    DressUpCanvas canvas;
    DressUpBackdropAnim backdropAnim;
    Pokemon *mon;
    enum HeapID heapID;
    int x;
    int y;
    int monOffsetX;
    int monOffsetY;
    u16 rotation;
    DressUpScale scale;
    u16 unk_25C;
    BOOL bgEnabled;
    BOOL monVisible;
};

typedef struct AccessoryPortraitAppWork {
    SaveFashionDataSub *fashionData;
    u32 unk_004;
    int selection;
    int mode;
    AccessoryPortrait *portrait;
    DressUpGfx gfx;
    Sprite *icon;
    Window *captionWindow;
} AccessoryPortraitAppWork;

DressUpSlotList *DressUpGfx_CreateSlotList(int count, enum HeapID heapID);
void DressUpGfx_FreeSlotList(DressUpSlotList *list);
void DressUpGfx_InitDisplay(void);
void DressUpGfx_ResetDisplay(void);
void DressUpGfx_UploadRawResTables(DressUpGfx *gfx, DressUpGfxResources *resources);
void DressUpGfx_FreeAllTexAndPltt(DressUpGfx *gfx);
Sprite *DressUpGfx_CreateSprite(DressUpGfx *gfx, int id, int x, int y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen);
void DressUpGfx_LoadCharRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int id);
void DressUpGfx_LoadPlttRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int plttNum, int id);
void DressUpGfx_LoadCellRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int id);
void DressUpGfx_LoadAnimRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int id);
void DressUpGfx_FreeCharRes(DressUpGfx *gfx, int id);
void DressUpGfx_FreePlttRes(DressUpGfx *gfx, int id);
void DressUpGfx_FreeCellRes(DressUpGfx *gfx, int id);
void DressUpGfx_FreeAnimRes(DressUpGfx *gfx, int id);
void DressUpGfx_DestroyRawResTables(DressUpGfxResources *resources);
void AccessoryPortrait_AllocRawResTables(DressUpGfxResources *resources, enum HeapID heapID);
void AccessoryPortrait_InitGfxCore(DressUpGfx *gfx, GraphicsManagerTemplate *template, enum HeapID heapID);
void AccessoryPortrait_InitBg(DressUpGfx *gfx, BgConfig *bgConfig, enum HeapID heapID);
void AccessoryPortrait_FreeGfxCore(DressUpGfx *gfx);
void AccessoryPortrait_FreeBg(DressUpGfx *gfx);
void DressUpGfx_HandlePokepicLoad(DressUpGfx *gfx);
void AccessoryPortrait_DrawTransformed(DressUpGfx *gfx, int x, int y, u16 angle, const DressUpScale *scale);
void AccessoryPortrait_InitAppGfx(DressUpGfx *gfx, enum HeapID heapID);
void AccessoryPortrait_FreeAppGfx(DressUpGfx *gfx);
void AccessoryPortrait_AppVBlank(DressUpGfx *gfx);
void AccessoryPortrait_RenderSprites(DressUpGfx *gfx);

BOOL DressUp_Init(OverlayManager *man, int *state);
BOOL DressUp_Main(OverlayManager *man, int *state);
BOOL DressUp_Exit(OverlayManager *man, int *state);
void DressUp_InitHud(DressUpWork *work, Options *options);

void DressUpCanvas_Init(DressUpCanvas *canvas, const DressUpCanvasTemplate *template);
void DressUpCanvas_Destroy(DressUpCanvas *canvas);
void AccessoryPortrait_CreateCanvasMonPic(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, int x, int y, int priority, enum HeapID heapID);
BOOL AccessoryPortrait_PlaceCanvasAccessory(DressUpCanvas *canvas, int accessory, int x, int y, int priority);
void DressUpCanvas_ClearObjects(DressUpCanvas *canvas);
void DressUpCanvas_SetOrigin(DressUpCanvas *canvas, int x, int y, int newX, int newY);
void DressUpCanvas_SetBackdrop(DressUpCanvas *canvas, int backdrop, enum HeapID heapID);
void DressUpCanvas_FreeBackdrop(DressUpCanvas *canvas);

int DressUpMon_GetPriority(DressUpMonPic *monPic);
void DressUpMon_GetCenterPos(DressUpMonPic *monPic, int *x, int *y);
void DressUpNodePool_Init(DressUpNodePool *pool, int count, enum HeapID heapID);
void DressUpNodePool_Free(DressUpNodePool *pool);

void DressUpHud_Init(DressUpHud *hud, const DressUpHudTemplate *template, u32 flags);
void DressUp_StartBackdropAnim(DressUpBackdropAnim *anim, DressUpCanvas *canvas, enum HeapID heapID);
void DressUp_UpdateBackdropAnim(DressUpBackdropAnim *anim);

#endif // POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H
