#include "ball_capsule_editor.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/seals.h"
#include "constants/sndseq.h"

#include "battle/battle.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0011.h"

#include "bg_window.h"
#include "font.h"
#include "game_stats.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_07.h"
#include "palette.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokemon_icon_idx.h"
#include "pokepic.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "font_sprite.h"
#include "particle_system.h"
#include "touchpad.h"
#include "swap_3d_buffers.h"
#include "unk_02091054.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

FS_EXTERN_OVERLAY(OVY_6);
FS_EXTERN_OVERLAY(OVY_7);

#define BALL_CAPSULE_EDITOR_NO_MON         0xFF // capsule not on a party Pokémon
#define BALL_CAPSULE_EDITOR_NONE           0xFF // no seal held / no slot free
#define BALL_CAPSULE_EDITOR_SEALS_PER_PAGE 8
#define BALL_CAPSULE_EDITOR_MAX_PAGES      10

// Touch hitboxes (sBallCapsuleEditorHitboxes, then the placed seals): the case's seals, the page
// arrows, the three buttons, the placed seals.
#define BALL_CAPSULE_EDITOR_HITBOX_PREV_PAGE  8
#define BALL_CAPSULE_EDITOR_HITBOX_NEXT_PAGE  9
#define BALL_CAPSULE_EDITOR_HITBOX_PREVIEW    10
#define BALL_CAPSULE_EDITOR_HITBOX_OK         11
#define BALL_CAPSULE_EDITOR_HITBOX_CANCEL     12
#define BALL_CAPSULE_EDITOR_HITBOX_FIRST_SEAL 13
#define BALL_CAPSULE_EDITOR_NUM_HITBOXES      21

// Capsule grid input results (BallCapsuleEditor_HandleSelection / HandleSwitchSelection)
#define BALL_CAPSULE_EDITOR_SELECT_BACK    12
#define BALL_CAPSULE_EDITOR_SELECT_CONFIRM 13

// Main modes, sBallCapsuleEditorModes
enum BallCapsuleEditorMode {
    BALL_CAPSULE_EDITOR_MODE_SELECT,
    BALL_CAPSULE_EDITOR_MODE_EDIT,
    BALL_CAPSULE_EDITOR_MODE_REMOVE,
    BALL_CAPSULE_EDITOR_MODE_SWITCH,
    BALL_CAPSULE_EDITOR_MODE_NONE,
};

typedef struct BallCapsuleEditorCapsule {
    u32 partySlot;    // 0x0, party slot of the Pokémon the capsule is on, or BALL_CAPSULE_EDITOR_NO_MON
    CAPSULE *capsule; // 0x4
} BallCapsuleEditorCapsule;

typedef struct BallCapsuleEditorCapsuleIcon {
    u32 partySlot;         // 0x0, copy of BallCapsuleEditorCapsule.partySlot
    BOOL hasSeals;         // 0x4, also the icon's animation
    ManagedSprite *sprite; // 0x8
} BallCapsuleEditorCapsuleIcon;

// A seal placed on the capsule being edited.
typedef struct BallCapsuleEditorSeal {
    BOOL active;               // 0x0
    SEAL seal;                 // 0x4
    u8 filler_7;               // 0x7
    ManagedSprite *sprite;     // 0x8
    TouchscreenHitbox *hitbox; // 0xC, one of BallCapsuleEditorDisplay.hitboxes
} BallCapsuleEditorSeal;

// Passed on its own to BallCapsuleEditor_InitSpriteSystem / FreeSpriteSystem.
typedef struct BallCapsuleEditorDisplay {
    u32 heldSeal;                                                 // 0x000 (0x0D4), the placed seal being dragged, or BALL_CAPSULE_EDITOR_NONE
    BOOL inputEnabled;                                            // 0x004 (0x0D8)
    SpriteSystem *spriteSystem;                                   // 0x008 (0x0DC)
    SpriteManager *spriteManager;                                 // 0x00C (0x0E0)
    BgConfig *bgConfig;                                           // 0x010 (0x0E4)
    PaletteData *paletteData;                                     // 0x014 (0x0E8)
    Window windows[13];                                           // 0x018 (0x0EC), 0: message, 3: top screen text, 5-12: seal counts
    ListMenuItem *menuItems;                                      // 0x0E8 (0x1BC)
    u8 unk_0EC[4];                                                // 0x0EC (0x1C0)
    PokepicManager *pokepicManager;                               // 0x0F0 (0x1C4)
    Pokepic *pokepic;                                             // 0x0F4 (0x1C8)
    int pokepicY;                                                 // 0x0F8 (0x1CC)
    u8 unk_0FC[0x1C];                                             // 0x0FC (0x1D0)
    TouchHitboxController *hitboxController;                      // 0x118 (0x1EC)
    TouchscreenHitbox hitboxes[BALL_CAPSULE_EDITOR_NUM_HITBOXES]; // 0x11C (0x1F0)
    YesNoPrompt *yesNoPrompt;                                     // 0x170 (0x244)
    TouchscreenListMenuSpawner *menuSpawner;                      // 0x174 (0x248)
} BallCapsuleEditorDisplay;                                       // size: 0x178

typedef struct BallCapsuleEditor {
    BallCapsuleEditorArgs *args;                             // 0x000
    BallCapsuleEditorCapsule capsules[MAX_CAPSULES];         // 0x004
    SEALBAG *sealBag;                                        // 0x064
    CAPSULE savedCapsule;                                    // 0x068, the edited capsule as it was before editing
    u8 savedSealCounts[SEAL_MAX + 1];                        // 0x080, the seal bag before editing
    u8 filler_D1[3];                                         // 0x0D1
    BallCapsuleEditorDisplay display;                        // 0x0D4
    TouchscreenListMenu *menu;                               // 0x24C
    void *pokepicEffect;                                     // 0x250, BallSealEffect_New
    UnkBallData *ballData;                                   // 0x254
    GF3DVramMan *vramMan;                                    // 0x258
    FontSystem *fontSystem;                          // 0x25C
    TextOBJ *buttonLabels[2];                                // 0x260, OK / CANCEL
    ObjCharSpaceReservation buttonLabelChars[2];                  // 0x268
    u32 unk_280;                                             // 0x280
    void *pokepicAnim;                                       // 0x284, PokepicAnimManager_New
    BOOL sortSeals;                                          // 0x288
    BallCapsuleEditorCapsuleIcon capsuleIcons[MAX_CAPSULES]; // 0x28C
    ManagedSprite *cursors[2];                               // 0x31C
    ManagedSprite *monIcons[PARTY_SIZE];                     // 0x324
    ManagedSprite *arrows[4];                                // 0x33C
    BallCapsuleEditorSeal seals[MAX_SEALS_ON_CAPSULE];       // 0x34C
    u8 unk_3CC[0x20];                                        // 0x3CC
    int cursorCapsule[2];                                    // 0x3EC, 0: the selected capsule, 1: the capsule to switch with
    u32 mode;                                                // 0x3F4
    u32 unk_3F8;                                             // 0x3F8
    u32 state;                                               // 0x3FC
    u32 returnState;                                         // 0x400
    int subState;                                            // 0x404
    int subStateValue;                                       // 0x408, Yes/No answer or frame counter
    MenuInputState menuInputState;                           // 0x40C
    int timer;                                               // 0x410
    ManagedSprite *caseSprites[13];                          // 0x414, 0-7: seals of the page, 8-12: page arrows and buttons
    int page;                                                // 0x448
    int pageCount;                                           // 0x44C
    u8 pageSeals[BALL_CAPSULE_EDITOR_SEALS_PER_PAGE];        // 0x450, seal ids shown on the page (0: none)
    Pokemon *previewMon;                                     // 0x458
    NARC *pokepicAnimNarc;                                   // 0x45C
} BallCapsuleEditor;                                         // size: 0x460

typedef struct BallCapsuleEditorMenuItem {
    u32 msgId;
    BOOL (*func)(BallCapsuleEditor *editor);
} BallCapsuleEditorMenuItem;

typedef struct BallCapsuleEditorPosition {
    s16 x;
    s16 y;
} BallCapsuleEditorPosition;

typedef struct BallCapsuleEditorArrow {
    int x;
    int y;
    int flipMode;
} BallCapsuleEditorArrow;

// Button press animation task
typedef struct BallCapsuleEditorButtonPress {
    u32 state;
    ManagedSprite *button;
    TextOBJ *label;
} BallCapsuleEditorButtonPress;

typedef struct BallCapsuleEditorMainBgTemplates {
    BgTemplate templates[3];
} BallCapsuleEditorMainBgTemplates;

typedef struct BallCapsuleEditorSubBgTemplates {
    BgTemplate templates[4];
} BallCapsuleEditorSubBgTemplates;

typedef struct BallCapsuleEditorCaseSealPositions {
    BallCapsuleEditorPosition positions[BALL_CAPSULE_EDITOR_SEALS_PER_PAGE];
} BallCapsuleEditorCaseSealPositions;

typedef struct BallCapsuleEditorArrows {
    BallCapsuleEditorArrow arrows[4];
} BallCapsuleEditorArrows;

typedef struct BallCapsuleEditorCasePositions {
    BallCapsuleEditorPosition positions[13];
} BallCapsuleEditorCasePositions;

// The capsule as SEALs (SealCase_SetCapsuleI argument).
typedef struct BallCapsuleEditorSealData {
    u8 bytes[sizeof(SEAL) * MAX_SEALS_ON_CAPSULE];
} BallCapsuleEditorSealData;

// ManagedSprite_SetDrawPriority / ManagedSprite_SetFlipMode as called here: the priority and flip mode are passed
// without the u16 / u8 truncation the prototypes in sprite_system.h imply.
typedef void (*BallCapsuleEditorSetDrawPriorityFunc)(ManagedSprite *managedSprite, u32 priority);
typedef void (*BallCapsuleEditorSetFlipModeFunc)(ManagedSprite *managedSprite, int flipMode);

// Defined in OVY_7; not declared in a header.
void *BallSealEffect_New(enum HeapID heapID, void *params);
void BallSealEffect_LoadResources(void *effect);
BOOL BallSealEffect_IsLoaded(void *effect);
void BallSealEffect_Start(void *effect);
BOOL BallSealEffect_IsActive(void *effect);
void BallSealEffect_Free(void *effect);
BOOL UnkBallData_IsActive(UnkBallData *data);
BOOL UnkBallData_HasOpened(UnkBallData *data);
s32 BallSealEffect_GetDistance(s32 x1, s32 y1, s32 x2, s32 y2);

// Defined in pokepic_animation.c / render_window.c; not declared in a public header.
void *PokepicAnimManager_New(enum HeapID heapID, int count, int flag);
void PokepicAnimManager_Delete(void *mgr);
BOOL PokepicAnimManager_GetAnimState(void *mgr, int index);
u32 GetUserFrameGfx2PaletteMember(u32 frame);

// BallSealEffect_New argument
typedef struct BallCapsuleEditorPokepicEffectParams {
    s32 kind;
    s32 unused;
    Pokemon *mon;
} BallCapsuleEditorPokepicEffectParams;

extern const GraphicsModes sBallCapsuleEditorGraphicsModes;
extern const GraphicsBanks sBallCapsuleEditorGraphicsBanks;
extern const BallCapsuleEditorMainBgTemplates sBallCapsuleEditorMainBgTemplates;
extern const BallCapsuleEditorSubBgTemplates sBallCapsuleEditorSubBgTemplates;
extern const TouchscreenListMenuTemplate sBallCapsuleEditorMenuTemplate;
extern const OamCharTransferParam sBallCapsuleEditorOamCharTransferParam;
extern const SpriteResourceCountsListUnion sBallCapsuleEditorSpriteResourceCounts;
extern const OamManagerParam sBallCapsuleEditorOamManagerParam;
extern const BallCapsuleEditorCaseSealPositions sBallCapsuleEditorCaseSealPositions;
extern const u32 sBallCapsuleEditorWindowFrameTiles[];
extern const BallCapsuleEditorArrows sBallCapsuleEditorArrows;
extern const BallCapsuleEditorCasePositions sBallCapsuleEditorCaseButtonPositions;
extern const BallCapsuleEditorCasePositions sBallCapsuleEditorCaseButtonHitboxSizes;
extern const TouchscreenHitbox sBallCapsuleEditorExitButtonHitboxes[];
extern BOOL (*const sBallCapsuleEditorModes[])(BallCapsuleEditor *editor);
extern const BallCapsuleEditorMenuItem sBallCapsuleEditorMenuItems[];
extern const TouchscreenHitbox sBallCapsuleEditorHitboxes[];

