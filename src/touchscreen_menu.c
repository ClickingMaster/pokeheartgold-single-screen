#include <nitro/gx/gx_vramcnt.h>

#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/items.h"
#include "constants/player_avatar.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/sprites.h"

#include "field/field_control.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0196.h"
#include "overlay_2/overlay_02_gear_phone.h"

#include "bag.h"
#include "bg_window.h"
#include "bug_contest_internal.h"
#include "catching_show.h"
#include "dsprot.h"
#include "field_system.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "map_object.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sprite_transfer.h"
#include "start_menu.h"
#include "sys_flags.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "task.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02035900.h"
#include "field_interaction.h"
#include "unk_02056D7C.h"
#include "communication_club_link.h"
#include "frontier_field_support.h"
#include "unk_0206D494.h"
#include "unk_02092BE8.h"

// Field bottom-screen app 0, the touchscreen start menu (ScrCmd_TouchscreenMenuShow, AppEntry 0 of
// field_overlay_app_dispatch.c): the start-menu icons (msg 196), the running shoes and action buttons,
// the two registered-item buttons, the Safari / Park / Sport ball counters and the Pokegear caller name.
// Touching an icon writes *lastTouchMenuInput (1 = MENU, menu index + 2), which start_menu.c and
// field_control.c consume.

FS_EXTERN_OVERLAY(ds_protect);

#define TOUCHSCREEN_MENU_NUM_ICONS     7
#define TOUCHSCREEN_MENU_NUM_RESOURCES 11
#define TOUCHSCREEN_MENU_ICON_NONE     13
#define TOUCHSCREEN_MENU_NO_HIGHLIGHT  -1

// sprites[]: 0-6 menu icons, 7/8 registered items, 9/10 their buttons, 11/13 running shoes,
// 12 action button, 14 MENU, 15 ball item of the Safari Zone / Bug Contest / Pal Park layouts.
#define TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON 6
#define TOUCHSCREEN_MENU_SPRITE_ITEM_1          7
#define TOUCHSCREEN_MENU_SPRITE_ITEM_2          8
#define TOUCHSCREEN_MENU_SPRITE_ITEM_1_BUTTON   9
#define TOUCHSCREEN_MENU_SPRITE_ITEM_2_BUTTON   10
#define TOUCHSCREEN_MENU_SPRITE_SHOES           11
#define TOUCHSCREEN_MENU_SPRITE_ACTION          12
#define TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL     13
#define TOUCHSCREEN_MENU_SPRITE_MENU            14
#define TOUCHSCREEN_MENU_SPRITE_BALL            15

enum TouchscreenMenuLayout {
    TOUCHSCREEN_MENU_LAYOUT_NORMAL,
    TOUCHSCREEN_MENU_LAYOUT_SAFARI,
    TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST,
    TOUCHSCREEN_MENU_LAYOUT_PAL_PARK,
    TOUCHSCREEN_MENU_LAYOUT_4, // bottomScreenType == 3
    TOUCHSCREEN_MENU_LAYOUT_5, // mapLoadType == 3
    TOUCHSCREEN_MENU_LAYOUT_BATTLE_TOWER_PARTNER_ROOM,
};

typedef struct TouchscreenMenuIcon {
    u8 shown;
    String *label;
} TouchscreenMenuIcon;

// Name of the caller while the Pokegear rings.
typedef struct TouchscreenMenuPhoneRing {
    BgConfig *bgConfig;
    GearPhoneRingManager *phoneRingManager;
    Sprite *pokegearSprite;
    Window callerWindow;
    BOOL shown;
} TouchscreenMenuPhoneRing;

typedef struct TouchscreenMenuData {
    u32 bgVariant; // always 0
    BgConfig *bgConfig;
    SysTask *task;
    u16 *lastTouchMenuInput;
    FieldSystem *fieldSystem;
    int selection;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    SpriteResource *resObjs[TOUCHSCREEN_MENU_NUM_RESOURCES][4];
    SpriteResourcesHeader headers[TOUCHSCREEN_MENU_NUM_RESOURCES];
    Sprite *sprites[16];
    Window actionWindow;
    Window menuWindow;
    Window iconWindows[8];
    TouchscreenMenuIcon icons[TOUCHSCREEN_MENU_NUM_ICONS];
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *actionStrings[5];
    String *menuString;
    String *unk_4C8;
    u16 iconPalettes[32]; // 0-15 normal, 16-31 highlighted
    u8 runningShoesShown;
    int actionType;
    u8 iconShown[TOUCHSCREEN_MENU_NUM_ICONS];
    u32 buttonsActive : 1;
    u32 layout : 4;
    u32 selectedIconAnim : 1;
    u32 registeredItemAnim : 2;
    u32 unused : 24;
    TouchscreenMenuPhoneRing phoneRing;
} TouchscreenMenuData; // size: 0x540

typedef struct TouchscreenMenuIconInfo {
    u16 charFileId; // 0xFFFF: the Bug Contest mon icon
    u16 msgId : 15;
    u16 enabled : 1;
} TouchscreenMenuIconInfo;

// Sprite, animation and label window of an icon that appears while the menu is shown.
typedef struct TouchscreenMenuIconAnim {
    u16 sprite;
    u16 anim;
    u8 window;
} TouchscreenMenuIconAnim;

typedef struct TouchscreenMenuPos {
    u16 x;
    u16 y;
} TouchscreenMenuPos;

typedef struct TouchscreenMenuWindowPos {
    u32 x;
    u32 y;
} TouchscreenMenuWindowPos;

// The three NARC 14 file tables interleave as rows of {palette, char, screen}; each column is
// addressed through its own label, so one entry of each is {fileId, the other two columns}.
typedef struct TouchscreenMenuBgFile {
    u32 fileId;
    u32 otherColumns[2];
} TouchscreenMenuBgFile;

// Defined in field_overlay_app_dispatch.c, which has no public header.
BOOL ov01_021F6BB0(int spriteId);
BOOL ov01_021F6BD0(int scriptId);

// Defined in link_lobby_2.c / link_lobby_4.c and communication_player_movement.c; no header declares them.
BOOL sub_02058258(void);
BOOL sub_02058AA0(void);
BOOL sub_02056EE0(void);

extern const u32 sTouchscreenMenuTextColors[];
extern const TouchscreenHitbox sTouchscreenMenuButtonHitboxes[];
extern const u32 sTouchscreenMenuActionMsgIds[];
extern const TouchscreenMenuBgFile sTouchscreenMenuBgPalettes[];
extern const TouchscreenMenuBgFile sTouchscreenMenuBgChars[];
extern const TouchscreenMenuBgFile sTouchscreenMenuBgScreens[];
extern const TouchscreenMenuIconAnim sTouchscreenMenuIconAnims[];
extern const u32 sTouchscreenMenuCellIds[];
extern const TouchscreenHitbox sTouchscreenMenuIconHitboxes[];
extern const TouchscreenMenuIconInfo sTouchscreenMenuIcons[];
extern const u8 sTouchscreenMenuLayouts[][8];
extern const BgTemplate sTouchscreenMenuBgTemplate_Sub0;
extern const BgTemplate sTouchscreenMenuBgTemplate_Sub1;
extern const TouchscreenMenuPos sTouchscreenMenuIconPositions[];
extern const TouchscreenMenuPos sTouchscreenMenuButtonPositions[];
extern const TouchscreenMenuWindowPos sTouchscreenMenuIconWindowPositions[];
extern const u8 sTouchscreenMenuIconNeighbors[][4][3];
extern const u8 sTouchscreenMenuActionTypeToString[];

