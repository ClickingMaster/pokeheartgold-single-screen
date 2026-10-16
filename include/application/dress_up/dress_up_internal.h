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

// graphics core shared by the Dress-Up editor and the Accessory Portrait renderer (offset 0 of both works)
typedef struct DressUpGfx {
    GraphicsManager *graphicsManager; // 0x000
    GraphicsImage **images;           // 0x004
    int imageMax;                     // 0x008
    int imageCount;                   // 0x00C
    GraphicsPalette **palettes;       // 0x010
    int paletteMax;                   // 0x014
    int paletteCount;                 // 0x018
    BOOL graphicsManagerActive;       // 0x01C
    PokepicManager *pokepicManager;   // 0x020
    NNSGfdTexKey texKey;              // 0x024
    NNSGfdPlttKey plttKey;            // 0x028
    BOOL pokepicActive;               // 0x02C
    void *monPicAttrs;                // 0x030, NARC_a_0_2_6 member 0xEB
    GF_2DGfxRawResMan *hitMapResMan;  // 0x034
    NNSG2dCharacterData **hitMaps;    // 0x038, [accessory], [100 + backdrop], [0x76] mon
    int hitMapCount;                  // 0x03C
    BgConfig *bgConfig;               // 0x040
    SpriteList *spriteList;           // 0x044
    GF_2DGfxResMan *charResMan;       // 0x048
    GF_2DGfxResMan *plttResMan;       // 0x04C
    GF_2DGfxResMan *cellResMan;       // 0x050
    GF_2DGfxResMan *animResMan;       // 0x054
    G2dRenderer renderer;             // 0x058
    NARC *narc;                       // 0x180
} DressUpGfx;                         // size 0x184

// image and palette templates loaded into the graphics manager
typedef struct DressUpGfxResources {
    GraphicsImageTemplate *images;     // 0x00
    int imageCount;                    // 0x04
    GraphicsPaletteTemplate *palettes; // 0x08
    int paletteCount;                  // 0x0C
    GF_2DGfxRawResMan *charResMan;     // 0x10
    GF_2DGfxRawResMan *plttResMan;     // 0x14
} DressUpGfxResources;                 // size 0x18

typedef struct DressUpSlot {
    u32 id;
    GraphicsObject *object;
} DressUpSlot;

typedef struct DressUpSlotList {
    DressUpSlot *slots;
    int count;
} DressUpSlotList;

typedef struct DressUpSlotTemplate {
    DressUpSlotList *list;            // 0x00
    GraphicsManager *graphicsManager; // 0x04
    GraphicsImage *image;             // 0x08
    GraphicsPalette *palette;         // 0x0C
    int x;                            // 0x10
    int y;                            // 0x14
    u32 id;                           // 0x18
    u32 paletteSlot;                  // 0x1C
} DressUpSlotTemplate;

typedef struct DressUpScale {
    fx32 x;
    fx32 y;
    fx32 z;
} DressUpScale;

// DressUpWork.uiState
#define DRESS_UP_UI_ACCESSORIES 0 // accessory tab
#define DRESS_UP_UI_BACKDROPS   1 // backdrop tab
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
    void *object;             // 0x00, DressUpSlot (accessory, backdrop) or DressUpMonPic (mon)
    u32 mode;                 // 0x04, 0 accessory, 1/2 backdrop, 3 mon
    struct DressUpNode *next; // 0x08
    struct DressUpNode *prev; // 0x0C
} DressUpNode;

typedef struct DressUpNodePool {
    DressUpNode *nodes;
    int count;
} DressUpNodePool;

// the dressed-up mon (Pokepic)
typedef struct DressUpMonPic {
    Pokepic *pokepic;         // 0x0
    TouchscreenHitbox hitbox; // 0x4
    u8 margins[4];            // 0x8, transparent left, right, top, bottom
    Pokemon *mon;             // 0xC
} DressUpMonPic;