void BallCapsuleEditor_LoadBattleOverlays(void);
void BallCapsuleEditor_ResetDisplay(enum HeapID heapID);
void BallCapsuleEditor_DisableInterrupts(enum HeapID heapID);
void BallCapsuleEditor_InitBgs(BgConfig *bgConfig);
void BallCapsuleEditor_VBlankCB(void *data);
u8 BallCapsuleEditor_GetArgsCapsuleIndex(BallCapsuleEditorArgs *args);
void BallCapsuleEditor_SetArgsCapsuleIndex(BallCapsuleEditorArgs *args, u8 capsuleIndex);
void BallCapsuleEditor_SetArgsSetCapsuleOnMon(BallCapsuleEditorArgs *args, u8 setCapsuleOnMon);
void BallCapsuleEditor_SetSealHitbox(TouchscreenHitbox *hitbox, int x, int y);
void BallCapsuleEditor_SetDragHitbox(TouchscreenHitbox *hitbox);
void BallCapsuleEditor_UpdateSealHitbox(TouchscreenHitbox *hitbox, ManagedSprite *sprite, BOOL dragging);
void BallCapsuleEditor_ShiftSealsToCapsule(BallCapsuleEditor *editor);
void BallCapsuleEditor_LoadSeals(BallCapsuleEditor *editor);
u32 BallCapsuleEditor_AddSeal(BallCapsuleEditor *editor, u8 caseIndex);
BOOL BallCapsuleEditor_HasFreeSealSlot(BallCapsuleEditor *editor);
void BallCapsuleEditor_BringSealToFront(BallCapsuleEditor *editor, s32 index);
void BallCapsuleEditor_LoadSealSpriteResources(BallCapsuleEditor *editor, u32 kind, s32 index);
void BallCapsuleEditor_SortSeals(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_CreateSealSprite(BallCapsuleEditor *editor, u8 index);
BOOL BallCapsuleEditor_HandleSealTouch(BallCapsuleEditor *editor, u32 event, s32 index);
void BallCapsuleEditor_RemoveAllSeals(BallCapsuleEditor *editor);
void BallCapsuleEditor_SetSealsDrawFlag(BallCapsuleEditor *editor, BOOL draw);
BOOL BallCapsuleEditor_IsSealOnCapsule(BallCapsuleEditor *editor, s32 index);
void BallCapsuleEditor_RemoveSeal(BallCapsuleEditor *editor, s32 index);
void BallCapsuleEditor_UpdateSealOamMode(BallCapsuleEditor *editor, s32 index);
BOOL BallCapsuleEditor_IsCapsuleChanged(BallCapsuleEditor *editor);
void BallCapsuleEditor_StoreSealPosition(BallCapsuleEditor *editor, s32 index);
void BallCapsuleEditor_SaveCapsule(BallCapsuleEditor *editor);
void BallCapsuleEditor_GetSealData(SEAL *seals, BallCapsuleEditor *editor);
void BallCapsuleEditor_SetSpriteHitbox(TouchscreenHitbox *hitbox, ManagedSprite *sprite, s32 halfWidth, s32 halfHeight);
void BallCapsuleEditor_AddMessageWindows(BallCapsuleEditor *editor);
void BallCapsuleEditor_RemoveMessageWindows(BallCapsuleEditor *editor);
void BallCapsuleEditor_PrintExitLabel(BallCapsuleEditor *editor, BOOL show);
void BallCapsuleEditor_InitFonts(BallCapsuleEditor *editor);
void BallCapsuleEditor_FreeFonts(BallCapsuleEditor *editor);
void BallCapsuleEditor_LoadButtonLabelPalette(BallCapsuleEditor *editor);
void BallCapsuleEditor_CreateButtonLabels(BallCapsuleEditor *editor);
void BallCapsuleEditor_SetButtonLabelsDrawFlag(BallCapsuleEditor *editor, BOOL draw);
void BallCapsuleEditor_CreateButtonLabel(BallCapsuleEditor *editor, int index, int x, int y, int palette);
void BallCapsuleEditor_FillPageSeals(BallCapsuleEditor *editor, int page);
void BallCapsuleEditor_LoadCaseSealSpriteResources(BallCapsuleEditor *editor);
void BallCapsuleEditor_CreateCaseSealSprites(BallCapsuleEditor *editor);
void BallCapsuleEditor_DeleteCaseSealSprites(BallCapsuleEditor *editor);
void BallCapsuleEditor_MoveSeals(BallCapsuleEditor *editor, s32 direction);
GF3DVramMan *BallCapsuleEditor_Create3DVramMan(void);
void BallCapsuleEditor_Init3D(void);
void BallCapsuleEditor_Alloc3DVram(void);
void BallCapsuleEditor_End3DFrame(void);
void BallCapsuleEditor_CreateMenu(BgConfig *bgConfig, Window *unusedWindow, int unused, BallCapsuleEditor *editor, int capsule);
void BallCapsuleEditor_DestroyMenu(BallCapsuleEditor *editor);
void BallCapsuleEditor_GetCapsulePosition(s32 capsule, s16 *x, s16 *y);
void BallCapsuleEditor_LoadMonIconResources(BallCapsuleEditor *editor);
void BallCapsuleEditor_CreateMonIcons(BallCapsuleEditor *editor);
void BallCapsuleEditor_PlaceMonIcons(BallCapsuleEditor *editor);
void BallCapsuleEditor_TickMonIcons(BallCapsuleEditor *editor);
void BallCapsuleEditor_DeleteMonIcons(BallCapsuleEditor *editor);
void BallCapsuleEditor_UpdateCapsuleIconStates(BallCapsuleEditor *editor);
void BallCapsuleEditor_InitSpriteSystem(BallCapsuleEditorDisplay *display);
void BallCapsuleEditor_FreeSpriteSystem(BallCapsuleEditorDisplay *display);
void BallCapsuleEditor_LoadArrowResources(BallCapsuleEditor *editor, NARC *narc);
void BallCapsuleEditor_CreateArrows(BallCapsuleEditor *editor);
void BallCapsuleEditor_TickArrows(BallCapsuleEditor *editor);
void BallCapsuleEditor_SetArrowsDrawFlag(BallCapsuleEditor *editor, BOOL draw);
void BallCapsuleEditor_DeleteArrows(BallCapsuleEditor *editor);
static void BallCapsuleEditor_LoadSpriteResources(SpriteSystem *spriteSystem, SpriteManager *spriteManager, PaletteData *paletteData, int charFile, int paletteFile, int cellFile, int animFile, int vram, int paletteBuffer, int paletteNum, NARC *narc);
void BallCapsuleEditor_CreateCapsuleIcons(BallCapsuleEditor *editor, NARC *narc);
void BallCapsuleEditor_RefreshCapsuleIcons(BallCapsuleEditor *editor);
void BallCapsuleEditor_SwitchCapsules(BallCapsuleEditor *editor, int a, int b);
void BallCapsuleEditor_TickCapsuleIcons(BallCapsuleEditor *editor);
void BallCapsuleEditor_DeleteCapsuleIcons(BallCapsuleEditor *editor);
void BallCapsuleEditor_LoadMainBgGraphics(BgConfig *bgConfig, PaletteData *paletteData, int frame);
void BallCapsuleEditor_LoadSubBgGraphics(BgConfig *bgConfig, PaletteData *paletteData, int frame);
void BallCapsuleEditor_PrintMessage(Window *window, int frameType, u32 msgId, BOOL drawFrame);
void BallCapsuleEditor_AddFramedWindow(BgConfig *bgConfig, Window *window, int bgId, int left, int top, int width, int height, int baseTile);
void BallCapsuleEditor_AddWindow(BgConfig *bgConfig, Window *window, int bgId, int left, int top, int width, int height, int baseTile);
void BallCapsuleEditor_AddWindowWithPalette(BgConfig *bgConfig, Window *window, int bgId, int left, int top, int width, int height, int baseTile, int palette);
void BallCapsuleEditor_AddSealCountWindows(BallCapsuleEditor *editor);
void BallCapsuleEditor_PrintSealCounts(BallCapsuleEditor *editor);
void BallCapsuleEditor_PrintSealCount(BallCapsuleEditor *editor, int index);
void BallCapsuleEditor_RemoveSealCountWindows(BallCapsuleEditor *editor);
void BallCapsuleEditor_PrintSealName(Window *window, u32 msgId);
void BallCapsuleEditor_ShowScreens(void);
void BallCapsuleEditor_FadeIn(void);
void BallCapsuleEditor_FadeOut(void);
void BallCapsuleEditor_LoadCaseBgGraphics(BallCapsuleEditor *editor, NARC *narc);
void BallCapsuleEditor_LoadSubBgCapsuleGraphics(BallCapsuleEditor *editor, NARC *narc);
void BallCapsuleEditor_LoadSubBgFrameGraphics(BallCapsuleEditor *editor, NARC *narc);
void BallCapsuleEditor_LoadCaseButtonResources(BallCapsuleEditor *editor, NARC *narc);
void BallCapsuleEditor_CreateCaseButtons(BallCapsuleEditor *editor);
void BallCapsuleEditor_SetCaseSpritesDrawFlag(BallCapsuleEditor *editor, BOOL draw);
void BallCapsuleEditor_Noop(BallCapsuleEditor *editor);
void BallCapsuleEditor_DeleteCaseButtons(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_CapsuleHasSeals(CAPSULE *capsule);
int BallCapsuleEditor_GetMenuKind(BallCapsuleEditor *editor, int capsule);
BOOL BallCapsuleEditor_RunMode(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_ModeSelect(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_ModeEdit(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_ModeRemove(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_ModeSwitch(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_ModeNone(BallCapsuleEditor *editor);
int BallCapsuleEditor_EnterEdit(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_LeaveEdit(BallCapsuleEditor *editor);
int BallCapsuleEditor_ConfirmQuitEdit(BallCapsuleEditor *editor);
int BallCapsuleEditor_PreviewCapsule(BallCapsuleEditor *editor);
void BallCapsuleEditor_HitboxCallback(u32 index, u32 event, void *arg);
int BallCapsuleEditor_UpdateMenuInputState(BallCapsuleEditor *editor);
int BallCapsuleEditor_HandleSelectInput(BallCapsuleEditor *editor);
int BallCapsuleEditor_HandleSelection(BallCapsuleEditor *editor, int selection);
int BallCapsuleEditor_HandleSelectKeys(BallCapsuleEditor *editor);
int BallCapsuleEditor_HandleSelectTouch(BallCapsuleEditor *editor);
int BallCapsuleEditor_HandleSwitchInput(BallCapsuleEditor *editor);
int BallCapsuleEditor_FinishSwitch(BallCapsuleEditor *editor, BOOL confirm);
int BallCapsuleEditor_HandleSwitchSelection(BallCapsuleEditor *editor, int selection);
int BallCapsuleEditor_HandleSwitchKeys(BallCapsuleEditor *editor);
int BallCapsuleEditor_HandleSwitchTouch(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_MoveCapsuleCursor(int *capsule);
void BallCapsuleEditor_UpdateCursors(BallCapsuleEditor *editor, int which, int copy);
int BallCapsuleEditor_GetTouchedCapsule(BallCapsuleEditor *editor);
int BallCapsuleEditor_HandleEditKeys(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_MenuEdit(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_MenuQuit(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_MenuSet(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_MenuRemove(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_MenuSwitch(BallCapsuleEditor *editor);
void BallCapsuleEditor_SaveEditedCapsule(BallCapsuleEditor *editor);
void BallCapsuleEditor_RestoreEditedCapsule(BallCapsuleEditor *editor);
void BallCapsuleEditor_SaveSealCounts(BallCapsuleEditor *editor);
void BallCapsuleEditor_RestoreSealCounts(BallCapsuleEditor *editor);
void BallCapsuleEditor_CopyPreviewMon(BallCapsuleEditor *editor);
void BallCapsuleEditor_SetMode(BallCapsuleEditor *editor, u32 mode, u32 state, u32 returnState);
void BallCapsuleEditor_SetButtonFrame(ManagedSprite *button, u32 event);
void BallCapsuleEditor_MoveButtonLabel(TextOBJ *label, s32 dx, s32 dy);
void BallCapsuleEditor_Task_ButtonPress(SysTask *task, void *data);
SysTask *BallCapsuleEditor_StartButtonPress(ManagedSprite *button, TextOBJ *label);
u32 ov57_0223B940(BallCapsuleEditor *editor);
void BallCapsuleEditor_SetInputEnabled(BallCapsuleEditor *editor, BOOL enabled);
void BallCapsuleEditor_CreatePreviewPokepic(BallCapsuleEditor *editor);
void BallCapsuleEditor_StartPreviewPokepicAnim(BallCapsuleEditor *editor);
void BallCapsuleEditor_ClearPreviewPokepicScale(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_GrowPreviewPokepic(BallCapsuleEditor *editor);
BOOL BallCapsuleEditor_ShrinkPreviewPokepic(BallCapsuleEditor *editor);
void BallCapsuleEditor_SetPreviewPokepicHidden(BallCapsuleEditor *editor, BOOL hidden);
void BallCapsuleEditor_DeletePreviewPokepic(BallCapsuleEditor *editor);
void BallCapsuleEditor_ResetBlend(void);
void BallCapsuleEditor_InitHitboxes(BallCapsuleEditor *editor);
void BallCapsuleEditor_ClearCapsuleMon(BallCapsuleEditor *editor, int capsule);

void BallCapsuleEditor_LoadBattleOverlays(void) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_6), OVY_LOAD_ASYNC);
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_7), OVY_LOAD_ASYNC);
}

// Overlay static initializer: the one .sinit entry of this overlay points at the function above.
#pragma define_section SINIT ".sinit" abs32 RWX
#pragma section SINIT begin
SDK_FORCE_EXPORT static void (*NitroStaticInit_[])(void) = { BallCapsuleEditor_LoadBattleOverlays };
#pragma section SINIT end

BOOL BallCapsuleEditor_Init(OverlayManager *manager, int *state) {
    BallCapsuleEditor *editor;
    int i;
    int count;
    int rem;

    Heap_Create(HEAP_ID_3, HEAP_ID_52, 0x80000);
    editor = OverlayManager_CreateAndGetData(manager, sizeof(BallCapsuleEditor), HEAP_ID_52);
    memset(editor, 0, sizeof(BallCapsuleEditor));
    editor->args = OverlayManager_GetArgs(manager);
    editor->pokepicAnimNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_52);
    editor->menuInputState = MenuInputStateMgr_GetState(editor->args->menuInputStatePtr);
    editor->previewMon = AllocMonZeroed(HEAP_ID_52);
    editor->display.heldSeal = BALL_CAPSULE_EDITOR_NONE;
    rem = 0;
    editor->page = rem;

    count = SealCase_CountUniqueSeals(editor->args->sealCase);
    if (count % BALL_CAPSULE_EDITOR_SEALS_PER_PAGE != 0) {
        rem = 1;
    }
    count = SealCase_CountUniqueSeals(editor->args->sealCase);
    editor->pageCount = rem + count / BALL_CAPSULE_EDITOR_SEALS_PER_PAGE;
    if (editor->pageCount > BALL_CAPSULE_EDITOR_MAX_PAGES) {
        editor->pageCount = BALL_CAPSULE_EDITOR_MAX_PAGES;
    }

    editor->cursorCapsule[0] = BallCapsuleEditor_GetArgsCapsuleIndex(editor->args);
    editor->cursorCapsule[1] = BallCapsuleEditor_GetArgsCapsuleIndex(editor->args);
    editor->sealBag = SealCase_inventory_Get(editor->args->sealCase);

    for (i = 0; i < MAX_CAPSULES; i++) {
        CAPSULE *capsule = SealCase_GetCapsuleI(editor->args->sealCase, i);
        editor->capsules[i].partySlot = BALL_CAPSULE_EDITOR_NO_MON;
        editor->capsules[i].capsule = capsule;
    }

    for (i = 0; i < PARTY_SIZE; i++) {
        Pokemon *mon = editor->args->mons[i];
        if (mon != NULL) {
            u32 capsuleId = GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL);
            if (capsuleId != 0) {
                editor->capsules[capsuleId - 1].partySlot = i;
            }
        }
    }

    BallCapsuleEditor_ResetDisplay(HEAP_ID_52);
    BallCapsuleEditor_ResetBlend();
    editor->vramMan = BallCapsuleEditor_Create3DVramMan();
    editor->display.bgConfig = BgConfig_Alloc(HEAP_ID_52);
    GF_CreateVramTransferManager(0x40, HEAP_ID_52);
    editor->display.paletteData = PaletteData_Init(HEAP_ID_52);
    PaletteData_SetAutoTransparent(editor->display.paletteData, TRUE);
    PaletteData_AllocBuffers(editor->display.paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_52);
    PaletteData_AllocBuffers(editor->display.paletteData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_52);
    PaletteData_AllocBuffers(editor->display.paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_52);
    PaletteData_AllocBuffers(editor->display.paletteData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_52);
    BallCapsuleEditor_InitBgs(editor->display.bgConfig);
    BallCapsuleEditor_Alloc3DVram();
    editor->display.pokepicManager = PokepicManager_Create(HEAP_ID_52);
    editor->pokepicAnim = PokepicAnimManager_New(HEAP_ID_52, 1, 0);
    i = Options_GetFrame(editor->args->options);
    BallCapsuleEditor_LoadMainBgGraphics(editor->display.bgConfig, editor->display.paletteData, i);
    BallCapsuleEditor_LoadSubBgGraphics(editor->display.bgConfig, editor->display.paletteData, i);
    BallCapsuleEditor_InitSpriteSystem(&editor->display);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    BallCapsuleEditor_InitHitboxes(editor);
    Main_SetVBlankIntrCB(BallCapsuleEditor_VBlankCB, editor);
    BallCapsuleEditor_InitFonts(editor);
    return TRUE;
}

BOOL BallCapsuleEditor_Main(OverlayManager *manager, int *state) {
    BallCapsuleEditor *editor = OverlayManager_GetData(manager);

    if (BallCapsuleEditor_RunMode(editor) == FALSE) {
        return TRUE;
    }
    PokepicManager_DrawAll(editor->display.pokepicManager);
    BallCapsuleEditor_End3DFrame();
    return FALSE;
}

BOOL BallCapsuleEditor_Exit(OverlayManager *manager, int *state) {
    BallCapsuleEditor *editor;

    editor = OverlayManager_GetData(manager);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(editor->display.bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(editor->display.bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(editor->display.bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(editor->display.bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(editor->display.bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(editor->display.bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(editor->display.bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(editor->display.bgConfig);
    PaletteData_FreeBuffers(editor->display.paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(editor->display.paletteData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(editor->display.paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(editor->display.paletteData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(editor->display.paletteData);
    BallCapsuleEditor_SetArgsCapsuleIndex(editor->args, editor->cursorCapsule[0]);
    Heap_Free(editor->previewMon);
    BallCapsuleEditor_RemoveAllSeals(editor);
    TouchHitboxController_Destroy(editor->display.hitboxController);
    PokepicManager_Delete(editor->display.pokepicManager);
    PokepicAnimManager_Delete(editor->pokepicAnim);
    BallCapsuleEditor_FreeFonts(editor);
    BallCapsuleEditor_FreeSpriteSystem(&editor->display);
    GF_DestroyVramTransferManager();
    GF_3DVramMan_Delete(editor->vramMan);
    BallCapsuleEditor_DisableInterrupts(HEAP_ID_52);
    NARC_Delete(editor->pokepicAnimNarc);
    MenuInputStateMgr_SetState(editor->args->menuInputStatePtr, editor->menuInputState);
    OverlayManager_FreeData(manager);
    GF_TouchpadStopSampling();
    Heap_Destroy(HEAP_ID_52);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_6));
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_7));
    return TRUE;
}

void BallCapsuleEditor_ResetDisplay(enum HeapID heapID) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

void BallCapsuleEditor_DisableInterrupts(enum HeapID heapID) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
}

#pragma force_active on

void BallCapsuleEditor_InitBgs(BgConfig *bgConfig) {
    BallCapsuleEditorMainBgTemplates mainTemplates;
    BallCapsuleEditorSubBgTemplates subTemplates;
    GraphicsBanks banks;
    GraphicsModes modes;

    GfGfx_DisableEngineAPlanes();
    banks = sBallCapsuleEditorGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (void *)HW_BG_VRAM, HW_BG_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
    MIi_CpuClear32(0, (void *)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
    modes = sBallCapsuleEditorGraphicsModes;
    SetBothScreensModesAndDisable(&modes);

    mainTemplates = sBallCapsuleEditorMainBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainTemplates.templates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainTemplates.templates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainTemplates.templates[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    G2_SetBG0Priority(1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);

    subTemplates = sBallCapsuleEditorSubBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subTemplates.templates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subTemplates.templates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subTemplates.templates[2], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &subTemplates.templates[3], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void BallCapsuleEditor_VBlankCB(void *data) {
    BallCapsuleEditor *editor = data;
    vu32 *dtcm = (vu32 *)0x027E0000; // for OS_SetIrqCheckFlag below, with the DTCM address as a constant

    PokepicManager_HandleLoadImgAndOrPltt(editor->display.pokepicManager);
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(editor->display.paletteData);
    DoScheduledBgGpuUpdates(editor->display.bgConfig);
    GF_RunVramTransferTasks();
    dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
}

#pragma force_active reset

u8 BallCapsuleEditor_GetArgsCapsuleIndex(BallCapsuleEditorArgs *args) {
    return args->capsuleIndex;
}

void BallCapsuleEditor_SetArgsCapsuleIndex(BallCapsuleEditorArgs *args, u8 capsuleIndex) {
    args->capsuleIndex = capsuleIndex;
}

void BallCapsuleEditor_SetArgsSetCapsuleOnMon(BallCapsuleEditorArgs *args, u8 setCapsuleOnMon) {
    args->setCapsuleOnMon = setCapsuleOnMon;
}

void BallCapsuleEditor_SetSealHitbox(TouchscreenHitbox *hitbox, int x, int y) {
    hitbox->rect.top = y - 10;
    hitbox->rect.left = x - 10;
    hitbox->rect.bottom = y + 10;
    hitbox->rect.right = x + 10;
}

// While a seal is dragged, its hitbox covers the whole touch screen.
void BallCapsuleEditor_SetDragHitbox(TouchscreenHitbox *hitbox) {
    hitbox->rect.top = 0;
    hitbox->rect.left = 0;
    hitbox->rect.bottom = 192;
    hitbox->rect.right = 255;
}

void BallCapsuleEditor_UpdateSealHitbox(TouchscreenHitbox *hitbox, ManagedSprite *sprite, BOOL dragging) {
    s16 x;
    s16 y;

    if (sprite == NULL) {
        return;
    }
    if (dragging == TRUE) {
        ManagedSprite_SetPositionXY(sprite, gSystem.touchX, gSystem.touchY);
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        BallCapsuleEditor_SetDragHitbox(hitbox);
    } else {
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        BallCapsuleEditor_SetSealHitbox(hitbox, (u8)x, (u8)y);
    }
}

// The capsule is drawn 56 px left and 16 px down of the coordinates stored in the seal case.
void BallCapsuleEditor_ShiftSealsToCapsule(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        if (editor->seals[i].sprite != NULL) {
            ManagedSprite_OffsetPositionXY(editor->seals[i].sprite, -56, 16);
        }
    }
}

void BallCapsuleEditor_LoadSeals(BallCapsuleEditor *editor) {
    const SEAL *seal = editor->capsules[editor->cursorCapsule[0]].capsule->seals;
    int i = 0;
    BallCapsuleEditor *shifted = editor; // shifted by i seals: seals[0] through it is seals[i]

    do {
        if (seal->kind != SEAL_NONE) {
            shifted->seals[0].seal.kind = seal->kind;
            shifted->seals[0].seal.x = seal->x;
            shifted->seals[0].seal.y = seal->y;
            BallCapsuleEditor_CreateSealSprite(editor, (u8)i);
            BallCapsuleEditor_UpdateSealHitbox(shifted->seals[0].hitbox, shifted->seals[0].sprite, FALSE);
            shifted->seals[0].active = TRUE;
        } else {
            shifted->seals[0].active = FALSE;
            shifted->seals[0].seal.kind = SEAL_NONE;
        }
        i++;
        seal++;
        shifted = (BallCapsuleEditor *)((u8 *)shifted + sizeof(BallCapsuleEditorSeal));
    } while (i < MAX_SEALS_ON_CAPSULE);
}

#pragma force_active on

u32 BallCapsuleEditor_AddSeal(BallCapsuleEditor *editor, u8 caseIndex) {
    int i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        if (editor->seals[i].active != TRUE) {
            u32 x, y;
            editor->seals[i].seal.kind = editor->pageSeals[caseIndex];
            editor->seals[i].seal.x = 190;
            editor->seals[i].seal.y = 70;
            System_GetTouchHeldCoords(&x, &y);
            editor->seals[i].seal.x = x;
            editor->seals[i].seal.y = y;
            BallCapsuleEditor_CreateSealSprite(editor, i);
            BallCapsuleEditor_BringSealToFront(editor, (u8)i);
            editor->seals[i].active = TRUE;
            return i;
        }
    }
    return BALL_CAPSULE_EDITOR_NONE;
}

#pragma force_active reset

BOOL BallCapsuleEditor_HasFreeSealSlot(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        if (editor->seals[i].active != TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

void BallCapsuleEditor_BringSealToFront(BallCapsuleEditor *editor, s32 index) {
    s32 i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        if (editor->seals[i].active == TRUE) {
            if (i == index) {
                ManagedSprite_SetDrawPriority(editor->seals[i].sprite, 0);
            } else {
                u32 priority = ManagedSprite_GetDrawPriority(editor->seals[i].sprite);
                if (priority == 0) {
                    ((BallCapsuleEditorSetDrawPriorityFunc)ManagedSprite_SetDrawPriority)(editor->seals[i].sprite, priority + 1);
                } else {
                    ((BallCapsuleEditorSetDrawPriorityFunc)ManagedSprite_SetDrawPriority)(editor->seals[i].sprite, priority + 2);
                }
            }
        }
    }
}

void BallCapsuleEditor_LoadSealSpriteResources(BallCapsuleEditor *editor, u32 kind, s32 index) {
    SpriteSystem *spriteSystem = editor->display.spriteSystem;
    SpriteManager *spriteManager = editor->display.spriteManager;

    SpriteSystem_LoadPaletteBuffer(editor->display.paletteData, PLTTBUF_SUB_OBJ, spriteSystem, spriteManager, NARC_a_0_8_7, 6, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 21006);
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_0_8_7, sub_02091054(kind), TRUE, NNS_G2D_VRAM_TYPE_2DSUB, index + 20000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_0_8_7, 38, TRUE, 22038);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_0_8_7, 36, TRUE, 23036);
}

// Gives the seals draw priorities 0-7 in their current order and stores them in that order.
void BallCapsuleEditor_SortSeals(BallCapsuleEditor *editor) {
    s32 priority[MAX_SEALS_ON_CAPSULE];
    s32 order[MAX_SEALS_ON_CAPSULE];
    BallCapsuleEditorSealData seals;
    BallCapsuleEditorSealData copy;
    s32 i;
    s32 j;

    if (editor->sortSeals == FALSE) {
        return;
    }
    editor->sortSeals = FALSE;
    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        priority[i] = 0xFF;
        order[i] = 0xFF;
        if (editor->seals[i].sprite != NULL) {
            priority[i] = ManagedSprite_GetDrawPriority(editor->seals[i].sprite);
            order[i] = i;
        }
    }
    for (i = 0; i < MAX_SEALS_ON_CAPSULE - 1; i++) {
        for (j = MAX_SEALS_ON_CAPSULE - 1; j > i; j--) {
            if (priority[j - 1] >= priority[j]) {
                s32 value = priority[j];
                s32 index = order[j];
                priority[j] = priority[j - 1];
                order[j] = order[j - 1];
                priority[j - 1] = value;
                order[j - 1] = index;
            }
        }
    }
    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        seals.bytes[i * 3] = editor->seals[i].seal.kind;
        seals.bytes[i * 3 + 1] = editor->seals[i].seal.x;
        seals.bytes[i * 3 + 2] = editor->seals[i].seal.y;
    }
    copy = seals;
    {
        s32 *sorted;
        BallCapsuleEditor *dst = editor; // shifted by n seals: seals[0] through it is seals[n]
        s32 n = 0;
        sorted = order;
        u8 *src = copy.bytes;
        for (; n < MAX_SEALS_ON_CAPSULE; n++, sorted++, dst = (BallCapsuleEditor *)((u8 *)dst + sizeof(BallCapsuleEditorSeal))) {
            if (*sorted == 0xFF) {
                dst->seals[0].seal.kind = SEAL_NONE;
                dst->seals[0].seal.x = 0;
                dst->seals[0].seal.y = 0;
            } else {
                dst->seals[0].seal.kind = src[*sorted * 3];
                dst->seals[0].seal.x = src[*sorted * 3 + 1];
                dst->seals[0].seal.y = src[*sorted * 3 + 2];
                if (editor->seals[*sorted].sprite != NULL) {
                    ((BallCapsuleEditorSetDrawPriorityFunc)ManagedSprite_SetDrawPriority)(editor->seals[*sorted].sprite, n);
                }
            }
        }
    }
}

#pragma force_active on

BOOL BallCapsuleEditor_CreateSealSprite(BallCapsuleEditor *editor, u8 index) {
    ManagedSpriteTemplate template;
    SpriteSystem *spriteSystem = editor->display.spriteSystem;
    SpriteManager *spriteManager = editor->display.spriteManager;
    BallCapsuleEditorSeal *seal = &editor->seals[index];

    if (editor->seals[index].active == TRUE) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    BallCapsuleEditor_LoadSealSpriteResources(editor, seal->seal.kind, index);
    template.x = seal->seal.x;
    template.y = seal->seal.y;
    template.z = 0;
    template.animation = 0;
    template.pal = 0;
    template.vramTransfer = FALSE;
    template.drawPriority = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.bgPriority = 1;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    sub_02091054(seal->seal.kind);
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 20000 + index;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 21006;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 22038;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 23036;
    seal->sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    ManagedSprite_TickFrame(seal->sprite);
    return TRUE;
}

#pragma force_active reset

BOOL BallCapsuleEditor_HandleSealTouch(BallCapsuleEditor *editor, u32 event, s32 index) {
    BOOL onCapsule;

    if (editor->seals[index].active == FALSE) {
        return TRUE;
    }
    switch (event) {
    case 0:
        if (editor->display.heldSeal == BALL_CAPSULE_EDITOR_NONE) {
            editor->display.heldSeal = index;
            BallCapsuleEditor_BringSealToFront(editor, index);
        }
        break;
    case 2:
        if (editor->display.heldSeal == (u32)index) {
            BallCapsuleEditor_UpdateSealHitbox(editor->seals[index].hitbox, editor->seals[index].sprite, TRUE);
            BallCapsuleEditor_UpdateSealOamMode(editor, index);
            BallCapsuleEditor_StoreSealPosition(editor, index);
        }
        break;
    case 1:
    case 3:
        if (editor->display.heldSeal == BALL_CAPSULE_EDITOR_NONE) {
            onCapsule = BallCapsuleEditor_IsSealOnCapsule(editor, index);
            BallCapsuleEditor_UpdateSealHitbox(editor->seals[index].hitbox, editor->seals[index].sprite, FALSE);
            if (!onCapsule) {
                BallCapsuleEditor_RemoveSeal(editor, index);
            }
            editor->display.heldSeal = BALL_CAPSULE_EDITOR_NONE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return TRUE;
}

void BallCapsuleEditor_RemoveAllSeals(BallCapsuleEditor *editor) {
    s32 i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        if (editor->seals[i].active == TRUE) {
            BallCapsuleEditor_RemoveSeal(editor, i);
        }
    }
}

void BallCapsuleEditor_SetSealsDrawFlag(BallCapsuleEditor *editor, BOOL draw) {
    s32 i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        if (editor->seals[i].active == TRUE) {
            ManagedSprite_SetDrawFlag(editor->seals[i].sprite, draw);
        }
    }
}

// TRUE when the seal is within 60 px of the capsule's centre (190, 70).
BOOL BallCapsuleEditor_IsSealOnCapsule(BallCapsuleEditor *editor, s32 index) {
    s16 x, y;

    if (editor->seals[index].active == FALSE) {
        return TRUE;
    }
    ManagedSprite_GetPositionXY(editor->seals[index].sprite, &x, &y);
    if (BallSealEffect_GetDistance(x, y, 190, 70) <= 60) {
        return TRUE;
    }
    return FALSE;
}

void BallCapsuleEditor_RemoveSeal(BallCapsuleEditor *editor, s32 index) {
    sub_02091054(editor->seals[index].seal.kind);
    SpriteManager_UnloadCharObjById(editor->display.spriteManager, index + 20000);
    Sprite_DeleteAndFreeResources(editor->seals[index].sprite);
    editor->seals[index].sprite = NULL;
    editor->seals[index].active = FALSE;
    editor->seals[index].seal.kind = SEAL_NONE;
    editor->seals[index].seal.x = 0;
    editor->seals[index].seal.y = 0;
    BallCapsuleEditor_SetSealHitbox(editor->seals[index].hitbox, 0, 0);
}

// Seals dragged off the capsule are drawn translucent.
void BallCapsuleEditor_UpdateSealOamMode(BallCapsuleEditor *editor, s32 index) {
    if (!BallCapsuleEditor_IsSealOnCapsule(editor, index)) {
        ManagedSprite_SetOamMode(editor->seals[index].sprite, GX_OAM_MODE_XLU);
    } else {
        ManagedSprite_SetOamMode(editor->seals[index].sprite, GX_OAM_MODE_NORMAL);
    }
}

BOOL BallCapsuleEditor_IsCapsuleChanged(BallCapsuleEditor *editor) {
    s32 i = 0;
    const SEAL *saved = editor->capsules[editor->cursorCapsule[0]].capsule->seals;

    // editor is shifted by i seals: seals[0] through it is seals[i]
    for (; i < MAX_SEALS_ON_CAPSULE; i++, saved++, editor = (BallCapsuleEditor *)((u8 *)editor + sizeof(BallCapsuleEditorSeal))) {
        if (editor->seals[0].seal.kind != saved->kind || editor->seals[0].seal.x != saved->x || editor->seals[0].seal.y != saved->y) {
            return TRUE;
        }
    }
    return FALSE;
}

void BallCapsuleEditor_StoreSealPosition(BallCapsuleEditor *editor, s32 index) {
    s16 x, y;

    ManagedSprite_GetPositionXY(editor->seals[index].sprite, &x, &y);
    editor->seals[index].seal.x = (u8)x;
    editor->seals[index].seal.y = (u8)y;
}

void BallCapsuleEditor_SaveCapsule(BallCapsuleEditor *editor) {
    BallCapsuleEditorSealData data;
    s32 i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        data.bytes[i * 3] = editor->seals[i].seal.kind;
        data.bytes[i * 3 + 1] = editor->seals[i].seal.x;
        data.bytes[i * 3 + 2] = editor->seals[i].seal.y;
    }
    SealCase_SetCapsuleI(editor->args->sealCase, (const CAPSULE *)&data, editor->cursorCapsule[0]);
}

void BallCapsuleEditor_GetSealData(SEAL *seals, BallCapsuleEditor *editor) {
    s32 i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++, seals++) {
        seals->kind = editor->seals[i].seal.kind;
        seals->x = editor->seals[i].seal.x;
        seals->y = editor->seals[i].seal.y;
    }
}

void BallCapsuleEditor_SetSpriteHitbox(TouchscreenHitbox *hitbox, ManagedSprite *sprite, s32 halfWidth, s32 halfHeight) {
    s16 x, y;

    ManagedSprite_GetPositionXY(sprite, &x, &y);
    hitbox->rect.top = (u8)(y - halfHeight);
    hitbox->rect.bottom = (u8)(y + halfHeight);
    hitbox->rect.left = (u8)(x - halfWidth);
    hitbox->rect.right = (u8)(x + halfWidth);
}

void BallCapsuleEditor_AddMessageWindows(BallCapsuleEditor *editor) {
    BallCapsuleEditor_AddWindow(editor->display.bgConfig, &editor->display.windows[0], GF_BG_LYR_MAIN_1, 1, 21, 30, 2, 58);
    BallCapsuleEditor_AddWindow(editor->display.bgConfig, &editor->display.windows[3], GF_BG_LYR_MAIN_1, 1, 21, 20, 2, 58);
    BallCapsuleEditor_AddWindow(editor->display.bgConfig, &editor->display.windows[2], GF_BG_LYR_MAIN_1, 23, 21, 8, 2, 118);
}

void BallCapsuleEditor_RemoveMessageWindows(BallCapsuleEditor *editor) {
    RemoveWindow(&editor->display.windows[0]);
    RemoveWindow(&editor->display.windows[3]);
    RemoveWindow(&editor->display.windows[2]);
}

#pragma force_active on

void BallCapsuleEditor_PrintExitLabel(BallCapsuleEditor *editor, BOOL show) {
    if (show) {
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0011_bin, HEAP_ID_52);
        String *string = NewString_ReadMsgData(msgData, msg_0011_00017);
        FillWindowPixelBuffer(&editor->display.windows[2], 0);
        {
            u32 x = 64 - FontID_String_GetWidth(4, string, 0);
            AddTextPrinterParameterizedWithColor(&editor->display.windows[2], 4, string, x >> 1, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 15, 0), NULL);
        }
        String_Delete(string);
        DestroyMsgData(msgData);
        ScheduleWindowCopyToVram(&editor->display.windows[2]);
    } else {
        ClearWindowTilemapAndScheduleTransfer(&editor->display.windows[2]);
    }
}

#pragma force_active reset

void BallCapsuleEditor_InitFonts(BallCapsuleEditor *editor) {
    editor->fontSystem = FontSystem_NewInit(2, HEAP_ID_52);
    FontID_Alloc(2, HEAP_ID_52);
    FontID_Alloc(4, HEAP_ID_52);
}

void BallCapsuleEditor_FreeFonts(BallCapsuleEditor *editor) {
    FontID_Release(4);
    FontID_Release(2);
    FontOAM_Delete(editor->buttonLabels[0]);
    ObjCharTransfer_FreeCharSpace(&editor->buttonLabelChars[0]);
    FontOAM_Delete(editor->buttonLabels[1]);
    ObjCharTransfer_FreeCharSpace(&editor->buttonLabelChars[1]);
    FontSystem_Delete(editor->fontSystem);
}

void BallCapsuleEditor_LoadButtonLabelPalette(BallCapsuleEditor *editor) {
    SpriteSystem_LoadPaletteBuffer(editor->display.paletteData, PLTTBUF_SUB_OBJ, editor->display.spriteSystem, editor->display.spriteManager, NARC_graphic_font, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 30000);
}

void BallCapsuleEditor_CreateButtonLabels(BallCapsuleEditor *editor) {
    BallCapsuleEditor_LoadButtonLabelPalette(editor);
    BallCapsuleEditor_CreateButtonLabel(editor, 0, 104, 165, 0);
    BallCapsuleEditor_CreateButtonLabel(editor, 1, 192, 165, 0);
}

void BallCapsuleEditor_SetButtonLabelsDrawFlag(BallCapsuleEditor *editor, BOOL draw) {
    TextOBJ_SetSpritesDrawFlag(editor->buttonLabels[0], draw);
    TextOBJ_SetSpritesDrawFlag(editor->buttonLabels[1], draw);
}

#pragma force_active on

void BallCapsuleEditor_CreateButtonLabel(BallCapsuleEditor *editor, int index, int x, int y, int palette) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0011_bin, HEAP_ID_52);
    String *volatile string = NewString_ReadMsgData(msgData, index + msg_0011_00005); // (volatile: matches the original stack use)
    TextOBJTemplate template;
    Window window;
    int chars;

    InitWindow(&window);
    AddTextWindowTopLeftCorner(editor->display.bgConfig, &window, 10, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(&window, 2, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 2), NULL);
    chars = FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_52);
    ObjCharTransfer_ReserveCharSpace(chars, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, &editor->buttonLabelChars[index]);
    template.fontSystem = editor->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(editor->display.spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(editor->display.spriteManager, 30000);
    template.sprite = NULL;
    template.offset = editor->buttonLabelChars[index].offset;
    template.x = x - FontID_String_GetWidth(2, string, 0) / 2;
    template.y = y + 192;
    template.unk_20 = 1;
    template.unk_24 = 40;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_52;
    editor->buttonLabels[index] = FontOAM_Create(&template);
    TextOBJ_SetPalOffsetRespectVramOffset(editor->buttonLabels[index], palette);
    String_Delete(string);
    DestroyMsgData(msgData);
    RemoveWindow(&window);
}

// Seals shown on a page: the owned kinds and those on any capsule, 8 per page.
void BallCapsuleEditor_FillPageSeals(BallCapsuleEditor *editor, int page) {
    int skip;
    int count = 0;
    int shown = 0;
    int i;
    int j;

    for (i = 0; i < BALL_CAPSULE_EDITOR_SEALS_PER_PAGE; i++) {
        editor->pageSeals[i] = SEAL_NONE;
    }
    skip = page * BALL_CAPSULE_EDITOR_SEALS_PER_PAGE;
    for (i = 0; i < SEAL_MAX + 1; i++) {
        int onCapsule;
        for (j = 0; j < MAX_CAPSULES; j++) {
            onCapsule = SealIsOnCapsule(editor->capsules[j].capsule, i);
            if (onCapsule) {
                break;
            }
        }
        if (editor->savedSealCounts[i] || onCapsule == TRUE) {
            shown++;
            if (shown > skip) {
                editor->pageSeals[count] = i + 1;
                count++;
                if (count >= BALL_CAPSULE_EDITOR_SEALS_PER_PAGE) {
                    break;
                }
            }
        }
    }
}

#pragma force_active reset

void BallCapsuleEditor_LoadCaseSealSpriteResources(BallCapsuleEditor *editor) {
    SpriteSystem *spriteSystem = editor->display.spriteSystem;
    int i;
    SpriteManager *spriteManager = editor->display.spriteManager;
    PaletteData *paletteData = editor->display.paletteData;

    for (i = 0; i < BALL_CAPSULE_EDITOR_SEALS_PER_PAGE; i++) {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_0_8_7, sub_02091054(editor->pageSeals[i]), TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 25000 + i);
    }
    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_SUB_OBJ, spriteSystem, spriteManager, NARC_a_0_8_7, 6, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 26007);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_0_8_7, 37, TRUE, 27037);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_0_8_7, 35, TRUE, 28035);
}

#pragma force_active on

void BallCapsuleEditor_CreateCaseSealSprites(BallCapsuleEditor *editor) {
    ManagedSpriteTemplate template;
    BallCapsuleEditorCaseSealPositions positions;
    SpriteSystem *spriteSystem = editor->display.spriteSystem;
    SpriteManager *spriteManager = editor->display.spriteManager;
    ManagedSprite **sprite;
    int i;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 60;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.bgPriority = 1;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 26007;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 27037;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 28035;
    template.vramTransfer = FALSE;
    template.pal = 0;
    sprite = editor->caseSprites;
    for (i = 0; i < BALL_CAPSULE_EDITOR_SEALS_PER_PAGE; i++) {
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = 25000 + i;
        *sprite++ = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    }
    positions = sBallCapsuleEditorCaseSealPositions;
    for (i = 0; i < BALL_CAPSULE_EDITOR_SEALS_PER_PAGE; i++) {
        ManagedSprite_SetPositionXY(editor->caseSprites[i], positions.positions[i].x, (s16)(positions.positions[i].y - 1));
        ManagedSprite_TickFrame(editor->caseSprites[i]);
        ManagedSprite_SetAnimationFrame(editor->caseSprites[i], 0);
    }
}

#pragma force_active reset

void BallCapsuleEditor_DeleteCaseSealSprites(BallCapsuleEditor *editor) {
    s32 i;

    for (i = 0; i < BALL_CAPSULE_EDITOR_SEALS_PER_PAGE; i++) {
        SpriteManager_UnloadCharObjById(editor->display.spriteManager, 25000 + i);
        Sprite_DeleteAndFreeResources(editor->caseSprites[i]);
    }
}

// direction 1: from the seal case coordinates to the bottom screen capsule, -1: back.
void BallCapsuleEditor_MoveSeals(BallCapsuleEditor *editor, s32 direction) {
    s16 dx = direction * 56;
    s16 dy = -(direction << 4);
    s32 i;

    for (i = 0; i < MAX_SEALS_ON_CAPSULE; i++) {
        if (editor->seals[i].active != FALSE) {
            ManagedSprite_OffsetPositionXY(editor->seals[i].sprite, dx, dy);
        }
    }
}

GF3DVramMan *BallCapsuleEditor_Create3DVramMan(void) {
    return GF_3DVramMan_Create(HEAP_ID_52, 0, 2, 0, 2, BallCapsuleEditor_Init3D);
}

#pragma force_active on

void BallCapsuleEditor_Init3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(RGB_BLACK, 0, GF_GX_CLEARCOLORDEPTH_MAX, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}

#pragma force_active reset

void BallCapsuleEditor_Alloc3DVram(void) {
    NNSGfdTexKey texKey = NNS_GfdAllocTexVram(0x8000, FALSE, 0);
    NNSGfdPlttKey plttKey = NNS_GfdAllocPlttVram(0x80, FALSE, 0);

    GF_ASSERT(texKey != 0);
    GF_ASSERT(plttKey != 0);
    ParticleSystem_ZeroAll();
}

void BallCapsuleEditor_End3DFrame(void) {
    Thunk_G3X_Reset();
    if (ParticleSystem_DrawAll() > 0) {
        Thunk_G3X_Reset();
        NNS_G2dSetupSoftwareSpriteCamera();
    }
    ParticleSystem_CalcAll();
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

static inline int BallCapsuleEditor_GetMenuItems(int items[4], int kind) {
    int size;

    switch (kind) {
    case 0:
        items[0] = 0;
        items[1] = 4;
        size = 6;
        items[2] = 1;
        break;
    case 1:
        items[0] = 2;
        items[1] = 0;
        items[2] = 4;
        size = 8;
        items[3] = 1;
        break;
    case 2:
        items[0] = 3;
        items[1] = 0;
        items[2] = 4;
        size = 8;
        items[3] = 1;
        break;
    case 3:
        items[0] = 2;
        items[1] = 0;
        items[2] = 4;
        size = 8;
        items[3] = 1;
        break;
    default:
        break;
    }
    return size;
}

#pragma force_active on

void BallCapsuleEditor_CreateMenu(BgConfig *bgConfig, Window *unusedWindow, int unused, BallCapsuleEditor *editor, int capsule) {
    TouchscreenListMenuHeader header;
    int items[4];
    int size;
    int count;
    int i;
    MsgData *msgData;
    String *string;

    size = BallCapsuleEditor_GetMenuItems(items, BallCapsuleEditor_GetMenuKind(editor, capsule));
    count = size / 2;
    editor->display.menuItems = ListMenuItems_New(count, HEAP_ID_52);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0011_bin, HEAP_ID_52);
    for (i = 0; i < count; i++) {
        string = NewString_ReadMsgData(msgData, sBallCapsuleEditorMenuItems[items[i]].msgId);
        ListMenuItems_AddItem(editor->display.menuItems, string, (int)sBallCapsuleEditorMenuItems[items[i]].func);
        String_Delete(string);
    }
    DestroyMsgData(msgData);
    MI_CpuFill8(&header, 0, sizeof(TouchscreenListMenuHeader));
    header.template = sBallCapsuleEditorMenuTemplate;
    header.listMenuItems = editor->display.menuItems;
    header.bgConfig = bgConfig;
    header.numWindows = (u8)count;
    editor->menu = TouchscreenListMenu_Create(editor->display.menuSpawner, &header, (u8)editor->menuInputState, 22, (u8)(23 - count * 3), 8, 0);
}

#pragma force_active reset

void BallCapsuleEditor_DestroyMenu(BallCapsuleEditor *editor) {
    editor->menuInputState = (MenuInputState)editor->menu->isTouch;
    TouchscreenListMenu_Destroy(editor->menu);
    ListMenuItems_Delete(editor->display.menuItems);
}

void BallCapsuleEditor_GetCapsulePosition(s32 capsule, s16 *x, s16 *y) {
    s32 column = capsule % 4;
    *x = column * 56 + 40;
    *y = (capsule >> 2) * 53 + 27;
}

#pragma force_active on

void BallCapsuleEditor_LoadMonIconResources(BallCapsuleEditor *editor) {
    SpriteSystem *spriteSystem = editor->display.spriteSystem;
    SpriteManager *spriteManager = editor->display.spriteManager;
    PaletteData *paletteData = editor->display.paletteData;

    SpriteSystem_LoadPaletteBuffer(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 16000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconCell64KNaix(), FALSE, 17000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconAnim64KNaix(), FALSE, 18000);
}

void BallCapsuleEditor_CreateMonIcons(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < editor->args->partyCount; i++) {
        Pokemon *mon = editor->args->mons[i];
        ManagedSpriteTemplate template;
        u32 species, isEgg, form;

        SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(editor->display.spriteSystem, editor->display.spriteManager, NARC_poketool_icongra_poke_icon, Pokemon_GetIconNaix(mon), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 15000 + i);
        template.x = 0;
        template.y = 0;
        template.z = 0;
        template.animation = 0;
        template.drawPriority = 10;
        template.pal = 0;
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.bgPriority = 2;
        template.vramTransfer = FALSE;
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = 15000 + i;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = 16000;
        template.resIdList[GF_GFX_RES_TYPE_CELL] = 17000;
        template.resIdList[GF_GFX_RES_TYPE_ANIM] = 18000;
        template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
        template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
        editor->monIcons[i] = SpriteSystem_NewSprite(editor->display.spriteSystem, editor->display.spriteManager, &template);
        species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        isEgg = GetMonData(mon, MON_DATA_IS_EGG, NULL);
        form = GetMonData(mon, MON_DATA_FORM, NULL);
        Sprite_SetPalOffsetRespectVramOffset(editor->monIcons[i]->sprite, GetMonIconPaletteEx(species, form, isEgg));
        ManagedSprite_SetAnim(editor->monIcons[i], 1);
    }
}

#pragma force_active reset

// The icon of a party Pokémon is shown on the capsule it has.
void BallCapsuleEditor_PlaceMonIcons(BallCapsuleEditor *editor) {
    s32 i;

    for (i = 0; i < editor->args->partyCount; i++) {
        ManagedSprite_SetDrawFlag(editor->monIcons[i], FALSE);
    }
    {
        BallCapsuleEditor *shifted; // shifted by j capsules: capsules[0] through it is capsules[j]
        s32 j;
        for (j = 0, shifted = editor; j < MAX_CAPSULES; j++, shifted = (BallCapsuleEditor *)((u8 *)shifted + sizeof(BallCapsuleEditorCapsule))) {
            u32 partySlot = shifted->capsules[0].partySlot;
            if (partySlot != BALL_CAPSULE_EDITOR_NO_MON) {
                s16 x, y;
                BallCapsuleEditor_GetCapsulePosition(j, &x, &y);
                ManagedSprite_SetPositionXY(editor->monIcons[partySlot], x - 16, y + 12);
                ManagedSprite_SetDrawFlag(editor->monIcons[partySlot], TRUE);
            }
        }
    }
}

void BallCapsuleEditor_TickMonIcons(BallCapsuleEditor *editor) {
    s32 i;

    for (i = 0; i < editor->args->partyCount; i++) {
        ManagedSprite_TickFrame(editor->monIcons[i]);
    }
}

void BallCapsuleEditor_DeleteMonIcons(BallCapsuleEditor *editor) {
    s32 i;

    for (i = 0; i < editor->args->partyCount; i++) {
        Sprite_DeleteAndFreeResources(editor->monIcons[i]);
    }
}

#pragma force_active on

void BallCapsuleEditor_UpdateCapsuleIconStates(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < MAX_CAPSULES; i++) {
        editor->capsuleIcons[i].hasSeals = BallCapsuleEditor_CapsuleHasSeals(editor->capsules[i].capsule);
        editor->capsuleIcons[i].partySlot = editor->capsules[i].partySlot;
        if (editor->capsuleIcons[i].hasSeals == FALSE && editor->capsuleIcons[i].partySlot != BALL_CAPSULE_EDITOR_NO_MON) {
            BallCapsuleEditor_ClearCapsuleMon(editor, i);
        }
    }
}

