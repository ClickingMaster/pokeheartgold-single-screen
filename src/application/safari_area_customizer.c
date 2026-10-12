#include "safari_area_customizer.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "safari_zone.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sprite_transfer.h"
#include "sys_flags.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "unk_02009D48.h"
#include "oam_manager.h"
#include "font_sprite.h"
#include "touchpad.h"
#include "unk_0208805C.h"
#include "safari_zone_encounters.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

#define NUM_AREA_SLOTS 6 // areas in a set (2 rows of 3)

// Sprite resources of the area picture sprites (12 = the 6 slots of the set and the 6 areas of a page).
typedef struct SafariAreaCustomizerSpriteRes {
    enum HeapID heapId;                // 0x000
    int transferMode;                  // 0x004
    u16 unk_08;                        // 0x008
    u16 unk_0A;                        // 0x00A
    u16 maxSprites;                    // 0x00C
    u16 vram;                          // 0x00E
    SpriteList *spriteList;            // 0x010
    SpriteList *externalSpriteList;    // 0x014
    G2dRenderer renderer;              // 0x018
    u8 filler_140[4];                  // 0x140
    GF_2DGfxResMan *resMan[4];         // 0x144
    GF_2DGfxResObjList *resObjList[4]; // 0x154
} SafariAreaCustomizerSpriteRes;       // size: 0x164

typedef struct SafariAreaCustomizer {
    enum HeapID heapId;                                       // 0x00000
    int state;                                                // 0x00004
    int subState;                                             // 0x00008
    int nextState;                                            // 0x0000C
    u32 touchMode;                                            // 0x00010
    SafariAreaCustomizerArgs *args;                           // 0x00014
    SafariZone *safariZone;                                   // 0x00018
    SafariZoneAreaSet areaSet;                                // 0x0001C
    MsgData *msgData;                                         // 0x00304
    MessageFormat *msgFormat;                                 // 0x00308
    String *string;                                           // 0x0030C
    String *labelStrings[5];                                  // 0x00310
    String *menuStrings[4];                                   // 0x00324
    String *pageTemplate;                                     // 0x00334
    String *countTemplate;                                    // 0x00338
    u8 filler_33C[4];                                         // 0x0033C
    BgConfig *bgConfig;                                       // 0x00340
    TouchscreenListMenuSpawner *listMenuSpawner;              // 0x00344
    SafariAreaCustomizerSpriteRes *spriteRes;                 // 0x00348
    SpriteSystem *spriteSystem;                               // 0x0034C
    SpriteManager *spriteManager;                             // 0x00350
    Sprite *sprites[6];                                       // 0x00354: 0 slot cursor, 1 area cursor, 4/5 page arrows
    ManagedSprite *areaSprites[2 * NUM_AREA_SLOTS];           // 0x0036C: 2 banks of 6, swapped when the page changes
    FontSystem *fontSystem;                           // 0x0039C
    TextOBJLayout *fontLayout;                           // 0x003A0
    TextOBJ *textObj;                                         // 0x003A4
    UnkStruct_02021AC8 charTransfer;                          // 0x003A8
    Window windows[15];                                       // 0x003B4
    Window textObjWindow;                                     // 0x004A4
    ListMenuItem *areaMenuItems;                              // 0x004B4
    ListMenuItem *confirmMenuItems;                           // 0x004B8
    TouchscreenListMenu *listMenu;                            // 0x004BC
    YesNoPrompt *yesNoPrompt;                                 // 0x004C0
    void (*onSwitchToButtons)(struct SafariAreaCustomizer *); // 0x004C4
    void (*onSwitchToTouch)(struct SafariAreaCustomizer *);   // 0x004C8
    void *selectFrameScreenFile;                              // 0x004CC
    void *pageScreenFile;                                     // 0x004D0
    NNSG2dScreenData *selectFrameScreen;                      // 0x004D4
    NNSG2dScreenData *pageScreen;                             // 0x004D8
    u8 filler_4DC[0x18000];                                   // 0x004DC
    u8 textSpeed;                                             // 0x184DC
    u8 frame;                                                 // 0x184DD
    u8 page;                                                  // 0x184DE
    u8 selectedSlot;                                          // 0x184DF
    u8 cursorPos;                                             // 0x184E0
    u8 animFrame;                                             // 0x184E1
    u8 scrollPending : 1;                                     // 0x184E2
    u8 pageDir : 1;                                           // 0x184E2
    u8 spriteBank : 1;                                        // 0x184E2
    u8 mode : 5;                                              // 0x184E2
    u8 objectsUnlocked;                                       // 0x184E3
    s16 mainBgY;                                              // 0x184E4
    s16 subBgY;                                               // 0x184E6
    BOOL modified;                                            // 0x184E8
} SafariAreaCustomizer;                                       // size: 0x184EC

typedef struct SafariAreaCustomizerBgTemplates {
    BgTemplate templates[8];
} SafariAreaCustomizerBgTemplates;

typedef struct SafariAreaCustomizerResCounts {
    u8 counts[4];
} SafariAreaCustomizerResCounts;

// _021EA720 holds the per-row scroll directions of the slot swap; ov108_021EA724 (the label text colours)
// and the list menu template follow it without labels of their own.
typedef struct SafariAreaCustomizerConsts {
    s8 swapDir[2][2];
    u32 labelColors[2];
    TouchscreenListMenuTemplate menuTemplate;
} SafariAreaCustomizerConsts;

extern const SafariAreaCustomizerConsts _021EA720;
extern const u32 ov108_021EA724[2];
extern const GraphicsModes ov108_021EA738;
extern const u16 ov108_021EA748[];
extern const u16 ov108_021EA74A[];
extern const TouchscreenHitbox ov108_021EA760[];
extern const GraphicsBanks ov108_021EA780;
extern const WindowTemplate ov108_021EA7A8[5];
extern const TouchscreenHitbox ov108_021EA7D0[];
extern const UnmanagedSpriteTemplate ov108_021EA7F8;
extern const UnmanagedSpriteTemplate ov108_021EA820;
extern const UnmanagedSpriteTemplate ov108_021EA848;
extern const UnmanagedSpriteTemplate ov108_021EA870;
extern const SafariAreaCustomizerBgTemplates ov108_021EA898;
extern const SafariAreaCustomizerResCounts ov108_021EA978;
extern const ResdatIdList ov108_021EA97C;
extern const OamCharTransferParam ov108_021EA98C;
extern const OamManagerParam ov108_021EA9A0;

