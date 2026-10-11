#ifndef POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H

#include "global.h"

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
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
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
    DressUpButton button;         // 0x00
    TextOBJ *textObj;             // 0x10
    UnkStruct_02021AC8 charAlloc; // 0x14, VRAM for the text
} DressUpTextButton;              // size 0x20

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
    int countMax;             // 0x10
    u32 unk_14;               // 0x14
    u32 unk_18;               // 0x18
    u32 unk_1C;               // 0x1C
    u32 unk_20;               // 0x20
    NARC *narc;               // 0x24
} DressUpHudTemplate;

// message box, accessory count meter and countdown
typedef struct DressUpHud {
    u8 unk_000[0x148];
} DressUpHud;

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

DressUpSlotList *DressUpGfx_CreateSlotList(int count, enum HeapID heapID);
void DressUpGfx_FreeSlotList(DressUpSlotList *list);
DressUpSlot *DressUpGfx_AddSlotObject(DressUpSlotTemplate *template);
BOOL DressUpGfx_IsTouchHeldOnSlot(DressUpSlot *slot);
BOOL DressUpGfx_HitTestSlotPixels(DressUpSlot *slot, int *x, int *y, NNSG2dCharacterData *hitMap);
void DressUpGfx_SetSlotPos(DressUpSlot *slot, s16 x, s16 y);
void DressUpGfx_GetSlotPos(DressUpSlot *slot, s32 *x, s32 *y);
void DressUpGfx_GetSlotSize(DressUpSlot *slot, s32 *width, s32 *height);
void DressUpGfx_SetSlotVisible(DressUpSlot *slot, BOOL visible);
void DressUpGfx_SetSlotPriority(DressUpSlot *slot, u32 depth);
void ov41_02246020(DressUpSlot *slot, int *x, int *y);
DressUpSlot *DressUpGfx_FindFreeSlot(DressUpSlotList *list);
GraphicsObject *DressUpGfx_CreateSlotGraphicsObject(const DressUpSlotTemplate *template);
void DressUpGfx_GetSlotHitbox(DressUpSlot *slot, TouchscreenHitbox *hitbox);
void DressUpGfx_InitDisplay(void);
void DressUpGfx_ResetDisplay(void);
void DressUp_InitGfxCore(DressUpGfx *gfx);
void DressUp_FreeGfxCore(DressUpGfx *gfx);
void DressUp_DrawFrame3D(DressUpGfx *gfx);
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
void DressUp_LoadRawResTables(DressUpGfx *gfx, DressUpGfxResources *resources);
void DressUpGfx_DestroyRawResTables(DressUpGfxResources *resources);
NNSG2dCharacterData *DressUpGfx_StoreUnpackedChar(DressUpGfx *gfx, void *data, int index);
void DressUp_DisableDragWindow(void);
void DressUp_EnableDragWindow(void);
void DressUp_VBlank(DressUpGfx *gfx);
void AccessoryPortrait_AllocRawResTables(DressUpGfxResources *resources, enum HeapID heapID);
int DressUpGfx_CharPixelEquals(NNSG2dCharacterData *character, int x, int y, int value);
void AccessoryPortrait_InitGfxCore(DressUpGfx *gfx, GraphicsManagerTemplate *template, enum HeapID heapID);
void AccessoryPortrait_InitBg(DressUpGfx *gfx, BgConfig *bgConfig, enum HeapID heapID);
void AccessoryPortrait_FreeGfxCore(DressUpGfx *gfx);
void AccessoryPortrait_FreeBg(DressUpGfx *gfx);
void DressUpGfx_HandlePokepicLoad(DressUpGfx *gfx);
void AccessoryPortrait_DrawTransformed(DressUpGfx *gfx, int x, int y, u16 angle, const DressUpScale *scale);
void AccessoryPortrait_InitAppGfx(DressUpGfx *gfx);
void AccessoryPortrait_FreeAppGfx(DressUpGfx *gfx);
void AccessoryPortrait_AppVBlank(DressUpGfx *gfx);
void AccessoryPortrait_RenderSprites(DressUpGfx *gfx);
void DressUpGfx_SetVramBanks(void);
void DressUpGfx_Init3D(void);
void DressUpGfx_Init2DModes(void);
void DressUpGfx_ResetFrameVram(void);
void DressUpGfx_DisablePlanes(void);
void DressUpGfx_CreateGraphicsManager(DressUpGfx *gfx, GraphicsManagerTemplate *template);
void DressUpGfx_DestroyGraphicsManager(DressUpGfx *gfx);
void DressUpGfx_DrawGraphicsManager(DressUpGfx *gfx);
void DressUpGfx_AddImages(DressUpGfx *gfx, GraphicsImageTemplate *templates, int count);
void DressUpGfx_AddPalettes(DressUpGfx *gfx, GraphicsPaletteTemplate *templates, int count);
void DressUpGfx_InitBgs(DressUpGfx *gfx);
void DressUpGfx_FreeBgs(DressUpGfx *gfx);
void DressUp_CreateCharRawResMan(DressUpGfx *gfx);
void DressUp_DestroyCharRawResMan(DressUpGfx *gfx);
void DressUpGfx_InitSpriteSystem(DressUpGfx *gfx);
void DressUpGfx_FreeSpriteSystem(DressUpGfx *gfx);
void DressUpGfx_RenderSprites(DressUpGfx *gfx);
void DressUp_LoadAccessoryChars(DressUpGfx *gfx, DressUpGfxResources *resources);
void DressUp_LoadBackdropCharsAndPalettes(DressUpGfx *gfx, DressUpGfxResources *resources);
void ov41_02246C90(DressUpGfx *gfx, enum HeapID heapID);
void ov41_02246CB0(DressUpGfx *gfx);
void DressUpGfx_CreatePokepicManager(DressUpGfx *gfx, enum HeapID heapID, u32 texSize, u32 plttSize);
void DressUpGfx_DestroyPokepicManager(DressUpGfx *gfx);
void DressUpGfx_AllocRawResTables(DressUpGfxResources *resources, int imageCount, int paletteCount, enum HeapID heapID);
void DressUpGfx_FreeRawResTables(DressUpGfxResources *resources);