SysTask *TouchscreenMenu_Create(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args);
void TouchscreenMenu_Destroy(BgConfig *bgConfig, SysTask *task);
BOOL ov27_0225A2C8(void *unused);
void TouchscreenMenu_RequestSelectedIconAnim(SysTask *task);
void TouchscreenMenu_RequestRegisteredItemAnim(SysTask *task, int item);
void TouchscreenMenu_Task(SysTask *task, void *taskData);
void TouchscreenMenu_SetRunningShoesSprites(TouchscreenMenuData *data, int on);
void TouchscreenMenu_HandleRunningShoesButton(TouchscreenMenuData *data, int button);
void TouchscreenMenu_UpdateRunningShoesSprites(TouchscreenMenuData *data);
BOOL TouchscreenMenu_UpdateRunningShoesVisibility(TouchscreenMenuData *data);
void TouchscreenMenu_HandleActionButton(TouchscreenMenuData *data, int heldButton);
int TouchscreenMenu_GetActionType(TouchscreenMenuData *data);
void TouchscreenMenu_PrintActionLabel(TouchscreenMenuData *data, int actionType);
void TouchscreenMenu_UpdateActionLabel(TouchscreenMenuData *data);
void TouchscreenMenu_SetButtonsActive(TouchscreenMenuData *data, BOOL active);
BOOL ov27_0225A714(TouchscreenMenuData *data);
void ov27_0225A7B0(TouchscreenMenuData *data);
void ov27_0225A7DC(TouchscreenMenuData *data);
void ov27_0225A7FC(TouchscreenMenuData *data);
void ov27_0225A86C(TouchscreenMenuData *data);
BOOL TouchscreenMenu_IsFieldBusy(FieldSystem *fieldSystem);
void TouchscreenMenu_SetDimmed(TouchscreenMenuData *data, BOOL dimmed);
void TouchscreenMenu_UpdateRegisteredItemButtons(TouchscreenMenuData *data, BOOL show);
u8 TouchscreenMenu_SumBytes(const u8 *values, int count);
void TouchscreenMenu_ShowIcons(TouchscreenMenuData *data);
void TouchscreenMenu_ShowNewIcons(TouchscreenMenuData *data);
void TouchscreenMenu_LoadBgAndWindows(BgConfig *bgConfig, int variant, Window *actionWindow, Window *menuWindow, Window *iconWindows);
void TouchscreenMenu_LoadSpriteResources(TouchscreenMenuData *data);
u32 TouchscreenMenu_GetBallItem(u32 layout);
void TouchscreenMenu_LoadIconResources(FieldSystem *fieldSystem, GF_2DGfxResMan **resMans, SpriteResource **resObjs, int index, int resId, int gender, Bag *bag, int layout);
void TouchscreenMenu_CreateSprites(TouchscreenMenuData *data);
int TouchscreenMenu_FindNeighborIcon(int selection, int dir, const TouchscreenMenuIcon *icons);
void TouchscreenMenu_HighlightIcon(TouchscreenMenuData *data, int selection);
void TouchscreenMenu_HandleDpadInput(TouchscreenMenuData *data);
void TouchscreenMenu_SetIconsTranslucent(Sprite **sprites, int selection);
BOOL TouchscreenMenu_HandleTouchInput(TouchscreenMenuData *data);
void TouchscreenMenu_DrawWindowFrame(Window *window, u32 mode);
void TouchscreenMenu_PrintFramedText(Window *window, String *string, int colorIndex);
void TouchscreenMenu_LoadStrings(TouchscreenMenuData *data, u32 unused);
void TouchscreenMenu_FreeStrings(TouchscreenMenuData *data);
void TouchscreenMenu_CopyIconLabelsToVram(TouchscreenMenuData *data);
void TouchscreenMenu_PrintIconLabels(TouchscreenMenuData *data);
u32 TouchscreenMenu_IsTextboxOpen(FieldSystem *fieldSystem);
int TouchscreenMenu_GetLayout(FieldSystem *fieldSystem);
BOOL TouchscreenMenu_LayoutHasRegisteredItems(TouchscreenMenuData *data);
BOOL TouchscreenMenu_HasRunningShoes(TouchscreenMenuData *data);
void TouchscreenMenu_InitPhoneRing(TouchscreenMenuPhoneRing *phoneRing, TouchscreenMenuData *data);
void TouchscreenMenu_UpdatePhoneRing(TouchscreenMenuPhoneRing *phoneRing);
void TouchscreenMenu_HidePhoneCaller(TouchscreenMenuPhoneRing *phoneRing);
void TouchscreenMenu_PrintMessageAt(TouchscreenMenuData *data, u32 msgId, u8 x, u8 y, u8 width, u8 height, u32 baseTile, s16 textX, s16 textY);
void TouchscreenMenu_PrintItemCount(TouchscreenMenuData *data, u8 x, u8 y, u16 count);
void TouchscreenMenu_PrintContestMonLevel(TouchscreenMenuData *data, BugContest *contest);
void TouchscreenMenu_PrintBallLabel(TouchscreenMenuData *data);
void TouchscreenMenu_PrintSafariBalls(TouchscreenMenuData *data);
void TouchscreenMenu_PrintParkBalls(TouchscreenMenuData *data);
void TouchscreenMenu_PrintBugContestInfo(TouchscreenMenuData *data);
void TouchscreenMenu_PrintBallInfo(TouchscreenMenuData *data);
void TouchscreenMenu_InitIconFlags(TouchscreenMenuData *data);
int TouchscreenMenu_GetMenuIndex(TouchscreenMenuData *data, int slot);
int TouchscreenMenu_GetSlotFromMenuIndex(TouchscreenMenuData *data, int menuIndex);
void TouchscreenMenu_ValidateSelection(TouchscreenMenuData *data);
void *ov27_0225C238(void);
void ov27_0225C248(void);
void ov27_0225C24C(void);

SysTask *TouchscreenMenu_Create(BgConfig *bgConfig, u16 *lastTouchMenuInput, FieldSystem *fieldSystem, void *args) {
    SysTask *task;
    MessageFormat *msgFormat;
    TouchscreenMenuData *data;
    u32 layout;
    int msgBank;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18D00);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sTouchscreenMenuBgTemplate_Sub0, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sTouchscreenMenuBgTemplate_Sub1, GF_BG_TYPE_TEXT);
    task = CreateSysTaskAndEnvironment(TouchscreenMenu_Task, sizeof(TouchscreenMenuData), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->task = task;
    data->bgVariant = 0;
    data->bgConfig = bgConfig;
    data->lastTouchMenuInput = lastTouchMenuInput;
    data->fieldSystem = fieldSystem;
    layout = TouchscreenMenu_GetLayout(fieldSystem);
    data->layout = layout;
    data->selectedIconAnim = FALSE;
    TouchscreenMenu_LoadBgAndWindows(bgConfig, data->bgVariant, &data->actionWindow, &data->menuWindow, data->iconWindows);
    FontID_Alloc(4, HEAP_ID_8);
    msgFormat = MessageFormat_New(HEAP_ID_8);
    msgBank = NARC_msg_msg_0196_bin;
    data->msgFormat = msgFormat;
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msgBank, HEAP_ID_8);
    TouchscreenMenu_InitIconFlags(data);
    data->selection = TouchscreenMenu_GetSlotFromMenuIndex(data, fieldSystem->unkD3);
    TouchscreenMenu_ValidateSelection(data);
    TouchscreenMenu_LoadSpriteResources(data);
    TouchscreenMenu_CreateSprites(data);
    TouchscreenMenu_LoadStrings(data, data->fieldSystem->unkD2_0);
    AddTextPrinterParameterizedWithColor(&data->actionWindow, 4, data->actionStrings[1], 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    AddTextPrinterParameterizedWithColor(&data->menuWindow, 0, data->menuString, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    TouchscreenMenu_PrintIconLabels(data);
    TouchscreenMenu_CopyIconLabelsToVram(data);
    TouchscreenMenu_SetButtonsActive(data, TRUE);
    TouchscreenMenu_PrintBallInfo(data);
    TouchscreenMenu_InitPhoneRing(&data->phoneRing, data);
    if (MenuInputStateMgr_GetState(&fieldSystem->menuInputState) == MENU_INPUT_STATE_BUTTONS) {
        fieldSystem->unkD2_7 = FALSE;
    } else if (!FieldSystem_TaskIsRunning(fieldSystem)) {
        fieldSystem->unkD2_7 = TRUE;
    }
    if (!ov27_0225A714(data)) {
        return task;
    }
    ov27_0225A7FC(data);
    SpriteList_RenderAndAnimateSprites(data->spriteList);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    return task;
}

void TouchscreenMenu_Destroy(BgConfig *bgConfig, SysTask *task) {
    TouchscreenMenuData *data = SysTask_GetData(task);
    int i;

    FS_LoadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(ds_protect));
    if (DSProt_DetectFlashcart(ov27_0225C238)) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    TouchscreenMenu_HidePhoneCaller(&data->phoneRing);
    DestroyMsgData(data->msgData);
    MessageFormat_Delete(data->msgFormat);
    for (i = 0; i < TOUCHSCREEN_MENU_NUM_RESOURCES; i++) {
        SpriteTransfer_DeleteCharTransferTask(data->resObjs[i][GF_GFX_RES_TYPE_CHAR]);
    }
    for (i = 0; i < TOUCHSCREEN_MENU_NUM_RESOURCES; i++) {
        SpriteTransfer_DeletePlttTransferTask(data->resObjs[i][GF_GFX_RES_TYPE_PLTT]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(data->resMans[i]);
    }
    if (!DSProt_DetectNotEmulator(ov27_0225C248)) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    SpriteList_Delete(data->spriteList);
    for (i = 0; i < 8; i++) {
        RemoveWindow(&data->iconWindows[i]);
    }
    RemoveWindow(&data->menuWindow);
    RemoveWindow(&data->actionWindow);
    TouchscreenMenu_FreeStrings(data);
    FontID_Release(4);
    DestroySysTaskAndEnvironment(task);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    Heap_Destroy(HEAP_ID_8);
    if (!DSProt_DetectNotDummy(ov27_0225C24C)) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    FS_UnloadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(ds_protect));
}