void BallCapsuleEditor_InitSpriteSystem(BallCapsuleEditorDisplay *display) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion resourceCounts;

    display->spriteSystem = SpriteSystem_Alloc(HEAP_ID_52);
    oamManagerParam = sBallCapsuleEditorOamManagerParam;
    oamCharTransferParam = sBallCapsuleEditorOamCharTransferParam;
    SpriteSystem_Init(display->spriteSystem, &oamManagerParam, &oamCharTransferParam, 32);
    resourceCounts = sBallCapsuleEditorSpriteResourceCounts;
    display->spriteManager = SpriteManager_New(display->spriteSystem);
    if (SpriteSystem_InitSprites(display->spriteSystem, display->spriteManager, 128) == FALSE) {
        GF_AssertFail();
    }
    if (SpriteSystem_InitManagerWithCapacities(display->spriteSystem, display->spriteManager, &resourceCounts) == FALSE) {
        GF_AssertFail();
    }
    display->menuSpawner = TouchscreenListMenuSpawner_Create(HEAP_ID_52, display->paletteData);
}

#pragma force_active reset

void BallCapsuleEditor_FreeSpriteSystem(BallCapsuleEditorDisplay *display) {
    TouchscreenListMenuSpawner_Destroy(display->menuSpawner);
    SpriteSystem_FreeResourcesAndManager(display->spriteSystem, display->spriteManager);
    SpriteSystem_Free(display->spriteSystem);
}