// dress_up.c
BOOL DressUp_Init(OverlayManager *man, int *state);
BOOL DressUp_Main(OverlayManager *man, int *state);
BOOL DressUp_Exit(OverlayManager *man, int *state);
void DressUp_InitGraphics(DressUpWork *work);
void DressUp_DrawFrame(DressUpWork *work);
void DressUp_FreeGraphics(DressUpWork *work);
void DressUp_InitCanvas(DressUpWork *work, Pokemon *mon, int limit, BOOL altMonPic);
void DressUp_FreeCanvas(DressUpWork *work);
void DressUp_InitTray(DressUpWork *work);
void DressUp_FreeTray(DressUpWork *work);
void DressUp_RestoreEditBgLayout(DressUpWork *work);
void DressUp_VBlankCB(void *arg);
void DressUp_LoadFrameBg(DressUpWork *work, int frame);
void DressUp_FreeFrameBg(DressUpWork *work);
void DressUp_InitButtons(DressUpWork *work);
void DressUp_FreeButtons(DressUpWork *work);
void DressUp_UpdateButtons(DressUpWork *work);
void ov41_02247588(DressUpWork *work);
void DressUp_OnDoneButton(void *button, void *arg);
void DressUp_OnNextPageButton(void *button, void *arg);
void DressUp_OnPrevPageButton(void *button, void *arg);
void DressUp_OnAccessoryTabButton(void *button, void *arg);
void DressUp_OnBackdropTabButton(void *button, void *arg);
void DressUp_InitHud(DressUpWork *work, Options *options);
void DressUp_FreeHud(DressUpWork *work);
void DressUp_StartFinishTransition(DressUpWork *work, u32 *done);
void DressUp_StartReturnTransition(DressUpWork *work, u32 *done);
void DressUp_SaveDesign(SaveFashionDataSub *entry, DressUpCanvas *canvas, PlayerProfile *profile);
void DressUp_ShowDressedUpPrompt(DressUpWork *work);
void DressUp_HidePrompt(DressUpWork *work);
int DressUp_HandleDressedUpPrompt(DressUpWork *work);
void DressUp_CreateMsgWindow(DressUpWork *work, int msgId, u8 x, u8 y, u8 width, u8 height);
void DressUp_PrintMsgWindow(DressUpWork *work, int msgId);
void DressUp_RemoveMsgWindow(DressUpWork *work);
void DressUp_CreatePromptMsgWindow(DressUpWork *work, int msgId);
void ov41_02247D34(DressUpWork *work, int msgId);
void ov41_02247D3C(DressUpWork *work);
void DressUp_UpdateCountMeter(DressUpWork *work);