BOOL ov27_0225A2C8(void *unused) {
    return TRUE;
}

// ov01_021F6B50: a start-menu action was launched; the task plays the selected icon's anim.
void TouchscreenMenu_RequestSelectedIconAnim(SysTask *task) {
    TouchscreenMenuData *data = SysTask_GetData(task);
    GF_ASSERT(data != NULL);
    data->selectedIconAnim = TRUE;
}

// ov01_021F6B64: registered item 1 or 2 was used from the field.
void TouchscreenMenu_RequestRegisteredItemAnim(SysTask *task, int item) {
    TouchscreenMenuData *data = SysTask_GetData(task);
    GF_ASSERT(data != NULL);
    GF_ASSERT(item == 1 || item == 2);
    data->registeredItemAnim = item;
}

#pragma force_active on

void TouchscreenMenu_Task(SysTask *task, void *taskData) {
    TouchscreenMenuData *data = taskData;
    BOOL busy = TouchscreenMenu_IsFieldBusy(data->fieldSystem);
    BOOL movementAllowed = FieldSystem_IsPlayerMovementAllowed(data->fieldSystem);
    int heldButton;
    int newButton;

    if (data->fieldSystem->unkD2_7) {
        if (movementAllowed && IsPaletteFadeFinished()) {
            data->fieldSystem->unkD2_7 = FALSE;
        } else {
            G2S_BlendNone();
        }
    } else {
        TouchscreenMenu_SetDimmed(data, busy);
    }
    if (data->selectedIconAnim) {
        Sprite_TryChangeAnimSeq(data->sprites[data->selection], 2);
    }
    if (data->registeredItemAnim == 1) {
        Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_1], 2);
        data->registeredItemAnim = 0;
    } else if (data->registeredItemAnim == 2) {
        Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_2], 2);
        data->registeredItemAnim = 0;
    }
    if (FieldSystem_TaskIsRunning(data->fieldSystem)) {
        ov27_0225A7FC(data);
    } else {
        if (data->fieldSystem->unkD2_0 == 2) {
            ov27_0225A86C(data);
            data->fieldSystem->unkD2_0 = 0;
        }
        if (movementAllowed) {
            TouchscreenMenu_UpdateActionLabel(data);
        }
        if (!busy) {
            TouchscreenMenu_HandleTouchInput(data);
        }
    }
    heldButton = TouchscreenHitbox_FindRectAtTouchHeld(sTouchscreenMenuButtonHitboxes);
    newButton = TouchscreenHitbox_FindRectAtTouchNew(sTouchscreenMenuButtonHitboxes);
    TouchscreenMenu_HandleActionButton(data, heldButton);
    if (data->buttonsActive && TouchscreenMenu_UpdateRunningShoesVisibility(data) && !busy) {
        TouchscreenMenu_HandleRunningShoesButton(data, newButton);
    }
    TouchscreenMenu_UpdatePhoneRing(&data->phoneRing);
    SpriteList_RenderAndAnimateSprites(data->spriteList);
}

#pragma force_active reset

void TouchscreenMenu_SetRunningShoesSprites(TouchscreenMenuData *data, int on) {
    Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL], 11 - on * 4);
    Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], on + 3);
}

void TouchscreenMenu_HandleRunningShoesButton(TouchscreenMenuData *data, int button) {
    PlayerAvatar *avatar;
    int on;

    if (button == 0) {
        avatar = FieldSystem_GetPlayerAvatar(data->fieldSystem);
        on = PlayerAvatar_CheckRunningShoesLock(avatar) ^ 1;
        TouchscreenMenu_SetRunningShoesSprites(data, on);
        // Called with an unnarrowed lock (player_avatar.h declares u16).
        ((void (*)(PlayerAvatar *, u32))PlayerAvatar_SetRunningShoesLock)(avatar, on);
    }
}

void TouchscreenMenu_UpdateRunningShoesSprites(TouchscreenMenuData *data) {
    TouchscreenMenu_SetRunningShoesSprites(data, PlayerAvatar_CheckRunningShoesLock(FieldSystem_GetPlayerAvatar(data->fieldSystem)));
}

BOOL TouchscreenMenu_UpdateRunningShoesVisibility(TouchscreenMenuData *data) {
    BOOL show = TouchscreenMenu_HasRunningShoes(data);

    if (PlayerAvatar_GetState(data->fieldSystem->playerAvatar) == PLAYER_STATE_CYCLING || data->fieldSystem->unkD2_0 != 0) {
        show = FALSE;
    }
    if (show) {
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], TRUE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL], TRUE);
    } else {
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], FALSE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL], FALSE);
    }
    return show;
}

// Touching the action button simulates an A press (and keeps doing so while it stays pressed).
void TouchscreenMenu_HandleActionButton(TouchscreenMenuData *data, int heldButton) {
    int anim = Sprite_GetAnimationNumber(data->sprites[TOUCHSCREEN_MENU_SPRITE_ACTION]);

    if (Sprite_GetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ACTION])) {
        if (heldButton == 1 && System_GetTouchNew()) {
            gSystem.simulatedInputs = PAD_BUTTON_A;
            Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_ACTION], 6);
            return;
        }
        if (heldButton == 1 && anim == 6) {
            gSystem.simulatedInputs = PAD_BUTTON_A;
            return;
        }
        Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_ACTION], 5);
    }
}

// 0/2 CHECK, 1 TALK, 3 FISHING, 4 NEXT (a textbox is open).
int TouchscreenMenu_GetActionType(TouchscreenMenuData *data) {
    FieldSystem *fieldSystem = data->fieldSystem;
    LocalMapObject *playerObj;
    LocalMapObject *facingObj;
    int actionType;

    if (TouchscreenMenu_IsTextboxOpen(fieldSystem)) {
        return 4;
    }
    if (!FieldSystem_IsPlayerMovementAllowed(fieldSystem)) {
        playerObj = PlayerAvatar_GetMapObject(FieldSystem_GetPlayerAvatar(fieldSystem));
        int facing = sub_0205F330(playerObj);
        u32 spriteId = MapObject_GetSpriteID(playerObj);
        if ((u32)(spriteId - SPRITE_FISHINGHERO) <= SPRITE_FISH_HEROINE - SPRITE_FISHINGHERO) {
            return facing == 1 ? 3 : 4;
        }
        if (data->actionType != 4) {
            return data->actionType;
        }
    }
    actionType = ov01_021E7F54(fieldSystem);
    if (actionType == 1) {
        FieldSystem_GetFacingObject(fieldSystem, &facingObj);
        if (ov01_021F6BD0(MapObject_GetScriptID(facingObj)) || ov01_021F6BB0(MapObject_GetSpriteID(facingObj))) {
            actionType = 0;
        }
    }
    return actionType;
}

void TouchscreenMenu_PrintActionLabel(TouchscreenMenuData *data, int actionType) {
    FillWindowPixelBuffer(&data->actionWindow, 0);
    AddTextPrinterParameterizedWithColor(&data->actionWindow, 4, data->actionStrings[sTouchscreenMenuActionTypeToString[actionType]], 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ACTION], TRUE);
}

void TouchscreenMenu_UpdateActionLabel(TouchscreenMenuData *data) {
    int actionType = TouchscreenMenu_GetActionType(data);
    if (data->actionType != actionType) {
        TouchscreenMenu_PrintActionLabel(data, actionType);
        data->actionType = actionType;
    }
}

#pragma force_active on