void SafariAreaCustomizer_InitFromArgs(SafariAreaCustomizer *s, SafariAreaCustomizerArgs *args);
void SafariAreaCustomizer_SaveResults(SafariAreaCustomizer *s);
int SafariAreaCustomizer_State_Setup(SafariAreaCustomizer *s);
int SafariAreaCustomizer_State_Teardown(SafariAreaCustomizer *s);
int SafariAreaCustomizer_State_SelectSlot(SafariAreaCustomizer *s);
int SafariAreaCustomizer_State_SelectArea(SafariAreaCustomizer *s);
int SafariAreaCustomizer_State_MoveArea(SafariAreaCustomizer *s);
int SafariAreaCustomizer_State_FadeIn(SafariAreaCustomizer *s);
int SafariAreaCustomizer_State_FadeOut(SafariAreaCustomizer *s);
int SafariAreaCustomizer_Setup(SafariAreaCustomizer *s);
int SafariAreaCustomizer_Teardown(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleSlotInput(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleSlotMenu(SafariAreaCustomizer *s);
int SafariAreaCustomizer_ScrollPages(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleAreaInput(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleConfirmArea(SafariAreaCustomizer *s);
int SafariAreaCustomizer_ChangePage(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleMoveInput(SafariAreaCustomizer *s);
int SafariAreaCustomizer_SwapAreas(SafariAreaCustomizer *s);
void SafariAreaCustomizer_ShowCursors(SafariAreaCustomizer *s);
void ov108_021E6238(SafariAreaCustomizer *s);
int SafariAreaCustomizer_CheckInputModeSwitch(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleSlotButtons(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleSlotTouch(SafariAreaCustomizer *s);
int SafariAreaCustomizer_ChooseArea(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleAreaButtons(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleAreaTouch(SafariAreaCustomizer *s);
void SafariAreaCustomizer_StartMove(SafariAreaCustomizer *s);
void SafariAreaCustomizer_EndMove(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleMoveButtons(SafariAreaCustomizer *s);
int SafariAreaCustomizer_HandleMoveTouch(SafariAreaCustomizer *s);
void SafariAreaCustomizer_OpenSlotMenu(SafariAreaCustomizer *s);
void SafariAreaCustomizer_AskConfirmArea(SafariAreaCustomizer *s);
void SafariAreaCustomizer_VBlankCB(void *cbArg);
void SafariAreaCustomizer_SetGfxBanks(void);
void SafariAreaCustomizer_InitBgLayers(SafariAreaCustomizer *s);
void SafariAreaCustomizer_FreeBgLayers(SafariAreaCustomizer *s);
void SafariAreaCustomizer_LoadBgGraphics(SafariAreaCustomizer *s);
void SafariAreaCustomizer_FreeScreenData(SafariAreaCustomizer *s);
void SafariAreaCustomizer_CreateStrings(SafariAreaCustomizer *s);
void SafariAreaCustomizer_DeleteStrings(SafariAreaCustomizer *s);
void SafariAreaCustomizer_AddWindows(SafariAreaCustomizer *s);
void SafariAreaCustomizer_RemoveWindows(SafariAreaCustomizer *s);
void SafariAreaCustomizer_CreateMenuItems(SafariAreaCustomizer *s);
void SafariAreaCustomizer_DeleteMenuItems(SafariAreaCustomizer *s);
void SafariAreaCustomizer_InitSprites(SafariAreaCustomizer *s);
void SafariAreaCustomizer_FreeSprites(SafariAreaCustomizer *s);
void SafariAreaCustomizer_CreateSprites(SafariAreaCustomizer *s);
void SafariAreaCustomizer_DeleteSprites(SafariAreaCustomizer *s);
void SafariAreaCustomizer_CreateCancelTextObj(SafariAreaCustomizer *s);
void SafariAreaCustomizer_DeleteCancelTextObj(SafariAreaCustomizer *s);
void ov108_021E767C(SafariAreaCustomizer *s, int value);
void SafariAreaCustomizer_PrintWindowText(SafariAreaCustomizer *s, u8 windowId, u8 msg, BOOL unused);
void SafariAreaCustomizer_PrintPageNumber(SafariAreaCustomizer *s);
void SafariAreaCustomizer_SetCursorAnim(SafariAreaCustomizer *s, u32 index, u32 anim, BOOL draw);
void SafariAreaCustomizer_MoveCursor(SafariAreaCustomizer *s, u32 index, u32 pos);
void SafariAreaCustomizer_DrawSelectFrame(SafariAreaCustomizer *s, u8 layer, int pos, BOOL show);
void SafariAreaCustomizer_InitSlotTilemaps(SafariAreaCustomizer *s);
void SafariAreaCustomizer_LoadAreaPictureFromNarc(SafariAreaCustomizer *s, NARC *narc, u32 slot, int area);
void SafariAreaCustomizer_LoadAreaPicture(SafariAreaCustomizer *s, u32 slot, int area);
void SafariAreaCustomizer_InitScreen(SafariAreaCustomizer *s);
void SafariAreaCustomizer_PrintObjectTypeLabels(SafariAreaCustomizer *s);
void SafariAreaCustomizer_PrintSlotInfo(SafariAreaCustomizer *s, u32 slot);
void SafariAreaCustomizer_PrintAreaNames(SafariAreaCustomizer *s);
void SafariAreaCustomizer_StartPageScroll(SafariAreaCustomizer *s);
BOOL SafariAreaCustomizer_RunPageScroll(SafariAreaCustomizer *s);
void SafariAreaCustomizer_StartPageChange(SafariAreaCustomizer *s);
BOOL SafariAreaCustomizer_RunPageChange(SafariAreaCustomizer *s);
BOOL SafariAreaCustomizer_RunSwapAnim(SafariAreaCustomizer *s, int dir);
void SafariAreaCustomizer_InitSpriteSystem(SafariAreaCustomizer *s);
void SafariAreaCustomizer_FreeSpriteSystem(SafariAreaCustomizer *s);
void SafariAreaCustomizer_CreateSpriteManager(SafariAreaCustomizer *s);
void SafariAreaCustomizer_DestroySpriteManager(SafariAreaCustomizer *s);
void SafariAreaCustomizer_DrawSprites(SafariAreaCustomizer *s);
void SafariAreaCustomizer_TransferOam(SafariAreaCustomizer *s);
SafariAreaCustomizerSpriteRes *SafariAreaCustomizerSpriteRes_New(enum HeapID heapId, u16 maxSprites, u16 vram, int transferMode);
void SafariAreaCustomizerSpriteRes_Delete(SafariAreaCustomizerSpriteRes *res);
void SafariAreaCustomizerSpriteRes_SetAreaPicture(SafariAreaCustomizerSpriteRes *res, u8 index, u8 area);
void SafariAreaCustomizerSpriteRes_DrawSprites(SafariAreaCustomizerSpriteRes *res);
void SafariAreaCustomizerSpriteRes_SetSpriteList(SafariAreaCustomizerSpriteRes *res, SpriteList *spriteList);
ManagedSprite *SafariAreaCustomizerSpriteRes_CreateAreaSprite(SafariAreaCustomizerSpriteRes *res, u16 x, u16 y, u8 transfer, u8 priority, u8 index, u8 area, int vram);
void SafariAreaCustomizer_DeleteAreaSprite(ManagedSprite *sprite);
void SafariAreaCustomizerSpriteRes_Init(SafariAreaCustomizerSpriteRes *res);
void SafariAreaCustomizerSpriteRes_Free(SafariAreaCustomizerSpriteRes *res);
void SafariAreaCustomizerSpriteRes_LoadResources(SafariAreaCustomizerSpriteRes *res);

BOOL SafariAreaCustomizer_Init(OverlayManager *man, int *state) {
    SafariAreaCustomizerArgs *args = OverlayManager_GetArgs(man);
    SafariAreaCustomizer *s;

#pragma unused(state)
    Heap_Create(HEAP_ID_3, HEAP_ID_95, 0x50000);
    s = OverlayManager_CreateAndGetData(man, sizeof(SafariAreaCustomizer), HEAP_ID_95);
    MI_CpuFill8(s, 0, sizeof(SafariAreaCustomizer));
    s->heapId = HEAP_ID_95;
    SafariAreaCustomizer_InitFromArgs(s, args);
    Sound_SetSceneAndPlayBGM(0x43, 0, 0);
    return TRUE;
}

BOOL SafariAreaCustomizer_Exit(OverlayManager *man, int *state) {
    SafariAreaCustomizer *s = OverlayManager_GetData(man);
    enum HeapID heapId;

#pragma unused(state)
    SafariAreaCustomizer_SaveResults(s);
    heapId = s->heapId;
    MI_CpuFill8(s, 0, sizeof(SafariAreaCustomizer));
    OverlayManager_FreeData(man);
    Heap_Destroy(heapId);
    return TRUE;
}

BOOL SafariAreaCustomizer_Main(OverlayManager *man, int *state) {
    SafariAreaCustomizer *s = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        *state = SafariAreaCustomizer_State_Setup(s);
        break;
    case 1:
        *state = SafariAreaCustomizer_State_Teardown(s);
        break;
    case 2:
        *state = SafariAreaCustomizer_State_SelectSlot(s);
        break;
    case 3:
        *state = SafariAreaCustomizer_State_SelectArea(s);
        break;
    case 4:
        *state = SafariAreaCustomizer_State_MoveArea(s);
        break;
    case 5:
        *state = SafariAreaCustomizer_State_FadeIn(s);
        break;
    case 6:
        *state = SafariAreaCustomizer_State_FadeOut(s);
        break;
    case 7:
        return TRUE;
    }
    return FALSE;
}

void SafariAreaCustomizer_InitFromArgs(SafariAreaCustomizer *s, SafariAreaCustomizerArgs *args) {
    Options *options;

    s->safariZone = Save_SafariZone_Get(args->saveData);
    SafariZone_CopyAreaSet(s->safariZone, 0, &s->areaSet);
    options = Save_PlayerData_GetOptionsAddr(args->saveData);
    s->textSpeed = Options_GetTextSpeed(options);
    s->frame = Options_GetFrame(options);
    s->touchMode = MenuInputStateMgr_GetState(args->menuInputStatePtr);
    s->modified = FALSE;
    s->args = args;
    if (SafariZone_GetObjectUnlockLevel(s->safariZone)) {
        s->objectsUnlocked = TRUE;
    }
}

void SafariAreaCustomizer_SaveResults(SafariAreaCustomizer *s) {
    SafariZone_SetAreaSet(s->safariZone, 0, &s->areaSet);
    ChangeFlag99D(Save_VarsFlags_Get(s->args->saveData), s->modified);
    MenuInputStateMgr_SetState(s->args->menuInputStatePtr, (MenuInputState)s->touchMode);
}

int SafariAreaCustomizer_State_Setup(SafariAreaCustomizer *s) {
    if (SafariAreaCustomizer_Setup(s)) {
        return 5;
    }
    return 0;
}

int SafariAreaCustomizer_State_Teardown(SafariAreaCustomizer *s) {
    if (SafariAreaCustomizer_Teardown(s)) {
        return 7;
    }
    return 1;
}

int SafariAreaCustomizer_State_SelectSlot(SafariAreaCustomizer *s) {
    switch (s->state) {
    case 0:
        s->state = SafariAreaCustomizer_HandleSlotInput(s);
        break;
    case 1:
        s->state = SafariAreaCustomizer_HandleSlotMenu(s);
        break;
    case 2:
        s->state = SafariAreaCustomizer_ScrollPages(s);
        break;
    case 3:
        break;
    case 4:
        s->state = 0;
        SafariAreaCustomizer_DrawSprites(s);
        return s->nextState;
    }
    SafariAreaCustomizer_DrawSprites(s);
    return 2;
}

int SafariAreaCustomizer_State_SelectArea(SafariAreaCustomizer *s) {
    switch (s->state) {
    case 0:
        s->state = SafariAreaCustomizer_HandleAreaInput(s);
        break;
    case 1:
        s->state = SafariAreaCustomizer_HandleConfirmArea(s);
        break;
    case 2:
        s->state = SafariAreaCustomizer_ScrollPages(s);
        break;
    case 3:
        s->state = SafariAreaCustomizer_ChangePage(s);
        break;
    case 4:
        s->state = 0;
        SafariAreaCustomizer_DrawSprites(s);
        return s->nextState;
    }
    SafariAreaCustomizer_DrawSprites(s);
    return 3;
}

int SafariAreaCustomizer_State_MoveArea(SafariAreaCustomizer *s) {
    switch (s->state) {
    case 0:
        s->state = SafariAreaCustomizer_HandleMoveInput(s);
        break;
    case 1:
        break;
    case 2:
        s->state = SafariAreaCustomizer_ScrollPages(s);
        break;
    case 3:
        s->state = SafariAreaCustomizer_SwapAreas(s);
        break;
    case 4:
        s->state = 0;
        SafariAreaCustomizer_DrawSprites(s);
        return s->nextState;
    }
    SafariAreaCustomizer_DrawSprites(s);
    return 4;
}

int SafariAreaCustomizer_State_FadeIn(SafariAreaCustomizer *s) {
    switch (s->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, s->heapId);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        s->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            s->state = 0;
            SafariAreaCustomizer_DrawSprites(s);
            return 2;
        }
        break;
    }
    SafariAreaCustomizer_DrawSprites(s);
    return 5;
}

int SafariAreaCustomizer_State_FadeOut(SafariAreaCustomizer *s) {
    switch (s->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, s->heapId);
        s->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            s->state = 0;
            SafariAreaCustomizer_DrawSprites(s);
            return 1;
        }
        break;
    }
    SafariAreaCustomizer_DrawSprites(s);
    return 6;
}

#pragma force_active on
int SafariAreaCustomizer_Setup(SafariAreaCustomizer *s) {
    switch (s->subState) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
        ResetVisibleHardwareWindows(PM_LCD_TOP);
        ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(4);
        break;
    case 1:
        SafariAreaCustomizer_InitBgLayers(s);
        SafariAreaCustomizer_LoadBgGraphics(s);
        SafariAreaCustomizer_CreateStrings(s);
        SafariAreaCustomizer_AddWindows(s);
        SafariAreaCustomizer_CreateMenuItems(s);
        SafariAreaCustomizer_InitSprites(s);
        SafariAreaCustomizer_CreateSprites(s);
        break;
    case 2:
        SafariAreaCustomizer_InitScreen(s);
        Main_SetVBlankIntrCB(SafariAreaCustomizer_VBlankCB, s);
        s->subState = 0;
        return TRUE;
    }
    s->subState++;
    return FALSE;
}

int SafariAreaCustomizer_Teardown(SafariAreaCustomizer *s) {
    switch (s->subState) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        SafariAreaCustomizer_DeleteSprites(s);
        SafariAreaCustomizer_FreeSprites(s);
        SafariAreaCustomizer_DeleteMenuItems(s);
        SafariAreaCustomizer_RemoveWindows(s);
        SafariAreaCustomizer_DeleteStrings(s);
        SafariAreaCustomizer_FreeScreenData(s);
        SafariAreaCustomizer_FreeBgLayers(s);
        break;
    case 2:
        GF_TouchpadStopSampling();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        s->subState = 0;
        return TRUE;
    }
    s->subState++;
    return FALSE;
}
#pragma force_active reset

int SafariAreaCustomizer_HandleSlotInput(SafariAreaCustomizer *s) {
    if (SafariAreaCustomizer_CheckInputModeSwitch(s)) {
        return 0;
    }
    if (s->touchMode == MENU_INPUT_STATE_BUTTONS) {
        return SafariAreaCustomizer_HandleSlotButtons(s);
    }
    return SafariAreaCustomizer_HandleSlotTouch(s);
}

int SafariAreaCustomizer_HandleSlotMenu(SafariAreaCustomizer *s) {
    s32 input = TouchscreenListMenu_HandleInput(s->listMenu);
    u32 touchMode;

    if (input == -1) {
        return 1;
    }
    touchMode = s->listMenu->isTouch;
    TouchscreenListMenu_Destroy(s->listMenu);
    if (touchMode != s->touchMode) {
        if (touchMode == MENU_INPUT_STATE_BUTTONS) {
            SafariAreaCustomizer_ShowCursors(s);
        } else {
            ov108_021E6238(s);
        }
    }
    s->touchMode = touchMode;
    switch (input) {
    case 0:
        s->cursorPos = 0;
        return 2;
    case 1:
        SafariAreaCustomizer_DrawSelectFrame(s, 1, 0, FALSE);
        SafariAreaCustomizer_DrawSelectFrame(s, 4, 0, FALSE);
        s->cursorPos = s->selectedSlot;
        SafariAreaCustomizer_StartMove(s);
        s->nextState = 4;
        return 4;
    case 2:
    default:
        SafariAreaCustomizer_DrawSelectFrame(s, 1, 0, FALSE);
        SafariAreaCustomizer_DrawSelectFrame(s, 4, 0, FALSE);
        return 0;
    }
}

int SafariAreaCustomizer_ScrollPages(SafariAreaCustomizer *s) {
    switch (s->subState) {
    case 0:
        SafariAreaCustomizer_StartPageScroll(s);
        s->subState++;
        break;
    case 1:
        if (SafariAreaCustomizer_RunPageScroll(s)) {
            s->subState++;
        }
        break;
    default:
        reg_GXS_DB_DISPCNT &= ~(REG_GXS_DB_DISPCNT_W0_MASK | REG_GXS_DB_DISPCNT_W1_MASK | REG_GXS_DB_DISPCNT_OW_MASK);
        s->animFrame = 0;
        s->subState = 0;
        if (s->mode == 0) {
            s->nextState = 3;
            SafariAreaCustomizer_PrintWindowText(s, 0, 1, TRUE);
            SafariAreaCustomizer_PrintAreaNames(s);
            s->mode = 1;
        } else {
            s->nextState = 2;
            SafariAreaCustomizer_PrintWindowText(s, 0, 0, TRUE);
            SafariAreaCustomizer_PrintSlotInfo(s, s->selectedSlot);
            s->mode = 0;
        }
        return 4;
    }
    return 2;
}

int SafariAreaCustomizer_HandleAreaInput(SafariAreaCustomizer *s) {
    if (SafariAreaCustomizer_CheckInputModeSwitch(s)) {
        return 0;
    }
    if (s->touchMode == MENU_INPUT_STATE_BUTTONS) {
        return SafariAreaCustomizer_HandleAreaButtons(s);
    }
    return SafariAreaCustomizer_HandleAreaTouch(s);
}

#pragma force_active on
int SafariAreaCustomizer_HandleConfirmArea(SafariAreaCustomizer *s) {
    int yes;
    int touchMode;

    switch (YesNoPrompt_HandleInput(s->yesNoPrompt)) {
    case YESNORESPONSE_YES: {
        u8 chosen;
        u8 current;

        yes = TRUE;
        chosen = (u8)(s->page * NUM_AREA_SLOTS + s->cursorPos);
        current = s->areaSet.areas[s->selectedSlot].area_no;
        if (chosen != current) {
            s->modified = TRUE;
            SafariZone_InitAreaInSet(&s->areaSet, s->selectedSlot, chosen);
        }
        break;
    }
    case YESNORESPONSE_NO:
        SafariAreaCustomizer_LoadAreaPicture(s, s->selectedSlot, s->areaSet.areas[s->selectedSlot].area_no);
        yes = FALSE;
        break;
    default:
        return 1;
    }
    touchMode = YesNoPrompt_IsInTouchMode(s->yesNoPrompt);
    if (touchMode != s->touchMode) {
        if (!touchMode) {
            SafariAreaCustomizer_ShowCursors(s);
        } else {
            ov108_021E6238(s);
        }
    }
    s->touchMode = touchMode;
    YesNoPrompt_Reset(s->yesNoPrompt);
    ClearWindowTilemapAndCopyToVram(&s->windows[1]);
    ov108_021E767C(s, 0);
    SafariAreaCustomizer_PrintWindowText(s, 0, 1, TRUE);
    SafariAreaCustomizer_DrawSelectFrame(s, 1, 0, FALSE);
    if (yes) {
        return 2;
    }
    return 0;
}
#pragma force_active reset

int SafariAreaCustomizer_ChangePage(SafariAreaCustomizer *s) {
    switch (s->subState) {
    case 0:
        SafariAreaCustomizer_StartPageChange(s);
        s->subState++;
        break;
    case 1:
        if (SafariAreaCustomizer_RunPageChange(s)) {
            s->subState++;
        }
        break;
    default:
        s->spriteBank ^= 1;
        s->subState = 0;
        return 0;
    }
    return 3;
}

int SafariAreaCustomizer_HandleMoveInput(SafariAreaCustomizer *s) {
    if (SafariAreaCustomizer_CheckInputModeSwitch(s)) {
        return 0;
    }
    if (s->touchMode == MENU_INPUT_STATE_BUTTONS) {
        return SafariAreaCustomizer_HandleMoveButtons(s);
    }
    return SafariAreaCustomizer_HandleMoveTouch(s);
}

int SafariAreaCustomizer_SwapAreas(SafariAreaCustomizer *s) {
    u32 bank = s->spriteBank * NUM_AREA_SLOTS;

    switch (s->subState) {
    case 0:
        ManagedSprite_SetPriority(s->areaSprites[s->selectedSlot + bank], 0);
        ManagedSprite_SetPriority(s->areaSprites[s->cursorPos + bank], 0);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        s->subState++;
        break;
    case 1:
        if (SafariAreaCustomizer_RunSwapAnim(s, 0)) {
            s->modified = TRUE;
            SafariZone_SwapAreasInSet(&s->areaSet, s->selectedSlot, s->cursorPos);
            SafariAreaCustomizerSpriteRes_SetAreaPicture(s->spriteRes, s->selectedSlot + bank, s->areaSet.areas[s->selectedSlot].area_no);
            SafariAreaCustomizer_LoadAreaPicture(s, s->selectedSlot, s->areaSet.areas[s->selectedSlot].area_no);
            SafariAreaCustomizerSpriteRes_SetAreaPicture(s->spriteRes, s->cursorPos + bank, s->areaSet.areas[s->cursorPos].area_no);
            SafariAreaCustomizer_LoadAreaPicture(s, s->cursorPos, s->areaSet.areas[s->cursorPos].area_no);
            s->subState++;
        }
        break;
    case 2:
        if (SafariAreaCustomizer_RunSwapAnim(s, 1)) {
            ManagedSprite_SetPriority(s->areaSprites[s->selectedSlot + bank], 2);
            ManagedSprite_SetPriority(s->areaSprites[s->cursorPos + bank], 2);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
            s->subState++;
        }
        break;
    default:
        s->selectedSlot = s->cursorPos;
        SafariAreaCustomizer_EndMove(s);
        s->subState = 0;
        s->nextState = 2;
        return 4;
    }
    return 3;
}

void SafariAreaCustomizer_ShowCursors(SafariAreaCustomizer *s) {
    switch (s->mode) {
    case 0:
        SafariAreaCustomizer_MoveCursor(s, 0, s->selectedSlot);
        break;
    case 1:
        SafariAreaCustomizer_MoveCursor(s, 1, s->cursorPos);
        SafariAreaCustomizer_MoveCursor(s, 2, s->selectedSlot);
        break;
    case 2:
        SafariAreaCustomizer_MoveCursor(s, 0, s->cursorPos);
        break;
    }
}

void ov108_021E6238(SafariAreaCustomizer *s) {
    SafariAreaCustomizer_ShowCursors(s);
}

int SafariAreaCustomizer_CheckInputModeSwitch(SafariAreaCustomizer *s) {
    if (s->touchMode == MENU_INPUT_STATE_TOUCH) {
        if (System_GetTouchHeld()) {
            return FALSE;
        }
        if (gSystem.heldKeys) {
            if (s->onSwitchToButtons != NULL) {
                s->onSwitchToButtons(s);
            }
            SafariAreaCustomizer_ShowCursors(s);
            s->touchMode = MENU_INPUT_STATE_BUTTONS;
            return FALSE;
        }
    } else {
        if (gSystem.heldKeys) {
            return FALSE;
        }
        if (System_GetTouchHeld()) {
            if (s->onSwitchToTouch != NULL) {
                s->onSwitchToTouch(s);
            }
            ov108_021E6238(s);
            s->touchMode = MENU_INPUT_STATE_TOUCH;
            return FALSE;
        }
    }
    return FALSE;
}

int SafariAreaCustomizer_HandleSlotButtons(SafariAreaCustomizer *s) {
    u32 keys = gSystem.newKeys;
    u8 old;
    u8 column;
    u8 row;

    if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        s->nextState = 6;
        return 4;
    }
    if (keys & PAD_BUTTON_A) {
        if (s->selectedSlot >= NUM_AREA_SLOTS) {
            PlaySE(SEQ_SE_DP_SELECT);
            s->nextState = 6;
            return 4;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        SafariAreaCustomizer_OpenSlotMenu(s);
        return 1;
    }
    if (!(keys & PAD_PLUS_KEY_MASK)) {
        return 0;
    }
    old = s->selectedSlot;
    column = old % 3;
    row = old / 3;
    if (row < 2) {
        if (keys & PAD_KEY_RIGHT) {
            column = (column + 1) % 3;
        } else if (keys & PAD_KEY_LEFT) {
            column = (column + 2) % 3;
        }
    }
    if (keys & PAD_KEY_UP) {
        row = (row + 2) % 3;
    } else if (keys & PAD_KEY_DOWN) {
        row = (row + 1) % 3;
    }
    s->selectedSlot = column + row * 3;
    if (row < 2 || (gSystem.newKeys & (PAD_KEY_UP | PAD_KEY_DOWN))) {
        PlaySE(SEQ_SE_DP_BUTTON3);
    }
    SafariAreaCustomizer_MoveCursor(s, 0, s->selectedSlot);
    SafariAreaCustomizer_PrintSlotInfo(s, s->selectedSlot);
    return 0;
}

int SafariAreaCustomizer_HandleSlotTouch(SafariAreaCustomizer *s) {
    s32 hit = TouchscreenHitbox_FindRectAtTouchNew(ov108_021EA760);

    if (hit == -1) {
        return 0;
    }
    if (hit == 6) {
        u16 transparent = 0xFFFE;
        if (!DoesPixelAtScreenXYMatchPtrVal(s->bgConfig, GF_BG_LYR_MAIN_3, gSystem.touchX, gSystem.touchY, &transparent)) {
            return 0;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        s->nextState = 6;
        return 4;
    }
    s->selectedSlot = hit;
    s->cursorPos = 0;
    SafariAreaCustomizer_MoveCursor(s, 0, s->selectedSlot);
    SafariAreaCustomizer_PrintSlotInfo(s, s->selectedSlot);
    PlaySE(SEQ_SE_DP_SELECT);
    SafariAreaCustomizer_OpenSlotMenu(s);
    return 1;
}

int SafariAreaCustomizer_ChooseArea(SafariAreaCustomizer *s) {
    u8 area;

    PlaySE(SEQ_SE_DP_SELECT);
    if (s->objectsUnlocked) {
        SafariAreaCustomizer_AskConfirmArea(s);
        return 1;
    }
    area = s->cursorPos + s->page * NUM_AREA_SLOTS;
    s->modified = TRUE;
    SafariZone_InitAreaInSet(&s->areaSet, s->selectedSlot, area);
    SafariAreaCustomizer_LoadAreaPicture(s, s->selectedSlot, s->areaSet.areas[s->selectedSlot].area_no);
    SafariAreaCustomizer_DrawSelectFrame(s, 1, 0, FALSE);
    return 2;
}

int SafariAreaCustomizer_HandleAreaButtons(SafariAreaCustomizer *s) {
    u8 old = s->cursorPos;
    u32 keys = gSystem.newKeys;
    u8 column;
    u8 row;
    u32 changePage;

    if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        return 2;
    }
    if (keys & PAD_BUTTON_A) {
        if (old >= NUM_AREA_SLOTS) {
            PlaySE(SEQ_SE_DP_SELECT);
            return 2;
        }
        if (old + s->page * NUM_AREA_SLOTS == s->areaSet.areas[s->selectedSlot].area_no) {
            return 0;
        }
        return SafariAreaCustomizer_ChooseArea(s);
    }
    if (!(keys & PAD_PLUS_KEY_MASK)) {
        return 0;
    }
    column = old % 3;
    row = old / 3;
    changePage = FALSE;
    if ((keys & PAD_KEY_RIGHT) && row < 2) {
        if (column == 2) {
            if (row == 0) {
                row ^= 1;
                column = (column + 1) % 3;
            } else if (row < 2 && s->page < 1) {
                changePage = TRUE;
                s->pageDir = 0;
                row ^= changePage;
                column = (column + 1) % 3;
            }
        } else {
            column = (column + 1) % 3;
        }
    } else if ((keys & PAD_KEY_LEFT) && row < 2) {
        if (column == 0) {
            if (row == 1) {
                row ^= 1;
                column = (column + 2) % 3;
            } else if (row < 2 && s->page != 0) {
                changePage = TRUE;
                s->pageDir = 1;
                row ^= changePage;
                column = (column + 2) % 3;
            }
        } else {
            column = (column + 2) % 3;
        }
    } else if (keys & PAD_KEY_UP) {
        row = (row + 2) % 3;
    } else if (keys & PAD_KEY_DOWN) {
        row = (row + 1) % 3;
    }
    s->cursorPos = column + row * 3;
    SafariAreaCustomizer_MoveCursor(s, 1, s->cursorPos);
    if (changePage) {
        PlaySE(SEQ_SE_DP_SELECT5);
        SafariAreaCustomizer_SetCursorAnim(s, 1, 0, FALSE);
        return 3;
    }
    if (old != s->cursorPos) {
        PlaySE(SEQ_SE_DP_BUTTON3);
    }
    SafariAreaCustomizer_PrintAreaNames(s);
    return 0;
}

int SafariAreaCustomizer_HandleAreaTouch(SafariAreaCustomizer *s) {
    s32 hit = TouchscreenHitbox_FindRectAtTouchNew(ov108_021EA7D0);

    if (hit == -1) {
        return 0;
    }
    switch (hit) {
    case 6: {
        u16 transparent = 0xFFFE;
        if (!DoesPixelAtScreenXYMatchPtrVal(s->bgConfig, GF_BG_LYR_MAIN_3, gSystem.touchX, gSystem.touchY, &transparent)) {
            return 0;
        }
        s->cursorPos = 6;
        PlaySE(SEQ_SE_DP_SELECT);
        return 2;
    }
    case 7:
        if (s->page == 0) {
            return 0;
        }
        s->pageDir = 1;
        s->cursorPos = 5;
        SafariAreaCustomizer_SetCursorAnim(s, 1, 0, FALSE);
        Sprite_SetAnimActiveFlag(s->sprites[4], TRUE);
        Sprite_ResetAnimCtrlState(s->sprites[4]);
        PlaySE(SEQ_SE_DP_SELECT5);
        return 3;
    case 8:
        if (s->page == 1) {
            return 0;
        }
        s->pageDir = 0;
        s->cursorPos = 0;
        SafariAreaCustomizer_SetCursorAnim(s, 1, 0, FALSE);
        Sprite_SetAnimActiveFlag(s->sprites[5], TRUE);
        Sprite_ResetAnimCtrlState(s->sprites[5]);
        PlaySE(SEQ_SE_DP_SELECT5);
        return 3;
    default:
        s->cursorPos = hit;
        SafariAreaCustomizer_MoveCursor(s, 1, s->cursorPos);
        SafariAreaCustomizer_PrintAreaNames(s);
        if (s->cursorPos + s->page * NUM_AREA_SLOTS == s->areaSet.areas[s->selectedSlot].area_no) {
            return 0;
        }
        return SafariAreaCustomizer_ChooseArea(s);
    }
}

void SafariAreaCustomizer_StartMove(SafariAreaCustomizer *s) {
    s->mode = 2;
    SafariAreaCustomizer_MoveCursor(s, 0, s->cursorPos);
    thunk_Sprite_SetPriority(s->sprites[0], 0);
    SafariAreaCustomizer_MoveCursor(s, 3, s->selectedSlot);
    SafariAreaCustomizer_PrintWindowText(s, 0, 2, TRUE);
}

void SafariAreaCustomizer_EndMove(SafariAreaCustomizer *s) {
    s->mode = 0;
    SafariAreaCustomizer_MoveCursor(s, 0, s->selectedSlot);
    thunk_Sprite_SetPriority(s->sprites[0], 2);
    SafariAreaCustomizer_SetCursorAnim(s, 3, 0, FALSE);
    SafariAreaCustomizer_PrintWindowText(s, 0, 0, TRUE);
}

int SafariAreaCustomizer_HandleMoveButtons(SafariAreaCustomizer *s) {
    u32 keys = gSystem.newKeys;
    u8 column;
    u8 row;

    if (keys & PAD_BUTTON_B) {
        SafariAreaCustomizer_EndMove(s);
        s->nextState = 2;
        PlaySE(SEQ_SE_DP_SELECT);
        return 4;
    }
    if (keys & PAD_BUTTON_A) {
        if (s->cursorPos >= NUM_AREA_SLOTS || s->selectedSlot == s->cursorPos) {
            SafariAreaCustomizer_EndMove(s);
            s->nextState = 2;
            PlaySE(SEQ_SE_DP_SELECT);
            return 4;
        }
        PlaySE(SEQ_SE_DP_SYU02);
        return 3;
    }
    if (!(keys & PAD_PLUS_KEY_MASK)) {
        return 0;
    }
    column = (s32)s->cursorPos % 3;
    row = (s32)s->cursorPos / 3;
    if (row < 2) {
        if (keys & PAD_KEY_RIGHT) {
            column = (column + 1) % 3;
        } else if (keys & PAD_KEY_LEFT) {
            column = (column + 2) % 3;
        }
    }
    if (keys & PAD_KEY_UP) {
        row = (row + 2) % 3;
    } else if (keys & PAD_KEY_DOWN) {
        row = (row + 1) % 3;
    }
    s->cursorPos = row * 3 + column;
    if (row < 2 || (gSystem.newKeys & (PAD_KEY_UP | PAD_KEY_DOWN))) {
        PlaySE(SEQ_SE_DP_BUTTON3);
    }
    SafariAreaCustomizer_MoveCursor(s, 0, s->cursorPos);
    return 0;
}

int SafariAreaCustomizer_HandleMoveTouch(SafariAreaCustomizer *s) {
    int hit = TouchscreenHitbox_FindRectAtTouchNew(ov108_021EA760);

    if (hit == -1) {
        return 0;
    }
    if (hit == 6) {
        u16 transparent = 0xFFFE;
        if (!DoesPixelAtScreenXYMatchPtrVal(s->bgConfig, GF_BG_LYR_MAIN_3, gSystem.touchX, gSystem.touchY, &transparent)) {
            return 0;
        }
        s->cursorPos = 6;
        SafariAreaCustomizer_EndMove(s);
        s->nextState = 2;
        PlaySE(SEQ_SE_DP_SELECT);
        return 4;
    }
    if (s->selectedSlot == hit) {
        SafariAreaCustomizer_EndMove(s);
        s->nextState = 2;
        PlaySE(SEQ_SE_DP_SELECT);
        return 4;
    }
    s->cursorPos = hit;
    SafariAreaCustomizer_MoveCursor(s, 0, s->cursorPos);
    PlaySE(SEQ_SE_DP_SYU02);
    return 3;
}

#pragma force_active on
void SafariAreaCustomizer_OpenSlotMenu(SafariAreaCustomizer *s) {
    TouchscreenListMenuHeader header;
    u16 x;
    u16 y;

    MI_CpuFill8(&header, 0, sizeof(header));
    header.template = _021EA720.menuTemplate;
    header.listMenuItems = s->areaMenuItems;
    header.bgConfig = s->bgConfig;
    header.numWindows = 3;
    y = ov108_021EA74A[s->selectedSlot * 2];
    x = ov108_021EA748[s->selectedSlot * 2];
    s->listMenu = TouchscreenListMenu_Create(s->listMenuSpawner, &header, s->touchMode, x, y, 0, 0);
    SafariAreaCustomizer_DrawSelectFrame(s, 1, s->selectedSlot, TRUE);
    SafariAreaCustomizer_DrawSelectFrame(s, 4, s->selectedSlot, TRUE);
}
#pragma force_active reset

void SafariAreaCustomizer_AskConfirmArea(SafariAreaCustomizer *s) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = s->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = 0x379;
    template.plttSlot = 13;
    template.x = 26;
    template.y = 16;
    template.ignoreTouchFlag = (u8)s->touchMode;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(s->yesNoPrompt, &template);
    SafariAreaCustomizer_DrawSelectFrame(s, 1, s->cursorPos, TRUE);
    SafariAreaCustomizer_LoadAreaPicture(s, s->selectedSlot, (u8)(s->cursorPos + NUM_AREA_SLOTS * s->page));
    ov108_021E767C(s, 1);
    SafariAreaCustomizer_PrintWindowText(s, 1, 3, TRUE);
}

void SafariAreaCustomizer_VBlankCB(void *cbArg) {
    SafariAreaCustomizer *s = cbArg;
    vu32 *dtcm;

    SafariAreaCustomizer_TransferOam(s);
    if (s->scrollPending) {
        BgSetPosTextAndCommit(s->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, s->mainBgY);
        BgSetPosTextAndCommit(s->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, s->mainBgY);
        BgSetPosTextAndCommit(s->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, s->subBgY);
        BgSetPosTextAndCommit(s->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, s->subBgY);
        BgSetPosTextAndCommit(s->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, s->subBgY);
        s->scrollPending = FALSE;
    }
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(s->bgConfig);
    dtcm = (vu32 *)0x027E0000;
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

void SafariAreaCustomizer_SetGfxBanks(void) {
    GraphicsBanks banks = ov108_021EA780;
    GfGfx_SetBanks(&banks);
}

void SafariAreaCustomizer_InitBgLayers(SafariAreaCustomizer *s) {
    GraphicsModes modes;
    SafariAreaCustomizerBgTemplates templates;
    int i;
    int count;
    BgTemplate *template;

    SafariAreaCustomizer_SetGfxBanks();
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    s->bgConfig = BgConfig_Alloc(s->heapId);
    modes = ov108_021EA738;
    SetBothScreensModesAndDisable(&modes);
    templates = ov108_021EA898;
    i = 0;
    count = 0;
    template = templates.templates;
    while (count < 8) {
        InitBgFromTemplate(s->bgConfig, (u8)i, template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(s->bgConfig, (u8)i);
        BG_ClearCharDataRange((u8)i, 0x40, 0, s->heapId);
        count++;
        i++;
        template ++;
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 28, 4);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 28, 4);
}

void SafariAreaCustomizer_FreeBgLayers(SafariAreaCustomizer *s) {
    int i;

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 0);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_NONE, 31, 0);
    for (i = 0; i < 8; i++) {
        FreeBgTilemapBuffer(s->bgConfig, (u8)i);
    }
    Heap_Free(s->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}

void SafariAreaCustomizer_LoadBgGraphics(SafariAreaCustomizer *s) {
    NARC *narc = NARC_New(NARC_a_1_6_6, s->heapId);

    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, 6, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_CHAR, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, 6, GF_BG_LYR_SUB_0, GF_BG_GFX_TYPE_CHAR, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, 1, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_CHAR, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, 5, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_PLTT, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, 0, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_PLTT, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, 10, GF_BG_LYR_MAIN_2, GF_BG_GFX_TYPE_SCRN, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, 7, GF_BG_LYR_MAIN_3, GF_BG_GFX_TYPE_SCRN, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, 4, GF_BG_LYR_SUB_2, GF_BG_GFX_TYPE_SCRN, 0, 0);
    BgConfig_LoadAssetFromOpenNarc(s->bgConfig, s->heapId, narc, NARC_a_1_6_6, (s->objectsUnlocked == FALSE) + 2, GF_BG_LYR_SUB_3, GF_BG_GFX_TYPE_SCRN, 0, 0);
    s->pageScreenFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 8, FALSE, &s->pageScreen, s->heapId);
    s->selectFrameScreenFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, FALSE, &s->selectFrameScreen, s->heapId);
    NARC_Delete(narc);
    SafariAreaCustomizer_InitSlotTilemaps(s);
    SafariAreaCustomizer_LoadAreaPicture(s, 0xFF, 0);
    ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_MAIN_0);
    ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_MAIN_2);
    ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_MAIN_3);
    ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_SUB_2);
    ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_SUB_3);
}