#pragma force_active on

void BallCapsuleEditor_LoadArrowResources(BallCapsuleEditor *editor, NARC *narc) {
    SpriteSystem_LoadCharResObjFromOpenNarc(editor->display.spriteSystem, editor->display.spriteManager, narc, 14, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 45000);
    SpriteSystem_LoadCellResObjFromOpenNarc(editor->display.spriteSystem, editor->display.spriteManager, narc, 16, TRUE, 47000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(editor->display.spriteSystem, editor->display.spriteManager, narc, 15, TRUE, 48000);
}

void BallCapsuleEditor_CreateArrows(BallCapsuleEditor *editor) {
    struct {
        BallCapsuleEditorArrows arrows;
        ManagedSpriteTemplate template;
    } data; // (one struct: matches the original stack layout)

    int i;

    data.arrows = sBallCapsuleEditorArrows;
    for (i = 0; i < 4; i++) {
        data.template.x = data.arrows.arrows[i].x;
        data.template.y = data.arrows.arrows[i].y;
        data.template.z = 0;
        data.template.animation = 0;
        data.template.drawPriority = 40;
        data.template.pal = 2;
        data.template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        data.template.bgPriority = 1;
        data.template.vramTransfer = FALSE;
        data.template.resIdList[GF_GFX_RES_TYPE_CHAR] = 45000;
        data.template.resIdList[GF_GFX_RES_TYPE_PLTT] = 26004;
        data.template.resIdList[GF_GFX_RES_TYPE_CELL] = 47000;
        data.template.resIdList[GF_GFX_RES_TYPE_ANIM] = 48000;
        data.template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
        data.template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
        editor->arrows[i] = SpriteSystem_NewSprite(editor->display.spriteSystem, editor->display.spriteManager, &data.template);
        ((BallCapsuleEditorSetFlipModeFunc)ManagedSprite_SetFlipMode)(editor->arrows[i], data.arrows.arrows[i].flipMode);
    }
}

#pragma force_active reset

void BallCapsuleEditor_TickArrows(BallCapsuleEditor *editor) {
    if (editor->display.inputEnabled != FALSE) {
        s32 i;
        for (i = 0; i < 4; i++) {
            ManagedSprite_TickFrame(editor->arrows[i]);
        }
    }
}

void BallCapsuleEditor_SetArrowsDrawFlag(BallCapsuleEditor *editor, BOOL draw) {
    int i;

    for (i = 0; i < 4; i++) {
        ManagedSprite_SetDrawFlag(editor->arrows[i], draw);
    }
}

void BallCapsuleEditor_DeleteArrows(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < 4; i++) {
        Sprite_DeleteAndFreeResources(editor->arrows[i]);
    }
}

static void BallCapsuleEditor_LoadSpriteResources(SpriteSystem *spriteSystem, SpriteManager *spriteManager, PaletteData *paletteData, int charFile, int paletteFile, int cellFile, int animFile, int vram, int paletteBuffer, int paletteNum, NARC *narc) {
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, charFile, TRUE, vram, charFile + 11000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, (PaletteBufferId)paletteBuffer, spriteSystem, spriteManager, narc, paletteFile, FALSE, paletteNum, vram, paletteFile + 11000);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, cellFile, TRUE, cellFile + 11000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, animFile, TRUE, animFile + 11000);
}