void TouchscreenMenu_SetButtonsActive(TouchscreenMenuData *data, BOOL active) {
    if (active) {
        data->buttonsActive = TRUE;
        TouchscreenMenu_UpdateRunningShoesVisibility(data);
        TouchscreenMenu_UpdateRegisteredItemButtons(data, TRUE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ACTION], TRUE);
    } else {
        data->buttonsActive = FALSE;
        FillWindowPixelBuffer(&data->actionWindow, 0);
        CopyWindowToVram(&data->actionWindow);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ACTION], FALSE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], FALSE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL], FALSE);
        TouchscreenMenu_UpdateRegisteredItemButtons(data, FALSE);
    }
}

BOOL ov27_0225A714(TouchscreenMenuData *data) {
    FieldSystem *fieldSystem;
    u16 *lastTouchMenuInput;
    int state;

    GF_ASSERT(data->lastTouchMenuInput != NULL);
    fieldSystem = data->fieldSystem;
    lastTouchMenuInput = data->lastTouchMenuInput;
    state = fieldSystem->unkD2_0;
    if (state == 4) {
        if (MenuInputStateMgr_GetState(&fieldSystem->menuInputState) == MENU_INPUT_STATE_BUTTONS) {
            TouchscreenMenu_SetDimmed(data, TRUE);
        } else {
            fieldSystem->unkD2_0 = 0;
        }
    } else if (state == 3) {
        if (MenuInputStateMgr_GetState(&fieldSystem->menuInputState) == MENU_INPUT_STATE_TOUCH) {
            *lastTouchMenuInput = 1;
            fieldSystem->unkD2_0 = 0;
            fieldSystem->unkD2_7 = TRUE;
        } else {
            TouchscreenMenu_HighlightIcon(data, data->selection);
            fieldSystem->unkD2_0 = 1;
        }
    }
    return TRUE;
}

#pragma force_active reset

void ov27_0225A7B0(TouchscreenMenuData *data) {
    if (sub_0203DF8C(data->fieldSystem) == TRUE) {
        TouchscreenMenu_UpdateActionLabel(data);
    }
    if (data->layout == TOUCHSCREEN_MENU_LAYOUT_NORMAL) {
        TouchscreenMenu_ShowNewIcons(data);
    }
}

void ov27_0225A7DC(TouchscreenMenuData *data) {
    GF_ASSERT(data->selection < TOUCHSCREEN_MENU_NUM_ICONS);
    TouchscreenMenu_HighlightIcon(data, data->selection);
    TouchscreenMenu_SetButtonsActive(data, FALSE);
}

#pragma force_active on

void ov27_0225A7FC(TouchscreenMenuData *data) {
    switch (data->fieldSystem->unkD2_0) {
    case 0:
        ov27_0225A7B0(data);
        break;
    case 1:
        ov27_0225A7DC(data);
        data->fieldSystem->unkD2_0 = 2;
        break;
    case 2:
        if (data->fieldSystem->runningFieldMap && IsPaletteFadeFinished() && TouchscreenMenu_HandleTouchInput(data)) {
            TouchscreenMenu_HandleDpadInput(data);
        }
        break;
    case 3:
    case 4:
    default:
        GF_AssertFail();
        break;
    }
}

#pragma force_active reset

void ov27_0225A86C(TouchscreenMenuData *data) {
    TouchscreenMenu_PrintActionLabel(data, TouchscreenMenu_GetActionType(data));
    TouchscreenMenu_HighlightIcon(data, TOUCHSCREEN_MENU_NO_HIGHLIGHT);
    CopyWindowToVram(&data->actionWindow);
    TouchscreenMenu_SetButtonsActive(data, TRUE);
}

BOOL TouchscreenMenu_IsFieldBusy(FieldSystem *fieldSystem) {
    if (!IsPaletteFadeFinished() || !fieldSystem->runningFieldMap || fieldSystem->unkD2_0 != 0 || !FieldSystem_IsPlayerMovementAllowed(fieldSystem) || TouchscreenMenu_IsTextboxOpen(fieldSystem) || MapSceneScriptCheck(fieldSystem) || sub_02058AA0()) {
        return TRUE;
    }
    return FALSE;
}

// While the field is busy the buttons are drawn translucent over a dimmed bottom screen.
void TouchscreenMenu_SetDimmed(TouchscreenMenuData *data, BOOL dimmed) {
    if (dimmed) {
        if (data->layout == TOUCHSCREEN_MENU_LAYOUT_5 && data->fieldSystem->unkD2_0 == 0) {
            TouchscreenMenu_HighlightIcon(data, TOUCHSCREEN_MENU_NO_HIGHLIGHT);
        }
        Sprite_SetOamMode(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_1], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_1_BUTTON], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_2], GX_OAM_MODE_XLU);
        Sprite_SetOamMode(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_2_BUTTON], GX_OAM_MODE_XLU);
        if (data->fieldSystem->unkD2_0 != 0) {
            TouchscreenMenu_SetIconsTranslucent(data->sprites, data->selection);
        } else {
            TouchscreenMenu_SetIconsTranslucent(data->sprites, TOUCHSCREEN_MENU_NO_HIGHLIGHT);
        }
        if (data->layout == TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST) {
            Sprite_SetOamMode(data->sprites[TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON], GX_OAM_MODE_NORMAL);
        }
        G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BD, 6, 9);
    } else {
        G2S_BlendNone();
    }
}

void TouchscreenMenu_UpdateRegisteredItemButtons(TouchscreenMenuData *data, BOOL show) {
    Bag *bag;
    BOOL hasItem1;
    BOOL hasItem2;

    if (!TouchscreenMenu_LayoutHasRegisteredItems(data)) {
        show = FALSE;
    }
    if (show) {
        bag = Save_Bag_Get(data->fieldSystem->saveData);
        hasItem1 = Bag_GetRegisteredItem1(bag) != ITEM_NONE;
        hasItem2 = Bag_GetRegisteredItem2(bag) != ITEM_NONE;
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_1], hasItem1);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_1_BUTTON], hasItem1);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_2], hasItem2);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_2_BUTTON], hasItem2);
    } else {
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_1], FALSE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_1_BUTTON], FALSE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_2], FALSE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_2_BUTTON], FALSE);
    }
}

u8 TouchscreenMenu_SumBytes(const u8 *values, int count) {
    int i;
    int sum = 0;
    for (i = 0; i < count; i++) {
        sum += values[i];
    }
    return sum;
}

void TouchscreenMenu_ShowIcons(TouchscreenMenuData *data) {
    int i;

    for (i = 0; i < TOUCHSCREEN_MENU_NUM_ICONS; i++) {
        Sprite_SetDrawFlag(data->sprites[i], data->iconShown[i]);
    }
    data->runningShoesShown = TRUE;
    if (!TouchscreenMenu_HasRunningShoes(data)) {
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], FALSE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL], FALSE);
        data->runningShoesShown = FALSE;
    }
}

// Icons unlocked while the menu is shown appear with an animation; the selection is re-validated.
void TouchscreenMenu_ShowNewIcons(TouchscreenMenuData *data) {
    int i;
    int count;
    int oldCount;
    const TouchscreenMenuIconAnim *iconAnim;

    oldCount = TouchscreenMenu_SumBytes(data->iconShown, TOUCHSCREEN_MENU_NUM_ICONS);
    count = oldCount;
    iconAnim = sTouchscreenMenuIconAnims;
    i = 0;
    while (i < TOUCHSCREEN_MENU_NUM_ICONS) {
        if (FieldSystem_ShouldDrawStartMenuIcon(data->fieldSystem, (StartMenuIcon)i) == TRUE && data->iconShown[i] == FALSE) {
            Sprite_SetDrawFlag(data->sprites[iconAnim->sprite], TRUE);
            Sprite_SetAnimCtrlSeq(data->sprites[iconAnim->sprite], iconAnim->anim);
            CopyWindowToVram(&data->iconWindows[iconAnim->window]);
            data->iconShown[i] = TRUE;
            data->icons[i].shown = TRUE;
            data->fieldSystem->unkD2_7 = TRUE;
            count++;
        }
        iconAnim++;
        i++;
    }
    if (count != 0 && oldCount == 0) {
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_MENU], TRUE);
        CopyWindowToVram(&data->menuWindow);
    }
    if (oldCount != count) {
        data->fieldSystem->unkD3 = 0;
        data->selection = TouchscreenMenu_GetSlotFromMenuIndex(data, data->fieldSystem->unkD3);
        TouchscreenMenu_ValidateSelection(data);
    }
    if (TouchscreenMenu_HasRunningShoes(data) == TRUE && data->runningShoesShown == FALSE) {
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], TRUE);
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL], TRUE);
        Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], 10);
        data->runningShoesShown = TRUE;
    }
}