void SafariAreaCustomizer_FreeScreenData(SafariAreaCustomizer *s) {
    Heap_Free(s->pageScreenFile);
    Heap_Free(s->selectFrameScreenFile);
}

void SafariAreaCustomizer_CreateStrings(SafariAreaCustomizer *s) {
    int i;

    FontID_Alloc(4, s->heapId);
    s->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0429_bin, s->heapId);
    s->msgFormat = MessageFormat_New_Custom(1, 17, s->heapId);
    s->string = String_New(81, s->heapId);
    s->pageTemplate = NewString_ReadMsgData(s->msgData, 9);
    s->countTemplate = NewString_ReadMsgData(s->msgData, 15);
    for (i = 0; i < 5; i++) {
        s->labelStrings[i] = NewString_ReadMsgData(s->msgData, i + 4);
    }
    for (i = 0; i < 4; i++) {
        s->menuStrings[i] = NewString_ReadMsgData(s->msgData, i);
    }
}

void SafariAreaCustomizer_DeleteStrings(SafariAreaCustomizer *s) {
    int i;

    for (i = 0; i < 4; i++) {
        String_Delete(s->menuStrings[i]);
    }
    for (i = 0; i < 5; i++) {
        String_Delete(s->labelStrings[i]);
    }
    String_Delete(s->countTemplate);
    String_Delete(s->pageTemplate);
    String_Delete(s->string);
    MessageFormat_Delete(s->msgFormat);
    DestroyMsgData(s->msgData);
    FontID_Release(4);
}