void BallCapsuleEditor_CreateCapsuleIcons(BallCapsuleEditor *editor, NARC *narc) {
    s16 position[4];
    ManagedSpriteTemplate template;
    int i;

    BallCapsuleEditor_LoadSpriteResources(editor->display.spriteSystem, editor->display.spriteManager, editor->display.paletteData, 17, 3, 19, 18, NNS_G2D_VRAM_TYPE_2DMAIN, PLTTBUF_MAIN_OBJ, 1, narc);
    BallCapsuleEditor_LoadSpriteResources(editor->display.spriteSystem, editor->display.spriteManager, editor->display.paletteData, 32, 5, 34, 33, NNS_G2D_VRAM_TYPE_2DMAIN, PLTTBUF_MAIN_OBJ, 1, narc);

    for (i = 0; i < MAX_CAPSULES; i++) {
        template.x = 0;
        template.y = 0;
        template.z = 0;
        template.animation = editor->capsuleIcons[i].hasSeals;
        template.drawPriority = 40 - i;
        template.pal = 0;
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.bgPriority = 2;
        template.vramTransfer = FALSE;
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = 17 + 11000;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = 3 + 11000;
        template.resIdList[GF_GFX_RES_TYPE_CELL] = 19 + 11000;
        template.resIdList[GF_GFX_RES_TYPE_ANIM] = 18 + 11000;
        template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
        template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
        editor->capsuleIcons[i].sprite = SpriteSystem_NewSprite(editor->display.spriteSystem, editor->display.spriteManager, &template);
        BallCapsuleEditor_GetCapsulePosition(i, &position[3], &position[2]);
        ManagedSprite_SetPositionXY(editor->capsuleIcons[i].sprite, position[3], position[2]);
    }

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 20;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = 2;
    template.vramTransfer = FALSE;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 32 + 11000;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 5 + 11000;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 34 + 11000;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 33 + 11000;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    editor->cursors[0] = SpriteSystem_NewSprite(editor->display.spriteSystem, editor->display.spriteManager, &template);
    editor->cursors[1] = SpriteSystem_NewSprite(editor->display.spriteSystem, editor->display.spriteManager, &template);
    BallCapsuleEditor_GetCapsulePosition(editor->cursorCapsule[0], &position[1], &position[0]);
    ManagedSprite_SetPositionXY(editor->cursors[0], position[1], position[0]);
    ManagedSprite_SetPositionXY(editor->cursors[1], position[1], position[0]);
    ManagedSprite_SetDrawPriority(editor->cursors[1], 25);
    ManagedSprite_SetAnim(editor->cursors[0], 0);
    ManagedSprite_SetAnim(editor->cursors[1], 0);
}

void BallCapsuleEditor_RefreshCapsuleIcons(BallCapsuleEditor *editor) {
    int i;

    BallCapsuleEditor_UpdateCapsuleIconStates(editor);
    BallCapsuleEditor_PlaceMonIcons(editor);
    for (i = 0; i < MAX_CAPSULES; i++) {
        ManagedSprite *sprite = editor->capsuleIcons[i].sprite;
        if (sprite != NULL) {
            ManagedSprite_SetAnim(sprite, editor->capsuleIcons[i].hasSeals);
        }
    }
}

void BallCapsuleEditor_SwitchCapsules(BallCapsuleEditor *editor, int a, int b) {
    CAPSULE temp;
    u32 partySlotA = editor->capsules[a].partySlot;
    u32 partySlotB = editor->capsules[b].partySlot;

    if (partySlotA != BALL_CAPSULE_EDITOR_NO_MON) {
        int capsuleId = b + 1;
        SetMonData(editor->args->mons[partySlotA], MON_DATA_BALL_CAPSULE_ID, &capsuleId);
    }
    if (partySlotB != BALL_CAPSULE_EDITOR_NO_MON) {
        int capsuleId = a + 1;
        SetMonData(editor->args->mons[partySlotB], MON_DATA_BALL_CAPSULE_ID, &capsuleId);
    }

    partySlotA = editor->capsules[a].partySlot;
    partySlotB = editor->capsules[b].partySlot;
    editor->capsules[a].partySlot = partySlotB;
    editor->capsules[b].partySlot = partySlotA;

    CopyCapsule(editor->capsules[a].capsule, &temp);
    CopyCapsule(editor->capsules[b].capsule, editor->capsules[a].capsule);
    CopyCapsule(&temp, editor->capsules[b].capsule);
    BallCapsuleEditor_RefreshCapsuleIcons(editor);
}

void BallCapsuleEditor_TickCapsuleIcons(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < MAX_CAPSULES; i++) {
        ManagedSprite_TickFrame(editor->capsuleIcons[i].sprite);
    }
    ManagedSprite_TickFrame(editor->cursors[0]);
    ManagedSprite_TickFrame(editor->cursors[1]);
}

void BallCapsuleEditor_DeleteCapsuleIcons(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < MAX_CAPSULES; i++) {
        Sprite_DeleteAndFreeResources(editor->capsuleIcons[i].sprite);
    }
    Sprite_DeleteAndFreeResources(editor->cursors[0]);
    Sprite_DeleteAndFreeResources(editor->cursors[1]);
}

// (frame is passed but not used)
void BallCapsuleEditor_LoadMainBgGraphics(BgConfig *bgConfig, PaletteData *paletteData, int frame) {
    NNSG2dCharacterData *charData;
    void *allocation;
    void *rawData;
    u32 i;

    allocation = GfGfxLoader_GetCharData(NARC_a_0_8_7, 8, TRUE, &charData, HEAP_ID_52);
    rawData = charData->pRawData;
    DC_FlushRange(rawData, charData->szByte);
    for (i = 0; i < 9; i++) {
        BG_LoadCharTilesData(bgConfig, GF_BG_LYR_MAIN_1, (u8 *)rawData + sBallCapsuleEditorWindowFrameTiles[i] * 32, 32, i + 1);
    }
    Heap_Free(allocation);
    PaletteData_LoadFromNarc(paletteData, NARC_a_0_8_7, 2, HEAP_ID_52, PLTTBUF_MAIN_BG, 0x20, 0xC0, 0x20);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_1, 31, 13, 0, HEAP_ID_52);
    PaletteData_LoadNarc(paletteData, NARC_a_0_3_8, GetUserFrameGfx1PaletteMember(), HEAP_ID_52, PLTTBUF_MAIN_BG, 0x20, 0xD0);
    PaletteData_LoadNarc(paletteData, NARC_graphic_font, 8, HEAP_ID_52, PLTTBUF_MAIN_BG, 0x20, 0xE0);
}

void BallCapsuleEditor_LoadSubBgGraphics(BgConfig *bgConfig, PaletteData *paletteData, int frame) {
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_SUB_0, 1, 15, frame, HEAP_ID_52);
    PaletteData_LoadNarc(paletteData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_52, PLTTBUF_SUB_BG, 0x20, 0xC0);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_SUB_0, 31, 13, 0, HEAP_ID_52);
    PaletteData_LoadNarc(paletteData, NARC_a_0_3_8, GetUserFrameGfx1PaletteMember(), HEAP_ID_52, PLTTBUF_SUB_BG, 0x20, 0xD0);
    PaletteData_LoadNarc(paletteData, NARC_graphic_font, 8, HEAP_ID_52, PLTTBUF_SUB_BG, 0x20, 0xE0);
    PaletteData_LoadNarc(paletteData, NARC_graphic_font, 8, HEAP_ID_52, PLTTBUF_SUB_BG, 0x20, 0x30);
    PaletteData_LoadNarc(paletteData, NARC_a_0_8_7, 0, HEAP_ID_52, PLTTBUF_SUB_BG, 0x20, 0xB0);
}

// msgId 0xFFFF: clear the window.
void BallCapsuleEditor_PrintMessage(Window *window, int frameType, u32 msgId, BOOL drawFrame) {
    MsgData *msgData;
    String *string;

    if (drawFrame) {
        if (frameType == 0) {
            DrawFrameAndWindow1(window, TRUE, 1, 12);
        } else {
            DrawFrameAndWindow2(window, TRUE, 1, 12);
        }
    }
    if (msgId == 0xFFFF) {
        FillWindowPixelBuffer(window, 15);
        ScheduleWindowCopyToVram(window);
        return;
    }
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0011_bin, HEAP_ID_52);
    string = NewString_ReadMsgData(msgData, msgId);
    FillWindowPixelBuffer(window, 15);
    AddTextPrinterParameterized(window, 0, string, 0, 0, TEXT_SPEED_INSTANT, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void BallCapsuleEditor_AddFramedWindow(BgConfig *bgConfig, Window *window, int bgId, int left, int top, int width, int height, int baseTile) {
    InitWindow(window);
    AddWindowParameterized(bgConfig, window, bgId, left, top, width, height, 14, baseTile);
    DrawFrameAndWindow2(window, TRUE, 1, 12);
    FillWindowPixelBuffer(window, 15);
    ScheduleWindowCopyToVram(window);
}

void BallCapsuleEditor_AddWindow(BgConfig *bgConfig, Window *window, int bgId, int left, int top, int width, int height, int baseTile) {
    InitWindow(window);
    AddWindowParameterized(bgConfig, window, bgId, left, top, width, height, 14, baseTile);
    FillWindowPixelBuffer(window, 15);
}

void BallCapsuleEditor_AddWindowWithPalette(BgConfig *bgConfig, Window *window, int bgId, int left, int top, int width, int height, int baseTile, int palette) {
    InitWindow(window);
    AddWindowParameterized(bgConfig, window, bgId, left, top, width, height, palette, baseTile);
    FillWindowPixelBuffer(window, 15);
    ScheduleWindowCopyToVram(window);
}

void BallCapsuleEditor_AddSealCountWindows(BallCapsuleEditor *editor) {
    BallCapsuleEditor_AddWindowWithPalette(editor->display.bgConfig, &editor->display.windows[5], GF_BG_LYR_SUB_2, 3, 2, 4, 2, 0xA6, 11);
    BallCapsuleEditor_AddWindowWithPalette(editor->display.bgConfig, &editor->display.windows[6], GF_BG_LYR_SUB_2, 10, 2, 4, 2, 0xAE, 11);
    BallCapsuleEditor_AddWindowWithPalette(editor->display.bgConfig, &editor->display.windows[7], GF_BG_LYR_SUB_2, 3, 5, 4, 2, 0xB6, 11);
    BallCapsuleEditor_AddWindowWithPalette(editor->display.bgConfig, &editor->display.windows[8], GF_BG_LYR_SUB_2, 10, 5, 4, 2, 0xBE, 11);
    BallCapsuleEditor_AddWindowWithPalette(editor->display.bgConfig, &editor->display.windows[9], GF_BG_LYR_SUB_2, 3, 8, 4, 2, 0xC6, 11);
    BallCapsuleEditor_AddWindowWithPalette(editor->display.bgConfig, &editor->display.windows[10], GF_BG_LYR_SUB_2, 10, 8, 4, 2, 0xCE, 11);
    BallCapsuleEditor_AddWindowWithPalette(editor->display.bgConfig, &editor->display.windows[11], GF_BG_LYR_SUB_2, 3, 11, 4, 2, 0xD6, 11);
    BallCapsuleEditor_AddWindowWithPalette(editor->display.bgConfig, &editor->display.windows[12], GF_BG_LYR_SUB_2, 10, 11, 4, 2, 0xDE, 11);
}

void BallCapsuleEditor_PrintSealCounts(BallCapsuleEditor *editor) {
    int i;
    Window *window;
    u32 seal;

    window = editor->display.windows;
    i = 5;
    window += 5;
    for (; i < 13; i++, window++) {
        seal = editor->pageSeals[i - 5];
        if (seal == SEAL_NONE) {
            FillWindowPixelBuffer(window, 0xEE);
            ScheduleWindowCopyToVram(window);
        } else {
            String *string;
            FillWindowPixelBuffer(window, 0);
            string = String_New(100, HEAP_ID_52);
            String16_FormatInteger(string, SealCaseInventory_GetSealQuantity(editor->sealBag, seal - 1), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            AddTextPrinterParameterizedWithColor(window, 0, string, 7, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
            ScheduleWindowCopyToVram(window);
            String_Delete(string);
        }
    }
}

void BallCapsuleEditor_PrintSealCount(BallCapsuleEditor *editor, int index) {
    String *string;
    Window *window;
    u8 seal;

    window = &editor->display.windows[index + 5];
    seal = editor->pageSeals[index];
    if (seal == SEAL_NONE) {
        FillWindowPixelBuffer(window, 0xEE);
        ScheduleWindowCopyToVram(window);
        return;
    }
    FillWindowPixelBuffer(window, 0);
    string = String_New(100, HEAP_ID_52);
    String16_FormatInteger(string, SealCaseInventory_GetSealQuantity(editor->sealBag, seal - 1), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    AddTextPrinterParameterizedWithColor(window, 0, string, 7, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
}

void BallCapsuleEditor_RemoveSealCountWindows(BallCapsuleEditor *editor) {
    int i;
    Window *window;

    window = editor->display.windows;
    i = 5;
    window += 5;
    for (; i < 13; i++, window++) {
        ClearWindowTilemapAndScheduleTransfer(window);
        RemoveWindow(window);
    }
}

// msgId: a seal name (message bank 15), 0xFFFF: clear the window.
void BallCapsuleEditor_PrintSealName(Window *window, u32 msgId) {
    MsgData *msgData;
    String *string;

    if (msgId == 0xFFFF) {
        FillWindowPixelBuffer(window, 15);
        ScheduleWindowCopyToVram(window);
        return;
    }
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0015_bin, HEAP_ID_52);
    string = NewString_ReadMsgData(msgData, msgId);
    FillWindowPixelBuffer(window, 15);
    AddTextPrinterParameterized(window, 0, string, 0, 0, TEXT_SPEED_INSTANT, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void BallCapsuleEditor_ShowScreens(void) {
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void BallCapsuleEditor_FadeIn(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_52);
}

void BallCapsuleEditor_FadeOut(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_52);
}

void BallCapsuleEditor_LoadCaseBgGraphics(BallCapsuleEditor *editor, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 8, editor->display.bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_52);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, editor->display.bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_52);
    PaletteData_LoadNarc(editor->display.paletteData, NARC_a_0_8_7, 2, HEAP_ID_52, PLTTBUF_MAIN_BG, 0x60, 0);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 9, editor->display.bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_52);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 13, editor->display.bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_52);
}

void BallCapsuleEditor_LoadSubBgCapsuleGraphics(BallCapsuleEditor *editor, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, editor->display.bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_52);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 11, editor->display.bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_52);
    PaletteData_LoadNarc(editor->display.paletteData, NARC_a_0_8_7, 1, HEAP_ID_52, PLTTBUF_SUB_BG, 0x40, 0);
}

void BallCapsuleEditor_LoadSubBgFrameGraphics(BallCapsuleEditor *editor, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, editor->display.bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_52);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, editor->display.bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_52);
    PaletteData_LoadNarc(editor->display.paletteData, NARC_a_0_8_7, 1, HEAP_ID_52, PLTTBUF_SUB_BG, 0x40, 0);
}

void BallCapsuleEditor_LoadCaseButtonResources(BallCapsuleEditor *editor, NARC *narc) {
    SpriteSystem *spriteSystem = editor->display.spriteSystem;
    SpriteManager *spriteManager = editor->display.spriteManager;
    PaletteBufferId buffer = PLTTBUF_SUB_OBJ;

    SpriteSystem_LoadPaletteBufferFromOpenNarc(editor->display.paletteData, buffer, spriteSystem, spriteManager, narc, 4, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 26004);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 26, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 25026);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 28, TRUE, 27028);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 27, TRUE, 28027);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 29, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 25029);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 31, TRUE, 27031);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 30, TRUE, 28030);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 20, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 25020);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 22, TRUE, 27022);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 21, TRUE, 28021);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 23, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 35023);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 25, TRUE, 27025);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 24, TRUE, 28024);
}

#pragma force_active on

void BallCapsuleEditor_CreateCaseButtons(BallCapsuleEditor *editor) {
    ManagedSpriteTemplate template;
    BallCapsuleEditorCasePositions positions;
    BallCapsuleEditorCasePositions hitboxSizes;
    int i;
    SpriteSystem *spriteSystem = editor->display.spriteSystem;
    ManagedSprite **sprites;
    SpriteManager *spriteManager = editor->display.spriteManager;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    sprites = editor->caseSprites;
    template.animation = 0;
    template.vramTransfer = FALSE;
    template.drawPriority = 60;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.bgPriority = 1;
    template.pal = 1;
    template.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 25026;
    template.resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 26004;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 27028;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 28027;
    sprites[BALL_CAPSULE_EDITOR_HITBOX_PREV_PAGE] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 25029;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 26004;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 27031;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 28030;
    sprites[BALL_CAPSULE_EDITOR_HITBOX_NEXT_PAGE] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    template.bgPriority = 1;
    template.pal = 1;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 35023;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 26004;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 27025;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 28024;
    sprites[BALL_CAPSULE_EDITOR_HITBOX_PREVIEW] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    template.bgPriority = 1;
    template.pal = 0;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 25020;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 26004;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 27022;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 28021;
    sprites[BALL_CAPSULE_EDITOR_HITBOX_OK] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    sprites[BALL_CAPSULE_EDITOR_HITBOX_CANCEL] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    positions = sBallCapsuleEditorCaseButtonPositions;
    hitboxSizes = sBallCapsuleEditorCaseButtonHitboxSizes;
    for (i = BALL_CAPSULE_EDITOR_HITBOX_PREV_PAGE; i < BALL_CAPSULE_EDITOR_HITBOX_FIRST_SEAL; i++) {
        ManagedSprite_SetPositionXY(editor->caseSprites[i], positions.positions[i].x, positions.positions[i].y);
        ManagedSprite_TickFrame(editor->caseSprites[i]);
        ManagedSprite_SetAnimationFrame(editor->caseSprites[i], 0);
        BallCapsuleEditor_SetSpriteHitbox(&editor->display.hitboxes[i], editor->caseSprites[i], hitboxSizes.positions[i].x, hitboxSizes.positions[i].y);
    }
}

#pragma force_active reset

void BallCapsuleEditor_SetCaseSpritesDrawFlag(BallCapsuleEditor *editor, BOOL draw) {
    int i;

    for (i = 0; i < BALL_CAPSULE_EDITOR_HITBOX_FIRST_SEAL; i++) {
        ManagedSprite *sprite = editor->caseSprites[i];
        if (sprite) {
            ManagedSprite_SetDrawFlag(sprite, draw);
        }
    }
}