void TouchscreenMenu_LoadBgAndWindows(BgConfig *bgConfig, int variant, Window *actionWindow, Window *menuWindow, Window *iconWindows) {
    int i;
    int baseTile;
    const TouchscreenMenuWindowPos *pos;
    Window *window;

    GfGfxLoader_LoadCharData(NARC_a_0_1_4, sTouchscreenMenuBgChars[variant].fileId, bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_4, sTouchscreenMenuBgScreens[variant].fileId, bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_8);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_4, sTouchscreenMenuBgPalettes[variant].fileId, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200, HEAP_ID_8);
    AddWindowParameterized(bgConfig, actionWindow, GF_BG_LYR_SUB_1, 24, 20, 8, 2, 4, 0xD2);
    FillWindowPixelBuffer(actionWindow, 0);
    AddWindowParameterized(bgConfig, menuWindow, GF_BG_LYR_SUB_1, 9, 0, 10, 2, 4, 0xE2);
    FillWindowPixelBuffer(menuWindow, 0);
    pos = sTouchscreenMenuIconWindowPositions;
    window = iconWindows;
    for (i = 0, baseTile = 0xF6; i < 8; i++, baseTile += 0x12, pos++, window++) {
        AddWindowParameterized(bgConfig, window, GF_BG_LYR_SUB_1, pos->x, pos->y, 9, 2, 4, baseTile);
        FillWindowPixelBuffer(window, 0);
    }
}

void TouchscreenMenu_LoadSpriteResources(TouchscreenMenuData *data) {
    int i;
    void *nclr;
    NNSG2dPaletteData *paletteData;

    data->spriteList = G2dRenderer_Init(16, &data->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&data->renderer, 0, FX32_CONST(256));
    for (i = 0; i < 4; i++) {
        data->resMans[i] = Create2DGfxResObjMan(TOUCHSCREEN_MENU_NUM_RESOURCES, (GfGfxResType)i, HEAP_ID_8);
    }
    for (i = 0; i < TOUCHSCREEN_MENU_NUM_RESOURCES; i++) {
        int gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(data->fieldSystem->saveData));
        Bag *bag = Save_Bag_Get(data->fieldSystem->saveData);
        TouchscreenMenu_LoadIconResources(data->fieldSystem, data->resMans, data->resObjs[i], i, i + 100, gender, bag, data->layout);
    }
    data->resObjs[0][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_0_1_4, 16, TRUE, 100, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    data->resObjs[0][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_0_1_4, 17, TRUE, 100, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
    data->resObjs[1][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_0_1_4, 68, TRUE, 101, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    data->resObjs[1][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_0_1_4, 69, TRUE, 101, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
    data->resObjs[2][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_0_1_4, 54, TRUE, 102, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    data->resObjs[2][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_0_1_4, 55, TRUE, 102, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
    nclr = GfGfxLoader_GetPlttData(NARC_a_0_1_4, 14, &paletteData, HEAP_ID_8);
    DC_FlushRange(paletteData->pRawData, sizeof(data->iconPalettes));
    MIi_CpuCopyFast(paletteData->pRawData, (u32 *)data->iconPalettes, sizeof(data->iconPalettes));
    Heap_Free(nclr);
}

u32 TouchscreenMenu_GetBallItem(u32 layout) {
    if (layout == TOUCHSCREEN_MENU_LAYOUT_SAFARI) {
        return ITEM_SAFARI_BALL;
    }
    if (layout == TOUCHSCREEN_MENU_LAYOUT_PAL_PARK) {
        return ITEM_PARK_BALL;
    }
    return ITEM_SPORT_BALL;
}

// Resource ids 100 + index: 0-6 icons of the layout, 7/8 registered item icons, 9 the buttons, 10 the ball item.
void TouchscreenMenu_LoadIconResources(FieldSystem *fieldSystem, GF_2DGfxResMan **resMans, SpriteResource **resObjs, int index, int resId, int gender, Bag *bag, int layout) {
    u8 icon = sTouchscreenMenuLayouts[layout][index];
    NarcId narcId = NARC_a_0_1_4;
    int charFileId = 18;
    int plttFileId = 14;
    int compressed = TRUE;
    int plttNum = 1;

    if (icon != TOUCHSCREEN_MENU_ICON_NONE) {
        if (icon == 2 && gender == PLAYER_GENDER_FEMALE) {
            charFileId = 27;
        } else {
            charFileId = sTouchscreenMenuIcons[icon].charFileId;
            if (charFileId == 0xFFFF) {
                BugContest *contest = FieldSystem_BugContest_Get(fieldSystem);
                GF_ASSERT(contest != NULL);
                if (contest->caught_poke) {
                    compressed = FALSE;
                    narcId = NARC_poketool_icongra_poke_icon;
                    plttNum = 3;
                    plttFileId = sub_02074490();
                    charFileId = Pokemon_GetIconNaix(contest->mon);
                } else {
                    charFileId = 18;
                }
            }
        }
    }
    if (index == 7) {
        compressed = FALSE;
        narcId = NARC_itemtool_itemdata_item_icon;
        plttFileId = GetItemIndexMapping(Bag_GetRegisteredItem1(bag), ITEMNARC_NCLR);
        charFileId = GetItemIndexMapping(Bag_GetRegisteredItem1(bag), ITEMNARC_NCGR);
    } else if (index == 8) {
        compressed = FALSE;
        narcId = NARC_itemtool_itemdata_item_icon;
        plttFileId = GetItemIndexMapping(Bag_GetRegisteredItem2(bag), ITEMNARC_NCLR);
        charFileId = GetItemIndexMapping(Bag_GetRegisteredItem2(bag), ITEMNARC_NCGR);
    } else if (index == 9) {
        plttFileId = 7;
        charFileId = 70;
        plttNum = 4;
    } else if (index == 10) {
        u32 item = TouchscreenMenu_GetBallItem(layout);
        compressed = FALSE;
        narcId = NARC_itemtool_itemdata_item_icon;
        plttFileId = GetItemIndexMapping(item, ITEMNARC_NCLR);
        charFileId = GetItemIndexMapping(item, ITEMNARC_NCGR);
    }
    resObjs[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(resMans[GF_GFX_RES_TYPE_CHAR], narcId, charFileId, compressed, resId, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_8);
    resObjs[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(resMans[GF_GFX_RES_TYPE_PLTT], narcId, plttFileId, FALSE, resId, NNS_G2D_VRAM_TYPE_2DSUB, plttNum, HEAP_ID_8);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resObjs[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(resObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(resObjs[GF_GFX_RES_TYPE_PLTT]);
    sub_0200A740(resObjs[GF_GFX_RES_TYPE_PLTT]);
}

// Pixel coordinate to fx32, rounded away from zero (the float conversion is what the code does).
static inline s32 TouchscreenMenu_RoundToFx32(int x) {
    float f;
    if (x > 0) {
        f = 0.5f + (float)(x << FX32_SHIFT);
    } else {
        f = (float)(x << FX32_SHIFT) - 0.5f;
    }
    return (s32)f;
}

static inline s32 TouchscreenMenu_PosToFx32(u16 x) {
    float f;
    if (x != 0) {
        f = 0.5f + (float)(x << FX32_SHIFT);
    } else {
        f = (float)(x << FX32_SHIFT) - 0.5f;
    }
    return (s32)f;
}

static inline s32 TouchscreenMenu_PosToFx32Half(u16 x, float half) {
    float f;
    if (x != 0) {
        f = half + (float)(x << FX32_SHIFT);
    } else {
        f = (float)(x << FX32_SHIFT) - half;
    }
    return (s32)f;
}

#pragma force_active on

void TouchscreenMenu_CreateSprites(TouchscreenMenuData *data) {
    SpriteTemplate template;
    SpriteResourcesHeader *header;
    int i;
    int y;
    int ballX = 100; // a variable, so the float rounding stays a runtime conversion
    BugContest *contest;
    const TouchscreenMenuPos *pos;

    for (i = 0; i < TOUCHSCREEN_MENU_NUM_RESOURCES; i++) {
        CreateSpriteResourcesHeader(&data->headers[i], i + 100, i + 100, sTouchscreenMenuCellIds[i], sTouchscreenMenuCellIds[i], -1, -1, FALSE, 0, data->resMans[GF_GFX_RES_TYPE_CHAR], data->resMans[GF_GFX_RES_TYPE_PLTT], data->resMans[GF_GFX_RES_TYPE_CELL], data->resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    }
    template.spriteList = data->spriteList;
    template.header = data->headers;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 1;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_8;
    for (i = 0; i < 9; i++) {
        template.header = &data->headers[i];
        template.position.x = TouchscreenMenu_PosToFx32(sTouchscreenMenuIconPositions[i].x);
        template.position.y = TouchscreenMenu_PosToFx32(sTouchscreenMenuIconPositions[i].y) + FX32_CONST(256);
        data->sprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(data->sprites[i], TRUE);
        Sprite_SetPriority(data->sprites[i], 0);
        Sprite_SetAffineOverwriteMode(data->sprites[i], NNS_G2D_RND_AFFINE_OVERWRITE_NORMAL);
    }
    {
        // Sprites 9-14 share the button resources. The loop steps a TouchscreenMenuData pointer by one
        // sprite slot (and passes the rounding constant in a variable), which codegen needs.
        TouchscreenMenuData *slot;
        int j;
        float half;
        for (j = 9, slot = (TouchscreenMenuData *)((u8 *)data + 9 * sizeof(Sprite *)), pos = sTouchscreenMenuButtonPositions, half = 0.5f, header = &data->headers[9]; j < 15; pos++, slot = (TouchscreenMenuData *)((u8 *)slot + sizeof(Sprite *)), j++) {
            template.header = header;
            template.position.x = TouchscreenMenu_PosToFx32Half(pos->x, half);
            template.position.y = TouchscreenMenu_PosToFx32Half(pos->y, half) + FX32_CONST(256);
            slot->sprites[0] = Sprite_CreateAffine(&template);
            Sprite_SetAnimActiveFlag(slot->sprites[0], TRUE);
            Sprite_SetPriority(slot->sprites[0], 2);
        }
    }
    if ((u32)(data->layout - TOUCHSCREEN_MENU_LAYOUT_SAFARI) <= TOUCHSCREEN_MENU_LAYOUT_PAL_PARK - TOUCHSCREEN_MENU_LAYOUT_SAFARI) {
        if (data->layout == TOUCHSCREEN_MENU_LAYOUT_SAFARI) {
            y = 144;
        } else if ((u32)(data->layout - TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST) <= TOUCHSCREEN_MENU_LAYOUT_PAL_PARK - TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST) {
            y = 104;
        }
        template.header = &data->headers[10];
        template.position.x = (s32)(0.5f + (float)(ballX << FX32_SHIFT));
        template.position.y = TouchscreenMenu_RoundToFx32(y) + FX32_CONST(256);
        data->sprites[TOUCHSCREEN_MENU_SPRITE_BALL] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_BALL], TRUE);
        Sprite_SetPriority(data->sprites[TOUCHSCREEN_MENU_SPRITE_BALL], 0);
        Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_BALL], 6);
    }
    if (data->layout == TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST) {
        contest = FieldSystem_BugContest_Get(data->fieldSystem);
        if (contest == NULL) {
            GF_AssertFail();
        }
        if (contest->caught_poke) {
            Sprite_SetPositionXYWithSubscreenOffset(data->sprites[TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON], 104, 136, FX32_CONST(256));
            Sprite_SetPalOffsetRespectVramOffset(data->sprites[TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON], Pokemon_GetIconPalette(contest->mon));
            Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON], 6);
        }
    } else if (data->layout == TOUCHSCREEN_MENU_LAYOUT_PAL_PARK) {
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON], FALSE);
    }
    Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_1_BUTTON], 0);
    Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_ITEM_2_BUTTON], 8);
    Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES], 3);
    Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_ACTION], 5);
    Sprite_SetAnimCtrlSeq(data->sprites[TOUCHSCREEN_MENU_SPRITE_MENU], 12);
    Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_MENU], FALSE);
    Sprite_SetPriority(data->sprites[TOUCHSCREEN_MENU_SPRITE_SHOES_LABEL], 1);
    TouchscreenMenu_UpdateRunningShoesSprites(data);
    TouchscreenMenu_UpdateRegisteredItemButtons(data, TRUE);
    TouchscreenMenu_ShowIcons(data);
}