// dress_up_canvas.c
void DressUp_ShowSavePrompt(DressUpWork *work);
int DressUp_HandleSavePrompt(DressUpWork *work);
void DressUpCanvas_GetRectSamplePoint(u32 top, u32 bottom, u32 left, u32 right, u32 index, u32 *outX, u32 *outY);
void DressUpCanvas_Init(DressUpCanvas *canvas, const DressUpCanvasTemplate *template);
void DressUpCanvas_Destroy(DressUpCanvas *canvas);
void AccessoryPortrait_CreateCanvasMonPic(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, int x, int y, int priority, enum HeapID heapID);
void DressUpCanvas_CreateMonPic(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID);
void ov41_02247FFC(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID);
void ov41_02248020(DressUpCanvas *canvas, DressUpNode *node);
void ov41_02248030(DressUpNode *node);
void DressUpCanvas_FreeMonPic(DressUpCanvas *canvas);
BOOL AccessoryPortrait_PlaceCanvasAccessory(DressUpCanvas *canvas, int accessory, int x, int y, int priority);
BOOL DressUpCanvas_TryAddAccessory(DressUpCanvas *canvas, DressUpNode *node, NNSG2dCharacterData **hitMaps);
void DressUpCanvas_RemoveAccessory(DressUpCanvas *canvas, DressUpNode *node);
void DressUpCanvas_ClearObjects(DressUpCanvas *canvas);
void DressUpCanvas_MoveAll(DressUpCanvas *canvas, int dx, int dy);
void DressUpCanvas_MoveMonGroup(DressUpCanvas *canvas, int dx, int dy);
void DressUpCanvas_SetOrigin(DressUpCanvas *canvas, int x, int y, int newX, int newY);
void DressUpCanvas_RefreshPriorities(DressUpCanvas *canvas);
void ov41_02248164(DressUpCanvas *canvas, int priority);
BOOL DressUpCanvas_IsTouchHeldIn(DressUpCanvas *canvas);
BOOL DressUpCanvas_IsPointIn(DressUpCanvas *canvas, u32 x, u32 y);
DressUpNode *DressUpCanvas_FindObjectAt(DressUpCanvas *canvas, int *x, int *y, NNSG2dCharacterData **hitMaps);
void DressUpCanvas_SetBackdrop(DressUpCanvas *canvas, int backdrop, enum HeapID heapID);
void DressUpCanvas_FreeBackdrop(DressUpCanvas *canvas);
int DressUpCanvas_GetCount(DressUpCanvas *canvas);
void DressUpCanvas_GetMaxOverflow(DressUpCanvas *canvas, int *outX, int *outY);
void DressUpCanvas_LinkByMonOverlap(DressUpCanvas *canvas, DressUpNode *node, BOOL atFront, NNSG2dCharacterData **hitMaps);