#pragma force_active on
void SafariAreaCustomizer_AddWindows(SafariAreaCustomizer *s) {
    int i;
    u8 y;
    int windowId;
    u8 height;
    int baseTile;

    for (i = 0; i <= 4; i++) {
        AddWindow(s->bgConfig, &s->windows[i], &ov108_021EA7A8[i]);
        FillWindowPixelBuffer(&s->windows[i], 0);
    }
    baseTile = 0x3A6;
    for (i = 0; i < 5; i++) {
        windowId = i * 2 + 5;
        y = i * 4 + 1;
        height = (u8)(1 ^ (i / 4)) * 2 + 2;
        baseTile -= 22 * height;
        AddWindowParameterized(s->bgConfig, &s->windows[windowId], GF_BG_LYR_SUB_3, 1, y, 22, height, 5, baseTile);
        FillWindowPixelBuffer(&s->windows[windowId], (ov108_021EA724[i % 2] & 0xFF));
        baseTile -= 22 * height;
        AddWindowParameterized(s->bgConfig, &s->windows[windowId + 1], GF_BG_LYR_SUB_3, 26, y + (u8)(1 ^ (i / 4)), 5, 2, 5, baseTile);
        FillWindowPixelBuffer(&s->windows[windowId + 1], (ov108_021EA724[i % 2] & 0xFF));
        baseTile -= 10;
    }
    InitWindow(&s->textObjWindow);
    AddTextWindowTopLeftCorner(s->bgConfig, &s->textObjWindow, 6, 2, 0x2E1, 0);
    FillWindowPixelBufferText_AssumeTileSize32(&s->textObjWindow, 0);
    s->yesNoPrompt = YesNoPrompt_Create(s->heapId);
}
#pragma force_active reset