#pragma force_active reset

int TouchscreenMenu_FindNeighborIcon(int selection, int dir, const TouchscreenMenuIcon *icons) {
    int i;
    int ret = -1;

    for (i = 0; i < 3; i++) {
        u8 icon = sTouchscreenMenuIconNeighbors[selection][dir][i];
        if (icons[icon].shown != FALSE) {
            ret = icon;
            break;
        }
    }
    return ret;
}

void TouchscreenMenu_HighlightIcon(TouchscreenMenuData *data, int selection) {
    GF_ASSERT(selection < TOUCHSCREEN_MENU_NUM_ICONS);
    int i = 0;
    int offset = i;

    while (i < TOUCHSCREEN_MENU_NUM_ICONS) {
        u8 icon = sTouchscreenMenuLayouts[data->layout][i];
        u16 charFileId = sTouchscreenMenuIcons[icon].charFileId;
        int palette = 0;
        if (icon != TOUCHSCREEN_MENU_ICON_NONE && charFileId != 0xFFFF) {
            if (i == selection) {
                palette = 16;
            }
            GXS_LoadOBJPltt(&data->iconPalettes[palette], offset, 0x20);
        }
        offset += 0x20;
        i++;
    }
}

#pragma force_active on

void TouchscreenMenu_HandleDpadInput(TouchscreenMenuData *data) {
    int dir = -1;
    int selection = data->selection;

    if (gSystem.newKeys & PAD_KEY_UP) {
        dir = 0;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        dir = 1;
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        dir = 2;
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        dir = 3;
    }
    if (dir >= 0) {
        selection = TouchscreenMenu_FindNeighborIcon(selection, dir, data->icons);
    }
    if (selection != -1 && data->selection != selection) {
        data->selection = selection;
        PlaySE(SEQ_SE_DP_SELECT78);
        if ((u32)dir <= 1) {
            Sprite_SetAnimCtrlSeq(data->sprites[data->selection], 3);
        } else if ((u32)(dir - 2) <= 1) {
            Sprite_SetAnimCtrlSeq(data->sprites[data->selection], 1);
        }
        TouchscreenMenu_HighlightIcon(data, data->selection);
        data->fieldSystem->unkD3 = TouchscreenMenu_GetMenuIndex(data, data->selection);
    }
}

#pragma force_active reset

void TouchscreenMenu_SetIconsTranslucent(Sprite **sprites, int selection) {
    int i;
    for (i = 0; i < 9; i++) {
        if (sprites[i] != NULL) {
            if (selection != i) {
                Sprite_SetOamMode(sprites[i], GX_OAM_MODE_XLU);
            } else {
                Sprite_SetOamMode(sprites[i], GX_OAM_MODE_NORMAL);
            }
        }
    }
}

// Returns FALSE when a touch was handled (or hit a hidden icon), TRUE when input is still free.
BOOL TouchscreenMenu_HandleTouchInput(TouchscreenMenuData *data) {
    int hit;
    Bag *bag;

    if (PlayerAvatar_GetPlayerMoveState(data->fieldSystem->playerAvatar) != PLAYER_MOVE_STATE_NONE || (gSystem.newKeys & PAD_PLUS_KEY_MASK) != 0) {
        return TRUE;
    }
    if (data->layout == TOUCHSCREEN_MENU_LAYOUT_5) {
        u32 netId = Comm_GetLocalNetId();
        if (!sub_02058740() || !sub_02058258() || !sub_02056EE0() || !sub_02057A0C() || sub_02057F18(netId) || sub_02037958()) {
            return TRUE;
        }
    }
    if (!IsPaletteFadeFinished()) {
        return TRUE;
    }
    if (data->selectedIconAnim == TRUE) {
        return TRUE;
    }
    hit = TouchscreenHitbox_FindRectAtTouchNew(sTouchscreenMenuIconHitboxes);
    if (hit > 0 && hit < 8) {
        if (data->icons[hit - 1].shown == FALSE) {
            return FALSE;
        }
    }
    bag = Save_Bag_Get(data->fieldSystem->saveData);
    if ((u32)(hit - 8) <= 1) {
        if (!data->buttonsActive) {
            return TRUE;
        }
        if (!TouchscreenMenu_LayoutHasRegisteredItems(data)) {
            return TRUE;
        }
    }
    if (hit == 8) {
        if (Bag_GetRegisteredItem1(bag) == ITEM_NONE) {
            return TRUE;
        }
    } else if (hit == 9 && Bag_GetRegisteredItem2(bag) == ITEM_NONE) {
        return TRUE;
    }
    if (hit != -1) {
        if (hit == 0) {
            *data->lastTouchMenuInput = 1;
        } else if (data->sprites[hit - 1] != NULL) {
            if (hit < 8) {
                data->selection = hit - 1;
                data->fieldSystem->unkD3 = TouchscreenMenu_GetMenuIndex(data, data->selection);
                TouchscreenMenu_HighlightIcon(data, data->selection);
            }
            *data->lastTouchMenuInput = TouchscreenMenu_GetMenuIndex(data, hit - 1) + 2;
        }
        return FALSE;
    }
    return TRUE;
}