void BallCapsuleEditor_Noop(BallCapsuleEditor *editor) {
}

void BallCapsuleEditor_DeleteCaseButtons(BallCapsuleEditor *editor) {
    int i;

    for (i = BALL_CAPSULE_EDITOR_HITBOX_PREV_PAGE; i < BALL_CAPSULE_EDITOR_HITBOX_FIRST_SEAL; i++) {
        Sprite_DeleteAndFreeResources(editor->caseSprites[i]);
    }
}

BOOL BallCapsuleEditor_CapsuleHasSeals(CAPSULE *capsule) {
    BOOL ret = FALSE;
    int i;
    int count;

    count = ret;
    i = ret;
    for (; i < MAX_SEALS_ON_CAPSULE; i++) {
        if (SealOnCapsuleGetID(CapsuleGetSealI(capsule, i)) != SEAL_NONE) {
            count++;
        }
    }
    if (count != 0) {
        ret = TRUE;
    }
    return ret;
}

// 0: empty capsule, 1: capsule with seals, 2: with seals and on a Pokémon, 3: other
int BallCapsuleEditor_GetMenuKind(BallCapsuleEditor *editor, int capsule) {
    u32 partySlot = editor->capsuleIcons[capsule].partySlot;
    u32 hasSeals = editor->capsuleIcons[capsule].hasSeals;

    if (partySlot == BALL_CAPSULE_EDITOR_NO_MON && hasSeals == FALSE) {
        return 0;
    }
    if (partySlot == BALL_CAPSULE_EDITOR_NO_MON && hasSeals == TRUE) {
        return 1;
    }
    if (partySlot != BALL_CAPSULE_EDITOR_NO_MON && hasSeals == TRUE) {
        return 2;
    }
    return 3;
}

BOOL BallCapsuleEditor_RunMode(BallCapsuleEditor *editor) {
    BOOL ret = sBallCapsuleEditorModes[editor->mode](editor);
    SpriteSystem_DrawSprites(editor->display.spriteManager);
    return ret;
}

BOOL BallCapsuleEditor_ModeSelect(BallCapsuleEditor *editor) {
    NARC *narc;
    int input;

    switch (editor->state) {
    case 0:
        narc = NARC_New(NARC_a_0_8_7, HEAP_ID_52);
        BallCapsuleEditor_LoadMonIconResources(editor);
        BallCapsuleEditor_CreateMonIcons(editor);
        BallCapsuleEditor_PlaceMonIcons(editor);
        BallCapsuleEditor_LoadCaseBgGraphics(editor, narc);
        BallCapsuleEditor_LoadSubBgCapsuleGraphics(editor, narc);
        BallCapsuleEditor_LoadSubBgFrameGraphics(editor, narc);
        BallCapsuleEditor_UpdateCapsuleIconStates(editor);
        BallCapsuleEditor_CreateCapsuleIcons(editor, narc);
        BallCapsuleEditor_AddMessageWindows(editor);
        BallCapsuleEditor_LoadSeals(editor);
        BallCapsuleEditor_SetSealsDrawFlag(editor, TRUE);
        BallCapsuleEditor_ShiftSealsToCapsule(editor);
        BallCapsuleEditor_LoadCaseButtonResources(editor, narc);
        BallCapsuleEditor_CreateCaseButtons(editor);
        BallCapsuleEditor_LoadArrowResources(editor, narc);
        BallCapsuleEditor_CreateArrows(editor);
        BallCapsuleEditor_SaveEditedCapsule(editor);
        BallCapsuleEditor_SaveSealCounts(editor);
        BallCapsuleEditor_FillPageSeals(editor, editor->page);
        BallCapsuleEditor_LoadCaseSealSpriteResources(editor);
        BallCapsuleEditor_CreateCaseSealSprites(editor);
        BallCapsuleEditor_SetCaseSpritesDrawFlag(editor, FALSE);
        BallCapsuleEditor_SetArrowsDrawFlag(editor, FALSE);
        BallCapsuleEditor_CreateButtonLabels(editor);
        BallCapsuleEditor_SetButtonLabelsDrawFlag(editor, FALSE);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00007, FALSE);
        BallCapsuleEditor_ShowScreens();
        BallCapsuleEditor_SetInputEnabled(editor, FALSE);
        BallCapsuleEditor_PrintExitLabel(editor, TRUE);
        GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
        NARC_Delete(narc);
        editor->state++;
        break;
    case 1:
        BallCapsuleEditor_FadeIn();
        editor->state++;
        break;
    case 2:
        if (IsPaletteFadeFinished() != TRUE) {
            break;
        }
        editor->state++;
        // fallthrough
    case 3:
        editor->state = BallCapsuleEditor_HandleSelectInput(editor);
        break;
    case 4:
        input = TouchscreenListMenu_HandleInput(editor->menu);
        switch (input) {
        case LIST_CANCEL:
            BallCapsuleEditor_MenuQuit(editor);
            BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00007, FALSE);
            editor->state = 3;
            break;
        case LIST_NOTHING_CHOSEN:
            break;
        default:
            if (input != 0) {
                if (((BOOL(*)(BallCapsuleEditor *))input)(editor) != TRUE) {
                    BallCapsuleEditor_MenuQuit(editor);
                } else {
                    BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00007, FALSE);
                    editor->state = 3;
                }
            }
            break;
        }
        break;
    case 5:
        BallCapsuleEditor_FadeOut();
        editor->state++;
        break;
    case 6:
        if (IsPaletteFadeFinished() == TRUE) {
            BallCapsuleEditor_RemoveMessageWindows(editor);
            BallCapsuleEditor_DeleteCapsuleIcons(editor);
            BallCapsuleEditor_DeleteMonIcons(editor);
            BallCapsuleEditor_DeleteCaseButtons(editor);
            BallCapsuleEditor_DeleteArrows(editor);
            BallCapsuleEditor_DeleteCaseSealSprites(editor);
            return FALSE;
        }
        break;
    }
    BallCapsuleEditor_TickMonIcons(editor);
    BallCapsuleEditor_TickCapsuleIcons(editor);
    return TRUE;
}

BOOL BallCapsuleEditor_ModeEdit(BallCapsuleEditor *editor) {
    u32 x;
    u32 y;
    int i;
    u32 partySlot;
    BallCapsuleEditorArgs *args;
    Pokemon *mon;
    BOOL touched;
    BOOL onCapsule;

    switch (editor->state) {
    case 0:
        editor->state = BallCapsuleEditor_EnterEdit(editor);
        break;
    case 4:
        editor->state = BallCapsuleEditor_HandleEditKeys(editor);
        break;
    case 5:
        editor->state = BallCapsuleEditor_PreviewCapsule(editor);
        break;
    case 6:
        BallCapsuleEditor_PrintMessage(&editor->display.windows[0], 0, msg_0011_00012, FALSE);
        editor->timer = 20;
        if (BallCapsuleEditor_IsCapsuleChanged(editor) == TRUE) {
            editor->sortSeals = TRUE;
        }
        BallCapsuleEditor_SaveCapsule(editor);
        BallCapsuleEditor_RefreshCapsuleIcons(editor);
        GameStats_AddScore(Save_GameStats_Get(editor->args->saveData), SCORE_EVENT_BALL_CAPSULE_MODIFIED);
        i = editor->cursorCapsule[0];
        partySlot = editor->capsuleIcons[i].partySlot;
        if (partySlot != BALL_CAPSULE_EDITOR_NO_MON) {
            args = editor->args;
            mon = args->mons[partySlot];
            SetMonData(mon, MON_DATA_BALL_CAPSULE, SealCase_GetCapsuleI(args->sealCase, i));
        }
        editor->state = 11;
        break;
    case 7:
        editor->state = BallCapsuleEditor_ConfirmQuitEdit(editor);
        break;
    case 11:
        if (BallCapsuleEditor_LeaveEdit(editor)) {
            return TRUE;
        }
        break;
    }
    if (editor->state == 4 || editor->state == 0) {
        i = editor->display.heldSeal;
        if (i == BALL_CAPSULE_EDITOR_NONE) {
            TouchHitboxController_IsTriggered(editor->display.hitboxController);
        } else {
            touched = System_GetTouchHeldCoords(&x, &y);
            if (editor->state != 4) {
                touched = FALSE;
            }
            if (touched) {
                BallCapsuleEditor_PrintSealName(&editor->display.windows[0], sub_0209106C(editor->seals[i].seal.kind));
                ManagedSprite_SetPositionXY(editor->seals[i].sprite, x, y);
                BallCapsuleEditor_UpdateSealOamMode(editor, i);
                BallCapsuleEditor_StoreSealPosition(editor, i);
            } else {
                onCapsule = BallCapsuleEditor_IsSealOnCapsule(editor, i);
                BallCapsuleEditor_UpdateSealHitbox(editor->seals[i].hitbox, editor->seals[i].sprite, FALSE);
                if (onCapsule == FALSE) {
                    GiveOrTakeSeal2(editor->args->sealCase, editor->seals[i].seal.kind, 1);
                    BallCapsuleEditor_PrintSealCounts(editor);
                    BallCapsuleEditor_RemoveSeal(editor, i);
                }
                PlaySE(SEQ_SE_DP_BOX01);
                editor->display.heldSeal = BALL_CAPSULE_EDITOR_NONE;
                BallCapsuleEditor_PrintSealName(&editor->display.windows[0], 0xFFFF);
            }
        }
    }
    BallCapsuleEditor_Noop(editor);
    BallCapsuleEditor_TickArrows(editor);
    BallCapsuleEditor_TickMonIcons(editor);
    BallCapsuleEditor_TickCapsuleIcons(editor);
    return TRUE;
}

BOOL BallCapsuleEditor_ModeRemove(BallCapsuleEditor *editor) {
    switch (editor->state) {
    case 0:
        BallCapsuleEditor_ClearCapsuleMon(editor, editor->cursorCapsule[0]);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[0], 0, msg_0011_00009, TRUE);
        editor->state++;
        break;
    case 1:
        BallCapsuleEditor_UpdateMenuInputState(editor);
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_PLUS_KEY_MASK)) || System_GetTouchNew()) {
            editor->state++;
        }
        break;
    case 2:
        BallCapsuleEditor_SetMode(editor, BALL_CAPSULE_EDITOR_MODE_SELECT, 0xFF, 0);
        EraseFrameAndWindow1(&editor->display.windows[0], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&editor->display.windows[0]);
        BallCapsuleEditor_PrintExitLabel(editor, TRUE);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00007, FALSE);
        break;
    }
    BallCapsuleEditor_TickMonIcons(editor);
    BallCapsuleEditor_TickCapsuleIcons(editor);
    return TRUE;
}

#pragma force_active on

BOOL BallCapsuleEditor_ModeSwitch(BallCapsuleEditor *editor) {
    switch (editor->state) {
    case 0:
        ManagedSprite_SetDrawPriority(editor->cursors[0], 25);
        ManagedSprite_SetDrawPriority(editor->cursors[1], 20);
        ManagedSprite_SetAnim(editor->cursors[0], 1);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00010, FALSE);
        ManagedSprite_SetDrawFlag(editor->cursors[1], TRUE);
        editor->state++;
        // fallthrough
    case 1:
        editor->state = BallCapsuleEditor_HandleSwitchInput(editor);
        break;
    case 2:
        BallCapsuleEditor_UpdateMenuInputState(editor);
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_PLUS_KEY_MASK)) || System_GetTouchNew()) {
            editor->state = 3;
        }
        break;
    case 3:
        ManagedSprite_SetDrawPriority(editor->cursors[0], 20);
        ManagedSprite_SetDrawPriority(editor->cursors[1], 25);
        BallCapsuleEditor_SetMode(editor, BALL_CAPSULE_EDITOR_MODE_SELECT, 0xFF, 0);
        ManagedSprite_SetAnim(editor->cursors[0], 0);
        EraseFrameAndWindow1(&editor->display.windows[0], TRUE);
        ClearWindowTilemapAndScheduleTransfer(&editor->display.windows[0]);
        BallCapsuleEditor_PrintExitLabel(editor, TRUE);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00007, FALSE);
        break;
    }
    BallCapsuleEditor_TickMonIcons(editor);
    BallCapsuleEditor_TickCapsuleIcons(editor);
    return TRUE;
}

#pragma force_active reset

BOOL BallCapsuleEditor_ModeNone(BallCapsuleEditor *editor) {
    return FALSE;
}

int BallCapsuleEditor_EnterEdit(BallCapsuleEditor *editor) {
    switch (editor->subState) {
    case 0:
        BallCapsuleEditor_SetInputEnabled(editor, FALSE);
        BallCapsuleEditor_AddSealCountWindows(editor);
        BallCapsuleEditor_PrintSealCounts(editor);
        BallCapsuleEditor_SaveEditedCapsule(editor);
        BallCapsuleEditor_SaveSealCounts(editor);
        editor->sortSeals = FALSE;
        editor->timer = 0;
        BallCapsuleEditor_FadeOut();
        editor->subState++;
        break;
    case 1:
        if (ov57_0223B940(editor) == 1 || IsPaletteFadeFinished() != TRUE) {
            break;
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        SetBgPriority(GF_BG_LYR_SUB_3, 1);
        BallCapsuleEditor_SetCaseSpritesDrawFlag(editor, TRUE);
        BallCapsuleEditor_SetArrowsDrawFlag(editor, TRUE);
        BallCapsuleEditor_SetButtonLabelsDrawFlag(editor, TRUE);
        BallCapsuleEditor_MoveSeals(editor, 1);
        ManagedSprite_SetAnimationFrame(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_PREV_PAGE], 0);
        ManagedSprite_SetAnimationFrame(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_NEXT_PAGE], 0);
        ManagedSprite_SetAnimationFrame(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_PREVIEW], 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        ClearWindowTilemapAndCopyToVram(&editor->display.windows[3]);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[0], 0, 0xFFFF, TRUE);
        GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
        BallCapsuleEditor_FadeIn();
        editor->subState++;
        break;
    case 2:
        if (IsPaletteFadeFinished() != TRUE) {
            break;
        }
        BallCapsuleEditor_SetInputEnabled(editor, TRUE);
        editor->subState = 0;
        return 4;
    }
    return 0;
}

#pragma force_active on

BOOL BallCapsuleEditor_LeaveEdit(BallCapsuleEditor *editor) {
    switch (editor->subState) {
    case 0:
        if (editor->timer > 0) {
            editor->timer--;
            return FALSE;
        }
        editor->timer = 0;
        BallCapsuleEditor_FadeOut();
        editor->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() != TRUE) {
            break;
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        SetBgPriority(GF_BG_LYR_MAIN_3, 3);
        SetBgPriority(GF_BG_LYR_SUB_3, 3);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        BallCapsuleEditor_SetCaseSpritesDrawFlag(editor, FALSE);
        BallCapsuleEditor_SetArrowsDrawFlag(editor, FALSE);
        BallCapsuleEditor_SetButtonLabelsDrawFlag(editor, FALSE);
        EraseFrameAndWindow1(&editor->display.windows[0], TRUE);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00007, FALSE);
        CopyWindowToVram(&editor->display.windows[3]);
        BallCapsuleEditor_PrintExitLabel(editor, TRUE);
        GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
        BallCapsuleEditor_MoveSeals(editor, -1);
        BallCapsuleEditor_SetInputEnabled(editor, FALSE);
        BallCapsuleEditor_FadeIn();
        editor->subState++;
        break;
    case 2:
        if (ov57_0223B940(editor) == 1) {
            break;
        }
        if (IsPaletteFadeFinished() != TRUE) {
            break;
        }
        ManagedSprite_SetDrawFlag(editor->cursors[1], FALSE);
        BallCapsuleEditor_SetMode(editor, BALL_CAPSULE_EDITOR_MODE_SELECT, 0xFF, 0);
        BallCapsuleEditor_RemoveSealCountWindows(editor);
        editor->subState = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

// "Quit without changing the Capsule design?"
int BallCapsuleEditor_ConfirmQuitEdit(BallCapsuleEditor *editor) {
    int result;
    YesNoPromptTemplate template;

    switch (editor->subState) {
    case 0:
        if (!BallCapsuleEditor_IsCapsuleChanged(editor)) {
            return 11;
        }
        PaletteData_BeginPaletteFade(editor->display.paletteData, 2, 0x80B, 0, 0, 10, RGB_BLACK);
        PaletteData_BeginPaletteFade(editor->display.paletteData, 8, 0xFFFF, 0, 0, 10, RGB_BLACK);
        BallCapsuleEditor_SetInputEnabled(editor, FALSE);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        editor->subState++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(editor->display.paletteData) != 0) {
            break;
        }
        PaletteData_SetAutoTransparent(editor->display.paletteData, FALSE);
        BallCapsuleEditor_AddFramedWindow(editor->display.bgConfig, &editor->display.windows[4], GF_BG_LYR_SUB_0, 2, 1, 27, 4, 58);
        MI_CpuFill8(&template, 0, sizeof(template));
        template.bgConfig = editor->display.bgConfig;
        template.bgId = GF_BG_LYR_SUB_0;
        template.tileStart = 230;
        template.plttSlot = 5;
        template.x = 25;
        template.y = 6;
        template.ignoreTouchFlag = editor->menuInputState;
        editor->display.yesNoPrompt = YesNoPrompt_Create(HEAP_ID_52);
        YesNoPrompt_InitFromTemplate(editor->display.yesNoPrompt, &template);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[4], 4, msg_0011_00014, FALSE);
        editor->subState++;
        break;
    case 2:
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        editor->subState++;
        // fallthrough
    case 3:
        result = YesNoPrompt_HandleInput(editor->display.yesNoPrompt);
        if (result == YESNORESPONSE_WAIT) {
            break;
        }
        if (result != YESNORESPONSE_YES && result != YESNORESPONSE_NO) {
            break;
        }
        editor->menuInputState = YesNoPrompt_IsInTouchMode(editor->display.yesNoPrompt);
        editor->subStateValue = result;
        PaletteData_SetAutoTransparent(editor->display.paletteData, TRUE);
        YesNoPrompt_Reset(editor->display.yesNoPrompt);
        YesNoPrompt_Destroy(editor->display.yesNoPrompt);
        ClearFrameAndWindow2(&editor->display.windows[4], FALSE);
        RemoveWindow(&editor->display.windows[4]);
        editor->subState++;
        break;
    case 4:
        PaletteData_BeginPaletteFade(editor->display.paletteData, 2, 0x80B, 0, 10, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(editor->display.paletteData, 8, 0xFFFF, 0, 10, 0, RGB_BLACK);
        editor->subState++;
        break;
    case 5:
        if (PaletteData_GetSelectedBuffersBitmask(editor->display.paletteData) != 0) {
            break;
        }
        switch (editor->subStateValue) {
        case YESNORESPONSE_WAIT:
            goto other;
        case YESNORESPONSE_YES:
            BallCapsuleEditor_RestoreEditedCapsule(editor);
            BallCapsuleEditor_RestoreSealCounts(editor);
            BallCapsuleEditor_PrintSealCounts(editor);
            BallCapsuleEditor_RemoveAllSeals(editor);
            BallCapsuleEditor_LoadSeals(editor);
            BallCapsuleEditor_SetSealsDrawFlag(editor, TRUE);
            result = 6;
            editor->timer = 20;
            break;
        case YESNORESPONSE_NO:
            result = 4;
            break;
        default:
other:
            result = 7;
            break;
        }
        BallCapsuleEditor_SetInputEnabled(editor, TRUE);
        editor->subState = 0;
        return result;
    }
    return 7;
}