// dress_up_tray.c
void DressUpCanvas_GetPushback(DressUpNode *node, int *outX, int *outY);
void DressUpTray_Init(DressUpTray *tray, const DressUpTrayTemplate *template);
void DressUpTray_Destroy(DressUpTray *tray);
int DressUpTray_GetItemPage(int mode, int id, u32 *caseTable);
void DressUpTray_GetItemSpawnPos(int mode, int id, int *outX, int *outY, int width, int height, u32 *caseTable);
void DressUpTray_AddItem(DressUpTray *tray, int mode, int id);
void DressUpTray_LinkObjectToPage(DressUpTray *tray, int mode, int page, DressUpNode *node);
void ov41_022486F0(DressUpNode *node);
void DressUpTray_ClearObjects(DressUpTray *tray);
void DressUpTray_RefreshPriorities(DressUpTray *tray);
BOOL DressUpTray_AnimateToPage(DressUpTray *tray, int group, int page);
int DressUpTray_CyclePage(DressUpTray *tray, int group, BOOL reverse);
void DressUpTray_SetPageImmediate(DressUpTray *tray, int group, int page);
BOOL DressUpTray_IsTouchHeldIn(DressUpTray *tray);
BOOL DressUpTray_IsPointIn(DressUpTray *tray, u32 x, u32 y);
DressUpNode *DressUpTray_FindObjectAt(DressUpTray *tray, int *x, int *y, NNSG2dCharacterData **hitMaps);
void DressUpTray_LoadTabBg(DressUpTray *tray, int tab);
void ov41_022488D8(DressUpTray *tray, int tab, u32 flags, int frames, BOOL *done);
void DressUpTray_FreeTabBg(DressUpTray *tray);
int DressUpTray_GetGroup(DressUpTray *tray);
int DressUpTray_GetGroupPage(DressUpTray *tray, int group);
void DressUpTray_MovePageObjects(DressUpTray *tray, int group, int page, int dx, int dy);
BOOL DressUpTray_IsIdle(DressUpTray *tray);
void DressUpTray_InitPageLists(DressUpTray *tray, const DressUpTrayTemplate *template);
void DressUpTray_SetListVisible(DressUpNode *head, BOOL visible);
void DressUpTray_SetPageVisible(DressUpTray *tray, int group, int page, BOOL visible);
void DressUpTray_SetCurPageVisible(DressUpTray *tray, BOOL visible);
void DressUpTray_AllocPageTable(DressUpTrayGroup *group, int count);
void DressUpTray_FreePageTable(DressUpTrayGroup *group);
DressUpNode *DressUpTray_GetCurPageHead(DressUpTray *tray);
DressUpNode *DressUpTray_GetPageHead(DressUpTray *tray, int group, int page);
int DressUpTray_CountPageObjects(DressUpTray *tray, int group, int page);
DressUpNode *DressUpTray_GetPageObject(DressUpTray *tray, int group, int page, int index);
void DressUpTray_SyncObjectVisible(DressUpTray *tray, DressUpSlot *slot, int group, int page);
void DressUpTray_GetGridPos(int index, int *x, int *y);
void DressUpTray_StartPageSwapTask(DressUpTray *tray, int fromGroup, int fromPage, int toGroup, int toPage);
void DressUp_StartSlideAnim(DressUpNode *node, DressUpSlideAnim *anims, int count);
DressUpSlideAnim *DressUp_FindFreeSlideAnim(DressUpSlideAnim *anims, int count);
void DressUp_TickSlideAnims(DressUpSlideAnim *anims, int count);
void DressUp_TickSlideAnim(DressUpSlideAnim *anim);

// dress_up_touch.c
int ov41_02248E10(int value, int divisor);
void DressUpTouch_Reset(DressUpTouch *touch);
void DressUpTouch_Update(DressUpTouch *touch);
void ov41_02248E80(DressUpTouch *touch);
void DressUp_LoadFashionCaseTable(FashionCase *fashionCase, u32 *caseTable);
u32 DressUp_GetAccessoryCount(u32 *caseTable, u32 accessory);
u32 DressUp_GetBackdropByOrder(u32 *caseTable, u32 order);
int DressUp_GetBackdropOrder(u32 *caseTable, u32 backdrop);
void DressUpDrag_Init(DressUpTouch *touch, DressUpCanvas *canvas, DressUpTray *tray, DressUpGfx *gfx, DressUpHud *hud, BOOL allowMonDrag);
void DressUpDrag_Destroy(DressUpTouch *touch);
void DressUpDrag_OnTouchIdle(DressUpTouch *touch);
void ov41_022490AC(DressUpTouch *touch);
void DressUpDrag_OnReleaseBackdrop(DressUpTouch *touch);
void DressUpDrag_OnReleaseAccessory(DressUpTouch *touch);
void DressUpDrag_OnHeldAccessory(DressUpTouch *touch);
void DressUpDrag_OnReleaseMon(DressUpTouch *touch);
void DressUpDrag_OnHeldMon(DressUpTouch *touch);
void DressUpDrag_ApplyBackdrop(DressUpTouch *touch);
void DressUpDrag_BeginDrag(DressUpDrag *drag, DressUpNode *node, BOOL fromCanvas, int grabX, int grabY, int priority);
void DressUpDrag_ClearSelection(DressUpDrag *drag);
void DressUpDrag_GetSelectedRect(DressUpTouch *touch, int *top, int *bottom, int *left, int *right);
void DressUpDrag_GetSelectedVisibleRect(DressUpTouch *touch, int *top, int *bottom, int *left, int *right);
void DressUp_StartTweenToTray(DressUpDrag *drag, int frames, int destX, int destY, int mode, int page);
void DressUp_PrintSelectedItemName(DressUpDrag *drag);
void DressUp_MoveMonWithAccessories(DressUpTouch *touch, int x, int y);
void DressUp_LoadMonHitMap(DressUpGfx *gfx, const PokepicTemplate *template);