#define TouchscreenMenu_FillFrameRow(bgId, tile, x, y, width, palette) FillBgTilemapRect(bgConfig, bgId, tile, x, y, width, 1, palette)

// Mode 1 draws, mode 2 erases a tile frame around the window (BG sub 0 left part, BG sub 1 the rest).
void TouchscreenMenu_DrawWindowFrame(Window *window, u32 mode) {
    BgConfig *bgConfig = window->bgConfig;
    u32 x = GetWindowX(window);
    u32 y = GetWindowY(window);
    u32 width = GetWindowWidth(window);
    u32 height = GetWindowHeight(window);

    switch (mode) {
    case 1:
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0xA9, x - 3, y - 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0xAA, x - 2, y - 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0xAB, x - 1, y - 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0xAC, x, y - 1, width, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0xAD, x + width, y - 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0x64, x - 3, y, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0x65, x - 2, y, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0x66, x - 1, y, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0x68, x + width, y, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0x84, x - 3, y + 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0x85, x - 2, y + 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0x86, x - 1, y + 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0x88, x + width, y + 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0xA4, x - 3, y + height, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0xA5, x - 2, y + height, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 0xA6, x - 1, y + height, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0xA7, x, y + height, width, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0xA8, x + width, y + height, 1, 2);
        break;
    case 2:
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 3, y - 1, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 2, y - 1, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 1, y - 1, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0, x, y - 1, width, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0, x + width, y - 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 3, y, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 2, y, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 1, y, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0, x + width, y, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 3, y + 1, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 2, y + 1, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 1, y + 1, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0, x + width, y + 1, 1, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 3, y + height, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 2, y + height, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_0, 7, x - 1, y + height, 1, 0);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0, x, y + height, width, 2);
        TouchscreenMenu_FillFrameRow(GF_BG_LYR_SUB_1, 0, x + width, y + height, 1, 2);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_0);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_1);
}

#undef TouchscreenMenu_FillFrameRow

void TouchscreenMenu_PrintFramedText(Window *window, String *string, int colorIndex) {
    u32 color = sTouchscreenMenuTextColors[colorIndex];
    FillWindowPixelBuffer(window, color);
    AddTextPrinterParameterizedWithColor(window, 0, string, 3, 0, TEXT_SPEED_INSTANT, color, NULL);
}

#pragma force_active on

void TouchscreenMenu_LoadStrings(TouchscreenMenuData *data, u32 unused) {
    int i;
    int j;

    BufferPlayersName(data->msgFormat, 0, Save_PlayerData_GetProfile(data->fieldSystem->saveData));
    for (i = 0; i < TOUCHSCREEN_MENU_NUM_ICONS; i++) {
        u8 icon = sTouchscreenMenuLayouts[data->layout][i];
        if (icon != TOUCHSCREEN_MENU_ICON_NONE) {
            data->icons[i].label = ReadMsgData_ExpandPlaceholders(data->msgFormat, data->msgData, sTouchscreenMenuIcons[icon].msgId, HEAP_ID_8);
        }
    }
    for (j = 0; j < 5; j++) {
        data->actionStrings[j] = NewString_ReadMsgData(data->msgData, sTouchscreenMenuActionMsgIds[j]);
    }
    data->menuString = ReadMsgData_ExpandPlaceholders(data->msgFormat, data->msgData, msg_0196_00012, HEAP_ID_8);
    data->unk_4C8 = ReadMsgData_ExpandPlaceholders(data->msgFormat, data->msgData, msg_0196_00013, HEAP_ID_8);
}

#pragma force_active reset

void TouchscreenMenu_FreeStrings(TouchscreenMenuData *data) {
    int i;

    String_Delete(data->unk_4C8);
    String_Delete(data->menuString);
    for (i = 0; i < 5; i++) {
        String_Delete(data->actionStrings[i]);
    }
    for (i = 0; i < TOUCHSCREEN_MENU_NUM_ICONS; i++) {
        if (data->icons[i].label != NULL) {
            String_Delete(data->icons[i].label);
        }
    }
}

#pragma force_active on

void TouchscreenMenu_CopyIconLabelsToVram(TouchscreenMenuData *data) {
    BOOL draw[8];
    int i;
    int count = 0;

    for (i = 0; i < 8; i++) {
        draw[i] = FieldSystem_ShouldDrawStartMenuIcon(data->fieldSystem, (StartMenuIcon)i);
    }
    for (i = 0; i < 8; i++) {
        if (draw[i]) {
            CopyWindowToVram(&data->iconWindows[i]);
            count++;
        }
    }
    if (count) {
        Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_MENU], TRUE);
        CopyWindowToVram(&data->menuWindow);
    }
}

#pragma force_active reset

void TouchscreenMenu_PrintIconLabels(TouchscreenMenuData *data) {
    int i = 0;
    Window *window = data->iconWindows;

    while (i < TOUCHSCREEN_MENU_NUM_ICONS) {
        if (data->icons[i].label != NULL) {
            int margin = 0x48 - (int)FontID_String_GetWidth(0, data->icons[i].label, 0);
            AddTextPrinterParameterizedWithColor(window, 0, data->icons[i].label, margin / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 2, 0), NULL);
        }
        i++;
        window++;
    }
}

u32 TouchscreenMenu_IsTextboxOpen(FieldSystem *fieldSystem) {
    return fieldSystem->textbox_open;
}

#pragma force_active on

int TouchscreenMenu_GetLayout(FieldSystem *fieldSystem) {
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    if (FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(fieldSystem)) {
        return TOUCHSCREEN_MENU_LAYOUT_BATTLE_TOWER_PARTNER_ROOM;
    }
    if (Save_VarsFlags_CheckSafariSysFlag(varsFlags)) {
        return TOUCHSCREEN_MENU_LAYOUT_SAFARI;
    }
    if (Save_VarsFlags_CheckBugContestFlag(varsFlags)) {
        return TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST;
    }
    if (Save_VarsFlags_CheckPalParkSysFlag(varsFlags)) {
        return TOUCHSCREEN_MENU_LAYOUT_PAL_PARK;
    }
    if (fieldSystem->bottomScreenType == 3) {
        return TOUCHSCREEN_MENU_LAYOUT_4;
    }
    if (fieldSystem->mapLoadType == 3) {
        return TOUCHSCREEN_MENU_LAYOUT_5;
    }
    return TOUCHSCREEN_MENU_LAYOUT_NORMAL;
}

#pragma force_active reset

BOOL TouchscreenMenu_LayoutHasRegisteredItems(TouchscreenMenuData *data) {
    u32 layout = data->layout;
    if (layout - TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST > TOUCHSCREEN_MENU_LAYOUT_BATTLE_TOWER_PARTNER_ROOM - TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST) {
        return TRUE;
    }
    return FALSE;
}

BOOL TouchscreenMenu_HasRunningShoes(TouchscreenMenuData *data) {
    return PlayerSaveData_CheckRunningShoes(LocalFieldData_GetPlayer(Save_LocalFieldData_Get(data->fieldSystem->saveData)));
}

void TouchscreenMenu_InitPhoneRing(TouchscreenMenuPhoneRing *phoneRing, TouchscreenMenuData *data) {
    phoneRing->phoneRingManager = FieldSystem_GetGearPhoneRingManager(data->fieldSystem);
    phoneRing->pokegearSprite = data->sprites[3];
    phoneRing->bgConfig = data->bgConfig;
    phoneRing->shown = FALSE;
}