// "Confirming...": the party Pokémon (or the first non-Egg) comes out of its ball with the capsule's seals.
int BallCapsuleEditor_PreviewCapsule(BallCapsuleEditor *editor) {
    u32 capsuleId;
    UnkStruct_134 ballParams;
    CAPSULE capsule;
    BallCapsuleEditorPokepicEffectParams effectParams;
    UnkStruct_134 ballParams2;
    int result;

    switch (editor->subState) {
    case 0:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        PaletteData_BeginPaletteFade(editor->display.paletteData, 2, 0x80B, 0, 0, 10, RGB_BLACK);
        PaletteData_BeginPaletteFade(editor->display.paletteData, 8, 0xFFFF, 0, 0, 10, RGB_BLACK);
        BallCapsuleEditor_SetInputEnabled(editor, FALSE);
        editor->subStateValue = 0;
        BallCapsuleEditor_CopyPreviewMon(editor);
        BallCapsuleEditor_CreatePreviewPokepic(editor);
        BallCapsuleEditor_SetPreviewPokepicHidden(editor, TRUE);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[0], 0, msg_0011_00013, FALSE);
        editor->subState++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(editor->display.paletteData) == 0) {
            ballParams.kind = 1;
            ballParams.heapID = HEAP_ID_52;
            ballParams.mode = 0;
            ballParams.resTagOffset = 0xFF;
            ballParams.bgPriority = 0;
            ballParams.keepSubSurface = 1;
            ballParams.spriteSystem = editor->display.spriteSystem;
            ballParams.paletteData = editor->display.paletteData;
            ballParams.ball = GetMonData(editor->previewMon, MON_DATA_POKEBALL, NULL);
            editor->ballData = UnkBallData_New(&ballParams);
            editor->subState++;
        }
        break;
    case 2: {
        volatile u32 *clear = (u32 *)&effectParams; // (volatile stores: matches the original)
        clear[0] = 0;
        clear[1] = 0;
        clear[2] = 0;
    }
        effectParams.kind = 1;
        effectParams.mon = editor->previewMon;
        capsuleId = editor->cursorCapsule[0] + 1;
        BallCapsuleEditor_GetSealData(capsule.seals, editor);
        SetMonData(editor->previewMon, MON_DATA_BALL_CAPSULE_ID, &capsuleId);
        SetMonData(editor->previewMon, MON_DATA_BALL_CAPSULE, &capsule);
        editor->pokepicEffect = BallSealEffect_New(HEAP_ID_52, &effectParams);
        BallSealEffect_LoadResources(editor->pokepicEffect);
        editor->subState++;
        editor->subStateValue = 0;
        break;
    case 3:
        if (BallSealEffect_IsLoaded(editor->pokepicEffect) == TRUE && UnkBallData_HasOpened(editor->ballData) == TRUE) {
            BallCapsuleEditor_GrowPreviewPokepic(editor);
            BallSealEffect_Start(editor->pokepicEffect);
            BallCapsuleEditor_SetPreviewPokepicHidden(editor, FALSE);
            BallCapsuleEditor_ClearPreviewPokepicScale(editor);
            PlaySE(SEQ_SE_DP_CUSTOM05);
            editor->subState++;
        }
        break;
    case 4:
        result = BallCapsuleEditor_GrowPreviewPokepic(editor);
        if (UnkBallData_IsActive(editor->ballData) == FALSE && result == FALSE) {
            UnkBallData_Delete(editor->ballData);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            BallCapsuleEditor_StartPreviewPokepicAnim(editor);
            editor->subStateValue = 0;
            editor->subState++;
        }
        break;
    case 5:
        if (BallSealEffect_IsActive(editor->pokepicEffect) == FALSE && PokepicAnimManager_GetAnimState(editor->pokepicAnim, 0) == TRUE && Pokepic_IsAnimFinished(editor->display.pokepic) == FALSE) {
            BallSealEffect_Free(editor->pokepicEffect);
            editor->subState++;
        }
        break;
    case 6:
        editor->subStateValue++;
        if (editor->subStateValue >= 30) {
            editor->subStateValue = 0;
            ballParams2.kind = 1;
            ballParams2.heapID = HEAP_ID_52;
            ballParams2.mode = 5;
            ballParams2.resTagOffset = 0xFF;
            ballParams2.bgPriority = 0;
            ballParams2.keepSubSurface = 1;
            ballParams2.spriteSystem = editor->display.spriteSystem;
            ballParams2.paletteData = editor->display.paletteData;
            ballParams2.ball = GetMonData(editor->previewMon, MON_DATA_POKEBALL, NULL);
            editor->ballData = UnkBallData_New(&ballParams2);
            BallCapsuleEditor_ResetBlend();
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            editor->subState++;
        }
        break;
    case 7:
        result = BallCapsuleEditor_ShrinkPreviewPokepic(editor);
        if (UnkBallData_IsActive(editor->ballData) == FALSE && result == FALSE) {
            PaletteData_BeginPaletteFade(editor->display.paletteData, 2, 0x80B, 0, 10, 0, RGB_BLACK);
            PaletteData_BeginPaletteFade(editor->display.paletteData, 8, 0xFFFF, 0, 10, 0, RGB_BLACK);
            BallCapsuleEditor_SetPreviewPokepicHidden(editor, TRUE);
            BallCapsuleEditor_DeletePreviewPokepic(editor);
            UnkBallData_Delete(editor->ballData);
            editor->subState++;
        }
        break;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(editor->display.paletteData) == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            BallCapsuleEditor_SetInputEnabled(editor, TRUE);
            editor->subState = 0;
            editor->state = 4;
            BallCapsuleEditor_PrintMessage(&editor->display.windows[0], 0, 0xFFFF, FALSE);
            return 4;
        }
        break;
    }
    return 5;
}

#pragma force_active on

void BallCapsuleEditor_HitboxCallback(u32 index, u32 event, void *arg) {
    BallCapsuleEditor *editor = arg;

    if (!editor->display.inputEnabled) {
        return;
    }
    switch (index) {
    case BALL_CAPSULE_EDITOR_HITBOX_PREV_PAGE:
        if (event == 0) {
            if (editor->page > 0) {
                editor->page--;
            } else {
                editor->page = editor->pageCount - 1;
            }
            BallCapsuleEditor_DeleteCaseSealSprites(editor);
            BallCapsuleEditor_FillPageSeals(editor, editor->page);
            BallCapsuleEditor_LoadCaseSealSpriteResources(editor);
            BallCapsuleEditor_CreateCaseSealSprites(editor);
            BallCapsuleEditor_PrintSealCounts(editor);
            PlaySE(SEQ_SE_DP_CUSTOM02);
        }
        BallCapsuleEditor_SetButtonFrame(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_PREV_PAGE], event);
        break;
    case BALL_CAPSULE_EDITOR_HITBOX_NEXT_PAGE:
        if (event == 0) {
            editor->page++;
            editor->page %= editor->pageCount;
            BallCapsuleEditor_DeleteCaseSealSprites(editor);
            BallCapsuleEditor_FillPageSeals(editor, editor->page);
            BallCapsuleEditor_LoadCaseSealSpriteResources(editor);
            BallCapsuleEditor_CreateCaseSealSprites(editor);
            BallCapsuleEditor_PrintSealCounts(editor);
            PlaySE(SEQ_SE_DP_CUSTOM02);
        }
        BallCapsuleEditor_SetButtonFrame(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_NEXT_PAGE], event);
        break;
    case BALL_CAPSULE_EDITOR_HITBOX_PREVIEW:
        if (event == 0) {
            if (editor->state != 5) {
                editor->state = 5;
                PlaySE(SEQ_SE_DP_DECIDE);
            }
            BallCapsuleEditor_StartButtonPress(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_PREVIEW], NULL);
        }
        BallCapsuleEditor_SetButtonFrame(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_PREVIEW], event);
        break;
    case BALL_CAPSULE_EDITOR_HITBOX_OK:
        if (event == 0) {
            if (editor->state != 6) {
                editor->state = 6;
                BallCapsuleEditor_SetInputEnabled(editor, FALSE);
                PlaySE(SEQ_SE_DP_PIRORIRO);
            }
            BallCapsuleEditor_StartButtonPress(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_OK], editor->buttonLabels[0]);
        }
        BallCapsuleEditor_SetButtonFrame(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_OK], event);
        break;
    case BALL_CAPSULE_EDITOR_HITBOX_CANCEL:
        if (event == 0) {
            if (editor->state != 7) {
                editor->state = 7;
                BallCapsuleEditor_SetInputEnabled(editor, FALSE);
                PlaySE(SEQ_SE_DP_DECIDE);
            }
            BallCapsuleEditor_StartButtonPress(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_CANCEL], editor->buttonLabels[1]);
        }
        BallCapsuleEditor_SetButtonFrame(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_CANCEL], event);
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (event == 0) {
            if (!BallCapsuleEditor_HasFreeSealSlot(editor)) {
                PlaySE(SEQ_SE_DP_CUSTOM06);
                BallCapsuleEditor_PrintMessage(&editor->display.windows[0], 0, msg_0011_00015, FALSE);
                return;
            }
            if (editor->pageSeals[index] != SEAL_NONE && SealCaseInventory_GetSealQuantity(editor->sealBag, editor->pageSeals[index] - 1) != 0) {
                editor->display.heldSeal = BallCapsuleEditor_AddSeal(editor, (u8)index);
                BallCapsuleEditor_PrintSealName(&editor->display.windows[0], sub_0209106C(editor->pageSeals[index]));
                GiveOrTakeSeal(editor->args->sealCase, editor->pageSeals[index], -1);
                BallCapsuleEditor_PrintSealCount(editor, index);
                PlaySE(SEQ_SE_DP_BOX02);
            } else if (editor->pageSeals[index] != SEAL_NONE) {
                PlaySE(SEQ_SE_DP_CUSTOM06);
                BallCapsuleEditor_PrintMessage(&editor->display.windows[0], 0, msg_0011_00016, FALSE);
            }
        }
        break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        if (event == 0) {
            int seal = index - BALL_CAPSULE_EDITOR_HITBOX_FIRST_SEAL;
            BallCapsuleEditor_HandleSealTouch(editor, event, (u8)seal);
            BallCapsuleEditor_PrintSealName(&editor->display.windows[0], sub_0209106C(editor->seals[seal].seal.kind));
            PlaySE(SEQ_SE_DP_BOX02);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
}

#pragma force_active reset

// Switches between key and touch input: returns TRUE when a key press ended touch input.
int BallCapsuleEditor_UpdateMenuInputState(BallCapsuleEditor *editor) {
    if (editor->menuInputState == MENU_INPUT_STATE_TOUCH) {
        if (System_GetTouchHeld()) {
            return FALSE;
        }
        if (gSystem.heldKeys != 0) {
            editor->menuInputState = MENU_INPUT_STATE_BUTTONS;
            return TRUE;
        }
    } else {
        if (gSystem.heldKeys != 0) {
            return FALSE;
        }
        if (System_GetTouchHeld()) {
            editor->menuInputState = MENU_INPUT_STATE_TOUCH;
            return FALSE;
        }
    }
    return FALSE;
}

int BallCapsuleEditor_HandleSelectInput(BallCapsuleEditor *editor) {
    BallCapsuleEditor_UpdateMenuInputState(editor);
    if (editor->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        return BallCapsuleEditor_HandleSelectKeys(editor);
    } else {
        return BallCapsuleEditor_HandleSelectTouch(editor);
    }
}

int BallCapsuleEditor_HandleSelection(BallCapsuleEditor *editor, int selection) {
    switch (selection) {
    case BALL_CAPSULE_EDITOR_SELECT_BACK:
        PlaySE(SEQ_SE_DP_DECIDE);
        BallCapsuleEditor_SetArgsSetCapsuleOnMon(editor->args, FALSE);
        return 5;
    case BALL_CAPSULE_EDITOR_SELECT_CONFIRM:
        BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00008, FALSE);
        BallCapsuleEditor_CreateMenu(editor->display.bgConfig, &editor->display.windows[1], 1, editor, editor->cursorCapsule[0]);
        PlaySE(SEQ_SE_DP_SELECT);
        return 4;
    default:
        break;
    }
    ((void (*)(void))BallCapsuleEditor_SortSeals)(); // called without an argument
    BallCapsuleEditor_SaveCapsule(editor);
    BallCapsuleEditor_RefreshCapsuleIcons(editor);
    editor->cursorCapsule[0] = selection;
    BallCapsuleEditor_UpdateCursors(editor, 0, 1);
    BallCapsuleEditor_RemoveAllSeals(editor);
    BallCapsuleEditor_LoadSeals(editor);
    BallCapsuleEditor_SetSealsDrawFlag(editor, TRUE);
    BallCapsuleEditor_ShiftSealsToCapsule(editor);
    if (editor->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        PlaySE(SEQ_SE_DP_SELECT);
    } else {
        BallCapsuleEditor_PrintMessage(&editor->display.windows[3], 3, msg_0011_00008, FALSE);
        BallCapsuleEditor_CreateMenu(editor->display.bgConfig, &editor->display.windows[1], 1, editor, editor->cursorCapsule[0]);
        PlaySE(SEQ_SE_DP_SELECT);
        return 4;
    }
    return 3;
}

#pragma force_active on

int BallCapsuleEditor_HandleSelectKeys(BallCapsuleEditor *editor) {
    int selection;

    if (gSystem.newKeys & PAD_BUTTON_B) {
        return BallCapsuleEditor_HandleSelection(editor, BALL_CAPSULE_EDITOR_SELECT_BACK);
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return BallCapsuleEditor_HandleSelection(editor, BALL_CAPSULE_EDITOR_SELECT_CONFIRM);
    }
    if (!(gSystem.newAndRepeatedKeys & PAD_PLUS_KEY_MASK)) {
        return 3;
    }
    selection = editor->cursorCapsule[0];
    switch (BallCapsuleEditor_MoveCapsuleCursor(&selection)) {
    case FALSE:
        return 3;
    default:
        return BallCapsuleEditor_HandleSelection(editor, (u8)selection);
    }
}

#pragma force_active reset

int BallCapsuleEditor_HandleSelectTouch(BallCapsuleEditor *editor) {
    int selection = BallCapsuleEditor_GetTouchedCapsule(editor);

    if (selection < 0) {
        return 3;
    }
    return BallCapsuleEditor_HandleSelection(editor, (u8)selection);
}

int BallCapsuleEditor_HandleSwitchInput(BallCapsuleEditor *editor) {
    BallCapsuleEditor_UpdateMenuInputState(editor);
    if (editor->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        return BallCapsuleEditor_HandleSwitchKeys(editor);
    }
    return BallCapsuleEditor_HandleSwitchTouch(editor);
}

#pragma force_active on

int BallCapsuleEditor_FinishSwitch(BallCapsuleEditor *editor, BOOL confirm) {
    int target = editor->cursorCapsule[1];
    int current = editor->cursorCapsule[0];

    if (current == target) {
        confirm = FALSE;
    }
    if (confirm) {
        BallCapsuleEditor_SwitchCapsules(editor, current, target);
        BallCapsuleEditor_UpdateCursors(editor, 1, 1);
        BallCapsuleEditor_PrintMessage(&editor->display.windows[0], 0, msg_0011_00011, TRUE);
        PlaySE(SEQ_SE_DP_SELECT);
        return 2;
    }
    ManagedSprite_SetDrawFlag(editor->cursors[1], FALSE);
    ManagedSprite_SetAnim(editor->cursors[0], 0);
    BallCapsuleEditor_UpdateCursors(editor, 0, 0);
    PlaySE(SEQ_SE_DP_DECIDE);
    return 3;
}

#pragma force_active reset