// BG screen loaded from NARC_a_0_2_6 (chars, palette, screen)
typedef struct DressUpBgScreenTemplate {
    BgConfig *bgConfig; // 0x00
    NarcId narcId;      // 0x04
    u32 charFileId;     // 0x08
    u32 plttFileId;     // 0x0C
    u32 screenFileId;   // 0x10
    int x;              // 0x14, pixels
    int y;              // 0x18
    GFBgLayer layer;    // 0x1C
    u32 plttCount;      // 0x20
    u32 plttSlot;       // 0x24
    u32 charOffset;     // 0x28
    enum HeapID heapID; // 0x2C
} DressUpBgScreenTemplate;

typedef struct DressUpBgScreen {
    BgConfig *bgConfig; // 0x00
    NarcId narcId;      // 0x04
    u32 screenFileId;   // 0x08
    int xTile;          // 0x0C
    int yTile;          // 0x10
    int widthTiles;     // 0x14
    int heightTiles;    // 0x18
    GFBgLayer layer;    // 0x1C
    u32 plttSlot;       // 0x20
    u32 plttCount;      // 0x24
    u32 charOffset;     // 0x28
} DressUpBgScreen;      // size 0x2C

typedef struct DressUpCanvasTemplate {
    GraphicsManager *graphicsManager; // 0x00
    GraphicsImage **images;           // 0x04
    GraphicsPalette **palettes;       // 0x08
    u8 *paletteSlots;                 // 0x0C, per accessory (DressUpGfx.monPicAttrs)
    PokepicManager *pokepicManager;   // 0x10
    BgConfig *bgConfig;               // 0x14
    DressUpSlotList *slotList;        // 0x18
    DressUpNodePool *nodePool;        // 0x1C
    int limit;                        // 0x20, accessories on the canvas
} DressUpCanvasTemplate;

// the stage: backdrop, the mon and the accessories placed on it (editor and Accessory Portrait)
typedef struct DressUpCanvas {
    DressUpNodePool *nodePool;        // 0x00
    DressUpNode looseList;            // 0x04, accessories not touching the mon
    DressUpNode monList;              // 0x14, the mon and the accessories on it
    int count;                        // 0x24
    int limit;                        // 0x28
    GraphicsManager *graphicsManager; // 0x2C
    GraphicsImage **images;           // 0x30
    GraphicsPalette **palettes;       // 0x34
    u8 *paletteSlots;                 // 0x38
    PokepicManager *pokepicManager;   // 0x3C
    BgConfig *bgConfig;               // 0x40
    DressUpSlotList *slotList;        // 0x44
    DressUpBgScreen backdrop;         // 0x48
    u32 backdropId;                   // 0x74
    DressUpMonPic monPic;             // 0x78
} DressUpCanvas;                      // size 0x88

typedef struct DressUpTrayTemplate {
    GraphicsManager *graphicsManager; // 0x00
    GraphicsImage **images;           // 0x04
    GraphicsPalette **palettes;       // 0x08
    u8 *paletteSlots;                 // 0x0C, per accessory (DressUpGfx.monPicAttrs)
    BgConfig *bgConfig;               // 0x10
    DressUpSlotList *slotList;        // 0x14
    DressUpNodePool *nodePool;        // 0x18
    int pageCounts[3];                // 0x1C, per item group
    u32 *caseTable;                   // 0x28, DressUpWork.caseTable
} DressUpTrayTemplate;

// pages of one tray group; each page is a list sentinel
typedef struct DressUpTrayGroup {
    DressUpNode *pages;
    int pageCount;
    int curPage;
} DressUpTrayGroup;