#pragma force_active on

void TouchscreenMenu_UpdatePhoneRing(TouchscreenMenuPhoneRing *phoneRing) {
    BOOL ringing = GearPhoneRingManager_IsRinging(phoneRing->phoneRingManager);

    if (phoneRing->shown == FALSE) {
        if (ringing == TRUE && phoneRing->shown == FALSE) {
            String *name = GetPhoneBookEntryName(phoneRing->phoneRingManager, HEAP_ID_8);
            int width = (int)FontID_String_GetWidth(0, name, 0) / 8 + 2;
            if (width > 9) {
                width = 9;
            }
            AddWindowParameterized(phoneRing->bgConfig, &phoneRing->callerWindow, GF_BG_LYR_SUB_1, 11, 19, width, 2, 4, 192);
            TouchscreenMenu_DrawWindowFrame(&phoneRing->callerWindow, 1);
            TouchscreenMenu_PrintFramedText(&phoneRing->callerWindow, name, 1);
            String_Delete(name);
            Sprite_TryChangeAnimSeq(phoneRing->pokegearSprite, 5);
            phoneRing->shown = TRUE;
        }
    } else if (ringing) {
        Sprite_TryChangeAnimSeq(phoneRing->pokegearSprite, 5);
    } else {
        Sprite_TryChangeAnimSeq(phoneRing->pokegearSprite, 0);
        TouchscreenMenu_HidePhoneCaller(phoneRing);
        phoneRing->shown = FALSE;
    }
}

#pragma force_active reset

void TouchscreenMenu_HidePhoneCaller(TouchscreenMenuPhoneRing *phoneRing) {
    if (phoneRing->shown != FALSE) {
        TouchscreenMenu_DrawWindowFrame(&phoneRing->callerWindow, 2);
        ClearWindowTilemapAndCopyToVram(&phoneRing->callerWindow);
        RemoveWindow(&phoneRing->callerWindow);
    }
}

#pragma force_active on

// Prints a message into a temporary window on BG sub 1; textX == -1 centres it.
void TouchscreenMenu_PrintMessageAt(TouchscreenMenuData *data, u32 msgId, u8 x, u8 y, u8 width, u8 height, u32 baseTile, s16 textX, s16 textY) {
    Window window;
    String *string;

    AddWindowParameterized(data->bgConfig, &window, GF_BG_LYR_SUB_1, x, y, width, height, 4, baseTile);
    FillWindowPixelBuffer(&window, 0);
    string = ReadMsgData_ExpandPlaceholders(data->msgFormat, data->msgData, msgId, HEAP_ID_8);
    if (textX == -1) {
        textX = (width * 8 - FontID_String_GetWidth(0, string, 0));
        textX /= 2;
    }
    AddTextPrinterParameterizedWithColor(&window, 0, string, textX, textY, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 2, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(&window);
    RemoveWindow(&window);
}

#pragma force_active reset

void TouchscreenMenu_PrintItemCount(TouchscreenMenuData *data, u8 x, u8 y, u16 count) {
    BufferIntegerAsString(data->msgFormat, 0, count, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    TouchscreenMenu_PrintMessageAt(data, msg_0196_00031, x, y, 6, 2, 0x186, 0, 0);
}

void TouchscreenMenu_PrintContestMonLevel(TouchscreenMenuData *data, BugContest *contest) {
    u32 level = GetMonData(contest->mon, MON_DATA_LEVEL, NULL);

    BufferIntegerAsString(data->msgFormat, 0, level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    TouchscreenMenu_PrintMessageAt(data, msg_0196_00033, 12, 21, 6, 2, 0x192, 4, 0);
}

void TouchscreenMenu_PrintBallLabel(TouchscreenMenuData *data) {
    TouchscreenMenu_PrintMessageAt(data, msg_0196_00032, 11, 21, 9, 2, 372, -1, 0);
}

void TouchscreenMenu_PrintSafariBalls(TouchscreenMenuData *data) {
    TouchscreenMenu_PrintItemCount(data, 14, 19, *LocalFieldData_GetSafariBallsCounter(Save_LocalFieldData_Get(data->fieldSystem->saveData)));
    TouchscreenMenu_PrintBallLabel(data);
}

void TouchscreenMenu_PrintParkBalls(TouchscreenMenuData *data) {
    TouchscreenMenu_PrintItemCount(data, 14, 14, FieldSystem_GetParkBallCount(data->fieldSystem));
}

#pragma force_active on

void TouchscreenMenu_PrintBugContestInfo(TouchscreenMenuData *data) {
    BugContest *contest = FieldSystem_BugContest_Get(data->fieldSystem);
    if (contest == NULL) {
        GF_AssertFail();
    }
    TouchscreenMenu_PrintItemCount(data, 14, 14, *BugContest_GetSportBallsAddr(contest));
    if (contest->caught_poke) {
        TouchscreenMenu_PrintContestMonLevel(data, contest);
    }
    Sprite_SetDrawFlag(data->sprites[TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON], contest->caught_poke);
    data->iconShown[TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON] = FALSE;
    data->icons[TOUCHSCREEN_MENU_SPRITE_BUG_CONTEST_MON].shown = FALSE;
}

#pragma force_active reset

void TouchscreenMenu_PrintBallInfo(TouchscreenMenuData *data) {
    u32 layout = data->layout;
    if (layout == TOUCHSCREEN_MENU_LAYOUT_SAFARI) {
        TouchscreenMenu_PrintSafariBalls(data);
        return;
    }
    if (layout == TOUCHSCREEN_MENU_LAYOUT_BUG_CONTEST) {
        TouchscreenMenu_PrintBugContestInfo(data);
        return;
    }
    if (layout == TOUCHSCREEN_MENU_LAYOUT_PAL_PARK) {
        TouchscreenMenu_PrintParkBalls(data);
    }
}

#pragma force_active on

void TouchscreenMenu_InitIconFlags(TouchscreenMenuData *data) {
    int i;
    for (i = 0; i < TOUCHSCREEN_MENU_NUM_ICONS; i++) {
        int icon = sTouchscreenMenuLayouts[data->layout][i];
        if (icon != TOUCHSCREEN_MENU_ICON_NONE) {
            if (icon < TOUCHSCREEN_MENU_NUM_ICONS) {
                data->iconShown[i] = FieldSystem_ShouldDrawStartMenuIcon(data->fieldSystem, (StartMenuIcon)icon);
            } else {
                data->iconShown[i] = sTouchscreenMenuIcons[icon].enabled;
            }
        }
        data->icons[i].shown = data->iconShown[i];
    }
}

#pragma force_active reset

// Slots 7/8 (registered items) pass through; icon slots map to their index among the shown icons.
int TouchscreenMenu_GetMenuIndex(TouchscreenMenuData *data, int slot) {
    int menuIndex = -1;
    int i;

    if ((u32)(slot - 7) <= 1) {
        return slot;
    }
    for (i = 0; i < slot + 1; i++) {
        if (data->icons[i].shown != FALSE) {
            menuIndex++;
        }
    }
    if (menuIndex == -1) {
        GF_AssertFail();
    }
    return menuIndex;
}

#pragma force_active on

int TouchscreenMenu_GetSlotFromMenuIndex(TouchscreenMenuData *data, int menuIndex) {
    int i;
    int slot;
    int count;

    slot = -1;
    count = 0;
    for (i = count; i < TOUCHSCREEN_MENU_NUM_ICONS; i++) {
        if (data->icons[i].shown) {
            if (menuIndex == count) {
                slot = i;
                break;
            }
            count++;
        }
    }
    if (slot == -1) {
        data->fieldSystem->unkD3 = 0;
        slot = 0;
    }
    return slot;
}

void TouchscreenMenu_ValidateSelection(TouchscreenMenuData *data) {
    int i;

    GF_ASSERT(data->fieldSystem);
    if (!data->icons[data->selection].shown) {
        for (i = 0; i < TOUCHSCREEN_MENU_NUM_ICONS; i++) {
            if (data->icons[i].shown) {
                data->fieldSystem->unkD3 = i;
                data->selection = TouchscreenMenu_GetSlotFromMenuIndex(data, data->fieldSystem->unkD3);
                return;
            }
        }
    }
}

#pragma force_active reset

void *ov27_0225C238(void) {
    return Heap_AllocAtEnd(HEAP_ID_3, 1000);
}

void ov27_0225C248(void) {
}

void ov27_0225C24C(void) {
}