void SafariAreaCustomizer_RemoveWindows(SafariAreaCustomizer *s) {
    int i;

    YesNoPrompt_Destroy(s->yesNoPrompt);
    RemoveWindow(&s->textObjWindow);
    for (i = 0; i < 15; i++) {
        ClearWindowTilemapAndCopyToVram(&s->windows[i]);
        RemoveWindow(&s->windows[i]);
    }
}

void SafariAreaCustomizer_CreateMenuItems(SafariAreaCustomizer *s) {
    s->areaMenuItems = ListMenuItems_New(3, s->heapId);
    s->confirmMenuItems = ListMenuItems_New(2, s->heapId);
    ListMenuItems_AppendFromMsgData(s->areaMenuItems, s->msgData, 7, 0);
    ListMenuItems_AppendFromMsgData(s->areaMenuItems, s->msgData, 8, 1);
    ListMenuItems_AppendFromMsgData(s->areaMenuItems, s->msgData, 6, 2);
    ListMenuItems_AppendFromMsgData(s->confirmMenuItems, s->msgData, 5, 0);
    ListMenuItems_AppendFromMsgData(s->confirmMenuItems, s->msgData, 6, 1);
}

void SafariAreaCustomizer_DeleteMenuItems(SafariAreaCustomizer *s) {
    ListMenuItems_Delete(s->confirmMenuItems);
    ListMenuItems_Delete(s->areaMenuItems);
    s->areaMenuItems = NULL;
    s->confirmMenuItems = NULL;
}