// accessory / backdrop picker
typedef struct DressUpTray {
    int group;                        // 0x00, shown group: 0 accessories, 1 backdrops, 3 none (DONE)
    u32 *caseTable;                   // 0x04
    DressUpNodePool *nodePool;        // 0x08
    DressUpTrayGroup groups[4];       // 0x0C
    BOOL pageSwapActive;              // 0x3C
    int tabBgIndex;                   // 0x40
    GraphicsManager *graphicsManager; // 0x44
    GraphicsImage **images;           // 0x48
    GraphicsPalette **palettes;       // 0x4C
    u8 *paletteSlots;                 // 0x50
    BgConfig *bgConfig;               // 0x54
    DressUpSlotList *slotList;        // 0x58
    DressUpBgScreen tabBg;            // 0x5C
    u32 unk_88;                       // 0x88
} DressUpTray;                        // size 0x8C

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
    DressUpBgScreen *screen; // 0x00
    u32 altScreenFileId;     // 0x04
    int delay;               // 0x08
    int ticks;               // 0x0C
    BOOL showingAlt;         // 0x10
    enum HeapID heapID;      // 0x14
} DressUpBgScreenAnim;       // size 0x18

// the backdrop's animation while the finished design is shown
typedef struct DressUpBackdropAnim {
    DressUpCanvas *canvas;
    DressUpBgScreenAnim anim;
} DressUpBackdropAnim; // size 0x1C

struct DressUpTouch;
typedef void (*DressUpTouchCallback)(struct DressUpTouch *touch);

// object being dragged
typedef struct DressUpDrag {
    DressUpGfx *gfx;        // 0x00
    DressUpCanvas *canvas;  // 0x04
    DressUpTray *tray;      // 0x08
    struct DressUpHud *hud; // 0x0C
    DressUpNode *selected;  // 0x10
    int grabX;              // 0x14, touch position within the object
    int grabY;              // 0x18
    s16 originX;            // 0x1C, where it was picked up
    s16 originY;            // 0x1E
    u8 fromCanvas;          // 0x20
    u8 trayPage;            // 0x21
    int monX;               // 0x24, last position of the mon while it is dragged
    int monY;               // 0x28
    BOOL allowMonDrag;      // 0x2C
    BOOL dragging;          // 0x30
} DressUpDrag;              // size 0x34

// touch dispatcher: new touch / held / released
typedef struct DressUpTouch {
    DressUpDrag *drag;              // 0x00
    DressUpTouchCallback onTouch;   // 0x04
    DressUpTouchCallback onRelease; // 0x08
    DressUpTouchCallback onHeld;    // 0x0C
    DressUpTouchCallback destroy;   // 0x10
    u16 lastX;                      // 0x14
    u16 lastY;                      // 0x16
    u8 wasHeld;                     // 0x18
} DressUpTouch;                     // size 0x1C

typedef void (*DressUpButtonCallback)(void *button, void *arg);

typedef struct DressUpButtonTemplate {
    const SpriteTemplate *spriteTemplate; // 0x00
    DressUpButtonCallback callback;       // 0x04, NULL: none
    void *arg;                            // 0x08
    u32 trigger;                          // 0x0C, touch state that fires the callback
} DressUpButtonTemplate;

typedef struct DressUpTextButtonTemplate {
    DressUpButtonTemplate button;       // 0x00
    Window *window;                     // 0x10, the text bitmap
    FontSystem *fontSystem;             // 0x14
    NNSG2dImagePaletteProxy *plttProxy; // 0x18
    int x;                              // 0x1C
    int y;                              // 0x20
    u32 charOffset;                     // 0x24
} DressUpTextButtonTemplate;

typedef struct DressUpButton {
    Sprite *sprite;                 // 0x0
    u32 trigger;                    // 0x4
    void *arg;                      // 0x8
    DressUpButtonCallback callback; // 0xC
} DressUpButton;

typedef struct DressUpTextButton {
    DressUpButton button;              // 0x00
    TextOBJ *textObj;                  // 0x10
    ObjCharSpaceReservation charAlloc; // 0x14, VRAM for the text
} DressUpTextButton;                   // size 0x20