// dress_up_objects.c
void DressUpMon_Create(DressUpMonPic *monPic, PokepicManager *pokepicManager, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID);
void DressUpMon_CreateEx(DressUpMonPic *monPic, PokepicManager *pokepicManager, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID, int mode);
void DressUpMon_Delete(DressUpMonPic *monPic);
void DressUpMon_SetPriority(DressUpMonPic *monPic, int priority);
int DressUpMon_GetPriority(DressUpMonPic *monPic);
void DressUpMon_SetCenterPos(DressUpMonPic *monPic, int x, int y);
BOOL DressUpMon_IsTouchHeldIn(DressUpMonPic *monPic);
BOOL DressUpMon_IsPointIn(DressUpMonPic *monPic, u32 x, u32 y);
void DressUpMon_GetCenterPos(DressUpMonPic *monPic, int *x, int *y);
void DressUpMon_GetSize(DressUpMonPic *monPic, int *width, int *height);
BOOL DressUpMon_HitTestTouch(DressUpMonPic *monPic, int *x, int *y, NNSG2dCharacterData *hitMap);
BOOL DressUpMon_HitTestPoint(DressUpMonPic *monPic, int x, int y, NNSG2dCharacterData *hitMap);
void DressUpMon_GetMargins(DressUpMonPic *monPic, u8 *margins);
int DressUpMon_FindPicLeftEdge(const s8 *pixels, int stride);
void DressUpMon_CalcPicMargins(s8 *pixels, int stride, int height, u8 *margins);
void DressUpMon_MakeClampedRect(TouchscreenHitbox *hitbox, int x, int y, int halfWidth, int halfHeight);
void DressUpNodePool_Init(DressUpNodePool *pool, int count, enum HeapID heapID);
void DressUpNodePool_Free(DressUpNodePool *pool);
DressUpNode *DressUpNodePool_Acquire(DressUpNodePool *pool, void *object, u32 mode);
void DressUpList_InitNode(DressUpNode *node);
void DressUpList_InsertAfter(DressUpNode *node, DressUpNode *at);
void DressUpList_Remove(DressUpNode *node);
void DressUpList_Clear(DressUpNode *head);
void DressUpObj_SetPriority(DressUpNode *node, int priority);
BOOL DressUpObj_HitTest(DressUpNode *node, int *x, int *y, NNSG2dCharacterData **hitMaps);
void DressUpObj_SetPos(DressUpNode *node, int x, int y);
void DressUpObj_GetPos(DressUpNode *node, int *x, int *y);
void DressUpObj_GetSize(DressUpNode *node, int *width, int *height);
void DressUpObj_GetMargins(DressUpNode *node, int *left, int *right, int *top, int *bottom);
void DressUpObj_MoveListBy(DressUpNode *head, int dx, int dy);
void DressUpObj_GetVisibleRect(DressUpNode *node, int *top, int *bottom, int *left, int *right);
void DressUpBgScreen_LoadFromTemplate(DressUpBgScreen *screen, const DressUpBgScreenTemplate *template);
void DressUpBgScreen_Unload(DressUpBgScreen *screen);
void DressUpBgScreen_InitAltAnim(DressUpBgScreenAnim *anim, const DressUpBgScreenAnimTemplate *template);
void DressUpBgScreen_UpdateAltAnim(DressUpBgScreenAnim *anim, int ticks);
void DressUpBgScreen_StopAltAnim(DressUpBgScreenAnim *anim);
void DressUp_StartBgSlide(DressUpBgScreen *screen, DressUpBgScreenTemplate *template, int dx, int dy, int frames, BOOL *done);
void DressUpBgScreen_AddCharOffset(NNSG2dScreenData *screenData, int charOffset);
void DressUpBgScreen_Load(const DressUpBgScreenTemplate *template, int *outWidth, int *outHeight);
void DressUpBgScreen_CopyToTilemap(BgConfig *bgConfig, int layer, NNSG2dScreenData *screenData, int width, int height, int x, int y, int charOffset, int palette);
SysTask *DressUpBgScreen_QueueLoadChars(NarcId narcId, int fileId, BgConfig *bgConfig, u32 layer, u32 tileStart, enum HeapID heapID);
void DressUpBgScreen_QueueLoadPltt(NarcId narcId, int fileId, u32 location, u32 offset, u32 size, enum HeapID heapID);
void DressUpButton_Init(DressUpButton *button, const DressUpButtonTemplate *template);
void DressUpButton_InitWithText(DressUpTextButton *button, const DressUpTextButtonTemplate *template);