void SafariAreaCustomizer_InitSprites(SafariAreaCustomizer *s) {
    SafariAreaCustomizer_InitSpriteSystem(s);
    s->spriteRes = SafariAreaCustomizerSpriteRes_New(s->heapId, 12, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
    SafariAreaCustomizer_CreateSpriteManager(s);
    s->listMenuSpawner = TouchscreenListMenuSpawner_Create(s->heapId, NULL);
    s->fontSystem = FontSystem_NewInit(1, s->heapId);
}

void SafariAreaCustomizer_FreeSprites(SafariAreaCustomizer *s) {
    FontSystem_Delete(s->fontSystem);
    TouchscreenListMenuSpawner_Destroy(s->listMenuSpawner);
    SafariAreaCustomizer_DestroySpriteManager(s);
    SafariAreaCustomizerSpriteRes_Delete(s->spriteRes);
    SafariAreaCustomizer_FreeSpriteSystem(s);
}

void SafariAreaCustomizer_CreateSprites(SafariAreaCustomizer *s) {
    int i = 0;

    for (; i < NUM_AREA_SLOTS; i++) {
        s->areaSprites[i] = SafariAreaCustomizerSpriteRes_CreateAreaSprite(s->spriteRes, (i % 3) * 72 + 56, (i / 3) * 72 + 56, 2, 128, i, s->areaSet.areas[i].area_no, NNS_G2D_VRAM_TYPE_2DMAIN);
    }
    for (i = 0; i < NUM_AREA_SLOTS; i++) {
        s->areaSprites[i + 6] = SafariAreaCustomizerSpriteRes_CreateAreaSprite(s->spriteRes, (i % 3) * 80 + 48, (i / 3) * 72 + 56, 3, 128, i + 6, i + 6 * s->page, NNS_G2D_VRAM_TYPE_2DMAIN);
    }
    s->sprites[0] = SpriteSystem_CreateSpriteFromResourceHeader(s->spriteSystem, s->spriteManager, &ov108_021EA7F8);
    s->sprites[1] = SpriteSystem_CreateSpriteFromResourceHeader(s->spriteSystem, s->spriteManager, &ov108_021EA7F8);
    s->sprites[3] = SpriteSystem_CreateSpriteFromResourceHeader(s->spriteSystem, s->spriteManager, &ov108_021EA7F8);
    s->sprites[2] = SpriteSystem_CreateSpriteFromResourceHeader(s->spriteSystem, s->spriteManager, &ov108_021EA820);
    s->sprites[4] = SpriteSystem_CreateSpriteFromResourceHeader(s->spriteSystem, s->spriteManager, &ov108_021EA848);
    s->sprites[5] = SpriteSystem_CreateSpriteFromResourceHeader(s->spriteSystem, s->spriteManager, &ov108_021EA870);
    thunk_Sprite_SetPriority(s->sprites[1], 3);
    thunk_Sprite_SetPriority(s->sprites[4], 3);
    thunk_Sprite_SetPriority(s->sprites[5], 3);
    thunk_Sprite_SetPriority(s->sprites[3], 0);
    thunk_Sprite_SetDrawPriority(s->sprites[3], 5);
    thunk_Sprite_SetDrawFlag(s->sprites[0], FALSE);
    thunk_Sprite_SetDrawFlag(s->sprites[1], FALSE);
    thunk_Sprite_SetDrawFlag(s->sprites[3], FALSE);
    thunk_Sprite_SetDrawFlag(s->sprites[2], FALSE);
    SafariAreaCustomizer_CreateCancelTextObj(s);
}

void SafariAreaCustomizer_DeleteSprites(SafariAreaCustomizer *s) {
    int i;

    SafariAreaCustomizer_DeleteCancelTextObj(s);
    thunk_Sprite_Delete(s->sprites[0]);
    thunk_Sprite_Delete(s->sprites[1]);
    thunk_Sprite_Delete(s->sprites[2]);
    thunk_Sprite_Delete(s->sprites[3]);
    thunk_Sprite_Delete(s->sprites[4]);
    thunk_Sprite_Delete(s->sprites[5]);
    for (i = 0; i < 12; i++) {
        SafariAreaCustomizer_DeleteAreaSprite(s->areaSprites[i]);
    }
}

// The "CANCEL" text object of the main screen.
void SafariAreaCustomizer_CreateCancelTextObj(SafariAreaCustomizer *s) {
    TextOBJTemplate template;
    u32 size;
    u32 width;
    u32 x;

    s->fontLayout = TextOBJLayout_New(&s->textObjWindow, s->heapId);
    size = TextOBJLayout_GetCharSize(s->fontLayout, NNS_G2D_VRAM_TYPE_2DMAIN);
    width = GetWindowWidth(&s->textObjWindow);
    x = (width * 8 - FontID_String_GetWidth(0, s->labelStrings[0], 0)) / 2;
    AddTextPrinterParameterizedWithColor(&s->textObjWindow, 0, s->labelStrings[0], x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(11, 6, 0), NULL);
    template.fontSystem = s->fontSystem;
    template.window = &s->textObjWindow;
    template.spriteList = SpriteManager_GetSpriteList(s->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(s->spriteManager, 0);
    template.unk_20 = 3;
    template.unk_24 = 128;
    template.x = 200;
    template.y = 172;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = s->heapId;
    sub_02021AC8(size, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, &s->charTransfer);
    template.offset = s->charTransfer.offset;
    template.sprite = NULL;
    s->textObj = TextOBJ_Create(&template, s->fontLayout);
    TextOBJ_SetSpritesDrawFlag(s->textObj, TRUE);
    TextOBJ_SetPaletteNum(s->textObj, 4);
}

void SafariAreaCustomizer_DeleteCancelTextObj(SafariAreaCustomizer *s) {
    TextOBJ_Destroy(s->textObj);
    sub_02021B5C(&s->charTransfer);
    s->textObj = NULL;
    TextOBJLayout_Delete(s->fontLayout);
}

#pragma force_active on
void ov108_021E767C(SafariAreaCustomizer *s, int value) {
    NNSG2dScreenData *screen;

    FillBgTilemapRect(s->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 19, 24, 5, 17);
    screen = s->pageScreen;
    CopyToBgTilemapRect(s->bgConfig, GF_BG_LYR_MAIN_0, 0, (u8)(21u - (u32)value * 2), 24, (u8)((u32)value * 2 + 3), screen->rawData, 0, (u8)((u32)value * 3), (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_MAIN_0);
}

void SafariAreaCustomizer_PrintWindowText(SafariAreaCustomizer *s, u8 windowId, u8 msg, BOOL unused) {
    String **strings;
    u32 x;
    u8 width;
    u8 height;
    u32 color;
    int y;

    width = GetWindowWidth(&s->windows[windowId]) * 8;
    height = GetWindowHeight(&s->windows[windowId]) * 8;
    color = MAKE_TEXT_COLOR(3, 1, 2);
    if (windowId != 2) {
        strings = s->menuStrings;
        x = 0;
        y = 4;
        if (windowId <= 1) {
            color = MAKE_TEXT_COLOR(13, 12, 14);
            FillWindowPixelBuffer(&s->windows[windowId], 14);
            FillWindowPixelRect(&s->windows[windowId], 6, 0, 0, width, 2);
            FillWindowPixelRect(&s->windows[windowId], 6, 0, height - 2, width, 2);
        } else {
            FillWindowPixelBuffer(&s->windows[windowId], 2);
        }
    } else {
        strings = s->labelStrings;
        x = (u8)((width - FontID_String_GetWidth(0, strings[msg], 0)) / 2);
        y = 4;
    }
    AddTextPrinterParameterizedWithColor(&s->windows[windowId], 0, strings[msg], x, y, TEXT_SPEED_INSTANT, color, NULL);
}
#pragma force_active reset

void SafariAreaCustomizer_PrintPageNumber(SafariAreaCustomizer *s) {
    u32 x;

    FillWindowPixelBuffer(&s->windows[3], 2);
    BufferIntegerAsString(s->msgFormat, 0, s->page + 1, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(s->msgFormat, s->string, s->pageTemplate);
    x = (GetWindowWidth(&s->windows[3]) * 8 - FontID_String_GetWidth(0, s->string, 0)) / 2;
    AddTextPrinterParameterizedWithColor(&s->windows[3], 0, s->string, x, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(3, 1, 2), NULL);
    ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_MAIN_3);
    Sprite_SetAnimActiveFlag(s->sprites[4], FALSE);
    if (s->page == 0) {
        Sprite_SetAnimCtrlSeq(s->sprites[4], 6);
    } else {
        Sprite_SetAnimCtrlSeq(s->sprites[4], 4);
    }
    Sprite_SetAnimActiveFlag(s->sprites[5], FALSE);
    if (s->page == 1) {
        Sprite_SetAnimCtrlSeq(s->sprites[5], 7);
    } else {
        Sprite_SetAnimCtrlSeq(s->sprites[5], 5);
    }
}

void SafariAreaCustomizer_SetCursorAnim(SafariAreaCustomizer *s, u32 index, u32 anim, BOOL draw) {
    if (draw == FALSE) {
        Sprite_SetDrawFlag(s->sprites[index], FALSE);
    } else {
        Sprite_SetDrawFlag(s->sprites[index], TRUE);
        Sprite_SetAnimCtrlSeq(s->sprites[index], anim);
    }
}

// Moves cursor sprite `index` to grid position `pos` (6 = the CANCEL button).
void SafariAreaCustomizer_MoveCursor(SafariAreaCustomizer *s, u32 index, u32 pos) {
    s16 x;
    s16 y;
    int anim = 0;

    if (pos >= 6) {
        x = 224;
        y = 180;
        anim = 1;
    } else if (index != 1) {
        x = ((s32)pos % 3) * 72 + 56;
        y = ((s32)pos / 3) * 72 + 56;
        if (index == 2) {
            y += 192;
        } else if (index == 3) {
            anim = 2;
        } else if (index == 0 && s->mode == 2) {
            anim = 3;
        }
    } else {
        x = ((s32)pos % 3) * 80 + 48;
        y = ((s32)pos / 3) * 72 + 56;
    }
    Sprite_SetPositionXY(s->sprites[index], x, y);
    SafariAreaCustomizer_SetCursorAnim(s, index, anim, TRUE);
}

#pragma force_active on
void SafariAreaCustomizer_DrawSelectFrame(SafariAreaCustomizer *s, u8 layer, int pos, BOOL show) {
    struct {
        u8 x;
        u8 y;
    } frame;

    if (!show) {
        BgClearTilemapBufferAndCommit(s->bgConfig, layer);
        ScheduleBgTilemapBufferTransfer(s->bgConfig, layer);
        return;
    }
    {
        NNSG2dScreenData *screen = s->selectFrameScreen;
        CopyToBgTilemapRect(s->bgConfig, layer, 0, (u8)(24 * s->mode), 32, 24, screen->rawData, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3);
    }
    if (s->mode == 0) {
        frame.x = (u8)((u32)(pos % 3) * 9 + 3);
        frame.y = (u8)((u32)(pos / 3) * 9 + 3);
        if (layer == GF_BG_LYR_SUB_0) {
            NNSG2dScreenData *screen = s->selectFrameScreen;
            CopyToBgTilemapRect(s->bgConfig, layer, 0, 21, 32, 3, screen->rawData, 0, 24, screen->screenWidth >> 3, screen->screenHeight >> 3);
        }
    } else {
        frame.x = (u8)((u32)(pos % 3) * 10 + 2);
        frame.y = (u8)((u32)(pos / 3) * 9 + 27);
    }
    FillBgTilemapRect(s->bgConfig, layer, 0, frame.x, frame.y, 8, 8, 17);
    ScheduleBgTilemapBufferTransfer(s->bgConfig, layer);
}
#pragma force_active reset

void SafariAreaCustomizer_InitSlotTilemaps(SafariAreaCustomizer *s) {
    int slot;
    int row;
    int column;
    int tile = 1;
    int x;
    u8 y;

    slot = 0;
    while (slot < NUM_AREA_SLOTS) {
        row = 0;
        x = (u8)((slot / 3) * 9 + 3);
        y = (slot % 3) * 9 + 3;
        while (row < 8) {
            u8 xPos = x;
            column = 0;
            while (column < 8) {
                FillBgTilemapRect(s->bgConfig, GF_BG_LYR_SUB_1, tile++, (u8)(y + column), xPos, 1, 1, 17);
                column++;
            }
            x++;
            row++;
        }
        slot++;
    }
    ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_SUB_1);
}

void SafariAreaCustomizer_LoadAreaPictureFromNarc(SafariAreaCustomizer *s, NARC *narc, u32 slot, int area) {
    NNSG2dCharacterData *charData;
    void *charFile = GfGfxLoader_GetCharDataFromOpenNarc(narc, area + 18, FALSE, &charData, s->heapId);

    BG_LoadCharTilesData(s->bgConfig, GF_BG_LYR_SUB_1, charData->pRawData, 0x1000, slot * 0x40 + 1);
    Heap_Free(charFile);
}

// Loads the picture of `area` into slot `slot`, or of every slot when `slot` >= 6.
void SafariAreaCustomizer_LoadAreaPicture(SafariAreaCustomizer *s, u32 slot, int area) {
    NARC *narc = NARC_New(NARC_a_1_6_6, s->heapId);

    if (slot >= NUM_AREA_SLOTS) {
        int i;
        for (i = 0; i < NUM_AREA_SLOTS; i++) {
            SafariAreaCustomizer_LoadAreaPictureFromNarc(s, narc, (u8)i, s->areaSet.areas[i].area_no);
        }
    } else {
        SafariAreaCustomizer_LoadAreaPictureFromNarc(s, narc, slot, area);
    }
    NARC_Delete(narc);
}

void SafariAreaCustomizer_InitScreen(SafariAreaCustomizer *s) {
    SafariAreaCustomizer_PrintObjectTypeLabels(s);
    SafariAreaCustomizer_PrintSlotInfo(s, s->selectedSlot);
    ov108_021E767C(s, 0);
    SafariAreaCustomizer_PrintWindowText(s, 0, 0, TRUE);
    if (s->touchMode == MENU_INPUT_STATE_BUTTONS) {
        SafariAreaCustomizer_ShowCursors(s);
    } else {
        ov108_021E6238(s);
    }
    s->mainBgY = 0;
    s->subBgY = -192;
    s->scrollPending = TRUE;
}

void SafariAreaCustomizer_PrintObjectTypeLabels(SafariAreaCustomizer *s) {
    int i;

    if (s->objectsUnlocked) {
        for (i = 0; i < 5; i++) {
            Window *window = &s->windows[5 + i * 2];
            ReadMsgDataIntoString(s->msgData, i + 10, s->string);
            AddTextPrinterParameterizedWithColor(window, 0, s->string, 0, 0, TEXT_SPEED_INSTANT, ov108_021EA724[i % 2], NULL);
            CopyWindowToVram(window);
        }
    }
}

#pragma force_active on
// Prints the name of the area in `slot` and, once objects are unlocked, its object counts per type.
void SafariAreaCustomizer_PrintSlotInfo(SafariAreaCustomizer *s, u32 slot) {
    int i;
    SAFARIZONE_AREA *area;
    u8 x;
    u8 counts[5];
    SafariObjectConfig config;

    if (slot >= NUM_AREA_SLOTS) {
        if (s->objectsUnlocked) {
            for (i = 0; i < 5; i++) {
                FillWindowPixelBuffer(&s->windows[i * 2 + 6], ov108_021EA724[i % 2]);
                ScheduleWindowCopyToVram(&s->windows[i * 2 + 6]);
            }
        }
        FillWindowPixelBuffer(&s->windows[4], 12);
        ScheduleWindowCopyToVram(&s->windows[4]);
        return;
    }
    area = &s->areaSet.areas[slot];
    {
        Window *window = &s->windows[4];
        ReadMsgDataIntoString(s->msgData, area->area_no + 16, s->string);
        x = (GetWindowWidth(window) * 8 - FontID_String_GetWidth(0, s->string, 0)) / 2;
        FillWindowPixelBuffer(window, 12);
        AddTextPrinterParameterizedWithColor(window, 0, s->string, x, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(8, 11, 12), NULL);
        ScheduleWindowCopyToVram(window);
    }
    if (s->objectsUnlocked) {
        MI_CpuFill8(counts, 0, 5);
        for (i = 0; i < area->active_object_count; i++) {
            GetSafariObjectConfig(&config, area->objects[i].id, 2);
            if (config.objectType == 0) {
                counts[4]++;
            } else {
                counts[config.objectType - 1]++;
            }
        }
        for (i = 0; i < 5; i++) {
            Window *window = &s->windows[i * 2 + 6];
            BufferIntegerAsString(s->msgFormat, 0, counts[i], 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            StringExpandPlaceholders(s->msgFormat, s->string, s->countTemplate);
            FillWindowPixelBuffer(window, ov108_021EA724[i % 2]);
            AddTextPrinterParameterizedWithColor(window, 0, s->string, 0, 0, TEXT_SPEED_NOTRANSFER, ov108_021EA724[i % 2], NULL);
            ScheduleWindowCopyToVram(window);
        }
    }
}
#pragma force_active reset

void SafariAreaCustomizer_PrintAreaNames(SafariAreaCustomizer *s) {
    u8 x;
    SAFARIZONE_AREA *area;
    Window *window = &s->windows[4];

    FillWindowPixelBuffer(window, 12);
    area = &s->areaSet.areas[s->selectedSlot];
    ReadMsgDataIntoString(s->msgData, area->area_no + 16, s->string);
    x = (96 - FontID_String_GetWidth(0, s->string, 0)) / 2;
    AddTextPrinterParameterizedWithColor(window, 0, s->string, x, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(8, 11, 12), NULL);
    if (s->cursorPos < NUM_AREA_SLOTS) {
        ReadMsgDataIntoString(s->msgData, s->cursorPos + 16 + s->page * NUM_AREA_SLOTS, s->string);
        x = (96 - FontID_String_GetWidth(0, s->string, 0)) / 2;
        AddTextPrinterParameterizedWithColor(window, 0, s->string, x + 144, 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(8, 11, 12), NULL);
    }
    ScheduleWindowCopyToVram(window);
}

void SafariAreaCustomizer_StartPageScroll(SafariAreaCustomizer *s) {
    int i;
    int bank;
    u32 ctrl;

    if (s->mode == 0) {
        FillBgTilemapRect(s->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 21, 32, 3, 17);
        ScheduleBgTilemapBufferTransfer(s->bgConfig, GF_BG_LYR_MAIN_1);
        s->subBgY = -192;
        s->scrollPending = TRUE;
        s->cursorPos = 0;
        SafariAreaCustomizer_MoveCursor(s, 1, s->cursorPos);
        SafariAreaCustomizer_MoveCursor(s, 2, s->selectedSlot);
        SafariAreaCustomizer_PrintPageNumber(s);
    } else {
        SafariAreaCustomizer_DrawSelectFrame(s, 1, 0xFF, FALSE);
        SafariAreaCustomizer_SetCursorAnim(s, 1, 0, FALSE);
        i = 0;
        bank = s->spriteBank * NUM_AREA_SLOTS;
        do {
            ManagedSprite_SetPositionXY(s->areaSprites[bank + i], 56 + 72 * (i % 3), -136 + 72 * (i / 3));
            ManagedSprite_SetPriority(s->areaSprites[bank + i], 2);
            SafariAreaCustomizerSpriteRes_SetAreaPicture(s->spriteRes, i + bank, s->areaSet.areas[i].area_no);
            i++;
        } while (i < NUM_AREA_SLOTS);
    }
    s->animFrame = 0;
    FillWindowPixelBuffer(&s->windows[4], 12);
    ScheduleWindowCopyToVram(&s->windows[4]);
    reg_G2S_DB_WIN0H = 0xF0;
    reg_G2S_DB_WIN0V = 0x10;
    ctrl = (reg_G2S_DB_WININ & ~0x3F) | 0xF;
    ctrl |= 0x20;
    reg_G2S_DB_WININ = ctrl;
    ctrl = (reg_G2S_DB_WINOUT & ~0x3F) | 0x1F;
    ctrl |= 0x20;
    reg_G2S_DB_WINOUT = ctrl;
    reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & ~(REG_GXS_DB_DISPCNT_W0_MASK | REG_GXS_DB_DISPCNT_W1_MASK | REG_GXS_DB_DISPCNT_OW_MASK)) | REG_GXS_DB_DISPCNT_W0_MASK;
}

// Scrolls the area pages between the set view and the page view, 8 frames.
BOOL SafariAreaCustomizer_RunPageScroll(SafariAreaCustomizer *s) {
    int i;
    int delta;

    if (s->mode == 0) {
        s->mainBgY += 24;
        s->subBgY += 24;
        delta = -24;
    } else {
        s->mainBgY -= 24;
        s->subBgY -= 24;
        delta = 24;
    }
    s->scrollPending = TRUE;
    for (i = 0; i < NUM_AREA_SLOTS; i++) {
        ManagedSprite_OffsetPositionXY(s->areaSprites[s->spriteBank * NUM_AREA_SLOTS + i], 0, delta);
    }
    Sprite_OffsetPositionXY(s->sprites[0], 0, delta);
    Sprite_OffsetPositionXY(s->sprites[2], 0, delta);
    s->animFrame++;
    return s->animFrame >= 8;
}

void SafariAreaCustomizer_StartPageChange(SafariAreaCustomizer *s) {
    int bank;
    int dx;
    int i;

    if (s->pageDir == 0) {
        s->page++;
        dx = 256;
    } else {
        s->page--;
        dx = -256;
    }
    bank = s->spriteBank * NUM_AREA_SLOTS;
    for (i = 0; i < NUM_AREA_SLOTS; i++) {
        ManagedSprite_SetPositionXY(s->areaSprites[bank + i], 48 + 80 * (i % 3) + dx, 56 + 72 * (i / 3));
        SafariAreaCustomizerSpriteRes_SetAreaPicture(s->spriteRes, (u8)(i + bank), (u8)(s->page * NUM_AREA_SLOTS + i));
        ManagedSprite_SetPriority(s->areaSprites[bank + i], 3);
    }
    s->animFrame = 0;
}

#pragma force_active on
// Slides the area page by 32 pixels per frame, 8 frames.
BOOL SafariAreaCustomizer_RunPageChange(SafariAreaCustomizer *s) {
    int i;
    int dx = s->pageDir == 0 ? -32 : 32;

    i = 0;
    do {
        ManagedSprite_OffsetPositionXY(s->areaSprites[i], (s16)dx, 0);
        i++;
    } while (i < 12);
    s->animFrame++;
    if (s->animFrame >= 8) {
        s->animFrame = 0;
        SafariAreaCustomizer_MoveCursor(s, 1, s->cursorPos);
        SafariAreaCustomizer_PrintAreaNames(s);
        SafariAreaCustomizer_PrintPageNumber(s);
        return TRUE;
    }
    return FALSE;
}

// Moves the two areas being swapped out of (dir 0) or back into (dir 1) their slots, 8 frames.
BOOL SafariAreaCustomizer_RunSwapAnim(SafariAreaCustomizer *s, int dir) {
    u8 sprites[2];
    u8 rows[2];
    int bank = NUM_AREA_SLOTS * s->spriteBank;
    int i;

    sprites[0] = s->selectedSlot + bank;
    sprites[1] = s->cursorPos + bank;
    rows[0] = s->selectedSlot / 3;
    rows[1] = s->cursorPos / 3;
    for (i = 0; i < 2; i++) {
        ManagedSprite_OffsetPositionXY(s->areaSprites[sprites[i]], 0, 12 * _021EA720.swapDir[dir][rows[i]]);
    }
    s->animFrame++;
    if (s->animFrame >= 8) {
        s->animFrame = 0;
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

void SafariAreaCustomizer_InitSpriteSystem(SafariAreaCustomizer *s) {
    GF_CreateVramTransferManager(32, s->heapId);
    s->spriteSystem = SpriteSystem_Alloc(s->heapId);
    SpriteSystem_Init(s->spriteSystem, &ov108_021EA9A0, &ov108_021EA98C, 4);
    thunk_ClearMainOAM(s->heapId);
    thunk_ClearSubOAM(s->heapId);
}

void SafariAreaCustomizer_FreeSpriteSystem(SafariAreaCustomizer *s) {
    SpriteSystem_Free(s->spriteSystem);
    s->spriteSystem = NULL;
    GF_DestroyVramTransferManager();
    thunk_ClearMainOAM(s->heapId);
    thunk_ClearSubOAM(s->heapId);
}

void SafariAreaCustomizer_CreateSpriteManager(SafariAreaCustomizer *s) {
    if (s->spriteSystem != NULL) {
        s->spriteManager = SpriteManager_New(s->spriteSystem);
        SpriteSystem_InitSprites(s->spriteSystem, s->spriteManager, 140);
        sub_0200D2A4(s->spriteSystem, s->spriteManager, &ov108_021EA97C, 2, 0);
        SafariAreaCustomizerSpriteRes_SetSpriteList(s->spriteRes, SpriteManager_GetSpriteList(s->spriteManager));
    }
}

void SafariAreaCustomizer_DestroySpriteManager(SafariAreaCustomizer *s) {
    if (s->spriteManager != NULL) {
        SpriteSystem_DestroySpriteManager(s->spriteSystem, s->spriteManager);
        s->spriteManager = NULL;
    }
}

void SafariAreaCustomizer_DrawSprites(SafariAreaCustomizer *s) {
    if (s->spriteRes != NULL) {
        SafariAreaCustomizerSpriteRes_DrawSprites(s->spriteRes);
    }
    if (s->spriteManager != NULL) {
        SpriteSystem_DrawSprites(s->spriteManager);
    }
}

void SafariAreaCustomizer_TransferOam(SafariAreaCustomizer *s) {
    if (s->spriteSystem != NULL) {
        SpriteSystem_TransferOam();
    }
}

SafariAreaCustomizerSpriteRes *SafariAreaCustomizerSpriteRes_New(enum HeapID heapId, u16 maxSprites, u16 vram, int transferMode) {
    SafariAreaCustomizerSpriteRes *res = Heap_Alloc(heapId, sizeof(SafariAreaCustomizerSpriteRes));

    MI_CpuFill8(res, 0, sizeof(SafariAreaCustomizerSpriteRes));
    res->heapId = heapId;
    res->unk_0A = 4;
    res->maxSprites = maxSprites;
    res->vram = vram;
    res->transferMode = transferMode;
    SafariAreaCustomizerSpriteRes_Init(res);
    return res;
}

void SafariAreaCustomizerSpriteRes_Delete(SafariAreaCustomizerSpriteRes *res) {
    SafariAreaCustomizerSpriteRes_Free(res);
    MI_CpuFill8(res, 0, sizeof(SafariAreaCustomizerSpriteRes));
    Heap_Free(res);
}

void SafariAreaCustomizerSpriteRes_SetAreaPicture(SafariAreaCustomizerSpriteRes *res, u8 index, u8 area) {
    SpriteResource *charRes = res->resObjList[GF_GFX_RES_TYPE_CHAR]->obj[index];

    ReplaceCharResObjFromNarc(res->resMan[GF_GFX_RES_TYPE_CHAR], charRes, NARC_a_1_6_6, area + 18, FALSE, res->heapId);
    SpriteTransfer_ReplaceCharData(charRes);
}

void SafariAreaCustomizerSpriteRes_DrawSprites(SafariAreaCustomizerSpriteRes *res) {
    if (res->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(res->spriteList);
    }
}

void SafariAreaCustomizerSpriteRes_SetSpriteList(SafariAreaCustomizerSpriteRes *res, SpriteList *spriteList) {
    res->externalSpriteList = spriteList;
}

#pragma force_active on
ManagedSprite *SafariAreaCustomizerSpriteRes_CreateAreaSprite(SafariAreaCustomizerSpriteRes *res, u16 x, u16 y, u8 transfer, u8 priority, u8 index, u8 area, int vram) {
    ManagedSprite *sprite;
    SpriteTemplate template;

    sprite = Heap_Alloc(res->heapId, sizeof(ManagedSprite));
    MI_CpuFill8(sprite, 0, sizeof(ManagedSprite));
    sprite->spriteResourceHeaderList = Heap_Alloc(res->heapId, sizeof(SpriteResourceHeaderList));
    sprite->spriteResourceHeaderList->headers = Heap_Alloc(res->heapId, sizeof(SpriteResourcesHeader));
    sprite->spriteResourcesHeader = sprite->spriteResourceHeaderList->headers;
    CreateSpriteResourcesHeader(sprite->spriteResourcesHeader, 0xE000 + index, 0xE000, 0xE000, 0xE000, -1, -1, 0, transfer, res->resMan[0], res->resMan[1], res->resMan[2], res->resMan[3], NULL, NULL);
    if (res->externalSpriteList != NULL) {
        template.spriteList = res->externalSpriteList;
    } else {
        template.spriteList = res->spriteList;
    }
    template.header = sprite->spriteResourcesHeader;
    template.position.x = FX32_CONST(x);
    template.position.y = FX32_CONST(y);
    template.position.z = 0;
    if (vram == NNS_G2D_VRAM_TYPE_2DSUB) {
        template.position.y += FX32_CONST(192);
    }
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = priority;
    template.whichScreen = (NNS_G2D_VRAM_TYPE)vram;
    template.heapID = res->heapId;
    SafariAreaCustomizerSpriteRes_SetAreaPicture(res, index, area);
    sprite->sprite = Sprite_CreateAffine(&template);
    GF_ASSERT(sprite->sprite != NULL);
    return sprite;
}
#pragma force_active reset

void SafariAreaCustomizer_DeleteAreaSprite(ManagedSprite *sprite) {
    Sprite_DeleteAndFreeResources(sprite);
}

void SafariAreaCustomizerSpriteRes_Init(SafariAreaCustomizerSpriteRes *res) {
    SafariAreaCustomizerResCounts counts = ov108_021EA978;
    u32 i;

    res->spriteList = G2dRenderer_Init(res->maxSprites, &res->renderer, res->heapId);
    for (i = 0; i < 4; i++) {
        u32 j;
        res->resMan[i] = Create2DGfxResObjMan(counts.counts[i], (GfGfxResType)i, res->heapId);
        res->resObjList[i] = Create2DGfxResObjList(counts.counts[i], res->heapId);
        for (j = 0; j < res->resObjList[i]->max; j++) {
            res->resObjList[i]->obj[j] = NULL;
        }
    }
    SafariAreaCustomizerSpriteRes_LoadResources(res);
}

void SafariAreaCustomizerSpriteRes_Free(SafariAreaCustomizerSpriteRes *res) {
    u32 i;

    SpriteList_Delete(res->spriteList);
    SpriteTransfer_DeleteAllCharTransferTasks(res->resObjList[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeleteAllPlttTransferTasks(res->resObjList[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        Delete2DGfxResObjList(res->resObjList[i]);
        Destroy2DGfxResObjMan(res->resMan[i]);
    }
}

#pragma force_active on
void SafariAreaCustomizerSpriteRes_LoadResources(SafariAreaCustomizerSpriteRes *res) {
    int i;
    GF_2DGfxResObjList *list = res->resObjList[GF_GFX_RES_TYPE_CHAR];

    for (i = 0; i < 12; i++) {
        list->obj[i] = AddCharResObjFromNarc(res->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_1_6_6, i + 18, FALSE, i + 0xE000, res->vram, res->heapId);
        if (list->obj[i] == NULL) {
            GF_AssertFail();
        }
        switch (res->transferMode) {
        case 0:
            SpriteTransfer_CreateCharTransferTask_AllocAtEnd(list->obj[i]);
            break;
        case 1:
            SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(list->obj[i]);
            break;
        case 2:
        default:
            SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW(list->obj[i]);
            break;
        }
    }
    for (i = 0; i < 2; i++) {
        GF_2DGfxResObjList *cellList = res->resObjList[GF_GFX_RES_TYPE_CELL + i];
        cellList->obj[0] = AddCellOrAnimResObjFromNarc(res->resMan[GF_GFX_RES_TYPE_CELL + i], NARC_a_1_6_6, i + 16, FALSE, 0xE000, (GfGfxResType)(GF_GFX_RES_TYPE_CELL + i), res->heapId);
        if (cellList->obj[0] == NULL) {
            GF_AssertFail();
        }
    }
    list = res->resObjList[GF_GFX_RES_TYPE_PLTT];
    list->obj[0] = AddPlttResObjFromNarc(res->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_1_6_6, 15, FALSE, 0xE000, res->vram, 4, res->heapId);
    if (list->obj[0] == NULL) {
        GF_AssertFail();
    }
    SpriteTransfer_CreatePlttTransferTask(list->obj[0]);
}
#pragma force_active reset