// page arrows, the two tabs and DONE
typedef struct DressUpButtonPanel {
    DressUpButton buttons[4];          // 0x00, next page, previous page, accessory tab, backdrop tab
    DressUpTextButton done;            // 0x40
    BOOL visible;                      // 0x60
    FontSystem *fontSystem;            // 0x64
    TouchHitboxController *controller; // 0x68
    TouchscreenHitbox hitboxes[5];     // 0x6C
    MenuInputState *inputState;        // 0x80
    u16 keyRepeat;                     // 0x84, frames a key-pressed button stays pressed
    u16 key;                           // 0x86
} DressUpButtonPanel;                  // size 0x88

typedef struct DressUpHudTemplate {
    BgConfig *bgConfig;       // 0x00
    SpriteList *spriteList;   // 0x04
    GF_2DGfxResMan **resMans; // 0x08, DressUpGfx.charResMan..animResMan
    Options *options;         // 0x0C
    int countMax;             // 0x10, accessories the meter shows
    u32 infoMsgBank;          // 0x14, info window (flag 0x20)
    u32 infoMsgId;            // 0x18
    int countdownSeconds;     // 0x1C, countdown (flag 0x10)
    int *countdownOutput;     // 0x20
    NARC *narc;               // 0x24
} DressUpHudTemplate;

// DressUpHud_Init flags
#define DRESS_UP_HUD_CLEAR       (1 << 0)
#define DRESS_UP_HUD_SUB_BG      (1 << 1)
#define DRESS_UP_HUD_MSG_WINDOW  (1 << 2)
#define DRESS_UP_HUD_COUNT_ICONS (1 << 3)
#define DRESS_UP_HUD_COUNTDOWN   (1 << 4) // never set
#define DRESS_UP_HUD_INFO_WINDOW (1 << 5) // never set

#define DRESS_UP_COUNT_ICON_NUM 20

// linear fx32 interpolation
typedef struct DressUpTween {
    fx32 value;    // 0x00
    fx32 start;    // 0x04
    fx32 distance; // 0x08
    s32 frame;     // 0x0C
    s32 frames;    // 0x10
} DressUpTween;

// the countdown digits shrinking back to normal size after each tick of the last ten seconds
typedef struct DressUpCountdownPulse {
    BOOL active;          // 0x00
    Sprite *digits[2];    // 0x04
    VecFx32 positions[2]; // 0x0C
    DressUpTween scale;   // 0x24
    DressUpTween x;       // 0x38
    DressUpTween y;       // 0x4C
} DressUpCountdownPulse;  // size 0x60

// accessories placed, as a 10x2 grid of icons on the sub screen
typedef struct DressUpCountIcons {
    SpriteResource *resources[4];             // 0x00, char, pltt, cell, anim
    Sprite *sprites[DRESS_UP_COUNT_ICON_NUM]; // 0x10
    int count;                                // 0x60
    u32 unk_64;                               // 0x64
} DressUpCountIcons;                          // size 0x68

// two-digit countdown (unused: DressUp_InitHud never sets DRESS_UP_HUD_COUNTDOWN)
typedef struct DressUpCountdown {
    SpriteResource *resources[4]; // 0x00
    Sprite *digits[2];            // 0x10
    Window *window;               // 0x18
    int value;                    // 0x1C, seconds shown
    int frames;                   // 0x20
    SysTask *tasks[2];            // 0x24, never created
    int *output;                  // 0x2C, [0] seconds left (set by the owner), [2] start value
    DressUpCountdownPulse pulse;  // 0x30
    BOOL warning;                 // 0x90, digits switched to the warning palette
} DressUpCountdown;               // size 0x94