// dress_up_buttons.c
void DressUpButton_Delete(DressUpButton *button);
void DressUpButton_DeleteWithText(DressUpTextButton *button);
void DressUpButton_TryCallback(DressUpButton *button, u32 state);
void DressUp_UpdateCategoryTabs(DressUpButtonPanel *panel, int selection, int state);
void DressUpButton_SetCallback(DressUpButton *button, DressUpButtonCallback callback, void *arg, u32 trigger);
void ov41_0224A254(void *button, void *arg);
void DressUpButton_SetAnimSelected(DressUpButton *button);
void DressUpButton_SetAnimNormal(DressUpButton *button);
void DressUpButton_SetAnimPressed(DressUpButton *button);
void DressUpButtonPanel_Create(DressUpButtonPanel *panel, DressUpGfx *gfx, MenuInputState *inputState);
void DressUpButtonPanel_Destroy(DressUpButtonPanel *panel, DressUpGfx *gfx);
void DressUpButtonPanel_HandleKeys(DressUpButtonPanel *panel);
int DressUpButtonPanel_UpdateInputMode(DressUpButtonPanel *panel);
void DressUpButtonPanel_Update(DressUpButtonPanel *panel);
void ov41_0224A580(DressUpButtonPanel *panel);
void DressUpButtonPanel_MoveBy(DressUpButtonPanel *panel, int dx, int dy);
void DressUpButtonPanel_SetCallback(DressUpButtonPanel *panel, int button, DressUpButtonCallback callback, void *arg, u32 trigger);
void DressUpButtonPanel_OnTouch(u32 which, u32 state, void *arg);
void DressUpButton_CreateSprite(DressUpButton *button, int resId, DressUpGfx *gfx, int x, int y, int width, int height);
void DressUpButton_CreateTextSprite(DressUpTextButton *button, int resId, DressUpGfx *gfx, FontSystem *fontSystem, Window *label, int x, int y, int width, int height);
void DressUp_SetHitboxRect(TouchscreenHitbox *hitboxes, int i, int x, int y, int width, int height);
void DressUpButtonPanel_LoadResources(DressUpGfx *gfx);
void DressUpButtonPanel_FreeResources(DressUpGfx *gfx);
void DressUpButton_SetAnimByTouchState(DressUpButton *button, u32 state);
void DressUpButton_SetTextByState(DressUpTextButton *button, u32 state);

// HUD, backdrop animation (next objects)
void DressUpButton_PlaySE(DressUpButton *button, u32 se, u32 trigger, u32 state);
Window *DressUpButton_CreateTextBitmap(DressUpGfx *gfx, NarcId narcId, int bank, int msgId, int width, int height);
void DressUpButton_DeleteTextBitmap(Window *window);
void DressUpButton_MoveBy(DressUpButton *button, int dx, int dy);
void DressUpButton_MoveTextBy(DressUpTextButton *button, int dx, int dy);
void DressUpHud_Init(DressUpHud *hud, const DressUpHudTemplate *template, u32 flags);
void DressUpHud_Free(DressUpHud *hud);
void DressUpHud_UpdateCountdown(DressUpHud *hud);
void DressUpHud_PrintMsgInstant(DressUpHud *hud, NarcId narcId, int bank, int msgId);
u8 DressUpHud_PrintMsg(DressUpHud *hud, NarcId narcId, int bank, int msgId);
void DressUpHud_FreeMsgString(DressUpHud *hud);
void DressUpHud_SetAccessoryCount(DressUpHud *hud, int count);
void DressUp_StartBackdropAnim(DressUpBackdropAnim *anim, DressUpCanvas *canvas, enum HeapID heapID);
void DressUp_UpdateBackdropAnim(DressUpBackdropAnim *anim);
void DressUp_StopBackdropAnim(DressUpBackdropAnim *anim);

#endif // POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H