int BallCapsuleEditor_HandleSwitchSelection(BallCapsuleEditor *editor, int selection) {
    switch (selection) {
    case BALL_CAPSULE_EDITOR_SELECT_BACK:
        return BallCapsuleEditor_FinishSwitch(editor, FALSE);
    case BALL_CAPSULE_EDITOR_SELECT_CONFIRM:
        return BallCapsuleEditor_FinishSwitch(editor, TRUE);
    default:
        break;
    }
    editor->cursorCapsule[1] = selection;
    BallCapsuleEditor_UpdateCursors(editor, 1, 0);
    if (editor->menuInputState == MENU_INPUT_STATE_BUTTONS) {
        PlaySE(SEQ_SE_DP_SELECT);
    } else {
        return BallCapsuleEditor_FinishSwitch(editor, TRUE);
    }
    return 1;
}

#pragma force_active on

int BallCapsuleEditor_HandleSwitchKeys(BallCapsuleEditor *editor) {
    int selection;

    if (gSystem.newKeys & PAD_BUTTON_B) {
        return BallCapsuleEditor_HandleSwitchSelection(editor, BALL_CAPSULE_EDITOR_SELECT_BACK);
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return BallCapsuleEditor_HandleSwitchSelection(editor, BALL_CAPSULE_EDITOR_SELECT_CONFIRM);
    }
    if (!(gSystem.newAndRepeatedKeys & PAD_PLUS_KEY_MASK)) {
        return 1;
    }
    selection = editor->cursorCapsule[1];
    if (BallCapsuleEditor_MoveCapsuleCursor(&selection) == FALSE) {
        return 1;
    }
    return BallCapsuleEditor_HandleSwitchSelection(editor, (u8)selection);
}

#pragma force_active reset

int BallCapsuleEditor_HandleSwitchTouch(BallCapsuleEditor *editor) {
    int selection = BallCapsuleEditor_GetTouchedCapsule(editor);

    if (selection < 0) {
        return 1;
    }
    return BallCapsuleEditor_HandleSwitchSelection(editor, (u8)selection);
}

// The capsules are a 4x3 grid.
BOOL BallCapsuleEditor_MoveCapsuleCursor(int *capsule) {
    u32 keys = gSystem.newAndRepeatedKeys;

    if (keys & PAD_KEY_RIGHT) {
        (*capsule)++;
        *capsule %= MAX_CAPSULES;
    } else if (keys & PAD_KEY_LEFT) {
        if (*capsule > 0) {
            (*capsule)--;
        } else {
            *capsule = MAX_CAPSULES - 1;
        }
    } else if (keys & PAD_KEY_UP) {
        if (*capsule / 4 != 0) {
            *capsule -= 4;
            *capsule %= MAX_CAPSULES;
        } else {
            return FALSE;
        }
    } else if (keys & PAD_KEY_DOWN) {
        if (*capsule / 4 != 2) {
            *capsule += 4;
            *capsule %= MAX_CAPSULES;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
    return TRUE;
}

// Moves cursors[which] to its capsule; copy == 1: the other cursor follows.
void BallCapsuleEditor_UpdateCursors(BallCapsuleEditor *editor, int which, int copy) {
    struct {
        s16 y, x;
    } position;

    int other = 0;

    if (which == 0) {
        other = 1;
    }
    BallCapsuleEditor_GetCapsulePosition(editor->cursorCapsule[which], &position.x, &position.y);
    ManagedSprite_SetPositionXY(editor->cursors[which], position.x, position.y);
    if (copy == 1) {
        editor->cursorCapsule[other] = editor->cursorCapsule[which];
        BallCapsuleEditor_GetCapsulePosition(editor->cursorCapsule[other], &position.x, &position.y);
        ManagedSprite_SetPositionXY(editor->cursors[other], position.x, position.y);
    }
}

// The touched capsule (0-11), BALL_CAPSULE_EDITOR_SELECT_BACK / _CONFIRM for the capsule area, or -1.
#pragma force_active on

int BallCapsuleEditor_GetTouchedCapsule(BallCapsuleEditor *editor) {
    struct {
        u16 pixel;
        TouchscreenHitbox hitbox;
    } local;

    int row, col, y, x, found;

    if (!System_GetTouchNew()) {
        return -1;
    }
    local.hitbox.circle.sentinel = TOUCHSCREEN_CIRCLE_SENTINEL;
    local.hitbox.circle.r = 24;
    y = 24;
    for (row = 0; row < 3; row++) {
        local.hitbox.circle.y = y;
        for (col = 0, x = 40; col < 4; col++, x += 56) {
            local.hitbox.circle.x = x;
            if (TouchscreenHitbox_TouchNewIsIn(&local.hitbox)) {
                return row * 4 + col;
            }
        }
        y += 56;
    }
    found = TouchscreenHitbox_FindRectAtTouchNew(sBallCapsuleEditorExitButtonHitboxes);
    if (found == -1) {
        return -1;
    }
    local.pixel = 1023;
    if (!DoesPixelAtScreenXYMatchPtrVal(editor->display.bgConfig, GF_BG_LYR_MAIN_3, gSystem.touchX, gSystem.touchY, &local.pixel)) {
        found = -1;
    } else {
        found += BALL_CAPSULE_EDITOR_SELECT_BACK;
    }
    return found;
}

#pragma force_active reset

int BallCapsuleEditor_HandleEditKeys(BallCapsuleEditor *editor) {
    u32 keys;

    BallCapsuleEditor_UpdateMenuInputState(editor);
    keys = gSystem.newKeys;
    if (keys & PAD_BUTTON_A) {
        if (editor->display.heldSeal != BALL_CAPSULE_EDITOR_NONE) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
            return 4;
        }
        BallCapsuleEditor_SetInputEnabled(editor, FALSE);
        PlaySE(SEQ_SE_DP_PIRORIRO);
        BallCapsuleEditor_StartButtonPress(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_OK], editor->buttonLabels[0]);
        return 6;
    }
    if (keys & PAD_BUTTON_B) {
        if (editor->display.heldSeal != BALL_CAPSULE_EDITOR_NONE) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
            return 4;
        }
        BallCapsuleEditor_SetInputEnabled(editor, FALSE);
        PlaySE(SEQ_SE_DP_DECIDE);
        BallCapsuleEditor_StartButtonPress(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_CANCEL], editor->buttonLabels[1]);
        return 7;
    }
    if (keys & (PAD_BUTTON_X | PAD_BUTTON_Y)) {
        if (editor->display.heldSeal != BALL_CAPSULE_EDITOR_NONE) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
            return 4;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        BallCapsuleEditor_StartButtonPress(editor->caseSprites[BALL_CAPSULE_EDITOR_HITBOX_PREVIEW], NULL);
        return 5;
    }
    return 4;
}

BOOL BallCapsuleEditor_MenuEdit(BallCapsuleEditor *editor) {
    BallCapsuleEditor_SetMode(editor, BALL_CAPSULE_EDITOR_MODE_EDIT, 0, 3);
    return FALSE;
}

BOOL BallCapsuleEditor_MenuQuit(BallCapsuleEditor *editor) {
    BallCapsuleEditor_DestroyMenu(editor);
    BallCapsuleEditor_PrintExitLabel(editor, TRUE);
    return TRUE;
}

#pragma force_active on

BOOL BallCapsuleEditor_MenuSet(BallCapsuleEditor *editor) {
    BallCapsuleEditor_SortSeals(editor);
    BallCapsuleEditor_SaveCapsule(editor);
    BallCapsuleEditor_RefreshCapsuleIcons(editor);
    BallCapsuleEditor_SetArgsSetCapsuleOnMon(editor->args, TRUE);
    editor->state = 5;
    return FALSE;
}

#pragma force_active reset

BOOL BallCapsuleEditor_MenuRemove(BallCapsuleEditor *editor) {
    BallCapsuleEditor_SetMode(editor, BALL_CAPSULE_EDITOR_MODE_REMOVE, 0, 3);
    return FALSE;
}

BOOL BallCapsuleEditor_MenuSwitch(BallCapsuleEditor *editor) {
    BallCapsuleEditor_SetMode(editor, BALL_CAPSULE_EDITOR_MODE_SWITCH, 0, 3);
    return FALSE;
}

void BallCapsuleEditor_SaveEditedCapsule(BallCapsuleEditor *editor) {
    u32 i = editor->cursorCapsule[0];
    CopyCapsule(editor->capsules[i].capsule, &editor->savedCapsule);
}

void BallCapsuleEditor_RestoreEditedCapsule(BallCapsuleEditor *editor) {
    u32 i = editor->cursorCapsule[0];
    CopyCapsule(&editor->savedCapsule, editor->capsules[i].capsule);
}

void BallCapsuleEditor_SaveSealCounts(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < SEAL_MAX + 1; i++) {
        editor->savedSealCounts[i] = SealCaseInventory_GetSealQuantity(editor->sealBag, i);
    }
}

void BallCapsuleEditor_RestoreSealCounts(BallCapsuleEditor *editor) {
    int i;

    for (i = 0; i < SEAL_MAX + 1; i++) {
        SealCaseInventory_SetSealQuantity(editor->sealBag, i, editor->savedSealCounts[i]);
    }
}

// The Pokémon with the capsule, else the first party Pokémon that is not an Egg.
#pragma force_active on

void BallCapsuleEditor_CopyPreviewMon(BallCapsuleEditor *editor) {
    int partySlot = editor->capsules[editor->cursorCapsule[0]].partySlot;
    int i;

    if (partySlot != BALL_CAPSULE_EDITOR_NO_MON) {
        CopyPokemonToPokemon(editor->args->mons[partySlot], editor->previewMon);
        return;
    }
    for (i = 0; i < editor->args->partyCount; i++) {
        Pokemon *mon = editor->args->mons[i];
        if (GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE) {
            CopyPokemonToPokemon(mon, editor->previewMon);
            return;
        }
    }
}

#pragma force_active reset

// state 0xFF: the saved returnState.
void BallCapsuleEditor_SetMode(BallCapsuleEditor *editor, u32 mode, u32 state, u32 returnState) {
    if (state == 0xFF) {
        editor->state = editor->returnState;
    } else {
        editor->state = state;
    }
    editor->mode = mode;
    editor->returnState = returnState;
    editor->subState = 0;
}

void BallCapsuleEditor_SetButtonFrame(ManagedSprite *button, u32 event) {
    switch (event) {
    case 0:
        ManagedSprite_SetAnimationFrame(button, 1);
        break;
    case 2:
        ManagedSprite_SetAnimationFrame(button, 2);
        break;
    case 1:
    case 3:
        ManagedSprite_SetAnimationFrame(button, 0);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void BallCapsuleEditor_MoveButtonLabel(TextOBJ *label, s32 dx, s32 dy) {
    s32 x, y;

    if (label) {
        TextOBJ_GetPosition(label, &x, &y);
        TextOBJ_SetPosition(label, x + dx, y + dy);
    }
}

void BallCapsuleEditor_Task_ButtonPress(SysTask *task, void *data) {
    BallCapsuleEditorButtonPress *press = data;

    switch (press->state) {
    case 3:
        BallCapsuleEditor_MoveButtonLabel(press->label, 0, -1);
        ManagedSprite_SetAnimationFrame(press->button, 2);
        press->state++;
        break;
    case 6:
        BallCapsuleEditor_MoveButtonLabel(press->label, 0, 2);
        ManagedSprite_SetAnimationFrame(press->button, 0);
        SysTask_Destroy(task);
        Heap_Free(press);
        break;
    default:
        press->state++;
        break;
    }
}

#pragma force_active on

SysTask *BallCapsuleEditor_StartButtonPress(ManagedSprite *button, TextOBJ *label) {
    BallCapsuleEditorButtonPress *press = Heap_Alloc(HEAP_ID_52, sizeof(BallCapsuleEditorButtonPress));

    press->state = 1;
    press->button = button;
    press->label = label;
    BallCapsuleEditor_MoveButtonLabel(press->label, 0, -1);
    return SysTask_CreateOnMainQueue(BallCapsuleEditor_Task_ButtonPress, press, 1000);
}

#pragma force_active reset

u32 ov57_0223B940(BallCapsuleEditor *editor) {
    return editor->unk_280;
}

void BallCapsuleEditor_SetInputEnabled(BallCapsuleEditor *editor, BOOL enabled) {
    editor->display.inputEnabled = enabled;
}

#pragma force_active on

void BallCapsuleEditor_CreatePreviewPokepic(BallCapsuleEditor *editor) {
    PokepicTemplate template;
    PokepicAnimScript animScript[10];
    int species, y;

    GetPokemonSpriteCharAndPlttNarcIds(&template, editor->previewMon, 2);
    species = GetMonData(editor->previewMon, MON_DATA_SPECIES, NULL);
    y = GetBoxMonPicHeight((BoxPokemon *)editor->previewMon, 2);
    editor->display.pokepicY = y;
    NARC_ReadPokepicAnimScript(editor->pokepicAnimNarc, animScript, (u16)species, 1);
    editor->display.pokepic = PokepicManager_CreatePokepic(editor->display.pokepicManager, &template, 192, y + 48, -640, 0, animScript, NULL);
}

void BallCapsuleEditor_StartPreviewPokepicAnim(BallCapsuleEditor *editor) {
    int species = GetMonData(editor->previewMon, MON_DATA_SPECIES, NULL);

    GetMonNature(editor->previewMon);
    ((void (*)(Pokepic *, int))Pokepic_StartAnim)(editor->display.pokepic, 1); // called with an extra argument
    NARC_StartPokepicAnim(editor->pokepicAnimNarc, editor->pokepicAnim, editor->display.pokepic, (u16)species, 2, 0, 0);
}

#pragma force_active reset

void BallCapsuleEditor_ClearPreviewPokepicScale(BallCapsuleEditor *editor) {
    Pokepic_SetAttr(editor->display.pokepic, POKEPIC_AFFINEW, 0);
    Pokepic_SetAttr(editor->display.pokepic, POKEPIC_AFFINEH, 0);
}

BOOL BallCapsuleEditor_GrowPreviewPokepic(BallCapsuleEditor *editor) {
    if (Pokepic_GetAttr(editor->display.pokepic, POKEPIC_AFFINEW) == 0x100) {
        return FALSE;
    }
    if (Pokepic_GetAttr(editor->display.pokepic, POKEPIC_AFFINEW) >= 0x100) {
        Pokepic_SetAttr(editor->display.pokepic, POKEPIC_AFFINEW, 0x100);
        Pokepic_SetAttr(editor->display.pokepic, POKEPIC_AFFINEH, 0x100);
        return FALSE;
    }
    Pokepic_AddAttr(editor->display.pokepic, POKEPIC_AFFINEW, 0x20);
    Pokepic_AddAttr(editor->display.pokepic, POKEPIC_AFFINEH, 0x20);
    Pokepic_SetYOffsetForAffineHeight(editor->display.pokepic, editor->display.pokepicY);
    return TRUE;
}

BOOL BallCapsuleEditor_ShrinkPreviewPokepic(BallCapsuleEditor *editor) {
    if (Pokepic_GetAttr(editor->display.pokepic, POKEPIC_AFFINEW) == 0) {
        return FALSE;
    }
    if (Pokepic_GetAttr(editor->display.pokepic, POKEPIC_AFFINEW) <= 0) {
        Pokepic_SetAttr(editor->display.pokepic, POKEPIC_AFFINEW, 0);
        Pokepic_SetAttr(editor->display.pokepic, POKEPIC_AFFINEH, 0);
        return FALSE;
    }
    Pokepic_AddAttr(editor->display.pokepic, POKEPIC_AFFINEW, -0x20);
    Pokepic_AddAttr(editor->display.pokepic, POKEPIC_AFFINEH, -0x20);
    Pokepic_SetYOffsetForAffineHeight(editor->display.pokepic, editor->display.pokepicY);
    return TRUE;
}

void BallCapsuleEditor_SetPreviewPokepicHidden(BallCapsuleEditor *editor, BOOL hidden) {
    Pokepic_SetAttr(editor->display.pokepic, POKEPIC_VANISHED, hidden);
}

void BallCapsuleEditor_DeletePreviewPokepic(BallCapsuleEditor *editor) {
    Pokepic_Delete(editor->display.pokepic);
}

void BallCapsuleEditor_ResetBlend(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 31);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 31);
}

#pragma force_active on

void BallCapsuleEditor_InitHitboxes(BallCapsuleEditor *editor) {
    const TouchscreenHitbox empty = { 0 };
    int i;

    for (i = 0; i < BALL_CAPSULE_EDITOR_HITBOX_FIRST_SEAL; i++) {
        editor->display.hitboxes[i] = sBallCapsuleEditorHitboxes[i];
    }
    for (; i < BALL_CAPSULE_EDITOR_NUM_HITBOXES; i++) {
        editor->display.hitboxes[i] = empty;
        editor->seals[i - BALL_CAPSULE_EDITOR_HITBOX_FIRST_SEAL].hitbox = &editor->display.hitboxes[i];
    }
    editor->display.hitboxController = TouchHitboxController_Create(editor->display.hitboxes, BALL_CAPSULE_EDITOR_NUM_HITBOXES, BallCapsuleEditor_HitboxCallback, editor, HEAP_ID_52);
}

#pragma force_active reset

void BallCapsuleEditor_ClearCapsuleMon(BallCapsuleEditor *editor, int capsule) {
    u32 capsuleId = 0;

    if (editor->capsules[capsule].partySlot != BALL_CAPSULE_EDITOR_NO_MON) {
        SetMonData(editor->args->mons[editor->capsules[capsule].partySlot], MON_DATA_BALL_CAPSULE_ID, &capsuleId);
    }
    editor->capsules[capsule].partySlot = BALL_CAPSULE_EDITOR_NO_MON;
    BallCapsuleEditor_RefreshCapsuleIcons(editor);
}