// sub screen of the editor: message box, accessory count meter and countdown
typedef struct DressUpHud {
    DressUpBgScreen subBg;        // 0x000
    Window *msgWindow;            // 0x02C
    SpriteList *spriteList;       // 0x030
    GF_2DGfxResMan **resMans;     // 0x034
    DressUpCountIcons countIcons; // 0x038
    DressUpCountdown countdown;   // 0x0A0
    Window *infoWindow;           // 0x134
    u32 flags;                    // 0x138
    u32 frame;                    // 0x13C, Options frame
    u32 textFrameDelay;           // 0x140
    String *msgString;            // 0x144
} DressUpHud;                     // size 0x148

typedef struct DressUpWork {
    DressUpGfx gfx;                                                // 0x000
    u32 caseTable[DRESS_UP_ACCESSORY_NUM + DRESS_UP_BACKDROP_NUM]; // 0x184, owned count per accessory, then backdrop
                                                                   // per order (18 = none)
    DressUpNodePool nodePool;                                      // 0x35C
    DressUpSlotList *slotList;                                     // 0x364
    DressUpTray tray;                                              // 0x368
    DressUpCanvas canvas;                                          // 0x3F4
    DressUpBackdropAnim backdropAnim;                              // 0x47C
    DressUpTouch touch;                                            // 0x498
    DressUpBgScreen frameBg;                                       // 0x4B4
    DressUpButtonPanel buttonPanel;                                // 0x4E0
    DressUpHud hud;                                                // 0x568
    int uiState;                                                   // 0x6B0
    u32 transitionDone;                                            // 0x6B4
    YesNoPrompt *yesNoPrompt;                                      // 0x6B8
    Window *msgWindow;                                             // 0x6BC
    BOOL saveRequested;                                            // 0x6C0
    u8 unk_6C4[0x18];                                              // 0x6C4
    Options *options;                                              // 0x6DC
    u32 printerId;                                                 // 0x6E0
    u32 unk_6E4;                                                   // 0x6E4
    MessageFormat *msgFormat;                                      // 0x6E8, never set
    MenuInputState menuInputState;                                 // 0x6EC
} DressUpWork;                                                     // size 0x6F0

// the 0x6C argument block of the Accessory Portrait renderer: one saved design plus the caller's config
typedef struct AccessoryPortraitArgs {
    struct SaveFashionMon *mon; // 0x00
    u8 *items[20];              // 0x04, the placed accessories
    int count;                  // 0x54
    BgConfig *bgConfig;         // 0x58
    u32 background;             // 0x5C
    int x;                      // 0x60
    int y;                      // 0x64
    enum HeapID heapID;         // 0x68
} AccessoryPortraitArgs;

// renders a saved dress-up design (Accessory Portrait app, VS Recorder dress-up photos)
struct AccessoryPortrait {
    DressUpGfx gfx;                   // 0x000
    DressUpNodePool nodePool;         // 0x184
    DressUpSlotList *slotList;        // 0x18C
    DressUpCanvas canvas;             // 0x190
    DressUpBackdropAnim backdropAnim; // 0x218
    Pokemon *mon;                     // 0x234
    enum HeapID heapID;               // 0x238
    int x;                            // 0x23C
    int y;                            // 0x240
    int monOffsetX;                   // 0x244
    int monOffsetY;                   // 0x248
    u16 rotation;                     // 0x24C
    DressUpScale scale;               // 0x250
    u16 unk_25C;                      // 0x25C
    BOOL bgEnabled;                   // 0x260
    BOOL monVisible;                  // 0x264
}; // size 0x268

// work of the Accessory Portrait app
typedef struct AccessoryPortraitAppWork {
    SaveFashionDataSub *fashionData; // 0x000
    u32 unk_004;                     // 0x004
    int selection;                   // 0x008
    int mode;                        // 0x00C, stored, never read
    AccessoryPortrait *portrait;     // 0x010
    DressUpGfx gfx;                  // 0x014
    Sprite *icon;                    // 0x198
    Window *captionWindow;           // 0x19C
} AccessoryPortraitAppWork;          // size 0x1A0

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
